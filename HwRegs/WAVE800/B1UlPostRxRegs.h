
/***********************************************************************************
File:				B1UlPostRxRegs.h
Module:				b1UlPostRx
SOC Revision:		latest
Generated at:       2024-06-26 12:54:47
Purpose:
Description:		This File was auto generated using SOC Online

************************************************************************************/
#ifndef _B1_UL_POST_RX_REGS_H_
#define _B1_UL_POST_RX_REGS_H_

/*---------------------------------------------------------------------------------
/						Registers Addresses													 
/----------------------------------------------------------------------------------*/
#include "HwMemoryMap.h"

#define B1_UL_POST_RX_BASE_ADDRESS                              MEMORY_MAP_UNIT_10043_BASE_ADDRESS
#define	REG_B1_UL_POST_RX_VAP_HE_VARIANT_SUPPORT_BITMAP             (B1_UL_POST_RX_BASE_ADDRESS + 0x0)
#define	REG_B1_UL_POST_RX_STA_HE_VARIANT_SUPPORT_BITMAP_31_0        (B1_UL_POST_RX_BASE_ADDRESS + 0x4)
#define	REG_B1_UL_POST_RX_STA_HE_VARIANT_SUPPORT_BITMAP_63_32       (B1_UL_POST_RX_BASE_ADDRESS + 0x8)
#define	REG_B1_UL_POST_RX_STA_HE_VARIANT_SUPPORT_BITMAP_95_64       (B1_UL_POST_RX_BASE_ADDRESS + 0xC)
#define	REG_B1_UL_POST_RX_STA_HE_VARIANT_SUPPORT_BITMAP_127_96      (B1_UL_POST_RX_BASE_ADDRESS + 0x10)
#define	REG_B1_UL_POST_RX_CONTROL_INFORMATION_LENGTH_ARRAY_3_0      (B1_UL_POST_RX_BASE_ADDRESS + 0x24)
#define	REG_B1_UL_POST_RX_CONTROL_INFORMATION_LENGTH_ARRAY_7_4      (B1_UL_POST_RX_BASE_ADDRESS + 0x28)
#define	REG_B1_UL_POST_RX_CONTROL_INFORMATION_LENGTH_ARRAY_11_8     (B1_UL_POST_RX_BASE_ADDRESS + 0x2C)
#define	REG_B1_UL_POST_RX_CONTROL_INFORMATION_LENGTH_ARRAY_15_12    (B1_UL_POST_RX_BASE_ADDRESS + 0x30)
#define	REG_B1_UL_POST_RX_A_CONTROL_FIELD_HW_PARSING_ENABLE         (B1_UL_POST_RX_BASE_ADDRESS + 0x34)
#define	REG_B1_UL_POST_RX_A_CONTROL_FIELD_SW_REPORT_ENABLE          (B1_UL_POST_RX_BASE_ADDRESS + 0x38)
#define	REG_B1_UL_POST_RX_DL_MU_SUSPENSION_BIT_ENABLE               (B1_UL_POST_RX_BASE_ADDRESS + 0x3C)
#define	REG_B1_UL_POST_RX_HE_MU_TB_REPORT_ENABLE                    (B1_UL_POST_RX_BASE_ADDRESS + 0x40)
#define	REG_B1_UL_POST_RX_HE_SU_REPORT_ENABLE                       (B1_UL_POST_RX_BASE_ADDRESS + 0x44)
#define	REG_B1_UL_POST_RX_HE_MU_DL_REPORT_ENABLE                    (B1_UL_POST_RX_BASE_ADDRESS + 0x48)
#define	REG_B1_UL_POST_RX_UL_MU_DISABLE_DATA_CONTROL                (B1_UL_POST_RX_BASE_ADDRESS + 0x4C)
#define	REG_B1_UL_POST_RX_ULPR_COMMON_FIFO_BASE_ADDR                (B1_UL_POST_RX_BASE_ADDRESS + 0x50)
#define	REG_B1_UL_POST_RX_ULPR_COMMON_FIFO_DEPTH_MINUS_ONE          (B1_UL_POST_RX_BASE_ADDRESS + 0x54)
#define	REG_B1_UL_POST_RX_ULPR_COMMON_FIFO_CLEAR_STRB               (B1_UL_POST_RX_BASE_ADDRESS + 0x58)
#define	REG_B1_UL_POST_RX_ULPR_COMMON_FIFO_RD_ENTRIES_NUM           (B1_UL_POST_RX_BASE_ADDRESS + 0x5C)
#define	REG_B1_UL_POST_RX_ULPR_COMMON_FIFO_NUM_ENTRIES_COUNT        (B1_UL_POST_RX_BASE_ADDRESS + 0x60)
#define	REG_B1_UL_POST_RX_ULPR_COMMON_FIFO_DEBUG                    (B1_UL_POST_RX_BASE_ADDRESS + 0x64)
#define	REG_B1_UL_POST_RX_ULPR_USER_FIFO_BASE_ADDR                  (B1_UL_POST_RX_BASE_ADDRESS + 0x68)
#define	REG_B1_UL_POST_RX_ULPR_USER_FIFO_DEPTH_MINUS_ONE            (B1_UL_POST_RX_BASE_ADDRESS + 0x6C)
#define	REG_B1_UL_POST_RX_ULPR_USER_FIFO_CLEAR_STRB                 (B1_UL_POST_RX_BASE_ADDRESS + 0x70)
#define	REG_B1_UL_POST_RX_ULPR_USER_FIFO_RD_ENTRIES_NUM             (B1_UL_POST_RX_BASE_ADDRESS + 0x74)
#define	REG_B1_UL_POST_RX_ULPR_USER_FIFO_NUM_ENTRIES_COUNT          (B1_UL_POST_RX_BASE_ADDRESS + 0x78)
#define	REG_B1_UL_POST_RX_ULPR_USER_FIFO_DEBUG                      (B1_UL_POST_RX_BASE_ADDRESS + 0x7C)
#define	REG_B1_UL_POST_RX_ULPR_INT_ERROR_STATUS                     (B1_UL_POST_RX_BASE_ADDRESS + 0x80)
#define	REG_B1_UL_POST_RX_ULPR_INT_ERROR_EN                         (B1_UL_POST_RX_BASE_ADDRESS + 0x84)
#define	REG_B1_UL_POST_RX_ULPR_INT_ERROR_CLEAR                      (B1_UL_POST_RX_BASE_ADDRESS + 0x88)
#define	REG_B1_UL_POST_RX_ULPR_FIFO_NOT_EMPTY_STATUS                (B1_UL_POST_RX_BASE_ADDRESS + 0x90)
#define	REG_B1_UL_POST_RX_ULPR_FIFO_NOT_EMPTY_EN                    (B1_UL_POST_RX_BASE_ADDRESS + 0x94)
#define	REG_B1_UL_POST_RX_ULPR_STATUS_0                             (B1_UL_POST_RX_BASE_ADDRESS + 0x9C)
#define	REG_B1_UL_POST_RX_ULPR_CONTROL                              (B1_UL_POST_RX_BASE_ADDRESS + 0xA0)
#define	REG_B1_UL_POST_RX_ULPR_SPARE_REGISTER                       (B1_UL_POST_RX_BASE_ADDRESS + 0xB0)
/*---------------------------------------------------------------------------------
/						Data Type Definition										
/----------------------------------------------------------------------------------*/
/*REG_B1_UL_POST_RX_VAP_HE_VARIANT_SUPPORT_BITMAP 0x0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vapHeVariantSupportBitmap : 32; //This bitmap designates whether the VAP ID (to which the Rx user belongs) supports the reception of HE Variant HT Control word. A-Control analysis can take place only if the respective bit for a given user is set. There is 1b per VAP, reset value: 0x0, access type: RW
	} bitFields;
} RegB1UlPostRxVapHeVariantSupportBitmap_u;

/*REG_B1_UL_POST_RX_STA_HE_VARIANT_SUPPORT_BITMAP_31_0 0x4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 staHeVariantSupportBitmap310 : 32; //This bitmap designates whether the STA ID (with which the Rx user is affiliated) supports the reception of HE Variant HT Control word. A-Control analysis can take place only if the respective bit for a given user is set. This vector designates Station Indexes 31:0. There is 1b per STA, reset value: 0x0, access type: RW
	} bitFields;
} RegB1UlPostRxStaHeVariantSupportBitmap310_u;

/*REG_B1_UL_POST_RX_STA_HE_VARIANT_SUPPORT_BITMAP_63_32 0x8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 staHeVariantSupportBitmap6332 : 32; //This bitmap designates whether the STA ID (with which the Rx user is affiliated) supports the reception of HE Variant HT Control word. A-Control analysis can take place only if the respective bit for a given user is set. This vector designates Station Indexes 63:32. There is 1b per STA, reset value: 0x0, access type: RW
	} bitFields;
} RegB1UlPostRxStaHeVariantSupportBitmap6332_u;

/*REG_B1_UL_POST_RX_STA_HE_VARIANT_SUPPORT_BITMAP_95_64 0xC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 staHeVariantSupportBitmap9564 : 32; //This bitmap designates whether the STA ID (with which the Rx user is affiliated) supports the reception of HE Variant HT Control word. A-Control analysis can take place only if the respective bit for a given user is set. This vector designates Station Indexes 95:64. There is 1b per STA, reset value: 0x0, access type: RW
	} bitFields;
} RegB1UlPostRxStaHeVariantSupportBitmap9564_u;

/*REG_B1_UL_POST_RX_STA_HE_VARIANT_SUPPORT_BITMAP_127_96 0x10 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 staHeVariantSupportBitmap12796 : 32; //This bitmap designates whether the STA ID (with which the Rx user is affiliated) supports the reception of HE Variant HT Control word. A-Control analysis can take place only if the respective bit for a given user is set. This vector designates Station Indexes 127:96. There is 1b per STA, reset value: 0x0, access type: RW
	} bitFields;
} RegB1UlPostRxStaHeVariantSupportBitmap12796_u;

/*REG_B1_UL_POST_RX_CONTROL_INFORMATION_LENGTH_ARRAY_3_0 0x24 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 controlInformationLength0 : 5; //Length (in bits) of the Control Information subfield (within the A-Control word) for Control ID 0., reset value: 0x1a, access type: RW
		uint32 reserved0 : 3;
		uint32 controlInformationLength1 : 5; //Length (in bits) of the Control Information subfield (within the A-Control word) for Control ID 1., reset value: 0xc, access type: RW
		uint32 reserved1 : 3;
		uint32 controlInformationLength2 : 5; //Length (in bits) of the Control Information subfield (within the A-Control word) for Control ID 2., reset value: 0x1a, access type: RW
		uint32 reserved2 : 3;
		uint32 controlInformationLength3 : 5; //Length (in bits) of the Control Information subfield (within the A-Control word) for Control ID 3., reset value: 0x1a, access type: RW
		uint32 reserved3 : 3;
	} bitFields;
} RegB1UlPostRxControlInformationLengthArray30_u;

/*REG_B1_UL_POST_RX_CONTROL_INFORMATION_LENGTH_ARRAY_7_4 0x28 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 controlInformationLength4 : 5; //Length (in bits) of the Control Information subfield (within the A-Control word) for Control ID 4., reset value: 0x8, access type: RW
		uint32 reserved0 : 3;
		uint32 controlInformationLength5 : 5; //Length (in bits) of the Control Information subfield (within the A-Control word) for Control ID 5., reset value: 0xa, access type: RW
		uint32 reserved1 : 3;
		uint32 controlInformationLength6 : 5; //Length (in bits) of the Control Information subfield (within the A-Control word) for Control ID 6., reset value: 0x8, access type: RW
		uint32 reserved2 : 3;
		uint32 controlInformationLength7 : 5; //Length (in bits) of the Control Information subfield (within the A-Control word) for Control ID 7., reset value: 0x6, access type: RW
		uint32 reserved3 : 3;
	} bitFields;
} RegB1UlPostRxControlInformationLengthArray74_u;

/*REG_B1_UL_POST_RX_CONTROL_INFORMATION_LENGTH_ARRAY_11_8 0x2C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 controlInformationLength8 : 5; //Length (in bits) of the Control Information subfield (within the A-Control word) for Control ID 8., reset value: 0x1f, access type: RW
		uint32 reserved0 : 3;
		uint32 controlInformationLength9 : 5; //Length (in bits) of the Control Information subfield (within the A-Control word) for Control ID 9., reset value: 0x1f, access type: RW
		uint32 reserved1 : 3;
		uint32 controlInformationLength10 : 5; //Length (in bits) of the Control Information subfield (within the A-Control word) for Control ID 10., reset value: 0x1f, access type: RW
		uint32 reserved2 : 3;
		uint32 controlInformationLength11 : 5; //Length (in bits) of the Control Information subfield (within the A-Control word) for Control ID 11., reset value: 0x1f, access type: RW
		uint32 reserved3 : 3;
	} bitFields;
} RegB1UlPostRxControlInformationLengthArray118_u;

/*REG_B1_UL_POST_RX_CONTROL_INFORMATION_LENGTH_ARRAY_15_12 0x30 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 controlInformationLength12 : 5; //Length (in bits) of the Control Information subfield (within the A-Control word) for Control ID 12., reset value: 0x1f, access type: RW
		uint32 reserved0 : 3;
		uint32 controlInformationLength13 : 5; //Length (in bits) of the Control Information subfield (within the A-Control word) for Control ID 13., reset value: 0x1f, access type: RW
		uint32 reserved1 : 3;
		uint32 controlInformationLength14 : 5; //Length (in bits) of the Control Information subfield (within the A-Control word) for Control ID 14., reset value: 0x1f, access type: RW
		uint32 reserved2 : 3;
		uint32 controlInformationLength15 : 5; //Length (in bits) of the Control Information subfield (within the A-Control word) for Control ID 15., reset value: 0x1a, access type: RW
		uint32 reserved3 : 3;
	} bitFields;
} RegB1UlPostRxControlInformationLengthArray1512_u;

/*REG_B1_UL_POST_RX_A_CONTROL_FIELD_HW_PARSING_ENABLE 0x34 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 aControlFieldHwParsingEnable : 16; //This bitmap designates whether the HW shall process (and parse) the Control Information subfield for each Control ID. It is used for the A-Control Analysis section. There is 1b per CTRL_ID, reset value: 0x2, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB1UlPostRxAControlFieldHwParsingEnable_u;

/*REG_B1_UL_POST_RX_A_CONTROL_FIELD_SW_REPORT_ENABLE 0x38 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 aControlFieldSwReportEnable : 16; //This bitmap designates whether a Control ID requires a SW report to be generated for it. It is used for the UL Report section. There is 1b per CTRL_ID, reset value: 0x12, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB1UlPostRxAControlFieldSwReportEnable_u;

/*REG_B1_UL_POST_RX_DL_MU_SUSPENSION_BIT_ENABLE 0x3C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dlMuSuspensionBitEnable : 1; //This global configuration controls the value that is written to DL MU Disable field within the TX Selector, based on the UL MU Disable bit, which is extracted from the Control Information subfield in case (CTRL_ID == 1) is analyzed. 0 ------ DL MU Disable is set to 0;  1 - DL MU Disable is set to the value of UL MU Disable, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB1UlPostRxDlMuSuspensionBitEnable_u;

/*REG_B1_UL_POST_RX_HE_MU_TB_REPORT_ENABLE 0x40 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 heMuTbReportEnable : 1; //This global configuration controls whether a report shall be generated in case (PHY Mode == 6), reset value: 0x1, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB1UlPostRxHeMuTbReportEnable_u;

/*REG_B1_UL_POST_RX_HE_SU_REPORT_ENABLE 0x44 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 heSuReportEnable : 1; //This global configuration controls (among other conditions) whether a report shall be generated in case (PHY Mode == 4,5), reset value: 0x1, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB1UlPostRxHeSuReportEnable_u;

/*REG_B1_UL_POST_RX_HE_MU_DL_REPORT_ENABLE 0x48 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 heMuDlReportEnable : 1; //This global configuration controls (among other conditions) whether a report shall be generated in case (PHY Mode == 7), reset value: 0x1, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB1UlPostRxHeMuDlReportEnable_u;

/*REG_B1_UL_POST_RX_UL_MU_DISABLE_DATA_CONTROL 0x4C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ulMuDisableDataFieldEnable : 1; //This global configuration designates whether the new UL MU Data Disable field is supported. If supported, then the location of the UL MU Data Disable field within the Control Information subfield in case (CTRL_ID == 1), is given in UL MU Disable Data Field Bit Position configuration., reset value: 0x1, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB1UlPostRxUlMuDisableDataControl_u;

/*REG_B1_UL_POST_RX_ULPR_COMMON_FIFO_BASE_ADDR 0x50 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ulprCommonFifoBaseAddr : 22; //ULPR common FIFO base address, reset value: 0x0, access type: RW
		uint32 reserved0 : 10;
	} bitFields;
} RegB1UlPostRxUlprCommonFifoBaseAddr_u;

/*REG_B1_UL_POST_RX_ULPR_COMMON_FIFO_DEPTH_MINUS_ONE 0x54 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ulprCommonFifoDepthMinusOne : 9; //ULPR common FIFO depth minus one, reset value: 0x1ff, access type: RW
		uint32 reserved0 : 23;
	} bitFields;
} RegB1UlPostRxUlprCommonFifoDepthMinusOne_u;

/*REG_B1_UL_POST_RX_ULPR_COMMON_FIFO_CLEAR_STRB 0x58 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ulprCommonFifoClearStrb : 1; //Clear ULPR common FIFO. , By writing '1'., reset value: 0x0, access type: WO
		uint32 reserved0 : 31;
	} bitFields;
} RegB1UlPostRxUlprCommonFifoClearStrb_u;

/*REG_B1_UL_POST_RX_ULPR_COMMON_FIFO_RD_ENTRIES_NUM 0x5C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ulprCommonFifoRdEntriesNum : 10; //ULPR common FIFO number of entries to decrement., reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegB1UlPostRxUlprCommonFifoRdEntriesNum_u;

/*REG_B1_UL_POST_RX_ULPR_COMMON_FIFO_NUM_ENTRIES_COUNT 0x60 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ulprCommonFifoNumEntriesCount : 10; //ULPR common FIFO number of entries count, reset value: 0x0, access type: RO
		uint32 reserved0 : 22;
	} bitFields;
} RegB1UlPostRxUlprCommonFifoNumEntriesCount_u;

/*REG_B1_UL_POST_RX_ULPR_COMMON_FIFO_DEBUG 0x64 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ulprCommonFifoWrPtr : 9; //ULPR common FIFO write pointer index, reset value: 0x0, access type: RO
		uint32 reserved0 : 3;
		uint32 ulprCommonFifoNotEmpty : 1; //ULPR common FIFO not empty indication, reset value: 0x0, access type: RO
		uint32 ulprCommonFifoFull : 1; //ULPR common FIFO full indication, reset value: 0x0, access type: RO
		uint32 reserved1 : 6;
		uint32 ulprCommonFifoFullDropCtr : 9; //ULPR common report setting FIFO full drop counter, reset value: 0x0, access type: RO
		uint32 reserved2 : 3;
	} bitFields;
} RegB1UlPostRxUlprCommonFifoDebug_u;

/*REG_B1_UL_POST_RX_ULPR_USER_FIFO_BASE_ADDR 0x68 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ulprUserFifoBaseAddr : 22; //ULPR user FIFO base address, reset value: 0x0, access type: RW
		uint32 reserved0 : 10;
	} bitFields;
} RegB1UlPostRxUlprUserFifoBaseAddr_u;

/*REG_B1_UL_POST_RX_ULPR_USER_FIFO_DEPTH_MINUS_ONE 0x6C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ulprUserFifoDepthMinusOne : 9; //ULPR user FIFO depth minus one, reset value: 0x1ff, access type: RW
		uint32 reserved0 : 23;
	} bitFields;
} RegB1UlPostRxUlprUserFifoDepthMinusOne_u;

/*REG_B1_UL_POST_RX_ULPR_USER_FIFO_CLEAR_STRB 0x70 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ulprUserFifoClearStrb : 1; //Clear ULPR user FIFO. , By writing '1'., reset value: 0x0, access type: WO
		uint32 reserved0 : 31;
	} bitFields;
} RegB1UlPostRxUlprUserFifoClearStrb_u;

/*REG_B1_UL_POST_RX_ULPR_USER_FIFO_RD_ENTRIES_NUM 0x74 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ulprUserFifoRdEntriesNum : 10; //ULPR user FIFO number of entries to decrement., reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegB1UlPostRxUlprUserFifoRdEntriesNum_u;

/*REG_B1_UL_POST_RX_ULPR_USER_FIFO_NUM_ENTRIES_COUNT 0x78 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ulprUserFifoNumEntriesCount : 10; //ULPR user FIFO number of entries count, reset value: 0x0, access type: RO
		uint32 reserved0 : 22;
	} bitFields;
} RegB1UlPostRxUlprUserFifoNumEntriesCount_u;

/*REG_B1_UL_POST_RX_ULPR_USER_FIFO_DEBUG 0x7C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ulprUserFifoWrPtr : 9; //ULPR user FIFO write pointer index, reset value: 0x0, access type: RO
		uint32 reserved0 : 1;
		uint32 ulprUserFifoNotEmpty : 1; //ULPR user FIFO not empty indication, reset value: 0x0, access type: RO
		uint32 ulprUserFifoFull : 1; //ULPR user FIFO full indication, reset value: 0x0, access type: RO
		uint32 ulprUserNonFirstFifoFullDropCtr : 9; //ULPR user FIFO drop counter for non first user, reset value: 0x0, access type: RO
		uint32 reserved1 : 1;
		uint32 ulprUserFirstFifoFullDropCtr : 9; //ULPR user FIFO drop counter for first user, reset value: 0x0, access type: RO
		uint32 reserved2 : 1;
	} bitFields;
} RegB1UlPostRxUlprUserFifoDebug_u;

/*REG_B1_UL_POST_RX_ULPR_INT_ERROR_STATUS 0x80 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ulprCommonFifoDropStatus : 1; //Common FIFO dropped entries event, reset value: 0x0, access type: RO
		uint32 ulprUserNonFirstFifoDropStatus : 1; //User FIFO non first user dropped entries event, reset value: 0x0, access type: RO
		uint32 ulprUserFirstFifoDropStatus : 1; //User FIFO first user dropped entries event, reset value: 0x0, access type: RO
		uint32 ulprPulse1CollisionStatus : 1; //Pulse_1 collision event. Set when pulse_1 is set during ULPR processing (of previous RX Window). When this even occurs, the ULPR block shall finish processing users which have started processing and abort the rest of the users, reset value: 0x0, access type: RO
		uint32 ulprPulse2CollisionStatus : 1; //Pulse_2 collision event. Set when pulse_2 is set during ULPR processing (necessarily finishing last user processing, since pulse_1 necessarily collided as well). When this event occurs, the processing due to pulse_2 (and the following pulse_3) shall not be done and pulse_2 and the following pulse_3 shall be ignored, reset value: 0x0, access type: RO
		uint32 ulprNonUniqueControlIdStatus : 1; //Non-unique Control ID event. Control ID appears more than once within A-Control word, reset value: 0x0, access type: RO
		uint32 ulprCommonFifoDecrementLessThanZeroStatus : 1; //Common FIFO decrement less than zero event. Number of entries of ULPR common FIFO  has been decremented to less than zero, reset value: 0x0, access type: RO
		uint32 ulprUserFifoDecrementLessThanZeroStatus : 1; //User FIFO decrement less than zero event. Number of entries of ULPR User FIFO  has been decremented to less than zero, reset value: 0x0, access type: RO
		uint32 reserved0 : 24;
	} bitFields;
} RegB1UlPostRxUlprIntErrorStatus_u;

/*REG_B1_UL_POST_RX_ULPR_INT_ERROR_EN 0x84 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ulprCommonFifoDropEn : 1; //Common FIFO dropped entries interrupt enable, reset value: 0x1, access type: RW
		uint32 ulprUserNonFirstFifoDropEn : 1; //User FIFO non first user dropped entries interrupt enable, reset value: 0x1, access type: RW
		uint32 ulprUserFirstFifoDropEn : 1; //User FIFO first user dropped entries interrupt enable, reset value: 0x1, access type: RW
		uint32 ulprPulse1CollisionEn : 1; //Pulse_1 collision interrupt enable, reset value: 0x0, access type: RW
		uint32 ulprPulse2CollisionEn : 1; //Pulse_3 collision interrupt enable, reset value: 0x0, access type: RW
		uint32 ulprNonUniqueControlIdEn : 1; //Non-unique Control ID interrupt enable, reset value: 0x0, access type: RW
		uint32 ulprCommonFifoDecrementLessThanZeroEn : 1; //Common FIFO decrement less than zero enable, reset value: 0x0, access type: RW
		uint32 ulprUserFifoDecrementLessThanZeroEn : 1; //User FIFO decrement less than zero enable, reset value: 0x0, access type: RW
		uint32 reserved0 : 24;
	} bitFields;
} RegB1UlPostRxUlprIntErrorEn_u;

/*REG_B1_UL_POST_RX_ULPR_INT_ERROR_CLEAR 0x88 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ulprCommonFifoDropClr : 1; //Common FIFO dropped entries interrupt clear. Writing '1' to this field shall also clear the value of the common FIFO drop counter, reset value: 0x0, access type: WO
		uint32 ulprUserNonFirstFifoDropClr : 1; //User FIFO non first user dropped entries interrupt clear. Writing '1' to this field shall also clear the value of the user FIFO non first drop counter, reset value: 0x0, access type: WO
		uint32 ulprUserFirstFifoDropClr : 1; //User FIFO first user dropped entries interrupt clear. Writing '1' to this field shall also clear the value of the user FIFO first drop counter, reset value: 0x0, access type: WO
		uint32 ulprPulse1CollisionClr : 1; //Pulse_1 collision interrupt clear, reset value: 0x0, access type: WO
		uint32 ulprPulse2CollisionClr : 1; //Pulse_3 collision interrupt clear, reset value: 0x0, access type: WO
		uint32 ulprNonUniqueControlIdClr : 1; //Non-unique Control ID interrupt clear, reset value: 0x0, access type: WO
		uint32 ulprCommonFifoDecrementLessThanZeroClr : 1; //Common FIFO decrement less than zero clear, reset value: 0x0, access type: WO
		uint32 ulprUserFifoDecrementLessThanZeroClr : 1; //User FIFO decrement less than zero clear, reset value: 0x0, access type: WO
		uint32 reserved0 : 24;
	} bitFields;
} RegB1UlPostRxUlprIntErrorClear_u;

/*REG_B1_UL_POST_RX_ULPR_FIFO_NOT_EMPTY_STATUS 0x90 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ulprCommonFifoNotEmptyStatus : 1; //ULPR common FIFO not empty indication, reset value: 0x0, access type: RO
		uint32 ulprUserFifoNotEmptyStatus : 1; //ULPR user FIFO not empty indication, reset value: 0x0, access type: RO
		uint32 reserved0 : 30;
	} bitFields;
} RegB1UlPostRxUlprFifoNotEmptyStatus_u;

/*REG_B1_UL_POST_RX_ULPR_FIFO_NOT_EMPTY_EN 0x94 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ulprCommonFifoNotEmptyEnable : 1; //ULPR common FIFO not empty interrupt enable, reset value: 0x0, access type: RW
		uint32 ulprUserFifoNotEmptyEnable : 1; //ULPR user FIFO not empty interrupt enable, reset value: 0x1, access type: RW
		uint32 reserved0 : 30;
	} bitFields;
} RegB1UlPostRxUlprFifoNotEmptyEn_u;

/*REG_B1_UL_POST_RX_ULPR_STATUS_0 0x9C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ulprCoreSmDbg : 3; //ULPR_CORE SM status, reset value: 0x0, access type: RO
		uint32 ulprRcrScanSmDbg : 2; //ULPR_RCR_SCAN SM status, reset value: 0x0, access type: RO
		uint32 ulprInitSmDbg : 2; //ULPR_INIT SM status, reset value: 0x0, access type: RO
		uint32 ulprUserMngrSmDbg : 2; //ULPR_USER_MNGR SM status, reset value: 0x0, access type: RO
		uint32 ulprUserGetDataSmDbg : 2; //ULPR_USER_GET_DATA SM status, reset value: 0x0, access type: RO
		uint32 ulprFldCtchSmDbg : 3; //ULPR_FLD_CTCH SM status, reset value: 0x0, access type: RO
		uint32 ulprPhyStatusUserSmDbg : 2; //ULPR_PHY_STATUS_USER SM status, reset value: 0x0, access type: RO
		uint32 ulprUserProcessSmDbg : 3; //ULPR_USER_PROCESS SM status, reset value: 0x0, access type: RO
		uint32 ulprTxselSmDbg : 3; //ULPR_TXSEL SM status, reset value: 0x0, access type: RO
		uint32 ulprUserReportSmDbg : 2; //ULPR_USER_REPORT SM status, reset value: 0x0, access type: RO
		uint32 ulprFifoCommonUserSmDbg : 3; //ULPR FIFO_COMMON_USER SM status, reset value: 0x0, access type: RO
		uint32 reserved0 : 5;
	} bitFields;
} RegB1UlPostRxUlprStatus0_u;

/*REG_B1_UL_POST_RX_ULPR_CONTROL 0xA0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ulprEnable : 1; //This global configuration enables the operation of the block. When disabled, the block shall remain IDLE, reset value: 0x1, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB1UlPostRxUlprControl_u;

/*REG_B1_UL_POST_RX_ULPR_SPARE_REGISTER 0xB0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 spareRegister : 16; //Spare Register, reset value: 0x8888, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB1UlPostRxUlprSpareRegister_u;

//========================================
/* REG_UL_POST_RX_VAP_HE_VARIANT_SUPPORT_BITMAP 0x0 */
#define B1_UL_POST_RX_VAP_HE_VARIANT_SUPPORT_BITMAP_FIELD_VAP_HE_VARIANT_SUPPORT_BITMAP_MASK                      0xFFFFFFFF
//========================================
/* REG_UL_POST_RX_STA_HE_VARIANT_SUPPORT_BITMAP_31_0 0x4 */
#define B1_UL_POST_RX_STA_HE_VARIANT_SUPPORT_BITMAP_31_0_FIELD_STA_HE_VARIANT_SUPPORT_BITMAP_31_0_MASK            0xFFFFFFFF
//========================================
/* REG_UL_POST_RX_STA_HE_VARIANT_SUPPORT_BITMAP_63_32 0x8 */
#define B1_UL_POST_RX_STA_HE_VARIANT_SUPPORT_BITMAP_63_32_FIELD_STA_HE_VARIANT_SUPPORT_BITMAP_63_32_MASK          0xFFFFFFFF
//========================================
/* REG_UL_POST_RX_STA_HE_VARIANT_SUPPORT_BITMAP_95_64 0xC */
#define B1_UL_POST_RX_STA_HE_VARIANT_SUPPORT_BITMAP_95_64_FIELD_STA_HE_VARIANT_SUPPORT_BITMAP_95_64_MASK          0xFFFFFFFF
//========================================
/* REG_UL_POST_RX_STA_HE_VARIANT_SUPPORT_BITMAP_127_96 0x10 */
#define B1_UL_POST_RX_STA_HE_VARIANT_SUPPORT_BITMAP_127_96_FIELD_STA_HE_VARIANT_SUPPORT_BITMAP_127_96_MASK        0xFFFFFFFF
//========================================
/* REG_UL_POST_RX_CONTROL_INFORMATION_LENGTH_ARRAY_3_0 0x24 */
#define B1_UL_POST_RX_CONTROL_INFORMATION_LENGTH_ARRAY_3_0_FIELD_CONTROL_INFORMATION_LENGTH_0_MASK                0x0000001F
#define B1_UL_POST_RX_CONTROL_INFORMATION_LENGTH_ARRAY_3_0_FIELD_CONTROL_INFORMATION_LENGTH_1_MASK                0x00001F00
#define B1_UL_POST_RX_CONTROL_INFORMATION_LENGTH_ARRAY_3_0_FIELD_CONTROL_INFORMATION_LENGTH_2_MASK                0x001F0000
#define B1_UL_POST_RX_CONTROL_INFORMATION_LENGTH_ARRAY_3_0_FIELD_CONTROL_INFORMATION_LENGTH_3_MASK                0x1F000000
//========================================
/* REG_UL_POST_RX_CONTROL_INFORMATION_LENGTH_ARRAY_7_4 0x28 */
#define B1_UL_POST_RX_CONTROL_INFORMATION_LENGTH_ARRAY_7_4_FIELD_CONTROL_INFORMATION_LENGTH_4_MASK                0x0000001F
#define B1_UL_POST_RX_CONTROL_INFORMATION_LENGTH_ARRAY_7_4_FIELD_CONTROL_INFORMATION_LENGTH_5_MASK                0x00001F00
#define B1_UL_POST_RX_CONTROL_INFORMATION_LENGTH_ARRAY_7_4_FIELD_CONTROL_INFORMATION_LENGTH_6_MASK                0x001F0000
#define B1_UL_POST_RX_CONTROL_INFORMATION_LENGTH_ARRAY_7_4_FIELD_CONTROL_INFORMATION_LENGTH_7_MASK                0x1F000000
//========================================
/* REG_UL_POST_RX_CONTROL_INFORMATION_LENGTH_ARRAY_11_8 0x2C */
#define B1_UL_POST_RX_CONTROL_INFORMATION_LENGTH_ARRAY_11_8_FIELD_CONTROL_INFORMATION_LENGTH_8_MASK               0x0000001F
#define B1_UL_POST_RX_CONTROL_INFORMATION_LENGTH_ARRAY_11_8_FIELD_CONTROL_INFORMATION_LENGTH_9_MASK               0x00001F00
#define B1_UL_POST_RX_CONTROL_INFORMATION_LENGTH_ARRAY_11_8_FIELD_CONTROL_INFORMATION_LENGTH_10_MASK              0x001F0000
#define B1_UL_POST_RX_CONTROL_INFORMATION_LENGTH_ARRAY_11_8_FIELD_CONTROL_INFORMATION_LENGTH_11_MASK              0x1F000000
//========================================
/* REG_UL_POST_RX_CONTROL_INFORMATION_LENGTH_ARRAY_15_12 0x30 */
#define B1_UL_POST_RX_CONTROL_INFORMATION_LENGTH_ARRAY_15_12_FIELD_CONTROL_INFORMATION_LENGTH_12_MASK             0x0000001F
#define B1_UL_POST_RX_CONTROL_INFORMATION_LENGTH_ARRAY_15_12_FIELD_CONTROL_INFORMATION_LENGTH_13_MASK             0x00001F00
#define B1_UL_POST_RX_CONTROL_INFORMATION_LENGTH_ARRAY_15_12_FIELD_CONTROL_INFORMATION_LENGTH_14_MASK             0x001F0000
#define B1_UL_POST_RX_CONTROL_INFORMATION_LENGTH_ARRAY_15_12_FIELD_CONTROL_INFORMATION_LENGTH_15_MASK             0x1F000000
//========================================
/* REG_UL_POST_RX_A_CONTROL_FIELD_HW_PARSING_ENABLE 0x34 */
#define B1_UL_POST_RX_A_CONTROL_FIELD_HW_PARSING_ENABLE_FIELD_A_CONTROL_FIELD_HW_PARSING_ENABLE_MASK              0x0000FFFF
//========================================
/* REG_UL_POST_RX_A_CONTROL_FIELD_SW_REPORT_ENABLE 0x38 */
#define B1_UL_POST_RX_A_CONTROL_FIELD_SW_REPORT_ENABLE_FIELD_A_CONTROL_FIELD_SW_REPORT_ENABLE_MASK                0x0000FFFF
//========================================
/* REG_UL_POST_RX_DL_MU_SUSPENSION_BIT_ENABLE 0x3C */
#define B1_UL_POST_RX_DL_MU_SUSPENSION_BIT_ENABLE_FIELD_DL_MU_SUSPENSION_BIT_ENABLE_MASK                          0x00000001
//========================================
/* REG_UL_POST_RX_HE_MU_TB_REPORT_ENABLE 0x40 */
#define B1_UL_POST_RX_HE_MU_TB_REPORT_ENABLE_FIELD_HE_MU_TB_REPORT_ENABLE_MASK                                    0x00000001
//========================================
/* REG_UL_POST_RX_HE_SU_REPORT_ENABLE 0x44 */
#define B1_UL_POST_RX_HE_SU_REPORT_ENABLE_FIELD_HE_SU_REPORT_ENABLE_MASK                                          0x00000001
//========================================
/* REG_UL_POST_RX_HE_MU_DL_REPORT_ENABLE 0x48 */
#define B1_UL_POST_RX_HE_MU_DL_REPORT_ENABLE_FIELD_HE_MU_DL_REPORT_ENABLE_MASK                                    0x00000001
//========================================
/* REG_UL_POST_RX_UL_MU_DISABLE_DATA_CONTROL 0x4C */
#define B1_UL_POST_RX_UL_MU_DISABLE_DATA_CONTROL_FIELD_UL_MU_DISABLE_DATA_FIELD_ENABLE_MASK                       0x00000001
//========================================
/* REG_UL_POST_RX_ULPR_COMMON_FIFO_BASE_ADDR 0x50 */
#define B1_UL_POST_RX_ULPR_COMMON_FIFO_BASE_ADDR_FIELD_ULPR_COMMON_FIFO_BASE_ADDR_MASK                            0x003FFFFF
//========================================
/* REG_UL_POST_RX_ULPR_COMMON_FIFO_DEPTH_MINUS_ONE 0x54 */
#define B1_UL_POST_RX_ULPR_COMMON_FIFO_DEPTH_MINUS_ONE_FIELD_ULPR_COMMON_FIFO_DEPTH_MINUS_ONE_MASK                0x000001FF
//========================================
/* REG_UL_POST_RX_ULPR_COMMON_FIFO_CLEAR_STRB 0x58 */
#define B1_UL_POST_RX_ULPR_COMMON_FIFO_CLEAR_STRB_FIELD_ULPR_COMMON_FIFO_CLEAR_STRB_MASK                          0x00000001
//========================================
/* REG_UL_POST_RX_ULPR_COMMON_FIFO_RD_ENTRIES_NUM 0x5C */
#define B1_UL_POST_RX_ULPR_COMMON_FIFO_RD_ENTRIES_NUM_FIELD_ULPR_COMMON_FIFO_RD_ENTRIES_NUM_MASK                  0x000003FF
//========================================
/* REG_UL_POST_RX_ULPR_COMMON_FIFO_NUM_ENTRIES_COUNT 0x60 */
#define B1_UL_POST_RX_ULPR_COMMON_FIFO_NUM_ENTRIES_COUNT_FIELD_ULPR_COMMON_FIFO_NUM_ENTRIES_COUNT_MASK            0x000003FF
//========================================
/* REG_UL_POST_RX_ULPR_COMMON_FIFO_DEBUG 0x64 */
#define B1_UL_POST_RX_ULPR_COMMON_FIFO_DEBUG_FIELD_ULPR_COMMON_FIFO_WR_PTR_MASK                                   0x000001FF
#define B1_UL_POST_RX_ULPR_COMMON_FIFO_DEBUG_FIELD_ULPR_COMMON_FIFO_NOT_EMPTY_MASK                                0x00001000
#define B1_UL_POST_RX_ULPR_COMMON_FIFO_DEBUG_FIELD_ULPR_COMMON_FIFO_FULL_MASK                                     0x00002000
#define B1_UL_POST_RX_ULPR_COMMON_FIFO_DEBUG_FIELD_ULPR_COMMON_FIFO_FULL_DROP_CTR_MASK                            0x1FF00000
//========================================
/* REG_UL_POST_RX_ULPR_USER_FIFO_BASE_ADDR 0x68 */
#define B1_UL_POST_RX_ULPR_USER_FIFO_BASE_ADDR_FIELD_ULPR_USER_FIFO_BASE_ADDR_MASK                                0x003FFFFF
//========================================
/* REG_UL_POST_RX_ULPR_USER_FIFO_DEPTH_MINUS_ONE 0x6C */
#define B1_UL_POST_RX_ULPR_USER_FIFO_DEPTH_MINUS_ONE_FIELD_ULPR_USER_FIFO_DEPTH_MINUS_ONE_MASK                    0x000001FF
//========================================
/* REG_UL_POST_RX_ULPR_USER_FIFO_CLEAR_STRB 0x70 */
#define B1_UL_POST_RX_ULPR_USER_FIFO_CLEAR_STRB_FIELD_ULPR_USER_FIFO_CLEAR_STRB_MASK                              0x00000001
//========================================
/* REG_UL_POST_RX_ULPR_USER_FIFO_RD_ENTRIES_NUM 0x74 */
#define B1_UL_POST_RX_ULPR_USER_FIFO_RD_ENTRIES_NUM_FIELD_ULPR_USER_FIFO_RD_ENTRIES_NUM_MASK                      0x000003FF
//========================================
/* REG_UL_POST_RX_ULPR_USER_FIFO_NUM_ENTRIES_COUNT 0x78 */
#define B1_UL_POST_RX_ULPR_USER_FIFO_NUM_ENTRIES_COUNT_FIELD_ULPR_USER_FIFO_NUM_ENTRIES_COUNT_MASK                0x000003FF
//========================================
/* REG_UL_POST_RX_ULPR_USER_FIFO_DEBUG 0x7C */
#define B1_UL_POST_RX_ULPR_USER_FIFO_DEBUG_FIELD_ULPR_USER_FIFO_WR_PTR_MASK                                       0x000001FF
#define B1_UL_POST_RX_ULPR_USER_FIFO_DEBUG_FIELD_ULPR_USER_FIFO_NOT_EMPTY_MASK                                    0x00000400
#define B1_UL_POST_RX_ULPR_USER_FIFO_DEBUG_FIELD_ULPR_USER_FIFO_FULL_MASK                                         0x00000800
#define B1_UL_POST_RX_ULPR_USER_FIFO_DEBUG_FIELD_ULPR_USER_NON_FIRST_FIFO_FULL_DROP_CTR_MASK                      0x001FF000
#define B1_UL_POST_RX_ULPR_USER_FIFO_DEBUG_FIELD_ULPR_USER_FIRST_FIFO_FULL_DROP_CTR_MASK                          0x7FC00000
//========================================
/* REG_UL_POST_RX_ULPR_INT_ERROR_STATUS 0x80 */
#define B1_UL_POST_RX_ULPR_INT_ERROR_STATUS_FIELD_ULPR_COMMON_FIFO_DROP_STATUS_MASK                               0x00000001
#define B1_UL_POST_RX_ULPR_INT_ERROR_STATUS_FIELD_ULPR_USER_NON_FIRST_FIFO_DROP_STATUS_MASK                       0x00000002
#define B1_UL_POST_RX_ULPR_INT_ERROR_STATUS_FIELD_ULPR_USER_FIRST_FIFO_DROP_STATUS_MASK                           0x00000004
#define B1_UL_POST_RX_ULPR_INT_ERROR_STATUS_FIELD_ULPR_PULSE_1_COLLISION_STATUS_MASK                              0x00000008
#define B1_UL_POST_RX_ULPR_INT_ERROR_STATUS_FIELD_ULPR_PULSE_2_COLLISION_STATUS_MASK                              0x00000010
#define B1_UL_POST_RX_ULPR_INT_ERROR_STATUS_FIELD_ULPR_NON_UNIQUE_CONTROL_ID_STATUS_MASK                          0x00000020
#define B1_UL_POST_RX_ULPR_INT_ERROR_STATUS_FIELD_ULPR_COMMON_FIFO_DECREMENT_LESS_THAN_ZERO_STATUS_MASK           0x00000040
#define B1_UL_POST_RX_ULPR_INT_ERROR_STATUS_FIELD_ULPR_USER_FIFO_DECREMENT_LESS_THAN_ZERO_STATUS_MASK             0x00000080
//========================================
/* REG_UL_POST_RX_ULPR_INT_ERROR_EN 0x84 */
#define B1_UL_POST_RX_ULPR_INT_ERROR_EN_FIELD_ULPR_COMMON_FIFO_DROP_EN_MASK                                       0x00000001
#define B1_UL_POST_RX_ULPR_INT_ERROR_EN_FIELD_ULPR_USER_NON_FIRST_FIFO_DROP_EN_MASK                               0x00000002
#define B1_UL_POST_RX_ULPR_INT_ERROR_EN_FIELD_ULPR_USER_FIRST_FIFO_DROP_EN_MASK                                   0x00000004
#define B1_UL_POST_RX_ULPR_INT_ERROR_EN_FIELD_ULPR_PULSE_1_COLLISION_EN_MASK                                      0x00000008
#define B1_UL_POST_RX_ULPR_INT_ERROR_EN_FIELD_ULPR_PULSE_2_COLLISION_EN_MASK                                      0x00000010
#define B1_UL_POST_RX_ULPR_INT_ERROR_EN_FIELD_ULPR_NON_UNIQUE_CONTROL_ID_EN_MASK                                  0x00000020
#define B1_UL_POST_RX_ULPR_INT_ERROR_EN_FIELD_ULPR_COMMON_FIFO_DECREMENT_LESS_THAN_ZERO_EN_MASK                   0x00000040
#define B1_UL_POST_RX_ULPR_INT_ERROR_EN_FIELD_ULPR_USER_FIFO_DECREMENT_LESS_THAN_ZERO_EN_MASK                     0x00000080
//========================================
/* REG_UL_POST_RX_ULPR_INT_ERROR_CLEAR 0x88 */
#define B1_UL_POST_RX_ULPR_INT_ERROR_CLEAR_FIELD_ULPR_COMMON_FIFO_DROP_CLR_MASK                                   0x00000001
#define B1_UL_POST_RX_ULPR_INT_ERROR_CLEAR_FIELD_ULPR_USER_NON_FIRST_FIFO_DROP_CLR_MASK                           0x00000002
#define B1_UL_POST_RX_ULPR_INT_ERROR_CLEAR_FIELD_ULPR_USER_FIRST_FIFO_DROP_CLR_MASK                               0x00000004
#define B1_UL_POST_RX_ULPR_INT_ERROR_CLEAR_FIELD_ULPR_PULSE_1_COLLISION_CLR_MASK                                  0x00000008
#define B1_UL_POST_RX_ULPR_INT_ERROR_CLEAR_FIELD_ULPR_PULSE_2_COLLISION_CLR_MASK                                  0x00000010
#define B1_UL_POST_RX_ULPR_INT_ERROR_CLEAR_FIELD_ULPR_NON_UNIQUE_CONTROL_ID_CLR_MASK                              0x00000020
#define B1_UL_POST_RX_ULPR_INT_ERROR_CLEAR_FIELD_ULPR_COMMON_FIFO_DECREMENT_LESS_THAN_ZERO_CLR_MASK               0x00000040
#define B1_UL_POST_RX_ULPR_INT_ERROR_CLEAR_FIELD_ULPR_USER_FIFO_DECREMENT_LESS_THAN_ZERO_CLR_MASK                 0x00000080
//========================================
/* REG_UL_POST_RX_ULPR_FIFO_NOT_EMPTY_STATUS 0x90 */
#define B1_UL_POST_RX_ULPR_FIFO_NOT_EMPTY_STATUS_FIELD_ULPR_COMMON_FIFO_NOT_EMPTY_STATUS_MASK                     0x00000001
#define B1_UL_POST_RX_ULPR_FIFO_NOT_EMPTY_STATUS_FIELD_ULPR_USER_FIFO_NOT_EMPTY_STATUS_MASK                       0x00000002
//========================================
/* REG_UL_POST_RX_ULPR_FIFO_NOT_EMPTY_EN 0x94 */
#define B1_UL_POST_RX_ULPR_FIFO_NOT_EMPTY_EN_FIELD_ULPR_COMMON_FIFO_NOT_EMPTY_ENABLE_MASK                         0x00000001
#define B1_UL_POST_RX_ULPR_FIFO_NOT_EMPTY_EN_FIELD_ULPR_USER_FIFO_NOT_EMPTY_ENABLE_MASK                           0x00000002
//========================================
/* REG_UL_POST_RX_ULPR_STATUS_0 0x9C */
#define B1_UL_POST_RX_ULPR_STATUS_0_FIELD_ULPR_CORE_SM_DBG_MASK                                                   0x00000007
#define B1_UL_POST_RX_ULPR_STATUS_0_FIELD_ULPR_RCR_SCAN_SM_DBG_MASK                                               0x00000018
#define B1_UL_POST_RX_ULPR_STATUS_0_FIELD_ULPR_INIT_SM_DBG_MASK                                                   0x00000060
#define B1_UL_POST_RX_ULPR_STATUS_0_FIELD_ULPR_USER_MNGR_SM_DBG_MASK                                              0x00000180
#define B1_UL_POST_RX_ULPR_STATUS_0_FIELD_ULPR_USER_GET_DATA_SM_DBG_MASK                                          0x00000600
#define B1_UL_POST_RX_ULPR_STATUS_0_FIELD_ULPR_FLD_CTCH_SM_DBG_MASK                                               0x00003800
#define B1_UL_POST_RX_ULPR_STATUS_0_FIELD_ULPR_PHY_STATUS_USER_SM_DBG_MASK                                        0x0000C000
#define B1_UL_POST_RX_ULPR_STATUS_0_FIELD_ULPR_USER_PROCESS_SM_DBG_MASK                                           0x00070000
#define B1_UL_POST_RX_ULPR_STATUS_0_FIELD_ULPR_TXSEL_SM_DBG_MASK                                                  0x00380000
#define B1_UL_POST_RX_ULPR_STATUS_0_FIELD_ULPR_USER_REPORT_SM_DBG_MASK                                            0x00C00000
#define B1_UL_POST_RX_ULPR_STATUS_0_FIELD_ULPR_FIFO_COMMON_USER_SM_DBG_MASK                                       0x07000000
//========================================
/* REG_UL_POST_RX_ULPR_CONTROL 0xA0 */
#define B1_UL_POST_RX_ULPR_CONTROL_FIELD_ULPR_ENABLE_MASK                                                         0x00000001
//========================================
/* REG_UL_POST_RX_ULPR_SPARE_REGISTER 0xB0 */
#define B1_UL_POST_RX_ULPR_SPARE_REGISTER_FIELD_SPARE_REGISTER_MASK                                               0x0000FFFF


#endif // _UL_POST_RX_REGS_H_
