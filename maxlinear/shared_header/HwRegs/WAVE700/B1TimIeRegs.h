
/***********************************************************************************
File:				B1TimIeRegs.h
Module:				b1TimIe
SOC Revision:		latest
Generated at:       2022-05-04 10:57:57
Purpose:
Description:		This File was auto generated using SOC Online

************************************************************************************/
#ifndef _B1_TIM_IE_REGS_H_
#define _B1_TIM_IE_REGS_H_

/*---------------------------------------------------------------------------------
/						Registers Addresses													 
/----------------------------------------------------------------------------------*/
#include "HwMemoryMap.h"

#define B1_TIM_IE_BASE_ADDRESS                             MEMORY_MAP_UNIT_10064_BASE_ADDRESS
#define	REG_B1_TIM_IE_TX_SEL_TIM_LENGTH                   (B1_TIM_IE_BASE_ADDRESS + 0x0)
#define	REG_B1_TIM_IE_TX_SEL_TIM_DEBUG                    (B1_TIM_IE_BASE_ADDRESS + 0x4)
#define	REG_B1_TIM_IE_TX_SEL_TIM_HEADER                   (B1_TIM_IE_BASE_ADDRESS + 0x8)
#define	REG_B1_TIM_IE_TX_SEL_TIM_BITMAP_CONTROL           (B1_TIM_IE_BASE_ADDRESS + 0xC)
#define	REG_B1_TIM_IE_TX_SEL_TIM_MPS_GROUP_LENGTH         (B1_TIM_IE_BASE_ADDRESS + 0x10)
#define	REG_B1_TIM_IE_TX_SEL_TIM_MPS_GROUP                (B1_TIM_IE_BASE_ADDRESS + 0x14)
#define	REG_B1_TIM_IE_TX_SEL_TIM_VIRTUAL_BITMAP_LENGTH    (B1_TIM_IE_BASE_ADDRESS + 0x18)
#define	REG_B1_TIM_IE_TX_SEL_TIM_VIRTUAL_BITMAP_SHIFT     (B1_TIM_IE_BASE_ADDRESS + 0x1C)
#define	REG_B1_TIM_IE_TX_SEL_TIM_VIRTUAL_BITMAP0          (B1_TIM_IE_BASE_ADDRESS + 0x20)
#define	REG_B1_TIM_IE_TX_SEL_TIM_VIRTUAL_BITMAP1          (B1_TIM_IE_BASE_ADDRESS + 0x24)
#define	REG_B1_TIM_IE_TX_SEL_TIM_VIRTUAL_BITMAP2          (B1_TIM_IE_BASE_ADDRESS + 0x28)
#define	REG_B1_TIM_IE_TX_SEL_TIM_VIRTUAL_BITMAP3          (B1_TIM_IE_BASE_ADDRESS + 0x2C)
#define	REG_B1_TIM_IE_TX_SEL_TIM_VIRTUAL_BITMAP4          (B1_TIM_IE_BASE_ADDRESS + 0x30)
#define	REG_B1_TIM_IE_TX_SEL_TIM_VIRTUAL_BITMAP5          (B1_TIM_IE_BASE_ADDRESS + 0x34)
#define	REG_B1_TIM_IE_TX_SEL_TIM_VIRTUAL_BITMAP6          (B1_TIM_IE_BASE_ADDRESS + 0x38)
#define	REG_B1_TIM_IE_TX_SEL_TIM_VIRTUAL_BITMAP7          (B1_TIM_IE_BASE_ADDRESS + 0x3C)
#define	REG_B1_TIM_IE_TX_SEL_TIM_VIRTUAL_BITMAP8          (B1_TIM_IE_BASE_ADDRESS + 0x40)
#define	REG_B1_TIM_IE_TX_SEL_TIM_VIRTUAL_BITMAP9          (B1_TIM_IE_BASE_ADDRESS + 0x44)
#define	REG_B1_TIM_IE_TX_SEL_TIM_VIRTUAL_BITMAP10         (B1_TIM_IE_BASE_ADDRESS + 0x48)
#define	REG_B1_TIM_IE_TX_SEL_TIM_ERR                      (B1_TIM_IE_BASE_ADDRESS + 0x50)
#define	REG_B1_TIM_IE_TX_SEL_TIM_ERR_CLR                  (B1_TIM_IE_BASE_ADDRESS + 0x54)
#define	REG_B1_TIM_IE_TX_SEL_TIM_MULTI_BSSID_CFG          (B1_TIM_IE_BASE_ADDRESS + 0x58)
#define	REG_B1_TIM_IE_TX_SEL_TIM_MULTI_BSSID_MPS          (B1_TIM_IE_BASE_ADDRESS + 0x60)
#define	REG_B1_TIM_IE_TX_SEL_TIM_MULTI_BSSID_MPS_EN       (B1_TIM_IE_BASE_ADDRESS + 0x64)
#define	REG_B1_TIM_IE_TX_SEL_TIM_DTIM_READ_VAP_IDX        (B1_TIM_IE_BASE_ADDRESS + 0x68)
#define	REG_B1_TIM_IE_TX_SEL_TIM_DTIM_VALUE               (B1_TIM_IE_BASE_ADDRESS + 0x6C)
#define	REG_B1_TIM_IE_TX_SEL_TIM_STA_BITMAP_SEL           (B1_TIM_IE_BASE_ADDRESS + 0x70)
/*---------------------------------------------------------------------------------
/						Data Type Definition										
/----------------------------------------------------------------------------------*/
/*REG_B1_TIM_IE_TX_SEL_TIM_LENGTH 0x0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 timTotalLength : 8; //TIM IE Total Byte length, reset value: 0x6, access type: RO
		uint32 reserved0 : 24;
	} bitFields;
} RegB1TimIeTxSelTimLength_u;

/*REG_B1_TIM_IE_TX_SEL_TIM_DEBUG 0x4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 timEmpty : 1; //TIM IE empty indication read by HOB, reset value: 0x1, access type: RO
		uint32 timReady : 1; //TIM IE ready to be transmitted indication, reset value: 0x0, access type: RO
		uint32 reserved0 : 30;
	} bitFields;
} RegB1TimIeTxSelTimDebug_u;

/*REG_B1_TIM_IE_TX_SEL_TIM_HEADER 0x8 */
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
} RegB1TimIeTxSelTimHeader_u;

