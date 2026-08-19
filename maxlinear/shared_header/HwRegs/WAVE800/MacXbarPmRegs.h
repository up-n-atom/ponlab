
/***********************************************************************************
File:				MacXbarPmRegs.h
Module:				macXbarPm
SOC Revision:		latest
Generated at:       2024-06-26 12:55:07
Purpose:
Description:		This File was auto generated using SOC Online

************************************************************************************/
#ifndef _MAC_XBAR_PM_REGS_H_
#define _MAC_XBAR_PM_REGS_H_

/*---------------------------------------------------------------------------------
/						Registers Addresses													 
/----------------------------------------------------------------------------------*/
#include "HwMemoryMap.h"

#define MAC_XBAR_PM_BASE_ADDRESS                                  MEMORY_MAP_UNIT_1116_BASE_ADDRESS
#define	REG_MAC_XBAR_PM_PROBE_MAINCTL                               (MAC_XBAR_PM_BASE_ADDRESS + 0x8)
#define	REG_MAC_XBAR_PM_PROBE_CFGCTL                                (MAC_XBAR_PM_BASE_ADDRESS + 0xC)
#define	REG_MAC_XBAR_PM_PROBE_STATPERIOD                            (MAC_XBAR_PM_BASE_ADDRESS + 0x24)
#define	REG_MAC_XBAR_PM_PROBE_STATGO                                (MAC_XBAR_PM_BASE_ADDRESS + 0x28)
#define	REG_MAC_XBAR_PM_PROBE_COUNTERS_0_SRC                        (MAC_XBAR_PM_BASE_ADDRESS + 0x204)
#define	REG_MAC_XBAR_PM_PROBE_COUNTERS_0_VAL                        (MAC_XBAR_PM_BASE_ADDRESS + 0x20C)
#define	REG_MAC_XBAR_PM_PROBE_COUNTERS_1_SRC                        (MAC_XBAR_PM_BASE_ADDRESS + 0x214)
#define	REG_MAC_XBAR_PM_PROBE_COUNTERS_1_VAL                        (MAC_XBAR_PM_BASE_ADDRESS + 0x21C)
#define	REG_MAC_XBAR_PM_PROBE_COUNTERS_2_SRC                        (MAC_XBAR_PM_BASE_ADDRESS + 0x224)
#define	REG_MAC_XBAR_PM_PROBE_COUNTERS_2_VAL                        (MAC_XBAR_PM_BASE_ADDRESS + 0x22C)
#define	REG_MAC_XBAR_PM_PROBE_COUNTERS_3_SRC                        (MAC_XBAR_PM_BASE_ADDRESS + 0x234)
#define	REG_MAC_XBAR_PM_PROBE_COUNTERS_3_VAL                        (MAC_XBAR_PM_BASE_ADDRESS + 0x23C)
#define	REG_MAC_XBAR_PM_TRANSACTIONSTATPROFILER_EN                  (MAC_XBAR_PM_BASE_ADDRESS + 0x408)
#define	REG_MAC_XBAR_PM_TRANSACTIONSTATPROFILER_MODE                (MAC_XBAR_PM_BASE_ADDRESS + 0x40C)
#define	REG_MAC_XBAR_PM_TRANSACTIONSTATPROFILER_THRESHOLDS_0_0      (MAC_XBAR_PM_BASE_ADDRESS + 0x42C)
#define	REG_MAC_XBAR_PM_TRANSACTIONSTATPROFILER_THRESHOLDS_0_1      (MAC_XBAR_PM_BASE_ADDRESS + 0x430)
#define	REG_MAC_XBAR_PM_TRANSACTIONSTATPROFILER_THRESHOLDS_0_2      (MAC_XBAR_PM_BASE_ADDRESS + 0x434)
#define	REG_MAC_XBAR_PM_TRANSACTIONSTATPROFILER_OVERFLOWSTATUS      (MAC_XBAR_PM_BASE_ADDRESS + 0x46C)
#define	REG_MAC_XBAR_PM_TRANSACTIONSTATPROFILER_OVERFLOWRESET       (MAC_XBAR_PM_BASE_ADDRESS + 0x470)
#define	REG_MAC_XBAR_PM_TRANSACTIONSTATPROFILER_PENDINGEVENTMODE    (MAC_XBAR_PM_BASE_ADDRESS + 0x474)
#define	REG_MAC_XBAR_PM_TRANSACTIONSTATPROFILER_PRESCALER           (MAC_XBAR_PM_BASE_ADDRESS + 0x478)
#define	REG_MAC_XBAR_PM_TRANSACTIONSTATFILTER_MODE                  (MAC_XBAR_PM_BASE_ADDRESS + 0x608)
#define	REG_MAC_XBAR_PM_TRANSACTIONSTATFILTER_ADDRBASE_LOW          (MAC_XBAR_PM_BASE_ADDRESS + 0x60C)
#define	REG_MAC_XBAR_PM_TRANSACTIONSTATFILTER_ADDRWINDOWSIZE        (MAC_XBAR_PM_BASE_ADDRESS + 0x614)
#define	REG_MAC_XBAR_PM_TRANSACTIONSTATFILTER_OPCODE                (MAC_XBAR_PM_BASE_ADDRESS + 0x620)
/*---------------------------------------------------------------------------------
/						Data Type Definition										
/----------------------------------------------------------------------------------*/
/*REG_MAC_XBAR_PM_PROBE_MAINCTL 0x8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 erren : 1; //Register field ErrEn enables the probe to send on the ObsTx output any packet with Error status, independently of filtering mechanisms, thus constituting a simple supplementary global filter., reset value: 0x0, access type: RW
		uint32 traceen : 1; //Register field TraceEn enables the probe to send filtered packets (Trace) on the ObsTx observation output., reset value: 0x0, access type: RO
		uint32 payloaden : 1; //Register field PayloadEn, when set to 1, enables traces to contain headers and payload. When set ot 0, only headers are reported., reset value: 0x0, access type: RW
		uint32 staten : 1; //When set to 1, register field StatEn enables statistics profiling. The probe sendS statistics results to the output for signal ObsTx. All statistics counters are cleared when the StatEn bit goes from 0 to 1. When set to 0, counters are disabled., reset value: 0x0, access type: RW
		uint32 alarmen : 1; //When set, register field AlarmEn enables the probe to collect alarm-related information. When the register field bit is null, both TraceAlarm and StatAlarm outputs are driven to 0., reset value: 0x0, access type: RW
		uint32 statconddump : 1; //When set, register field StatCondDump enables the dump of a statistics frame to the range of counter values set for registers StatAlarmMin, StatAlarmMax, and AlarmMode. This field also renders register StatAlarmStatus inoperative. When parameter statisticsCounterAlarm is set to False, the StatCondDump register bit is reserved., reset value: 0x0, access type: RW
		uint32 intrusivemode : 1; //When set to 1, register field IntrusiveMode enables trace operation in Intrusive flow-control mode. When set to 0, the register  enables trace operation in Overflow flow-control mode, reset value: 0x0, access type: RO
		uint32 filtbytealwayschainableen : 1; //When set to 0, filters are mapped to all statistic counters when counting bytes or enabled bytes. Therefore, only filter events mapped to even counters can be counted using a pair of chained counters.When set to 1, filters are mapped only to even statistic counters when counting bytes or enabled bytes. Thus events from any filter can be counted using a pair of chained counters., reset value: 0x0, access type: RW
		uint32 reserved0 : 24;
	} bitFields;
} RegMacXbarPmProbeMainctl_u;

/*REG_MAC_XBAR_PM_PROBE_CFGCTL 0xC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 globalen : 1; //Enables or disables the tracing and statistics collection subsystems of the packet probe., reset value: 0x0, access type: RW
		uint32 active : 1; //Active status of the packet probe., reset value: 0x0, access type: RO
		uint32 reserved0 : 30;
	} bitFields;
} RegMacXbarPmProbeCfgctl_u;

/*REG_MAC_XBAR_PM_PROBE_STATPERIOD 0x24 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 statperiod : 5; //Register StatPeriod is a 5-bit register that sets a period, within a range of 2 cycles to 2 gigacycles, during which statistics are collected before being dumped automatically. Setting the register implicitly enables automatic mode operation for statistics collection. The period is calculated with the formula: N_Cycle = 2**StatPeriodWhen register StatPeriod is set to its default value 0, automatic dump mode is disabled, and register StatGo is activated for manual mode operation. Note: When parameter statisticsCollection is set to False, StatPeriod is reserved., reset value: 0x0, access type: RW
		uint32 reserved0 : 27;
	} bitFields;
} RegMacXbarPmProbeStatperiod_u;

/*REG_MAC_XBAR_PM_PROBE_STATGO 0x28 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 statgo : 1; //Writing a 1 to the 1-bit pulse register StatGo generates a statistics dump.The register is active when statistics collection operates in manual mode, that is, when register StatPeriod is set to 0.NOTE  The written value is not stored in StatGo. A read always returns 0., reset value: 0x0, access type: WO
		uint32 reserved0 : 31;
	} bitFields;
} RegMacXbarPmProbeStatgo_u;

/*REG_MAC_XBAR_PM_PROBE_COUNTERS_0_SRC 0x204 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 counters0Intevent : 5; //Internal packet event, reset value: 0x0, access type: RW
		uint32 counters0Extevent : 1; //When greater than 0, the entire register value is used to select an external hardware event source. The index of the external event is equal to {ExtEvent,IntEvent}., reset value: 0x0, access type: RW
		uint32 reserved0 : 26;
	} bitFields;
} RegMacXbarPmProbeCounters0Src_u;

/*REG_MAC_XBAR_PM_PROBE_COUNTERS_0_VAL 0x20C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 counters0Val : 32; //Register Val is a read-only register that is always present. The register containsthe statistics counter value either pending StatAlarm output, or when statisticscollection is suspended subsequent to triggers or signal statSuspend., reset value: 0x0, access type: RO
	} bitFields;
} RegMacXbarPmProbeCounters0Val_u;

/*REG_MAC_XBAR_PM_PROBE_COUNTERS_1_SRC 0x214 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 counters1Intevent : 5; //Internal packet event, reset value: 0x0, access type: RW
		uint32 counters1Extevent : 1; //When greater than 0, the entire register value is used to select an external hardware event source. The index of the external event is equal to {ExtEvent,IntEvent}., reset value: 0x0, access type: RW
		uint32 reserved0 : 26;
	} bitFields;
} RegMacXbarPmProbeCounters1Src_u;

/*REG_MAC_XBAR_PM_PROBE_COUNTERS_1_VAL 0x21C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 counters1Val : 32; //Register Val is a read-only register that is always present. The register containsthe statistics counter value either pending StatAlarm output, or when statisticscollection is suspended subsequent to triggers or signal statSuspend., reset value: 0x0, access type: RO
	} bitFields;
} RegMacXbarPmProbeCounters1Val_u;

/*REG_MAC_XBAR_PM_PROBE_COUNTERS_2_SRC 0x224 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 counters2Intevent : 5; //Internal packet event, reset value: 0x0, access type: RW
		uint32 counters2Extevent : 1; //When greater than 0, the entire register value is used to select an external hardware event source. The index of the external event is equal to {ExtEvent,IntEvent}., reset value: 0x0, access type: RW
		uint32 reserved0 : 26;
	} bitFields;
} RegMacXbarPmProbeCounters2Src_u;

/*REG_MAC_XBAR_PM_PROBE_COUNTERS_2_VAL 0x22C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 counters2Val : 32; //Register Val is a read-only register that is always present. The register containsthe statistics counter value either pending StatAlarm output, or when statisticscollection is suspended subsequent to triggers or signal statSuspend., reset value: 0x0, access type: RO
	} bitFields;
} RegMacXbarPmProbeCounters2Val_u;

/*REG_MAC_XBAR_PM_PROBE_COUNTERS_3_SRC 0x234 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 counters3Intevent : 5; //Internal packet event, reset value: 0x0, access type: RW
		uint32 counters3Extevent : 1; //When greater than 0, the entire register value is used to select an external hardware event source. The index of the external event is equal to {ExtEvent,IntEvent}., reset value: 0x0, access type: RW
		uint32 reserved0 : 26;
	} bitFields;
} RegMacXbarPmProbeCounters3Src_u;

/*REG_MAC_XBAR_PM_PROBE_COUNTERS_3_VAL 0x23C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 counters3Val : 32; //Register Val is a read-only register that is always present. The register containsthe statistics counter value either pending StatAlarm output, or when statisticscollection is suspended subsequent to triggers or signal statSuspend., reset value: 0x0, access type: RO
	} bitFields;
} RegMacXbarPmProbeCounters3Val_u;

/*REG_MAC_XBAR_PM_TRANSACTIONSTATPROFILER_EN 0x408 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tspEn : 1; //Register En is a 1-bit register that enables the transaction probe counter unit., reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegMacXbarPmTransactionstatprofilerEn_u;

/*REG_MAC_XBAR_PM_TRANSACTIONSTATPROFILER_MODE 0x40C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tspMode : 1; //Register Mode sets the counting mode per observed port. Each bit per observation port defines the incrementing mode. (Mode = 0 for Delay, Mode = 1 for Pending), reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegMacXbarPmTransactionstatprofilerMode_u;

/*REG_MAC_XBAR_PM_TRANSACTIONSTATPROFILER_THRESHOLDS_0_0 0x42C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tspThresholds00 : 11; //Register Thresholds_i_j contains the threshold index "0" that allows computation of threshold values., reset value: 0x0, access type: RW
		uint32 reserved0 : 21;
	} bitFields;
} RegMacXbarPmTransactionstatprofilerThresholds00_u;

/*REG_MAC_XBAR_PM_TRANSACTIONSTATPROFILER_THRESHOLDS_0_1 0x430 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tspThresholds01 : 11; //Register Thresholds_i_j contains the threshold index "1" that allows computation of threshold values., reset value: 0x0, access type: RW
		uint32 reserved0 : 21;
	} bitFields;
} RegMacXbarPmTransactionstatprofilerThresholds01_u;

/*REG_MAC_XBAR_PM_TRANSACTIONSTATPROFILER_THRESHOLDS_0_2 0x434 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tspThresholds02 : 11; //Register Thresholds_i_j contains the threshold index "2" that allows computation of threshold values., reset value: 0x0, access type: RW
		uint32 reserved0 : 21;
	} bitFields;
} RegMacXbarPmTransactionstatprofilerThresholds02_u;

/*REG_MAC_XBAR_PM_TRANSACTIONSTATPROFILER_OVERFLOWSTATUS 0x46C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tspOverflowstatus : 1; //Bit n of register OverflowStatus is set to 1 if a start event occurs on observed port n and eitherof the following conditions occurs: All tenure counters allocated to the port are already in use. No tenure lines have been allocated to the port. The number of bits in this register is equal to the value set for parameter nObservable., reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegMacXbarPmTransactionstatprofilerOverflowstatus_u;

/*REG_MAC_XBAR_PM_TRANSACTIONSTATPROFILER_OVERFLOWRESET 0x470 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tspOverflowreset : 1; //Register OverflowReset is a pulse register that clears overflow status bits per observed port on each write access. OverflowReset = nObservable. Writing 0x2  clears the overflow status of observed port 1., reset value: 0x0, access type: WO
		uint32 reserved0 : 31;
	} bitFields;
} RegMacXbarPmTransactionstatprofilerOverflowreset_u;

/*REG_MAC_XBAR_PM_TRANSACTIONSTATPROFILER_PENDINGEVENTMODE 0x474 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tspPendingeventmode : 1; //Register pendingEventMode is a 1-bit register that configures the pending event mode. When set to 0 (CYCLE), and when register mode is set to PENDING, the pending event is generated on each cycle when the counter is greater than zero.When set to 1 (STOP) the pending event is generated on each stop event., reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegMacXbarPmTransactionstatprofilerPendingeventmode_u;

/*REG_MAC_XBAR_PM_TRANSACTIONSTATPROFILER_PRESCALER 0x478 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tspPrescaler : 8; //8Register Prescaler is an-bit pre-scaling register that accepts any pre-scaling value between 1 (default) and 256. If set to 0, pre-scaling is disabled. If set to any other supported value "n", the threshold counter value is divided by (n + 1)., reset value: 0x0, access type: RW
		uint32 reserved0 : 24;
	} bitFields;
} RegMacXbarPmTransactionstatprofilerPrescaler_u;

/*REG_MAC_XBAR_PM_TRANSACTIONSTATFILTER_MODE 0x608 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tsfMode : 1; //Register Mode is a 1-bit register that sets the filtering mode as follows: handshake Mode = 0 or latency Mode = 1., reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegMacXbarPmTransactionstatfilterMode_u;

/*REG_MAC_XBAR_PM_TRANSACTIONSTATFILTER_ADDRBASE_LOW 0x60C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tsfAddrbaseLow : 32; //Address base LSB register., reset value: 0x0, access type: RW
	} bitFields;
} RegMacXbarPmTransactionstatfilterAddrbaseLow_u;

/*REG_MAC_XBAR_PM_TRANSACTIONSTATFILTER_ADDRWINDOWSIZE 0x614 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tsfAddrwindowsize : 6; //Register AddrWindowSize contains the encoded address mask used to filter packets: the effective Mask value is equal to ~(2**AddrWindowSize - 1). A packet is a candidate when ReqInfo.Addr & AddrMask = AddrBase & AddrMask., reset value: 0x0, access type: RW
		uint32 reserved0 : 26;
	} bitFields;
} RegMacXbarPmTransactionstatfilterAddrwindowsize_u;

/*REG_MAC_XBAR_PM_TRANSACTIONSTATFILTER_OPCODE 0x620 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tsfRden : 1; //When set to 1, selects RD requests., reset value: 0x0, access type: RW
		uint32 tsfWren : 1; //When set to 1, selects WR requests., reset value: 0x0, access type: RW
		uint32 reserved0 : 30;
	} bitFields;
} RegMacXbarPmTransactionstatfilterOpcode_u;

//========================================
/* REG_MAC_XBAR_PM_PROBE_MAINCTL 0x8 */
#define MAC_XBAR_PM_PROBE_MAINCTL_FIELD_ERREN_MASK                                                                0x00000001
#define MAC_XBAR_PM_PROBE_MAINCTL_FIELD_TRACEEN_MASK                                                              0x00000002
#define MAC_XBAR_PM_PROBE_MAINCTL_FIELD_PAYLOADEN_MASK                                                            0x00000004
#define MAC_XBAR_PM_PROBE_MAINCTL_FIELD_STATEN_MASK                                                               0x00000008
#define MAC_XBAR_PM_PROBE_MAINCTL_FIELD_ALARMEN_MASK                                                              0x00000010
#define MAC_XBAR_PM_PROBE_MAINCTL_FIELD_STATCONDDUMP_MASK                                                         0x00000020
#define MAC_XBAR_PM_PROBE_MAINCTL_FIELD_INTRUSIVEMODE_MASK                                                        0x00000040
#define MAC_XBAR_PM_PROBE_MAINCTL_FIELD_FILTBYTEALWAYSCHAINABLEEN_MASK                                            0x00000080
//========================================
/* REG_MAC_XBAR_PM_PROBE_CFGCTL 0xC */
#define MAC_XBAR_PM_PROBE_CFGCTL_FIELD_GLOBALEN_MASK                                                              0x00000001
#define MAC_XBAR_PM_PROBE_CFGCTL_FIELD_ACTIVE_MASK                                                                0x00000002
//========================================
/* REG_MAC_XBAR_PM_PROBE_STATPERIOD 0x24 */
#define MAC_XBAR_PM_PROBE_STATPERIOD_FIELD_STATPERIOD_MASK                                                        0x0000001F
//========================================
/* REG_MAC_XBAR_PM_PROBE_STATGO 0x28 */
#define MAC_XBAR_PM_PROBE_STATGO_FIELD_STATGO_MASK                                                                0x00000001
//========================================
/* REG_MAC_XBAR_PM_PROBE_COUNTERS_0_SRC 0x204 */
#define MAC_XBAR_PM_PROBE_COUNTERS_0_SRC_FIELD_COUNTERS_0_INTEVENT_MASK                                           0x0000001F
#define MAC_XBAR_PM_PROBE_COUNTERS_0_SRC_FIELD_COUNTERS_0_EXTEVENT_MASK                                           0x00000020
//========================================
/* REG_MAC_XBAR_PM_PROBE_COUNTERS_0_VAL 0x20C */
#define MAC_XBAR_PM_PROBE_COUNTERS_0_VAL_FIELD_COUNTERS_0_VAL_MASK                                                0xFFFFFFFF
//========================================
/* REG_MAC_XBAR_PM_PROBE_COUNTERS_1_SRC 0x214 */
#define MAC_XBAR_PM_PROBE_COUNTERS_1_SRC_FIELD_COUNTERS_1_INTEVENT_MASK                                           0x0000001F
#define MAC_XBAR_PM_PROBE_COUNTERS_1_SRC_FIELD_COUNTERS_1_EXTEVENT_MASK                                           0x00000020
//========================================
/* REG_MAC_XBAR_PM_PROBE_COUNTERS_1_VAL 0x21C */
#define MAC_XBAR_PM_PROBE_COUNTERS_1_VAL_FIELD_COUNTERS_1_VAL_MASK                                                0xFFFFFFFF
//========================================
/* REG_MAC_XBAR_PM_PROBE_COUNTERS_2_SRC 0x224 */
#define MAC_XBAR_PM_PROBE_COUNTERS_2_SRC_FIELD_COUNTERS_2_INTEVENT_MASK                                           0x0000001F
#define MAC_XBAR_PM_PROBE_COUNTERS_2_SRC_FIELD_COUNTERS_2_EXTEVENT_MASK                                           0x00000020
//========================================
/* REG_MAC_XBAR_PM_PROBE_COUNTERS_2_VAL 0x22C */
#define MAC_XBAR_PM_PROBE_COUNTERS_2_VAL_FIELD_COUNTERS_2_VAL_MASK                                                0xFFFFFFFF
//========================================
/* REG_MAC_XBAR_PM_PROBE_COUNTERS_3_SRC 0x234 */
#define MAC_XBAR_PM_PROBE_COUNTERS_3_SRC_FIELD_COUNTERS_3_INTEVENT_MASK                                           0x0000001F
#define MAC_XBAR_PM_PROBE_COUNTERS_3_SRC_FIELD_COUNTERS_3_EXTEVENT_MASK                                           0x00000020
//========================================
/* REG_MAC_XBAR_PM_PROBE_COUNTERS_3_VAL 0x23C */
#define MAC_XBAR_PM_PROBE_COUNTERS_3_VAL_FIELD_COUNTERS_3_VAL_MASK                                                0xFFFFFFFF
//========================================
/* REG_MAC_XBAR_PM_TRANSACTIONSTATPROFILER_EN 0x408 */
#define MAC_XBAR_PM_TRANSACTIONSTATPROFILER_EN_FIELD_TSP_EN_MASK                                                  0x00000001
//========================================
/* REG_MAC_XBAR_PM_TRANSACTIONSTATPROFILER_MODE 0x40C */
#define MAC_XBAR_PM_TRANSACTIONSTATPROFILER_MODE_FIELD_TSP_MODE_MASK                                              0x00000001
//========================================
/* REG_MAC_XBAR_PM_TRANSACTIONSTATPROFILER_THRESHOLDS_0_0 0x42C */
#define MAC_XBAR_PM_TRANSACTIONSTATPROFILER_THRESHOLDS_0_0_FIELD_TSP_THRESHOLDS_0_0_MASK                          0x000007FF
//========================================
/* REG_MAC_XBAR_PM_TRANSACTIONSTATPROFILER_THRESHOLDS_0_1 0x430 */
#define MAC_XBAR_PM_TRANSACTIONSTATPROFILER_THRESHOLDS_0_1_FIELD_TSP_THRESHOLDS_0_1_MASK                          0x000007FF
//========================================
/* REG_MAC_XBAR_PM_TRANSACTIONSTATPROFILER_THRESHOLDS_0_2 0x434 */
#define MAC_XBAR_PM_TRANSACTIONSTATPROFILER_THRESHOLDS_0_2_FIELD_TSP_THRESHOLDS_0_2_MASK                          0x000007FF
//========================================
/* REG_MAC_XBAR_PM_TRANSACTIONSTATPROFILER_OVERFLOWSTATUS 0x46C */
#define MAC_XBAR_PM_TRANSACTIONSTATPROFILER_OVERFLOWSTATUS_FIELD_TSP_OVERFLOWSTATUS_MASK                          0x00000001
//========================================
/* REG_MAC_XBAR_PM_TRANSACTIONSTATPROFILER_OVERFLOWRESET 0x470 */
#define MAC_XBAR_PM_TRANSACTIONSTATPROFILER_OVERFLOWRESET_FIELD_TSP_OVERFLOWRESET_MASK                            0x00000001
//========================================
/* REG_MAC_XBAR_PM_TRANSACTIONSTATPROFILER_PENDINGEVENTMODE 0x474 */
#define MAC_XBAR_PM_TRANSACTIONSTATPROFILER_PENDINGEVENTMODE_FIELD_TSP_PENDINGEVENTMODE_MASK                      0x00000001
//========================================
/* REG_MAC_XBAR_PM_TRANSACTIONSTATPROFILER_PRESCALER 0x478 */
#define MAC_XBAR_PM_TRANSACTIONSTATPROFILER_PRESCALER_FIELD_TSP_PRESCALER_MASK                                    0x000000FF
//========================================
/* REG_MAC_XBAR_PM_TRANSACTIONSTATFILTER_MODE 0x608 */
#define MAC_XBAR_PM_TRANSACTIONSTATFILTER_MODE_FIELD_TSF_MODE_MASK                                                0x00000001
//========================================
/* REG_MAC_XBAR_PM_TRANSACTIONSTATFILTER_ADDRBASE_LOW 0x60C */
#define MAC_XBAR_PM_TRANSACTIONSTATFILTER_ADDRBASE_LOW_FIELD_TSF_ADDRBASE_LOW_MASK                                0xFFFFFFFF
//========================================
/* REG_MAC_XBAR_PM_TRANSACTIONSTATFILTER_ADDRWINDOWSIZE 0x614 */
#define MAC_XBAR_PM_TRANSACTIONSTATFILTER_ADDRWINDOWSIZE_FIELD_TSF_ADDRWINDOWSIZE_MASK                            0x0000003F
//========================================
/* REG_MAC_XBAR_PM_TRANSACTIONSTATFILTER_OPCODE 0x620 */
#define MAC_XBAR_PM_TRANSACTIONSTATFILTER_OPCODE_FIELD_TSF_RDEN_MASK                                              0x00000001
#define MAC_XBAR_PM_TRANSACTIONSTATFILTER_OPCODE_FIELD_TSF_WREN_MASK                                              0x00000002


#endif // _MAC_XBAR_PM_REGS_H_
