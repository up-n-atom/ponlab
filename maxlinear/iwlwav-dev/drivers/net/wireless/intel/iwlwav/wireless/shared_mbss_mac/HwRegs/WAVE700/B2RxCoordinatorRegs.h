
/***********************************************************************************
File:				B2RxCoordinatorRegs.h
Module:				b2RxCoordinator
SOC Revision:		latest
Generated at:       2022-05-04 10:58:29
Purpose:
Description:		This File was auto generated using SOC Online

************************************************************************************/
#ifndef _B2_RX_COORDINATOR_REGS_H_
#define _B2_RX_COORDINATOR_REGS_H_

/*---------------------------------------------------------------------------------
/						Registers Addresses													 
/----------------------------------------------------------------------------------*/
#include "HwMemoryMap.h"

#define B2_RX_COORDINATOR_BASE_ADDRESS                             MEMORY_MAP_UNIT_20007_BASE_ADDRESS
#define	REG_B2_RX_COORDINATOR_ENABLE_STATE_MACHINE                  (B2_RX_COORDINATOR_BASE_ADDRESS + 0x0)
#define	REG_B2_RX_COORDINATOR_BA_AGREEMENT_EN                       (B2_RX_COORDINATOR_BASE_ADDRESS + 0x4)
#define	REG_B2_RX_COORDINATOR_STATUS_TRACE_MASTERS                  (B2_RX_COORDINATOR_BASE_ADDRESS + 0x8)
#define	REG_B2_RX_COORDINATOR_PUSHED_NULL                           (B2_RX_COORDINATOR_BASE_ADDRESS + 0xC)
#define	REG_B2_RX_COORDINATOR_EXTERNAL_CNTR_CTRL                    (B2_RX_COORDINATOR_BASE_ADDRESS + 0x14)
#define	REG_B2_RX_COORDINATOR_STATUS_TRACE_DLM_CTL                  (B2_RX_COORDINATOR_BASE_ADDRESS + 0x18)
#define	REG_B2_RX_COORDINATOR_FRAME_CLASS_VIO_CTRL                  (B2_RX_COORDINATOR_BASE_ADDRESS + 0x20)
#define	REG_B2_RX_COORDINATOR_FRAME_CLASS_VIO_IRQ                   (B2_RX_COORDINATOR_BASE_ADDRESS + 0x24)
#define	REG_B2_RX_COORDINATOR_FRAME_CLASS_VIO_CLR_IRQ               (B2_RX_COORDINATOR_BASE_ADDRESS + 0x28)
#define	REG_B2_RX_COORDINATOR_FRAME_CLASS_VIO_VALID_MAC_ADDR        (B2_RX_COORDINATOR_BASE_ADDRESS + 0x2C)
#define	REG_B2_RX_COORDINATOR_FRAME_CLASS_VIO_CLR_VALID_MAC_ADDR    (B2_RX_COORDINATOR_BASE_ADDRESS + 0x30)
#define	REG_B2_RX_COORDINATOR_WRAP_UP_RXH_ERROR_PASS                (B2_RX_COORDINATOR_BASE_ADDRESS + 0x40)
#define	REG_B2_RX_COORDINATOR_LOGGER_CNTRL                          (B2_RX_COORDINATOR_BASE_ADDRESS + 0x60)
#define	REG_B2_RX_COORDINATOR_COOR_CNTR_LO_CNTR_CTRL                (B2_RX_COORDINATOR_BASE_ADDRESS + 0x64)
#define	REG_B2_RX_COORDINATOR_INTERNAL_STATE_MACHINES               (B2_RX_COORDINATOR_BASE_ADDRESS + 0x80)
#define	REG_B2_RX_COORDINATOR_BUS_CLK_REQUESTS                      (B2_RX_COORDINATOR_BASE_ADDRESS + 0x84)
#define	REG_B2_RX_COORDINATOR_COOR_CNTR_VALIDD_MPDU                 (B2_RX_COORDINATOR_BASE_ADDRESS + 0xC0)
#define	REG_B2_RX_COORDINATOR_COOR_CNTR_RXH_ROUTE0                  (B2_RX_COORDINATOR_BASE_ADDRESS + 0xC4)
#define	REG_B2_RX_COORDINATOR_COOR_CNTR_RXH_ROUTE1                  (B2_RX_COORDINATOR_BASE_ADDRESS + 0xC8)
#define	REG_B2_RX_COORDINATOR_COOR_CNTR_FRAME_CLASS_VIO             (B2_RX_COORDINATOR_BASE_ADDRESS + 0xCC)
#define	REG_B2_RX_COORDINATOR_COOR_CNTR_FRAME_CLASS_VIO_OVFLW       (B2_RX_COORDINATOR_BASE_ADDRESS + 0xD0)
#define	REG_B2_RX_COORDINATOR_COOR_CNTR_ERR_FCS                     (B2_RX_COORDINATOR_BASE_ADDRESS + 0xD4)
#define	REG_B2_RX_COORDINATOR_COOR_CNTR_ERR_CIRC_BUF                (B2_RX_COORDINATOR_BASE_ADDRESS + 0xD8)
#define	REG_B2_RX_COORDINATOR_COOR_CNTR_RXH_ERR                     (B2_RX_COORDINATOR_BASE_ADDRESS + 0xDC)
#define	REG_B2_RX_COORDINATOR_COOR_CNTR_DE_AGG_ERR                  (B2_RX_COORDINATOR_BASE_ADDRESS + 0xE0)
#define	REG_B2_RX_COORDINATOR_COOR_CNTR_RD_DROP_ERR                 (B2_RX_COORDINATOR_BASE_ADDRESS + 0xE4)
#define	REG_B2_RX_COORDINATOR_DLM_LIST_CFG1                         (B2_RX_COORDINATOR_BASE_ADDRESS + 0x100)
#define	REG_B2_RX_COORDINATOR_DLM_LIST_CFG2                         (B2_RX_COORDINATOR_BASE_ADDRESS + 0x104)
#define	REG_B2_RX_COORDINATOR_DLM_LIST_RD_FREE_CFG1                 (B2_RX_COORDINATOR_BASE_ADDRESS + 0x108)
#define	REG_B2_RX_COORDINATOR_DLM_LIST_RD_FREE_CFG2                 (B2_RX_COORDINATOR_BASE_ADDRESS + 0x10C)
#define	REG_B2_RX_COORDINATOR_DLM_LIST_BEHAVIOR                     (B2_RX_COORDINATOR_BASE_ADDRESS + 0x120)
#define	REG_B2_RX_COORDINATOR_SPARE_REG1                            (B2_RX_COORDINATOR_BASE_ADDRESS + 0x1C0)
#define	REG_B2_RX_COORDINATOR_SPARE_REG2                            (B2_RX_COORDINATOR_BASE_ADDRESS + 0x1C4)
#define	REG_B2_RX_COORDINATOR_SPARE_REG3                            (B2_RX_COORDINATOR_BASE_ADDRESS + 0x1C8)
/*---------------------------------------------------------------------------------
/						Data Type Definition										
/----------------------------------------------------------------------------------*/
/*REG_B2_RX_COORDINATOR_ENABLE_STATE_MACHINE 0x0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swStatusTraceStayIdle : 1; //force status trace state machine to stay in idle if set to 0b1, reset value: 0x0, access type: RW
		uint32 reserved0 : 7;
		uint32 swWrapUpStayIdle : 1; //Force wrap up state machine to stay in idle if set to 0b1.  , Note that this bit must be set to 1 while RX_WRAP_UP_INPUT_LIST from queue manager is in unknown state., reset value: 0x1, access type: RW
		uint32 reserved1 : 23;
	} bitFields;
} RegB2RxCoordinatorEnableStateMachine_u;

/*REG_B2_RX_COORDINATOR_BA_AGREEMENT_EN 0x4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swEnBaAgreement : 1; //Enable the external BA Agreement for the calculated MPDUs , 0b1 - Allow the update , 0b0 - Disable the update, reset value: 0x1, access type: RW
		uint32 reserved0 : 3;
		uint32 swEnMulticastProbeReqUpdateBa : 1; //Enable multicast probe request frame to set STA activity bitmap indications, reset value: 0x0, access type: RW
		uint32 swEnBeaconUpdateBa : 1; //Enable beacon frame to set STA activity bitmap indications, reset value: 0x1, access type: RW
		uint32 reserved1 : 26;
	} bitFields;
} RegB2RxCoordinatorBaAgreementEn_u;

/*REG_B2_RX_COORDINATOR_STATUS_TRACE_MASTERS 0x8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swStatusTraceWaitDoneSecurity : 1; //selects whether to wait for security done indication before forwarding the MPDU to wrap up stage, reset value: 0x1, access type: RW
		uint32 reserved0 : 7;
		uint32 swStatusTraceWaitDoneRxc : 1; //selects whether to wait for rxc done indication before forwarding the MPDU to wrap up stage, reset value: 0x1, access type: RW
		uint32 reserved1 : 7;
		uint32 swStatusTraceWaitDoneDeAgg : 1; //selects whether to wait forde_agg done indication before forwarding the MPDU to wrap up stage, reset value: 0x1, access type: RW
		uint32 reserved2 : 7;
		uint32 swStatusTraceWaitDoneDma : 1; //selects whether to wait for dma done indication before forwarding the MPDU to wrap up stage, reset value: 0x1, access type: RW
		uint32 reserved3 : 7;
	} bitFields;
} RegB2RxCoordinatorStatusTraceMasters_u;

/*REG_B2_RX_COORDINATOR_PUSHED_NULL 0xC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 statusTracePushedNull : 1; //Indication for a case that the status trace module pushed a null MPDU pointer towards the wrap up, reset value: 0x0, access type: RO
		uint32 statusTracePushedNullClr : 1; //Write 0b1 to this bit to clear status trace null MPDU indication, reset value: 0x0, access type: WO
		uint32 reserved0 : 6;
		uint32 wrapUpPushedNull : 1; //Indication for a case that the wrap up module pushed a null MPDU pointer towards the wrap up, reset value: 0x0, access type: RO
		uint32 wrapUpPushedNullClr : 1; //Write 0b1 to this bit to clear wrap up null MPDU indication, reset value: 0x0, access type: WO
		uint32 reserved1 : 22;
	} bitFields;
} RegB2RxCoordinatorPushedNull_u;

/*REG_B2_RX_COORDINATOR_EXTERNAL_CNTR_CTRL 0x14 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swExtCntrRdCounterEn : 1; //enable RX Coordinator to update rd_counter in RX_PP , 0b0 - disable , 0b1 - enable, reset value: 0x1, access type: RW
		uint32 swExtCntrAmsduCounterEn : 1; //enable RX Coordinator to update amsdu_counter in RX_PP , 0b0 - disable , 0b1 - enable, reset value: 0x1, access type: RW
		uint32 swExtCntrClassifierDropEn : 1; //enable RX Coordinator to update classifier_drop counter in RX_PP , 0b0 - disable , 0b1 - enable, reset value: 0x1, access type: RW
		uint32 swExtCntrNumBytesInAmsduEn : 1; //enable RX Coordinator to update num_bytes_in_amsdu counter in RX_PP , 0b0 - disable , 0b1 - enable, reset value: 0x1, access type: RW
		uint32 swExtCntrNumMpduInAmpduEn : 1; //enable RX Coordinator to update num_mpdu_in_ampdu counter in RX_PP , 0b0 - disable , 0b1 - enable, reset value: 0x1, access type: RW
		uint32 swExtCntrNumOctetsInAmpduEn : 1; //enable RX Coordinator to update num_octets_in_ampdu counter in RX_PP , 0b0 - disable , 0b1 - enable, reset value: 0x1, access type: RW
		uint32 swExtCntrSecurityMismatchEn : 1; //enable RX Coordinator to update security_mismatch counter in RX_PP , 0b0 - disable , 0b1 - enable, reset value: 0x1, access type: RW
		uint32 swExtCntrSecurityTkipEn : 1; //enable RX Coordinator to update security_tkip counter in RX_PP , 0b0 - disable , 0b1 - enable, reset value: 0x1, access type: RW
		uint32 swExtCntrSecurityFailureEn : 1; //enable RX Coordinator to update security_failure counter in RX_PP , 0b0 - disable , 0b1 - enable, reset value: 0x1, access type: RW
		uint32 reserved0 : 7;
		uint32 swExtCntrFreeRdEn : 1; //enable RX Coordinator to update the free RD counter controlled by the liberator , 0b0 - disable , 0b1 - enable, reset value: 0x1, access type: RW
		uint32 reserved1 : 15;
	} bitFields;
} RegB2RxCoordinatorExternalCntrCtrl_u;

/*REG_B2_RX_COORDINATOR_STATUS_TRACE_DLM_CTL 0x18 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swDlmOutFifoCleanPulse : 1; //clear the internal DLM access fifo only in case of fatal event in the RX Coordinator. , write 1'b1 to this field to activate a pulse for the internal fifo clear., reset value: 0x0, access type: RW
		uint32 reserved0 : 3;
		uint32 coordMpduDlmReq1 : 1; //read the request signal towards the DLM. , 0b1 - there is a pending access , 0b0 - there is no pending access, reset value: 0x0, access type: RO
		uint32 reserved1 : 3;
		uint32 dlmOutFifoNumEntries : 4; //Amount of entries occupied with pending DLM accesses., reset value: 0x0, access type: RO
		uint32 reserved2 : 20;
	} bitFields;
} RegB2RxCoordinatorStatusTraceDlmCtl_u;

/*REG_B2_RX_COORDINATOR_FRAME_CLASS_VIO_CTRL 0x20 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swEnableFrameClassVioCheck : 1; //enable/disable frame class violation check in RX Coordinator, reset value: 0x1, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB2RxCoordinatorFrameClassVioCtrl_u;

/*REG_B2_RX_COORDINATOR_FRAME_CLASS_VIO_IRQ 0x24 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 frameClassViolIrq : 16; //Frame Class violation interrupt status, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegB2RxCoordinatorFrameClassVioIrq_u;

/*REG_B2_RX_COORDINATOR_FRAME_CLASS_VIO_CLR_IRQ 0x28 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 frameClassViolIrqClr : 16; //Frame Class violation clear interrupt status. , Clear by write '1', reset value: 0x0, access type: WO
		uint32 reserved0 : 16;
	} bitFields;
} RegB2RxCoordinatorFrameClassVioClrIrq_u;

/*REG_B2_RX_COORDINATOR_FRAME_CLASS_VIO_VALID_MAC_ADDR 0x2C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 frameClassViolValidMacAddr : 16; //Frame Class violation valid MAC addresses, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegB2RxCoordinatorFrameClassVioValidMacAddr_u;

/*REG_B2_RX_COORDINATOR_FRAME_CLASS_VIO_CLR_VALID_MAC_ADDR 0x30 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 frameClassViolValidMacAddrClr : 16; //Frame Class violation clear valid MAC addresses vector. , Clear by write '1' , , reset value: 0x0, access type: WO
		uint32 reserved0 : 16;
	} bitFields;
} RegB2RxCoordinatorFrameClassVioClrValidMacAddr_u;

/*REG_B2_RX_COORDINATOR_WRAP_UP_RXH_ERROR_PASS 0x40 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swRxhErrorBypass : 1; //selects whether to move the rxh_error category to rxh_error list or to error list , 0 - move to error , 1 - move to rxh_error, reset value: 0x1, access type: RW
		uint32 reserved0 : 7;
		uint32 swRxhErrorPassFcsFail : 1; //This field is valid only if sw_rxh_error_bypass is set to 1. , selects whether to move MPDU to rxh_error category if FCS failed , 0 - move to error , 1 - move to rxh_error, reset value: 0x0, access type: RW
		uint32 swRxhErrorPassRxhError : 1; //This field is valid only if sw_rxh_error_bypass is set to 1. , selects whether to move MPDU to rxh_error category if RXH_Error bit is set in MPDU descriptor , 0 - move to error , 1 - move to rxh_error, reset value: 0x1, access type: RW
		uint32 swRxhErrorPassRdType : 1; //This field is valid only if sw_rxh_error_bypass is set to 1. , selects whether to move MPDU to rxh_error category if RD_type in MPDU descriptor is "Drop" , 0 - move to error , 1 - move to rxh_error, reset value: 0x1, access type: RW
		uint32 swRxhErrorPassDeAgg : 1; //This field is valid only if sw_rxh_error_bypass is set to 1. , selects whether to move MPDU to rxh_error category if there is a de-agg error in MPDU descriptor , 0 - move to error , 1 - move to rxh_error, reset value: 0x1, access type: RW
		uint32 reserved1 : 4;
		uint32 swRxhErrorPassSecurityError : 7; //This field is valid only if sw_rxh_error_bypass is set to 1. , selects whether to move MPDU to rxh_error category if one of the security indication bits is set to error. There is a configuration bit per security status bit. , 0 - move to error , 1 - move to rxh_error, reset value: 0x7F, access type: RW
		uint32 reserved2 : 9;
	} bitFields;
} RegB2RxCoordinatorWrapUpRxhErrorPass_u;

/*REG_B2_RX_COORDINATOR_LOGGER_CNTRL 0x60 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swRxCoordinatorLoggerEn : 1; //enable RX coordinator logger activity. , Support soft activation/deactivation., reset value: 0x0, access type: RW
		uint32 rxCoordinatorLoggerActive : 1; //poll this bit to actually verify the logger machine status: , 0b0 - machine is off (no logger transactions) , 0b1 - machine is on (module generates logger transactions) , , reset value: 0x0, access type: RO
		uint32 swRxCoordinatorLoggerAllowValidWhileDone : 1; //enable RX coordinator logger to send "valid" during "done" to the logger, reset value: 0x0, access type: RW
		uint32 swRxCoordinatorLoggerSelectCounters : 1; //select RX coordinator logger events type , 1'b0 - Coordinator debug logger (msg_ID 0) , 1'b1 - Plume counters logger (msg_ID 1), reset value: 0x0, access type: RW
		uint32 swRxCoordinatorLoggerPriority : 2; //set the priority of the rx coordinator towards the logger, reset value: 0x0, access type: RW
		uint32 swRxCoordinatorCountersSelectDb : 1; //select which double buffer to update for logger counters, reset value: 0x0, access type: RW
		uint32 swRxCoordinatorShrinkCounters : 1; //set high to shrink the "per MPDU" logger message o 32 bits. This will allow highr less traffic over logger., reset value: 0x0, access type: RW
		uint32 swRxCoordinatorLoggerStream : 16; //each bit set to 1 enables a data stream to the logger, reset value: 0x0, access type: RW
		uint32 swRxCoordinatorLoggerLimit : 7; //Limit the number of "valid" events between 2 "done" events, reset value: 0x7D, access type: RW
		uint32 reserved0 : 1;
	} bitFields;
} RegB2RxCoordinatorLoggerCntrl_u;

/*REG_B2_RX_COORDINATOR_COOR_CNTR_LO_CNTR_CTRL 0x64 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swRxhRouteCounter0CntPostPpdu : 1; //allow rxh_route_counter0 to count whenever the post_ppdu_phy_status forwarding category was selected. , counter supports all configurations., reset value: 0x0, access type: RW
		uint32 swRxhRouteCounter0CntRxhPass : 1; //allow rxh_route_counter0 to count whenever the rxh_pass forwarding category was selected. , counter supports all configurations., reset value: 0x1, access type: RW
		uint32 swRxhRouteCounter0CntRxhError : 1; //allow rxh_route_counter0 to count whenever the rxh_error forwarding category was selected. , counter supports all configurations., reset value: 0x0, access type: RW
		uint32 reserved0 : 5;
		uint32 swRxhRouteCounter1CntPostPpdu : 1; //allow rxh_route_counter1 to count whenever the post_ppdu_phy_status forwarding category was selected. , counter supports all configurations., reset value: 0x0, access type: RW
		uint32 swRxhRouteCounter1CntRxhPass : 1; //allow rxh_route_counter1 to count whenever the rxh_pass forwarding category was selected. , counter supports all configurations., reset value: 0x0, access type: RW
		uint32 swRxhRouteCounter1CntRxhError : 1; //allow rxh_route_counter0 to count whenever the rxh_error forwarding category was selected. , counter supports all configurations., reset value: 0x1, access type: RW
		uint32 reserved1 : 5;
		uint32 swCountAllFrameClassVio : 1; //allows local counter to count frame class violation events even when they are masked in the wrap up state machine. , 0b0 - count only if sw_enable_frame_class_vio_check == 0b1 , 0b1 - count whenever there is a frame class violation and ignore sw_enable_frame_class_vio_check value, reset value: 0x0, access type: RW
		uint32 reserved2 : 15;
	} bitFields;
} RegB2RxCoordinatorCoorCntrLoCntrCtrl_u;

/*REG_B2_RX_COORDINATOR_INTERNAL_STATE_MACHINES 0x80 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 statusTraceSm : 3; //state machine for debug, reset value: 0x0, access type: RO
		uint32 reserved0 : 5;
		uint32 wrapUpSm : 4; //state machine for debug, reset value: 0x0, access type: RO
		uint32 selectedFwCat : 3; //no description, reset value: 0x0, access type: RO
		uint32 reserved1 : 1;
		uint32 frameClassFilterSm : 3; //state machine for debug, reset value: 0x0, access type: RO
		uint32 reserved2 : 5;
		uint32 extCounterSm : 3; //state machine for debug, reset value: 0x0, access type: RO
		uint32 reserved3 : 5;
	} bitFields;
} RegB2RxCoordinatorInternalStateMachines_u;

/*REG_B2_RX_COORDINATOR_BUS_CLK_REQUESTS 0x84 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 coordMpduDlmReq : 1; //dlm request from status trace, reset value: 0x0, access type: RO
		uint32 coordStatusTraceSmcReq : 1; //smc request from status trace, reset value: 0x0, access type: RO
		uint32 coordWrapUpToDescSmcReq : 1; //smc request from wrap up to Descriptor RAM, reset value: 0x0, access type: RO
		uint32 coordApbPsel : 1; //coordinator apb psel, reset value: 0x0, access type: RO
		uint32 coordWrapUpToMdSmcReq : 1; //smc request from wrap up to MPDU RAM, reset value: 0x0, access type: RO
		uint32 coordWrapUpMpduDlmReq : 1; //dlm request from wrap up to MPDU list, reset value: 0x0, access type: RO
		uint32 coordRxDlmReq : 1; //dlm request from wrap up to RD list, reset value: 0x0, access type: RO
		uint32 reserved0 : 1;
		uint32 statusTraceGatedClkEn : 1; //gated clock dynamic control, reset value: 0x0, access type: RO
		uint32 statusTraceDmaFifoClkEn : 1; //gated clock dynamic control, reset value: 0x0, access type: RO
		uint32 statusTraceRxcFifoClkEn : 1; //gated clock dynamic control, reset value: 0x0, access type: RO
		uint32 statusTraceSecurityFifoClkEn : 1; //gated clock dynamic control, reset value: 0x0, access type: RO
		uint32 statusTraceDeAggFifoClkEn : 1; //gated clock dynamic control, reset value: 0x0, access type: RO
		uint32 wrapUpGatedClkEn : 1; //gated clock dynamic control, reset value: 0x0, access type: RO
		uint32 frameClassVioGatedClkEn : 1; //gated clock dynamic control, reset value: 0x0, access type: RO
		uint32 statisticsDynamicClkEn : 1; //gated clock dynamic control, reset value: 0x0, access type: RW
		uint32 reserved1 : 16;
	} bitFields;
} RegB2RxCoordinatorBusClkRequests_u;

/*REG_B2_RX_COORDINATOR_COOR_CNTR_VALIDD_MPDU 0xC0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 validMpduCounter : 30; //counter value, reset value: 0x0, access type: RO
		uint32 validMpduCounterOverlfow : 1; //overflow indication. If this bit is 0b1 then counter was in overflow., reset value: 0x0, access type: RO
		uint32 validMpduCounterClear : 1; //Write 0b1 to this bit to clear counter value and overflow indication., reset value: 0x0, access type: WO
	} bitFields;
} RegB2RxCoordinatorCoorCntrValiddMpdu_u;

/*REG_B2_RX_COORDINATOR_COOR_CNTR_RXH_ROUTE0 0xC4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxhRoute0Counter : 24; //counter value, reset value: 0x0, access type: RO
		uint32 reserved0 : 6;
		uint32 rxhRoute0CounterOverlfow : 1; //overflow indication. If this bit is 0b1 then counter was in overflow., reset value: 0x0, access type: RO
		uint32 rxhRoute0CounterClear : 1; //Write 0b1 to this bit to clear counter value and overflow indication., reset value: 0x0, access type: WO
	} bitFields;
} RegB2RxCoordinatorCoorCntrRxhRoute0_u;

/*REG_B2_RX_COORDINATOR_COOR_CNTR_RXH_ROUTE1 0xC8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxhRoute1Counter : 24; //counter value, reset value: 0x0, access type: RO
		uint32 reserved0 : 6;
		uint32 rxhRoute1CounterOverlfow : 1; //overflow indication. If this bit is 0b1 then counter was in overflow., reset value: 0x0, access type: RO
		uint32 rxhRoute1CounterClear : 1; //Write 0b1 to this bit to clear counter value and overflow indication., reset value: 0x0, access type: WO
	} bitFields;
} RegB2RxCoordinatorCoorCntrRxhRoute1_u;

/*REG_B2_RX_COORDINATOR_COOR_CNTR_FRAME_CLASS_VIO 0xCC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 frameClassVioCounter : 24; //counter value, reset value: 0x0, access type: RO
		uint32 reserved0 : 6;
		uint32 frameClassVioCounterOverlfow : 1; //overflow indication. If this bit is 0b1 then counter was in overflow., reset value: 0x0, access type: RO
		uint32 frameClassVioCounterClear : 1; //Write 0b1 to this bit to clear counter value and overflow indication., reset value: 0x0, access type: WO
	} bitFields;
} RegB2RxCoordinatorCoorCntrFrameClassVio_u;

/*REG_B2_RX_COORDINATOR_COOR_CNTR_FRAME_CLASS_VIO_OVFLW 0xD0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 frameClassViolBufFullCnt : 16; //counter value, reset value: 0x0, access type: RO
		uint32 reserved0 : 15;
		uint32 frameClassViolBufFullCntClr : 1; //Write 0b1 to this bit to clear counter value and overflow indication., reset value: 0x0, access type: WO
	} bitFields;
} RegB2RxCoordinatorCoorCntrFrameClassVioOvflw_u;

/*REG_B2_RX_COORDINATOR_COOR_CNTR_ERR_FCS 0xD4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 fcsErrCounter : 20; //counter value, reset value: 0x0, access type: RO
		uint32 reserved0 : 10;
		uint32 fcsErrCounterOverlfow : 1; //overflow indication. If this bit is 0b1 then counter was in overflow., reset value: 0x0, access type: RO
		uint32 fcsErrCounterClear : 1; //Write 0b1 to this bit to clear counter value and overflow indication., reset value: 0x0, access type: WO
	} bitFields;
} RegB2RxCoordinatorCoorCntrErrFcs_u;

/*REG_B2_RX_COORDINATOR_COOR_CNTR_ERR_CIRC_BUF 0xD8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 circBufErrCounter : 20; //counter value, reset value: 0x0, access type: RO
		uint32 reserved0 : 10;
		uint32 circBufErrCounterOverlfow : 1; //overflow indication. If this bit is 0b1 then counter was in overflow., reset value: 0x0, access type: RO
		uint32 circBufErrCounterClear : 1; //Write 0b1 to this bit to clear counter value and overflow indication., reset value: 0x0, access type: WO
	} bitFields;
} RegB2RxCoordinatorCoorCntrErrCircBuf_u;

/*REG_B2_RX_COORDINATOR_COOR_CNTR_RXH_ERR 0xDC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxhErrCounter : 20; //counter value, reset value: 0x0, access type: RO
		uint32 reserved0 : 10;
		uint32 rxhErrCounterOverlfow : 1; //overflow indication. If this bit is 0b1 then counter was in overflow., reset value: 0x0, access type: RO
		uint32 rxhErrCounterClear : 1; //Write 0b1 to this bit to clear counter value and overflow indication., reset value: 0x0, access type: WO
	} bitFields;
} RegB2RxCoordinatorCoorCntrRxhErr_u;

/*REG_B2_RX_COORDINATOR_COOR_CNTR_DE_AGG_ERR 0xE0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 deAggErrCounter : 20; //counter value, reset value: 0x0, access type: RO
		uint32 reserved0 : 10;
		uint32 deAggErrCounterOverlfow : 1; //overflow indication. If this bit is 0b1 then counter was in overflow., reset value: 0x0, access type: RO
		uint32 deAggErrCounterClear : 1; //Write 0b1 to this bit to clear counter value and overflow indication., reset value: 0x0, access type: WO
	} bitFields;
} RegB2RxCoordinatorCoorCntrDeAggErr_u;

/*REG_B2_RX_COORDINATOR_COOR_CNTR_RD_DROP_ERR 0xE4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rdDropErrCounter : 20; //counter value, reset value: 0x0, access type: RO
		uint32 reserved0 : 10;
		uint32 rdDropErrCounterOverlfow : 1; //overflow indication. If this bit is 0b1 then counter was in overflow., reset value: 0x0, access type: RO
		uint32 rdDropErrCounterClear : 1; //Write 0b1 to this bit to clear counter value and overflow indication., reset value: 0x0, access type: WO
	} bitFields;
} RegB2RxCoordinatorCoorCntrRdDropErr_u;

/*REG_B2_RX_COORDINATOR_DLM_LIST_CFG1 0x100 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxWrapUpInputListIdx : 8; //dlm list index configuration, reset value: 0x0, access type: RW
		uint32 postPpduPhyStatusIdx : 8; //dlm list index configuration, reset value: 0x0, access type: RW
		uint32 rxhPassIdx : 8; //dlm list index configuration, reset value: 0x0, access type: RW
		uint32 rxhErrorListIdx : 8; //dlm list index configuration, reset value: 0x0, access type: RW
	} bitFields;
} RegB2RxCoordinatorDlmListCfg1_u;

/*REG_B2_RX_COORDINATOR_DLM_LIST_CFG2 0x104 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mpduFreeListIdx : 8; //dlm list index configuration, reset value: 0x0, access type: RW
		uint32 rxPp01InputListIdx : 8; //dlm list index configuration, reset value: 0x0, access type: RW
		uint32 rxPp2InputListIdx : 8; //dlm list index configuration, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegB2RxCoordinatorDlmListCfg2_u;

/*REG_B2_RX_COORDINATOR_DLM_LIST_RD_FREE_CFG1 0x108 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rdFreeList0Idx : 8; //dlm list index configuration, reset value: 0x0, access type: RW
		uint32 rdFreeList1Idx : 8; //dlm list index configuration, reset value: 0x0, access type: RW
		uint32 rdFreeList2Idx : 8; //dlm list index configuration, reset value: 0x0, access type: RW
		uint32 rdFreeList3Idx : 8; //dlm list index configuration, reset value: 0x0, access type: RW
	} bitFields;
} RegB2RxCoordinatorDlmListRdFreeCfg1_u;

/*REG_B2_RX_COORDINATOR_DLM_LIST_RD_FREE_CFG2 0x10C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rdFreeList4Idx : 8; //dlm list index configuration, reset value: 0x0, access type: RW
		uint32 rdFreeList5Idx : 8; //dlm list index configuration, reset value: 0x0, access type: RW
		uint32 rdFreeList6Idx : 8; //dlm list index configuration, reset value: 0x0, access type: RW
		uint32 rdFreeList7Idx : 8; //dlm list index configuration, reset value: 0x0, access type: RW
	} bitFields;
} RegB2RxCoordinatorDlmListRdFreeCfg2_u;

/*REG_B2_RX_COORDINATOR_DLM_LIST_BEHAVIOR 0x120 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rdFreeListIsIndeedFree : 8; //For each bit require to set if the rd_free_list is indeed a free list. If one of the bits is set to 0 then push to this list wont promote the free RD counter in the liberator, reset value: 0xFF, access type: RW
		uint32 reserved0 : 24;
	} bitFields;
} RegB2RxCoordinatorDlmListBehavior_u;

/*REG_B2_RX_COORDINATOR_SPARE_REG1 0x1C0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 spareField0 : 32; //spare, reset value: 0xFF000000, access type: RW
	} bitFields;
} RegB2RxCoordinatorSpareReg1_u;

/*REG_B2_RX_COORDINATOR_SPARE_REG2 0x1C4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 spareField1 : 8; //spare, reset value: 0xF0, access type: RW
		uint32 spareField2 : 4; //spare, reset value: 0x0, access type: RW
		uint32 reserved0 : 4;
		uint32 spareField3 : 4; //spare, reset value: 0x0, access type: RW
		uint32 reserved1 : 4;
		uint32 spareField4 : 4; //spare, reset value: 0x0, access type: RW
		uint32 reserved2 : 4;
	} bitFields;
} RegB2RxCoordinatorSpareReg2_u;

/*REG_B2_RX_COORDINATOR_SPARE_REG3 0x1C8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 spareField5 : 4; //spare, reset value: 0x0, access type: RW
		uint32 reserved0 : 28;
	} bitFields;
} RegB2RxCoordinatorSpareReg3_u;



#endif // _B2_RX_COORDINATOR_REGS_H_
