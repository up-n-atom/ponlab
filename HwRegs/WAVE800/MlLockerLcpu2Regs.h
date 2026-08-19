
/***********************************************************************************
File:				MlLockerLcpu2Regs.h
Module:				mlLockerLcpu2
SOC Revision:		latest
Generated at:       2024-06-26 12:55:08
Purpose:
Description:		This File was auto generated using SOC Online

************************************************************************************/
#ifndef _ML_LOCKER_LCPU2_REGS_H_
#define _ML_LOCKER_LCPU2_REGS_H_

/*---------------------------------------------------------------------------------
/						Registers Addresses													 
/----------------------------------------------------------------------------------*/
#include "HwMemoryMap.h"

#define ML_LOCKER_LCPU2_BASE_ADDRESS                        MEMORY_MAP_UNIT_1147_BASE_ADDRESS
#define	REG_ML_LOCKER_LCPU2_LOCK_STA_TID_REG                      (ML_LOCKER_LCPU2_BASE_ADDRESS + 0x0)
#define	REG_ML_LOCKER_LCPU2_LOCK_STA_TID_IRQ_REG                  (ML_LOCKER_LCPU2_BASE_ADDRESS + 0x4)
#define	REG_ML_LOCKER_LCPU2_LOCK_REQ_STATUS_REG                   (ML_LOCKER_LCPU2_BASE_ADDRESS + 0x8)
#define	REG_ML_LOCKER_LCPU2_UPDATE_MLD_REG                        (ML_LOCKER_LCPU2_BASE_ADDRESS + 0xC)
#define	REG_ML_LOCKER_LCPU2_SUSPEND_MLD_REG                       (ML_LOCKER_LCPU2_BASE_ADDRESS + 0x10)
#define	REG_ML_LOCKER_LCPU2_SUSPEND_MLD_IRQ_REG                   (ML_LOCKER_LCPU2_BASE_ADDRESS + 0x14)
#define	REG_ML_LOCKER_LCPU2_UPDATE_EMLSR_REG                      (ML_LOCKER_LCPU2_BASE_ADDRESS + 0x18)
#define	REG_ML_LOCKER_LCPU2_UPDATE_ALLOW_LOCK_REG                 (ML_LOCKER_LCPU2_BASE_ADDRESS + 0x1C)
#define	REG_ML_LOCKER_LCPU2_FIFOS_ALMOST_FULL_THR_REGISTER        (ML_LOCKER_LCPU2_BASE_ADDRESS + 0x20)
#define	REG_ML_LOCKER_LCPU2_INIT_MEMORIES_REG                     (ML_LOCKER_LCPU2_BASE_ADDRESS + 0x24)
#define	REG_ML_LOCKER_LCPU2_INIT_MEMORIES_DONE_REG                (ML_LOCKER_LCPU2_BASE_ADDRESS + 0x28)
#define	REG_ML_LOCKER_LCPU2_PS_POLL_DURATION_REG                  (ML_LOCKER_LCPU2_BASE_ADDRESS + 0x2C)
#define	REG_ML_LOCKER_LCPU2_IGNORE_TWT_SERVICE_PERIOD_REG         (ML_LOCKER_LCPU2_BASE_ADDRESS + 0x30)
#define	REG_ML_LOCKER_LCPU2_RX_TXOP_DETECTOR_CONFIGURATION_REG    (ML_LOCKER_LCPU2_BASE_ADDRESS + 0x34)
#define	REG_ML_LOCKER_LCPU2_UNLOCK_BUSY_REG                       (ML_LOCKER_LCPU2_BASE_ADDRESS + 0x38)
/*---------------------------------------------------------------------------------
/						Data Type Definition										
/----------------------------------------------------------------------------------*/
/*REG_ML_LOCKER_LCPU2_LOCK_STA_TID_REG 0x0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 lockStaTidOp : 2; //no description, reset value: 0x0, access type: WO
		uint32 lockStaTidBand : 2; //no description, reset value: 0x0, access type: WO
		uint32 lockStaTidInbandStaIdx : 7; //no description, reset value: 0x0, access type: WO
		uint32 lockStaTidTid : 4; //no description, reset value: 0x0, access type: WO
		uint32 lockStaTidLockType : 1; //no description, reset value: 0x0, access type: WO
		uint32 reserved0 : 16;
	} bitFields;
} RegMlLockerLcpu2LockStaTidReg_u;

/*REG_ML_LOCKER_LCPU2_LOCK_STA_TID_IRQ_REG 0x4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 lockStaTidClearIrq : 1; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 7;
		uint32 lockStaTidEnableIrq : 1; //no description, reset value: 0x1, access type: RW
		uint32 reserved1 : 23;
	} bitFields;
} RegMlLockerLcpu2LockStaTidIrqReg_u;

/*REG_ML_LOCKER_LCPU2_LOCK_REQ_STATUS_REG 0x8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 lockReqStatus : 2; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 30;
	} bitFields;
} RegMlLockerLcpu2LockReqStatusReg_u;

/*REG_ML_LOCKER_LCPU2_UPDATE_MLD_REG 0xC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 updateMldOp : 1; //no description, reset value: 0x0, access type: WO
		uint32 updateMldMldIdx : 7; //no description, reset value: 0x0, access type: WO
		uint32 updateMldBand : 2; //no description, reset value: 0x0, access type: WO
		uint32 updateMldInbandStaIdx : 7; //no description, reset value: 0x0, access type: WO
		uint32 updateMldPsModeValue : 1; //no description, reset value: 0x0, access type: WO
		uint32 updateMldPsModeEnable : 1; //no description, reset value: 0x0, access type: WO
		uint32 reserved0 : 13;
	} bitFields;
} RegMlLockerLcpu2UpdateMldReg_u;

/*REG_ML_LOCKER_LCPU2_SUSPEND_MLD_REG 0x10 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 suspendMldBand : 2; //no description, reset value: 0x0, access type: WO
		uint32 suspendMldInbandStaIdx : 7; //no description, reset value: 0x0, access type: WO
		uint32 suspendMldIntrRequest : 1; //no description, reset value: 0x0, access type: WO
		uint32 reserved0 : 22;
	} bitFields;
} RegMlLockerLcpu2SuspendMldReg_u;

/*REG_ML_LOCKER_LCPU2_SUSPEND_MLD_IRQ_REG 0x14 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 suspendMldClearIrq : 1; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 7;
		uint32 suspendMldEnableIrq : 1; //no description, reset value: 0x1, access type: RW
		uint32 reserved1 : 23;
	} bitFields;
} RegMlLockerLcpu2SuspendMldIrqReg_u;

/*REG_ML_LOCKER_LCPU2_UPDATE_EMLSR_REG 0x18 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 updateEmlsrBand : 2; //no description, reset value: 0x0, access type: WO
		uint32 updateEmlsrInbandStaIdx : 7; //no description, reset value: 0x0, access type: WO
		uint32 emlsrLinkBitmap : 3; //no description, reset value: 0x0, access type: WO
		uint32 unlockRxDelay : 8; //no description, reset value: 0x80, access type: WO
		uint32 psModeValue : 3; //no description, reset value: 0x0, access type: WO
		uint32 psModeEnable : 3; //no description, reset value: 0x7, access type: WO
		uint32 reserved0 : 6;
	} bitFields;
} RegMlLockerLcpu2UpdateEmlsrReg_u;

/*REG_ML_LOCKER_LCPU2_UPDATE_ALLOW_LOCK_REG 0x1C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 updateAllowLockBand : 2; //no description, reset value: 0x0, access type: WO
		uint32 updateAllowLockInbandStaIdx : 7; //no description, reset value: 0x0, access type: WO
		uint32 allowLockEn : 1; //no description, reset value: 0x0, access type: WO
		uint32 reserved0 : 22;
	} bitFields;
} RegMlLockerLcpu2UpdateAllowLockReg_u;

/*REG_ML_LOCKER_LCPU2_FIFOS_ALMOST_FULL_THR_REGISTER 0x20 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txselLowPrioFifo : 3; //no description, reset value: 0x6, access type: WO
		uint32 txselHighPrioFifo : 3; //no description, reset value: 0x6, access type: WO
		uint32 reserved0 : 26;
	} bitFields;
} RegMlLockerLcpu2FifosAlmostFullThrRegister_u;

/*REG_ML_LOCKER_LCPU2_INIT_MEMORIES_REG 0x24 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 initMlLockerDb : 1; //no description, reset value: 0x0, access type: WO
		uint32 reserved0 : 7;
		uint32 initMlLockerBitmap : 1; //no description, reset value: 0x0, access type: WO
		uint32 reserved1 : 23;
	} bitFields;
} RegMlLockerLcpu2InitMemoriesReg_u;

/*REG_ML_LOCKER_LCPU2_INIT_MEMORIES_DONE_REG 0x28 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 initMlLockerDbDone : 1; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 7;
		uint32 initMlLockerBitmapDone : 1; //no description, reset value: 0x0, access type: RO
		uint32 reserved1 : 23;
	} bitFields;
} RegMlLockerLcpu2InitMemoriesDoneReg_u;

/*REG_ML_LOCKER_LCPU2_PS_POLL_DURATION_REG 0x2C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 psPollDuration : 16; //no description, reset value: 0x0, access type: WO
		uint32 reserved0 : 16;
	} bitFields;
} RegMlLockerLcpu2PsPollDurationReg_u;

/*REG_ML_LOCKER_LCPU2_IGNORE_TWT_SERVICE_PERIOD_REG 0x30 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ignoreTwtServicePeriod : 1; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegMlLockerLcpu2IgnoreTwtServicePeriodReg_u;

/*REG_ML_LOCKER_LCPU2_RX_TXOP_DETECTOR_CONFIGURATION_REG 0x34 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 extUnlockRxDelayInUsec : 6; //no description, reset value: 0x10, access type: RW
		uint32 afterRxEndedDelayInUsec : 4; //no description, reset value: 0x1, access type: RW
		uint32 txRxTimeoutInUsec : 6; //set this value to N [us] and the actual timeout in code will be in range [N-1 , N] [us]. , for example, setting 4 [us] will set timeout between 3-4 [us]. , reset value: 0x10, access type: RW
		uint32 rxRxTimeoutInUsec : 6; //no description, reset value: 0x10, access type: RW
		uint32 enableRxTxopDetector : 1; //no description, reset value: 0x1, access type: RW
		uint32 reserved0 : 9;
	} bitFields;
} RegMlLockerLcpu2RxTxopDetectorConfigurationReg_u;

/*REG_ML_LOCKER_LCPU2_UNLOCK_BUSY_REG 0x38 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 unlockBusy : 1; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegMlLockerLcpu2UnlockBusyReg_u;

//========================================
/* REG_ML_LOCKER_LCPU2_LOCK_STA_TID_REG 0x0 */
#define ML_LOCKER_LCPU2_LOCK_STA_TID_REG_FIELD_LOCK_STA_TID_OP_MASK                                               0x00000003
#define ML_LOCKER_LCPU2_LOCK_STA_TID_REG_FIELD_LOCK_STA_TID_BAND_MASK                                             0x0000000C
#define ML_LOCKER_LCPU2_LOCK_STA_TID_REG_FIELD_LOCK_STA_TID_INBAND_STA_IDX_MASK                                   0x000007F0
#define ML_LOCKER_LCPU2_LOCK_STA_TID_REG_FIELD_LOCK_STA_TID_TID_MASK                                              0x00007800
#define ML_LOCKER_LCPU2_LOCK_STA_TID_REG_FIELD_LOCK_STA_TID_LOCK_TYPE_MASK                                        0x00008000
//========================================
/* REG_ML_LOCKER_LCPU2_LOCK_STA_TID_IRQ_REG 0x4 */
#define ML_LOCKER_LCPU2_LOCK_STA_TID_IRQ_REG_FIELD_LOCK_STA_TID_CLEAR_IRQ_MASK                                    0x00000001
#define ML_LOCKER_LCPU2_LOCK_STA_TID_IRQ_REG_FIELD_LOCK_STA_TID_ENABLE_IRQ_MASK                                   0x00000100
//========================================
/* REG_ML_LOCKER_LCPU2_LOCK_REQ_STATUS_REG 0x8 */
#define ML_LOCKER_LCPU2_LOCK_REQ_STATUS_REG_FIELD_LOCK_REQ_STATUS_MASK                                            0x00000003
//========================================
/* REG_ML_LOCKER_LCPU2_UPDATE_MLD_REG 0xC */
#define ML_LOCKER_LCPU2_UPDATE_MLD_REG_FIELD_UPDATE_MLD_OP_MASK                                                   0x00000001
#define ML_LOCKER_LCPU2_UPDATE_MLD_REG_FIELD_UPDATE_MLD_MLD_IDX_MASK                                              0x000000FE
#define ML_LOCKER_LCPU2_UPDATE_MLD_REG_FIELD_UPDATE_MLD_BAND_MASK                                                 0x00000300
#define ML_LOCKER_LCPU2_UPDATE_MLD_REG_FIELD_UPDATE_MLD_INBAND_STA_IDX_MASK                                       0x0001FC00
#define ML_LOCKER_LCPU2_UPDATE_MLD_REG_FIELD_UPDATE_MLD_PS_MODE_VALUE_MASK                                        0x00020000
#define ML_LOCKER_LCPU2_UPDATE_MLD_REG_FIELD_UPDATE_MLD_PS_MODE_ENABLE_MASK                                       0x00040000
//========================================
/* REG_ML_LOCKER_LCPU2_SUSPEND_MLD_REG 0x10 */
#define ML_LOCKER_LCPU2_SUSPEND_MLD_REG_FIELD_SUSPEND_MLD_BAND_MASK                                               0x00000003
#define ML_LOCKER_LCPU2_SUSPEND_MLD_REG_FIELD_SUSPEND_MLD_INBAND_STA_IDX_MASK                                     0x000001FC
#define ML_LOCKER_LCPU2_SUSPEND_MLD_REG_FIELD_SUSPEND_MLD_INTR_REQUEST_MASK                                       0x00000200
//========================================
/* REG_ML_LOCKER_LCPU2_SUSPEND_MLD_IRQ_REG 0x14 */
#define ML_LOCKER_LCPU2_SUSPEND_MLD_IRQ_REG_FIELD_SUSPEND_MLD_CLEAR_IRQ_MASK                                      0x00000001
#define ML_LOCKER_LCPU2_SUSPEND_MLD_IRQ_REG_FIELD_SUSPEND_MLD_ENABLE_IRQ_MASK                                     0x00000100
//========================================
/* REG_ML_LOCKER_LCPU2_UPDATE_EMLSR_REG 0x18 */
#define ML_LOCKER_LCPU2_UPDATE_EMLSR_REG_FIELD_UPDATE_EMLSR_BAND_MASK                                             0x00000003
#define ML_LOCKER_LCPU2_UPDATE_EMLSR_REG_FIELD_UPDATE_EMLSR_INBAND_STA_IDX_MASK                                   0x000001FC
#define ML_LOCKER_LCPU2_UPDATE_EMLSR_REG_FIELD_EMLSR_LINK_BITMAP_MASK                                             0x00000E00
#define ML_LOCKER_LCPU2_UPDATE_EMLSR_REG_FIELD_UNLOCK_RX_DELAY_MASK                                               0x000FF000
#define ML_LOCKER_LCPU2_UPDATE_EMLSR_REG_FIELD_PS_MODE_VALUE_MASK                                                 0x00700000
#define ML_LOCKER_LCPU2_UPDATE_EMLSR_REG_FIELD_PS_MODE_ENABLE_MASK                                                0x03800000
//========================================
/* REG_ML_LOCKER_LCPU2_UPDATE_ALLOW_LOCK_REG 0x1C */
#define ML_LOCKER_LCPU2_UPDATE_ALLOW_LOCK_REG_FIELD_UPDATE_ALLOW_LOCK_BAND_MASK                                   0x00000003
#define ML_LOCKER_LCPU2_UPDATE_ALLOW_LOCK_REG_FIELD_UPDATE_ALLOW_LOCK_INBAND_STA_IDX_MASK                         0x000001FC
#define ML_LOCKER_LCPU2_UPDATE_ALLOW_LOCK_REG_FIELD_ALLOW_LOCK_EN_MASK                                            0x00000200
//========================================
/* REG_ML_LOCKER_LCPU2_FIFOS_ALMOST_FULL_THR_REGISTER 0x20 */
#define ML_LOCKER_LCPU2_FIFOS_ALMOST_FULL_THR_REGISTER_FIELD_TXSEL_LOW_PRIO_FIFO_MASK                             0x00000007
#define ML_LOCKER_LCPU2_FIFOS_ALMOST_FULL_THR_REGISTER_FIELD_TXSEL_HIGH_PRIO_FIFO_MASK                            0x00000038
//========================================
/* REG_ML_LOCKER_LCPU2_INIT_MEMORIES_REG 0x24 */
#define ML_LOCKER_LCPU2_INIT_MEMORIES_REG_FIELD_INIT_ML_LOCKER_DB_MASK                                            0x00000001
#define ML_LOCKER_LCPU2_INIT_MEMORIES_REG_FIELD_INIT_ML_LOCKER_BITMAP_MASK                                        0x00000100
//========================================
/* REG_ML_LOCKER_LCPU2_INIT_MEMORIES_DONE_REG 0x28 */
#define ML_LOCKER_LCPU2_INIT_MEMORIES_DONE_REG_FIELD_INIT_ML_LOCKER_DB_DONE_MASK                                  0x00000001
#define ML_LOCKER_LCPU2_INIT_MEMORIES_DONE_REG_FIELD_INIT_ML_LOCKER_BITMAP_DONE_MASK                              0x00000100
//========================================
/* REG_ML_LOCKER_LCPU2_PS_POLL_DURATION_REG 0x2C */
#define ML_LOCKER_LCPU2_PS_POLL_DURATION_REG_FIELD_PS_POLL_DURATION_MASK                                          0x0000FFFF
//========================================
/* REG_ML_LOCKER_LCPU2_IGNORE_TWT_SERVICE_PERIOD_REG 0x30 */
#define ML_LOCKER_LCPU2_IGNORE_TWT_SERVICE_PERIOD_REG_FIELD_IGNORE_TWT_SERVICE_PERIOD_MASK                        0x00000001
//========================================
/* REG_ML_LOCKER_LCPU2_RX_TXOP_DETECTOR_CONFIGURATION_REG 0x34 */
#define ML_LOCKER_LCPU2_RX_TXOP_DETECTOR_CONFIGURATION_REG_FIELD_EXT_UNLOCK_RX_DELAY_IN_USEC_MASK                 0x0000003F
#define ML_LOCKER_LCPU2_RX_TXOP_DETECTOR_CONFIGURATION_REG_FIELD_AFTER_RX_ENDED_DELAY_IN_USEC_MASK                0x000003C0
#define ML_LOCKER_LCPU2_RX_TXOP_DETECTOR_CONFIGURATION_REG_FIELD_TX_RX_TIMEOUT_IN_USEC_MASK                       0x0000FC00
#define ML_LOCKER_LCPU2_RX_TXOP_DETECTOR_CONFIGURATION_REG_FIELD_RX_RX_TIMEOUT_IN_USEC_MASK                       0x003F0000
#define ML_LOCKER_LCPU2_RX_TXOP_DETECTOR_CONFIGURATION_REG_FIELD_ENABLE_RX_TXOP_DETECTOR_MASK                     0x00400000
//========================================
/* REG_ML_LOCKER_LCPU2_UNLOCK_BUSY_REG 0x38 */
#define ML_LOCKER_LCPU2_UNLOCK_BUSY_REG_FIELD_UNLOCK_BUSY_MASK                                                    0x00000001


#endif // _ML_LOCKER_LCPU2_REGS_H_
