
/***********************************************************************************
File:				PacTimRegs.h
Module:				pacTim
SOC Revision:		latest
Generated at:       2024-06-26 12:55:15
Purpose:
Description:		This File was auto generated using SOC Online

************************************************************************************/
#ifndef _PAC_TIM_REGS_H_
#define _PAC_TIM_REGS_H_

/*---------------------------------------------------------------------------------
/						Registers Addresses													 
/----------------------------------------------------------------------------------*/
#include "HwMemoryMap.h"

#define PAC_TIM_BASE_ADDRESS                                       MEMORY_MAP_UNIT_13_BASE_ADDRESS
#define	REG_PAC_TIM_ONE_MICRO_PRESCALER                          (PAC_TIM_BASE_ADDRESS + 0x0)
#define	REG_PAC_TIM_EIGHTH_MICRO_PRESCALER                       (PAC_TIM_BASE_ADDRESS + 0x4)
#define	REG_PAC_TIM_TSF_HW_EVENT0                                (PAC_TIM_BASE_ADDRESS + 0x8)
#define	REG_PAC_TIM_TSF_HW_EVENT1                                (PAC_TIM_BASE_ADDRESS + 0xC)
#define	REG_PAC_TIM_TSF_REQ_SWITCH                               (PAC_TIM_BASE_ADDRESS + 0x10)
#define	REG_PAC_TIM_TSF_SW_EVENT2                                (PAC_TIM_BASE_ADDRESS + 0x14)
#define	REG_PAC_TIM_TSF_SW_EVENT3                                (PAC_TIM_BASE_ADDRESS + 0x18)
#define	REG_PAC_TIM_TICK_TIMER                                   (PAC_TIM_BASE_ADDRESS + 0x2C)
#define	REG_PAC_TIM_SLOT_TIMER                                   (PAC_TIM_BASE_ADDRESS + 0x30)
#define	REG_PAC_TIM_TIMER_CONTROL                                (PAC_TIM_BASE_ADDRESS + 0x34)
#define	REG_PAC_TIM_TIMER_CONTROL_RD                             (PAC_TIM_BASE_ADDRESS + 0x38)
#define	REG_PAC_TIM_TICK                                         (PAC_TIM_BASE_ADDRESS + 0x44)
#define	REG_PAC_TIM_SLOT                                         (PAC_TIM_BASE_ADDRESS + 0x48)
#define	REG_PAC_TIM_IFS_TIMER                                    (PAC_TIM_BASE_ADDRESS + 0x50)
#define	REG_PAC_TIM_RESP_TIMER                                   (PAC_TIM_BASE_ADDRESS + 0x54)
#define	REG_PAC_TIM_TSF_TIMER_LOW_WPORT                          (PAC_TIM_BASE_ADDRESS + 0x58)
#define	REG_PAC_TIM_TSF_TIMER_HIGH_WPORT                         (PAC_TIM_BASE_ADDRESS + 0x5C)
#define	REG_PAC_TIM_TSF_TIMER_LOW_RPORT                          (PAC_TIM_BASE_ADDRESS + 0x60)
#define	REG_PAC_TIM_TSF_TIMER_HIGH_RPORT                         (PAC_TIM_BASE_ADDRESS + 0x64)
#define	REG_PAC_TIM_TSF_HW_EVENT2                                (PAC_TIM_BASE_ADDRESS + 0x68)
#define	REG_PAC_TIM_TTL                                          (PAC_TIM_BASE_ADDRESS + 0x6C)
#define	REG_PAC_TIM_QBSS_LOAD_INTEGRATOR_WR                      (PAC_TIM_BASE_ADDRESS + 0x88)
#define	REG_PAC_TIM_QBSS_LOAD_INTEGRATOR_RD                      (PAC_TIM_BASE_ADDRESS + 0x8C)
#define	REG_PAC_TIM_REDUCED_FREQ_CHANGE_LATCHED_TSF_TIMER_LOW    (PAC_TIM_BASE_ADDRESS + 0x90)
#define	REG_PAC_TIM_RX_DUR_TIMER                                 (PAC_TIM_BASE_ADDRESS + 0x98)
#define	REG_PAC_TIM_DELIA_TIMER                                  (PAC_TIM_BASE_ADDRESS + 0x9C)
#define	REG_PAC_TIM_DELIA_MAX_LIMIT_TIMER                        (PAC_TIM_BASE_ADDRESS + 0xA0)
#define	REG_PAC_TIM_TXH_PAUSER_REP_TIMER                         (PAC_TIM_BASE_ADDRESS + 0xA4)
#define	REG_PAC_TIM_TX_NAV_TIMER_CFG_SET_VALUE                   (PAC_TIM_BASE_ADDRESS + 0xB0)
#define	REG_PAC_TIM_TX_NAV_TIMER                                 (PAC_TIM_BASE_ADDRESS + 0xB4)
#define	REG_PAC_TIM_TX_NAV_TIMER_IFS_COMPENSATION                (PAC_TIM_BASE_ADDRESS + 0xB8)
#define	REG_PAC_TIM_TIMER_CONTROL2                               (PAC_TIM_BASE_ADDRESS + 0xBC)
#define	REG_PAC_TIM_QBSS2_LOAD_INTEGRATOR_WR                     (PAC_TIM_BASE_ADDRESS + 0xC0)
#define	REG_PAC_TIM_QBSS2_LOAD_INTEGRATOR_RD                     (PAC_TIM_BASE_ADDRESS + 0xC4)
#define	REG_PAC_TIM_TIMER_CONTROL3                               (PAC_TIM_BASE_ADDRESS + 0xC8)
#define	REG_PAC_TIM_QBSS3_LOAD_INTEGRATOR_WR                     (PAC_TIM_BASE_ADDRESS + 0xCC)
#define	REG_PAC_TIM_QBSS3_LOAD_INTEGRATOR_RD                     (PAC_TIM_BASE_ADDRESS + 0xE0)
#define	REG_PAC_TIM_TIMER_CONTROL4                               (PAC_TIM_BASE_ADDRESS + 0xE4)
#define	REG_PAC_TIM_QBSS4_LOAD_INTEGRATOR_WR                     (PAC_TIM_BASE_ADDRESS + 0xE8)
#define	REG_PAC_TIM_BSSS4_LOAD_INTEGRATOR_RD                     (PAC_TIM_BASE_ADDRESS + 0xEC)
/*---------------------------------------------------------------------------------
/						Data Type Definition										
/----------------------------------------------------------------------------------*/
/*REG_PAC_TIM_ONE_MICRO_PRESCALER 0x0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 oneMicroPrescaler : 10; //TIM one micro prescaler, reset value: 0x27f, access type: RW
		uint32 reserved0 : 6;
		uint32 oneMicroPrescalerFreqReduced : 10; //TIM one micro prescaler reduced clk, reset value: 0x0, access type: RW
		uint32 reserved1 : 6;
	} bitFields;
} RegPacTimOneMicroPrescaler_u;

/*REG_PAC_TIM_EIGHTH_MICRO_PRESCALER 0x4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 eighthMicroPrescaler : 10; //TIM eighth micro prescaler, reset value: 0x4f, access type: RW
		uint32 reserved0 : 6;
		uint32 eighthMicroPrescalerFreqReduced : 10; //TIM eighth micro prescaler reduced clk, reset value: 0x0, access type: RW
		uint32 reserved1 : 6;
	} bitFields;
} RegPacTimEighthMicroPrescaler_u;

/*REG_PAC_TIM_TSF_HW_EVENT0 0x8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tsfHwEvent0Time : 24; //TIM  time, reset value: 0x0, access type: RW
		uint32 reserved0 : 4;
		uint32 tsfHwEvent0HwEnableRd : 1; //TIM  hw enable RD, reset value: 0x0, access type: RO
		uint32 tsfHwEvent0SwEnableRd : 1; //TIM  sw enable RD, reset value: 0x0, access type: RO
		uint32 tsfHwEvent0HwEnableWr : 1; //TIM  hw enable WR, reset value: 0x0, access type: WO
		uint32 tsfHwEvent0SwEnableWr : 1; //TIM  sw enable WR, reset value: 0x0, access type: WO
	} bitFields;
} RegPacTimTsfHwEvent0_u;

/*REG_PAC_TIM_TSF_HW_EVENT1 0xC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tsfHwEvent1Time : 24; //TIM  time, reset value: 0x0, access type: RW
		uint32 reserved0 : 4;
		uint32 tsfHwEvent1HwEnableRd : 1; //TIM  hw enable RD, reset value: 0x0, access type: RO
		uint32 tsfHwEvent1SwEnableRd : 1; //TIM  sw enable RD, reset value: 0x0, access type: RO
		uint32 tsfHwEvent1HwEnableWr : 1; //TIM  hw enable WR, reset value: 0x0, access type: WO
		uint32 tsfHwEvent1SwEnableWr : 1; //TIM  sw enable WR, reset value: 0x0, access type: WO
	} bitFields;
} RegPacTimTsfHwEvent1_u;

/*REG_PAC_TIM_TSF_REQ_SWITCH 0x10 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tsfReqSwitchTime : 24; //TIM  time, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
		uint32 tsfReqSwitchHwEnableRd : 1; //TIM  hw enable RD, reset value: 0x0, access type: RO
		uint32 tsfReqSwitchSwEnableRd : 1; //TIM  sw enable RD, reset value: 0x0, access type: RO
		uint32 reserved1 : 1;
		uint32 tsfReqSwitchTo1 : 1; //TIM  to 1, reset value: 0x0, access type: RW
		uint32 tsfReqSwitchHwEnableWr : 1; //TIM  hw enable WR, reset value: 0x0, access type: WO
		uint32 tsfReqSwitchSwEnableWr : 1; //TIM  sw enable WR, reset value: 0x0, access type: WO
	} bitFields;
} RegPacTimTsfReqSwitch_u;

/*REG_PAC_TIM_TSF_SW_EVENT2 0x14 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tsfSwEvent2Time : 24; //TIM  time event 2, reset value: 0x0, access type: RW
		uint32 reserved0 : 6;
		uint32 tsfSwEvent2EnableRd : 1; //TIM  enable event 2 RD, reset value: 0x0, access type: RO
		uint32 tsfSwEvent2EnableWr : 1; //TIM  enable event 2 WR, reset value: 0x0, access type: WO
	} bitFields;
} RegPacTimTsfSwEvent2_u;

/*REG_PAC_TIM_TSF_SW_EVENT3 0x18 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tsfSwEvent3Time : 24; //TIM  time event 3, reset value: 0x0, access type: RW
		uint32 reserved0 : 6;
		uint32 tsfSwEvent3EnableRd : 1; //TIM  enable event 3 RD, reset value: 0x0, access type: RO
		uint32 tsfSwEvent3EnableWr : 1; //TIM  enable event 3 WR, reset value: 0x0, access type: WO
	} bitFields;
} RegPacTimTsfSwEvent3_u;

/*REG_PAC_TIM_TICK_TIMER 0x2C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tickTimerPeriod : 16; //TIM  period, reset value: 0xffff, access type: RW
		uint32 reserved0 : 15;
		uint32 tickTimerEnable : 1; //TIM  enable, reset value: 0x0, access type: RW
	} bitFields;
} RegPacTimTickTimer_u;

/*REG_PAC_TIM_SLOT_TIMER 0x30 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 16;
		uint32 slotTimerPeriod : 8; //Slot period, reset value: 0x8, access type: RW
		uint32 reserved1 : 8;
	} bitFields;
} RegPacTimSlotTimer_u;

/*REG_PAC_TIM_TIMER_CONTROL 0x34 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 1;
		uint32 tsfEnable : 1; //TIM  tsf enable, reset value: 0x0, access type: RW
		uint32 tsfNudgeWr : 1; //TIM  tsf nudge WR, reset value: 0x0, access type: WO
		uint32 reserved1 : 1;
		uint32 tsfNudgeRd : 1; //TIM  tsf nudge RD, reset value: 0x0, access type: RO
		uint32 integrateTxTimeToQbss : 1; //Integrate Tx time to QBSS load integrator, reset value: 0x0, access type: RW
		uint32 integrateNavTimeToQbss : 1; //Integrate NAV time to QBSS load integrator, reset value: 0x0, access type: RW
		uint32 integrateCcaTimeToQbss : 1; //Integrate CCA time to QBSS load integrator, reset value: 0x0, access type: RW
		uint32 tsfAdj : 9; //TIM  tsf adj, reset value: 0x0, access type: RW
		uint32 reserved2 : 7;
		uint32 tsfSwEvent0Wr : 1; //TIM  tsf sw event0 WR, reset value: 0x0, access type: WO
		uint32 tsfSwEvent1Wr : 1; //TIM  tsf sw event1 WR, reset value: 0x0, access type: WO
		uint32 tsfSwEvent2Wr : 1; //TIM  tsf sw event2 WR, reset value: 0x0, access type: WO
		uint32 tsfSwEvent3Wr : 1; //TIM  tsf sw event3 WR, reset value: 0x0, access type: WO
		uint32 tsfSwEvent4Wr : 1; //TIM  tsf sw event4 WR, reset value: 0x0, access type: WO
		uint32 tsfSwEvent5Wr : 1; //TIM  tsf sw event5 WR, reset value: 0x0, access type: WO
		uint32 reserved3 : 2;
	} bitFields;
} RegPacTimTimerControl_u;

/*REG_PAC_TIM_TIMER_CONTROL_RD 0x38 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 24;
		uint32 tsfSwEvent0Rd : 1; //TIM  tsf sw event0 RD, reset value: 0x0, access type: RO
		uint32 tsfSwEvent1Rd : 1; //TIM  tsf sw event1 RD, reset value: 0x0, access type: RO
		uint32 tsfSwEvent2Rd : 1; //TIM  tsf sw event2 RD, reset value: 0x0, access type: RO
		uint32 tsfSwEvent3Rd : 1; //TIM  tsf sw event3 RD, reset value: 0x0, access type: RO
		uint32 tsfSwEvent4Rd : 1; //TIM  tsf sw event4 RD, reset value: 0x0, access type: RO
		uint32 tsfSwEvent5Rd : 1; //TIM  tsf sw event5 RD, reset value: 0x0, access type: RO
		uint32 reserved1 : 2;
	} bitFields;
} RegPacTimTimerControlRd_u;

/*REG_PAC_TIM_TICK 0x44 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 timerTick : 16; //TIM  timer, reset value: 0xffff, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegPacTimTick_u;

/*REG_PAC_TIM_SLOT 0x48 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 16;
		uint32 slotTimer : 8; //Slot timer, reset value: 0x8, access type: RO
		uint32 reserved1 : 8;
	} bitFields;
} RegPacTimSlot_u;

/*REG_PAC_TIM_IFS_TIMER 0x50 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ifsTimer : 19; //TIM  ifs timer, reset value: 0x0, access type: RO
		uint32 reserved0 : 13;
	} bitFields;
} RegPacTimIfsTimer_u;

/*REG_PAC_TIM_RESP_TIMER 0x54 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 respTimer : 13; //TIM  resp timer, reset value: 0x0, access type: RO
		uint32 reserved0 : 19;
	} bitFields;
} RegPacTimRespTimer_u;

/*REG_PAC_TIM_TSF_TIMER_LOW_WPORT 0x58 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tsfTimerLowWport : 32; //TIM  tsf timer low WR, reset value: 0x0, access type: WO
	} bitFields;
} RegPacTimTsfTimerLowWport_u;

/*REG_PAC_TIM_TSF_TIMER_HIGH_WPORT 0x5C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tsfTimerHighWport : 32; //TIM  tsf timer high WR, reset value: 0x0, access type: WO
	} bitFields;
} RegPacTimTsfTimerHighWport_u;

/*REG_PAC_TIM_TSF_TIMER_LOW_RPORT 0x60 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tsfTimerLowRport : 32; //TIM  tsf timer low RD, reset value: 0x0, access type: RO
	} bitFields;
} RegPacTimTsfTimerLowRport_u;

/*REG_PAC_TIM_TSF_TIMER_HIGH_RPORT 0x64 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tsfTimerHighRport : 32; //TIM  tsf timer high RD, reset value: 0x0, access type: RO
	} bitFields;
} RegPacTimTsfTimerHighRport_u;

/*REG_PAC_TIM_TSF_HW_EVENT2 0x68 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tsfHwEvent2 : 24; //TSF HW event 2, reset value: 0x0, access type: RW
		uint32 reserved0 : 4;
		uint32 tsfHwEvent2HwEnableRd : 1; //TSF HW event 2 enable RD, reset value: 0x0, access type: RO
		uint32 tsfHwEvent2SwEnableRd : 1; //TSF SW event 2 enable RD, reset value: 0x0, access type: RO
		uint32 tsfHwEvent2HwEnableWr : 1; //TSF HW event 2 enable WR, reset value: 0x0, access type: WO
		uint32 tsfHwEvent2SwEnableWr : 1; //TSF SW event 2 enable WR, reset value: 0x0, access type: WO
	} bitFields;
} RegPacTimTsfHwEvent2_u;

/*REG_PAC_TIM_TTL 0x6C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ttl : 7; //TTL value - Quantized to 256K uS, reset value: 0x0, access type: RO
		uint32 reserved0 : 25;
	} bitFields;
} RegPacTimTtl_u;

/*REG_PAC_TIM_QBSS_LOAD_INTEGRATOR_WR 0x88 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 qbssLoadIntegratorWr : 28; //QBSS load integrator WR, reset value: 0x0, access type: WO
		uint32 reserved0 : 4;
	} bitFields;
} RegPacTimQbssLoadIntegratorWr_u;

/*REG_PAC_TIM_QBSS_LOAD_INTEGRATOR_RD 0x8C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 qbssLoadIntegratorRd : 28; //QBSS load integrator RD, reset value: 0x0, access type: RO
		uint32 reserved0 : 4;
	} bitFields;
} RegPacTimQbssLoadIntegratorRd_u;

/*REG_PAC_TIM_REDUCED_FREQ_CHANGE_LATCHED_TSF_TIMER_LOW 0x90 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reducedFreqChangeLatchedTsfTimerLow : 32; //reduced_freq_change_latched_tsf_timer_low, reset value: 0x0, access type: RO
	} bitFields;
} RegPacTimReducedFreqChangeLatchedTsfTimerLow_u;

/*REG_PAC_TIM_RX_DUR_TIMER 0x98 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxDurTimer : 8; //rx duration timer, reset value: 0x0, access type: RO
		uint32 reserved0 : 24;
	} bitFields;
} RegPacTimRxDurTimer_u;

/*REG_PAC_TIM_DELIA_TIMER 0x9C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 deliaTimer : 24; //delia timer, reset value: 0x0, access type: RO
		uint32 reserved0 : 8;
	} bitFields;
} RegPacTimDeliaTimer_u;

/*REG_PAC_TIM_DELIA_MAX_LIMIT_TIMER 0xA0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 deliaMaxLimitTimer : 24; //delia max limit timer, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegPacTimDeliaMaxLimitTimer_u;

/*REG_PAC_TIM_TXH_PAUSER_REP_TIMER 0xA4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txhPauserRepTimer : 16; //TxH Pauser repetition timer, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegPacTimTxhPauserRepTimer_u;

/*REG_PAC_TIM_TX_NAV_TIMER_CFG_SET_VALUE 0xB0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txNavTimerCfgSetValue : 18; //Setting Tx NAV timer value, reset value: 0x0, access type: RW
		uint32 reserved0 : 14;
	} bitFields;
} RegPacTimTxNavTimerCfgSetValue_u;

/*REG_PAC_TIM_TX_NAV_TIMER 0xB4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txNavTimer : 18; //Tx NAV timer, reset value: 0x0, access type: RO
		uint32 reserved0 : 14;
	} bitFields;
} RegPacTimTxNavTimer_u;

/*REG_PAC_TIM_TX_NAV_TIMER_IFS_COMPENSATION 0xB8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txNavIfsCompensation : 16; //Tx NAV timer IFS compensation [resolution in cycles], reset value: 0x0, access type: RW
		uint32 reducedFreqTxNavIfsCompensation : 16; //Tx NAV timer IFS compensation in mode of reduced frequency [resolution in cycles], reset value: 0x0, access type: RW
	} bitFields;
} RegPacTimTxNavTimerIfsCompensation_u;

/*REG_PAC_TIM_TIMER_CONTROL2 0xBC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 integrateTxTimeToQbss2 : 1; //Integrate Tx time to QBSS load integrator, reset value: 0x0, access type: RW
		uint32 integrateNavTimeToQbss2 : 1; //Integrate NAV time to QBSS load integrator, reset value: 0x0, access type: RW
		uint32 integrateCcaTimeToQbss2 : 1; //Integrate CCA time to QBSS load integrator, reset value: 0x0, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegPacTimTimerControl2_u;

/*REG_PAC_TIM_QBSS2_LOAD_INTEGRATOR_WR 0xC0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 qbss2LoadIntegratorWr : 28; //QBSS load integrator WR, reset value: 0x0, access type: WO
		uint32 reserved0 : 4;
	} bitFields;
} RegPacTimQbss2LoadIntegratorWr_u;

/*REG_PAC_TIM_QBSS2_LOAD_INTEGRATOR_RD 0xC4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 qbss2LoadIntegratorRd : 28; //QBSS load integrator RD, reset value: 0x0, access type: RO
		uint32 reserved0 : 4;
	} bitFields;
} RegPacTimQbss2LoadIntegratorRd_u;

/*REG_PAC_TIM_TIMER_CONTROL3 0xC8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 integrateTxTimeToQbss3 : 1; //Integrate Tx time to QBSS load integrator, reset value: 0x0, access type: RW
		uint32 integrateNavTimeToQbss3 : 1; //Integrate NAV time to QBSS load integrator, reset value: 0x0, access type: RW
		uint32 integrateCcaTimeToQbss3 : 1; //Integrate CCA time to QBSS load integrator, reset value: 0x0, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegPacTimTimerControl3_u;

/*REG_PAC_TIM_QBSS3_LOAD_INTEGRATOR_WR 0xCC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 qbss3LoadIntegratorWr : 28; //QBSS load integrator WR, reset value: 0x0, access type: WO
		uint32 reserved0 : 4;
	} bitFields;
} RegPacTimQbss3LoadIntegratorWr_u;

/*REG_PAC_TIM_QBSS3_LOAD_INTEGRATOR_RD 0xE0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 qbss3LoadIntegratorRd : 28; //QBSS load integrator RD, reset value: 0x0, access type: RO
		uint32 reserved0 : 4;
	} bitFields;
} RegPacTimQbss3LoadIntegratorRd_u;

/*REG_PAC_TIM_TIMER_CONTROL4 0xE4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 integrateTxTimeToQbss4 : 1; //Integrate Tx time to QBSS load integrator, reset value: 0x0, access type: RW
		uint32 integrateNavTimeToQbss4 : 1; //Integrate NAV time to QBSS load integrator, reset value: 0x0, access type: RW
		uint32 integrateCcaTimeToQbss4 : 1; //Integrate CCA time to QBSS load integrator, reset value: 0x0, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegPacTimTimerControl4_u;

/*REG_PAC_TIM_QBSS4_LOAD_INTEGRATOR_WR 0xE8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 qbss4LoadIntegratorWr : 28; //QBSS load integrator WR, reset value: 0x0, access type: WO
		uint32 reserved0 : 4;
	} bitFields;
} RegPacTimQbss4LoadIntegratorWr_u;

/*REG_PAC_TIM_BSSS4_LOAD_INTEGRATOR_RD 0xEC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 qbss4LoadIntegratorRd : 28; //QBSS load integrator RD, reset value: 0x0, access type: RO
		uint32 reserved0 : 4;
	} bitFields;
} RegPacTimBsss4LoadIntegratorRd_u;

//========================================
/* REG_PAC_TIM_ONE_MICRO_PRESCALER 0x0 */
#define B0_PAC_TIM_ONE_MICRO_PRESCALER_FIELD_ONE_MICRO_PRESCALER_MASK                                             0x000003FF
#define B0_PAC_TIM_ONE_MICRO_PRESCALER_FIELD_ONE_MICRO_PRESCALER_FREQ_REDUCED_MASK                                0x03FF0000
//========================================
/* REG_PAC_TIM_EIGHTH_MICRO_PRESCALER 0x4 */
#define B0_PAC_TIM_EIGHTH_MICRO_PRESCALER_FIELD_EIGHTH_MICRO_PRESCALER_MASK                                       0x000003FF
#define B0_PAC_TIM_EIGHTH_MICRO_PRESCALER_FIELD_EIGHTH_MICRO_PRESCALER_FREQ_REDUCED_MASK                          0x03FF0000
//========================================
/* REG_PAC_TIM_TSF_HW_EVENT0 0x8 */
#define B0_PAC_TIM_TSF_HW_EVENT0_FIELD_TSF_HW_EVENT0_TIME_MASK                                                    0x00FFFFFF
#define B0_PAC_TIM_TSF_HW_EVENT0_FIELD_TSF_HW_EVENT0_HW_ENABLE_RD_MASK                                            0x10000000
#define B0_PAC_TIM_TSF_HW_EVENT0_FIELD_TSF_HW_EVENT0_SW_ENABLE_RD_MASK                                            0x20000000
#define B0_PAC_TIM_TSF_HW_EVENT0_FIELD_TSF_HW_EVENT0_HW_ENABLE_WR_MASK                                            0x40000000
#define B0_PAC_TIM_TSF_HW_EVENT0_FIELD_TSF_HW_EVENT0_SW_ENABLE_WR_MASK                                            0x80000000
//========================================
/* REG_PAC_TIM_TSF_HW_EVENT1 0xC */
#define B0_PAC_TIM_TSF_HW_EVENT1_FIELD_TSF_HW_EVENT1_TIME_MASK                                                    0x00FFFFFF
#define B0_PAC_TIM_TSF_HW_EVENT1_FIELD_TSF_HW_EVENT1_HW_ENABLE_RD_MASK                                            0x10000000
#define B0_PAC_TIM_TSF_HW_EVENT1_FIELD_TSF_HW_EVENT1_SW_ENABLE_RD_MASK                                            0x20000000
#define B0_PAC_TIM_TSF_HW_EVENT1_FIELD_TSF_HW_EVENT1_HW_ENABLE_WR_MASK                                            0x40000000
#define B0_PAC_TIM_TSF_HW_EVENT1_FIELD_TSF_HW_EVENT1_SW_ENABLE_WR_MASK                                            0x80000000
//========================================
/* REG_PAC_TIM_TSF_REQ_SWITCH 0x10 */
#define B0_PAC_TIM_TSF_REQ_SWITCH_FIELD_TSF_REQ_SWITCH_TIME_MASK                                                  0x00FFFFFF
#define B0_PAC_TIM_TSF_REQ_SWITCH_FIELD_TSF_REQ_SWITCH_HW_ENABLE_RD_MASK                                          0x04000000
#define B0_PAC_TIM_TSF_REQ_SWITCH_FIELD_TSF_REQ_SWITCH_SW_ENABLE_RD_MASK                                          0x08000000
#define B0_PAC_TIM_TSF_REQ_SWITCH_FIELD_TSF_REQ_SWITCH_TO_1_MASK                                                  0x20000000
#define B0_PAC_TIM_TSF_REQ_SWITCH_FIELD_TSF_REQ_SWITCH_HW_ENABLE_WR_MASK                                          0x40000000
#define B0_PAC_TIM_TSF_REQ_SWITCH_FIELD_TSF_REQ_SWITCH_SW_ENABLE_WR_MASK                                          0x80000000
//========================================
/* REG_PAC_TIM_TSF_SW_EVENT2 0x14 */
#define B0_PAC_TIM_TSF_SW_EVENT2_FIELD_TSF_SW_EVENT2_TIME_MASK                                                    0x00FFFFFF
#define B0_PAC_TIM_TSF_SW_EVENT2_FIELD_TSF_SW_EVENT2_ENABLE_RD_MASK                                               0x40000000
#define B0_PAC_TIM_TSF_SW_EVENT2_FIELD_TSF_SW_EVENT2_ENABLE_WR_MASK                                               0x80000000
//========================================
/* REG_PAC_TIM_TSF_SW_EVENT3 0x18 */
#define B0_PAC_TIM_TSF_SW_EVENT3_FIELD_TSF_SW_EVENT3_TIME_MASK                                                    0x00FFFFFF
#define B0_PAC_TIM_TSF_SW_EVENT3_FIELD_TSF_SW_EVENT3_ENABLE_RD_MASK                                               0x40000000
#define B0_PAC_TIM_TSF_SW_EVENT3_FIELD_TSF_SW_EVENT3_ENABLE_WR_MASK                                               0x80000000
//========================================
/* REG_PAC_TIM_TICK_TIMER 0x2C */
#define B0_PAC_TIM_TICK_TIMER_FIELD_TICK_TIMER_PERIOD_MASK                                                        0x0000FFFF
#define B0_PAC_TIM_TICK_TIMER_FIELD_TICK_TIMER_ENABLE_MASK                                                        0x80000000
//========================================
/* REG_PAC_TIM_SLOT_TIMER 0x30 */
#define B0_PAC_TIM_SLOT_TIMER_FIELD_SLOT_TIMER_PERIOD_MASK                                                        0x00FF0000
//========================================
/* REG_PAC_TIM_TIMER_CONTROL 0x34 */
#define B0_PAC_TIM_TIMER_CONTROL_FIELD_TSF_ENABLE_MASK                                                            0x00000002
#define B0_PAC_TIM_TIMER_CONTROL_FIELD_TSF_NUDGE_WR_MASK                                                          0x00000004
#define B0_PAC_TIM_TIMER_CONTROL_FIELD_TSF_NUDGE_RD_MASK                                                          0x00000010
#define B0_PAC_TIM_TIMER_CONTROL_FIELD_INTEGRATE_TX_TIME_TO_QBSS_MASK                                             0x00000020
#define B0_PAC_TIM_TIMER_CONTROL_FIELD_INTEGRATE_NAV_TIME_TO_QBSS_MASK                                            0x00000040
#define B0_PAC_TIM_TIMER_CONTROL_FIELD_INTEGRATE_CCA_TIME_TO_QBSS_MASK                                            0x00000080
#define B0_PAC_TIM_TIMER_CONTROL_FIELD_TSF_ADJ_MASK                                                               0x0001FF00
#define B0_PAC_TIM_TIMER_CONTROL_FIELD_TSF_SW_EVENT0_WR_MASK                                                      0x01000000
#define B0_PAC_TIM_TIMER_CONTROL_FIELD_TSF_SW_EVENT1_WR_MASK                                                      0x02000000
#define B0_PAC_TIM_TIMER_CONTROL_FIELD_TSF_SW_EVENT2_WR_MASK                                                      0x04000000
#define B0_PAC_TIM_TIMER_CONTROL_FIELD_TSF_SW_EVENT3_WR_MASK                                                      0x08000000
#define B0_PAC_TIM_TIMER_CONTROL_FIELD_TSF_SW_EVENT4_WR_MASK                                                      0x10000000
#define B0_PAC_TIM_TIMER_CONTROL_FIELD_TSF_SW_EVENT5_WR_MASK                                                      0x20000000
//========================================
/* REG_PAC_TIM_TIMER_CONTROL_RD 0x38 */
#define B0_PAC_TIM_TIMER_CONTROL_RD_FIELD_TSF_SW_EVENT0_RD_MASK                                                   0x01000000
#define B0_PAC_TIM_TIMER_CONTROL_RD_FIELD_TSF_SW_EVENT1_RD_MASK                                                   0x02000000
#define B0_PAC_TIM_TIMER_CONTROL_RD_FIELD_TSF_SW_EVENT2_RD_MASK                                                   0x04000000
#define B0_PAC_TIM_TIMER_CONTROL_RD_FIELD_TSF_SW_EVENT3_RD_MASK                                                   0x08000000
#define B0_PAC_TIM_TIMER_CONTROL_RD_FIELD_TSF_SW_EVENT4_RD_MASK                                                   0x10000000
#define B0_PAC_TIM_TIMER_CONTROL_RD_FIELD_TSF_SW_EVENT5_RD_MASK                                                   0x20000000
//========================================
/* REG_PAC_TIM_TICK 0x44 */
#define B0_PAC_TIM_TICK_FIELD_TIMER_TICK_MASK                                                                     0x0000FFFF
//========================================
/* REG_PAC_TIM_SLOT 0x48 */
#define B0_PAC_TIM_SLOT_FIELD_SLOT_TIMER_MASK                                                                     0x00FF0000
//========================================
/* REG_PAC_TIM_IFS_TIMER 0x50 */
#define B0_PAC_TIM_IFS_TIMER_FIELD_IFS_TIMER_MASK                                                                 0x0007FFFF
//========================================
/* REG_PAC_TIM_RESP_TIMER 0x54 */
#define B0_PAC_TIM_RESP_TIMER_FIELD_RESP_TIMER_MASK                                                               0x00001FFF
//========================================
/* REG_PAC_TIM_TSF_TIMER_LOW_WPORT 0x58 */
#define B0_PAC_TIM_TSF_TIMER_LOW_WPORT_FIELD_TSF_TIMER_LOW_WPORT_MASK                                             0xFFFFFFFF
//========================================
/* REG_PAC_TIM_TSF_TIMER_HIGH_WPORT 0x5C */
#define B0_PAC_TIM_TSF_TIMER_HIGH_WPORT_FIELD_TSF_TIMER_HIGH_WPORT_MASK                                           0xFFFFFFFF
//========================================
/* REG_PAC_TIM_TSF_TIMER_LOW_RPORT 0x60 */
#define B0_PAC_TIM_TSF_TIMER_LOW_RPORT_FIELD_TSF_TIMER_LOW_RPORT_MASK                                             0xFFFFFFFF
//========================================
/* REG_PAC_TIM_TSF_TIMER_HIGH_RPORT 0x64 */
#define B0_PAC_TIM_TSF_TIMER_HIGH_RPORT_FIELD_TSF_TIMER_HIGH_RPORT_MASK                                           0xFFFFFFFF
//========================================
/* REG_PAC_TIM_TSF_HW_EVENT2 0x68 */
#define B0_PAC_TIM_TSF_HW_EVENT2_FIELD_TSF_HW_EVENT2_MASK                                                         0x00FFFFFF
#define B0_PAC_TIM_TSF_HW_EVENT2_FIELD_TSF_HW_EVENT2_HW_ENABLE_RD_MASK                                            0x10000000
#define B0_PAC_TIM_TSF_HW_EVENT2_FIELD_TSF_HW_EVENT2_SW_ENABLE_RD_MASK                                            0x20000000
#define B0_PAC_TIM_TSF_HW_EVENT2_FIELD_TSF_HW_EVENT2_HW_ENABLE_WR_MASK                                            0x40000000
#define B0_PAC_TIM_TSF_HW_EVENT2_FIELD_TSF_HW_EVENT2_SW_ENABLE_WR_MASK                                            0x80000000
//========================================
/* REG_PAC_TIM_TTL 0x6C */
#define B0_PAC_TIM_TTL_FIELD_TTL_MASK                                                                             0x0000007F
//========================================
/* REG_PAC_TIM_QBSS_LOAD_INTEGRATOR_WR 0x88 */
#define B0_PAC_TIM_QBSS_LOAD_INTEGRATOR_WR_FIELD_QBSS_LOAD_INTEGRATOR_WR_MASK                                     0x0FFFFFFF
//========================================
/* REG_PAC_TIM_QBSS_LOAD_INTEGRATOR_RD 0x8C */
#define B0_PAC_TIM_QBSS_LOAD_INTEGRATOR_RD_FIELD_QBSS_LOAD_INTEGRATOR_RD_MASK                                     0x0FFFFFFF
//========================================
/* REG_PAC_TIM_REDUCED_FREQ_CHANGE_LATCHED_TSF_TIMER_LOW 0x90 */
#define B0_PAC_TIM_REDUCED_FREQ_CHANGE_LATCHED_TSF_TIMER_LOW_FIELD_REDUCED_FREQ_CHANGE_LATCHED_TSF_TIMER_LOW_MASK 0xFFFFFFFF
//========================================
/* REG_PAC_TIM_RX_DUR_TIMER 0x98 */
#define B0_PAC_TIM_RX_DUR_TIMER_FIELD_RX_DUR_TIMER_MASK                                                           0x000000FF
//========================================
/* REG_PAC_TIM_DELIA_TIMER 0x9C */
#define B0_PAC_TIM_DELIA_TIMER_FIELD_DELIA_TIMER_MASK                                                             0x00FFFFFF
//========================================
/* REG_PAC_TIM_DELIA_MAX_LIMIT_TIMER 0xA0 */
#define B0_PAC_TIM_DELIA_MAX_LIMIT_TIMER_FIELD_DELIA_MAX_LIMIT_TIMER_MASK                                         0x00FFFFFF
//========================================
/* REG_PAC_TIM_TXH_PAUSER_REP_TIMER 0xA4 */
#define B0_PAC_TIM_TXH_PAUSER_REP_TIMER_FIELD_TXH_PAUSER_REP_TIMER_MASK                                           0x0000FFFF
//========================================
/* REG_PAC_TIM_TX_NAV_TIMER_CFG_SET_VALUE 0xB0 */
#define B0_PAC_TIM_TX_NAV_TIMER_CFG_SET_VALUE_FIELD_TX_NAV_TIMER_CFG_SET_VALUE_MASK                               0x0003FFFF
//========================================
/* REG_PAC_TIM_TX_NAV_TIMER 0xB4 */
#define B0_PAC_TIM_TX_NAV_TIMER_FIELD_TX_NAV_TIMER_MASK                                                           0x0003FFFF
//========================================
/* REG_PAC_TIM_TX_NAV_TIMER_IFS_COMPENSATION 0xB8 */
#define B0_PAC_TIM_TX_NAV_TIMER_IFS_COMPENSATION_FIELD_TX_NAV_IFS_COMPENSATION_MASK                               0x0000FFFF
#define B0_PAC_TIM_TX_NAV_TIMER_IFS_COMPENSATION_FIELD_REDUCED_FREQ_TX_NAV_IFS_COMPENSATION_MASK                  0xFFFF0000
//========================================
/* REG_PAC_TIM_TIMER_CONTROL2 0xBC */
#define B0_PAC_TIM_TIMER_CONTROL2_FIELD_INTEGRATE_TX_TIME_TO_QBSS2_MASK                                           0x00000001
#define B0_PAC_TIM_TIMER_CONTROL2_FIELD_INTEGRATE_NAV_TIME_TO_QBSS2_MASK                                          0x00000002
#define B0_PAC_TIM_TIMER_CONTROL2_FIELD_INTEGRATE_CCA_TIME_TO_QBSS2_MASK                                          0x00000004
//========================================
/* REG_PAC_TIM_QBSS2_LOAD_INTEGRATOR_WR 0xC0 */
#define B0_PAC_TIM_QBSS2_LOAD_INTEGRATOR_WR_FIELD_QBSS2_LOAD_INTEGRATOR_WR_MASK                                   0x0FFFFFFF
//========================================
/* REG_PAC_TIM_QBSS2_LOAD_INTEGRATOR_RD 0xC4 */
#define B0_PAC_TIM_QBSS2_LOAD_INTEGRATOR_RD_FIELD_QBSS2_LOAD_INTEGRATOR_RD_MASK                                   0x0FFFFFFF
//========================================
/* REG_PAC_TIM_TIMER_CONTROL3 0xC8 */
#define B0_PAC_TIM_TIMER_CONTROL3_FIELD_INTEGRATE_TX_TIME_TO_QBSS3_MASK                                           0x00000001
#define B0_PAC_TIM_TIMER_CONTROL3_FIELD_INTEGRATE_NAV_TIME_TO_QBSS3_MASK                                          0x00000002
#define B0_PAC_TIM_TIMER_CONTROL3_FIELD_INTEGRATE_CCA_TIME_TO_QBSS3_MASK                                          0x00000004
//========================================
/* REG_PAC_TIM_QBSS3_LOAD_INTEGRATOR_WR 0xCC */
#define B0_PAC_TIM_QBSS3_LOAD_INTEGRATOR_WR_FIELD_QBSS3_LOAD_INTEGRATOR_WR_MASK                                   0x0FFFFFFF
//========================================
/* REG_PAC_TIM_QBSS3_LOAD_INTEGRATOR_RD 0xE0 */
#define B0_PAC_TIM_QBSS3_LOAD_INTEGRATOR_RD_FIELD_QBSS3_LOAD_INTEGRATOR_RD_MASK                                   0x0FFFFFFF
//========================================
/* REG_PAC_TIM_TIMER_CONTROL4 0xE4 */
#define B0_PAC_TIM_TIMER_CONTROL4_FIELD_INTEGRATE_TX_TIME_TO_QBSS4_MASK                                           0x00000001
#define B0_PAC_TIM_TIMER_CONTROL4_FIELD_INTEGRATE_NAV_TIME_TO_QBSS4_MASK                                          0x00000002
#define B0_PAC_TIM_TIMER_CONTROL4_FIELD_INTEGRATE_CCA_TIME_TO_QBSS4_MASK                                          0x00000004
//========================================
/* REG_PAC_TIM_QBSS4_LOAD_INTEGRATOR_WR 0xE8 */
#define B0_PAC_TIM_QBSS4_LOAD_INTEGRATOR_WR_FIELD_QBSS4_LOAD_INTEGRATOR_WR_MASK                                   0x0FFFFFFF
//========================================
/* REG_PAC_TIM_BSSS4_LOAD_INTEGRATOR_RD 0xEC */
#define B0_PAC_TIM_BSSS4_LOAD_INTEGRATOR_RD_FIELD_QBSS4_LOAD_INTEGRATOR_RD_MASK                                   0x0FFFFFFF


#endif // _PAC_TIM_REGS_H_
