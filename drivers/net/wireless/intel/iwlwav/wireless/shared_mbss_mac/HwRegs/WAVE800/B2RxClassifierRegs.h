
/***********************************************************************************
File:				B2RxClassifierRegs.h
Module:				b2RxClassifier
SOC Revision:		latest
Generated at:       2024-06-26 12:54:58
Purpose:
Description:		This File was auto generated using SOC Online

************************************************************************************/
#ifndef _B2_RX_CLASSIFIER_REGS_H_
#define _B2_RX_CLASSIFIER_REGS_H_

/*---------------------------------------------------------------------------------
/						Registers Addresses													 
/----------------------------------------------------------------------------------*/
#include "HwMemoryMap.h"

#define B2_RX_CLASSIFIER_BASE_ADDRESS                MEMORY_MAP_UNIT_20057_BASE_ADDRESS
#define	REG_B2_RX_CLASSIFIER_FRAME_CLASS_FILTER_CONTROL     (B2_RX_CLASSIFIER_BASE_ADDRESS + 0x0)
#define	REG_B2_RX_CLASSIFIER_FRAME_DATA_SUBTYPES_CLASS      (B2_RX_CLASSIFIER_BASE_ADDRESS + 0x4)
#define	REG_B2_RX_CLASSIFIER_FRAME_CTRL_SUBTYPES_CLASS      (B2_RX_CLASSIFIER_BASE_ADDRESS + 0x8)
#define	REG_B2_RX_CLASSIFIER_FRAME_MGMT_SUBTYPES_CLASS      (B2_RX_CLASSIFIER_BASE_ADDRESS + 0xC)
#define	REG_B2_RX_CLASSIFIER_FRAME_MGMT_ACTION_CLASS        (B2_RX_CLASSIFIER_BASE_ADDRESS + 0x10)
#define	REG_B2_RX_CLASSIFIER_DLM_SEC_LIST_IDX               (B2_RX_CLASSIFIER_BASE_ADDRESS + 0x40)
#define	REG_B2_RX_CLASSIFIER_DLM_IF_DBG                     (B2_RX_CLASSIFIER_BASE_ADDRESS + 0x44)
#define	REG_B2_RX_CLASSIFIER_WPA_SEC_PASS_THROUGH_VAP_EN    (B2_RX_CLASSIFIER_BASE_ADDRESS + 0x48)
#define	REG_B2_RX_CLASSIFIER_MPDU_HDR_FIELDS_FILTER_EN      (B2_RX_CLASSIFIER_BASE_ADDRESS + 0x4C)
#define	REG_B2_RX_CLASSIFIER_RD_TYPE_DROP_VALUE             (B2_RX_CLASSIFIER_BASE_ADDRESS + 0x50)
#define	REG_B2_RX_CLASSIFIER_RXQ_ID_FILTER0                 (B2_RX_CLASSIFIER_BASE_ADDRESS + 0x60)
#define	REG_B2_RX_CLASSIFIER_RXQ_ID_FILTER1                 (B2_RX_CLASSIFIER_BASE_ADDRESS + 0x64)
#define	REG_B2_RX_CLASSIFIER_RXQ_ID_FILTER2                 (B2_RX_CLASSIFIER_BASE_ADDRESS + 0x68)
#define	REG_B2_RX_CLASSIFIER_RXQ_ID_FILTER3                 (B2_RX_CLASSIFIER_BASE_ADDRESS + 0x6C)
#define	REG_B2_RX_CLASSIFIER_RXQ_ID_FILTER4                 (B2_RX_CLASSIFIER_BASE_ADDRESS + 0x70)
#define	REG_B2_RX_CLASSIFIER_RXQ_ID_FILTER5                 (B2_RX_CLASSIFIER_BASE_ADDRESS + 0x74)
#define	REG_B2_RX_CLASSIFIER_RXQ_ID_FILTER6                 (B2_RX_CLASSIFIER_BASE_ADDRESS + 0x78)
#define	REG_B2_RX_CLASSIFIER_RXQ_ID_FILTER7                 (B2_RX_CLASSIFIER_BASE_ADDRESS + 0x7C)
#define	REG_B2_RX_CLASSIFIER_RXQ_ID_FILTER8                 (B2_RX_CLASSIFIER_BASE_ADDRESS + 0x80)
#define	REG_B2_RX_CLASSIFIER_RXQ_ID_FILTER9                 (B2_RX_CLASSIFIER_BASE_ADDRESS + 0x84)
#define	REG_B2_RX_CLASSIFIER_RXQ_ID_FILTER10                (B2_RX_CLASSIFIER_BASE_ADDRESS + 0x88)
#define	REG_B2_RX_CLASSIFIER_RXQ_ID_FILTER11                (B2_RX_CLASSIFIER_BASE_ADDRESS + 0x8C)
#define	REG_B2_RX_CLASSIFIER_RXQ_ID_FILTER12                (B2_RX_CLASSIFIER_BASE_ADDRESS + 0x90)
#define	REG_B2_RX_CLASSIFIER_RXQ_ID_FILTER13                (B2_RX_CLASSIFIER_BASE_ADDRESS + 0x94)
#define	REG_B2_RX_CLASSIFIER_RXQ_ID_FILTER14                (B2_RX_CLASSIFIER_BASE_ADDRESS + 0x98)
#define	REG_B2_RX_CLASSIFIER_RXQ_ID_FILTER15                (B2_RX_CLASSIFIER_BASE_ADDRESS + 0x9C)
#define	REG_B2_RX_CLASSIFIER_RXQ_ID_FILTER_CONTROL          (B2_RX_CLASSIFIER_BASE_ADDRESS + 0xA0)
#define	REG_B2_RX_CLASSIFIER_RX_CLASSIFIER_ENABLE           (B2_RX_CLASSIFIER_BASE_ADDRESS + 0xB0)
#define	REG_B2_RX_CLASSIFIER_WEP_KEY_IDX_DB_LOCATION        (B2_RX_CLASSIFIER_BASE_ADDRESS + 0xB4)
#define	REG_B2_RX_CLASSIFIER_RX_CLAS_RTD_CTRL               (B2_RX_CLASSIFIER_BASE_ADDRESS + 0x100)
#define	REG_B2_RX_CLASSIFIER_RX_CLAS_SM_DBG                 (B2_RX_CLASSIFIER_BASE_ADDRESS + 0x114)
#define	REG_B2_RX_CLASSIFIER_RX_CLAS_LOGGER_CONTROL         (B2_RX_CLASSIFIER_BASE_ADDRESS + 0x134)
#define	REG_B2_RX_CLASSIFIER_RX_CLAS_LOGGER_ACTIVE          (B2_RX_CLASSIFIER_BASE_ADDRESS + 0x138)
#define	REG_B2_RX_CLASSIFIER_SPARE_REGISTERS                (B2_RX_CLASSIFIER_BASE_ADDRESS + 0x1FC)
/*---------------------------------------------------------------------------------
/						Data Type Definition										
/----------------------------------------------------------------------------------*/
/*REG_B2_RX_CLASSIFIER_FRAME_CLASS_FILTER_CONTROL 0x0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 frameClassFilterAddr2ValidDropEn : 1; //Enable frame class drop filter when Addr2 index is valid, reset value: 0x1, access type: RW
		uint32 frameClassFilterAddr2InvalidDropEn : 1; //Enable frame class drop filter when Addr2 index is invalid, reset value: 0x1, access type: RW
		uint32 vapStaMisFilterDropEn : 1; //Enable VAP-STA mismatch drop filter, reset value: 0x1, access type: RW
		uint32 reserved0 : 1;
		uint32 addr2InvalidDropMpduClassVal : 2; //MPDU frame class to be dropped in case of addr2 index is not valid, reset value: 0x3, access type: RW
		uint32 vapStaMisDropMpduClassVal : 2; //MPDU frame class to be dropped in case of VAP-STA mismatch, reset value: 0x3, access type: RW
		uint32 reserved1 : 24;
	} bitFields;
} RegB2RxClassifierFrameClassFilterControl_u;

/*REG_B2_RX_CLASSIFIER_FRAME_DATA_SUBTYPES_CLASS 0x4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 frameDataSubtypesClass : 32; //no description, reset value: 0xffffffff, access type: RW
	} bitFields;
} RegB2RxClassifierFrameDataSubtypesClass_u;

/*REG_B2_RX_CLASSIFIER_FRAME_CTRL_SUBTYPES_CLASS 0x8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 frameCtrlSubtypesClass : 32; //no description, reset value: 0x557fffff, access type: RW
	} bitFields;
} RegB2RxClassifierFrameCtrlSubtypesClass_u;

/*REG_B2_RX_CLASSIFIER_FRAME_MGMT_SUBTYPES_CLASS 0xC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 frameMgmtSubtypesClass : 32; //no description, reset value: 0xfd65f5aa, access type: RW
	} bitFields;
} RegB2RxClassifierFrameMgmtSubtypesClass_u;

/*REG_B2_RX_CLASSIFIER_FRAME_MGMT_ACTION_CLASS 0x10 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 frameMgmtAction4Class : 2; //no description, reset value: 0x1, access type: RW
		uint32 frameMgmtAction15Class : 2; //no description, reset value: 0x1, access type: RW
		uint32 frameMgmtActionOtherClass : 2; //no description, reset value: 0x3, access type: RW
		uint32 reserved0 : 26;
	} bitFields;
} RegB2RxClassifierFrameMgmtActionClass_u;

/*REG_B2_RX_CLASSIFIER_DLM_SEC_LIST_IDX 0x40 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dlmSecListIdx : 8; //Rx MPDU descriptor DLM list index of Security engine user0, reset value: 0x8, access type: RW
		uint32 reserved0 : 24;
	} bitFields;
} RegB2RxClassifierDlmSecListIdx_u;

/*REG_B2_RX_CLASSIFIER_DLM_IF_DBG 0x44 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dlmOutFifoNumEntries : 2; //DLM out FIFO number of entries, reset value: 0x0, access type: RO
		uint32 reserved0 : 5;
		uint32 dlmOutFifoCleanStb : 1; //DLM out FIFO clean strobe, reset value: 0x0, access type: WO
		uint32 dlmInFifoNumEntries : 2; //DLM in FIFO number of entries, reset value: 0x0, access type: RO
		uint32 reserved1 : 5;
		uint32 dlmInFifoCleanStb : 1; //DLM in FIFO clean strobe, reset value: 0x0, access type: WO
		uint32 reserved2 : 16;
	} bitFields;
} RegB2RxClassifierDlmIfDbg_u;

/*REG_B2_RX_CLASSIFIER_WPA_SEC_PASS_THROUGH_VAP_EN 0x48 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 wpaSecPassThroughVapEn : 32; //Enable setting RD type of WPA in case of Protected and security mismatch. , 1 bit per VAP, reset value: 0x0, access type: RW
	} bitFields;
} RegB2RxClassifierWpaSecPassThroughVapEn_u;

/*REG_B2_RX_CLASSIFIER_MPDU_HDR_FIELDS_FILTER_EN 0x4C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mpduHdrFcFieldsFilterEn : 8; //MPDU header Frame control fields filter enable, 1bit per group., reset value: 0x0, access type: RW
		uint32 mpduHdrQosFieldsFilterEn : 8; //MPDU header QoS control fields filter enable, 1bit per group., reset value: 0x0, access type: RW
		uint32 mpduHdrHtcFieldsFilterEn : 8; //MPDU header HT control fields filter enable, 1bit per group., reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegB2RxClassifierMpduHdrFieldsFilterEn_u;

/*REG_B2_RX_CLASSIFIER_RD_TYPE_DROP_VALUE 0x50 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rdTypeDropValue : 5; //RD type value of DROP, reset value: 0x9, access type: RW
		uint32 reserved0 : 27;
	} bitFields;
} RegB2RxClassifierRdTypeDropValue_u;

/*REG_B2_RX_CLASSIFIER_RXQ_ID_FILTER0 0x60 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxqIdFilter0DropEn : 1; //Enable RXQ-ID filter drop indication, reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 rxqIdFilter0FrameType : 2; //Field frame type in RXQ-ID filter, reset value: 0x0, access type: RW
		uint32 rxqIdFilter0TypeNa : 1; //Field frame type in RXQ-ID filter is N/A, reset value: 0x0, access type: RW
		uint32 rxqIdFilter0FrameSubtype : 4; //Field frame subtype in RXQ-ID filter, reset value: 0x0, access type: RW
		uint32 rxqIdFilter0SubtypeNa : 1; //Field frame subtype in RXQ-ID filter is N/A, reset value: 0x0, access type: RW
		uint32 rxqIdFilter0SubtypeEqual : 1; //Subtype compare logic: , 1: logic of equal , 0: logic of not equal, reset value: 0x0, access type: RW
		uint32 reserved1 : 2;
		uint32 rxqIdFilter0Addr2IdVal : 1; //Address2 index valid bit in RXQ-ID filter, reset value: 0x0, access type: RW
		uint32 rxqIdFilter0Addr2IdValNa : 1; //Address2 index valid bit in RXQ-ID filter is N/A, reset value: 0x0, access type: RW
		uint32 rxqIdFilter0Addr1Unicast : 1; //Address1 unicast bit in RXQ-ID filter, reset value: 0x0, access type: RW
		uint32 rxqIdFilter0Addr1UnicastNa : 1; //Address1 unicast bit in RXQ-ID filter is N/A, reset value: 0x0, access type: RW
		uint32 reserved2 : 15;
	} bitFields;
} RegB2RxClassifierRxqIdFilter0_u;

/*REG_B2_RX_CLASSIFIER_RXQ_ID_FILTER1 0x64 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxqIdFilter1DropEn : 1; //Enable RXQ-ID filter drop indication, reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 rxqIdFilter1FrameType : 2; //Field frame type in RXQ-ID filter, reset value: 0x0, access type: RW
		uint32 rxqIdFilter1TypeNa : 1; //Field frame type in RXQ-ID filter is N/A, reset value: 0x0, access type: RW
		uint32 rxqIdFilter1FrameSubtype : 4; //Field frame subtype in RXQ-ID filter, reset value: 0x0, access type: RW
		uint32 rxqIdFilter1SubtypeNa : 1; //Field frame subtype in RXQ-ID filter is N/A, reset value: 0x0, access type: RW
		uint32 rxqIdFilter1SubtypeEqual : 1; //Subtype compare logic: , 1: logic of equal , 0: logic of not equal, reset value: 0x0, access type: RW
		uint32 reserved1 : 2;
		uint32 rxqIdFilter1Addr2IdVal : 1; //Address2 index valid bit in RXQ-ID filter, reset value: 0x0, access type: RW
		uint32 rxqIdFilter1Addr2IdValNa : 1; //Address2 index valid bit in RXQ-ID filter is N/A, reset value: 0x0, access type: RW
		uint32 rxqIdFilter1Addr1Unicast : 1; //Address1 unicast bit in RXQ-ID filter, reset value: 0x0, access type: RW
		uint32 rxqIdFilter1Addr1UnicastNa : 1; //Address1 unicast bit in RXQ-ID filter is N/A, reset value: 0x0, access type: RW
		uint32 reserved2 : 15;
	} bitFields;
} RegB2RxClassifierRxqIdFilter1_u;

/*REG_B2_RX_CLASSIFIER_RXQ_ID_FILTER2 0x68 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxqIdFilter2DropEn : 1; //Enable RXQ-ID filter drop indication, reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 rxqIdFilter2FrameType : 2; //Field frame type in RXQ-ID filter, reset value: 0x0, access type: RW
		uint32 rxqIdFilter2TypeNa : 1; //Field frame type in RXQ-ID filter is N/A, reset value: 0x0, access type: RW
		uint32 rxqIdFilter2FrameSubtype : 4; //Field frame subtype in RXQ-ID filter, reset value: 0x0, access type: RW
		uint32 rxqIdFilter2SubtypeNa : 1; //Field frame subtype in RXQ-ID filter is N/A, reset value: 0x0, access type: RW
		uint32 rxqIdFilter2SubtypeEqual : 1; //Subtype compare logic: , 1: logic of equal , 0: logic of not equal, reset value: 0x0, access type: RW
		uint32 reserved1 : 2;
		uint32 rxqIdFilter2Addr2IdVal : 1; //Address2 index valid bit in RXQ-ID filter, reset value: 0x0, access type: RW
		uint32 rxqIdFilter2Addr2IdValNa : 1; //Address2 index valid bit in RXQ-ID filter is N/A, reset value: 0x0, access type: RW
		uint32 rxqIdFilter2Addr1Unicast : 1; //Address1 unicast bit in RXQ-ID filter, reset value: 0x0, access type: RW
		uint32 rxqIdFilter2Addr1UnicastNa : 1; //Address1 unicast bit in RXQ-ID filter is N/A, reset value: 0x0, access type: RW
		uint32 reserved2 : 15;
	} bitFields;
} RegB2RxClassifierRxqIdFilter2_u;

/*REG_B2_RX_CLASSIFIER_RXQ_ID_FILTER3 0x6C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxqIdFilter3DropEn : 1; //Enable RXQ-ID filter drop indication, reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 rxqIdFilter3FrameType : 2; //Field frame type in RXQ-ID filter, reset value: 0x0, access type: RW
		uint32 rxqIdFilter3TypeNa : 1; //Field frame type in RXQ-ID filter is N/A, reset value: 0x0, access type: RW
		uint32 rxqIdFilter3FrameSubtype : 4; //Field frame subtype in RXQ-ID filter, reset value: 0x0, access type: RW
		uint32 rxqIdFilter3SubtypeNa : 1; //Field frame subtype in RXQ-ID filter is N/A, reset value: 0x0, access type: RW
		uint32 rxqIdFilter3SubtypeEqual : 1; //Subtype compare logic: , 1: logic of equal , 0: logic of not equal, reset value: 0x0, access type: RW
		uint32 reserved1 : 2;
		uint32 rxqIdFilter3Addr2IdVal : 1; //Address2 index valid bit in RXQ-ID filter, reset value: 0x0, access type: RW
		uint32 rxqIdFilter3Addr2IdValNa : 1; //Address2 index valid bit in RXQ-ID filter is N/A, reset value: 0x0, access type: RW
		uint32 rxqIdFilter3Addr1Unicast : 1; //Address1 unicast bit in RXQ-ID filter, reset value: 0x0, access type: RW
		uint32 rxqIdFilter3Addr1UnicastNa : 1; //Address1 unicast bit in RXQ-ID filter is N/A, reset value: 0x0, access type: RW
		uint32 reserved2 : 15;
	} bitFields;
} RegB2RxClassifierRxqIdFilter3_u;

/*REG_B2_RX_CLASSIFIER_RXQ_ID_FILTER4 0x70 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxqIdFilter4DropEn : 1; //Enable RXQ-ID filter drop indication, reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 rxqIdFilter4FrameType : 2; //Field frame type in RXQ-ID filter, reset value: 0x0, access type: RW
		uint32 rxqIdFilter4TypeNa : 1; //Field frame type in RXQ-ID filter is N/A, reset value: 0x0, access type: RW
		uint32 rxqIdFilter4FrameSubtype : 4; //Field frame subtype in RXQ-ID filter, reset value: 0x0, access type: RW
		uint32 rxqIdFilter4SubtypeNa : 1; //Field frame subtype in RXQ-ID filter is N/A, reset value: 0x0, access type: RW
		uint32 rxqIdFilter4SubtypeEqual : 1; //Subtype compare logic: , 1: logic of equal , 0: logic of not equal, reset value: 0x0, access type: RW
		uint32 reserved1 : 2;
		uint32 rxqIdFilter4Addr2IdVal : 1; //Address2 index valid bit in RXQ-ID filter, reset value: 0x0, access type: RW
		uint32 rxqIdFilter4Addr2IdValNa : 1; //Address2 index valid bit in RXQ-ID filter is N/A, reset value: 0x0, access type: RW
		uint32 rxqIdFilter4Addr1Unicast : 1; //Address1 unicast bit in RXQ-ID filter, reset value: 0x0, access type: RW
		uint32 rxqIdFilter4Addr1UnicastNa : 1; //Address1 unicast bit in RXQ-ID filter is N/A, reset value: 0x0, access type: RW
		uint32 reserved2 : 15;
	} bitFields;
} RegB2RxClassifierRxqIdFilter4_u;

/*REG_B2_RX_CLASSIFIER_RXQ_ID_FILTER5 0x74 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxqIdFilter5DropEn : 1; //Enable RXQ-ID filter drop indication, reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 rxqIdFilter5FrameType : 2; //Field frame type in RXQ-ID filter, reset value: 0x0, access type: RW
		uint32 rxqIdFilter5TypeNa : 1; //Field frame type in RXQ-ID filter is N/A, reset value: 0x0, access type: RW
		uint32 rxqIdFilter5FrameSubtype : 4; //Field frame subtype in RXQ-ID filter, reset value: 0x0, access type: RW
		uint32 rxqIdFilter5SubtypeNa : 1; //Field frame subtype in RXQ-ID filter is N/A, reset value: 0x0, access type: RW
		uint32 rxqIdFilter5SubtypeEqual : 1; //Subtype compare logic: , 1: logic of equal , 0: logic of not equal, reset value: 0x0, access type: RW
		uint32 reserved1 : 2;
		uint32 rxqIdFilter5Addr2IdVal : 1; //Address2 index valid bit in RXQ-ID filter, reset value: 0x0, access type: RW
		uint32 rxqIdFilter5Addr2IdValNa : 1; //Address2 index valid bit in RXQ-ID filter is N/A, reset value: 0x0, access type: RW
		uint32 rxqIdFilter5Addr1Unicast : 1; //Address1 unicast bit in RXQ-ID filter, reset value: 0x0, access type: RW
		uint32 rxqIdFilter5Addr1UnicastNa : 1; //Address1 unicast bit in RXQ-ID filter is N/A, reset value: 0x0, access type: RW
		uint32 reserved2 : 15;
	} bitFields;
} RegB2RxClassifierRxqIdFilter5_u;

/*REG_B2_RX_CLASSIFIER_RXQ_ID_FILTER6 0x78 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxqIdFilter6DropEn : 1; //Enable RXQ-ID filter drop indication, reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 rxqIdFilter6FrameType : 2; //Field frame type in RXQ-ID filter, reset value: 0x0, access type: RW
		uint32 rxqIdFilter6TypeNa : 1; //Field frame type in RXQ-ID filter is N/A, reset value: 0x0, access type: RW
		uint32 rxqIdFilter6FrameSubtype : 4; //Field frame subtype in RXQ-ID filter, reset value: 0x0, access type: RW
		uint32 rxqIdFilter6SubtypeNa : 1; //Field frame subtype in RXQ-ID filter is N/A, reset value: 0x0, access type: RW
		uint32 rxqIdFilter6SubtypeEqual : 1; //Subtype compare logic: , 1: logic of equal , 0: logic of not equal, reset value: 0x0, access type: RW
		uint32 reserved1 : 2;
		uint32 rxqIdFilter6Addr2IdVal : 1; //Address2 index valid bit in RXQ-ID filter, reset value: 0x0, access type: RW
		uint32 rxqIdFilter6Addr2IdValNa : 1; //Address2 index valid bit in RXQ-ID filter is N/A, reset value: 0x0, access type: RW
		uint32 rxqIdFilter6Addr1Unicast : 1; //Address1 unicast bit in RXQ-ID filter, reset value: 0x0, access type: RW
		uint32 rxqIdFilter6Addr1UnicastNa : 1; //Address1 unicast bit in RXQ-ID filter is N/A, reset value: 0x0, access type: RW
		uint32 reserved2 : 15;
	} bitFields;
} RegB2RxClassifierRxqIdFilter6_u;

/*REG_B2_RX_CLASSIFIER_RXQ_ID_FILTER7 0x7C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxqIdFilter7DropEn : 1; //Enable RXQ-ID filter drop indication, reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 rxqIdFilter7FrameType : 2; //Field frame type in RXQ-ID filter, reset value: 0x0, access type: RW
		uint32 rxqIdFilter7TypeNa : 1; //Field frame type in RXQ-ID filter is N/A, reset value: 0x0, access type: RW
		uint32 rxqIdFilter7FrameSubtype : 4; //Field frame subtype in RXQ-ID filter, reset value: 0x0, access type: RW
		uint32 rxqIdFilter7SubtypeNa : 1; //Field frame subtype in RXQ-ID filter is N/A, reset value: 0x0, access type: RW
		uint32 rxqIdFilter7SubtypeEqual : 1; //Subtype compare logic: , 1: logic of equal , 0: logic of not equal, reset value: 0x0, access type: RW
		uint32 reserved1 : 2;
		uint32 rxqIdFilter7Addr2IdVal : 1; //Address2 index valid bit in RXQ-ID filter, reset value: 0x0, access type: RW
		uint32 rxqIdFilter7Addr2IdValNa : 1; //Address2 index valid bit in RXQ-ID filter is N/A, reset value: 0x0, access type: RW
		uint32 rxqIdFilter7Addr1Unicast : 1; //Address1 unicast bit in RXQ-ID filter, reset value: 0x0, access type: RW
		uint32 rxqIdFilter7Addr1UnicastNa : 1; //Address1 unicast bit in RXQ-ID filter is N/A, reset value: 0x0, access type: RW
		uint32 reserved2 : 15;
	} bitFields;
} RegB2RxClassifierRxqIdFilter7_u;

/*REG_B2_RX_CLASSIFIER_RXQ_ID_FILTER8 0x80 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxqIdFilter8DropEn : 1; //Enable RXQ-ID filter drop indication, reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 rxqIdFilter8FrameType : 2; //Field frame type in RXQ-ID filter, reset value: 0x0, access type: RW
		uint32 rxqIdFilter8TypeNa : 1; //Field frame type in RXQ-ID filter is N/A, reset value: 0x0, access type: RW
		uint32 rxqIdFilter8FrameSubtype : 4; //Field frame subtype in RXQ-ID filter, reset value: 0x0, access type: RW
		uint32 rxqIdFilter8SubtypeNa : 1; //Field frame subtype in RXQ-ID filter is N/A, reset value: 0x0, access type: RW
		uint32 rxqIdFilter8SubtypeEqual : 1; //Subtype compare logic: , 1: logic of equal , 0: logic of not equal, reset value: 0x0, access type: RW
		uint32 reserved1 : 2;
		uint32 rxqIdFilter8Addr2IdVal : 1; //Address2 index valid bit in RXQ-ID filter, reset value: 0x0, access type: RW
		uint32 rxqIdFilter8Addr2IdValNa : 1; //Address2 index valid bit in RXQ-ID filter is N/A, reset value: 0x0, access type: RW
		uint32 rxqIdFilter8Addr1Unicast : 1; //Address1 unicast bit in RXQ-ID filter, reset value: 0x0, access type: RW
		uint32 rxqIdFilter8Addr1UnicastNa : 1; //Address1 unicast bit in RXQ-ID filter is N/A, reset value: 0x0, access type: RW
		uint32 reserved2 : 15;
	} bitFields;
} RegB2RxClassifierRxqIdFilter8_u;

/*REG_B2_RX_CLASSIFIER_RXQ_ID_FILTER9 0x84 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxqIdFilter9DropEn : 1; //Enable RXQ-ID filter drop indication, reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 rxqIdFilter9FrameType : 2; //Field frame type in RXQ-ID filter, reset value: 0x0, access type: RW
		uint32 rxqIdFilter9TypeNa : 1; //Field frame type in RXQ-ID filter is N/A, reset value: 0x0, access type: RW
		uint32 rxqIdFilter9FrameSubtype : 4; //Field frame subtype in RXQ-ID filter, reset value: 0x0, access type: RW
		uint32 rxqIdFilter9SubtypeNa : 1; //Field frame subtype in RXQ-ID filter is N/A, reset value: 0x0, access type: RW
		uint32 rxqIdFilter9SubtypeEqual : 1; //Subtype compare logic: , 1: logic of equal , 0: logic of not equal, reset value: 0x0, access type: RW
		uint32 reserved1 : 2;
		uint32 rxqIdFilter9Addr2IdVal : 1; //Address2 index valid bit in RXQ-ID filter, reset value: 0x0, access type: RW
		uint32 rxqIdFilter9Addr2IdValNa : 1; //Address2 index valid bit in RXQ-ID filter is N/A, reset value: 0x0, access type: RW
		uint32 rxqIdFilter9Addr1Unicast : 1; //Address1 unicast bit in RXQ-ID filter, reset value: 0x0, access type: RW
		uint32 rxqIdFilter9Addr1UnicastNa : 1; //Address1 unicast bit in RXQ-ID filter is N/A, reset value: 0x0, access type: RW
		uint32 reserved2 : 15;
	} bitFields;
} RegB2RxClassifierRxqIdFilter9_u;

/*REG_B2_RX_CLASSIFIER_RXQ_ID_FILTER10 0x88 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxqIdFilter10DropEn : 1; //Enable RXQ-ID filter drop indication, reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 rxqIdFilter10FrameType : 2; //Field frame type in RXQ-ID filter, reset value: 0x0, access type: RW
		uint32 rxqIdFilter10TypeNa : 1; //Field frame type in RXQ-ID filter is N/A, reset value: 0x0, access type: RW
		uint32 rxqIdFilter10FrameSubtype : 4; //Field frame subtype in RXQ-ID filter, reset value: 0x0, access type: RW
		uint32 rxqIdFilter10SubtypeNa : 1; //Field frame subtype in RXQ-ID filter is N/A, reset value: 0x0, access type: RW
		uint32 rxqIdFilter10SubtypeEqual : 1; //Subtype compare logic: , 1: logic of equal , 0: logic of not equal, reset value: 0x0, access type: RW
		uint32 reserved1 : 2;
		uint32 rxqIdFilter10Addr2IdVal : 1; //Address2 index valid bit in RXQ-ID filter, reset value: 0x0, access type: RW
		uint32 rxqIdFilter10Addr2IdValNa : 1; //Address2 index valid bit in RXQ-ID filter is N/A, reset value: 0x0, access type: RW
		uint32 rxqIdFilter10Addr1Unicast : 1; //Address1 unicast bit in RXQ-ID filter, reset value: 0x0, access type: RW
		uint32 rxqIdFilter10Addr1UnicastNa : 1; //Address1 unicast bit in RXQ-ID filter is N/A, reset value: 0x0, access type: RW
		uint32 reserved2 : 15;
	} bitFields;
} RegB2RxClassifierRxqIdFilter10_u;

/*REG_B2_RX_CLASSIFIER_RXQ_ID_FILTER11 0x8C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxqIdFilter11DropEn : 1; //Enable RXQ-ID filter drop indication, reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 rxqIdFilter11FrameType : 2; //Field frame type in RXQ-ID filter, reset value: 0x0, access type: RW
		uint32 rxqIdFilter11TypeNa : 1; //Field frame type in RXQ-ID filter is N/A, reset value: 0x0, access type: RW
		uint32 rxqIdFilter11FrameSubtype : 4; //Field frame subtype in RXQ-ID filter, reset value: 0x0, access type: RW
		uint32 rxqIdFilter11SubtypeNa : 1; //Field frame subtype in RXQ-ID filter is N/A, reset value: 0x0, access type: RW
		uint32 rxqIdFilter11SubtypeEqual : 1; //Subtype compare logic: , 1: logic of equal , 0: logic of not equal, reset value: 0x0, access type: RW
		uint32 reserved1 : 2;
		uint32 rxqIdFilter11Addr2IdVal : 1; //Address2 index valid bit in RXQ-ID filter, reset value: 0x0, access type: RW
		uint32 rxqIdFilter11Addr2IdValNa : 1; //Address2 index valid bit in RXQ-ID filter is N/A, reset value: 0x0, access type: RW
		uint32 rxqIdFilter11Addr1Unicast : 1; //Address1 unicast bit in RXQ-ID filter, reset value: 0x0, access type: RW
		uint32 rxqIdFilter11Addr1UnicastNa : 1; //Address1 unicast bit in RXQ-ID filter is N/A, reset value: 0x0, access type: RW
		uint32 reserved2 : 15;
	} bitFields;
} RegB2RxClassifierRxqIdFilter11_u;

/*REG_B2_RX_CLASSIFIER_RXQ_ID_FILTER12 0x90 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxqIdFilter12DropEn : 1; //Enable RXQ-ID filter drop indication, reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 rxqIdFilter12FrameType : 2; //Field frame type in RXQ-ID filter, reset value: 0x0, access type: RW
		uint32 rxqIdFilter12TypeNa : 1; //Field frame type in RXQ-ID filter is N/A, reset value: 0x0, access type: RW
		uint32 rxqIdFilter12FrameSubtype : 4; //Field frame subtype in RXQ-ID filter, reset value: 0x0, access type: RW
		uint32 rxqIdFilter12SubtypeNa : 1; //Field frame subtype in RXQ-ID filter is N/A, reset value: 0x0, access type: RW
		uint32 rxqIdFilter12SubtypeEqual : 1; //Subtype compare logic: , 1: logic of equal , 0: logic of not equal, reset value: 0x0, access type: RW
		uint32 reserved1 : 2;
		uint32 rxqIdFilter12Addr2IdVal : 1; //Address2 index valid bit in RXQ-ID filter, reset value: 0x0, access type: RW
		uint32 rxqIdFilter12Addr2IdValNa : 1; //Address2 index valid bit in RXQ-ID filter is N/A, reset value: 0x0, access type: RW
		uint32 rxqIdFilter12Addr1Unicast : 1; //Address1 unicast bit in RXQ-ID filter, reset value: 0x0, access type: RW
		uint32 rxqIdFilter12Addr1UnicastNa : 1; //Address1 unicast bit in RXQ-ID filter is N/A, reset value: 0x0, access type: RW
		uint32 reserved2 : 15;
	} bitFields;
} RegB2RxClassifierRxqIdFilter12_u;

/*REG_B2_RX_CLASSIFIER_RXQ_ID_FILTER13 0x94 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxqIdFilter13DropEn : 1; //Enable RXQ-ID filter drop indication, reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 rxqIdFilter13FrameType : 2; //Field frame type in RXQ-ID filter, reset value: 0x0, access type: RW
		uint32 rxqIdFilter13TypeNa : 1; //Field frame type in RXQ-ID filter is N/A, reset value: 0x0, access type: RW
		uint32 rxqIdFilter13FrameSubtype : 4; //Field frame subtype in RXQ-ID filter, reset value: 0x0, access type: RW
		uint32 rxqIdFilter13SubtypeNa : 1; //Field frame subtype in RXQ-ID filter is N/A, reset value: 0x0, access type: RW
		uint32 rxqIdFilter13SubtypeEqual : 1; //Subtype compare logic: , 1: logic of equal , 0: logic of not equal, reset value: 0x0, access type: RW
		uint32 reserved1 : 2;
		uint32 rxqIdFilter13Addr2IdVal : 1; //Address2 index valid bit in RXQ-ID filter, reset value: 0x0, access type: RW
		uint32 rxqIdFilter13Addr2IdValNa : 1; //Address2 index valid bit in RXQ-ID filter is N/A, reset value: 0x0, access type: RW
		uint32 rxqIdFilter13Addr1Unicast : 1; //Address1 unicast bit in RXQ-ID filter, reset value: 0x0, access type: RW
		uint32 rxqIdFilter13Addr1UnicastNa : 1; //Address1 unicast bit in RXQ-ID filter is N/A, reset value: 0x0, access type: RW
		uint32 reserved2 : 15;
	} bitFields;
} RegB2RxClassifierRxqIdFilter13_u;

/*REG_B2_RX_CLASSIFIER_RXQ_ID_FILTER14 0x98 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxqIdFilter14DropEn : 1; //Enable RXQ-ID filter drop indication, reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 rxqIdFilter14FrameType : 2; //Field frame type in RXQ-ID filter, reset value: 0x0, access type: RW
		uint32 rxqIdFilter14TypeNa : 1; //Field frame type in RXQ-ID filter is N/A, reset value: 0x0, access type: RW
		uint32 rxqIdFilter14FrameSubtype : 4; //Field frame subtype in RXQ-ID filter, reset value: 0x0, access type: RW
		uint32 rxqIdFilter14SubtypeNa : 1; //Field frame subtype in RXQ-ID filter is N/A, reset value: 0x0, access type: RW
		uint32 rxqIdFilter14SubtypeEqual : 1; //Subtype compare logic: , 1: logic of equal , 0: logic of not equal, reset value: 0x0, access type: RW
		uint32 reserved1 : 2;
		uint32 rxqIdFilter14Addr2IdVal : 1; //Address2 index valid bit in RXQ-ID filter, reset value: 0x0, access type: RW
		uint32 rxqIdFilter14Addr2IdValNa : 1; //Address2 index valid bit in RXQ-ID filter is N/A, reset value: 0x0, access type: RW
		uint32 rxqIdFilter14Addr1Unicast : 1; //Address1 unicast bit in RXQ-ID filter, reset value: 0x0, access type: RW
		uint32 rxqIdFilter14Addr1UnicastNa : 1; //Address1 unicast bit in RXQ-ID filter is N/A, reset value: 0x0, access type: RW
		uint32 reserved2 : 15;
	} bitFields;
} RegB2RxClassifierRxqIdFilter14_u;

/*REG_B2_RX_CLASSIFIER_RXQ_ID_FILTER15 0x9C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxqIdFilter15DropEn : 1; //Enable RXQ-ID filter drop indication, reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 rxqIdFilter15FrameType : 2; //Field frame type in RXQ-ID filter, reset value: 0x0, access type: RW
		uint32 rxqIdFilter15TypeNa : 1; //Field frame type in RXQ-ID filter is N/A, reset value: 0x0, access type: RW
		uint32 rxqIdFilter15FrameSubtype : 4; //Field frame subtype in RXQ-ID filter, reset value: 0x0, access type: RW
		uint32 rxqIdFilter15SubtypeNa : 1; //Field frame subtype in RXQ-ID filter is N/A, reset value: 0x0, access type: RW
		uint32 rxqIdFilter15SubtypeEqual : 1; //Subtype compare logic: , 1: logic of equal , 0: logic of not equal, reset value: 0x0, access type: RW
		uint32 reserved1 : 2;
		uint32 rxqIdFilter15Addr2IdVal : 1; //Address2 index valid bit in RXQ-ID filter, reset value: 0x0, access type: RW
		uint32 rxqIdFilter15Addr2IdValNa : 1; //Address2 index valid bit in RXQ-ID filter is N/A, reset value: 0x0, access type: RW
		uint32 rxqIdFilter15Addr1Unicast : 1; //Address1 unicast bit in RXQ-ID filter, reset value: 0x0, access type: RW
		uint32 rxqIdFilter15Addr1UnicastNa : 1; //Address1 unicast bit in RXQ-ID filter is N/A, reset value: 0x0, access type: RW
		uint32 reserved2 : 15;
	} bitFields;
} RegB2RxClassifierRxqIdFilter15_u;

/*REG_B2_RX_CLASSIFIER_RXQ_ID_FILTER_CONTROL 0xA0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxqIdFilterGroupAndLogic : 3; //The number of filters minus one (starting from filter #0) which gathered as AND group, i.e. the result will be only if all the filters till this value indicate as DROP/Unsupported., reset value: 0x0, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB2RxClassifierRxqIdFilterControl_u;

/*REG_B2_RX_CLASSIFIER_RX_CLASSIFIER_ENABLE 0xB0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxClasTopEn : 1; //Rx Classifier enable, reset value: 0x0, access type: RW
		uint32 rxClasSecurityEn : 1; //Enable Security engine initiation, reset value: 0x1, access type: RW
		uint32 rxClasRtdGenerationEn : 1; //Enable RD template generation, reset value: 0x1, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB2RxClassifierRxClassifierEnable_u;

/*REG_B2_RX_CLASSIFIER_WEP_KEY_IDX_DB_LOCATION 0xB4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 wepWdsKeyIdxDbLocation : 1; //WEP Key Index Location in case of unicast WDS: , 0 --- STA DB , 1 --- VAP DB, reset value: 0x0, access type: RW
		uint32 wepNoWdsKeyIdxDbLocation : 1; //WEP Key Index Location in case of unicast no WDS: , 0 --- STA DB , 1 --- VAP DB, reset value: 0x0, access type: RW
		uint32 reserved0 : 30;
	} bitFields;
} RegB2RxClassifierWepKeyIdxDbLocation_u;

/*REG_B2_RX_CLASSIFIER_RX_CLAS_RTD_CTRL 0x100 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 5;
		uint32 lengthCalcNegativeDropEn : 1; //Set RD type DROP in case of negative length result, reset value: 0x1, access type: RW
		uint32 misDataProtFrameDbDropEn : 1; //Set RD type DROP in case of mismatch between frame protected bit and security enable in DB for Data Frames., reset value: 0x1, access type: RW
		uint32 reserved1 : 25;
	} bitFields;
} RegB2RxClassifierRxClasRtdCtrl_u;

/*REG_B2_RX_CLASSIFIER_RX_CLAS_SM_DBG 0x114 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxClassifierSm : 4; //Main Rx Classifier SM, reset value: 0x0, access type: RO
		uint32 rdCirBufSm : 2; //Read Cir buffer SM, reset value: 0x0, access type: RO
		uint32 rdMpduDescSm : 2; //Read MPDU desc SM, reset value: 0x0, access type: RO
		uint32 rxqIdSm : 2; //RxQ ID SM, reset value: 0x0, access type: RO
		uint32 secInitSm : 4; //Security init SM, reset value: 0x0, access type: RO
		uint32 reserved0 : 4;
		uint32 rmdFillSm : 3; //Rx MPDU desc fill SM, reset value: 0x0, access type: RO
		uint32 dlmIfSm : 2; //DLM IF SM, reset value: 0x0, access type: RO
		uint32 mpduFieldsFilterSm : 3; //MPDU fields filter SM, reset value: 0x0, access type: RO
		uint32 rdSecHdrSm : 2; //Read Security header SM, reset value: 0x0, access type: RO
		uint32 reserved1 : 4;
	} bitFields;
} RegB2RxClassifierRxClasSmDbg_u;

/*REG_B2_RX_CLASSIFIER_RX_CLAS_LOGGER_CONTROL 0x134 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxClasLoggerEn : 1; //Rx Classifier logger enable, reset value: 0x0, access type: RW
		uint32 reserved0 : 7;
		uint32 rxClasLoggerPriority : 2; //Rx Classifier logger priority, reset value: 0x0, access type: RW
		uint32 reserved1 : 22;
	} bitFields;
} RegB2RxClassifierRxClasLoggerControl_u;

/*REG_B2_RX_CLASSIFIER_RX_CLAS_LOGGER_ACTIVE 0x138 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxClasLoggerActive : 1; //Rx Classifier logger active indication, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB2RxClassifierRxClasLoggerActive_u;

/*REG_B2_RX_CLASSIFIER_SPARE_REGISTERS 0x1FC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 spareRegisters : 16; //Spare registers, reset value: 0x8888, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB2RxClassifierSpareRegisters_u;

//========================================
/* REG_RX_CLASSIFIER_FRAME_CLASS_FILTER_CONTROL 0x0 */
#define B2_RX_CLASSIFIER_FRAME_CLASS_FILTER_CONTROL_FIELD_FRAME_CLASS_FILTER_ADDR2_VALID_DROP_EN_MASK             0x00000001
#define B2_RX_CLASSIFIER_FRAME_CLASS_FILTER_CONTROL_FIELD_FRAME_CLASS_FILTER_ADDR2_INVALID_DROP_EN_MASK           0x00000002
#define B2_RX_CLASSIFIER_FRAME_CLASS_FILTER_CONTROL_FIELD_VAP_STA_MIS_FILTER_DROP_EN_MASK                         0x00000004
#define B2_RX_CLASSIFIER_FRAME_CLASS_FILTER_CONTROL_FIELD_ADDR2_INVALID_DROP_MPDU_CLASS_VAL_MASK                  0x00000030
#define B2_RX_CLASSIFIER_FRAME_CLASS_FILTER_CONTROL_FIELD_VAP_STA_MIS_DROP_MPDU_CLASS_VAL_MASK                    0x000000C0
//========================================
/* REG_RX_CLASSIFIER_FRAME_DATA_SUBTYPES_CLASS 0x4 */
#define B2_RX_CLASSIFIER_FRAME_DATA_SUBTYPES_CLASS_FIELD_FRAME_DATA_SUBTYPES_CLASS_MASK                           0xFFFFFFFF
//========================================
/* REG_RX_CLASSIFIER_FRAME_CTRL_SUBTYPES_CLASS 0x8 */
#define B2_RX_CLASSIFIER_FRAME_CTRL_SUBTYPES_CLASS_FIELD_FRAME_CTRL_SUBTYPES_CLASS_MASK                           0xFFFFFFFF
//========================================
/* REG_RX_CLASSIFIER_FRAME_MGMT_SUBTYPES_CLASS 0xC */
#define B2_RX_CLASSIFIER_FRAME_MGMT_SUBTYPES_CLASS_FIELD_FRAME_MGMT_SUBTYPES_CLASS_MASK                           0xFFFFFFFF
//========================================
/* REG_RX_CLASSIFIER_FRAME_MGMT_ACTION_CLASS 0x10 */
#define B2_RX_CLASSIFIER_FRAME_MGMT_ACTION_CLASS_FIELD_FRAME_MGMT_ACTION4_CLASS_MASK                              0x00000003
#define B2_RX_CLASSIFIER_FRAME_MGMT_ACTION_CLASS_FIELD_FRAME_MGMT_ACTION15_CLASS_MASK                             0x0000000C
#define B2_RX_CLASSIFIER_FRAME_MGMT_ACTION_CLASS_FIELD_FRAME_MGMT_ACTION_OTHER_CLASS_MASK                         0x00000030
//========================================
/* REG_RX_CLASSIFIER_DLM_SEC_LIST_IDX 0x40 */
#define B2_RX_CLASSIFIER_DLM_SEC_LIST_IDX_FIELD_DLM_SEC_LIST_IDX_MASK                                             0x000000FF
//========================================
/* REG_RX_CLASSIFIER_DLM_IF_DBG 0x44 */
#define B2_RX_CLASSIFIER_DLM_IF_DBG_FIELD_DLM_OUT_FIFO_NUM_ENTRIES_MASK                                           0x00000003
#define B2_RX_CLASSIFIER_DLM_IF_DBG_FIELD_DLM_OUT_FIFO_CLEAN_STB_MASK                                             0x00000080
#define B2_RX_CLASSIFIER_DLM_IF_DBG_FIELD_DLM_IN_FIFO_NUM_ENTRIES_MASK                                            0x00000300
#define B2_RX_CLASSIFIER_DLM_IF_DBG_FIELD_DLM_IN_FIFO_CLEAN_STB_MASK                                              0x00008000
//========================================
/* REG_RX_CLASSIFIER_WPA_SEC_PASS_THROUGH_VAP_EN 0x48 */
#define B2_RX_CLASSIFIER_WPA_SEC_PASS_THROUGH_VAP_EN_FIELD_WPA_SEC_PASS_THROUGH_VAP_EN_MASK                       0xFFFFFFFF
//========================================
/* REG_RX_CLASSIFIER_MPDU_HDR_FIELDS_FILTER_EN 0x4C */
#define B2_RX_CLASSIFIER_MPDU_HDR_FIELDS_FILTER_EN_FIELD_MPDU_HDR_FC_FIELDS_FILTER_EN_MASK                        0x000000FF
#define B2_RX_CLASSIFIER_MPDU_HDR_FIELDS_FILTER_EN_FIELD_MPDU_HDR_QOS_FIELDS_FILTER_EN_MASK                       0x0000FF00
#define B2_RX_CLASSIFIER_MPDU_HDR_FIELDS_FILTER_EN_FIELD_MPDU_HDR_HTC_FIELDS_FILTER_EN_MASK                       0x00FF0000
//========================================
/* REG_RX_CLASSIFIER_RD_TYPE_DROP_VALUE 0x50 */
#define B2_RX_CLASSIFIER_RD_TYPE_DROP_VALUE_FIELD_RD_TYPE_DROP_VALUE_MASK                                         0x0000001F
//========================================
/* REG_RX_CLASSIFIER_RXQ_ID_FILTER0 0x60 */
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER0_FIELD_RXQ_ID_FILTER0_DROP_EN_MASK                                         0x00000001
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER0_FIELD_RXQ_ID_FILTER0_FRAME_TYPE_MASK                                      0x0000000C
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER0_FIELD_RXQ_ID_FILTER0_TYPE_NA_MASK                                         0x00000010
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER0_FIELD_RXQ_ID_FILTER0_FRAME_SUBTYPE_MASK                                   0x000001E0
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER0_FIELD_RXQ_ID_FILTER0_SUBTYPE_NA_MASK                                      0x00000200
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER0_FIELD_RXQ_ID_FILTER0_SUBTYPE_EQUAL_MASK                                   0x00000400
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER0_FIELD_RXQ_ID_FILTER0_ADDR2_ID_VAL_MASK                                    0x00002000
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER0_FIELD_RXQ_ID_FILTER0_ADDR2_ID_VAL_NA_MASK                                 0x00004000
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER0_FIELD_RXQ_ID_FILTER0_ADDR1_UNICAST_MASK                                   0x00008000
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER0_FIELD_RXQ_ID_FILTER0_ADDR1_UNICAST_NA_MASK                                0x00010000
//========================================
/* REG_RX_CLASSIFIER_RXQ_ID_FILTER1 0x64 */
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER1_FIELD_RXQ_ID_FILTER1_DROP_EN_MASK                                         0x00000001
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER1_FIELD_RXQ_ID_FILTER1_FRAME_TYPE_MASK                                      0x0000000C
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER1_FIELD_RXQ_ID_FILTER1_TYPE_NA_MASK                                         0x00000010
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER1_FIELD_RXQ_ID_FILTER1_FRAME_SUBTYPE_MASK                                   0x000001E0
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER1_FIELD_RXQ_ID_FILTER1_SUBTYPE_NA_MASK                                      0x00000200
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER1_FIELD_RXQ_ID_FILTER1_SUBTYPE_EQUAL_MASK                                   0x00000400
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER1_FIELD_RXQ_ID_FILTER1_ADDR2_ID_VAL_MASK                                    0x00002000
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER1_FIELD_RXQ_ID_FILTER1_ADDR2_ID_VAL_NA_MASK                                 0x00004000
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER1_FIELD_RXQ_ID_FILTER1_ADDR1_UNICAST_MASK                                   0x00008000
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER1_FIELD_RXQ_ID_FILTER1_ADDR1_UNICAST_NA_MASK                                0x00010000
//========================================
/* REG_RX_CLASSIFIER_RXQ_ID_FILTER2 0x68 */
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER2_FIELD_RXQ_ID_FILTER2_DROP_EN_MASK                                         0x00000001
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER2_FIELD_RXQ_ID_FILTER2_FRAME_TYPE_MASK                                      0x0000000C
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER2_FIELD_RXQ_ID_FILTER2_TYPE_NA_MASK                                         0x00000010
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER2_FIELD_RXQ_ID_FILTER2_FRAME_SUBTYPE_MASK                                   0x000001E0
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER2_FIELD_RXQ_ID_FILTER2_SUBTYPE_NA_MASK                                      0x00000200
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER2_FIELD_RXQ_ID_FILTER2_SUBTYPE_EQUAL_MASK                                   0x00000400
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER2_FIELD_RXQ_ID_FILTER2_ADDR2_ID_VAL_MASK                                    0x00002000
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER2_FIELD_RXQ_ID_FILTER2_ADDR2_ID_VAL_NA_MASK                                 0x00004000
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER2_FIELD_RXQ_ID_FILTER2_ADDR1_UNICAST_MASK                                   0x00008000
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER2_FIELD_RXQ_ID_FILTER2_ADDR1_UNICAST_NA_MASK                                0x00010000
//========================================
/* REG_RX_CLASSIFIER_RXQ_ID_FILTER3 0x6C */
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER3_FIELD_RXQ_ID_FILTER3_DROP_EN_MASK                                         0x00000001
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER3_FIELD_RXQ_ID_FILTER3_FRAME_TYPE_MASK                                      0x0000000C
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER3_FIELD_RXQ_ID_FILTER3_TYPE_NA_MASK                                         0x00000010
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER3_FIELD_RXQ_ID_FILTER3_FRAME_SUBTYPE_MASK                                   0x000001E0
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER3_FIELD_RXQ_ID_FILTER3_SUBTYPE_NA_MASK                                      0x00000200
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER3_FIELD_RXQ_ID_FILTER3_SUBTYPE_EQUAL_MASK                                   0x00000400
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER3_FIELD_RXQ_ID_FILTER3_ADDR2_ID_VAL_MASK                                    0x00002000
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER3_FIELD_RXQ_ID_FILTER3_ADDR2_ID_VAL_NA_MASK                                 0x00004000
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER3_FIELD_RXQ_ID_FILTER3_ADDR1_UNICAST_MASK                                   0x00008000
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER3_FIELD_RXQ_ID_FILTER3_ADDR1_UNICAST_NA_MASK                                0x00010000
//========================================
/* REG_RX_CLASSIFIER_RXQ_ID_FILTER4 0x70 */
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER4_FIELD_RXQ_ID_FILTER4_DROP_EN_MASK                                         0x00000001
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER4_FIELD_RXQ_ID_FILTER4_FRAME_TYPE_MASK                                      0x0000000C
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER4_FIELD_RXQ_ID_FILTER4_TYPE_NA_MASK                                         0x00000010
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER4_FIELD_RXQ_ID_FILTER4_FRAME_SUBTYPE_MASK                                   0x000001E0
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER4_FIELD_RXQ_ID_FILTER4_SUBTYPE_NA_MASK                                      0x00000200
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER4_FIELD_RXQ_ID_FILTER4_SUBTYPE_EQUAL_MASK                                   0x00000400
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER4_FIELD_RXQ_ID_FILTER4_ADDR2_ID_VAL_MASK                                    0x00002000
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER4_FIELD_RXQ_ID_FILTER4_ADDR2_ID_VAL_NA_MASK                                 0x00004000
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER4_FIELD_RXQ_ID_FILTER4_ADDR1_UNICAST_MASK                                   0x00008000
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER4_FIELD_RXQ_ID_FILTER4_ADDR1_UNICAST_NA_MASK                                0x00010000
//========================================
/* REG_RX_CLASSIFIER_RXQ_ID_FILTER5 0x74 */
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER5_FIELD_RXQ_ID_FILTER5_DROP_EN_MASK                                         0x00000001
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER5_FIELD_RXQ_ID_FILTER5_FRAME_TYPE_MASK                                      0x0000000C
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER5_FIELD_RXQ_ID_FILTER5_TYPE_NA_MASK                                         0x00000010
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER5_FIELD_RXQ_ID_FILTER5_FRAME_SUBTYPE_MASK                                   0x000001E0
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER5_FIELD_RXQ_ID_FILTER5_SUBTYPE_NA_MASK                                      0x00000200
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER5_FIELD_RXQ_ID_FILTER5_SUBTYPE_EQUAL_MASK                                   0x00000400
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER5_FIELD_RXQ_ID_FILTER5_ADDR2_ID_VAL_MASK                                    0x00002000
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER5_FIELD_RXQ_ID_FILTER5_ADDR2_ID_VAL_NA_MASK                                 0x00004000
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER5_FIELD_RXQ_ID_FILTER5_ADDR1_UNICAST_MASK                                   0x00008000
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER5_FIELD_RXQ_ID_FILTER5_ADDR1_UNICAST_NA_MASK                                0x00010000
//========================================
/* REG_RX_CLASSIFIER_RXQ_ID_FILTER6 0x78 */
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER6_FIELD_RXQ_ID_FILTER6_DROP_EN_MASK                                         0x00000001
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER6_FIELD_RXQ_ID_FILTER6_FRAME_TYPE_MASK                                      0x0000000C
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER6_FIELD_RXQ_ID_FILTER6_TYPE_NA_MASK                                         0x00000010
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER6_FIELD_RXQ_ID_FILTER6_FRAME_SUBTYPE_MASK                                   0x000001E0
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER6_FIELD_RXQ_ID_FILTER6_SUBTYPE_NA_MASK                                      0x00000200
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER6_FIELD_RXQ_ID_FILTER6_SUBTYPE_EQUAL_MASK                                   0x00000400
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER6_FIELD_RXQ_ID_FILTER6_ADDR2_ID_VAL_MASK                                    0x00002000
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER6_FIELD_RXQ_ID_FILTER6_ADDR2_ID_VAL_NA_MASK                                 0x00004000
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER6_FIELD_RXQ_ID_FILTER6_ADDR1_UNICAST_MASK                                   0x00008000
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER6_FIELD_RXQ_ID_FILTER6_ADDR1_UNICAST_NA_MASK                                0x00010000
//========================================
/* REG_RX_CLASSIFIER_RXQ_ID_FILTER7 0x7C */
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER7_FIELD_RXQ_ID_FILTER7_DROP_EN_MASK                                         0x00000001
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER7_FIELD_RXQ_ID_FILTER7_FRAME_TYPE_MASK                                      0x0000000C
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER7_FIELD_RXQ_ID_FILTER7_TYPE_NA_MASK                                         0x00000010
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER7_FIELD_RXQ_ID_FILTER7_FRAME_SUBTYPE_MASK                                   0x000001E0
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER7_FIELD_RXQ_ID_FILTER7_SUBTYPE_NA_MASK                                      0x00000200
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER7_FIELD_RXQ_ID_FILTER7_SUBTYPE_EQUAL_MASK                                   0x00000400
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER7_FIELD_RXQ_ID_FILTER7_ADDR2_ID_VAL_MASK                                    0x00002000
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER7_FIELD_RXQ_ID_FILTER7_ADDR2_ID_VAL_NA_MASK                                 0x00004000
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER7_FIELD_RXQ_ID_FILTER7_ADDR1_UNICAST_MASK                                   0x00008000
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER7_FIELD_RXQ_ID_FILTER7_ADDR1_UNICAST_NA_MASK                                0x00010000
//========================================
/* REG_RX_CLASSIFIER_RXQ_ID_FILTER8 0x80 */
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER8_FIELD_RXQ_ID_FILTER8_DROP_EN_MASK                                         0x00000001
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER8_FIELD_RXQ_ID_FILTER8_FRAME_TYPE_MASK                                      0x0000000C
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER8_FIELD_RXQ_ID_FILTER8_TYPE_NA_MASK                                         0x00000010
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER8_FIELD_RXQ_ID_FILTER8_FRAME_SUBTYPE_MASK                                   0x000001E0
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER8_FIELD_RXQ_ID_FILTER8_SUBTYPE_NA_MASK                                      0x00000200
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER8_FIELD_RXQ_ID_FILTER8_SUBTYPE_EQUAL_MASK                                   0x00000400
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER8_FIELD_RXQ_ID_FILTER8_ADDR2_ID_VAL_MASK                                    0x00002000
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER8_FIELD_RXQ_ID_FILTER8_ADDR2_ID_VAL_NA_MASK                                 0x00004000
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER8_FIELD_RXQ_ID_FILTER8_ADDR1_UNICAST_MASK                                   0x00008000
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER8_FIELD_RXQ_ID_FILTER8_ADDR1_UNICAST_NA_MASK                                0x00010000
//========================================
/* REG_RX_CLASSIFIER_RXQ_ID_FILTER9 0x84 */
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER9_FIELD_RXQ_ID_FILTER9_DROP_EN_MASK                                         0x00000001
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER9_FIELD_RXQ_ID_FILTER9_FRAME_TYPE_MASK                                      0x0000000C
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER9_FIELD_RXQ_ID_FILTER9_TYPE_NA_MASK                                         0x00000010
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER9_FIELD_RXQ_ID_FILTER9_FRAME_SUBTYPE_MASK                                   0x000001E0
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER9_FIELD_RXQ_ID_FILTER9_SUBTYPE_NA_MASK                                      0x00000200
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER9_FIELD_RXQ_ID_FILTER9_SUBTYPE_EQUAL_MASK                                   0x00000400
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER9_FIELD_RXQ_ID_FILTER9_ADDR2_ID_VAL_MASK                                    0x00002000
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER9_FIELD_RXQ_ID_FILTER9_ADDR2_ID_VAL_NA_MASK                                 0x00004000
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER9_FIELD_RXQ_ID_FILTER9_ADDR1_UNICAST_MASK                                   0x00008000
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER9_FIELD_RXQ_ID_FILTER9_ADDR1_UNICAST_NA_MASK                                0x00010000
//========================================
/* REG_RX_CLASSIFIER_RXQ_ID_FILTER10 0x88 */
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER10_FIELD_RXQ_ID_FILTER10_DROP_EN_MASK                                       0x00000001
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER10_FIELD_RXQ_ID_FILTER10_FRAME_TYPE_MASK                                    0x0000000C
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER10_FIELD_RXQ_ID_FILTER10_TYPE_NA_MASK                                       0x00000010
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER10_FIELD_RXQ_ID_FILTER10_FRAME_SUBTYPE_MASK                                 0x000001E0
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER10_FIELD_RXQ_ID_FILTER10_SUBTYPE_NA_MASK                                    0x00000200
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER10_FIELD_RXQ_ID_FILTER10_SUBTYPE_EQUAL_MASK                                 0x00000400
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER10_FIELD_RXQ_ID_FILTER10_ADDR2_ID_VAL_MASK                                  0x00002000
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER10_FIELD_RXQ_ID_FILTER10_ADDR2_ID_VAL_NA_MASK                               0x00004000
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER10_FIELD_RXQ_ID_FILTER10_ADDR1_UNICAST_MASK                                 0x00008000
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER10_FIELD_RXQ_ID_FILTER10_ADDR1_UNICAST_NA_MASK                              0x00010000
//========================================
/* REG_RX_CLASSIFIER_RXQ_ID_FILTER11 0x8C */
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER11_FIELD_RXQ_ID_FILTER11_DROP_EN_MASK                                       0x00000001
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER11_FIELD_RXQ_ID_FILTER11_FRAME_TYPE_MASK                                    0x0000000C
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER11_FIELD_RXQ_ID_FILTER11_TYPE_NA_MASK                                       0x00000010
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER11_FIELD_RXQ_ID_FILTER11_FRAME_SUBTYPE_MASK                                 0x000001E0
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER11_FIELD_RXQ_ID_FILTER11_SUBTYPE_NA_MASK                                    0x00000200
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER11_FIELD_RXQ_ID_FILTER11_SUBTYPE_EQUAL_MASK                                 0x00000400
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER11_FIELD_RXQ_ID_FILTER11_ADDR2_ID_VAL_MASK                                  0x00002000
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER11_FIELD_RXQ_ID_FILTER11_ADDR2_ID_VAL_NA_MASK                               0x00004000
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER11_FIELD_RXQ_ID_FILTER11_ADDR1_UNICAST_MASK                                 0x00008000
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER11_FIELD_RXQ_ID_FILTER11_ADDR1_UNICAST_NA_MASK                              0x00010000
//========================================
/* REG_RX_CLASSIFIER_RXQ_ID_FILTER12 0x90 */
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER12_FIELD_RXQ_ID_FILTER12_DROP_EN_MASK                                       0x00000001
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER12_FIELD_RXQ_ID_FILTER12_FRAME_TYPE_MASK                                    0x0000000C
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER12_FIELD_RXQ_ID_FILTER12_TYPE_NA_MASK                                       0x00000010
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER12_FIELD_RXQ_ID_FILTER12_FRAME_SUBTYPE_MASK                                 0x000001E0
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER12_FIELD_RXQ_ID_FILTER12_SUBTYPE_NA_MASK                                    0x00000200
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER12_FIELD_RXQ_ID_FILTER12_SUBTYPE_EQUAL_MASK                                 0x00000400
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER12_FIELD_RXQ_ID_FILTER12_ADDR2_ID_VAL_MASK                                  0x00002000
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER12_FIELD_RXQ_ID_FILTER12_ADDR2_ID_VAL_NA_MASK                               0x00004000
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER12_FIELD_RXQ_ID_FILTER12_ADDR1_UNICAST_MASK                                 0x00008000
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER12_FIELD_RXQ_ID_FILTER12_ADDR1_UNICAST_NA_MASK                              0x00010000
//========================================
/* REG_RX_CLASSIFIER_RXQ_ID_FILTER13 0x94 */
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER13_FIELD_RXQ_ID_FILTER13_DROP_EN_MASK                                       0x00000001
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER13_FIELD_RXQ_ID_FILTER13_FRAME_TYPE_MASK                                    0x0000000C
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER13_FIELD_RXQ_ID_FILTER13_TYPE_NA_MASK                                       0x00000010
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER13_FIELD_RXQ_ID_FILTER13_FRAME_SUBTYPE_MASK                                 0x000001E0
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER13_FIELD_RXQ_ID_FILTER13_SUBTYPE_NA_MASK                                    0x00000200
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER13_FIELD_RXQ_ID_FILTER13_SUBTYPE_EQUAL_MASK                                 0x00000400
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER13_FIELD_RXQ_ID_FILTER13_ADDR2_ID_VAL_MASK                                  0x00002000
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER13_FIELD_RXQ_ID_FILTER13_ADDR2_ID_VAL_NA_MASK                               0x00004000
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER13_FIELD_RXQ_ID_FILTER13_ADDR1_UNICAST_MASK                                 0x00008000
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER13_FIELD_RXQ_ID_FILTER13_ADDR1_UNICAST_NA_MASK                              0x00010000
//========================================
/* REG_RX_CLASSIFIER_RXQ_ID_FILTER14 0x98 */
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER14_FIELD_RXQ_ID_FILTER14_DROP_EN_MASK                                       0x00000001
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER14_FIELD_RXQ_ID_FILTER14_FRAME_TYPE_MASK                                    0x0000000C
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER14_FIELD_RXQ_ID_FILTER14_TYPE_NA_MASK                                       0x00000010
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER14_FIELD_RXQ_ID_FILTER14_FRAME_SUBTYPE_MASK                                 0x000001E0
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER14_FIELD_RXQ_ID_FILTER14_SUBTYPE_NA_MASK                                    0x00000200
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER14_FIELD_RXQ_ID_FILTER14_SUBTYPE_EQUAL_MASK                                 0x00000400
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER14_FIELD_RXQ_ID_FILTER14_ADDR2_ID_VAL_MASK                                  0x00002000
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER14_FIELD_RXQ_ID_FILTER14_ADDR2_ID_VAL_NA_MASK                               0x00004000
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER14_FIELD_RXQ_ID_FILTER14_ADDR1_UNICAST_MASK                                 0x00008000
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER14_FIELD_RXQ_ID_FILTER14_ADDR1_UNICAST_NA_MASK                              0x00010000
//========================================
/* REG_RX_CLASSIFIER_RXQ_ID_FILTER15 0x9C */
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER15_FIELD_RXQ_ID_FILTER15_DROP_EN_MASK                                       0x00000001
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER15_FIELD_RXQ_ID_FILTER15_FRAME_TYPE_MASK                                    0x0000000C
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER15_FIELD_RXQ_ID_FILTER15_TYPE_NA_MASK                                       0x00000010
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER15_FIELD_RXQ_ID_FILTER15_FRAME_SUBTYPE_MASK                                 0x000001E0
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER15_FIELD_RXQ_ID_FILTER15_SUBTYPE_NA_MASK                                    0x00000200
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER15_FIELD_RXQ_ID_FILTER15_SUBTYPE_EQUAL_MASK                                 0x00000400
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER15_FIELD_RXQ_ID_FILTER15_ADDR2_ID_VAL_MASK                                  0x00002000
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER15_FIELD_RXQ_ID_FILTER15_ADDR2_ID_VAL_NA_MASK                               0x00004000
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER15_FIELD_RXQ_ID_FILTER15_ADDR1_UNICAST_MASK                                 0x00008000
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER15_FIELD_RXQ_ID_FILTER15_ADDR1_UNICAST_NA_MASK                              0x00010000
//========================================
/* REG_RX_CLASSIFIER_RXQ_ID_FILTER_CONTROL 0xA0 */
#define B2_RX_CLASSIFIER_RXQ_ID_FILTER_CONTROL_FIELD_RXQ_ID_FILTER_GROUP_AND_LOGIC_MASK                           0x00000007
//========================================
/* REG_RX_CLASSIFIER_RX_CLASSIFIER_ENABLE 0xB0 */
#define B2_RX_CLASSIFIER_RX_CLASSIFIER_ENABLE_FIELD_RX_CLAS_TOP_EN_MASK                                           0x00000001
#define B2_RX_CLASSIFIER_RX_CLASSIFIER_ENABLE_FIELD_RX_CLAS_SECURITY_EN_MASK                                      0x00000002
#define B2_RX_CLASSIFIER_RX_CLASSIFIER_ENABLE_FIELD_RX_CLAS_RTD_GENERATION_EN_MASK                                0x00000004
//========================================
/* REG_RX_CLASSIFIER_WEP_KEY_IDX_DB_LOCATION 0xB4 */
#define B2_RX_CLASSIFIER_WEP_KEY_IDX_DB_LOCATION_FIELD_WEP_WDS_KEY_IDX_DB_LOCATION_MASK                           0x00000001
#define B2_RX_CLASSIFIER_WEP_KEY_IDX_DB_LOCATION_FIELD_WEP_NO_WDS_KEY_IDX_DB_LOCATION_MASK                        0x00000002
//========================================
/* REG_RX_CLASSIFIER_RX_CLAS_RTD_CTRL 0x100 */
#define B2_RX_CLASSIFIER_RX_CLAS_RTD_CTRL_FIELD_LENGTH_CALC_NEGATIVE_DROP_EN_MASK                                 0x00000020
#define B2_RX_CLASSIFIER_RX_CLAS_RTD_CTRL_FIELD_MIS_DATA_PROT_FRAME_DB_DROP_EN_MASK                               0x00000040
//========================================
/* REG_RX_CLASSIFIER_RX_CLAS_SM_DBG 0x114 */
#define B2_RX_CLASSIFIER_RX_CLAS_SM_DBG_FIELD_RX_CLASSIFIER_SM_MASK                                               0x0000000F
#define B2_RX_CLASSIFIER_RX_CLAS_SM_DBG_FIELD_RD_CIR_BUF_SM_MASK                                                  0x00000030
#define B2_RX_CLASSIFIER_RX_CLAS_SM_DBG_FIELD_RD_MPDU_DESC_SM_MASK                                                0x000000C0
#define B2_RX_CLASSIFIER_RX_CLAS_SM_DBG_FIELD_RXQ_ID_SM_MASK                                                      0x00000300
#define B2_RX_CLASSIFIER_RX_CLAS_SM_DBG_FIELD_SEC_INIT_SM_MASK                                                    0x00003C00
#define B2_RX_CLASSIFIER_RX_CLAS_SM_DBG_FIELD_RMD_FILL_SM_MASK                                                    0x001C0000
#define B2_RX_CLASSIFIER_RX_CLAS_SM_DBG_FIELD_DLM_IF_SM_MASK                                                      0x00600000
#define B2_RX_CLASSIFIER_RX_CLAS_SM_DBG_FIELD_MPDU_FIELDS_FILTER_SM_MASK                                          0x03800000
#define B2_RX_CLASSIFIER_RX_CLAS_SM_DBG_FIELD_RD_SEC_HDR_SM_MASK                                                  0x0C000000
//========================================
/* REG_RX_CLASSIFIER_RX_CLAS_LOGGER_CONTROL 0x134 */
#define B2_RX_CLASSIFIER_RX_CLAS_LOGGER_CONTROL_FIELD_RX_CLAS_LOGGER_EN_MASK                                      0x00000001
#define B2_RX_CLASSIFIER_RX_CLAS_LOGGER_CONTROL_FIELD_RX_CLAS_LOGGER_PRIORITY_MASK                                0x00000300
//========================================
/* REG_RX_CLASSIFIER_RX_CLAS_LOGGER_ACTIVE 0x138 */
#define B2_RX_CLASSIFIER_RX_CLAS_LOGGER_ACTIVE_FIELD_RX_CLAS_LOGGER_ACTIVE_MASK                                   0x00000001
//========================================
/* REG_RX_CLASSIFIER_SPARE_REGISTERS 0x1FC */
#define B2_RX_CLASSIFIER_SPARE_REGISTERS_FIELD_SPARE_REGISTERS_MASK                                               0x0000FFFF


#endif // _RX_CLASSIFIER_REGS_H_