/*REG_B1_TIM_IE_TX_SEL_TIM_BITMAP_CONTROL 0xC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 timBitmapControl : 8; //Control field value, reset value: 0x0, access type: RO
		uint32 reserved0 : 24;
	} bitFields;
} RegB1TimIeTxSelTimBitmapControl_u;

/*REG_B1_TIM_IE_TX_SEL_TIM_MPS_GROUP_LENGTH 0x10 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 timMpsGroupLength : 3; //Length of MPS bits to be transmitted in TIM IE, reset value: 0x0, access type: RO
		uint32 reserved0 : 29;
	} bitFields;
} RegB1TimIeTxSelTimMpsGroupLength_u;

/*REG_B1_TIM_IE_TX_SEL_TIM_MPS_GROUP 0x14 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 timMpsGroup : 32; //MPS result of the VAP/VAPs activated in the TIM IE, to be transmitted in TIM IE, reset value: 0x0, access type: RO
	} bitFields;
} RegB1TimIeTxSelTimMpsGroup_u;

/*REG_B1_TIM_IE_TX_SEL_TIM_VIRTUAL_BITMAP_LENGTH 0x18 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 timVirtualBitmapLength : 8; //TIM Virtual bitmap length to be transmitted, reset value: 0x1, access type: RO
		uint32 timVirtualBitmapOffset : 8; //TIM Virtual bitmap offset to be transmitted from, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegB1TimIeTxSelTimVirtualBitmapLength_u;

/*REG_B1_TIM_IE_TX_SEL_TIM_VIRTUAL_BITMAP_SHIFT 0x1C */
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
} RegB1TimIeTxSelTimVirtualBitmapShift_u;

