
/***********************************************************************************
File:				B0TimIeRegs.h
Module:				b0TimIe
SOC Revision:		latest
Generated at:       2024-06-26 12:54:33
Purpose:
Description:		This File was auto generated using SOC Online

************************************************************************************/
#ifndef _B0_TIM_IE_REGS_H_
#define _B0_TIM_IE_REGS_H_

/*---------------------------------------------------------------------------------
/						Registers Addresses													 
/----------------------------------------------------------------------------------*/
#include "HwMemoryMap.h"

#define B0_TIM_IE_BASE_ADDRESS                            MEMORY_MAP_UNIT_64_BASE_ADDRESS
#define	REG_B0_TIM_IE_TX_SEL_TIM_LENGTH                   (B0_TIM_IE_BASE_ADDRESS + 0x0)
#define	REG_B0_TIM_IE_TX_SEL_TIM_DEBUG                    (B0_TIM_IE_BASE_ADDRESS + 0x4)
#define	REG_B0_TIM_IE_TX_SEL_TIM_HEADER                   (B0_TIM_IE_BASE_ADDRESS + 0x8)
#define	REG_B0_TIM_IE_TX_SEL_TIM_BITMAP_CONTROL           (B0_TIM_IE_BASE_ADDRESS + 0xC)
#define	REG_B0_TIM_IE_TX_SEL_TIM_MPS_GROUP_LENGTH         (B0_TIM_IE_BASE_ADDRESS + 0x10)
#define	REG_B0_TIM_IE_TX_SEL_TIM_MPS_GROUP                (B0_TIM_IE_BASE_ADDRESS + 0x14)
#define	REG_B0_TIM_IE_TX_SEL_TIM_VIRTUAL_BITMAP_LENGTH    (B0_TIM_IE_BASE_ADDRESS + 0x18)
#define	REG_B0_TIM_IE_TX_SEL_TIM_VIRTUAL_BITMAP_SHIFT     (B0_TIM_IE_BASE_ADDRESS + 0x1C)
#define	REG_B0_TIM_IE_TX_SEL_TIM_VIRTUAL_BITMAP0          (B0_TIM_IE_BASE_ADDRESS + 0x20)
#define	REG_B0_TIM_IE_TX_SEL_TIM_VIRTUAL_BITMAP1          (B0_TIM_IE_BASE_ADDRESS + 0x24)
#define	REG_B0_TIM_IE_TX_SEL_TIM_VIRTUAL_BITMAP2          (B0_TIM_IE_BASE_ADDRESS + 0x28)
#define	REG_B0_TIM_IE_TX_SEL_TIM_VIRTUAL_BITMAP3          (B0_TIM_IE_BASE_ADDRESS + 0x2C)
#define	REG_B0_TIM_IE_TX_SEL_TIM_VIRTUAL_BITMAP4          (B0_TIM_IE_BASE_ADDRESS + 0x30)
#define	REG_B0_TIM_IE_TX_SEL_TIM_VIRTUAL_BITMAP5          (B0_TIM_IE_BASE_ADDRESS + 0x34)
#define	REG_B0_TIM_IE_TX_SEL_TIM_VIRTUAL_BITMAP6          (B0_TIM_IE_BASE_ADDRESS + 0x38)
#define	REG_B0_TIM_IE_TX_SEL_TIM_VIRTUAL_BITMAP7          (B0_TIM_IE_BASE_ADDRESS + 0x3C)
#define	REG_B0_TIM_IE_TX_SEL_TIM_VIRTUAL_BITMAP8          (B0_TIM_IE_BASE_ADDRESS + 0x40)
#define	REG_B0_TIM_IE_TX_SEL_TIM_VIRTUAL_BITMAP9          (B0_TIM_IE_BASE_ADDRESS + 0x44)
#define	REG_B0_TIM_IE_TX_SEL_TIM_VIRTUAL_BITMAP10         (B0_TIM_IE_BASE_ADDRESS + 0x48)
#define	REG_B0_TIM_IE_TX_SEL_TIM_ERR                      (B0_TIM_IE_BASE_ADDRESS + 0x50)
#define	REG_B0_TIM_IE_TX_SEL_TIM_ERR_CLR                  (B0_TIM_IE_BASE_ADDRESS + 0x54)
#define	REG_B0_TIM_IE_TX_SEL_TIM_MULTI_BSSID_CFG          (B0_TIM_IE_BASE_ADDRESS + 0x58)
#define	REG_B0_TIM_IE_TX_SEL_TIM_MULTI_BSSID_MPS          (B0_TIM_IE_BASE_ADDRESS + 0x60)
#define	REG_B0_TIM_IE_TX_SEL_TIM_MULTI_BSSID_MPS_EN       (B0_TIM_IE_BASE_ADDRESS + 0x64)
#define	REG_B0_TIM_IE_TX_SEL_TIM_DTIM_READ_VAP_IDX        (B0_TIM_IE_BASE_ADDRESS + 0x68)
#define	REG_B0_TIM_IE_TX_SEL_TIM_DTIM_VALUE               (B0_TIM_IE_BASE_ADDRESS + 0x6C)
#define	REG_B0_TIM_IE_TX_SEL_TIM_STA_BITMAP_SEL           (B0_TIM_IE_BASE_ADDRESS + 0x70)
/*---------------------------------------------------------------------------------
/						Data Type Definition										
/----------------------------------------------------------------------------------*/
/*REG_B0_TIM_IE_TX_SEL_TIM_LENGTH 0x0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 timTotalLength : 8; //TIM IE Total Byte length, reset value: 0x6, access type: RO
		uint32 reserved0 : 24;
	} bitFields;
} RegB0TimIeTxSelTimLength_u;

/*REG_B0_TIM_IE_TX_SEL_TIM_DEBUG 0x4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 timEmpty : 1; //TIM IE empty indication read by HOB, reset value: 0x1, access type: RO
		uint32 timReady : 1; //TIM IE ready to be transmitted indication, reset value: 0x0, access type: RO
		uint32 reserved0 : 30;
	} bitFields;
} RegB0TimIeTxSelTimDebug_u;

/*REG_B0_TIM_IE_TX_SEL_TIM_HEADER 0x8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 timHeaderElementId : 8; //Element ID value, reset value: 0x5, access type: RO
		uint32 timHeaderLength : 8; //TIM IE Byte length, reset value: 0x4, access type: RO
		uint32 dtimCount : 8; //DTIM counter value, reset value: 0x0, access type: RO
		uint32 dtimPeriod : 8; //DTIM period value, reset value: 0x0, access type: RO
	} bitFields;
} RegB0TimIeTxSelTimHeader_u;

/*REG_B0_TIM_IE_TX_SEL_TIM_BITMAP_CONTROL 0xC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 timBitmapControl : 8; //Control field value, reset value: 0x0, access type: RO
		uint32 reserved0 : 24;
	} bitFields;
} RegB0TimIeTxSelTimBitmapControl_u;

/*REG_B0_TIM_IE_TX_SEL_TIM_MPS_GROUP_LENGTH 0x10 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 timMpsGroupLength : 3; //Length of MPS bits to be transmitted in TIM IE, reset value: 0x0, access type: RO
		uint32 reserved0 : 29;
	} bitFields;
} RegB0TimIeTxSelTimMpsGroupLength_u;

/*REG_B0_TIM_IE_TX_SEL_TIM_MPS_GROUP 0x14 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 timMpsGroup : 32; //MPS result of the VAP/VAPs activated in the TIM IE, to be transmitted in TIM IE, reset value: 0x0, access type: RO
	} bitFields;
} RegB0TimIeTxSelTimMpsGroup_u;

/*REG_B0_TIM_IE_TX_SEL_TIM_VIRTUAL_BITMAP_LENGTH 0x18 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 timVirtualBitmapLength : 8; //TIM Virtual bitmap length to be transmitted, reset value: 0x1, access type: RO
		uint32 timVirtualBitmapOffset : 8; //TIM Virtual bitmap offset to be transmitted from, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegB0TimIeTxSelTimVirtualBitmapLength_u;

/*REG_B0_TIM_IE_TX_SEL_TIM_VIRTUAL_BITMAP_SHIFT 0x1C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 timVirtualBitmapShiftNum : 2; //TIM virtual bitmap shift number of bytes, reset value: 0x0, access type: RW
		uint32 reserved0 : 6;
		uint32 timVirtualBitmapShiftRight : 1; //TIM virtual bitmap shift side. 1 - right, 0- left, reset value: 0x0, access type: RW
		uint32 reserved1 : 23;
	} bitFields;
} RegB0TimIeTxSelTimVirtualBitmapShift_u;

/*REG_B0_TIM_IE_TX_SEL_TIM_VIRTUAL_BITMAP0 0x20 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 timVirtualBitmap0 : 32; //TIM Virtual Bitmap value. Bytes 0-3 of bitmap., reset value: 0x0, access type: RO
	} bitFields;
} RegB0TimIeTxSelTimVirtualBitmap0_u;

/*REG_B0_TIM_IE_TX_SEL_TIM_VIRTUAL_BITMAP1 0x24 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 timVirtualBitmap1 : 32; //TIM Virtual Bitmap value. Bytes 4-7 of bitmap., reset value: 0x0, access type: RO
	} bitFields;
} RegB0TimIeTxSelTimVirtualBitmap1_u;

/*REG_B0_TIM_IE_TX_SEL_TIM_VIRTUAL_BITMAP2 0x28 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 timVirtualBitmap2 : 32; //TIM Virtual Bitmap value. Bytes 8-11 of bitmap., reset value: 0x0, access type: RO
	} bitFields;
} RegB0TimIeTxSelTimVirtualBitmap2_u;

/*REG_B0_TIM_IE_TX_SEL_TIM_VIRTUAL_BITMAP3 0x2C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 timVirtualBitmap3 : 32; //TIM Virtual Bitmap value. Bytes 12-15 of bitmap., reset value: 0x0, access type: RO
	} bitFields;
} RegB0TimIeTxSelTimVirtualBitmap3_u;

/*REG_B0_TIM_IE_TX_SEL_TIM_VIRTUAL_BITMAP4 0x30 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 timVirtualBitmap4 : 32; //TIM Virtual Bitmap value. Bytes 16-19 of bitmap., reset value: 0x0, access type: RO
	} bitFields;
} RegB0TimIeTxSelTimVirtualBitmap4_u;

/*REG_B0_TIM_IE_TX_SEL_TIM_VIRTUAL_BITMAP5 0x34 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 timVirtualBitmap5 : 32; //TIM Virtual Bitmap value. Bytes 20-23 of bitmap., reset value: 0x0, access type: RO
	} bitFields;
} RegB0TimIeTxSelTimVirtualBitmap5_u;

/*REG_B0_TIM_IE_TX_SEL_TIM_VIRTUAL_BITMAP6 0x38 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 timVirtualBitmap6 : 32; //TIM Virtual Bitmap value. Bytes 24-27 of bitmap., reset value: 0x0, access type: RO
	} bitFields;
} RegB0TimIeTxSelTimVirtualBitmap6_u;

/*REG_B0_TIM_IE_TX_SEL_TIM_VIRTUAL_BITMAP7 0x3C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 timVirtualBitmap7 : 32; //TIM Virtual Bitmap value. Bytes 28-31 of bitmap., reset value: 0x0, access type: RO
	} bitFields;
} RegB0TimIeTxSelTimVirtualBitmap7_u;

/*REG_B0_TIM_IE_TX_SEL_TIM_VIRTUAL_BITMAP8 0x40 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 timVirtualBitmap8 : 32; //TIM Virtual Bitmap value. Bytes 32-35 of bitmap., reset value: 0x0, access type: RO
	} bitFields;
} RegB0TimIeTxSelTimVirtualBitmap8_u;

/*REG_B0_TIM_IE_TX_SEL_TIM_VIRTUAL_BITMAP9 0x44 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 timVirtualBitmap9 : 32; //TIM Virtual Bitmap value. Bytes 36-39 of bitmap., reset value: 0x0, access type: RO
	} bitFields;
} RegB0TimIeTxSelTimVirtualBitmap9_u;

/*REG_B0_TIM_IE_TX_SEL_TIM_VIRTUAL_BITMAP10 0x48 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 timVirtualBitmap10 : 32; //TIM Virtual Bitmap value. Bytes 40-43 of bitmap., reset value: 0x0, access type: RO
	} bitFields;
} RegB0TimIeTxSelTimVirtualBitmap10_u;

/*REG_B0_TIM_IE_TX_SEL_TIM_ERR 0x50 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 timWrongVapSel : 32; //Selection of VAP without Beacon enabled, reset value: 0x0, access type: RO
	} bitFields;
} RegB0TimIeTxSelTimErr_u;

/*REG_B0_TIM_IE_TX_SEL_TIM_ERR_CLR 0x54 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 timWrongVapSelClr : 32; //Selection of VAP without Beacon enabled clear, reset value: 0x0, access type: WO
	} bitFields;
} RegB0TimIeTxSelTimErrClr_u;

/*REG_B0_TIM_IE_TX_SEL_TIM_MULTI_BSSID_CFG 0x58 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 timIeCompressed : 1; //Defines if TIM IE is compressed or not. Legacy TIM IE is compressed only., reset value: 0x1, access type: RW
		uint32 reserved0 : 1;
		uint32 primaryVapIs16 : 1; //Primary VAP cfg: , 0: Primary VAP is 0 , 1: Primary VAP is 16, reset value: 0x0, access type: RW
		uint32 reserved1 : 1;
		uint32 multiBssidGroupSize : 6; //Number of VAPs in group. Supported values are 0(Single BSSID),2,4,8,16,32, reset value: 0x0, access type: RW
		uint32 reserved2 : 22;
	} bitFields;
} RegB0TimIeTxSelTimMultiBssidCfg_u;

/*REG_B0_TIM_IE_TX_SEL_TIM_MULTI_BSSID_MPS 0x60 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mps : 32; //MPS result of the VAP/VAPs activated in the TIM IE, reset value: 0x0, access type: RO
	} bitFields;
} RegB0TimIeTxSelTimMultiBssidMps_u;

/*REG_B0_TIM_IE_TX_SEL_TIM_MULTI_BSSID_MPS_EN 0x64 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mpsEn : 32; //MPS valid VAP/VAPs in the TIM IE, reset value: 0x0, access type: RO
	} bitFields;
} RegB0TimIeTxSelTimMultiBssidMpsEn_u;

/*REG_B0_TIM_IE_TX_SEL_TIM_DTIM_READ_VAP_IDX 0x68 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSelTimDtimReadVapIdx : 5; //VAP index of which the DTIM values are reflected in the adjacent register., reset value: 0x0, access type: RW
		uint32 reserved0 : 27;
	} bitFields;
} RegB0TimIeTxSelTimDtimReadVapIdx_u;

/*REG_B0_TIM_IE_TX_SEL_TIM_DTIM_VALUE 0x6C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dtimPeriodRead : 8; //Number of TIMs minus 1 between DTIMs, reset value: 0x0, access type: RO
		uint32 dtimCountRead : 8; //DTIM count, when equals 0 it is DTIM, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegB0TimIeTxSelTimDtimValue_u;

/*REG_B0_TIM_IE_TX_SEL_TIM_STA_BITMAP_SEL 0x70 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSelTimStaBitmapSel : 32; //Select which STA bitmap to be used by TIM IE,1 bit per VAP: , 0: Valid PS per STA. , 1: Data in Queue per STA., reset value: 0x0, access type: RW
	} bitFields;
} RegB0TimIeTxSelTimStaBitmapSel_u;

//========================================
/* REG_TIM_IE_TX_SEL_TIM_LENGTH 0x0 */
#define B0_TIM_IE_TX_SEL_TIM_LENGTH_FIELD_TIM_TOTAL_LENGTH_MASK                                                   0x000000FF
//========================================
/* REG_TIM_IE_TX_SEL_TIM_DEBUG 0x4 */
#define B0_TIM_IE_TX_SEL_TIM_DEBUG_FIELD_TIM_EMPTY_MASK                                                           0x00000001
#define B0_TIM_IE_TX_SEL_TIM_DEBUG_FIELD_TIM_READY_MASK                                                           0x00000002
//========================================
/* REG_TIM_IE_TX_SEL_TIM_HEADER 0x8 */
#define B0_TIM_IE_TX_SEL_TIM_HEADER_FIELD_TIM_HEADER_ELEMENT_ID_MASK                                              0x000000FF
#define B0_TIM_IE_TX_SEL_TIM_HEADER_FIELD_TIM_HEADER_LENGTH_MASK                                                  0x0000FF00
#define B0_TIM_IE_TX_SEL_TIM_HEADER_FIELD_DTIM_COUNT_MASK                                                         0x00FF0000
#define B0_TIM_IE_TX_SEL_TIM_HEADER_FIELD_DTIM_PERIOD_MASK                                                        0xFF000000
//========================================
/* REG_TIM_IE_TX_SEL_TIM_BITMAP_CONTROL 0xC */
#define B0_TIM_IE_TX_SEL_TIM_BITMAP_CONTROL_FIELD_TIM_BITMAP_CONTROL_MASK                                         0x000000FF
//========================================
/* REG_TIM_IE_TX_SEL_TIM_MPS_GROUP_LENGTH 0x10 */
#define B0_TIM_IE_TX_SEL_TIM_MPS_GROUP_LENGTH_FIELD_TIM_MPS_GROUP_LENGTH_MASK                                     0x00000007
//========================================
/* REG_TIM_IE_TX_SEL_TIM_MPS_GROUP 0x14 */
#define B0_TIM_IE_TX_SEL_TIM_MPS_GROUP_FIELD_TIM_MPS_GROUP_MASK                                                   0xFFFFFFFF
//========================================
/* REG_TIM_IE_TX_SEL_TIM_VIRTUAL_BITMAP_LENGTH 0x18 */
#define B0_TIM_IE_TX_SEL_TIM_VIRTUAL_BITMAP_LENGTH_FIELD_TIM_VIRTUAL_BITMAP_LENGTH_MASK                           0x000000FF
#define B0_TIM_IE_TX_SEL_TIM_VIRTUAL_BITMAP_LENGTH_FIELD_TIM_VIRTUAL_BITMAP_OFFSET_MASK                           0x0000FF00
//========================================
/* REG_TIM_IE_TX_SEL_TIM_VIRTUAL_BITMAP_SHIFT 0x1C */
#define B0_TIM_IE_TX_SEL_TIM_VIRTUAL_BITMAP_SHIFT_FIELD_TIM_VIRTUAL_BITMAP_SHIFT_NUM_MASK                         0x00000003
#define B0_TIM_IE_TX_SEL_TIM_VIRTUAL_BITMAP_SHIFT_FIELD_TIM_VIRTUAL_BITMAP_SHIFT_RIGHT_MASK                       0x00000100
//========================================
/* REG_TIM_IE_TX_SEL_TIM_VIRTUAL_BITMAP0 0x20 */
#define B0_TIM_IE_TX_SEL_TIM_VIRTUAL_BITMAP0_FIELD_TIM_VIRTUAL_BITMAP0_MASK                                       0xFFFFFFFF
//========================================
/* REG_TIM_IE_TX_SEL_TIM_VIRTUAL_BITMAP1 0x24 */
#define B0_TIM_IE_TX_SEL_TIM_VIRTUAL_BITMAP1_FIELD_TIM_VIRTUAL_BITMAP1_MASK                                       0xFFFFFFFF
//========================================
/* REG_TIM_IE_TX_SEL_TIM_VIRTUAL_BITMAP2 0x28 */
#define B0_TIM_IE_TX_SEL_TIM_VIRTUAL_BITMAP2_FIELD_TIM_VIRTUAL_BITMAP2_MASK                                       0xFFFFFFFF
//========================================
/* REG_TIM_IE_TX_SEL_TIM_VIRTUAL_BITMAP3 0x2C */
#define B0_TIM_IE_TX_SEL_TIM_VIRTUAL_BITMAP3_FIELD_TIM_VIRTUAL_BITMAP3_MASK                                       0xFFFFFFFF
//========================================
/* REG_TIM_IE_TX_SEL_TIM_VIRTUAL_BITMAP4 0x30 */
#define B0_TIM_IE_TX_SEL_TIM_VIRTUAL_BITMAP4_FIELD_TIM_VIRTUAL_BITMAP4_MASK                                       0xFFFFFFFF
//========================================
/* REG_TIM_IE_TX_SEL_TIM_VIRTUAL_BITMAP5 0x34 */
#define B0_TIM_IE_TX_SEL_TIM_VIRTUAL_BITMAP5_FIELD_TIM_VIRTUAL_BITMAP5_MASK                                       0xFFFFFFFF
//========================================
/* REG_TIM_IE_TX_SEL_TIM_VIRTUAL_BITMAP6 0x38 */
#define B0_TIM_IE_TX_SEL_TIM_VIRTUAL_BITMAP6_FIELD_TIM_VIRTUAL_BITMAP6_MASK                                       0xFFFFFFFF
//========================================
/* REG_TIM_IE_TX_SEL_TIM_VIRTUAL_BITMAP7 0x3C */
#define B0_TIM_IE_TX_SEL_TIM_VIRTUAL_BITMAP7_FIELD_TIM_VIRTUAL_BITMAP7_MASK                                       0xFFFFFFFF
//========================================
/* REG_TIM_IE_TX_SEL_TIM_VIRTUAL_BITMAP8 0x40 */
#define B0_TIM_IE_TX_SEL_TIM_VIRTUAL_BITMAP8_FIELD_TIM_VIRTUAL_BITMAP8_MASK                                       0xFFFFFFFF
//========================================
/* REG_TIM_IE_TX_SEL_TIM_VIRTUAL_BITMAP9 0x44 */
#define B0_TIM_IE_TX_SEL_TIM_VIRTUAL_BITMAP9_FIELD_TIM_VIRTUAL_BITMAP9_MASK                                       0xFFFFFFFF
//========================================
/* REG_TIM_IE_TX_SEL_TIM_VIRTUAL_BITMAP10 0x48 */
#define B0_TIM_IE_TX_SEL_TIM_VIRTUAL_BITMAP10_FIELD_TIM_VIRTUAL_BITMAP10_MASK                                     0xFFFFFFFF
//========================================
/* REG_TIM_IE_TX_SEL_TIM_ERR 0x50 */
#define B0_TIM_IE_TX_SEL_TIM_ERR_FIELD_TIM_WRONG_VAP_SEL_MASK                                                     0xFFFFFFFF
//========================================
/* REG_TIM_IE_TX_SEL_TIM_ERR_CLR 0x54 */
#define B0_TIM_IE_TX_SEL_TIM_ERR_CLR_FIELD_TIM_WRONG_VAP_SEL_CLR_MASK                                             0xFFFFFFFF
//========================================
/* REG_TIM_IE_TX_SEL_TIM_MULTI_BSSID_CFG 0x58 */
#define B0_TIM_IE_TX_SEL_TIM_MULTI_BSSID_CFG_FIELD_TIM_IE_COMPRESSED_MASK                                         0x00000001
#define B0_TIM_IE_TX_SEL_TIM_MULTI_BSSID_CFG_FIELD_PRIMARY_VAP_IS16_MASK                                          0x00000004
#define B0_TIM_IE_TX_SEL_TIM_MULTI_BSSID_CFG_FIELD_MULTI_BSSID_GROUP_SIZE_MASK                                    0x000003F0
//========================================
/* REG_TIM_IE_TX_SEL_TIM_MULTI_BSSID_MPS 0x60 */
#define B0_TIM_IE_TX_SEL_TIM_MULTI_BSSID_MPS_FIELD_MPS_MASK                                                       0xFFFFFFFF
//========================================
/* REG_TIM_IE_TX_SEL_TIM_MULTI_BSSID_MPS_EN 0x64 */
#define B0_TIM_IE_TX_SEL_TIM_MULTI_BSSID_MPS_EN_FIELD_MPS_EN_MASK                                                 0xFFFFFFFF
//========================================
/* REG_TIM_IE_TX_SEL_TIM_DTIM_READ_VAP_IDX 0x68 */
#define B0_TIM_IE_TX_SEL_TIM_DTIM_READ_VAP_IDX_FIELD_TX_SEL_TIM_DTIM_READ_VAP_IDX_MASK                            0x0000001F
//========================================
/* REG_TIM_IE_TX_SEL_TIM_DTIM_VALUE 0x6C */
#define B0_TIM_IE_TX_SEL_TIM_DTIM_VALUE_FIELD_DTIM_PERIOD_READ_MASK                                               0x000000FF
#define B0_TIM_IE_TX_SEL_TIM_DTIM_VALUE_FIELD_DTIM_COUNT_READ_MASK                                                0x0000FF00
//========================================
/* REG_TIM_IE_TX_SEL_TIM_STA_BITMAP_SEL 0x70 */
#define B0_TIM_IE_TX_SEL_TIM_STA_BITMAP_SEL_FIELD_TX_SEL_TIM_STA_BITMAP_SEL_MASK                                  0xFFFFFFFF


#endif // _TIM_IE_REGS_H_
