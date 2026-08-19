
/***********************************************************************************
File:				B2TxhNtdRegs.h
Module:				b2TxhNtd
SOC Revision:		latest
Generated at:       2024-06-26 12:55:01
Purpose:
Description:		This File was auto generated using SOC Online

************************************************************************************/
#ifndef _B2_TXH_NTD_REGS_H_
#define _B2_TXH_NTD_REGS_H_

/*---------------------------------------------------------------------------------
/						Registers Addresses													 
/----------------------------------------------------------------------------------*/
#include "HwMemoryMap.h"

#define B2_TXH_NTD_BASE_ADDRESS            MEMORY_MAP_UNIT_20054_BASE_ADDRESS
#define	REG_B2_TXH_NTD_SW_REQ_TCS_PART0     (B2_TXH_NTD_BASE_ADDRESS + 0x60)
#define	REG_B2_TXH_NTD_SW_REQ_TCS_PART1     (B2_TXH_NTD_BASE_ADDRESS + 0x64)
#define	REG_B2_TXH_NTD_SW_REQ_SET_CLEAR     (B2_TXH_NTD_BASE_ADDRESS + 0x68)
#define	REG_B2_TXH_NTD_SW_REQ_STATUS        (B2_TXH_NTD_BASE_ADDRESS + 0x6C)
#define	REG_B2_TXH_NTD_SW_REQ_IRQ_ENABLE    (B2_TXH_NTD_BASE_ADDRESS + 0x70)
#define	REG_B2_TXH_NTD_SW_REQ_IRQ_CLEAR     (B2_TXH_NTD_BASE_ADDRESS + 0x74)
#define	REG_B2_TXH_NTD_SW_REQ_IRQ_STATUS    (B2_TXH_NTD_BASE_ADDRESS + 0x78)
#define	REG_B2_TXH_NTD_NTD_ARBITER_DEBUG    (B2_TXH_NTD_BASE_ADDRESS + 0x80)
/*---------------------------------------------------------------------------------
/						Data Type Definition										
/----------------------------------------------------------------------------------*/
/*REG_B2_TXH_NTD_SW_REQ_TCS_PART0 0x60 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swReqRecipePtr : 26; //SW request recipe pointer, reset value: 0x0, access type: RW
		uint32 swReqCcaAbort : 1; //SW request CCA primary abort, reset value: 0x0, access type: RW
		uint32 swReqRxNavAbort : 1; //SW request Rx NAV abort, reset value: 0x0, access type: RW
		uint32 swReqTxNavAbort : 1; //SW request Tx NAV abort, reset value: 0x0, access type: RW
		uint32 swReqCcaSensitive : 1; //SW request CCA primary sensitive, reset value: 0x0, access type: RW
		uint32 swReqRxNavSensitive : 1; //SW request Rx NAV sensitive, reset value: 0x0, access type: RW
		uint32 swReqTxNavSensitive : 1; //SW request Tx NAV sensitive, reset value: 0x0, access type: RW
	} bitFields;
} RegB2TxhNtdSwReqTcsPart0_u;

/*REG_B2_TXH_NTD_SW_REQ_TCS_PART1 0x64 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swReqIfsn : 4; //SW request IFSN, reset value: 0x0, access type: RW
		uint32 reserved0 : 4;
		uint32 swReqBackoff : 15; //SW request Backoff, reset value: 0x0, access type: RW
		uint32 reserved1 : 7;
		uint32 swReqIrqClient : 2; //SW request IRQ client, reset value: 0x0, access type: RW
	} bitFields;
} RegB2TxhNtdSwReqTcsPart1_u;

/*REG_B2_TXH_NTD_SW_REQ_SET_CLEAR 0x68 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swReqSet : 1; //SW request set, reset value: 0x0, access type: WO
		uint32 swReqClear : 1; //SW request Clear, reset value: 0x0, access type: WO
		uint32 reserved0 : 30;
	} bitFields;
} RegB2TxhNtdSwReqSetClear_u;

/*REG_B2_TXH_NTD_SW_REQ_STATUS 0x6C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swReqTxReq : 1; //SW request Tx request, reset value: 0x0, access type: RO
		uint32 swReqPendingClrTxReq : 1; //SW request pending clear Tx request, reset value: 0x0, access type: RO
		uint32 reserved0 : 2;
		uint32 swReqCurrIfsn : 4; //SW request current IFSN, reset value: 0x0, access type: RO
		uint32 swReqCurrBackoff : 15; //SW request current Backoff, reset value: 0x0, access type: RO
		uint32 reserved1 : 9;
	} bitFields;
} RegB2TxhNtdSwReqStatus_u;

/*REG_B2_TXH_NTD_SW_REQ_IRQ_ENABLE 0x70 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swReqIrqEnable : 5; //SW request IRQ enable, reset value: 0x1f, access type: RW
		uint32 reserved0 : 27;
	} bitFields;
} RegB2TxhNtdSwReqIrqEnable_u;

/*REG_B2_TXH_NTD_SW_REQ_IRQ_CLEAR 0x74 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swReqIrqClear : 5; //SW request IRQ clear, reset value: 0x0, access type: WO
		uint32 reserved0 : 27;
	} bitFields;
} RegB2TxhNtdSwReqIrqClear_u;

/*REG_B2_TXH_NTD_SW_REQ_IRQ_STATUS 0x78 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swReqTxDoneIrq : 1; //SW request Transmission done, reset value: 0x0, access type: RO
		uint32 swReqClearDoneIrq : 1; //SW request Clear done, reset value: 0x0, access type: RO
		uint32 swReqCcaAbortDoneIrq : 1; //SW request CCA primary abort done, reset value: 0x0, access type: RO
		uint32 swReqRxNavAbortDoneIrq : 1; //SW request Rx NAV abort done, reset value: 0x0, access type: RO
		uint32 swReqTxNavAbortDoneIrq : 1; //SW request Tx NAV abort done, reset value: 0x0, access type: RO
		uint32 reserved0 : 27;
	} bitFields;
} RegB2TxhNtdSwReqIrqStatus_u;

/*REG_B2_TXH_NTD_NTD_ARBITER_DEBUG 0x80 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ntdArbLastClient : 2; //Tx last client, reset value: 0x0, access type: RO
		uint32 reserved0 : 2;
		uint32 currAutoRespTxReq : 1; //Current Auto Response Tx request, reset value: 0x0, access type: RO
		uint32 currSequencerTxReq : 1; //Current Sequencer Tx request, reset value: 0x0, access type: RO
		uint32 currSwReqTxReq : 1; //Current SW req Tx request, reset value: 0x0, access type: RO
		uint32 currTxhMapTxReq : 1; //Current TXH MAP Tx request, reset value: 0x0, access type: RO
		uint32 reserved1 : 24;
	} bitFields;
} RegB2TxhNtdNtdArbiterDebug_u;

//========================================
/* REG_TXH_NTD_SW_REQ_TCS_PART0 0x60 */
#define B2_TXH_NTD_SW_REQ_TCS_PART0_FIELD_SW_REQ_RECIPE_PTR_MASK                                                  0x03FFFFFF
#define B2_TXH_NTD_SW_REQ_TCS_PART0_FIELD_SW_REQ_CCA_ABORT_MASK                                                   0x04000000
#define B2_TXH_NTD_SW_REQ_TCS_PART0_FIELD_SW_REQ_RX_NAV_ABORT_MASK                                                0x08000000
#define B2_TXH_NTD_SW_REQ_TCS_PART0_FIELD_SW_REQ_TX_NAV_ABORT_MASK                                                0x10000000
#define B2_TXH_NTD_SW_REQ_TCS_PART0_FIELD_SW_REQ_CCA_SENSITIVE_MASK                                               0x20000000
#define B2_TXH_NTD_SW_REQ_TCS_PART0_FIELD_SW_REQ_RX_NAV_SENSITIVE_MASK                                            0x40000000
#define B2_TXH_NTD_SW_REQ_TCS_PART0_FIELD_SW_REQ_TX_NAV_SENSITIVE_MASK                                            0x80000000
//========================================
/* REG_TXH_NTD_SW_REQ_TCS_PART1 0x64 */
#define B2_TXH_NTD_SW_REQ_TCS_PART1_FIELD_SW_REQ_IFSN_MASK                                                        0x0000000F
#define B2_TXH_NTD_SW_REQ_TCS_PART1_FIELD_SW_REQ_BACKOFF_MASK                                                     0x007FFF00
#define B2_TXH_NTD_SW_REQ_TCS_PART1_FIELD_SW_REQ_IRQ_CLIENT_MASK                                                  0xC0000000
//========================================
/* REG_TXH_NTD_SW_REQ_SET_CLEAR 0x68 */
#define B2_TXH_NTD_SW_REQ_SET_CLEAR_FIELD_SW_REQ_SET_MASK                                                         0x00000001
#define B2_TXH_NTD_SW_REQ_SET_CLEAR_FIELD_SW_REQ_CLEAR_MASK                                                       0x00000002
//========================================
/* REG_TXH_NTD_SW_REQ_STATUS 0x6C */
#define B2_TXH_NTD_SW_REQ_STATUS_FIELD_SW_REQ_TX_REQ_MASK                                                         0x00000001
#define B2_TXH_NTD_SW_REQ_STATUS_FIELD_SW_REQ_PENDING_CLR_TX_REQ_MASK                                             0x00000002
#define B2_TXH_NTD_SW_REQ_STATUS_FIELD_SW_REQ_CURR_IFSN_MASK                                                      0x000000F0
#define B2_TXH_NTD_SW_REQ_STATUS_FIELD_SW_REQ_CURR_BACKOFF_MASK                                                   0x007FFF00
//========================================
/* REG_TXH_NTD_SW_REQ_IRQ_ENABLE 0x70 */
#define B2_TXH_NTD_SW_REQ_IRQ_ENABLE_FIELD_SW_REQ_IRQ_ENABLE_MASK                                                 0x0000001F
//========================================
/* REG_TXH_NTD_SW_REQ_IRQ_CLEAR 0x74 */
#define B2_TXH_NTD_SW_REQ_IRQ_CLEAR_FIELD_SW_REQ_IRQ_CLEAR_MASK                                                   0x0000001F
//========================================
/* REG_TXH_NTD_SW_REQ_IRQ_STATUS 0x78 */
#define B2_TXH_NTD_SW_REQ_IRQ_STATUS_FIELD_SW_REQ_TX_DONE_IRQ_MASK                                                0x00000001
#define B2_TXH_NTD_SW_REQ_IRQ_STATUS_FIELD_SW_REQ_CLEAR_DONE_IRQ_MASK                                             0x00000002
#define B2_TXH_NTD_SW_REQ_IRQ_STATUS_FIELD_SW_REQ_CCA_ABORT_DONE_IRQ_MASK                                         0x00000004
#define B2_TXH_NTD_SW_REQ_IRQ_STATUS_FIELD_SW_REQ_RX_NAV_ABORT_DONE_IRQ_MASK                                      0x00000008
#define B2_TXH_NTD_SW_REQ_IRQ_STATUS_FIELD_SW_REQ_TX_NAV_ABORT_DONE_IRQ_MASK                                      0x00000010
//========================================
/* REG_TXH_NTD_NTD_ARBITER_DEBUG 0x80 */
#define B2_TXH_NTD_NTD_ARBITER_DEBUG_FIELD_NTD_ARB_LAST_CLIENT_MASK                                               0x00000003
#define B2_TXH_NTD_NTD_ARBITER_DEBUG_FIELD_CURR_AUTO_RESP_TX_REQ_MASK                                             0x00000010
#define B2_TXH_NTD_NTD_ARBITER_DEBUG_FIELD_CURR_SEQUENCER_TX_REQ_MASK                                             0x00000020
#define B2_TXH_NTD_NTD_ARBITER_DEBUG_FIELD_CURR_SW_REQ_TX_REQ_MASK                                                0x00000040
#define B2_TXH_NTD_NTD_ARBITER_DEBUG_FIELD_CURR_TXH_MAP_TX_REQ_MASK                                               0x00000080


#endif // _TXH_NTD_REGS_H_