/*REG_B1_TIM_IE_TX_SEL_TIM_VIRTUAL_BITMAP0 0x20 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 timVirtualBitmap0 : 32; //TIM Virtual Bitmap value. Bytes 0-3 of bitmap., reset value: 0x0, access type: RO
	} bitFields;
} RegB1TimIeTxSelTimVirtualBitmap0_u;

/*REG_B1_TIM_IE_TX_SEL_TIM_VIRTUAL_BITMAP1 0x24 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 timVirtualBitmap1 : 32; //TIM Virtual Bitmap value. Bytes 4-7 of bitmap., reset value: 0x0, access type: RO
	} bitFields;
} RegB1TimIeTxSelTimVirtualBitmap1_u;

/*REG_B1_TIM_IE_TX_SEL_TIM_VIRTUAL_BITMAP2 0x28 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 timVirtualBitmap2 : 32; //TIM Virtual Bitmap value. Bytes 8-11 of bitmap., reset value: 0x0, access type: RO
	} bitFields;
} RegB1TimIeTxSelTimVirtualBitmap2_u;

/*REG_B1_TIM_IE_TX_SEL_TIM_VIRTUAL_BITMAP3 0x2C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 timVirtualBitmap3 : 32; //TIM Virtual Bitmap value. Bytes 12-15 of bitmap., reset value: 0x0, access type: RO
	} bitFields;
} RegB1TimIeTxSelTimVirtualBitmap3_u;

/*REG_B1_TIM_IE_TX_SEL_TIM_VIRTUAL_BITMAP4 0x30 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 timVirtualBitmap4 : 32; //TIM Virtual Bitmap value. Bytes 16-19 of bitmap., reset value: 0x0, access type: RO
	} bitFields;
} RegB1TimIeTxSelTimVirtualBitmap4_u;

/*REG_B1_TIM_IE_TX_SEL_TIM_VIRTUAL_BITMAP5 0x34 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 timVirtualBitmap5 : 32; //TIM Virtual Bitmap value. Bytes 20-23 of bitmap., reset value: 0x0, access type: RO
	} bitFields;
} RegB1TimIeTxSelTimVirtualBitmap5_u;

/*REG_B1_TIM_IE_TX_SEL_TIM_VIRTUAL_BITMAP6 0x38 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 timVirtualBitmap6 : 32; //TIM Virtual Bitmap value. Bytes 24-27 of bitmap., reset value: 0x0, access type: RO
	} bitFields;
} RegB1TimIeTxSelTimVirtualBitmap6_u;

/*REG_B1_TIM_IE_TX_SEL_TIM_VIRTUAL_BITMAP7 0x3C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 timVirtualBitmap7 : 32; //TIM Virtual Bitmap value. Bytes 28-31 of bitmap., reset value: 0x0, access type: RO
	} bitFields;
} RegB1TimIeTxSelTimVirtualBitmap7_u;

/*REG_B1_TIM_IE_TX_SEL_TIM_VIRTUAL_BITMAP8 0x40 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 timVirtualBitmap8 : 32; //TIM Virtual Bitmap value. Bytes 32-35 of bitmap., reset value: 0x0, access type: RO
	} bitFields;
} RegB1TimIeTxSelTimVirtualBitmap8_u;

/*REG_B1_TIM_IE_TX_SEL_TIM_VIRTUAL_BITMAP9 0x44 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 timVirtualBitmap9 : 32; //TIM Virtual Bitmap value. Bytes 36-39 of bitmap., reset value: 0x0, access type: RO
	} bitFields;
} RegB1TimIeTxSelTimVirtualBitmap9_u;

/*REG_B1_TIM_IE_TX_SEL_TIM_VIRTUAL_BITMAP10 0x48 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 timVirtualBitmap10 : 32; //TIM Virtual Bitmap value. Bytes 40-43 of bitmap., reset value: 0x0, access type: RO
	} bitFields;
} RegB1TimIeTxSelTimVirtualBitmap10_u;

/*REG_B1_TIM_IE_TX_SEL_TIM_ERR 0x50 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 timWrongVapSel : 32; //Selection of VAP without Beacon enabled, reset value: 0x0, access type: RO
	} bitFields;
} RegB1TimIeTxSelTimErr_u;

/*REG_B1_TIM_IE_TX_SEL_TIM_ERR_CLR 0x54 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 timWrongVapSelClr : 32; //Selection of VAP without Beacon enabled clear, reset value: 0x0, access type: WO
	} bitFields;
} RegB1TimIeTxSelTimErrClr_u;

/*REG_B1_TIM_IE_TX_SEL_TIM_MULTI_BSSID_CFG 0x58 */
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
} RegB1TimIeTxSelTimMultiBssidCfg_u;

/*REG_B1_TIM_IE_TX_SEL_TIM_MULTI_BSSID_MPS 0x60 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mps : 32; //MPS result of the VAP/VAPs activated in the TIM IE, reset value: 0x0, access type: RO
	} bitFields;
} RegB1TimIeTxSelTimMultiBssidMps_u;

/*REG_B1_TIM_IE_TX_SEL_TIM_MULTI_BSSID_MPS_EN 0x64 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mpsEn : 32; //MPS valid VAP/VAPs in the TIM IE, reset value: 0x0, access type: RO
	} bitFields;
} RegB1TimIeTxSelTimMultiBssidMpsEn_u;

/*REG_B1_TIM_IE_TX_SEL_TIM_DTIM_READ_VAP_IDX 0x68 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSelTimDtimReadVapIdx : 5; //VAP index of which the DTIM values are reflected in the adjacent register., reset value: 0x0, access type: RW
		uint32 reserved0 : 27;
	} bitFields;
} RegB1TimIeTxSelTimDtimReadVapIdx_u;

/*REG_B1_TIM_IE_TX_SEL_TIM_DTIM_VALUE 0x6C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dtimPeriodRead : 8; //Number of TIMs minus 1 between DTIMs, reset value: 0x0, access type: RO
		uint32 dtimCountRead : 8; //DTIM count, when equals 0 it is DTIM, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegB1TimIeTxSelTimDtimValue_u;

/*REG_B1_TIM_IE_TX_SEL_TIM_STA_BITMAP_SEL 0x70 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSelTimStaBitmapSel : 32; //Select which STA bitmap to be used by TIM IE,1 bit per VAP: , 0: Valid PS per STA. , 1: Data in Queue per STA., reset value: 0x0, access type: RW
	} bitFields;
} RegB1TimIeTxSelTimStaBitmapSel_u;



#endif // _B1_TIM_IE_REGS_H_
