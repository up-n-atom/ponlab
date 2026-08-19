
/***********************************************************************************
File:				TxSelectorRegs.h
Module:				txSelector
SOC Revision:		latest
Generated at:       2024-06-26 12:55:23
Purpose:
Description:		This File was auto generated using SOC Online

************************************************************************************/
#ifndef _TX_SELECTOR_REGS_H_
#define _TX_SELECTOR_REGS_H_

/*---------------------------------------------------------------------------------
/						Registers Addresses													 
/----------------------------------------------------------------------------------*/
#include "HwMemoryMap.h"

#define TX_SELECTOR_BASE_ADDRESS                                MEMORY_MAP_UNIT_29_BASE_ADDRESS
#define	REG_TX_SELECTOR_TX_SEL_INSTR                              (TX_SELECTOR_BASE_ADDRESS + 0x0)
#define	REG_TX_SELECTOR_TX_SEL_BITMAP_UPDATE_MASTER0              (TX_SELECTOR_BASE_ADDRESS + 0x4)
#define	REG_TX_SELECTOR_TX_SEL_BITMAP_UPDATE_DATA_MASTER0         (TX_SELECTOR_BASE_ADDRESS + 0x8)
#define	REG_TX_SELECTOR_TX_SEL_BITMAP_UPDATE_MASTER1              (TX_SELECTOR_BASE_ADDRESS + 0xC)
#define	REG_TX_SELECTOR_TX_SEL_BITMAP_UPDATE_DATA_MASTER1         (TX_SELECTOR_BASE_ADDRESS + 0x10)
#define	REG_TX_SELECTOR_TX_SEL_BITMAP_UPDATE_MASTER2              (TX_SELECTOR_BASE_ADDRESS + 0x14)
#define	REG_TX_SELECTOR_TX_SEL_BITMAP_UPDATE_DATA_MASTER2         (TX_SELECTOR_BASE_ADDRESS + 0x18)
#define	REG_TX_SELECTOR_TX_SEL_BITMAP_UPDATE_MASTER3              (TX_SELECTOR_BASE_ADDRESS + 0x1C)
#define	REG_TX_SELECTOR_TX_SEL_BITMAP_UPDATE_DATA_MASTER3         (TX_SELECTOR_BASE_ADDRESS + 0x20)
#define	REG_TX_SELECTOR_TX_SEL_STA_PS_WRR_LOW0                    (TX_SELECTOR_BASE_ADDRESS + 0x24)
#define	REG_TX_SELECTOR_TX_SEL_STA_PS_WRR_LOW1                    (TX_SELECTOR_BASE_ADDRESS + 0x28)
#define	REG_TX_SELECTOR_TX_SEL_STA_PS_WRR_HIGH0                   (TX_SELECTOR_BASE_ADDRESS + 0x2C)
#define	REG_TX_SELECTOR_TX_SEL_STA_PS_WRR_HIGH1                   (TX_SELECTOR_BASE_ADDRESS + 0x30)
#define	REG_TX_SELECTOR_TX_SEL_STA_ACTIVE_WRR_LOW0                (TX_SELECTOR_BASE_ADDRESS + 0x34)
#define	REG_TX_SELECTOR_TX_SEL_STA_ACTIVE_WRR_LOW1                (TX_SELECTOR_BASE_ADDRESS + 0x38)
#define	REG_TX_SELECTOR_TX_SEL_STA_ACTIVE_WRR_HIGH0               (TX_SELECTOR_BASE_ADDRESS + 0x3C)
#define	REG_TX_SELECTOR_TX_SEL_STA_ACTIVE_WRR_HIGH1               (TX_SELECTOR_BASE_ADDRESS + 0x40)
#define	REG_TX_SELECTOR_TX_SEL_STRICT_PRIO                        (TX_SELECTOR_BASE_ADDRESS + 0x44)
#define	REG_TX_SELECTOR_TX_SEL_SELECTION                          (TX_SELECTOR_BASE_ADDRESS + 0x48)
#define	REG_TX_SELECTOR_TX_SEL_SELECTED_BITMAP                    (TX_SELECTOR_BASE_ADDRESS + 0x4C)
#define	REG_TX_SELECTOR_TX_SEL_SELECTED_BITMAP_MGMT               (TX_SELECTOR_BASE_ADDRESS + 0x50)
#define	REG_TX_SELECTOR_TX_SEL_SRC0_SELECTION                     (TX_SELECTOR_BASE_ADDRESS + 0x54)
#define	REG_TX_SELECTOR_TX_SEL_SELECTED_SRC0_BITMAP               (TX_SELECTOR_BASE_ADDRESS + 0x58)
#define	REG_TX_SELECTOR_TX_SEL_SELECTED_SRC0_BITMAP_MGMT          (TX_SELECTOR_BASE_ADDRESS + 0x5C)
#define	REG_TX_SELECTOR_TX_SEL_SRC1_SELECTION                     (TX_SELECTOR_BASE_ADDRESS + 0x60)
#define	REG_TX_SELECTOR_TX_SEL_SELECTED_SRC1_BITMAP               (TX_SELECTOR_BASE_ADDRESS + 0x64)
#define	REG_TX_SELECTOR_TX_SEL_SELECTED_SRC1_BITMAP_MGMT          (TX_SELECTOR_BASE_ADDRESS + 0x68)
#define	REG_TX_SELECTOR_TX_SEL_SRC2_SELECTION                     (TX_SELECTOR_BASE_ADDRESS + 0x6C)
#define	REG_TX_SELECTOR_TX_SEL_SELECTED_SRC2_BITMAP               (TX_SELECTOR_BASE_ADDRESS + 0x70)
#define	REG_TX_SELECTOR_TX_SEL_SELECTED_SRC2_BITMAP_MGMT          (TX_SELECTOR_BASE_ADDRESS + 0x74)
#define	REG_TX_SELECTOR_TX_SEL_SRC3_SELECTION                     (TX_SELECTOR_BASE_ADDRESS + 0x78)
#define	REG_TX_SELECTOR_TX_SEL_SELECTED_SRC3_BITMAP               (TX_SELECTOR_BASE_ADDRESS + 0x7C)
#define	REG_TX_SELECTOR_TX_SEL_SRC4_SELECTION                     (TX_SELECTOR_BASE_ADDRESS + 0x80)
#define	REG_TX_SELECTOR_TX_SEL_SELECTED_SRC4_BITMAP               (TX_SELECTOR_BASE_ADDRESS + 0x84)
#define	REG_TX_SELECTOR_TX_SEL_SRC5_SELECTION                     (TX_SELECTOR_BASE_ADDRESS + 0x88)
#define	REG_TX_SELECTOR_TX_SEL_SELECTED_SRC5_BITMAP               (TX_SELECTOR_BASE_ADDRESS + 0x8C)
#define	REG_TX_SELECTOR_TX_SEL_SRC6_SELECTION                     (TX_SELECTOR_BASE_ADDRESS + 0x90)
#define	REG_TX_SELECTOR_TX_SEL_SELECTED_SRC6_BITMAP               (TX_SELECTOR_BASE_ADDRESS + 0x94)
#define	REG_TX_SELECTOR_TX_SEL_STATUS                             (TX_SELECTOR_BASE_ADDRESS + 0x98)
#define	REG_TX_SELECTOR_TX_SEL_ERR                                (TX_SELECTOR_BASE_ADDRESS + 0x9C)
#define	REG_TX_SELECTOR_TX_SEL_ERR_CLR                            (TX_SELECTOR_BASE_ADDRESS + 0xA0)
#define	REG_TX_SELECTOR_TX_SEL_DL_VAP_AC_MATRIX_0_3               (TX_SELECTOR_BASE_ADDRESS + 0xA4)
#define	REG_TX_SELECTOR_TX_SEL_DL_VAP_AC_MATRIX_4_7               (TX_SELECTOR_BASE_ADDRESS + 0xA8)
#define	REG_TX_SELECTOR_TX_SEL_DL_VAP_AC_MATRIX_8_11              (TX_SELECTOR_BASE_ADDRESS + 0xAC)
#define	REG_TX_SELECTOR_TX_SEL_DL_VAP_AC_MATRIX_12_15             (TX_SELECTOR_BASE_ADDRESS + 0xB0)
#define	REG_TX_SELECTOR_TX_SEL_DL_VAP_AC_MATRIX_16_19             (TX_SELECTOR_BASE_ADDRESS + 0xB4)
#define	REG_TX_SELECTOR_TX_SEL_DL_VAP_AC_MATRIX_20_23             (TX_SELECTOR_BASE_ADDRESS + 0xB8)
#define	REG_TX_SELECTOR_TX_SEL_DL_VAP_AC_MATRIX_24_27             (TX_SELECTOR_BASE_ADDRESS + 0xBC)
#define	REG_TX_SELECTOR_TX_SEL_DL_VAP_AC_MATRIX_28_31             (TX_SELECTOR_BASE_ADDRESS + 0xC0)
#define	REG_TX_SELECTOR_TX_SEL_UL_VAP_AC_MATRIX_0_7               (TX_SELECTOR_BASE_ADDRESS + 0xC4)
#define	REG_TX_SELECTOR_TX_SEL_UL_VAP_AC_MATRIX_8_15              (TX_SELECTOR_BASE_ADDRESS + 0xC8)
#define	REG_TX_SELECTOR_TX_SEL_UL_VAP_AC_MATRIX_16_23             (TX_SELECTOR_BASE_ADDRESS + 0xCC)
#define	REG_TX_SELECTOR_TX_SEL_UL_VAP_AC_MATRIX_24_31             (TX_SELECTOR_BASE_ADDRESS + 0xD0)
#define	REG_TX_SELECTOR_TX_SEL_STA_ASSOCIATE                      (TX_SELECTOR_BASE_ADDRESS + 0xD4)
#define	REG_TX_SELECTOR_TX_SEL_VAP_AC_BYPASS                      (TX_SELECTOR_BASE_ADDRESS + 0xD8)
#define	REG_TX_SELECTOR_TX_SEL_VAP_PS_STATE_EN                    (TX_SELECTOR_BASE_ADDRESS + 0xDC)
#define	REG_TX_SELECTOR_TX_SEL_VAP_PS_STATE                       (TX_SELECTOR_BASE_ADDRESS + 0xE0)
#define	REG_TX_SELECTOR_TX_SEL_VAP_PS_NEW_IND_PS                  (TX_SELECTOR_BASE_ADDRESS + 0xE4)
#define	REG_TX_SELECTOR_TX_SEL_VAP_PS_NEW_IND_ACTIVE              (TX_SELECTOR_BASE_ADDRESS + 0xE8)
#define	REG_TX_SELECTOR_TX_SEL_STA_ACTIVE_SEL_COUNT               (TX_SELECTOR_BASE_ADDRESS + 0xEC)
#define	REG_TX_SELECTOR_TX_SEL_STA_PS_SEL_COUNT                   (TX_SELECTOR_BASE_ADDRESS + 0xF0)
#define	REG_TX_SELECTOR_TX_SEL_VAP_ACTIVE_SEL_COUNT               (TX_SELECTOR_BASE_ADDRESS + 0xF4)
#define	REG_TX_SELECTOR_TX_SEL_VAP_PS_SEL_COUNT                   (TX_SELECTOR_BASE_ADDRESS + 0xF8)
#define	REG_TX_SELECTOR_TX_SEL_GLOBAL_SEL_COUNT                   (TX_SELECTOR_BASE_ADDRESS + 0xFC)
#define	REG_TX_SELECTOR_TX_SEL_GPLP_SEL_COUNT                     (TX_SELECTOR_BASE_ADDRESS + 0x100)
#define	REG_TX_SELECTOR_TX_SEL_GPHP_SEL_COUNT                     (TX_SELECTOR_BASE_ADDRESS + 0x104)
#define	REG_TX_SELECTOR_TX_SEL_BEACON_SEL_COUNT                   (TX_SELECTOR_BASE_ADDRESS + 0x108)
#define	REG_TX_SELECTOR_TX_SEL_EMPTY_SEL_COUNT                    (TX_SELECTOR_BASE_ADDRESS + 0x10C)
#define	REG_TX_SELECTOR_TX_SEL_COUNT_CLR                          (TX_SELECTOR_BASE_ADDRESS + 0x110)
#define	REG_TX_SELECTOR_TX_SEL_LOGGER_CFG                         (TX_SELECTOR_BASE_ADDRESS + 0x114)
#define	REG_TX_SELECTOR_TX_SEL_LOGGER_BUSY                        (TX_SELECTOR_BASE_ADDRESS + 0x118)
#define	REG_TX_SELECTOR_TX_SEL_LOCKER_EN                          (TX_SELECTOR_BASE_ADDRESS + 0x1A0)
#define	REG_TX_SELECTOR_TX_SEL_SW_MASTER0_LOCKER                  (TX_SELECTOR_BASE_ADDRESS + 0x1A4)
#define	REG_TX_SELECTOR_TX_SEL_SW_MASTER1_LOCKER                  (TX_SELECTOR_BASE_ADDRESS + 0x1A8)
#define	REG_TX_SELECTOR_TX_SEL_SW_MASTER2_LOCKER                  (TX_SELECTOR_BASE_ADDRESS + 0x1AC)
#define	REG_TX_SELECTOR_TX_SEL_SW_MASTER3_LOCKER                  (TX_SELECTOR_BASE_ADDRESS + 0x1B0)
#define	REG_TX_SELECTOR_TX_SEL_SW_MASTER0_LOCKER_STATUS           (TX_SELECTOR_BASE_ADDRESS + 0x1B4)
#define	REG_TX_SELECTOR_TX_SEL_SW_MASTER1_LOCKER_STATUS           (TX_SELECTOR_BASE_ADDRESS + 0x1B8)
#define	REG_TX_SELECTOR_TX_SEL_SW_MASTER2_LOCKER_STATUS           (TX_SELECTOR_BASE_ADDRESS + 0x1BC)
#define	REG_TX_SELECTOR_TX_SEL_SW_MASTER3_LOCKER_STATUS           (TX_SELECTOR_BASE_ADDRESS + 0x1C0)
#define	REG_TX_SELECTOR_TX_SEL_LOCKER_INT_CLR                     (TX_SELECTOR_BASE_ADDRESS + 0x1C4)
#define	REG_TX_SELECTOR_TX_SEL_LOCKER_PUSH_WHILE_BUSY             (TX_SELECTOR_BASE_ADDRESS + 0x1C8)
#define	REG_TX_SELECTOR_TX_SEL_LOCKER_PUSH_WHILE_BUSY_CLR         (TX_SELECTOR_BASE_ADDRESS + 0x1CC)
#define	REG_TX_SELECTOR_TX_SELECTOR_SPARE                         (TX_SELECTOR_BASE_ADDRESS + 0x1D0)
#define	REG_TX_SELECTOR_TX_SEL_MU_SELECT_CFG                      (TX_SELECTOR_BASE_ADDRESS + 0x200)
#define	REG_TX_SELECTOR_TX_SEL_MU_BITMAP_UPDATE                   (TX_SELECTOR_BASE_ADDRESS + 0x204)
#define	REG_TX_SELECTOR_TX_SEL_MU_BITMAP_UPDATE_ALL_STA_WDATA     (TX_SELECTOR_BASE_ADDRESS + 0x208)
#define	REG_TX_SELECTOR_TX_SEL_MU_PRIMARY_THRES                   (TX_SELECTOR_BASE_ADDRESS + 0x20C)
#define	REG_TX_SELECTOR_TX_SEL_MU_SECONDARY_THRES                 (TX_SELECTOR_BASE_ADDRESS + 0x210)
#define	REG_TX_SELECTOR_TX_SEL_MU_ERR                             (TX_SELECTOR_BASE_ADDRESS + 0x214)
#define	REG_TX_SELECTOR_TX_SEL_MU_ERR_CLR                         (TX_SELECTOR_BASE_ADDRESS + 0x218)
#define	REG_TX_SELECTOR_TX_SEL_MU_LOCKER_EN                       (TX_SELECTOR_BASE_ADDRESS + 0x220)
#define	REG_TX_SELECTOR_TX_SEL_MU_SW_MASTER0_LOCKER               (TX_SELECTOR_BASE_ADDRESS + 0x224)
#define	REG_TX_SELECTOR_TX_SEL_MU_SW_MASTER1_LOCKER               (TX_SELECTOR_BASE_ADDRESS + 0x228)
#define	REG_TX_SELECTOR_TX_SEL_MU_SW_MASTER2_LOCKER               (TX_SELECTOR_BASE_ADDRESS + 0x22C)
#define	REG_TX_SELECTOR_TX_SEL_MU_SW_MASTER3_LOCKER               (TX_SELECTOR_BASE_ADDRESS + 0x230)
#define	REG_TX_SELECTOR_TX_SEL_MU_SW_MASTER0_LOCKER_STATUS        (TX_SELECTOR_BASE_ADDRESS + 0x234)
#define	REG_TX_SELECTOR_TX_SEL_MU_SW_MASTER1_LOCKER_STATUS        (TX_SELECTOR_BASE_ADDRESS + 0x238)
#define	REG_TX_SELECTOR_TX_SEL_MU_SW_MASTER2_LOCKER_STATUS        (TX_SELECTOR_BASE_ADDRESS + 0x23C)
#define	REG_TX_SELECTOR_TX_SEL_MU_SW_MASTER3_LOCKER_STATUS        (TX_SELECTOR_BASE_ADDRESS + 0x240)
#define	REG_TX_SELECTOR_TX_SEL_MU_LOCKER_INT_CLR                  (TX_SELECTOR_BASE_ADDRESS + 0x244)
#define	REG_TX_SELECTOR_TX_SEL_MU_LOCKER_PUSH_WHILE_BUSY          (TX_SELECTOR_BASE_ADDRESS + 0x248)
#define	REG_TX_SELECTOR_TX_SEL_MU_LOCKER_PUSH_WHILE_BUSY_CLR      (TX_SELECTOR_BASE_ADDRESS + 0x24C)
#define	REG_TX_SELECTOR_TX_SEL_MU_UP0_RESULT                      (TX_SELECTOR_BASE_ADDRESS + 0x250)
#define	REG_TX_SELECTOR_TX_SEL_MU_UP1_RESULT                      (TX_SELECTOR_BASE_ADDRESS + 0x254)
#define	REG_TX_SELECTOR_TX_SEL_MU_UP2_RESULT                      (TX_SELECTOR_BASE_ADDRESS + 0x258)
#define	REG_TX_SELECTOR_TX_SEL_MU_UP3_RESULT                      (TX_SELECTOR_BASE_ADDRESS + 0x25C)
#define	REG_TX_SELECTOR_TX_SEL_MU_UP0_BITMAP                      (TX_SELECTOR_BASE_ADDRESS + 0x260)
#define	REG_TX_SELECTOR_TX_SEL_HE_MU_UP0_BITMAP                   (TX_SELECTOR_BASE_ADDRESS + 0x264)
#define	REG_TX_SELECTOR_TX_SEL_HE_PRE_AGG_UP0_BITMAP              (TX_SELECTOR_BASE_ADDRESS + 0x268)
#define	REG_TX_SELECTOR_TX_SEL_MU_UP1_BITMAP                      (TX_SELECTOR_BASE_ADDRESS + 0x26C)
#define	REG_TX_SELECTOR_TX_SEL_HE_MU_UP1_BITMAP                   (TX_SELECTOR_BASE_ADDRESS + 0x270)
#define	REG_TX_SELECTOR_TX_SEL_HE_PRE_AGG_UP1_BITMAP              (TX_SELECTOR_BASE_ADDRESS + 0x274)
#define	REG_TX_SELECTOR_TX_SEL_MU_UP2_BITMAP                      (TX_SELECTOR_BASE_ADDRESS + 0x278)
#define	REG_TX_SELECTOR_TX_SEL_HE_MU_UP2_BITMAP                   (TX_SELECTOR_BASE_ADDRESS + 0x27C)
#define	REG_TX_SELECTOR_TX_SEL_HE_PRE_AGG_UP2_BITMAP              (TX_SELECTOR_BASE_ADDRESS + 0x280)
#define	REG_TX_SELECTOR_TX_SEL_MU_UP3_BITMAP                      (TX_SELECTOR_BASE_ADDRESS + 0x284)
#define	REG_TX_SELECTOR_TX_SEL_HE_MU_UP3_BITMAP                   (TX_SELECTOR_BASE_ADDRESS + 0x288)
#define	REG_TX_SELECTOR_TX_SEL_HE_PRE_AGG_UP3_BITMAP              (TX_SELECTOR_BASE_ADDRESS + 0x28C)
#define	REG_TX_SELECTOR_TX_SEL_MU_GRP_BITMAP                      (TX_SELECTOR_BASE_ADDRESS + 0x290)
#define	REG_TX_SELECTOR_TX_SEL_HE_GRP_BITMAP                      (TX_SELECTOR_BASE_ADDRESS + 0x294)
#define	REG_TX_SELECTOR_TX_SEL_MU_SM_STATUS                       (TX_SELECTOR_BASE_ADDRESS + 0x298)
#define	REG_TX_SELECTOR_TX_SEL_MU_SU_REPORT_STATUS                (TX_SELECTOR_BASE_ADDRESS + 0x29C)
#define	REG_TX_SELECTOR_TX_SEL_SELECT_MODE_CFG                    (TX_SELECTOR_BASE_ADDRESS + 0x2A0)
#define	REG_TX_SELECTOR_TX_SEL_PLAN_LOCKER_EN                     (TX_SELECTOR_BASE_ADDRESS + 0x2A4)
#define	REG_TX_SELECTOR_TX_SEL_PLAN_SW_MASTER0_LOCKER             (TX_SELECTOR_BASE_ADDRESS + 0x300)
#define	REG_TX_SELECTOR_TX_SEL_PLAN_SW_MASTER1_LOCKER             (TX_SELECTOR_BASE_ADDRESS + 0x304)
#define	REG_TX_SELECTOR_TX_SEL_PLAN_SW_MASTER2_LOCKER             (TX_SELECTOR_BASE_ADDRESS + 0x308)
#define	REG_TX_SELECTOR_TX_SEL_PLAN_SW_MASTER3_LOCKER             (TX_SELECTOR_BASE_ADDRESS + 0x30C)
#define	REG_TX_SELECTOR_TX_SEL_PLAN_SW_MASTER0_LOCKER_STATUS      (TX_SELECTOR_BASE_ADDRESS + 0x310)
#define	REG_TX_SELECTOR_TX_SEL_PLAN_SW_MASTER1_LOCKER_STATUS      (TX_SELECTOR_BASE_ADDRESS + 0x314)
#define	REG_TX_SELECTOR_TX_SEL_PLAN_SW_MASTER2_LOCKER_STATUS      (TX_SELECTOR_BASE_ADDRESS + 0x318)
#define	REG_TX_SELECTOR_TX_SEL_PLAN_SW_MASTER3_LOCKER_STATUS      (TX_SELECTOR_BASE_ADDRESS + 0x31C)
#define	REG_TX_SELECTOR_TX_SEL_PLAN_LOCKER_INT_CLR                (TX_SELECTOR_BASE_ADDRESS + 0x320)
#define	REG_TX_SELECTOR_TX_SEL_PLAN_LOCKER_PUSH_WHILE_BUSY        (TX_SELECTOR_BASE_ADDRESS + 0x324)
#define	REG_TX_SELECTOR_TX_SEL_PLAN_LOCKER_PUSH_WHILE_BUSY_CLR    (TX_SELECTOR_BASE_ADDRESS + 0x328)
#define	REG_TX_SELECTOR_TX_SEL_PRE_AGG_SEL_REQ_BITS0              (TX_SELECTOR_BASE_ADDRESS + 0x380)
#define	REG_TX_SELECTOR_TX_SEL_PRE_AGG_SEL_REQ_BITS1              (TX_SELECTOR_BASE_ADDRESS + 0x384)
#define	REG_TX_SELECTOR_TX_SEL_PRE_AGG_SEL_RESULT_BITS0           (TX_SELECTOR_BASE_ADDRESS + 0x388)
#define	REG_TX_SELECTOR_TX_SEL_PRE_AGG_SEL_RESULT_BITS1           (TX_SELECTOR_BASE_ADDRESS + 0x38C)
#define	REG_TX_SELECTOR_TX_SEL_PRE_AGG_SEL_RESULT_BITS2           (TX_SELECTOR_BASE_ADDRESS + 0x390)
#define	REG_TX_SELECTOR_TX_SEL_CHICKEN_BITS                       (TX_SELECTOR_BASE_ADDRESS + 0x394)
#define	REG_TX_SELECTOR_TX_SEL_MLD_VAP_PS                         (TX_SELECTOR_BASE_ADDRESS + 0x398)
/*---------------------------------------------------------------------------------
/						Data Type Definition										
/----------------------------------------------------------------------------------*/
/*REG_TX_SELECTOR_TX_SEL_INSTR 0x0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 instr : 2; //CPU instruction to TX Selector, reset value: 0x0, access type: RW
		uint32 reserved0 : 30;
	} bitFields;
} RegTxSelectorTxSelInstr_u;

/*REG_TX_SELECTOR_TX_SEL_BITMAP_UPDATE_MASTER0 0x4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 master0StaSel : 8; //Station number or VAP number according to bitmap, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
		uint32 master0BitmapSel : 3; //Bitmap Queue selection: , 0x0: STA-TID , 0x2: VAP-MC , 0x4: GLOBAL , 0x5: GPLP , 0x6: GPHP , 0x7: BEACON, reset value: 0x0, access type: RW
		uint32 reserved1 : 1;
		uint32 master0BitTypeSel : 5; //Information bit type selector: , 0x0:    Data in Q - 1 bit Per TID bit type , 0x1:    Lock - 1 bit Per TID bit type , 0x2:    Request - 1 bit Per TID bit type , 0x3:    Power Save type - 1 bit Per TID bit type , 0x4:    Active/Power Save mode - 1 bit Per TID bit type , 0x5:    DL HE MU En - 1 bit Per TID bit type , 0x6:    UL data in Q - 1 bit Per TID bit type , 0x8:    Disable - 1 bit Per STA bit type , 0x9:    BF sequence request enable/disable - 1 bit Per STA bit type , 0xA:    BF sequence type - 1 bit Per STA bit type , 0xB:   TWT Announced type - 1 bit Per STA bit type , 0xC:   TWT SP - 1 bit Per STA bit type , 0xD:   TWT Availability - 1 bit Per STA bit type , 0xE:   DL Plan index - 5 bits Per STA bit type , 0xF:   UL Plan index - 5 bits Per STA bit type , 0x10: Plan lock - 1 bit Per STA bit type , 0x11: Insert A-control - 1 bit Per STA bit type , 0x12: MU MIMO Group lock - 1 bit Per MU MIMO Group bit type , 0x13: MU MIMO TX Mode BF req (MSB bit only) - 1 bit Per Group bit type , 0x14: OMI_VALID - 1 bit Per STA bit type , 0x15: OMI_SUPPORTED - 1 bit Per STA bit type , 0x16: OMI_DATA_BITS - 11 OMI bits: (Note: Shadow, all values will actually written on OMI_DATA_BITS2)    ,                     TOMI UL HE MU Dis - 1 bit Per STA bit type ,                     TOMI DL HE MU Dis - 1 bit Per STA bit type ,                     ROMI Max Nss - 3 bits Per STA bit type ,                     OMI BW - 3 bits Per STA bit type ,                     TOMI Max Nss - 3 bits Per STA bit type , 0x17: UL HE MU En - 1 bit Per STA bit type , 0x18: TWT_COMBINED_ANOUNCED_PS_REQ , 0x19: TWT_COMBINED_ANOUNCED_PS_REQ_IGNORE_UL_DATA , 0x1A: PLAN_IGNORE_UL_DATA , 0x1B: OMI_DATA_BITS2 - 1 bit (DISABLE HE SU ER)  (Note: All OMI_DATA_BITS actually updating bitmap with this access) , 0x1F: DUMMY_CMD ,  ,  ,  , , reset value: 0x0, access type: RW
		uint32 reserved2 : 7;
	} bitFields;
} RegTxSelectorTxSelBitmapUpdateMaster0_u;

/*REG_TX_SELECTOR_TX_SEL_BITMAP_UPDATE_DATA_MASTER0 0x8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 master0TidWriteEn : 9; //Write enable vector - Valid for TID resolution updates only., reset value: 0x0, access type: RW
		uint32 master0TidWriteValue : 9; //Data per TID vector - Valid for TID resolution updates only., reset value: 0x0, access type: RW
		uint32 master0StaWriteData : 11; //STA data field - Valid for STA resolution updates only., reset value: 0x0, access type: RW
		uint32 reserved0 : 3;
	} bitFields;
} RegTxSelectorTxSelBitmapUpdateDataMaster0_u;

/*REG_TX_SELECTOR_TX_SEL_BITMAP_UPDATE_MASTER1 0xC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 master1StaSel : 8; //Station number or VAP number according to bitmap, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
		uint32 master1BitmapSel : 3; //Bitmap Queue selection: , 0x0: STA-TID , 0x2: VAP-MC , 0x4: GLOBAL , 0x5: GPLP , 0x6: GPHP , 0x7: BEACON, reset value: 0x0, access type: RW
		uint32 reserved1 : 1;
		uint32 master1BitTypeSel : 5; //Information bit type selector: , 0x0: Data in Q - 1 bit Per TID bit type , 0x1: Lock - 1 bit Per TID bit type , 0x2: Request - 1 bit Per TID bit type , 0x3: Power Save type - 1 bit Per TID bit type , 0x4: Active/Power Save mode - 1 bit Per TID bit type , 0x5: DL HE MU En - 1 bit Per TID bit type , 0x6: UL data in Q - 1 bit Per TID bit type , 0x8: Disable - 1 bit Per STA bit type , 0x9: BF sequence request enable/disable - 1 bit Per STA bit type , 0xA: BF sequence type - 1 bit Per STA bit type , 0xB: TWT Announced type - 1 bit Per STA bit type , 0xC: TWT SP - 1 bit Per STA bit type , 0xD: TWT Availability - 1 bit Per STA bit type , 0xE: DL Plan index - 5 bits Per STA bit type , 0xF: UL Plan index - 5 bits Per STA bit type , 0x10: Plan lock - 1 bit Per STA bit type , 0x11: Insert A-control - 1 bit Per STA bit type , 0x12: MU MIMO Group lock - 1 bit Per MU MIMO Group bit type , 0x13: MU MIMO TX Mode BF req (MSB bit only) - 1 bit Per Group bit type , 0x14: OMI_VALID - 1 bit Per STA bit type , 0x15: OMI_SUPPORTED - 1 bit Per STA bit type , 0x16: OMI_DATA_BITS - 11 OMI bits: (Note: Shadow, all values will actually written on OMI_DATA_BITS2) , TOMI UL HE MU Dis - 1 bit Per STA bit type , TOMI DL HE MU Dis - 1 bit Per STA bit type , ROMI Max Nss - 3 bits Per STA bit type , OMI BW - 3 bits Per STA bit type , TOMI Max Nss - 3 bits Per STA bit type , 0x17: UL HE MU En - 1 bit Per STA bit type , 0x18: TWT_COMBINED_ANOUNCED_PS_REQ , 0x19: TWT_COMBINED_ANOUNCED_PS_REQ_IGNORE_UL_DATA , 0x1A: PLAN_IGNORE_UL_DATA , 0x1B: OMI_DATA_BITS2 - 1 bit (DISABLE HE SU ER) (Note: All OMI_DATA_BITS actually updating bitmap with this access) , 0x1F: DUMMY_CMD , , reset value: 0x0, access type: RW
		uint32 reserved2 : 7;
	} bitFields;
} RegTxSelectorTxSelBitmapUpdateMaster1_u;

/*REG_TX_SELECTOR_TX_SEL_BITMAP_UPDATE_DATA_MASTER1 0x10 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 master1TidWriteEn : 9; //Write enable vector - Valid for TID resolution updates only., reset value: 0x0, access type: RW
		uint32 master1TidWriteValue : 9; //Data per TID vector - Valid for TID resolution updates only., reset value: 0x0, access type: RW
		uint32 master1StaWriteData : 11; //STA data field - Valid for STA resolution updates only., reset value: 0x0, access type: RW
		uint32 reserved0 : 3;
	} bitFields;
} RegTxSelectorTxSelBitmapUpdateDataMaster1_u;

/*REG_TX_SELECTOR_TX_SEL_BITMAP_UPDATE_MASTER2 0x14 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 master2StaSel : 8; //Station number or VAP number according to bitmap, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
		uint32 master2BitmapSel : 3; //Bitmap Queue selection: , 0x0: STA-TID , 0x2: VAP-MC , 0x4: GLOBAL , 0x5: GPLP , 0x6: GPHP , 0x7: BEACON, reset value: 0x0, access type: RW
		uint32 reserved1 : 1;
		uint32 master2BitTypeSel : 5; //Information bit type selector: , 0x0: Data in Q - 1 bit Per TID bit type , 0x1: Lock - 1 bit Per TID bit type , 0x2: Request - 1 bit Per TID bit type , 0x3: Power Save type - 1 bit Per TID bit type , 0x4: Active/Power Save mode - 1 bit Per TID bit type , 0x5: DL HE MU En - 1 bit Per TID bit type , 0x6: UL data in Q - 1 bit Per TID bit type , 0x8: Disable - 1 bit Per STA bit type , 0x9: BF sequence request enable/disable - 1 bit Per STA bit type , 0xA: BF sequence type - 1 bit Per STA bit type , 0xB: TWT Announced type - 1 bit Per STA bit type , 0xC: TWT SP - 1 bit Per STA bit type , 0xD: TWT Availability - 1 bit Per STA bit type , 0xE: DL Plan index - 5 bits Per STA bit type , 0xF: UL Plan index - 5 bits Per STA bit type , 0x10: Plan lock - 1 bit Per STA bit type , 0x11: Insert A-control - 1 bit Per STA bit type , 0x12: MU MIMO Group lock - 1 bit Per MU MIMO Group bit type , 0x13: MU MIMO TX Mode BF req (MSB bit only) - 1 bit Per Group bit type , 0x14: OMI_VALID - 1 bit Per STA bit type , 0x15: OMI_SUPPORTED - 1 bit Per STA bit type , 0x16: OMI_DATA_BITS - 11 OMI bits: (Note: Shadow, all values will actually written on OMI_DATA_BITS2) ,      TOMI UL HE MU Dis - 1 bit Per STA bit type ,      TOMI DL HE MU Dis - 1 bit Per STA bit type ,      ROMI Max Nss - 3 bits Per STA bit type ,      OMI BW - 3 bits Per STA bit type ,      TOMI Max Nss - 3 bits Per STA bit type , 0x17: UL HE MU En - 1 bit Per STA bit type , 0x18: TWT_COMBINED_ANOUNCED_PS_REQ , 0x19: TWT_COMBINED_ANOUNCED_PS_REQ_IGNORE_UL_DATA , 0x1A: PLAN_IGNORE_UL_DATA , 0x1B: OMI_DATA_BITS2 - 1 bit (DISABLE HE SU ER) (Note: All OMI_DATA_BITS actually updating bitmap with this access) , 0x1F: DUMMY_CMD , , reset value: 0x0, access type: RW
		uint32 reserved2 : 7;
	} bitFields;
} RegTxSelectorTxSelBitmapUpdateMaster2_u;

/*REG_TX_SELECTOR_TX_SEL_BITMAP_UPDATE_DATA_MASTER2 0x18 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 master2TidWriteEn : 9; //Write enable vector - Valid for TID resolution updates only., reset value: 0x0, access type: RW
		uint32 master2TidWriteValue : 9; //Data per TID vector - Valid for TID resolution updates only., reset value: 0x0, access type: RW
		uint32 master2StaWriteData : 11; //STA data field - Valid for STA resolution updates only., reset value: 0x0, access type: RW
		uint32 reserved0 : 3;
	} bitFields;
} RegTxSelectorTxSelBitmapUpdateDataMaster2_u;

/*REG_TX_SELECTOR_TX_SEL_BITMAP_UPDATE_MASTER3 0x1C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 master3StaSel : 8; //Station number or VAP number according to bitmap, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
		uint32 master3BitmapSel : 3; //Bitmap Queue selection: , 0x0: STA-TID , 0x2: VAP-MC , 0x4: GLOBAL , 0x5: GPLP , 0x6: GPHP , 0x7: BEACON, reset value: 0x0, access type: RW
		uint32 reserved1 : 1;
		uint32 master3BitTypeSel : 5; //Information bit type selector: , 0x0: Data in Q - 1 bit Per TID bit type , 0x1: Lock - 1 bit Per TID bit type , 0x2: Request - 1 bit Per TID bit type , 0x3: Power Save type - 1 bit Per TID bit type , 0x4: Active/Power Save mode - 1 bit Per TID bit type , 0x5: DL HE MU En - 1 bit Per TID bit type , 0x6: UL data in Q - 1 bit Per TID bit type , 0x8: Disable - 1 bit Per STA bit type , 0x9: BF sequence request enable/disable - 1 bit Per STA bit type , 0xA: BF sequence type - 1 bit Per STA bit type , 0xB: TWT Announced type - 1 bit Per STA bit type , 0xC: TWT SP - 1 bit Per STA bit type , 0xD: TWT Availability - 1 bit Per STA bit type , 0xE: DL Plan index - 5 bits Per STA bit type , 0xF: UL Plan index - 5 bits Per STA bit type , 0x10: Plan lock - 1 bit Per STA bit type , 0x11: Insert A-control - 1 bit Per STA bit type , 0x12: MU MIMO Group lock - 1 bit Per MU MIMO Group bit type , 0x13: MU MIMO TX Mode BF req (MSB bit only) - 1 bit Per Group bit type , 0x14: OMI_VALID - 1 bit Per STA bit type , 0x15: OMI_SUPPORTED - 1 bit Per STA bit type , 0x16: OMI_DATA_BITS - 11 OMI bits: (Note: Shadow, all values will actually written on OMI_DATA_BITS2) ,      TOMI UL HE MU Dis - 1 bit Per STA bit type ,      TOMI DL HE MU Dis - 1 bit Per STA bit type ,      ROMI Max Nss - 3 bits Per STA bit type ,      OMI BW - 3 bits Per STA bit type ,      TOMI Max Nss - 3 bits Per STA bit type , 0x17: UL HE MU En - 1 bit Per STA bit type , 0x18: TWT_COMBINED_ANOUNCED_PS_REQ , 0x19: TWT_COMBINED_ANOUNCED_PS_REQ_IGNORE_UL_DATA , 0x1A: PLAN_IGNORE_UL_DATA , 0x1B: OMI_DATA_BITS2 - 1 bit (DISABLE HE SU ER) (Note: All OMI_DATA_BITS actually updating bitmap with this access) , 0x1F: DUMMY_CMD , , reset value: 0x0, access type: RW
		uint32 reserved2 : 7;
	} bitFields;
} RegTxSelectorTxSelBitmapUpdateMaster3_u;

/*REG_TX_SELECTOR_TX_SEL_BITMAP_UPDATE_DATA_MASTER3 0x20 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 master3TidWriteEn : 9; //Write enable vector - Valid for TID resolution updates only., reset value: 0x0, access type: RW
		uint32 master3TidWriteValue : 9; //Data per TID vector - Valid for TID resolution updates only., reset value: 0x0, access type: RW
		uint32 master3StaWriteData : 11; //STA data field - Valid for STA resolution updates only., reset value: 0x0, access type: RW
		uint32 reserved0 : 3;
	} bitFields;
} RegTxSelectorTxSelBitmapUpdateDataMaster3_u;

/*REG_TX_SELECTOR_TX_SEL_STA_PS_WRR_LOW0 0x24 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 psLowVap0Prio : 3; //VAP0 priority, reset value: 0x0, access type: RW
		uint32 psLowVap1Prio : 3; //VAP1 priority, reset value: 0x0, access type: RW
		uint32 psLowVap2Prio : 3; //VAP2 priority, reset value: 0x0, access type: RW
		uint32 psLowVap3Prio : 3; //VAP3 priority, reset value: 0x0, access type: RW
		uint32 psLowVap4Prio : 3; //VAP4 priority, reset value: 0x0, access type: RW
		uint32 psLowVap5Prio : 3; //VAP5 priority, reset value: 0x0, access type: RW
		uint32 psLowVap6Prio : 3; //VAP6 priority, reset value: 0x0, access type: RW
		uint32 psLowVap7Prio : 3; //VAP7 priority, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegTxSelectorTxSelStaPsWrrLow0_u;

/*REG_TX_SELECTOR_TX_SEL_STA_PS_WRR_LOW1 0x28 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 psLowVap8Prio : 3; //VAP8 priority, reset value: 0x0, access type: RW
		uint32 psLowVap9Prio : 3; //VAP9 priority, reset value: 0x0, access type: RW
		uint32 psLowVap10Prio : 3; //VAP10 priority, reset value: 0x0, access type: RW
		uint32 psLowVap11Prio : 3; //VAP11 priority, reset value: 0x0, access type: RW
		uint32 psLowVap12Prio : 3; //VAP12 priority, reset value: 0x0, access type: RW
		uint32 psLowVap13Prio : 3; //VAP13 priority, reset value: 0x0, access type: RW
		uint32 psLowVap14Prio : 3; //VAP14 priority, reset value: 0x0, access type: RW
		uint32 psLowVap15Prio : 3; //VAP15 priority, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegTxSelectorTxSelStaPsWrrLow1_u;

/*REG_TX_SELECTOR_TX_SEL_STA_PS_WRR_HIGH0 0x2C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 psHighVap16Prio : 3; //VAP16 priority, reset value: 0x0, access type: RW
		uint32 psHighVap17Prio : 3; //VAP17 priority, reset value: 0x0, access type: RW
		uint32 psHighVap18Prio : 3; //VAP18 priority, reset value: 0x0, access type: RW
		uint32 psHighVap19Prio : 3; //VAP19 priority, reset value: 0x0, access type: RW
		uint32 psHighVap20Prio : 3; //VAP20 priority, reset value: 0x0, access type: RW
		uint32 psHighVap21Prio : 3; //VAP21 priority, reset value: 0x0, access type: RW
		uint32 psHighVap22Prio : 3; //VAP22 priority, reset value: 0x0, access type: RW
		uint32 psHighVap23Prio : 3; //VAP23 priority, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegTxSelectorTxSelStaPsWrrHigh0_u;

/*REG_TX_SELECTOR_TX_SEL_STA_PS_WRR_HIGH1 0x30 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 psHighVap24Prio : 3; //VAP24 priority, reset value: 0x0, access type: RW
		uint32 psHighVap25Prio : 3; //VAP25 priority, reset value: 0x0, access type: RW
		uint32 psHighVap26Prio : 3; //VAP26 priority, reset value: 0x0, access type: RW
		uint32 psHighVap27Prio : 3; //VAP27 priority, reset value: 0x0, access type: RW
		uint32 psHighVap28Prio : 3; //VAP28 priority, reset value: 0x0, access type: RW
		uint32 psHighVap29Prio : 3; //VAP29 priority, reset value: 0x0, access type: RW
		uint32 psHighVap30Prio : 3; //VAP30 priority, reset value: 0x0, access type: RW
		uint32 psHighVap31Prio : 3; //VAP31 priority, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegTxSelectorTxSelStaPsWrrHigh1_u;

/*REG_TX_SELECTOR_TX_SEL_STA_ACTIVE_WRR_LOW0 0x34 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 activeLowVap0Prio : 3; //VAP0 priority, reset value: 0x0, access type: RW
		uint32 activeLowVap1Prio : 3; //VAP1 priority, reset value: 0x0, access type: RW
		uint32 activeLowVap2Prio : 3; //VAP2 priority, reset value: 0x0, access type: RW
		uint32 activeLowVap3Prio : 3; //VAP3 priority, reset value: 0x0, access type: RW
		uint32 activeLowVap4Prio : 3; //VAP4 priority, reset value: 0x0, access type: RW
		uint32 activeLowVap5Prio : 3; //VAP5 priority, reset value: 0x0, access type: RW
		uint32 activeLowVap6Prio : 3; //VAP6 priority, reset value: 0x0, access type: RW
		uint32 activeLowVap7Prio : 3; //VAP7 priority, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegTxSelectorTxSelStaActiveWrrLow0_u;

/*REG_TX_SELECTOR_TX_SEL_STA_ACTIVE_WRR_LOW1 0x38 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 activeLowVap8Prio : 3; //VAP8 priority, reset value: 0x0, access type: RW
		uint32 activeLowVap9Prio : 3; //VAP9 priority, reset value: 0x0, access type: RW
		uint32 activeLowVap10Prio : 3; //VAP10 priority, reset value: 0x0, access type: RW
		uint32 activeLowVap11Prio : 3; //VAP11 priority, reset value: 0x0, access type: RW
		uint32 activeLowVap12Prio : 3; //VAP12 priority, reset value: 0x0, access type: RW
		uint32 activeLowVap13Prio : 3; //VAP13 priority, reset value: 0x0, access type: RW
		uint32 activeLowVap14Prio : 3; //VAP14 priority, reset value: 0x0, access type: RW
		uint32 activeLowVap15Prio : 3; //VAP15 priority, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegTxSelectorTxSelStaActiveWrrLow1_u;

/*REG_TX_SELECTOR_TX_SEL_STA_ACTIVE_WRR_HIGH0 0x3C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 activeHighVap16Prio : 3; //VAP16 priority, reset value: 0x0, access type: RW
		uint32 activeHighVap17Prio : 3; //VAP17 priority, reset value: 0x0, access type: RW
		uint32 activeHighVap18Prio : 3; //VAP18 priority, reset value: 0x0, access type: RW
		uint32 activeHighVap19Prio : 3; //VAP19 priority, reset value: 0x0, access type: RW
		uint32 activeHighVap20Prio : 3; //VAP20 priority, reset value: 0x0, access type: RW
		uint32 activeHighVap21Prio : 3; //VAP21 priority, reset value: 0x0, access type: RW
		uint32 activeHighVap22Prio : 3; //VAP22 priority, reset value: 0x0, access type: RW
		uint32 activeHighVap23Prio : 3; //VAP23 priority, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegTxSelectorTxSelStaActiveWrrHigh0_u;

/*REG_TX_SELECTOR_TX_SEL_STA_ACTIVE_WRR_HIGH1 0x40 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 activeHighVap24Prio : 3; //VAP24 priority, reset value: 0x0, access type: RW
		uint32 activeHighVap25Prio : 3; //VAP25 priority, reset value: 0x0, access type: RW
		uint32 activeHighVap26Prio : 3; //VAP26 priority, reset value: 0x0, access type: RW
		uint32 activeHighVap27Prio : 3; //VAP27 priority, reset value: 0x0, access type: RW
		uint32 activeHighVap28Prio : 3; //VAP28 priority, reset value: 0x0, access type: RW
		uint32 activeHighVap29Prio : 3; //VAP29 priority, reset value: 0x0, access type: RW
		uint32 activeHighVap30Prio : 3; //VAP30 priority, reset value: 0x0, access type: RW
		uint32 activeHighVap31Prio : 3; //VAP31 priority, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegTxSelectorTxSelStaActiveWrrHigh1_u;

/*REG_TX_SELECTOR_TX_SEL_STRICT_PRIO 0x44 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 strictPrioGroupSelEn : 6; //Enable selection groups, reset value: 0x3F, access type: RW
		uint32 strictPrioWrrPrio : 2; //Priority of active selection, reset value: 0x3, access type: RW
		uint32 strictPrioVapMpsPrio : 2; //Priority of VAP TID PS selection, reset value: 0x2, access type: RW
		uint32 strictPrioGlobalPrio : 2; //Priority of Global selection, reset value: 0x1, access type: RW
		uint32 strictPrioSelectorLockEn : 1; //Selector lock enable, reset value: 0x1, access type: RW
		uint32 reserved0 : 19;
	} bitFields;
} RegTxSelectorTxSelStrictPrio_u;

/*REG_TX_SELECTOR_TX_SEL_SELECTION 0x48 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 selectionTidNum : 4; //TID number, reset value: 0x0, access type: RO
		uint32 selectionStaNum : 8; //STA number, reset value: 0x0, access type: RO
		uint32 selectionVapNum : 5; //VAP number, reset value: 0x0, access type: RO
		uint32 reserved0 : 1;
		uint32 selectionAcNum : 2; //AC number, reset value: 0x0, access type: RO
		uint32 selectionTxqId : 3; //Selected Queue index, reset value: 0x0, access type: RO
		uint32 reserved1 : 1;
		uint32 selectionSrcNum : 3; //Source from which the selected result came from, reset value: 0x0, access type: RO
		uint32 reserved2 : 1;
		uint32 selectionBf : 2; //Beam Forming bits, reset value: 0x0, access type: RO
		uint32 selectionEmpty : 1; //No selection exist bit, reset value: 0x0, access type: RO
		uint32 selectionValid : 1; //Selection valid bit, reset value: 0x0, access type: RO
	} bitFields;
} RegTxSelectorTxSelSelection_u;

/*REG_TX_SELECTOR_TX_SEL_SELECTED_BITMAP 0x4C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 selectedBitmapReq0 : 1; //Request bit, reset value: 0x0, access type: RO
		uint32 selectedBitmapPsType0 : 1; //PS type, reset value: 0x0, access type: RO
		uint32 selectedBitmapPsMode0 : 1; //PS mode, reset value: 0x0, access type: RO
		uint32 selectedBitmapDataInQ0 : 1; //Data in Q, reset value: 0x0, access type: RO
		uint32 selectedBitmapReq1 : 1; //Request bit, reset value: 0x0, access type: RO
		uint32 selectedBitmapPsType1 : 1; //PS type, reset value: 0x0, access type: RO
		uint32 selectedBitmapPsMode1 : 1; //PS mode, reset value: 0x0, access type: RO
		uint32 selectedBitmapDataInQ1 : 1; //Data in Q, reset value: 0x0, access type: RO
		uint32 selectedBitmapReq2 : 1; //Request bit, reset value: 0x0, access type: RO
		uint32 selectedBitmapPsType2 : 1; //PS type, reset value: 0x0, access type: RO
		uint32 selectedBitmapPsMode2 : 1; //PS mode, reset value: 0x0, access type: RO
		uint32 selectedBitmapDataInQ2 : 1; //Data in Q, reset value: 0x0, access type: RO
		uint32 selectedBitmapReq3 : 1; //Request bit, reset value: 0x0, access type: RO
		uint32 selectedBitmapPsType3 : 1; //PS type, reset value: 0x0, access type: RO
		uint32 selectedBitmapPsMode3 : 1; //PS mode, reset value: 0x0, access type: RO
		uint32 selectedBitmapDataInQ3 : 1; //Data in Q, reset value: 0x0, access type: RO
		uint32 selectedBitmapReq4 : 1; //Request bit, reset value: 0x0, access type: RO
		uint32 selectedBitmapPsType4 : 1; //PS type, reset value: 0x0, access type: RO
		uint32 selectedBitmapPsMode4 : 1; //PS mode, reset value: 0x0, access type: RO
		uint32 selectedBitmapDataInQ4 : 1; //Data in Q, reset value: 0x0, access type: RO
		uint32 selectedBitmapReq5 : 1; //Request bit, reset value: 0x0, access type: RO
		uint32 selectedBitmapPsType5 : 1; //PS type, reset value: 0x0, access type: RO
		uint32 selectedBitmapPsMode5 : 1; //PS mode, reset value: 0x0, access type: RO
		uint32 selectedBitmapDataInQ5 : 1; //Data in Q, reset value: 0x0, access type: RO
		uint32 selectedBitmapReq6 : 1; //Request bit, reset value: 0x0, access type: RO
		uint32 selectedBitmapPsType6 : 1; //PS type, reset value: 0x0, access type: RO
		uint32 selectedBitmapPsMode6 : 1; //PS mode, reset value: 0x0, access type: RO
		uint32 selectedBitmapDataInQ6 : 1; //Data in Q, reset value: 0x0, access type: RO
		uint32 selectedBitmapReq7 : 1; //Request bit, reset value: 0x0, access type: RO
		uint32 selectedBitmapPsType7 : 1; //PS type, reset value: 0x0, access type: RO
		uint32 selectedBitmapPsMode7 : 1; //PS mode, reset value: 0x0, access type: RO
		uint32 selectedBitmapDataInQ7 : 1; //Data in Q, reset value: 0x0, access type: RO
	} bitFields;
} RegTxSelectorTxSelSelectedBitmap_u;

/*REG_TX_SELECTOR_TX_SEL_SELECTED_BITMAP_MGMT 0x50 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 selectedBitmapReq8 : 1; //Request bit, reset value: 0x0, access type: RO
		uint32 selectedBitmapPsType8 : 1; //PS type, reset value: 0x0, access type: RO
		uint32 selectedBitmapPsMode8 : 1; //PS mode, reset value: 0x0, access type: RO
		uint32 selectedBitmapDataInQ8 : 1; //Data in Q, reset value: 0x0, access type: RO
		uint32 reserved0 : 28;
	} bitFields;
} RegTxSelectorTxSelSelectedBitmapMgmt_u;

/*REG_TX_SELECTOR_TX_SEL_SRC0_SELECTION 0x54 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 src0SelectionTidNum : 4; //TID number, reset value: 0x0, access type: RO
		uint32 src0SelectionStaNum : 8; //STA number, reset value: 0x0, access type: RO
		uint32 src0SelectionVapNum : 5; //VAP number, reset value: 0x0, access type: RO
		uint32 src0SelectionTxqId : 3; //Selected Queue index, reset value: 0x0, access type: RO
		uint32 src0SelectionAcNum : 2; //AC number, reset value: 0x0, access type: RO
		uint32 reserved0 : 2;
		uint32 src0SelectionBf : 2; //Beam Forming bits, reset value: 0x0, access type: RO
		uint32 src0SelectionSrcNum : 3; //Source from which the selected result came from, reset value: 0x0, access type: RO
		uint32 reserved1 : 1;
		uint32 src0SelectionEmpty : 1; //No selection exist bit, reset value: 0x0, access type: RO
		uint32 src0SelectionValid : 1; //Selection valid bit, reset value: 0x0, access type: RO
	} bitFields;
} RegTxSelectorTxSelSrc0Selection_u;

/*REG_TX_SELECTOR_TX_SEL_SELECTED_SRC0_BITMAP 0x58 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 3;
		uint32 selectedSrc0BitmapDataInQ0 : 1; //Data in Q, reset value: 0x0, access type: RO
		uint32 reserved1 : 3;
		uint32 selectedSrc0BitmapDataInQ1 : 1; //Data in Q, reset value: 0x0, access type: RO
		uint32 reserved2 : 3;
		uint32 selectedSrc0BitmapDataInQ2 : 1; //Data in Q, reset value: 0x0, access type: RO
		uint32 reserved3 : 3;
		uint32 selectedSrc0BitmapDataInQ3 : 1; //Data in Q, reset value: 0x0, access type: RO
		uint32 reserved4 : 3;
		uint32 selectedSrc0BitmapDataInQ4 : 1; //Data in Q, reset value: 0x0, access type: RO
		uint32 reserved5 : 3;
		uint32 selectedSrc0BitmapDataInQ5 : 1; //Data in Q, reset value: 0x0, access type: RO
		uint32 reserved6 : 3;
		uint32 selectedSrc0BitmapDataInQ6 : 1; //Data in Q, reset value: 0x0, access type: RO
		uint32 reserved7 : 3;
		uint32 selectedSrc0BitmapDataInQ7 : 1; //Data in Q, reset value: 0x0, access type: RO
	} bitFields;
} RegTxSelectorTxSelSelectedSrc0Bitmap_u;

/*REG_TX_SELECTOR_TX_SEL_SELECTED_SRC0_BITMAP_MGMT 0x5C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 3;
		uint32 selectedSrc0BitmapDataInQ8 : 1; //Data in Q, reset value: 0x0, access type: RO
		uint32 reserved1 : 28;
	} bitFields;
} RegTxSelectorTxSelSelectedSrc0BitmapMgmt_u;

/*REG_TX_SELECTOR_TX_SEL_SRC1_SELECTION 0x60 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 src1SelectionTidNum : 4; //TID number, reset value: 0x0, access type: RO
		uint32 src1SelectionStaNum : 8; //STA number, reset value: 0x0, access type: RO
		uint32 src1SelectionVapNum : 5; //VAP number, reset value: 0x0, access type: RO
		uint32 src1SelectionTxqId : 3; //Selected Queue index, reset value: 0x0, access type: RO
		uint32 src1SelectionAcNum : 2; //AC number, reset value: 0x0, access type: RO
		uint32 reserved0 : 2;
		uint32 src1SelectionBf : 2; //Beam Forming bits, reset value: 0x0, access type: RO
		uint32 src1SelectionSrcNum : 3; //Source from which the selected result came from, reset value: 0x1, access type: RO
		uint32 reserved1 : 1;
		uint32 src1SelectionEmpty : 1; //No selection exist bit, reset value: 0x0, access type: RO
		uint32 src1SelectionValid : 1; //Selection valid bit, reset value: 0x0, access type: RO
	} bitFields;
} RegTxSelectorTxSelSrc1Selection_u;

/*REG_TX_SELECTOR_TX_SEL_SELECTED_SRC1_BITMAP 0x64 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 selectedSrc1BitmapReq0 : 1; //Request bit, reset value: 0x0, access type: RO
		uint32 selectedSrc1BitmapPsType0 : 1; //PS type, reset value: 0x0, access type: RO
		uint32 selectedSrc1BitmapPsMode0 : 1; //PS mode, reset value: 0x1, access type: RO
		uint32 selectedSrc1BitmapDataInQ0 : 1; //Data in Q, reset value: 0x0, access type: RO
		uint32 selectedSrc1BitmapReq1 : 1; //Request bit, reset value: 0x0, access type: RO
		uint32 selectedSrc1BitmapPsType1 : 1; //PS type, reset value: 0x0, access type: RO
		uint32 selectedSrc1BitmapPsMode1 : 1; //PS mode, reset value: 0x1, access type: RO
		uint32 selectedSrc1BitmapDataInQ1 : 1; //Data in Q, reset value: 0x0, access type: RO
		uint32 selectedSrc1BitmapReq2 : 1; //Request bit, reset value: 0x0, access type: RO
		uint32 selectedSrc1BitmapPsType2 : 1; //PS type, reset value: 0x0, access type: RO
		uint32 selectedSrc1BitmapPsMode2 : 1; //PS mode, reset value: 0x1, access type: RO
		uint32 selectedSrc1BitmapDataInQ2 : 1; //Data in Q, reset value: 0x0, access type: RO
		uint32 selectedSrc1BitmapReq3 : 1; //Request bit, reset value: 0x0, access type: RO
		uint32 selectedSrc1BitmapPsType3 : 1; //PS type, reset value: 0x0, access type: RO
		uint32 selectedSrc1BitmapPsMode3 : 1; //PS mode, reset value: 0x1, access type: RO
		uint32 selectedSrc1BitmapDataInQ3 : 1; //Data in Q, reset value: 0x0, access type: RO
		uint32 selectedSrc1BitmapReq4 : 1; //Request bit, reset value: 0x0, access type: RO
		uint32 selectedSrc1BitmapPsType4 : 1; //PS type, reset value: 0x0, access type: RO
		uint32 selectedSrc1BitmapPsMode4 : 1; //PS mode, reset value: 0x1, access type: RO
		uint32 selectedSrc1BitmapDataInQ4 : 1; //Data in Q, reset value: 0x0, access type: RO
		uint32 selectedSrc1BitmapReq5 : 1; //Request bit, reset value: 0x0, access type: RO
		uint32 selectedSrc1BitmapPsType5 : 1; //PS type, reset value: 0x0, access type: RO
		uint32 selectedSrc1BitmapPsMode5 : 1; //PS mode, reset value: 0x1, access type: RO
		uint32 selectedSrc1BitmapDataInQ5 : 1; //Data in Q, reset value: 0x0, access type: RO
		uint32 selectedSrc1BitmapReq6 : 1; //Request bit, reset value: 0x0, access type: RO
		uint32 selectedSrc1BitmapPsType6 : 1; //PS type, reset value: 0x0, access type: RO
		uint32 selectedSrc1BitmapPsMode6 : 1; //PS mode, reset value: 0x1, access type: RO
		uint32 selectedSrc1BitmapDataInQ6 : 1; //Data in Q, reset value: 0x0, access type: RO
		uint32 selectedSrc1BitmapReq7 : 1; //Request bit, reset value: 0x0, access type: RO
		uint32 selectedSrc1BitmapPsType7 : 1; //PS type, reset value: 0x0, access type: RO
		uint32 selectedSrc1BitmapPsMode7 : 1; //PS mode, reset value: 0x1, access type: RO
		uint32 selectedSrc1BitmapDataInQ7 : 1; //Data in Q, reset value: 0x0, access type: RO
	} bitFields;
} RegTxSelectorTxSelSelectedSrc1Bitmap_u;

/*REG_TX_SELECTOR_TX_SEL_SELECTED_SRC1_BITMAP_MGMT 0x68 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 selectedSrc1BitmapReq8 : 1; //Request bit, reset value: 0x0, access type: RO
		uint32 selectedSrc1BitmapPsType8 : 1; //PS type, reset value: 0x0, access type: RO
		uint32 selectedSrc1BitmapPsMode8 : 1; //PS mode, reset value: 0x1, access type: RO
		uint32 selectedSrc1BitmapDataInQ8 : 1; //Data in Q, reset value: 0x0, access type: RO
		uint32 reserved0 : 28;
	} bitFields;
} RegTxSelectorTxSelSelectedSrc1BitmapMgmt_u;

/*REG_TX_SELECTOR_TX_SEL_SRC2_SELECTION 0x6C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 src2SelectionTidNum : 4; //TID number, reset value: 0x0, access type: RO
		uint32 src2SelectionStaNum : 8; //STA number, reset value: 0x0, access type: RO
		uint32 src2SelectionVapNum : 5; //VAP number, reset value: 0x0, access type: RO
		uint32 src2SelectionTxqId : 3; //Selected Queue index, reset value: 0x2, access type: RO
		uint32 src2SelectionAcNum : 2; //AC number, reset value: 0x0, access type: RO
		uint32 reserved0 : 2;
		uint32 src2SelectionBf : 2; //Beam Forming bits, reset value: 0x0, access type: RO
		uint32 src2SelectionSrcNum : 3; //Source from which the selected result came from, reset value: 0x3, access type: RO
		uint32 reserved1 : 1;
		uint32 src2SelectionEmpty : 1; //No selection exist bit, reset value: 0x0, access type: RO
		uint32 src2SelectionValid : 1; //Selection valid bit, reset value: 0x0, access type: RO
	} bitFields;
} RegTxSelectorTxSelSrc2Selection_u;

/*REG_TX_SELECTOR_TX_SEL_SELECTED_SRC2_BITMAP 0x70 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 selectedSrc2BitmapReq0 : 1; //Request bit, reset value: 0x0, access type: RO
		uint32 selectedSrc2BitmapPsType0 : 1; //PS type, reset value: 0x0, access type: RO
		uint32 selectedSrc2BitmapPsMode0 : 1; //PS mode, reset value: 0x1, access type: RO
		uint32 selectedSrc2BitmapDataInQ0 : 1; //Data in Q, reset value: 0x0, access type: RO
		uint32 selectedSrc2BitmapReq1 : 1; //Request bit, reset value: 0x0, access type: RO
		uint32 selectedSrc2BitmapPsType1 : 1; //PS type, reset value: 0x0, access type: RO
		uint32 selectedSrc2BitmapPsMode1 : 1; //PS mode, reset value: 0x1, access type: RO
		uint32 selectedSrc2BitmapDataInQ1 : 1; //Data in Q, reset value: 0x0, access type: RO
		uint32 selectedSrc2BitmapReq2 : 1; //Request bit, reset value: 0x0, access type: RO
		uint32 selectedSrc2BitmapPsType2 : 1; //PS type, reset value: 0x0, access type: RO
		uint32 selectedSrc2BitmapPsMode2 : 1; //PS mode, reset value: 0x1, access type: RO
		uint32 selectedSrc2BitmapDataInQ2 : 1; //Data in Q, reset value: 0x0, access type: RO
		uint32 selectedSrc2BitmapReq3 : 1; //Request bit, reset value: 0x0, access type: RO
		uint32 selectedSrc2BitmapPsType3 : 1; //PS type, reset value: 0x0, access type: RO
		uint32 selectedSrc2BitmapPsMode3 : 1; //PS mode, reset value: 0x1, access type: RO
		uint32 selectedSrc2BitmapDataInQ3 : 1; //Data in Q, reset value: 0x0, access type: RO
		uint32 selectedSrc2BitmapReq4 : 1; //Request bit, reset value: 0x0, access type: RO
		uint32 selectedSrc2BitmapPsType4 : 1; //PS type, reset value: 0x0, access type: RO
		uint32 selectedSrc2BitmapPsMode4 : 1; //PS mode, reset value: 0x1, access type: RO
		uint32 selectedSrc2BitmapDataInQ4 : 1; //Data in Q, reset value: 0x0, access type: RO
		uint32 selectedSrc2BitmapReq5 : 1; //Request bit, reset value: 0x0, access type: RO
		uint32 selectedSrc2BitmapPsType5 : 1; //PS type, reset value: 0x0, access type: RO
		uint32 selectedSrc2BitmapPsMode5 : 1; //PS mode, reset value: 0x1, access type: RO
		uint32 selectedSrc2BitmapDataInQ5 : 1; //Data in Q, reset value: 0x0, access type: RO
		uint32 selectedSrc2BitmapReq6 : 1; //Request bit, reset value: 0x0, access type: RO
		uint32 selectedSrc2BitmapPsType6 : 1; //PS type, reset value: 0x0, access type: RO
		uint32 selectedSrc2BitmapPsMode6 : 1; //PS mode, reset value: 0x1, access type: RO
		uint32 selectedSrc2BitmapDataInQ6 : 1; //Data in Q, reset value: 0x0, access type: RO
		uint32 selectedSrc2BitmapReq7 : 1; //Request bit, reset value: 0x0, access type: RO
		uint32 selectedSrc2BitmapPsType7 : 1; //PS type, reset value: 0x0, access type: RO
		uint32 selectedSrc2BitmapPsMode7 : 1; //PS mode, reset value: 0x1, access type: RO
		uint32 selectedSrc2BitmapDataInQ7 : 1; //Data in Q, reset value: 0x0, access type: RO
	} bitFields;
} RegTxSelectorTxSelSelectedSrc2Bitmap_u;

/*REG_TX_SELECTOR_TX_SEL_SELECTED_SRC2_BITMAP_MGMT 0x74 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 selectedSrc2BitmapReq8 : 1; //Request bit, reset value: 0x0, access type: RO
		uint32 selectedSrc2BitmapPsType8 : 1; //PS type, reset value: 0x0, access type: RO
		uint32 selectedSrc2BitmapPsMode8 : 1; //PS mode, reset value: 0x1, access type: RO
		uint32 selectedSrc2BitmapDataInQ8 : 1; //Data in Q, reset value: 0x0, access type: RO
		uint32 reserved0 : 28;
	} bitFields;
} RegTxSelectorTxSelSelectedSrc2BitmapMgmt_u;

/*REG_TX_SELECTOR_TX_SEL_SRC3_SELECTION 0x78 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 src3SelectionTidNum : 4; //TID number, reset value: 0x0, access type: RO
		uint32 src3SelectionStaNum : 8; //STA number, reset value: 0x0, access type: RO
		uint32 src3SelectionVapNum : 5; //VAP number, reset value: 0x0, access type: RO
		uint32 src3SelectionTxqId : 3; //Selected Queue index, reset value: 0x4, access type: RO
		uint32 src3SelectionAcNum : 2; //AC number, reset value: 0x0, access type: RO
		uint32 reserved0 : 2;
		uint32 src3SelectionBf : 2; //Beam Forming bits, reset value: 0x0, access type: RO
		uint32 src3SelectionSrcNum : 3; //Source from which the selected result came from, reset value: 0x4, access type: RO
		uint32 reserved1 : 1;
		uint32 src3SelectionEmpty : 1; //No selection exist bit, reset value: 0x0, access type: RO
		uint32 src3SelectionValid : 1; //Selection valid bit, reset value: 0x0, access type: RO
	} bitFields;
} RegTxSelectorTxSelSrc3Selection_u;

/*REG_TX_SELECTOR_TX_SEL_SELECTED_SRC3_BITMAP 0x7C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 selectedSrc3BitmapReq0 : 1; //Request bit, reset value: 0x0, access type: RO
		uint32 selectedSrc3BitmapPsType0 : 1; //PS type, reset value: 0x0, access type: RO
		uint32 selectedSrc3BitmapPsMode0 : 1; //PS mode, reset value: 0x0, access type: RO
		uint32 selectedSrc3BitmapDataInQ0 : 1; //Data in Q, reset value: 0x0, access type: RO
		uint32 selectedSrc3BitmapReq1 : 1; //Request bit, reset value: 0x0, access type: RO
		uint32 selectedSrc3BitmapPsType1 : 1; //PS type, reset value: 0x0, access type: RO
		uint32 selectedSrc3BitmapPsMode1 : 1; //PS mode, reset value: 0x0, access type: RO
		uint32 selectedSrc3BitmapDataInQ1 : 1; //Data in Q, reset value: 0x0, access type: RO
		uint32 selectedSrc3BitmapReq2 : 1; //Request bit, reset value: 0x0, access type: RO
		uint32 selectedSrc3BitmapPsType2 : 1; //PS type, reset value: 0x0, access type: RO
		uint32 selectedSrc3BitmapPsMode2 : 1; //PS mode, reset value: 0x0, access type: RO
		uint32 selectedSrc3BitmapDataInQ2 : 1; //Data in Q, reset value: 0x0, access type: RO
		uint32 selectedSrc3BitmapReq3 : 1; //Request bit, reset value: 0x0, access type: RO
		uint32 selectedSrc3BitmapPsType3 : 1; //PS type, reset value: 0x0, access type: RO
		uint32 selectedSrc3BitmapPsMode3 : 1; //PS mode, reset value: 0x0, access type: RO
		uint32 selectedSrc3BitmapDataInQ3 : 1; //Data in Q, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegTxSelectorTxSelSelectedSrc3Bitmap_u;

/*REG_TX_SELECTOR_TX_SEL_SRC4_SELECTION 0x80 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 src4SelectionTidNum : 4; //TID number, reset value: 0x0, access type: RO
		uint32 src4SelectionStaNum : 8; //STA number, reset value: 0x0, access type: RO
		uint32 src4SelectionVapNum : 5; //VAP number, reset value: 0x0, access type: RO
		uint32 src4SelectionTxqId : 3; //Selected Queue index, reset value: 0x5, access type: RO
		uint32 src4SelectionAcNum : 2; //AC number, reset value: 0x0, access type: RO
		uint32 reserved0 : 2;
		uint32 src4SelectionBf : 2; //Beam Forming bits, reset value: 0x0, access type: RO
		uint32 src4SelectionSrcNum : 3; //Source from which the selected result came from, reset value: 0x5, access type: RO
		uint32 reserved1 : 1;
		uint32 src4SelectionEmpty : 1; //No selection exist bit, reset value: 0x0, access type: RO
		uint32 src4SelectionValid : 1; //Selection valid bit, reset value: 0x0, access type: RO
	} bitFields;
} RegTxSelectorTxSelSrc4Selection_u;

/*REG_TX_SELECTOR_TX_SEL_SELECTED_SRC4_BITMAP 0x84 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 3;
		uint32 selectedSrc4BitmapDataInQ : 1; //Data in Q, reset value: 0x0, access type: RO
		uint32 reserved1 : 28;
	} bitFields;
} RegTxSelectorTxSelSelectedSrc4Bitmap_u;

/*REG_TX_SELECTOR_TX_SEL_SRC5_SELECTION 0x88 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 src5SelectionTidNum : 4; //TID number, reset value: 0x0, access type: RO
		uint32 src5SelectionStaNum : 8; //STA number, reset value: 0x0, access type: RO
		uint32 src5SelectionVapNum : 5; //VAP number, reset value: 0x0, access type: RO
		uint32 src5SelectionTxqId : 3; //Selected Queue index, reset value: 0x6, access type: RO
		uint32 src5SelectionAcNum : 2; //AC number, reset value: 0x0, access type: RO
		uint32 reserved0 : 2;
		uint32 src5SelectionBf : 2; //Beam Forming bits, reset value: 0x0, access type: RO
		uint32 src5SelectionSrcNum : 3; //Source from which the selected result came from, reset value: 0x6, access type: RO
		uint32 reserved1 : 1;
		uint32 src5SelectionEmpty : 1; //No selection exist bit, reset value: 0x0, access type: RO
		uint32 src5SelectionValid : 1; //Selection valid bit, reset value: 0x0, access type: RO
	} bitFields;
} RegTxSelectorTxSelSrc5Selection_u;

/*REG_TX_SELECTOR_TX_SEL_SELECTED_SRC5_BITMAP 0x8C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 3;
		uint32 selectedSrc5BitmapDataInQ : 1; //Data in Q, reset value: 0x0, access type: RO
		uint32 reserved1 : 28;
	} bitFields;
} RegTxSelectorTxSelSelectedSrc5Bitmap_u;

/*REG_TX_SELECTOR_TX_SEL_SRC6_SELECTION 0x90 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 src6SelectionTidNum : 4; //TID number, reset value: 0x0, access type: RO
		uint32 src6SelectionStaNum : 8; //STA number, reset value: 0x0, access type: RO
		uint32 src6SelectionVapNum : 5; //VAP number, reset value: 0x0, access type: RO
		uint32 src6SelectionTxqId : 3; //Selected Queue index, reset value: 0x7, access type: RO
		uint32 src6SelectionAcNum : 2; //AC number, reset value: 0x0, access type: RO
		uint32 reserved0 : 2;
		uint32 src6SelectionBf : 2; //Beam Forming bits, reset value: 0x0, access type: RO
		uint32 src6SelectionSrcNum : 3; //Source from which the selected result came from, reset value: 0x7, access type: RO
		uint32 reserved1 : 1;
		uint32 src6SelectionEmpty : 1; //No selection exist bit, reset value: 0x0, access type: RO
		uint32 src6SelectionValid : 1; //Selection valid bit, reset value: 0x0, access type: RO
	} bitFields;
} RegTxSelectorTxSelSrc6Selection_u;

/*REG_TX_SELECTOR_TX_SEL_SELECTED_SRC6_BITMAP 0x94 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 selectedSrc6BitmapReq0 : 1; //Request bit, reset value: 0x0, access type: RO
		uint32 reserved0 : 2;
		uint32 selectedSrc6BitmapDataInQ0 : 1; //Data in Q, reset value: 0x0, access type: RO
		uint32 reserved1 : 28;
	} bitFields;
} RegTxSelectorTxSelSelectedSrc6Bitmap_u;

/*REG_TX_SELECTOR_TX_SEL_STATUS 0x98 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 statusSwFifoPopWhileEmpty : 1; //SW FIFO underflow , reset value: 0x0, access type: RO
		uint32 statusSwFifoPushWhileFull : 1; //SW FIFO overflow , reset value: 0x0, access type: RO
		uint32 statusSwFifoEmpty : 1; //SW FIFO empty , reset value: 0x1, access type: RO
		uint32 statusSwFifoFull : 1; //SW FIFO full , reset value: 0x0, access type: RO
		uint32 statusSwFifoHfull : 1; //SW FIFO half full , reset value: 0x0, access type: RO
		uint32 statusDataInQFifoEmpty : 1; //Data in Q FIFO empty , reset value: 0x1, access type: RO
		uint32 statusDataInQFifoFull : 1; //Data in Q FIFO full , reset value: 0x0, access type: RO
		uint32 dataInQFifoWasFull : 1; //no description, reset value: 0x0, access type: RO
		uint32 statusInstruction : 2; //Last instruction recieved, reset value: 0x0, access type: RO
		uint32 statusInstrSrc : 1; //Source of instruction, reset value: 0x0, access type: RO
		uint32 reserved0 : 21;
	} bitFields;
} RegTxSelectorTxSelStatus_u;

/*REG_TX_SELECTOR_TX_SEL_ERR 0x9C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 errSwInstrWhileBusy : 1; //New SW instruction to while selection is being done , reset value: 0x0, access type: RO
		uint32 errHwInstrWhileBusy : 1; //New HW instruction to while selection is being done , reset value: 0x0, access type: RO
		uint32 errDataLost : 1; //Data insertion overflow/underflow, reset value: 0x0, access type: RO
		uint32 reserved0 : 17;
		uint32 errDataInQLost : 1; //no description, reset value: 0x0, access type: RO
		uint32 reserved1 : 3;
		uint32 wrongBitmapAccessErr : 1; //no description, reset value: 0x0, access type: RO
		uint32 wrongBitmapAccessErrBitmap : 3; //no description, reset value: 0x0, access type: RO
		uint32 timWrongVapIrq : 1; //TIM IE wrong VAP error IRQ. the status per VAP and the clear located at TIM IE register file, reset value: 0x0, access type: RO
		uint32 reserved2 : 3;
	} bitFields;
} RegTxSelectorTxSelErr_u;

/*REG_TX_SELECTOR_TX_SEL_ERR_CLR 0xA0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 errClrSwInstrWhileBusyClr : 1; //SW instruction error clear , reset value: 0x0, access type: WO
		uint32 errClrHwInstrWhileBusyClr : 1; //HW instruction error clear , reset value: 0x0, access type: WO
		uint32 errClrDataLostClr : 1; //Data insertion overflow/underflow error clear, reset value: 0x0, access type: WO
		uint32 reserved0 : 17;
		uint32 errClrDataInQLostClr : 1; //no description, reset value: 0x0, access type: WO
		uint32 reserved1 : 3;
		uint32 wrongBitmapAccessErrClr : 1; //no description, reset value: 0x0, access type: WO
		uint32 reserved2 : 7;
	} bitFields;
} RegTxSelectorTxSelErrClr_u;

/*REG_TX_SELECTOR_TX_SEL_DL_VAP_AC_MATRIX_0_3 0xA4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dlVap0Ac : 7; //VAP0-AC vector , reset value: 0x0, access type: RO
		uint32 reserved0 : 1;
		uint32 dlVap1Ac : 7; //VAP1-AC vector , reset value: 0x0, access type: RO
		uint32 reserved1 : 1;
		uint32 dlVap2Ac : 7; //VAP2-AC vector , reset value: 0x0, access type: RO
		uint32 reserved2 : 1;
		uint32 dlVap3Ac : 7; //VAP3-AC vector , reset value: 0x0, access type: RO
		uint32 reserved3 : 1;
	} bitFields;
} RegTxSelectorTxSelDlVapAcMatrix03_u;

/*REG_TX_SELECTOR_TX_SEL_DL_VAP_AC_MATRIX_4_7 0xA8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dlVap4Ac : 7; //VAP4-AC vector , reset value: 0x0, access type: RO
		uint32 reserved0 : 1;
		uint32 dlVap5Ac : 7; //VAP5-AC vector , reset value: 0x0, access type: RO
		uint32 reserved1 : 1;
		uint32 dlVap6Ac : 7; //VAP6-AC vector , reset value: 0x0, access type: RO
		uint32 reserved2 : 1;
		uint32 dlVap7Ac : 7; //VAP7-AC vector , reset value: 0x0, access type: RO
		uint32 reserved3 : 1;
	} bitFields;
} RegTxSelectorTxSelDlVapAcMatrix47_u;

/*REG_TX_SELECTOR_TX_SEL_DL_VAP_AC_MATRIX_8_11 0xAC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dlVap8Ac : 7; //VAP8-AC vector , reset value: 0x0, access type: RO
		uint32 reserved0 : 1;
		uint32 dlVap9Ac : 7; //VAP9-AC vector , reset value: 0x0, access type: RO
		uint32 reserved1 : 1;
		uint32 dlVap10Ac : 7; //VAP10-AC vector , reset value: 0x0, access type: RO
		uint32 reserved2 : 1;
		uint32 dlVap11Ac : 7; //VAP11-AC vector , reset value: 0x0, access type: RO
		uint32 reserved3 : 1;
	} bitFields;
} RegTxSelectorTxSelDlVapAcMatrix811_u;

/*REG_TX_SELECTOR_TX_SEL_DL_VAP_AC_MATRIX_12_15 0xB0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dlVap12Ac : 7; //VAP12-AC vector , reset value: 0x0, access type: RO
		uint32 reserved0 : 1;
		uint32 dlVap13Ac : 7; //VAP13-AC vector , reset value: 0x0, access type: RO
		uint32 reserved1 : 1;
		uint32 dlVap14Ac : 7; //VAP14-AC vector , reset value: 0x0, access type: RO
		uint32 reserved2 : 1;
		uint32 dlVap15Ac : 7; //VAP15-AC vector , reset value: 0x0, access type: RO
		uint32 reserved3 : 1;
	} bitFields;
} RegTxSelectorTxSelDlVapAcMatrix1215_u;

/*REG_TX_SELECTOR_TX_SEL_DL_VAP_AC_MATRIX_16_19 0xB4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dlVap16Ac : 7; //VAP16-AC vector , reset value: 0x0, access type: RO
		uint32 reserved0 : 1;
		uint32 dlVap17Ac : 7; //VAP17-AC vector , reset value: 0x0, access type: RO
		uint32 reserved1 : 1;
		uint32 dlVap18Ac : 7; //VAP18-AC vector , reset value: 0x0, access type: RO
		uint32 reserved2 : 1;
		uint32 dlVap19Ac : 7; //VAP19-AC vector , reset value: 0x0, access type: RO
		uint32 reserved3 : 1;
	} bitFields;
} RegTxSelectorTxSelDlVapAcMatrix1619_u;

/*REG_TX_SELECTOR_TX_SEL_DL_VAP_AC_MATRIX_20_23 0xB8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dlVap20Ac : 7; //VAP20-AC vector , reset value: 0x0, access type: RO
		uint32 reserved0 : 1;
		uint32 dlVap21Ac : 7; //VAP21-AC vector , reset value: 0x0, access type: RO
		uint32 reserved1 : 1;
		uint32 dlVap22Ac : 7; //VAP22-AC vector , reset value: 0x0, access type: RO
		uint32 reserved2 : 1;
		uint32 dlVap23Ac : 7; //VAP23-AC vector , reset value: 0x0, access type: RO
		uint32 reserved3 : 1;
	} bitFields;
} RegTxSelectorTxSelDlVapAcMatrix2023_u;

/*REG_TX_SELECTOR_TX_SEL_DL_VAP_AC_MATRIX_24_27 0xBC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dlVap24Ac : 7; //VAP24-AC vector , reset value: 0x0, access type: RO
		uint32 reserved0 : 1;
		uint32 dlVap25Ac : 7; //VAP25-AC vector , reset value: 0x0, access type: RO
		uint32 reserved1 : 1;
		uint32 dlVap26Ac : 7; //VAP26-AC vector , reset value: 0x0, access type: RO
		uint32 reserved2 : 1;
		uint32 dlVap27Ac : 7; //VAP27-AC vector , reset value: 0x0, access type: RO
		uint32 reserved3 : 1;
	} bitFields;
} RegTxSelectorTxSelDlVapAcMatrix2427_u;

/*REG_TX_SELECTOR_TX_SEL_DL_VAP_AC_MATRIX_28_31 0xC0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dlVap28Ac : 7; //VAP28-AC vector , reset value: 0x0, access type: RO
		uint32 reserved0 : 1;
		uint32 dlVap29Ac : 7; //VAP29-AC vector , reset value: 0x0, access type: RO
		uint32 reserved1 : 1;
		uint32 dlVap30Ac : 7; //VAP30-AC vector , reset value: 0x0, access type: RO
		uint32 reserved2 : 1;
		uint32 dlVap31Ac : 7; //VAP31-AC vector , reset value: 0x0, access type: RO
		uint32 reserved3 : 1;
	} bitFields;
} RegTxSelectorTxSelDlVapAcMatrix2831_u;

/*REG_TX_SELECTOR_TX_SEL_UL_VAP_AC_MATRIX_0_7 0xC4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ulVap0Ac : 4; //VAP0-AC vector , reset value: 0x0, access type: RO
		uint32 ulVap1Ac : 4; //VAP1-AC vector , reset value: 0x0, access type: RO
		uint32 ulVap2Ac : 4; //VAP2-AC vector , reset value: 0x0, access type: RO
		uint32 ulVap3Ac : 4; //VAP3-AC vector , reset value: 0x0, access type: RO
		uint32 ulVap4Ac : 4; //VAP4-AC vector , reset value: 0x0, access type: RO
		uint32 ulVap5Ac : 4; //VAP5-AC vector , reset value: 0x0, access type: RO
		uint32 ulVap6Ac : 4; //VAP6-AC vector , reset value: 0x0, access type: RO
		uint32 ulVap7Ac : 4; //VAP7-AC vector , reset value: 0x0, access type: RO
	} bitFields;
} RegTxSelectorTxSelUlVapAcMatrix07_u;

/*REG_TX_SELECTOR_TX_SEL_UL_VAP_AC_MATRIX_8_15 0xC8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ulVap8Ac : 4; //VAP8-AC vector , reset value: 0x0, access type: RO
		uint32 ulVap9Ac : 4; //VAP9-AC vector , reset value: 0x0, access type: RO
		uint32 ulVap10Ac : 4; //VAP10-AC vector , reset value: 0x0, access type: RO
		uint32 ulVap11Ac : 4; //VAP11-AC vector , reset value: 0x0, access type: RO
		uint32 ulVap12Ac : 4; //VAP12-AC vector , reset value: 0x0, access type: RO
		uint32 ulVap13Ac : 4; //VAP13-AC vector , reset value: 0x0, access type: RO
		uint32 ulVap14Ac : 4; //VAP14-AC vector , reset value: 0x0, access type: RO
		uint32 ulVap15Ac : 4; //VAP15-AC vector , reset value: 0x0, access type: RO
	} bitFields;
} RegTxSelectorTxSelUlVapAcMatrix815_u;

/*REG_TX_SELECTOR_TX_SEL_UL_VAP_AC_MATRIX_16_23 0xCC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ulVap16Ac : 4; //VAP16-AC vector , reset value: 0x0, access type: RO
		uint32 ulVap17Ac : 4; //VAP17-AC vector , reset value: 0x0, access type: RO
		uint32 ulVap18Ac : 4; //VAP18-AC vector , reset value: 0x0, access type: RO
		uint32 ulVap19Ac : 4; //VAP19-AC vector , reset value: 0x0, access type: RO
		uint32 ulVap20Ac : 4; //VAP20-AC vector , reset value: 0x0, access type: RO
		uint32 ulVap21Ac : 4; //VAP21-AC vector , reset value: 0x0, access type: RO
		uint32 ulVap22Ac : 4; //VAP22-AC vector , reset value: 0x0, access type: RO
		uint32 ulVap23Ac : 4; //VAP23-AC vector , reset value: 0x0, access type: RO
	} bitFields;
} RegTxSelectorTxSelUlVapAcMatrix1623_u;

/*REG_TX_SELECTOR_TX_SEL_UL_VAP_AC_MATRIX_24_31 0xD0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ulVap24Ac : 4; //VAP24-AC vector , reset value: 0x0, access type: RO
		uint32 ulVap25Ac : 4; //VAP25-AC vector , reset value: 0x0, access type: RO
		uint32 ulVap26Ac : 4; //VAP26-AC vector , reset value: 0x0, access type: RO
		uint32 ulVap27Ac : 4; //VAP27-AC vector , reset value: 0x0, access type: RO
		uint32 ulVap28Ac : 4; //VAP28-AC vector , reset value: 0x0, access type: RO
		uint32 ulVap29Ac : 4; //VAP29-AC vector , reset value: 0x0, access type: RO
		uint32 ulVap30Ac : 4; //VAP30-AC vector , reset value: 0x0, access type: RO
		uint32 ulVap31Ac : 4; //VAP31-AC vector , reset value: 0x0, access type: RO
	} bitFields;
} RegTxSelectorTxSelUlVapAcMatrix2431_u;

/*REG_TX_SELECTOR_TX_SEL_STA_ASSOCIATE 0xD4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 staAssociateStation : 8; //Station to be associated with the selected VAP, reset value: 0x0, access type: RW
		uint32 staAssociateVap : 5; //Selected VAP, reset value: 0x0, access type: RW
		uint32 reserved0 : 3;
		uint32 staAssociateVapStaBitValue : 1; //Selected VAP, reset value: 0x0, access type: RW
		uint32 reserved1 : 7;
		uint32 vapZero : 1; //When asserted, disconnect all stations of all VAPs, reset value: 0x0, access type: RW
		uint32 reserved2 : 7;
	} bitFields;
} RegTxSelectorTxSelStaAssociate_u;

/*REG_TX_SELECTOR_TX_SEL_VAP_AC_BYPASS 0xD8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vapAcBypassAcSel : 3; //TX Handler AC sel bypass, reset value: 0x0, access type: RW
		uint32 reserved0 : 5;
		uint32 vapAcBypassVapSel : 5; //TX Handler VAP sel bypass, reset value: 0x0, access type: RW
		uint32 reserved1 : 3;
		uint32 vapAcBypassVaild : 1; //TX Handler bypass valid, reset value: 0x0, access type: RW
		uint32 reserved2 : 15;
	} bitFields;
} RegTxSelectorTxSelVapAcBypass_u;

/*REG_TX_SELECTOR_TX_SEL_VAP_PS_STATE_EN 0xDC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 16;
		uint32 vapPsStateVapInPsAutoEn : 1; //Enables VAP in PS bit automatic update by TX Selector, reset value: 0x1, access type: RW
		uint32 reserved1 : 15;
	} bitFields;
} RegTxSelectorTxSelVapPsStateEn_u;

/*REG_TX_SELECTOR_TX_SEL_VAP_PS_STATE 0xE0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vapPsStateVapPsState : 32; //Each bit indicates its VAP PS state, reset value: 0x0, access type: RO
	} bitFields;
} RegTxSelectorTxSelVapPsState_u;

/*REG_TX_SELECTOR_TX_SEL_VAP_PS_NEW_IND_PS 0xE4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vapPsNewIndPs : 32; //Each bit indicates its VAP PS state, reset value: 0x0, access type: RO
	} bitFields;
} RegTxSelectorTxSelVapPsNewIndPs_u;

/*REG_TX_SELECTOR_TX_SEL_VAP_PS_NEW_IND_ACTIVE 0xE8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vapPsNewIndActive : 32; //Each bit indicates its VAP PS state, reset value: 0x0, access type: RO
	} bitFields;
} RegTxSelectorTxSelVapPsNewIndActive_u;

/*REG_TX_SELECTOR_TX_SEL_STA_ACTIVE_SEL_COUNT 0xEC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 staActiveCount : 12; //Counts Selection of STA ACTIVE , reset value: 0x0, access type: RO
		uint32 reserved0 : 20;
	} bitFields;
} RegTxSelectorTxSelStaActiveSelCount_u;

/*REG_TX_SELECTOR_TX_SEL_STA_PS_SEL_COUNT 0xF0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 staPsCount : 12; //Counts Selection of sta ps , reset value: 0x0, access type: RO
		uint32 reserved0 : 20;
	} bitFields;
} RegTxSelectorTxSelStaPsSelCount_u;

/*REG_TX_SELECTOR_TX_SEL_VAP_ACTIVE_SEL_COUNT 0xF4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vapActiveCount : 12; //Counts Selection of vap ACTIVE , reset value: 0x0, access type: RO
		uint32 reserved0 : 20;
	} bitFields;
} RegTxSelectorTxSelVapActiveSelCount_u;

/*REG_TX_SELECTOR_TX_SEL_VAP_PS_SEL_COUNT 0xF8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vapPsCount : 12; //Counts Selection of vap ps , reset value: 0x0, access type: RO
		uint32 reserved0 : 20;
	} bitFields;
} RegTxSelectorTxSelVapPsSelCount_u;

/*REG_TX_SELECTOR_TX_SEL_GLOBAL_SEL_COUNT 0xFC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 globalCount : 12; //Counts Selection of global , reset value: 0x0, access type: RO
		uint32 reserved0 : 20;
	} bitFields;
} RegTxSelectorTxSelGlobalSelCount_u;

/*REG_TX_SELECTOR_TX_SEL_GPLP_SEL_COUNT 0x100 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gplpCount : 12; //Counts Selection of gplp , reset value: 0x0, access type: RO
		uint32 reserved0 : 20;
	} bitFields;
} RegTxSelectorTxSelGplpSelCount_u;

/*REG_TX_SELECTOR_TX_SEL_GPHP_SEL_COUNT 0x104 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 gphpCount : 12; //Counts Selection of gphp , reset value: 0x0, access type: RO
		uint32 reserved0 : 20;
	} bitFields;
} RegTxSelectorTxSelGphpSelCount_u;

/*REG_TX_SELECTOR_TX_SEL_BEACON_SEL_COUNT 0x108 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 beaconCount : 12; //Counts Selection of beacon , reset value: 0x0, access type: RO
		uint32 reserved0 : 20;
	} bitFields;
} RegTxSelectorTxSelBeaconSelCount_u;

/*REG_TX_SELECTOR_TX_SEL_EMPTY_SEL_COUNT 0x10C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 emptyCount : 12; //Counts Empty Selections, reset value: 0x0, access type: RO
		uint32 reserved0 : 20;
	} bitFields;
} RegTxSelectorTxSelEmptySelCount_u;

/*REG_TX_SELECTOR_TX_SEL_COUNT_CLR 0x110 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 countersClr : 1; //Clears selection counters, reset value: 0x0, access type: WO
		uint32 reserved0 : 31;
	} bitFields;
} RegTxSelectorTxSelCountClr_u;

/*REG_TX_SELECTOR_TX_SEL_LOGGER_CFG 0x114 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSelLoggerSelectionLockerPriority : 2; //no description, reset value: 0x0, access type: RW
		uint32 txSelLoggerSelectionLockerEn : 1; //no description, reset value: 0x0, access type: RW
		uint32 txSelLoggerSelectionLockerSel : 1; //0x0: Selection logger , 0x1: Locker logger, reset value: 0x0, access type: RW
		uint32 txSelLoggerBitmapPriority : 2; //no description, reset value: 0x0, access type: RW
		uint32 txSelLoggerBitmapUpdateEn : 1; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 txSelLoggerMuLockerPriority : 2; //no description, reset value: 0x0, access type: RW
		uint32 txSelLoggerMuLockerEn : 1; //no description, reset value: 0x0, access type: RW
		uint32 reserved1 : 1;
		uint32 txSelLoggerMuBitmapPriority : 2; //no description, reset value: 0x0, access type: RW
		uint32 txSelLoggerMuBitmapUpdateEn : 1; //no description, reset value: 0x0, access type: RW
		uint32 reserved2 : 1;
		uint32 txSelLoggerMuSelectionEn : 1; //no description, reset value: 0x0, access type: RW
		uint32 txSelLoggerBeaconSelectionEn : 1; //Logger Beacon selection enable control., reset value: 0x0, access type: RW
		uint32 reserved3 : 2;
		uint32 txSelLoggerPlanLockerPriority : 2; //no description, reset value: 0x0, access type: RW
		uint32 txSelLoggerPlanLockerEn : 1; //no description, reset value: 0x0, access type: RW
		uint32 reserved4 : 9;
	} bitFields;
} RegTxSelectorTxSelLoggerCfg_u;

/*REG_TX_SELECTOR_TX_SEL_LOGGER_BUSY 0x118 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSelLoggerSelectionLockerBusy : 1; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 3;
		uint32 txSelLoggerMuLockerBusy : 1; //no description, reset value: 0x0, access type: RO
		uint32 reserved1 : 3;
		uint32 txSelLoggerBitmapUpdateBusy : 1; //no description, reset value: 0x0, access type: RO
		uint32 reserved2 : 3;
		uint32 txSelLoggerMuBitmapUpdateBusy : 1; //no description, reset value: 0x0, access type: RO
		uint32 reserved3 : 3;
		uint32 txSelLoggerPlanLockerBusy : 1; //no description, reset value: 0x0, access type: RO
		uint32 reserved4 : 15;
	} bitFields;
} RegTxSelectorTxSelLoggerBusy_u;

/*REG_TX_SELECTOR_TX_SEL_LOCKER_EN 0x1A0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 lockerEn : 1; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 7;
		uint32 lockerSm : 2; //no description, reset value: 0x0, access type: RO
		uint32 reserved1 : 22;
	} bitFields;
} RegTxSelectorTxSelLockerEn_u;

/*REG_TX_SELECTOR_TX_SEL_SW_MASTER0_LOCKER 0x1A4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 master0LockerStaSel : 8; //Station number, reset value: 0x0, access type: RW
		uint32 master0LockerTidSel : 4; //TID number, reset value: 0x0, access type: RW
		uint32 reserved0 : 4;
		uint32 master0LockerBitmapSel : 3; //Bitmap Queue selection, reset value: 0x0, access type: RW
		uint32 reserved1 : 5;
		uint32 master0LockerBitTypeSel : 2; //Information bit type selector: , 00: Try Disable request. , 01: Try lock request. , 10: Force Disable request. , 11: Force lock request., reset value: 0x0, access type: RW
		uint32 reserved2 : 5;
		uint32 master0LockerAllTids : 1; //no description, reset value: 0x0, access type: RW
	} bitFields;
} RegTxSelectorTxSelSwMaster0Locker_u;

/*REG_TX_SELECTOR_TX_SEL_SW_MASTER1_LOCKER 0x1A8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 master1LockerStaSel : 8; //Station number, reset value: 0x0, access type: RW
		uint32 master1LockerTidSel : 4; //TID number, reset value: 0x0, access type: RW
		uint32 reserved0 : 4;
		uint32 master1LockerBitmapSel : 3; //Bitmap Queue selection, reset value: 0x0, access type: RW
		uint32 reserved1 : 5;
		uint32 master1LockerBitTypeSel : 2; //Information bit type selector: , 00: Try Disable request. , 01: Try lock request. , 10: Force Disable request. , 11: Force lock request., reset value: 0x0, access type: RW
		uint32 reserved2 : 5;
		uint32 master1LockerAllTids : 1; //no description, reset value: 0x0, access type: RW
	} bitFields;
} RegTxSelectorTxSelSwMaster1Locker_u;

/*REG_TX_SELECTOR_TX_SEL_SW_MASTER2_LOCKER 0x1AC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 master2LockerStaSel : 8; //Station number, reset value: 0x0, access type: RW
		uint32 master2LockerTidSel : 4; //TID number, reset value: 0x0, access type: RW
		uint32 reserved0 : 4;
		uint32 master2LockerBitmapSel : 3; //Bitmap Queue selection, reset value: 0x0, access type: RW
		uint32 reserved1 : 5;
		uint32 master2LockerBitTypeSel : 2; //Information bit type selector: , 00: Try Disable request. , 01: Try lock request. , 10: Force Disable request. , 11: Force lock request., reset value: 0x0, access type: RW
		uint32 reserved2 : 5;
		uint32 master2LockerAllTids : 1; //no description, reset value: 0x0, access type: RW
	} bitFields;
} RegTxSelectorTxSelSwMaster2Locker_u;

/*REG_TX_SELECTOR_TX_SEL_SW_MASTER3_LOCKER 0x1B0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 master3LockerStaSel : 8; //Station number, reset value: 0x0, access type: RW
		uint32 master3LockerTidSel : 4; //TID number, reset value: 0x0, access type: RW
		uint32 reserved0 : 4;
		uint32 master3LockerBitmapSel : 3; //Bitmap Queue selection, reset value: 0x0, access type: RW
		uint32 reserved1 : 5;
		uint32 master3LockerBitTypeSel : 2; //Information bit type selector: , 00: Try Disable request. , 01: Try lock request. , 10: Force Disable request. , 11: Force lock request., reset value: 0x0, access type: RW
		uint32 reserved2 : 5;
		uint32 master3LockerAllTids : 1; //no description, reset value: 0x0, access type: RW
	} bitFields;
} RegTxSelectorTxSelSwMaster3Locker_u;

/*REG_TX_SELECTOR_TX_SEL_SW_MASTER0_LOCKER_STATUS 0x1B4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swMaster0ReqStatus : 5; //SW Master request status: , 0x0 --- IDLE. , 0x1 --- Pending. Waiting for client to unlock. Possible only for Force request. , 0x2 --- Off. Client is locked. , 0x4 --- Disabled. Client is disabled. , 0x8 --- Reject. Client is lock/Disable request is rejected. Relevant for Try request. , 0x10 --- Error. Lock request for a disabled client., reset value: 0x0, access type: RO
		uint32 reserved0 : 3;
		uint32 master0LockerDoneInt : 1; //Lock request done indication, reset value: 0x0, access type: RO
		uint32 reserved1 : 23;
	} bitFields;
} RegTxSelectorTxSelSwMaster0LockerStatus_u;

/*REG_TX_SELECTOR_TX_SEL_SW_MASTER1_LOCKER_STATUS 0x1B8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swMaster1ReqStatus : 5; //SW Master request status: , 0x0 --- IDLE. , 0x1 --- Pending. Waiting for client to unlock. Possible only for Force request. , 0x2 --- Off. Client is locked. , 0x4 --- Disabled. Client is disabled. , 0x8 --- Reject. Client is lock/Disable request is rejected. Relevant for Try request. , 0x10 --- Error. Lock request for a disabled client., reset value: 0x0, access type: RO
		uint32 reserved0 : 3;
		uint32 master1LockerDoneInt : 1; //Lock request done indication, reset value: 0x0, access type: RO
		uint32 reserved1 : 23;
	} bitFields;
} RegTxSelectorTxSelSwMaster1LockerStatus_u;

/*REG_TX_SELECTOR_TX_SEL_SW_MASTER2_LOCKER_STATUS 0x1BC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swMaster2ReqStatus : 5; //SW Master request status: , 0x0 --- IDLE. , 0x1 --- Pending. Waiting for client to unlock. Possible only for Force request. , 0x2 --- Off. Client is locked. , 0x4 --- Disabled. Client is disabled. , 0x8 --- Reject. Client is lock/Disable request is rejected. Relevant for Try request. , 0x10 --- Error. Lock request for a disabled client., reset value: 0x0, access type: RO
		uint32 reserved0 : 3;
		uint32 master2LockerDoneInt : 1; //Lock request done indication, reset value: 0x0, access type: RO
		uint32 reserved1 : 23;
	} bitFields;
} RegTxSelectorTxSelSwMaster2LockerStatus_u;

/*REG_TX_SELECTOR_TX_SEL_SW_MASTER3_LOCKER_STATUS 0x1C0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swMaster3ReqStatus : 5; //SW Master request status: , 0x0 --- IDLE. , 0x1 --- Pending. Waiting for client to unlock. Possible only for Force request. , 0x2 --- Off. Client is locked. , 0x4 --- Disabled. Client is disabled. , 0x8 --- Reject. Client is lock/Disable request is rejected. Relevant for Try request. , 0x10 --- Error. Lock request for a disabled client., reset value: 0x0, access type: RO
		uint32 reserved0 : 3;
		uint32 master3LockerDoneInt : 1; //Lock request done indication, reset value: 0x0, access type: RO
		uint32 reserved1 : 23;
	} bitFields;
} RegTxSelectorTxSelSwMaster3LockerStatus_u;

/*REG_TX_SELECTOR_TX_SEL_LOCKER_INT_CLR 0x1C4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swMaster0LockerIntClr : 1; //no description, reset value: 0x0, access type: WO
		uint32 swMaster1LockerIntClr : 1; //no description, reset value: 0x0, access type: WO
		uint32 swMaster2LockerIntClr : 1; //no description, reset value: 0x0, access type: WO
		uint32 swMaster3LockerIntClr : 1; //no description, reset value: 0x0, access type: WO
		uint32 reserved0 : 28;
	} bitFields;
} RegTxSelectorTxSelLockerIntClr_u;

/*REG_TX_SELECTOR_TX_SEL_LOCKER_PUSH_WHILE_BUSY 0x1C8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 master0PushWhileBusy : 1; //no description, reset value: 0x0, access type: RO
		uint32 master1PushWhileBusy : 1; //no description, reset value: 0x0, access type: RO
		uint32 master2PushWhileBusy : 1; //no description, reset value: 0x0, access type: RO
		uint32 master3PushWhileBusy : 1; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 28;
	} bitFields;
} RegTxSelectorTxSelLockerPushWhileBusy_u;

/*REG_TX_SELECTOR_TX_SEL_LOCKER_PUSH_WHILE_BUSY_CLR 0x1CC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 master0PushWhileBusyClr : 1; //no description, reset value: 0x0, access type: WO
		uint32 master1PushWhileBusyClr : 1; //no description, reset value: 0x0, access type: WO
		uint32 master2PushWhileBusyClr : 1; //no description, reset value: 0x0, access type: WO
		uint32 master3PushWhileBusyClr : 1; //no description, reset value: 0x0, access type: WO
		uint32 reserved0 : 28;
	} bitFields;
} RegTxSelectorTxSelLockerPushWhileBusyClr_u;

/*REG_TX_SELECTOR_TX_SELECTOR_SPARE 0x1D0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSelectorSpare : 32; //no description, reset value: 0x0, access type: RW
	} bitFields;
} RegTxSelectorTxSelectorSpare_u;

/*REG_TX_SELECTOR_TX_SEL_MU_SELECT_CFG 0x200 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 muEn : 1; //MU enable of the TX Selector, reset value: 0x0, access type: RW
		uint32 reserved0 : 3;
		uint32 muSecondarySel : 1; //MU secondary selection algorithm: , 0: MU Secondary TID equal to Primary TID , 1: MU Secondary TID can be any TID , , reset value: 0x0, access type: RW
		uint32 reserved1 : 3;
		uint32 maxGroupNum : 7; //Maximum number of supported groups. Number of groups is equal to max_group_num., reset value: 0xF, access type: RW
		uint32 reserved2 : 1;
		uint32 selectorMuLockEn : 1; //Enables/Disables the TX Selector HW lock: , 0x0: Don't lock after selection , 0x1: Lock after selection, reset value: 0x1, access type: RW
		uint32 reserved3 : 2;
		uint32 vhtMuSuSelBypass : 1; //no description, reset value: 0x1, access type: RW
		uint32 suSelEn1Sec : 1; //In case of VHT MU threshold fail - Configures if SU selection will happen, reset value: 0x1, access type: RW
		uint32 reserved4 : 1;
		uint32 muThresh1Sec : 1; //Minimum number of secondaries to be selected to enable selection. Possible values 0-1, reset value: 0x0, access type: RW
		uint32 reserved5 : 1;
		uint32 suSelEn2Sec : 1; //In case of VHT MU threshold fail - Configures if SU selection will happen, reset value: 0x1, access type: RW
		uint32 reserved6 : 1;
		uint32 muThresh2Sec : 2; //Minimum number of secondaries to be selected to enable selection. Possible values 0-2, reset value: 0x0, access type: RW
		uint32 suSelEn3Sec : 1; //In case of VHT MU threshold fail - Configures if SU selection will happen, reset value: 0x1, access type: RW
		uint32 reserved7 : 1;
		uint32 muThresh3Sec : 2; //Minimum number of secondaries to be selected to enable selection. Possible values 0-3 , Not functioning properly: WLAN7BVLSIIP-976, reset value: 0x0, access type: RW
	} bitFields;
} RegTxSelectorTxSelMuSelectCfg_u;

/*REG_TX_SELECTOR_TX_SEL_MU_BITMAP_UPDATE 0x204 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 memLineNum : 8; //MU Group index number. Can be 0 to 127., reset value: 0x0, access type: RW
		uint32 userPositionIdx : 2; //User position index number in a group, reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
		uint32 muBitTypeSel : 3; //MU bit type: , 0x0: Group User position STA , 0x1: Group lock , 0x2: Group Tx mode , 0x3: STA group mapping , 0x4: STA MU tid Enable , , reset value: 0x0, access type: RW
		uint32 allGrp : 1; //Indication that the lower 36 bits of the group are updated with the adjacent register data., reset value: 0x0, access type: RW
		uint32 muBitmapUpdateValue : 8; //Write data value - For each update there is a different data width. Data decoding is according to mu_bit_type_sel: , 0x0: 8 bits are STA num, Valid bit will be taken from "all_grp_valid_up". , 0x1: LSB bit is MU lock bit value. , 0x2: 2 LSB bits are TX mode value. , 0x3: 7 LSB bits are Group index, MSB is STA Primary enable bit. , 0x4: 8 bits for 8 TIDs enable/disable MU., reset value: 0x0, access type: RW
		uint32 allGrpValidUp : 4; //For all_grp the entire field is used to update the UP valid bits. When a single UP STA idx is updated the relevant bit is used., reset value: 0x0, access type: RW
		uint32 reserved1 : 3;
		uint32 muBitmapUpdateFree : 1; //Bit inidcating that the register is free for update instruction, reset value: 0x1, access type: RO
	} bitFields;
} RegTxSelectorTxSelMuBitmapUpdate_u;

/*REG_TX_SELECTOR_TX_SEL_MU_BITMAP_UPDATE_ALL_STA_WDATA 0x208 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 allGrpWdata : 32; //no description, reset value: 0x0, access type: RW
	} bitFields;
} RegTxSelectorTxSelMuBitmapUpdateAllStaWdata_u;

/*REG_TX_SELECTOR_TX_SEL_MU_PRIMARY_THRES 0x20C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pdPrimaryThres : 20; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 12;
	} bitFields;
} RegTxSelectorTxSelMuPrimaryThres_u;

/*REG_TX_SELECTOR_TX_SEL_MU_SECONDARY_THRES 0x210 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pdSecondaryThres : 20; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 12;
	} bitFields;
} RegTxSelectorTxSelMuSecondaryThres_u;

/*REG_TX_SELECTOR_TX_SEL_MU_ERR 0x214 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 primaryNotValidErr : 1; //no description, reset value: 0x0, access type: RO
		uint32 noPrimaryInGrpErr : 1; //no description, reset value: 0x0, access type: RO
		uint32 primaryGrpThresErr : 1; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 29;
	} bitFields;
} RegTxSelectorTxSelMuErr_u;

/*REG_TX_SELECTOR_TX_SEL_MU_ERR_CLR 0x218 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 primaryNotValidErrClr : 1; //no description, reset value: 0x0, access type: WO
		uint32 noPrimaryInGrpErrClr : 1; //no description, reset value: 0x0, access type: WO
		uint32 primaryGrpThresErrClr : 1; //no description, reset value: 0x0, access type: WO
		uint32 reserved0 : 29;
	} bitFields;
} RegTxSelectorTxSelMuErrClr_u;

/*REG_TX_SELECTOR_TX_SEL_MU_LOCKER_EN 0x220 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 muLockerEn : 1; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegTxSelectorTxSelMuLockerEn_u;

/*REG_TX_SELECTOR_TX_SEL_MU_SW_MASTER0_LOCKER 0x224 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 master0MuLockerGrpSel : 7; //Group index number, reset value: 0x0, access type: RW
		uint32 reserved0 : 17;
		uint32 master0MuLockerBitTypeSel : 2; //Information bit type selector, reset value: 0x0, access type: RW
		uint32 reserved1 : 6;
	} bitFields;
} RegTxSelectorTxSelMuSwMaster0Locker_u;

/*REG_TX_SELECTOR_TX_SEL_MU_SW_MASTER1_LOCKER 0x228 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 master1MuLockerGrpSel : 7; //Group index number, reset value: 0x0, access type: RW
		uint32 reserved0 : 17;
		uint32 master1MuLockerBitTypeSel : 2; //Information bit type selector, reset value: 0x0, access type: RW
		uint32 reserved1 : 6;
	} bitFields;
} RegTxSelectorTxSelMuSwMaster1Locker_u;

/*REG_TX_SELECTOR_TX_SEL_MU_SW_MASTER2_LOCKER 0x22C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 master2MuLockerGrpSel : 7; //Group index number, reset value: 0x0, access type: RW
		uint32 reserved0 : 17;
		uint32 master2MuLockerBitTypeSel : 2; //Information bit type selector, reset value: 0x0, access type: RW
		uint32 reserved1 : 6;
	} bitFields;
} RegTxSelectorTxSelMuSwMaster2Locker_u;

/*REG_TX_SELECTOR_TX_SEL_MU_SW_MASTER3_LOCKER 0x230 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 master3MuLockerGrpSel : 7; //Group index number, reset value: 0x0, access type: RW
		uint32 reserved0 : 17;
		uint32 master3MuLockerBitTypeSel : 2; //Information bit type selector, reset value: 0x0, access type: RW
		uint32 reserved1 : 6;
	} bitFields;
} RegTxSelectorTxSelMuSwMaster3Locker_u;

/*REG_TX_SELECTOR_TX_SEL_MU_SW_MASTER0_LOCKER_STATUS 0x234 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swMaster0MuReqStatus : 5; //SW Master request status: , 0x0 --- IDLE. , 0x1 --- Pending. Waiting for client to unlock. Possible only for Force request. , 0x2 --- Off. Client is locked. , 0x8 --- Reject. Client is lock/Disable request is rejected. Relevant for Try request., reset value: 0x0, access type: RO
		uint32 reserved0 : 3;
		uint32 master0MuLockerDoneInt : 1; //Lock request done indication, reset value: 0x0, access type: RO
		uint32 reserved1 : 23;
	} bitFields;
} RegTxSelectorTxSelMuSwMaster0LockerStatus_u;

/*REG_TX_SELECTOR_TX_SEL_MU_SW_MASTER1_LOCKER_STATUS 0x238 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swMaster1MuReqStatus : 5; //SW Master request status: , 0x0 --- IDLE. , 0x1 --- Pending. Waiting for client to unlock. Possible only for Force request. , 0x2 --- Off. Client is locked. , 0x8 --- Reject. Client is lock/Disable request is rejected. Relevant for Try request., reset value: 0x0, access type: RO
		uint32 reserved0 : 3;
		uint32 master1MuLockerDoneInt : 1; //Lock request done indication, reset value: 0x0, access type: RO
		uint32 reserved1 : 23;
	} bitFields;
} RegTxSelectorTxSelMuSwMaster1LockerStatus_u;

/*REG_TX_SELECTOR_TX_SEL_MU_SW_MASTER2_LOCKER_STATUS 0x23C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swMaster2MuReqStatus : 5; //SW Master request status: , 0x0 --- IDLE. , 0x1 --- Pending. Waiting for client to unlock. Possible only for Force request. , 0x2 --- Off. Client is locked. , 0x8 --- Reject. Client is lock/Disable request is rejected. Relevant for Try request., reset value: 0x0, access type: RO
		uint32 reserved0 : 3;
		uint32 master2MuLockerDoneInt : 1; //Lock request done indication, reset value: 0x0, access type: RO
		uint32 reserved1 : 23;
	} bitFields;
} RegTxSelectorTxSelMuSwMaster2LockerStatus_u;

/*REG_TX_SELECTOR_TX_SEL_MU_SW_MASTER3_LOCKER_STATUS 0x240 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swMaster3MuReqStatus : 5; //SW Master request status: , 0x0 --- IDLE. , 0x1 --- Pending. Waiting for client to unlock. Possible only for Force request. , 0x2 --- Off. Client is locked. , 0x8 --- Reject. Client is lock/Disable request is rejected. Relevant for Try request., reset value: 0x0, access type: RO
		uint32 reserved0 : 3;
		uint32 master3MuLockerDoneInt : 1; //Lock request done indication, reset value: 0x0, access type: RO
		uint32 reserved1 : 23;
	} bitFields;
} RegTxSelectorTxSelMuSwMaster3LockerStatus_u;

/*REG_TX_SELECTOR_TX_SEL_MU_LOCKER_INT_CLR 0x244 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swMaster0MuLockerIntClr : 1; //no description, reset value: 0x0, access type: WO
		uint32 swMaster1MuLockerIntClr : 1; //no description, reset value: 0x0, access type: WO
		uint32 swMaster2MuLockerIntClr : 1; //no description, reset value: 0x0, access type: WO
		uint32 swMaster3MuLockerIntClr : 1; //no description, reset value: 0x0, access type: WO
		uint32 reserved0 : 28;
	} bitFields;
} RegTxSelectorTxSelMuLockerIntClr_u;

/*REG_TX_SELECTOR_TX_SEL_MU_LOCKER_PUSH_WHILE_BUSY 0x248 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 master0MuPushWhileBusy : 1; //no description, reset value: 0x0, access type: RO
		uint32 master1MuPushWhileBusy : 1; //no description, reset value: 0x0, access type: RO
		uint32 master2MuPushWhileBusy : 1; //no description, reset value: 0x0, access type: RO
		uint32 master3MuPushWhileBusy : 1; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 28;
	} bitFields;
} RegTxSelectorTxSelMuLockerPushWhileBusy_u;

/*REG_TX_SELECTOR_TX_SEL_MU_LOCKER_PUSH_WHILE_BUSY_CLR 0x24C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 master0MuPushWhileBusyClr : 1; //no description, reset value: 0x0, access type: WO
		uint32 master1MuPushWhileBusyClr : 1; //no description, reset value: 0x0, access type: WO
		uint32 master2MuPushWhileBusyClr : 1; //no description, reset value: 0x0, access type: WO
		uint32 master3MuPushWhileBusyClr : 1; //no description, reset value: 0x0, access type: WO
		uint32 reserved0 : 28;
	} bitFields;
} RegTxSelectorTxSelMuLockerPushWhileBusyClr_u;

/*REG_TX_SELECTOR_TX_SEL_MU_UP0_RESULT 0x250 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 muUp0Tid : 3; //no description, reset value: 0x0, access type: RO
		uint32 muUp0Sta : 8; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 1;
		uint32 muUp0Vap : 5; //no description, reset value: 0x0, access type: RO
		uint32 muUp0TxqId : 3; //no description, reset value: 0x0, access type: RO
		uint32 muUp0AcId : 2; //no description, reset value: 0x0, access type: RO
		uint32 reserved1 : 2;
		uint32 muUp0BfReq : 2; //no description, reset value: 0x0, access type: RO
		uint32 reserved2 : 2;
		uint32 muUp0Valid : 1; //no description, reset value: 0x0, access type: RO
		uint32 reserved3 : 3;
	} bitFields;
} RegTxSelectorTxSelMuUp0Result_u;

/*REG_TX_SELECTOR_TX_SEL_MU_UP1_RESULT 0x254 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 muUp1Tid : 3; //no description, reset value: 0x0, access type: RO
		uint32 muUp1Sta : 8; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 1;
		uint32 muUp1Vap : 5; //no description, reset value: 0x0, access type: RO
		uint32 muUp1TxqId : 3; //no description, reset value: 0x0, access type: RO
		uint32 muUp1AcId : 2; //no description, reset value: 0x0, access type: RO
		uint32 reserved1 : 2;
		uint32 muUp1BfReq : 2; //no description, reset value: 0x0, access type: RO
		uint32 reserved2 : 2;
		uint32 muUp1Valid : 1; //no description, reset value: 0x0, access type: RO
		uint32 reserved3 : 3;
	} bitFields;
} RegTxSelectorTxSelMuUp1Result_u;

/*REG_TX_SELECTOR_TX_SEL_MU_UP2_RESULT 0x258 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 muUp2Tid : 3; //no description, reset value: 0x0, access type: RO
		uint32 muUp2Sta : 8; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 1;
		uint32 muUp2Vap : 5; //no description, reset value: 0x0, access type: RO
		uint32 muUp2TxqId : 3; //no description, reset value: 0x0, access type: RO
		uint32 muUp2AcId : 2; //no description, reset value: 0x0, access type: RO
		uint32 reserved1 : 2;
		uint32 muUp2BfReq : 2; //no description, reset value: 0x0, access type: RO
		uint32 reserved2 : 2;
		uint32 muUp2Valid : 1; //no description, reset value: 0x0, access type: RO
		uint32 reserved3 : 3;
	} bitFields;
} RegTxSelectorTxSelMuUp2Result_u;

/*REG_TX_SELECTOR_TX_SEL_MU_UP3_RESULT 0x25C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 muUp3Tid : 3; //no description, reset value: 0x0, access type: RO
		uint32 muUp3Sta : 8; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 1;
		uint32 muUp3Vap : 5; //no description, reset value: 0x0, access type: RO
		uint32 muUp3TxqId : 3; //no description, reset value: 0x0, access type: RO
		uint32 muUp3AcId : 2; //no description, reset value: 0x0, access type: RO
		uint32 reserved1 : 2;
		uint32 muUp3BfReq : 2; //no description, reset value: 0x0, access type: RO
		uint32 reserved2 : 2;
		uint32 muUp3Valid : 1; //no description, reset value: 0x0, access type: RO
		uint32 reserved3 : 3;
	} bitFields;
} RegTxSelectorTxSelMuUp3Result_u;

/*REG_TX_SELECTOR_TX_SEL_MU_UP0_BITMAP 0x260 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 muUp0BitmapReq0 : 1; //Request bit, reset value: 0x0, access type: RO
		uint32 muUp0BitmapPsType0 : 1; //PS type, reset value: 0x0, access type: RO
		uint32 muUp0BitmapPsMode0 : 1; //PS mode, reset value: 0x0, access type: RO
		uint32 muUp0BitmapDataInQ0 : 1; //Data in Q, reset value: 0x0, access type: RO
		uint32 muUp0BitmapReq1 : 1; //Request bit, reset value: 0x0, access type: RO
		uint32 muUp0BitmapPsType1 : 1; //PS type, reset value: 0x0, access type: RO
		uint32 muUp0BitmapPsMode1 : 1; //PS mode, reset value: 0x0, access type: RO
		uint32 muUp0BitmapDataInQ1 : 1; //Data in Q, reset value: 0x0, access type: RO
		uint32 muUp0BitmapReq2 : 1; //Request bit, reset value: 0x0, access type: RO
		uint32 muUp0BitmapPsType2 : 1; //PS type, reset value: 0x0, access type: RO
		uint32 muUp0BitmapPsMode2 : 1; //PS mode, reset value: 0x0, access type: RO
		uint32 muUp0BitmapDataInQ2 : 1; //Data in Q, reset value: 0x0, access type: RO
		uint32 muUp0BitmapReq3 : 1; //Request bit, reset value: 0x0, access type: RO
		uint32 muUp0BitmapPsType3 : 1; //PS type, reset value: 0x0, access type: RO
		uint32 muUp0BitmapPsMode3 : 1; //PS mode, reset value: 0x0, access type: RO
		uint32 muUp0BitmapDataInQ3 : 1; //Data in Q, reset value: 0x0, access type: RO
		uint32 muUp0BitmapReq4 : 1; //Request bit, reset value: 0x0, access type: RO
		uint32 muUp0BitmapPsType4 : 1; //PS type, reset value: 0x0, access type: RO
		uint32 muUp0BitmapPsMode4 : 1; //PS mode, reset value: 0x0, access type: RO
		uint32 muUp0BitmapDataInQ4 : 1; //Data in Q, reset value: 0x0, access type: RO
		uint32 muUp0BitmapReq5 : 1; //Request bit, reset value: 0x0, access type: RO
		uint32 muUp0BitmapPsType5 : 1; //PS type, reset value: 0x0, access type: RO
		uint32 muUp0BitmapPsMode5 : 1; //PS mode, reset value: 0x0, access type: RO
		uint32 muUp0BitmapDataInQ5 : 1; //Data in Q, reset value: 0x0, access type: RO
		uint32 muUp0BitmapReq6 : 1; //Request bit, reset value: 0x0, access type: RO
		uint32 muUp0BitmapPsType6 : 1; //PS type, reset value: 0x0, access type: RO
		uint32 muUp0BitmapPsMode6 : 1; //PS mode, reset value: 0x0, access type: RO
		uint32 muUp0BitmapDataInQ6 : 1; //Data in Q, reset value: 0x0, access type: RO
		uint32 muUp0BitmapReq7 : 1; //Request bit, reset value: 0x0, access type: RO
		uint32 muUp0BitmapPsType7 : 1; //PS type, reset value: 0x0, access type: RO
		uint32 muUp0BitmapPsMode7 : 1; //PS mode, reset value: 0x0, access type: RO
		uint32 muUp0BitmapDataInQ7 : 1; //Data in Q, reset value: 0x0, access type: RO
	} bitFields;
} RegTxSelectorTxSelMuUp0Bitmap_u;

/*REG_TX_SELECTOR_TX_SEL_HE_MU_UP0_BITMAP 0x264 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 heMuUp0BitmapReq8 : 1; //Request bit, reset value: 0x0, access type: RO
		uint32 heMuUp0BitmapPsType8 : 1; //PS type, reset value: 0x0, access type: RO
		uint32 heMuUp0BitmapPsMode8 : 1; //PS mode, reset value: 0x0, access type: RO
		uint32 heMuUp0BitmapDataInQ8 : 1; //Data in Q, reset value: 0x0, access type: RO
		uint32 heMuUp0BitmapStatus : 25; //HE bitmap Status - {ul_he_mu_en,dl_he_mu_en[8:0],ul_sta_tid_data_in_q[7:0],twt_avail,twt_sp,twt_ann,omi_valid,omi_supp,tomi_ul_dis,tomi_dl_dis}, reset value: 0x0, access type: RO
		uint32 reserved0 : 3;
	} bitFields;
} RegTxSelectorTxSelHeMuUp0Bitmap_u;

/*REG_TX_SELECTOR_TX_SEL_HE_PRE_AGG_UP0_BITMAP 0x268 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 heSelUp0PreAggBitmap : 14; //Pre agg bitmap: , 	DISABLE_HE_SU_ER - [13] , 	TOMI_MAX_NSS     - [12:10] , 	OMI_BW           - [9:7] , 	ROMI_MAX_NSS     - [6:4] , 	OMI_SUPPORTED    - [3]  , 	OMI_VALID        - [2] , 	UL_HE_MU_EN      - [1]  , 	INSERT_A_CONTROL - [0] , reset value: 0x0, access type: RO
		uint32 reserved0 : 18;
	} bitFields;
} RegTxSelectorTxSelHePreAggUp0Bitmap_u;

/*REG_TX_SELECTOR_TX_SEL_MU_UP1_BITMAP 0x26C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 9;
		uint32 muUp1BitmapPsType2 : 1; //PS type, reset value: 0x0, access type: RO
		uint32 muUp1BitmapPsMode2 : 1; //PS mode, reset value: 0x0, access type: RO
		uint32 muUp1BitmapDataInQ2 : 1; //Data in Q, reset value: 0x0, access type: RO
		uint32 muUp1BitmapReq3 : 1; //Request bit, reset value: 0x0, access type: RO
		uint32 muUp1BitmapPsType3 : 1; //PS type, reset value: 0x0, access type: RO
		uint32 muUp1BitmapPsMode3 : 1; //PS mode, reset value: 0x0, access type: RO
		uint32 muUp1BitmapDataInQ3 : 1; //Data in Q, reset value: 0x0, access type: RO
		uint32 muUp1BitmapReq4 : 1; //Request bit, reset value: 0x0, access type: RO
		uint32 muUp1BitmapPsType4 : 1; //PS type, reset value: 0x0, access type: RO
		uint32 muUp1BitmapPsMode4 : 1; //PS mode, reset value: 0x0, access type: RO
		uint32 muUp1BitmapDataInQ4 : 1; //Data in Q, reset value: 0x0, access type: RO
		uint32 muUp1BitmapReq5 : 1; //Request bit, reset value: 0x0, access type: RO
		uint32 muUp1BitmapPsType5 : 1; //PS type, reset value: 0x0, access type: RO
		uint32 muUp1BitmapPsMode5 : 1; //PS mode, reset value: 0x0, access type: RO
		uint32 muUp1BitmapDataInQ5 : 1; //Data in Q, reset value: 0x0, access type: RO
		uint32 muUp1BitmapReq6 : 1; //Request bit, reset value: 0x0, access type: RO
		uint32 muUp1BitmapPsType6 : 1; //PS type, reset value: 0x0, access type: RO
		uint32 muUp1BitmapPsMode6 : 1; //PS mode, reset value: 0x0, access type: RO
		uint32 muUp1BitmapDataInQ6 : 1; //Data in Q, reset value: 0x0, access type: RO
		uint32 muUp1BitmapReq7 : 1; //Request bit, reset value: 0x0, access type: RO
		uint32 muUp1BitmapPsType7 : 1; //PS type, reset value: 0x0, access type: RO
		uint32 muUp1BitmapPsMode7 : 1; //PS mode, reset value: 0x0, access type: RO
		uint32 muUp1BitmapDataInQ7 : 1; //Data in Q, reset value: 0x0, access type: RO
	} bitFields;
} RegTxSelectorTxSelMuUp1Bitmap_u;

/*REG_TX_SELECTOR_TX_SEL_HE_MU_UP1_BITMAP 0x270 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 heMuUp1BitmapReq8 : 1; //Request bit, reset value: 0x0, access type: RO
		uint32 heMuUp1BitmapPsType8 : 1; //PS type, reset value: 0x0, access type: RO
		uint32 heMuUp1BitmapPsMode8 : 1; //PS mode, reset value: 0x0, access type: RO
		uint32 heMuUp1BitmapDataInQ8 : 1; //Data in Q, reset value: 0x0, access type: RO
		uint32 heMuUp1BitmapStatus : 25; //HE bitmap Status - {ul_he_mu_en,dl_he_mu_en[8:0],ul_sta_tid_data_in_q[7:0],twt_avail,twt_sp,twt_ann,omi_valid,omi_supp,tomi_ul_dis,tomi_dl_dis}, reset value: 0x0, access type: RO
		uint32 reserved0 : 3;
	} bitFields;
} RegTxSelectorTxSelHeMuUp1Bitmap_u;

/*REG_TX_SELECTOR_TX_SEL_HE_PRE_AGG_UP1_BITMAP 0x274 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 heSelUp1PreAggBitmap : 14; //Pre agg bitmap: , 	DISABLE_HE_SU_ER - [13] , 	TOMI_MAX_NSS     - [12:10] , 	OMI_BW           - [9:7] , 	ROMI_MAX_NSS     - [6:4] , 	OMI_SUPPORTED    - [3]  , 	OMI_VALID        - [2] , 	UL_HE_MU_EN      - [1]  , 	INSERT_A_CONTROL - [0] , reset value: 0x0, access type: RO
		uint32 reserved0 : 18;
	} bitFields;
} RegTxSelectorTxSelHePreAggUp1Bitmap_u;

/*REG_TX_SELECTOR_TX_SEL_MU_UP2_BITMAP 0x278 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 7;
		uint32 muUp2BitmapDataInQ1 : 1; //Data in Q, reset value: 0x0, access type: RO
		uint32 muUp2BitmapReq2 : 1; //Request bit, reset value: 0x0, access type: RO
		uint32 muUp2BitmapPsType2 : 1; //PS type, reset value: 0x0, access type: RO
		uint32 muUp2BitmapPsMode2 : 1; //PS mode, reset value: 0x0, access type: RO
		uint32 muUp2BitmapDataInQ2 : 1; //Data in Q, reset value: 0x0, access type: RO
		uint32 muUp2BitmapReq3 : 1; //Request bit, reset value: 0x0, access type: RO
		uint32 muUp2BitmapPsType3 : 1; //PS type, reset value: 0x0, access type: RO
		uint32 muUp2BitmapPsMode3 : 1; //PS mode, reset value: 0x0, access type: RO
		uint32 muUp2BitmapDataInQ3 : 1; //Data in Q, reset value: 0x0, access type: RO
		uint32 muUp2BitmapReq4 : 1; //Request bit, reset value: 0x0, access type: RO
		uint32 muUp2BitmapPsType4 : 1; //PS type, reset value: 0x0, access type: RO
		uint32 muUp2BitmapPsMode4 : 1; //PS mode, reset value: 0x0, access type: RO
		uint32 muUp2BitmapDataInQ4 : 1; //Data in Q, reset value: 0x0, access type: RO
		uint32 muUp2BitmapReq5 : 1; //Request bit, reset value: 0x0, access type: RO
		uint32 muUp2BitmapPsType5 : 1; //PS type, reset value: 0x0, access type: RO
		uint32 muUp2BitmapPsMode5 : 1; //PS mode, reset value: 0x0, access type: RO
		uint32 muUp2BitmapDataInQ5 : 1; //Data in Q, reset value: 0x0, access type: RO
		uint32 muUp2BitmapReq6 : 1; //Request bit, reset value: 0x0, access type: RO
		uint32 muUp2BitmapPsType6 : 1; //PS type, reset value: 0x0, access type: RO
		uint32 muUp2BitmapPsMode6 : 1; //PS mode, reset value: 0x0, access type: RO
		uint32 muUp2BitmapDataInQ6 : 1; //Data in Q, reset value: 0x0, access type: RO
		uint32 muUp2BitmapReq7 : 1; //Request bit, reset value: 0x0, access type: RO
		uint32 muUp2BitmapPsType7 : 1; //PS type, reset value: 0x0, access type: RO
		uint32 muUp2BitmapPsMode7 : 1; //PS mode, reset value: 0x0, access type: RO
		uint32 muUp2BitmapDataInQ7 : 1; //Data in Q, reset value: 0x0, access type: RO
	} bitFields;
} RegTxSelectorTxSelMuUp2Bitmap_u;

/*REG_TX_SELECTOR_TX_SEL_HE_MU_UP2_BITMAP 0x27C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 heMuUp2BitmapReq8 : 1; //Request bit, reset value: 0x0, access type: RO
		uint32 heMuUp2BitmapPsType8 : 1; //PS type, reset value: 0x0, access type: RO
		uint32 heMuUp2BitmapPsMode8 : 1; //PS mode, reset value: 0x0, access type: RO
		uint32 heMuUp2BitmapDataInQ8 : 1; //Data in Q, reset value: 0x0, access type: RO
		uint32 heMuUp2BitmapStatus : 25; //HE bitmap Status - {ul_he_mu_en,dl_he_mu_en[8:0],ul_sta_tid_data_in_q[7:0],twt_avail,twt_sp,twt_ann,omi_valid,omi_supp,tomi_ul_dis,tomi_dl_dis}, reset value: 0x0, access type: RO
		uint32 reserved0 : 3;
	} bitFields;
} RegTxSelectorTxSelHeMuUp2Bitmap_u;

/*REG_TX_SELECTOR_TX_SEL_HE_PRE_AGG_UP2_BITMAP 0x280 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 heSelUp2PreAggBitmap : 14; //Pre agg bitmap: , 	DISABLE_HE_SU_ER - [13] , 	TOMI_MAX_NSS     - [12:10] , 	OMI_BW           - [9:7] , 	ROMI_MAX_NSS     - [6:4] , 	OMI_SUPPORTED    - [3]  , 	OMI_VALID        - [2] , 	UL_HE_MU_EN      - [1]  , 	INSERT_A_CONTROL - [0] , reset value: 0x0, access type: RO
		uint32 reserved0 : 18;
	} bitFields;
} RegTxSelectorTxSelHePreAggUp2Bitmap_u;

/*REG_TX_SELECTOR_TX_SEL_MU_UP3_BITMAP 0x284 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 muUp3BitmapReq0 : 1; //Request bit, reset value: 0x0, access type: RO
		uint32 muUp3BitmapPsType0 : 1; //PS type, reset value: 0x0, access type: RO
		uint32 muUp3BitmapPsMode0 : 1; //PS mode, reset value: 0x0, access type: RO
		uint32 muUp3BitmapDataInQ0 : 1; //Data in Q, reset value: 0x0, access type: RO
		uint32 muUp3BitmapReq1 : 1; //Request bit, reset value: 0x0, access type: RO
		uint32 muUp3BitmapPsType1 : 1; //PS type, reset value: 0x0, access type: RO
		uint32 muUp3BitmapPsMode1 : 1; //PS mode, reset value: 0x0, access type: RO
		uint32 muUp3BitmapDataInQ1 : 1; //Data in Q, reset value: 0x0, access type: RO
		uint32 muUp3BitmapReq2 : 1; //Request bit, reset value: 0x0, access type: RO
		uint32 muUp3BitmapPsType2 : 1; //PS type, reset value: 0x0, access type: RO
		uint32 muUp3BitmapPsMode2 : 1; //PS mode, reset value: 0x0, access type: RO
		uint32 muUp3BitmapDataInQ2 : 1; //Data in Q, reset value: 0x0, access type: RO
		uint32 muUp3BitmapReq3 : 1; //Request bit, reset value: 0x0, access type: RO
		uint32 muUp3BitmapPsType3 : 1; //PS type, reset value: 0x0, access type: RO
		uint32 muUp3BitmapPsMode3 : 1; //PS mode, reset value: 0x0, access type: RO
		uint32 muUp3BitmapDataInQ3 : 1; //Data in Q, reset value: 0x0, access type: RO
		uint32 muUp3BitmapReq4 : 1; //Request bit, reset value: 0x0, access type: RO
		uint32 muUp3BitmapPsType4 : 1; //PS type, reset value: 0x0, access type: RO
		uint32 muUp3BitmapPsMode4 : 1; //PS mode, reset value: 0x0, access type: RO
		uint32 muUp3BitmapDataInQ4 : 1; //Data in Q, reset value: 0x0, access type: RO
		uint32 muUp3BitmapReq5 : 1; //Request bit, reset value: 0x0, access type: RO
		uint32 muUp3BitmapPsType5 : 1; //PS type, reset value: 0x0, access type: RO
		uint32 muUp3BitmapPsMode5 : 1; //PS mode, reset value: 0x0, access type: RO
		uint32 muUp3BitmapDataInQ5 : 1; //Data in Q, reset value: 0x0, access type: RO
		uint32 muUp3BitmapReq6 : 1; //Request bit, reset value: 0x0, access type: RO
		uint32 muUp3BitmapPsType6 : 1; //PS type, reset value: 0x0, access type: RO
		uint32 muUp3BitmapPsMode6 : 1; //PS mode, reset value: 0x0, access type: RO
		uint32 muUp3BitmapDataInQ6 : 1; //Data in Q, reset value: 0x0, access type: RO
		uint32 muUp3BitmapReq7 : 1; //Request bit, reset value: 0x0, access type: RO
		uint32 muUp3BitmapPsType7 : 1; //PS type, reset value: 0x0, access type: RO
		uint32 muUp3BitmapPsMode7 : 1; //PS mode, reset value: 0x0, access type: RO
		uint32 muUp3BitmapDataInQ7 : 1; //Data in Q, reset value: 0x0, access type: RO
	} bitFields;
} RegTxSelectorTxSelMuUp3Bitmap_u;

/*REG_TX_SELECTOR_TX_SEL_HE_MU_UP3_BITMAP 0x288 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 heMuUp3BitmapReq8 : 1; //Request bit, reset value: 0x0, access type: RO
		uint32 heMuUp3BitmapPsType8 : 1; //PS type, reset value: 0x0, access type: RO
		uint32 heMuUp3BitmapPsMode8 : 1; //PS mode, reset value: 0x0, access type: RO
		uint32 heMuUp3BitmapDataInQ8 : 1; //Data in Q, reset value: 0x0, access type: RO
		uint32 heMuUp3BitmapStatus : 25; //HE bitmap Status - {ul_he_mu_en,dl_he_mu_en[8:0],ul_sta_tid_data_in_q[7:0],twt_avail,twt_sp,twt_ann,omi_valid,omi_supp,tomi_ul_dis,tomi_dl_dis}, reset value: 0x0, access type: RO
		uint32 reserved0 : 3;
	} bitFields;
} RegTxSelectorTxSelHeMuUp3Bitmap_u;

/*REG_TX_SELECTOR_TX_SEL_HE_PRE_AGG_UP3_BITMAP 0x28C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 heSelUp3PreAggBitmap : 14; //Pre agg bitmap: , 	DISABLE_HE_SU_ER - [13] , 	TOMI_MAX_NSS     - [12:10] , 	OMI_BW           - [9:7] , 	ROMI_MAX_NSS     - [6:4] , 	OMI_SUPPORTED    - [3]  , 	OMI_VALID        - [2] , 	UL_HE_MU_EN      - [1]  , 	INSERT_A_CONTROL - [0] , reset value: 0x0, access type: RO
		uint32 reserved0 : 18;
	} bitFields;
} RegTxSelectorTxSelHePreAggUp3Bitmap_u;

/*REG_TX_SELECTOR_TX_SEL_MU_GRP_BITMAP 0x290 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 muGrpValid : 1; //MU selection valid bit, reset value: 0x0, access type: RO
		uint32 primaryGrpPointer : 7; //Group number, reset value: 0x0, access type: RO
		uint32 primaryIdx : 2; //Index number indicating which UP is primary at the group, reset value: 0x0, access type: RO
		uint32 txMode : 2; //no description, reset value: 0x0, access type: RO
		uint32 upLockDone : 4; //4 bits, 1 bit per UP indicating which UP was locked at the SU bitmap, reset value: 0x0, access type: RO
		uint32 grpLockDone : 1; //MU group lock done/Not done, reset value: 0x0, access type: RO
		uint32 reserved0 : 15;
	} bitFields;
} RegTxSelectorTxSelMuGrpBitmap_u;

/*REG_TX_SELECTOR_TX_SEL_HE_GRP_BITMAP 0x294 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 selDlSel : 1; //DL/UL selection: , 0: UL , 1: DL, reset value: 0x0, access type: RO
		uint32 reserved0 : 3;
		uint32 selHeMuSu : 2; //HE/MU/SU Selection: , 0: Single User , 1: VHT MU , 2: HE, reset value: 0x0, access type: RO
		uint32 reserved1 : 2;
		uint32 selectionType : 6; //Selection Type, reset value: 0x0, access type: RO
		uint32 reserved2 : 2;
		uint32 selHePlanEntry : 7; //no description, reset value: 0x0, access type: RO
		uint32 reserved3 : 1;
		uint32 selHePlanLockDone : 1; //HE lock done, reset value: 0x0, access type: RO
		uint32 reserved4 : 7;
	} bitFields;
} RegTxSelectorTxSelHeGrpBitmap_u;

/*REG_TX_SELECTOR_TX_SEL_MU_SM_STATUS 0x298 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 muSelSm : 4; //MU selection main SM, reset value: 0x0, access type: RO
		uint32 muSelLockSm : 3; //MU lock SM, reset value: 0x0, access type: RO
		uint32 reserved0 : 1;
		uint32 secPdCountRdSm : 3; //RD read from QoS RAM SM, reset value: 0x0, access type: RO
		uint32 reserved1 : 1;
		uint32 muLockerSm : 2; //no description, reset value: 0x0, access type: RO
		uint32 reserved2 : 18;
	} bitFields;
} RegTxSelectorTxSelMuSmStatus_u;

/*REG_TX_SELECTOR_TX_SEL_MU_SU_REPORT_STATUS 0x29C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 txSelMuSuSecondary0 : 4; //MU secondary participated in the MU TX with reason. Only one reason will be displayed according to this Prioirty list: , 0-  Selected (i.e. there was a TX for that user position) , 1-  Selector ------ Secondary not valid in group , 2 - Selector ------ STA disabled , 3-  Selector ------ No data in queue , 4-  Selector ------ STA lock , 5-  Selector ------ PS without request , 6 - Selector ------ PD threshold fail , 7 - Selector ------ TID MU En bit is 0, reset value: 0x0, access type: RO
		uint32 txSelMuSuSecondary1 : 4; //MU secondary participated in the MU TX with reason. Only one reason will be displayed according to this Prioirty list: , 0-  Selected (i.e. there was a TX for that user position) , 1-  Selector ------ Secondary not valid in group , 2 - Selector ------ STA disabled , 3-  Selector ------ No data in queue , 4-  Selector ------ STA lock , 5-  Selector ------ PS without request , 6 - Selector ------ PD threshold fail , 7 - Selector ------ TID MU En bit is 0, reset value: 0x0, access type: RO
		uint32 txSelMuSuSecondary2 : 4; //MU secondary participated in the MU TX with reason. Only one reason will be displayed according to this Prioirty list: , 0-  Selected (i.e. there was a TX for that user position) , 1-  Selector ------ Secondary not valid in group , 2 - Selector ------ STA disabled , 3-  Selector ------ No data in queue , 4-  Selector ------ STA lock , 5-  Selector ------ PS without request , 6 - Selector ------ PD threshold fail , 7 - Selector ------ TID MU En bit is 0, reset value: 0x0, access type: RO
		uint32 txSelMuSuSecondary3 : 4; //MU secondary participated in the MU TX with reason. Only one reason will be displayed according to this Prioirty list: , 0-  Selected (i.e. there was a TX for that user position) , 1-  Selector ------ Secondary not valid in group , 2 - Selector ------ STA disabled , 3-  Selector ------ No data in queue , 4-  Selector ------ STA lock , 5-  Selector ------ PS without request , 6 - Selector ------ PD threshold fail , 7 - Selector ------ TID MU En bit is 0, reset value: 0x0, access type: RO
		uint32 txSelMuSuPrimary : 7; //MU was changed to SU because of Primary(More than one reason possible): , Bit0 ------ Primary not in group , Bit1 ------ Primary is not valid , Bit2 ------ Group number is bigger than threshold , Bit3 ------ Group locked , Bit4 ------ All secondaries are not valid , Bit5 ------ No MU because of PS legacy selection , Bit6 ------ No MU because of FW threshold fail, reset value: 0x0, access type: RO
		uint32 reserved0 : 1;
		uint32 txSelMuSuValid : 1; //no description, reset value: 0x0, access type: RO
		uint32 reserved1 : 7;
	} bitFields;
} RegTxSelectorTxSelMuSuReportStatus_u;

/*REG_TX_SELECTOR_TX_SEL_SELECT_MODE_CFG 0x2A0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dlWithUlEn : 1; //no description, reset value: 0x1, access type: RW
		uint32 reserved0 : 3;
		uint32 mgmtBypassEn : 1; //Enables MGMT selection over other TIDs feature, reset value: 0x0, access type: RW
		uint32 reserved1 : 3;
		uint32 mgmtAcNum : 2; //no description, reset value: 0x3, access type: RW
		uint32 reserved2 : 2;
		uint32 heMuFallbackSu : 1; //Enables selection fallback from HE MU to SU in case of plan lock., reset value: 0x0, access type: RW
		uint32 heMuOmiDisFallbackSu : 1; //Enables selection fallback from HE MU to SU in case of OMI DL disabled., reset value: 0x0, access type: RW
		uint32 reserved3 : 2;
		uint32 dlHePsWithoutReqEn : 1; //Allowing selection of DL HE MU PS with TWT==111 without PS req., reset value: 0x0, access type: RW
		uint32 reserved4 : 15;
	} bitFields;
} RegTxSelectorTxSelSelectModeCfg_u;

/*REG_TX_SELECTOR_TX_SEL_PLAN_LOCKER_EN 0x2A4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 planLockerEn : 1; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 7;
		uint32 planLockerSm : 2; //no description, reset value: 0x0, access type: RO
		uint32 reserved1 : 22;
	} bitFields;
} RegTxSelectorTxSelPlanLockerEn_u;

/*REG_TX_SELECTOR_TX_SEL_PLAN_SW_MASTER0_LOCKER 0x300 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 master0PlanLockerIdxSel : 7; //plan index number, reset value: 0x0, access type: RW
		uint32 reserved0 : 17;
		uint32 master0PlanLockerBitTypeSel : 2; //Information bit type selector , 00: NA , 01: Try lock request. , 10: NA , 11: Force lock request., reset value: 0x0, access type: RW
		uint32 reserved1 : 6;
	} bitFields;
} RegTxSelectorTxSelPlanSwMaster0Locker_u;

/*REG_TX_SELECTOR_TX_SEL_PLAN_SW_MASTER1_LOCKER 0x304 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 master1PlanLockerIdxSel : 7; //plan index number, reset value: 0x0, access type: RW
		uint32 reserved0 : 17;
		uint32 master1PlanLockerBitTypeSel : 2; //Information bit type selector , 00: NA , 01: Try lock request. , 10: NA , 11: Force lock request., reset value: 0x0, access type: RW
		uint32 reserved1 : 6;
	} bitFields;
} RegTxSelectorTxSelPlanSwMaster1Locker_u;

/*REG_TX_SELECTOR_TX_SEL_PLAN_SW_MASTER2_LOCKER 0x308 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 master2PlanLockerIdxSel : 7; //plan index number, reset value: 0x0, access type: RW
		uint32 reserved0 : 17;
		uint32 master2PlanLockerBitTypeSel : 2; //Information bit type selector , 00: NA , 01: Try lock request. , 10: NA , 11: Force lock request., reset value: 0x0, access type: RW
		uint32 reserved1 : 6;
	} bitFields;
} RegTxSelectorTxSelPlanSwMaster2Locker_u;

/*REG_TX_SELECTOR_TX_SEL_PLAN_SW_MASTER3_LOCKER 0x30C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 master3PlanLockerIdxSel : 7; //plan index number, reset value: 0x0, access type: RW
		uint32 reserved0 : 17;
		uint32 master3PlanLockerBitTypeSel : 2; //Information bit type selector , 00: NA , 01: Try lock request. , 10: NA , 11: Force lock request., reset value: 0x0, access type: RW
		uint32 reserved1 : 6;
	} bitFields;
} RegTxSelectorTxSelPlanSwMaster3Locker_u;

/*REG_TX_SELECTOR_TX_SEL_PLAN_SW_MASTER0_LOCKER_STATUS 0x310 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swMaster0PlanReqStatus : 5; //SW Master request status: , 0x0 --- IDLE. , 0x1 --- Pending. Waiting for client to unlock. Possible only for Force request. , 0x2 --- Off. Client is locked. , 0x8 --- Reject. Client is lock/Disable request is rejected. Relevant for Try request., reset value: 0x0, access type: RO
		uint32 reserved0 : 3;
		uint32 master0PlanLockerDoneInt : 1; //Lock request done indication, reset value: 0x0, access type: RO
		uint32 reserved1 : 23;
	} bitFields;
} RegTxSelectorTxSelPlanSwMaster0LockerStatus_u;

/*REG_TX_SELECTOR_TX_SEL_PLAN_SW_MASTER1_LOCKER_STATUS 0x314 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swMaster1PlanReqStatus : 5; //SW Master request status: , 0x0 --- IDLE. , 0x1 --- Pending. Waiting for client to unlock. Possible only for Force request. , 0x2 --- Off. Client is locked. , 0x8 --- Reject. Client is lock/Disable request is rejected. Relevant for Try request., reset value: 0x0, access type: RO
		uint32 reserved0 : 3;
		uint32 master1PlanLockerDoneInt : 1; //Lock request done indication, reset value: 0x0, access type: RO
		uint32 reserved1 : 23;
	} bitFields;
} RegTxSelectorTxSelPlanSwMaster1LockerStatus_u;

/*REG_TX_SELECTOR_TX_SEL_PLAN_SW_MASTER2_LOCKER_STATUS 0x318 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swMaster2PlanReqStatus : 5; //SW Master request status: , 0x0 --- IDLE. , 0x1 --- Pending. Waiting for client to unlock. Possible only for Force request. , 0x2 --- Off. Client is locked. , 0x8 --- Reject. Client is lock/Disable request is rejected. Relevant for Try request., reset value: 0x0, access type: RO
		uint32 reserved0 : 3;
		uint32 master2PlanLockerDoneInt : 1; //Lock request done indication, reset value: 0x0, access type: RO
		uint32 reserved1 : 23;
	} bitFields;
} RegTxSelectorTxSelPlanSwMaster2LockerStatus_u;

/*REG_TX_SELECTOR_TX_SEL_PLAN_SW_MASTER3_LOCKER_STATUS 0x31C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swMaster3PlanReqStatus : 5; //SW Master request status: , 0x0 --- IDLE. , 0x1 --- Pending. Waiting for client to unlock. Possible only for Force request. , 0x2 --- Off. Client is locked. , 0x8 --- Reject. Client is lock/Disable request is rejected. Relevant for Try request., reset value: 0x0, access type: RO
		uint32 reserved0 : 3;
		uint32 master3PlanLockerDoneInt : 1; //Lock request done indication, reset value: 0x0, access type: RO
		uint32 reserved1 : 23;
	} bitFields;
} RegTxSelectorTxSelPlanSwMaster3LockerStatus_u;

/*REG_TX_SELECTOR_TX_SEL_PLAN_LOCKER_INT_CLR 0x320 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swMaster0PlanLockerIntClr : 1; //no description, reset value: 0x0, access type: WO
		uint32 swMaster1PlanLockerIntClr : 1; //no description, reset value: 0x0, access type: WO
		uint32 swMaster2PlanLockerIntClr : 1; //no description, reset value: 0x0, access type: WO
		uint32 swMaster3PlanLockerIntClr : 1; //no description, reset value: 0x0, access type: WO
		uint32 reserved0 : 28;
	} bitFields;
} RegTxSelectorTxSelPlanLockerIntClr_u;

/*REG_TX_SELECTOR_TX_SEL_PLAN_LOCKER_PUSH_WHILE_BUSY 0x324 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 master0PlanPushWhileBusy : 1; //no description, reset value: 0x0, access type: RO
		uint32 master1PlanPushWhileBusy : 1; //no description, reset value: 0x0, access type: RO
		uint32 master2PlanPushWhileBusy : 1; //no description, reset value: 0x0, access type: RO
		uint32 master3PlanPushWhileBusy : 1; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 28;
	} bitFields;
} RegTxSelectorTxSelPlanLockerPushWhileBusy_u;

/*REG_TX_SELECTOR_TX_SEL_PLAN_LOCKER_PUSH_WHILE_BUSY_CLR 0x328 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 master0PlanPushWhileBusyClr : 1; //no description, reset value: 0x0, access type: WO
		uint32 master1PlanPushWhileBusyClr : 1; //no description, reset value: 0x0, access type: WO
		uint32 master2PlanPushWhileBusyClr : 1; //no description, reset value: 0x0, access type: WO
		uint32 master3PlanPushWhileBusyClr : 1; //no description, reset value: 0x0, access type: WO
		uint32 reserved0 : 28;
	} bitFields;
} RegTxSelectorTxSelPlanLockerPushWhileBusyClr_u;

/*REG_TX_SELECTOR_TX_SEL_PRE_AGG_SEL_REQ_BITS0 0x380 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 preAggDl : 1; //no description, reset value: 0x0, access type: RO
		uint32 preAggMgmtOnly : 1; //no description, reset value: 0x0, access type: RO
		uint32 preAggPolicy : 2; //no description, reset value: 0x0, access type: RO
		uint32 preAggLegacyPsEn : 1; //no description, reset value: 0x0, access type: RO
		uint32 preAggHeMuOnly : 1; //no description, reset value: 0x0, access type: RO
		uint32 preAggOmiControl : 2; //no description, reset value: 0x0, access type: RO
		uint32 preAggStaNum : 8; //no description, reset value: 0x0, access type: RO
		uint32 preAggTidEn : 9; //no description, reset value: 0x0, access type: RO
		uint32 preAggMaxSelTidNum : 3; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 1;
		uint32 preAggPrimaryTid : 3; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegTxSelectorTxSelPreAggSelReqBits0_u;

/*REG_TX_SELECTOR_TX_SEL_PRE_AGG_SEL_REQ_BITS1 0x384 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 preAggTidInRetry : 9; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 3;
		uint32 preAggHwLockedTid : 9; //no description, reset value: 0x0, access type: RO
		uint32 reserved1 : 1;
		uint32 validateDlHeMuEn : 1; //no description, reset value: 0x0, access type: RO
		uint32 preAggDlHePsWithoutReqEn : 1; //no description, reset value: 0x0, access type: RO
		uint32 preAggLockSelectedMode : 2; //no description, reset value: 0x0, access type: RO
		uint32 reserved2 : 1;
		uint32 preAggVapIdx : 5; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegTxSelectorTxSelPreAggSelReqBits1_u;

/*REG_TX_SELECTOR_TX_SEL_PRE_AGG_SEL_RESULT_BITS0 0x388 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 preAggResultSelectedTids : 9; //no description, reset value: 0x0, access type: RO
		uint32 preAggResultSelectorLockedTids : 9; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 2;
		uint32 preAggStaNotSelectedReason : 11; //no description, reset value: 0x0, access type: RO
		uint32 reserved1 : 1;
	} bitFields;
} RegTxSelectorTxSelPreAggSelResultBits0_u;

/*REG_TX_SELECTOR_TX_SEL_PRE_AGG_SEL_RESULT_BITS1 0x38C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 preAggSelStatus031 : 32; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegTxSelectorTxSelPreAggSelResultBits1_u;

/*REG_TX_SELECTOR_TX_SEL_PRE_AGG_SEL_RESULT_BITS2 0x390 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 preAggUlStaTidDataInQ : 8; //no description, reset value: 0x0, access type: RO
		uint32 preAggTwtAvail : 1; //no description, reset value: 0x0, access type: RO
		uint32 preAggTwtSp : 1; //no description, reset value: 0x0, access type: RO
		uint32 preAggTwtAnn : 1; //no description, reset value: 0x0, access type: RO
		uint32 preAggOmiValid : 1; //no description, reset value: 0x0, access type: RO
		uint32 preAggOmiSupp : 1; //no description, reset value: 0x0, access type: RO
		uint32 preAggTomiDis : 1; //no description, reset value: 0x0, access type: RO
		uint32 preAggOmiBw : 3; //no description, reset value: 0x0, access type: RO
		uint32 preAggTomiMaxNss : 3; //no description, reset value: 0x0, access type: RO
		uint32 preAggRomiMaxNss : 3; //no description, reset value: 0x0, access type: RO
		uint32 preAggDisableHeSuEr : 1; //no description, reset value: 0x0, access type: RO
		uint32 preAggInsertAControl : 1; //no description, reset value: 0x0, access type: RO
		uint32 preAggUlHeMuEn : 1; //no description, reset value: 0x0, access type: RO
		uint32 preAggSelStatus3235 : 4; //no description, reset value: 0x0, access type: RO
		uint32 preAggSpValidInVap : 1; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 1;
	} bitFields;
} RegTxSelectorTxSelPreAggSelResultBits2_u;

/*REG_TX_SELECTOR_TX_SEL_CHICKEN_BITS 0x394 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 crKeepSelBitmapOrder : 1; //When set selector bitmap read command waits for all previous write commands to be invoked before it starts, which keeps the commands to be invoked in order., reset value: 0x1, access type: RW
		uint32 crIgnorePsReqInUlSelStaPsLegacy : 1; //When set PS request is ignored in UL_STA_PS_LEGACY_RX_DATA selection., reset value: 0x0, access type: RW
		uint32 crIgnorePsReqInUlSelStaPsWmm : 1; //When set PS request is ignored in UL_STA_PS_WMM_RX_DATA selection., reset value: 0x0, access type: RW
		uint32 crIgnoreRecalc : 1; //When set Recalculation Request from TXH will be ignored , reset value: 0x0, access type: RW
		uint32 crIgnoreTgenBiUlDataInQ : 1; //Ignore UL Data in Q relation in TIM GEN interface BI logic, reset value: 0x1, access type: RW
		uint32 reserved0 : 27;
	} bitFields;
} RegTxSelectorTxSelChickenBits_u;

/*REG_TX_SELECTOR_TX_SEL_MLD_VAP_PS 0x398 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 crVapPs : 32; //When set selector bitmap read command waits for all previous write commands to be invoked before it starts, which keeps the commands to be invoked in order., reset value: 0x0, access type: RW
	} bitFields;
} RegTxSelectorTxSelMldVapPs_u;

//========================================
/* REG_TX_SELECTOR_TX_SEL_INSTR 0x0 */
#define B0_TX_SELECTOR_TX_SEL_INSTR_FIELD_INSTR_MASK                                                              0x00000003
//========================================
/* REG_TX_SELECTOR_TX_SEL_BITMAP_UPDATE_MASTER0 0x4 */
#define B0_TX_SELECTOR_TX_SEL_BITMAP_UPDATE_MASTER0_FIELD_MASTER0_STA_SEL_MASK                                    0x000000FF
#define B0_TX_SELECTOR_TX_SEL_BITMAP_UPDATE_MASTER0_FIELD_MASTER0_BITMAP_SEL_MASK                                 0x00070000
#define B0_TX_SELECTOR_TX_SEL_BITMAP_UPDATE_MASTER0_FIELD_MASTER0_BIT_TYPE_SEL_MASK                               0x01F00000
//========================================
/* REG_TX_SELECTOR_TX_SEL_BITMAP_UPDATE_DATA_MASTER0 0x8 */
#define B0_TX_SELECTOR_TX_SEL_BITMAP_UPDATE_DATA_MASTER0_FIELD_MASTER0_TID_WRITE_EN_MASK                          0x000001FF
#define B0_TX_SELECTOR_TX_SEL_BITMAP_UPDATE_DATA_MASTER0_FIELD_MASTER0_TID_WRITE_VALUE_MASK                       0x0003FE00
#define B0_TX_SELECTOR_TX_SEL_BITMAP_UPDATE_DATA_MASTER0_FIELD_MASTER0_STA_WRITE_DATA_MASK                        0x1FFC0000
//========================================
/* REG_TX_SELECTOR_TX_SEL_BITMAP_UPDATE_MASTER1 0xC */
#define B0_TX_SELECTOR_TX_SEL_BITMAP_UPDATE_MASTER1_FIELD_MASTER1_STA_SEL_MASK                                    0x000000FF
#define B0_TX_SELECTOR_TX_SEL_BITMAP_UPDATE_MASTER1_FIELD_MASTER1_BITMAP_SEL_MASK                                 0x00070000
#define B0_TX_SELECTOR_TX_SEL_BITMAP_UPDATE_MASTER1_FIELD_MASTER1_BIT_TYPE_SEL_MASK                               0x01F00000
//========================================
/* REG_TX_SELECTOR_TX_SEL_BITMAP_UPDATE_DATA_MASTER1 0x10 */
#define B0_TX_SELECTOR_TX_SEL_BITMAP_UPDATE_DATA_MASTER1_FIELD_MASTER1_TID_WRITE_EN_MASK                          0x000001FF
#define B0_TX_SELECTOR_TX_SEL_BITMAP_UPDATE_DATA_MASTER1_FIELD_MASTER1_TID_WRITE_VALUE_MASK                       0x0003FE00
#define B0_TX_SELECTOR_TX_SEL_BITMAP_UPDATE_DATA_MASTER1_FIELD_MASTER1_STA_WRITE_DATA_MASK                        0x1FFC0000
//========================================
/* REG_TX_SELECTOR_TX_SEL_BITMAP_UPDATE_MASTER2 0x14 */
#define B0_TX_SELECTOR_TX_SEL_BITMAP_UPDATE_MASTER2_FIELD_MASTER2_STA_SEL_MASK                                    0x000000FF
#define B0_TX_SELECTOR_TX_SEL_BITMAP_UPDATE_MASTER2_FIELD_MASTER2_BITMAP_SEL_MASK                                 0x00070000
#define B0_TX_SELECTOR_TX_SEL_BITMAP_UPDATE_MASTER2_FIELD_MASTER2_BIT_TYPE_SEL_MASK                               0x01F00000
//========================================
/* REG_TX_SELECTOR_TX_SEL_BITMAP_UPDATE_DATA_MASTER2 0x18 */
#define B0_TX_SELECTOR_TX_SEL_BITMAP_UPDATE_DATA_MASTER2_FIELD_MASTER2_TID_WRITE_EN_MASK                          0x000001FF
#define B0_TX_SELECTOR_TX_SEL_BITMAP_UPDATE_DATA_MASTER2_FIELD_MASTER2_TID_WRITE_VALUE_MASK                       0x0003FE00
#define B0_TX_SELECTOR_TX_SEL_BITMAP_UPDATE_DATA_MASTER2_FIELD_MASTER2_STA_WRITE_DATA_MASK                        0x1FFC0000
//========================================
/* REG_TX_SELECTOR_TX_SEL_BITMAP_UPDATE_MASTER3 0x1C */
#define B0_TX_SELECTOR_TX_SEL_BITMAP_UPDATE_MASTER3_FIELD_MASTER3_STA_SEL_MASK                                    0x000000FF
#define B0_TX_SELECTOR_TX_SEL_BITMAP_UPDATE_MASTER3_FIELD_MASTER3_BITMAP_SEL_MASK                                 0x00070000
#define B0_TX_SELECTOR_TX_SEL_BITMAP_UPDATE_MASTER3_FIELD_MASTER3_BIT_TYPE_SEL_MASK                               0x01F00000
//========================================
/* REG_TX_SELECTOR_TX_SEL_BITMAP_UPDATE_DATA_MASTER3 0x20 */
#define B0_TX_SELECTOR_TX_SEL_BITMAP_UPDATE_DATA_MASTER3_FIELD_MASTER3_TID_WRITE_EN_MASK                          0x000001FF
#define B0_TX_SELECTOR_TX_SEL_BITMAP_UPDATE_DATA_MASTER3_FIELD_MASTER3_TID_WRITE_VALUE_MASK                       0x0003FE00
#define B0_TX_SELECTOR_TX_SEL_BITMAP_UPDATE_DATA_MASTER3_FIELD_MASTER3_STA_WRITE_DATA_MASK                        0x1FFC0000
//========================================
/* REG_TX_SELECTOR_TX_SEL_STA_PS_WRR_LOW0 0x24 */
#define B0_TX_SELECTOR_TX_SEL_STA_PS_WRR_LOW0_FIELD_PS_LOW_VAP0_PRIO_MASK                                         0x00000007
#define B0_TX_SELECTOR_TX_SEL_STA_PS_WRR_LOW0_FIELD_PS_LOW_VAP1_PRIO_MASK                                         0x00000038
#define B0_TX_SELECTOR_TX_SEL_STA_PS_WRR_LOW0_FIELD_PS_LOW_VAP2_PRIO_MASK                                         0x000001C0
#define B0_TX_SELECTOR_TX_SEL_STA_PS_WRR_LOW0_FIELD_PS_LOW_VAP3_PRIO_MASK                                         0x00000E00
#define B0_TX_SELECTOR_TX_SEL_STA_PS_WRR_LOW0_FIELD_PS_LOW_VAP4_PRIO_MASK                                         0x00007000
#define B0_TX_SELECTOR_TX_SEL_STA_PS_WRR_LOW0_FIELD_PS_LOW_VAP5_PRIO_MASK                                         0x00038000
#define B0_TX_SELECTOR_TX_SEL_STA_PS_WRR_LOW0_FIELD_PS_LOW_VAP6_PRIO_MASK                                         0x001C0000
#define B0_TX_SELECTOR_TX_SEL_STA_PS_WRR_LOW0_FIELD_PS_LOW_VAP7_PRIO_MASK                                         0x00E00000
//========================================
/* REG_TX_SELECTOR_TX_SEL_STA_PS_WRR_LOW1 0x28 */
#define B0_TX_SELECTOR_TX_SEL_STA_PS_WRR_LOW1_FIELD_PS_LOW_VAP8_PRIO_MASK                                         0x00000007
#define B0_TX_SELECTOR_TX_SEL_STA_PS_WRR_LOW1_FIELD_PS_LOW_VAP9_PRIO_MASK                                         0x00000038
#define B0_TX_SELECTOR_TX_SEL_STA_PS_WRR_LOW1_FIELD_PS_LOW_VAP10_PRIO_MASK                                        0x000001C0
#define B0_TX_SELECTOR_TX_SEL_STA_PS_WRR_LOW1_FIELD_PS_LOW_VAP11_PRIO_MASK                                        0x00000E00
#define B0_TX_SELECTOR_TX_SEL_STA_PS_WRR_LOW1_FIELD_PS_LOW_VAP12_PRIO_MASK                                        0x00007000
#define B0_TX_SELECTOR_TX_SEL_STA_PS_WRR_LOW1_FIELD_PS_LOW_VAP13_PRIO_MASK                                        0x00038000
#define B0_TX_SELECTOR_TX_SEL_STA_PS_WRR_LOW1_FIELD_PS_LOW_VAP14_PRIO_MASK                                        0x001C0000
#define B0_TX_SELECTOR_TX_SEL_STA_PS_WRR_LOW1_FIELD_PS_LOW_VAP15_PRIO_MASK                                        0x00E00000
//========================================
/* REG_TX_SELECTOR_TX_SEL_STA_PS_WRR_HIGH0 0x2C */
#define B0_TX_SELECTOR_TX_SEL_STA_PS_WRR_HIGH0_FIELD_PS_HIGH_VAP16_PRIO_MASK                                      0x00000007
#define B0_TX_SELECTOR_TX_SEL_STA_PS_WRR_HIGH0_FIELD_PS_HIGH_VAP17_PRIO_MASK                                      0x00000038
#define B0_TX_SELECTOR_TX_SEL_STA_PS_WRR_HIGH0_FIELD_PS_HIGH_VAP18_PRIO_MASK                                      0x000001C0
#define B0_TX_SELECTOR_TX_SEL_STA_PS_WRR_HIGH0_FIELD_PS_HIGH_VAP19_PRIO_MASK                                      0x00000E00
#define B0_TX_SELECTOR_TX_SEL_STA_PS_WRR_HIGH0_FIELD_PS_HIGH_VAP20_PRIO_MASK                                      0x00007000
#define B0_TX_SELECTOR_TX_SEL_STA_PS_WRR_HIGH0_FIELD_PS_HIGH_VAP21_PRIO_MASK                                      0x00038000
#define B0_TX_SELECTOR_TX_SEL_STA_PS_WRR_HIGH0_FIELD_PS_HIGH_VAP22_PRIO_MASK                                      0x001C0000
#define B0_TX_SELECTOR_TX_SEL_STA_PS_WRR_HIGH0_FIELD_PS_HIGH_VAP23_PRIO_MASK                                      0x00E00000
//========================================
/* REG_TX_SELECTOR_TX_SEL_STA_PS_WRR_HIGH1 0x30 */
#define B0_TX_SELECTOR_TX_SEL_STA_PS_WRR_HIGH1_FIELD_PS_HIGH_VAP24_PRIO_MASK                                      0x00000007
#define B0_TX_SELECTOR_TX_SEL_STA_PS_WRR_HIGH1_FIELD_PS_HIGH_VAP25_PRIO_MASK                                      0x00000038
#define B0_TX_SELECTOR_TX_SEL_STA_PS_WRR_HIGH1_FIELD_PS_HIGH_VAP26_PRIO_MASK                                      0x000001C0
#define B0_TX_SELECTOR_TX_SEL_STA_PS_WRR_HIGH1_FIELD_PS_HIGH_VAP27_PRIO_MASK                                      0x00000E00
#define B0_TX_SELECTOR_TX_SEL_STA_PS_WRR_HIGH1_FIELD_PS_HIGH_VAP28_PRIO_MASK                                      0x00007000
#define B0_TX_SELECTOR_TX_SEL_STA_PS_WRR_HIGH1_FIELD_PS_HIGH_VAP29_PRIO_MASK                                      0x00038000
#define B0_TX_SELECTOR_TX_SEL_STA_PS_WRR_HIGH1_FIELD_PS_HIGH_VAP30_PRIO_MASK                                      0x001C0000
#define B0_TX_SELECTOR_TX_SEL_STA_PS_WRR_HIGH1_FIELD_PS_HIGH_VAP31_PRIO_MASK                                      0x00E00000
//========================================
/* REG_TX_SELECTOR_TX_SEL_STA_ACTIVE_WRR_LOW0 0x34 */
#define B0_TX_SELECTOR_TX_SEL_STA_ACTIVE_WRR_LOW0_FIELD_ACTIVE_LOW_VAP0_PRIO_MASK                                 0x00000007
#define B0_TX_SELECTOR_TX_SEL_STA_ACTIVE_WRR_LOW0_FIELD_ACTIVE_LOW_VAP1_PRIO_MASK                                 0x00000038
#define B0_TX_SELECTOR_TX_SEL_STA_ACTIVE_WRR_LOW0_FIELD_ACTIVE_LOW_VAP2_PRIO_MASK                                 0x000001C0
#define B0_TX_SELECTOR_TX_SEL_STA_ACTIVE_WRR_LOW0_FIELD_ACTIVE_LOW_VAP3_PRIO_MASK                                 0x00000E00
#define B0_TX_SELECTOR_TX_SEL_STA_ACTIVE_WRR_LOW0_FIELD_ACTIVE_LOW_VAP4_PRIO_MASK                                 0x00007000
#define B0_TX_SELECTOR_TX_SEL_STA_ACTIVE_WRR_LOW0_FIELD_ACTIVE_LOW_VAP5_PRIO_MASK                                 0x00038000
#define B0_TX_SELECTOR_TX_SEL_STA_ACTIVE_WRR_LOW0_FIELD_ACTIVE_LOW_VAP6_PRIO_MASK                                 0x001C0000
#define B0_TX_SELECTOR_TX_SEL_STA_ACTIVE_WRR_LOW0_FIELD_ACTIVE_LOW_VAP7_PRIO_MASK                                 0x00E00000
//========================================
/* REG_TX_SELECTOR_TX_SEL_STA_ACTIVE_WRR_LOW1 0x38 */
#define B0_TX_SELECTOR_TX_SEL_STA_ACTIVE_WRR_LOW1_FIELD_ACTIVE_LOW_VAP8_PRIO_MASK                                 0x00000007
#define B0_TX_SELECTOR_TX_SEL_STA_ACTIVE_WRR_LOW1_FIELD_ACTIVE_LOW_VAP9_PRIO_MASK                                 0x00000038
#define B0_TX_SELECTOR_TX_SEL_STA_ACTIVE_WRR_LOW1_FIELD_ACTIVE_LOW_VAP10_PRIO_MASK                                0x000001C0
#define B0_TX_SELECTOR_TX_SEL_STA_ACTIVE_WRR_LOW1_FIELD_ACTIVE_LOW_VAP11_PRIO_MASK                                0x00000E00
#define B0_TX_SELECTOR_TX_SEL_STA_ACTIVE_WRR_LOW1_FIELD_ACTIVE_LOW_VAP12_PRIO_MASK                                0x00007000
#define B0_TX_SELECTOR_TX_SEL_STA_ACTIVE_WRR_LOW1_FIELD_ACTIVE_LOW_VAP13_PRIO_MASK                                0x00038000
#define B0_TX_SELECTOR_TX_SEL_STA_ACTIVE_WRR_LOW1_FIELD_ACTIVE_LOW_VAP14_PRIO_MASK                                0x001C0000
#define B0_TX_SELECTOR_TX_SEL_STA_ACTIVE_WRR_LOW1_FIELD_ACTIVE_LOW_VAP15_PRIO_MASK                                0x00E00000
//========================================
/* REG_TX_SELECTOR_TX_SEL_STA_ACTIVE_WRR_HIGH0 0x3C */
#define B0_TX_SELECTOR_TX_SEL_STA_ACTIVE_WRR_HIGH0_FIELD_ACTIVE_HIGH_VAP16_PRIO_MASK                              0x00000007
#define B0_TX_SELECTOR_TX_SEL_STA_ACTIVE_WRR_HIGH0_FIELD_ACTIVE_HIGH_VAP17_PRIO_MASK                              0x00000038
#define B0_TX_SELECTOR_TX_SEL_STA_ACTIVE_WRR_HIGH0_FIELD_ACTIVE_HIGH_VAP18_PRIO_MASK                              0x000001C0
#define B0_TX_SELECTOR_TX_SEL_STA_ACTIVE_WRR_HIGH0_FIELD_ACTIVE_HIGH_VAP19_PRIO_MASK                              0x00000E00
#define B0_TX_SELECTOR_TX_SEL_STA_ACTIVE_WRR_HIGH0_FIELD_ACTIVE_HIGH_VAP20_PRIO_MASK                              0x00007000
#define B0_TX_SELECTOR_TX_SEL_STA_ACTIVE_WRR_HIGH0_FIELD_ACTIVE_HIGH_VAP21_PRIO_MASK                              0x00038000
#define B0_TX_SELECTOR_TX_SEL_STA_ACTIVE_WRR_HIGH0_FIELD_ACTIVE_HIGH_VAP22_PRIO_MASK                              0x001C0000
#define B0_TX_SELECTOR_TX_SEL_STA_ACTIVE_WRR_HIGH0_FIELD_ACTIVE_HIGH_VAP23_PRIO_MASK                              0x00E00000
//========================================
/* REG_TX_SELECTOR_TX_SEL_STA_ACTIVE_WRR_HIGH1 0x40 */
#define B0_TX_SELECTOR_TX_SEL_STA_ACTIVE_WRR_HIGH1_FIELD_ACTIVE_HIGH_VAP24_PRIO_MASK                              0x00000007
#define B0_TX_SELECTOR_TX_SEL_STA_ACTIVE_WRR_HIGH1_FIELD_ACTIVE_HIGH_VAP25_PRIO_MASK                              0x00000038
#define B0_TX_SELECTOR_TX_SEL_STA_ACTIVE_WRR_HIGH1_FIELD_ACTIVE_HIGH_VAP26_PRIO_MASK                              0x000001C0
#define B0_TX_SELECTOR_TX_SEL_STA_ACTIVE_WRR_HIGH1_FIELD_ACTIVE_HIGH_VAP27_PRIO_MASK                              0x00000E00
#define B0_TX_SELECTOR_TX_SEL_STA_ACTIVE_WRR_HIGH1_FIELD_ACTIVE_HIGH_VAP28_PRIO_MASK                              0x00007000
#define B0_TX_SELECTOR_TX_SEL_STA_ACTIVE_WRR_HIGH1_FIELD_ACTIVE_HIGH_VAP29_PRIO_MASK                              0x00038000
#define B0_TX_SELECTOR_TX_SEL_STA_ACTIVE_WRR_HIGH1_FIELD_ACTIVE_HIGH_VAP30_PRIO_MASK                              0x001C0000
#define B0_TX_SELECTOR_TX_SEL_STA_ACTIVE_WRR_HIGH1_FIELD_ACTIVE_HIGH_VAP31_PRIO_MASK                              0x00E00000
//========================================
/* REG_TX_SELECTOR_TX_SEL_STRICT_PRIO 0x44 */
#define B0_TX_SELECTOR_TX_SEL_STRICT_PRIO_FIELD_STRICT_PRIO_GROUP_SEL_EN_MASK                                     0x0000003F
#define B0_TX_SELECTOR_TX_SEL_STRICT_PRIO_FIELD_STRICT_PRIO_WRR_PRIO_MASK                                         0x000000C0
#define B0_TX_SELECTOR_TX_SEL_STRICT_PRIO_FIELD_STRICT_PRIO_VAP_MPS_PRIO_MASK                                     0x00000300
#define B0_TX_SELECTOR_TX_SEL_STRICT_PRIO_FIELD_STRICT_PRIO_GLOBAL_PRIO_MASK                                      0x00000C00
#define B0_TX_SELECTOR_TX_SEL_STRICT_PRIO_FIELD_STRICT_PRIO_SELECTOR_LOCK_EN_MASK                                 0x00001000
//========================================
/* REG_TX_SELECTOR_TX_SEL_SELECTION 0x48 */
#define B0_TX_SELECTOR_TX_SEL_SELECTION_FIELD_SELECTION_TID_NUM_MASK                                              0x0000000F
#define B0_TX_SELECTOR_TX_SEL_SELECTION_FIELD_SELECTION_STA_NUM_MASK                                              0x00000FF0
#define B0_TX_SELECTOR_TX_SEL_SELECTION_FIELD_SELECTION_VAP_NUM_MASK                                              0x0001F000
#define B0_TX_SELECTOR_TX_SEL_SELECTION_FIELD_SELECTION_AC_NUM_MASK                                               0x000C0000
#define B0_TX_SELECTOR_TX_SEL_SELECTION_FIELD_SELECTION_TXQ_ID_MASK                                               0x00700000
#define B0_TX_SELECTOR_TX_SEL_SELECTION_FIELD_SELECTION_SRC_NUM_MASK                                              0x07000000
#define B0_TX_SELECTOR_TX_SEL_SELECTION_FIELD_SELECTION_BF_MASK                                                   0x30000000
#define B0_TX_SELECTOR_TX_SEL_SELECTION_FIELD_SELECTION_EMPTY_MASK                                                0x40000000
#define B0_TX_SELECTOR_TX_SEL_SELECTION_FIELD_SELECTION_VALID_MASK                                                0x80000000
//========================================
/* REG_TX_SELECTOR_TX_SEL_SELECTED_BITMAP 0x4C */
#define B0_TX_SELECTOR_TX_SEL_SELECTED_BITMAP_FIELD_SELECTED_BITMAP_REQ0_MASK                                     0x00000001
#define B0_TX_SELECTOR_TX_SEL_SELECTED_BITMAP_FIELD_SELECTED_BITMAP_PS_TYPE0_MASK                                 0x00000002
#define B0_TX_SELECTOR_TX_SEL_SELECTED_BITMAP_FIELD_SELECTED_BITMAP_PS_MODE0_MASK                                 0x00000004
#define B0_TX_SELECTOR_TX_SEL_SELECTED_BITMAP_FIELD_SELECTED_BITMAP_DATA_IN_Q0_MASK                               0x00000008
#define B0_TX_SELECTOR_TX_SEL_SELECTED_BITMAP_FIELD_SELECTED_BITMAP_REQ1_MASK                                     0x00000010
#define B0_TX_SELECTOR_TX_SEL_SELECTED_BITMAP_FIELD_SELECTED_BITMAP_PS_TYPE1_MASK                                 0x00000020
#define B0_TX_SELECTOR_TX_SEL_SELECTED_BITMAP_FIELD_SELECTED_BITMAP_PS_MODE1_MASK                                 0x00000040
#define B0_TX_SELECTOR_TX_SEL_SELECTED_BITMAP_FIELD_SELECTED_BITMAP_DATA_IN_Q1_MASK                               0x00000080
#define B0_TX_SELECTOR_TX_SEL_SELECTED_BITMAP_FIELD_SELECTED_BITMAP_REQ2_MASK                                     0x00000100
#define B0_TX_SELECTOR_TX_SEL_SELECTED_BITMAP_FIELD_SELECTED_BITMAP_PS_TYPE2_MASK                                 0x00000200
#define B0_TX_SELECTOR_TX_SEL_SELECTED_BITMAP_FIELD_SELECTED_BITMAP_PS_MODE2_MASK                                 0x00000400
#define B0_TX_SELECTOR_TX_SEL_SELECTED_BITMAP_FIELD_SELECTED_BITMAP_DATA_IN_Q2_MASK                               0x00000800
#define B0_TX_SELECTOR_TX_SEL_SELECTED_BITMAP_FIELD_SELECTED_BITMAP_REQ3_MASK                                     0x00001000
#define B0_TX_SELECTOR_TX_SEL_SELECTED_BITMAP_FIELD_SELECTED_BITMAP_PS_TYPE3_MASK                                 0x00002000
#define B0_TX_SELECTOR_TX_SEL_SELECTED_BITMAP_FIELD_SELECTED_BITMAP_PS_MODE3_MASK                                 0x00004000
#define B0_TX_SELECTOR_TX_SEL_SELECTED_BITMAP_FIELD_SELECTED_BITMAP_DATA_IN_Q3_MASK                               0x00008000
#define B0_TX_SELECTOR_TX_SEL_SELECTED_BITMAP_FIELD_SELECTED_BITMAP_REQ4_MASK                                     0x00010000
#define B0_TX_SELECTOR_TX_SEL_SELECTED_BITMAP_FIELD_SELECTED_BITMAP_PS_TYPE4_MASK                                 0x00020000
#define B0_TX_SELECTOR_TX_SEL_SELECTED_BITMAP_FIELD_SELECTED_BITMAP_PS_MODE4_MASK                                 0x00040000
#define B0_TX_SELECTOR_TX_SEL_SELECTED_BITMAP_FIELD_SELECTED_BITMAP_DATA_IN_Q4_MASK                               0x00080000
#define B0_TX_SELECTOR_TX_SEL_SELECTED_BITMAP_FIELD_SELECTED_BITMAP_REQ5_MASK                                     0x00100000
#define B0_TX_SELECTOR_TX_SEL_SELECTED_BITMAP_FIELD_SELECTED_BITMAP_PS_TYPE5_MASK                                 0x00200000
#define B0_TX_SELECTOR_TX_SEL_SELECTED_BITMAP_FIELD_SELECTED_BITMAP_PS_MODE5_MASK                                 0x00400000
#define B0_TX_SELECTOR_TX_SEL_SELECTED_BITMAP_FIELD_SELECTED_BITMAP_DATA_IN_Q5_MASK                               0x00800000
#define B0_TX_SELECTOR_TX_SEL_SELECTED_BITMAP_FIELD_SELECTED_BITMAP_REQ6_MASK                                     0x01000000
#define B0_TX_SELECTOR_TX_SEL_SELECTED_BITMAP_FIELD_SELECTED_BITMAP_PS_TYPE6_MASK                                 0x02000000
#define B0_TX_SELECTOR_TX_SEL_SELECTED_BITMAP_FIELD_SELECTED_BITMAP_PS_MODE6_MASK                                 0x04000000
#define B0_TX_SELECTOR_TX_SEL_SELECTED_BITMAP_FIELD_SELECTED_BITMAP_DATA_IN_Q6_MASK                               0x08000000
#define B0_TX_SELECTOR_TX_SEL_SELECTED_BITMAP_FIELD_SELECTED_BITMAP_REQ7_MASK                                     0x10000000
#define B0_TX_SELECTOR_TX_SEL_SELECTED_BITMAP_FIELD_SELECTED_BITMAP_PS_TYPE7_MASK                                 0x20000000
#define B0_TX_SELECTOR_TX_SEL_SELECTED_BITMAP_FIELD_SELECTED_BITMAP_PS_MODE7_MASK                                 0x40000000
#define B0_TX_SELECTOR_TX_SEL_SELECTED_BITMAP_FIELD_SELECTED_BITMAP_DATA_IN_Q7_MASK                               0x80000000
//========================================
/* REG_TX_SELECTOR_TX_SEL_SELECTED_BITMAP_MGMT 0x50 */
#define B0_TX_SELECTOR_TX_SEL_SELECTED_BITMAP_MGMT_FIELD_SELECTED_BITMAP_REQ8_MASK                                0x00000001
#define B0_TX_SELECTOR_TX_SEL_SELECTED_BITMAP_MGMT_FIELD_SELECTED_BITMAP_PS_TYPE8_MASK                            0x00000002
#define B0_TX_SELECTOR_TX_SEL_SELECTED_BITMAP_MGMT_FIELD_SELECTED_BITMAP_PS_MODE8_MASK                            0x00000004
#define B0_TX_SELECTOR_TX_SEL_SELECTED_BITMAP_MGMT_FIELD_SELECTED_BITMAP_DATA_IN_Q8_MASK                          0x00000008
//========================================
/* REG_TX_SELECTOR_TX_SEL_SRC0_SELECTION 0x54 */
#define B0_TX_SELECTOR_TX_SEL_SRC0_SELECTION_FIELD_SRC0_SELECTION_TID_NUM_MASK                                    0x0000000F
#define B0_TX_SELECTOR_TX_SEL_SRC0_SELECTION_FIELD_SRC0_SELECTION_STA_NUM_MASK                                    0x00000FF0
#define B0_TX_SELECTOR_TX_SEL_SRC0_SELECTION_FIELD_SRC0_SELECTION_VAP_NUM_MASK                                    0x0001F000
#define B0_TX_SELECTOR_TX_SEL_SRC0_SELECTION_FIELD_SRC0_SELECTION_TXQ_ID_MASK                                     0x000E0000
#define B0_TX_SELECTOR_TX_SEL_SRC0_SELECTION_FIELD_SRC0_SELECTION_AC_NUM_MASK                                     0x00300000
#define B0_TX_SELECTOR_TX_SEL_SRC0_SELECTION_FIELD_SRC0_SELECTION_BF_MASK                                         0x03000000
#define B0_TX_SELECTOR_TX_SEL_SRC0_SELECTION_FIELD_SRC0_SELECTION_SRC_NUM_MASK                                    0x1C000000
#define B0_TX_SELECTOR_TX_SEL_SRC0_SELECTION_FIELD_SRC0_SELECTION_EMPTY_MASK                                      0x40000000
#define B0_TX_SELECTOR_TX_SEL_SRC0_SELECTION_FIELD_SRC0_SELECTION_VALID_MASK                                      0x80000000
//========================================
/* REG_TX_SELECTOR_TX_SEL_SELECTED_SRC0_BITMAP 0x58 */
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC0_BITMAP_FIELD_SELECTED_SRC0_BITMAP_DATA_IN_Q0_MASK                     0x00000008
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC0_BITMAP_FIELD_SELECTED_SRC0_BITMAP_DATA_IN_Q1_MASK                     0x00000080
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC0_BITMAP_FIELD_SELECTED_SRC0_BITMAP_DATA_IN_Q2_MASK                     0x00000800
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC0_BITMAP_FIELD_SELECTED_SRC0_BITMAP_DATA_IN_Q3_MASK                     0x00008000
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC0_BITMAP_FIELD_SELECTED_SRC0_BITMAP_DATA_IN_Q4_MASK                     0x00080000
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC0_BITMAP_FIELD_SELECTED_SRC0_BITMAP_DATA_IN_Q5_MASK                     0x00800000
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC0_BITMAP_FIELD_SELECTED_SRC0_BITMAP_DATA_IN_Q6_MASK                     0x08000000
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC0_BITMAP_FIELD_SELECTED_SRC0_BITMAP_DATA_IN_Q7_MASK                     0x80000000
//========================================
/* REG_TX_SELECTOR_TX_SEL_SELECTED_SRC0_BITMAP_MGMT 0x5C */
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC0_BITMAP_MGMT_FIELD_SELECTED_SRC0_BITMAP_DATA_IN_Q8_MASK                0x00000008
//========================================
/* REG_TX_SELECTOR_TX_SEL_SRC1_SELECTION 0x60 */
#define B0_TX_SELECTOR_TX_SEL_SRC1_SELECTION_FIELD_SRC1_SELECTION_TID_NUM_MASK                                    0x0000000F
#define B0_TX_SELECTOR_TX_SEL_SRC1_SELECTION_FIELD_SRC1_SELECTION_STA_NUM_MASK                                    0x00000FF0
#define B0_TX_SELECTOR_TX_SEL_SRC1_SELECTION_FIELD_SRC1_SELECTION_VAP_NUM_MASK                                    0x0001F000
#define B0_TX_SELECTOR_TX_SEL_SRC1_SELECTION_FIELD_SRC1_SELECTION_TXQ_ID_MASK                                     0x000E0000
#define B0_TX_SELECTOR_TX_SEL_SRC1_SELECTION_FIELD_SRC1_SELECTION_AC_NUM_MASK                                     0x00300000
#define B0_TX_SELECTOR_TX_SEL_SRC1_SELECTION_FIELD_SRC1_SELECTION_BF_MASK                                         0x03000000
#define B0_TX_SELECTOR_TX_SEL_SRC1_SELECTION_FIELD_SRC1_SELECTION_SRC_NUM_MASK                                    0x1C000000
#define B0_TX_SELECTOR_TX_SEL_SRC1_SELECTION_FIELD_SRC1_SELECTION_EMPTY_MASK                                      0x40000000
#define B0_TX_SELECTOR_TX_SEL_SRC1_SELECTION_FIELD_SRC1_SELECTION_VALID_MASK                                      0x80000000
//========================================
/* REG_TX_SELECTOR_TX_SEL_SELECTED_SRC1_BITMAP 0x64 */
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC1_BITMAP_FIELD_SELECTED_SRC1_BITMAP_REQ0_MASK                           0x00000001
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC1_BITMAP_FIELD_SELECTED_SRC1_BITMAP_PS_TYPE0_MASK                       0x00000002
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC1_BITMAP_FIELD_SELECTED_SRC1_BITMAP_PS_MODE0_MASK                       0x00000004
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC1_BITMAP_FIELD_SELECTED_SRC1_BITMAP_DATA_IN_Q0_MASK                     0x00000008
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC1_BITMAP_FIELD_SELECTED_SRC1_BITMAP_REQ1_MASK                           0x00000010
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC1_BITMAP_FIELD_SELECTED_SRC1_BITMAP_PS_TYPE1_MASK                       0x00000020
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC1_BITMAP_FIELD_SELECTED_SRC1_BITMAP_PS_MODE1_MASK                       0x00000040
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC1_BITMAP_FIELD_SELECTED_SRC1_BITMAP_DATA_IN_Q1_MASK                     0x00000080
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC1_BITMAP_FIELD_SELECTED_SRC1_BITMAP_REQ2_MASK                           0x00000100
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC1_BITMAP_FIELD_SELECTED_SRC1_BITMAP_PS_TYPE2_MASK                       0x00000200
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC1_BITMAP_FIELD_SELECTED_SRC1_BITMAP_PS_MODE2_MASK                       0x00000400
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC1_BITMAP_FIELD_SELECTED_SRC1_BITMAP_DATA_IN_Q2_MASK                     0x00000800
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC1_BITMAP_FIELD_SELECTED_SRC1_BITMAP_REQ3_MASK                           0x00001000
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC1_BITMAP_FIELD_SELECTED_SRC1_BITMAP_PS_TYPE3_MASK                       0x00002000
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC1_BITMAP_FIELD_SELECTED_SRC1_BITMAP_PS_MODE3_MASK                       0x00004000
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC1_BITMAP_FIELD_SELECTED_SRC1_BITMAP_DATA_IN_Q3_MASK                     0x00008000
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC1_BITMAP_FIELD_SELECTED_SRC1_BITMAP_REQ4_MASK                           0x00010000
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC1_BITMAP_FIELD_SELECTED_SRC1_BITMAP_PS_TYPE4_MASK                       0x00020000
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC1_BITMAP_FIELD_SELECTED_SRC1_BITMAP_PS_MODE4_MASK                       0x00040000
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC1_BITMAP_FIELD_SELECTED_SRC1_BITMAP_DATA_IN_Q4_MASK                     0x00080000
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC1_BITMAP_FIELD_SELECTED_SRC1_BITMAP_REQ5_MASK                           0x00100000
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC1_BITMAP_FIELD_SELECTED_SRC1_BITMAP_PS_TYPE5_MASK                       0x00200000
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC1_BITMAP_FIELD_SELECTED_SRC1_BITMAP_PS_MODE5_MASK                       0x00400000
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC1_BITMAP_FIELD_SELECTED_SRC1_BITMAP_DATA_IN_Q5_MASK                     0x00800000
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC1_BITMAP_FIELD_SELECTED_SRC1_BITMAP_REQ6_MASK                           0x01000000
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC1_BITMAP_FIELD_SELECTED_SRC1_BITMAP_PS_TYPE6_MASK                       0x02000000
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC1_BITMAP_FIELD_SELECTED_SRC1_BITMAP_PS_MODE6_MASK                       0x04000000
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC1_BITMAP_FIELD_SELECTED_SRC1_BITMAP_DATA_IN_Q6_MASK                     0x08000000
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC1_BITMAP_FIELD_SELECTED_SRC1_BITMAP_REQ7_MASK                           0x10000000
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC1_BITMAP_FIELD_SELECTED_SRC1_BITMAP_PS_TYPE7_MASK                       0x20000000
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC1_BITMAP_FIELD_SELECTED_SRC1_BITMAP_PS_MODE7_MASK                       0x40000000
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC1_BITMAP_FIELD_SELECTED_SRC1_BITMAP_DATA_IN_Q7_MASK                     0x80000000
//========================================
/* REG_TX_SELECTOR_TX_SEL_SELECTED_SRC1_BITMAP_MGMT 0x68 */
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC1_BITMAP_MGMT_FIELD_SELECTED_SRC1_BITMAP_REQ8_MASK                      0x00000001
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC1_BITMAP_MGMT_FIELD_SELECTED_SRC1_BITMAP_PS_TYPE8_MASK                  0x00000002
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC1_BITMAP_MGMT_FIELD_SELECTED_SRC1_BITMAP_PS_MODE8_MASK                  0x00000004
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC1_BITMAP_MGMT_FIELD_SELECTED_SRC1_BITMAP_DATA_IN_Q8_MASK                0x00000008
//========================================
/* REG_TX_SELECTOR_TX_SEL_SRC2_SELECTION 0x6C */
#define B0_TX_SELECTOR_TX_SEL_SRC2_SELECTION_FIELD_SRC2_SELECTION_TID_NUM_MASK                                    0x0000000F
#define B0_TX_SELECTOR_TX_SEL_SRC2_SELECTION_FIELD_SRC2_SELECTION_STA_NUM_MASK                                    0x00000FF0
#define B0_TX_SELECTOR_TX_SEL_SRC2_SELECTION_FIELD_SRC2_SELECTION_VAP_NUM_MASK                                    0x0001F000
#define B0_TX_SELECTOR_TX_SEL_SRC2_SELECTION_FIELD_SRC2_SELECTION_TXQ_ID_MASK                                     0x000E0000
#define B0_TX_SELECTOR_TX_SEL_SRC2_SELECTION_FIELD_SRC2_SELECTION_AC_NUM_MASK                                     0x00300000
#define B0_TX_SELECTOR_TX_SEL_SRC2_SELECTION_FIELD_SRC2_SELECTION_BF_MASK                                         0x03000000
#define B0_TX_SELECTOR_TX_SEL_SRC2_SELECTION_FIELD_SRC2_SELECTION_SRC_NUM_MASK                                    0x1C000000
#define B0_TX_SELECTOR_TX_SEL_SRC2_SELECTION_FIELD_SRC2_SELECTION_EMPTY_MASK                                      0x40000000
#define B0_TX_SELECTOR_TX_SEL_SRC2_SELECTION_FIELD_SRC2_SELECTION_VALID_MASK                                      0x80000000
//========================================
/* REG_TX_SELECTOR_TX_SEL_SELECTED_SRC2_BITMAP 0x70 */
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC2_BITMAP_FIELD_SELECTED_SRC2_BITMAP_REQ0_MASK                           0x00000001
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC2_BITMAP_FIELD_SELECTED_SRC2_BITMAP_PS_TYPE0_MASK                       0x00000002
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC2_BITMAP_FIELD_SELECTED_SRC2_BITMAP_PS_MODE0_MASK                       0x00000004
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC2_BITMAP_FIELD_SELECTED_SRC2_BITMAP_DATA_IN_Q0_MASK                     0x00000008
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC2_BITMAP_FIELD_SELECTED_SRC2_BITMAP_REQ1_MASK                           0x00000010
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC2_BITMAP_FIELD_SELECTED_SRC2_BITMAP_PS_TYPE1_MASK                       0x00000020
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC2_BITMAP_FIELD_SELECTED_SRC2_BITMAP_PS_MODE1_MASK                       0x00000040
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC2_BITMAP_FIELD_SELECTED_SRC2_BITMAP_DATA_IN_Q1_MASK                     0x00000080
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC2_BITMAP_FIELD_SELECTED_SRC2_BITMAP_REQ2_MASK                           0x00000100
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC2_BITMAP_FIELD_SELECTED_SRC2_BITMAP_PS_TYPE2_MASK                       0x00000200
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC2_BITMAP_FIELD_SELECTED_SRC2_BITMAP_PS_MODE2_MASK                       0x00000400
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC2_BITMAP_FIELD_SELECTED_SRC2_BITMAP_DATA_IN_Q2_MASK                     0x00000800
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC2_BITMAP_FIELD_SELECTED_SRC2_BITMAP_REQ3_MASK                           0x00001000
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC2_BITMAP_FIELD_SELECTED_SRC2_BITMAP_PS_TYPE3_MASK                       0x00002000
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC2_BITMAP_FIELD_SELECTED_SRC2_BITMAP_PS_MODE3_MASK                       0x00004000
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC2_BITMAP_FIELD_SELECTED_SRC2_BITMAP_DATA_IN_Q3_MASK                     0x00008000
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC2_BITMAP_FIELD_SELECTED_SRC2_BITMAP_REQ4_MASK                           0x00010000
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC2_BITMAP_FIELD_SELECTED_SRC2_BITMAP_PS_TYPE4_MASK                       0x00020000
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC2_BITMAP_FIELD_SELECTED_SRC2_BITMAP_PS_MODE4_MASK                       0x00040000
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC2_BITMAP_FIELD_SELECTED_SRC2_BITMAP_DATA_IN_Q4_MASK                     0x00080000
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC2_BITMAP_FIELD_SELECTED_SRC2_BITMAP_REQ5_MASK                           0x00100000
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC2_BITMAP_FIELD_SELECTED_SRC2_BITMAP_PS_TYPE5_MASK                       0x00200000
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC2_BITMAP_FIELD_SELECTED_SRC2_BITMAP_PS_MODE5_MASK                       0x00400000
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC2_BITMAP_FIELD_SELECTED_SRC2_BITMAP_DATA_IN_Q5_MASK                     0x00800000
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC2_BITMAP_FIELD_SELECTED_SRC2_BITMAP_REQ6_MASK                           0x01000000
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC2_BITMAP_FIELD_SELECTED_SRC2_BITMAP_PS_TYPE6_MASK                       0x02000000
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC2_BITMAP_FIELD_SELECTED_SRC2_BITMAP_PS_MODE6_MASK                       0x04000000
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC2_BITMAP_FIELD_SELECTED_SRC2_BITMAP_DATA_IN_Q6_MASK                     0x08000000
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC2_BITMAP_FIELD_SELECTED_SRC2_BITMAP_REQ7_MASK                           0x10000000
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC2_BITMAP_FIELD_SELECTED_SRC2_BITMAP_PS_TYPE7_MASK                       0x20000000
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC2_BITMAP_FIELD_SELECTED_SRC2_BITMAP_PS_MODE7_MASK                       0x40000000
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC2_BITMAP_FIELD_SELECTED_SRC2_BITMAP_DATA_IN_Q7_MASK                     0x80000000
//========================================
/* REG_TX_SELECTOR_TX_SEL_SELECTED_SRC2_BITMAP_MGMT 0x74 */
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC2_BITMAP_MGMT_FIELD_SELECTED_SRC2_BITMAP_REQ8_MASK                      0x00000001
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC2_BITMAP_MGMT_FIELD_SELECTED_SRC2_BITMAP_PS_TYPE8_MASK                  0x00000002
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC2_BITMAP_MGMT_FIELD_SELECTED_SRC2_BITMAP_PS_MODE8_MASK                  0x00000004
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC2_BITMAP_MGMT_FIELD_SELECTED_SRC2_BITMAP_DATA_IN_Q8_MASK                0x00000008
//========================================
/* REG_TX_SELECTOR_TX_SEL_SRC3_SELECTION 0x78 */
#define B0_TX_SELECTOR_TX_SEL_SRC3_SELECTION_FIELD_SRC3_SELECTION_TID_NUM_MASK                                    0x0000000F
#define B0_TX_SELECTOR_TX_SEL_SRC3_SELECTION_FIELD_SRC3_SELECTION_STA_NUM_MASK                                    0x00000FF0
#define B0_TX_SELECTOR_TX_SEL_SRC3_SELECTION_FIELD_SRC3_SELECTION_VAP_NUM_MASK                                    0x0001F000
#define B0_TX_SELECTOR_TX_SEL_SRC3_SELECTION_FIELD_SRC3_SELECTION_TXQ_ID_MASK                                     0x000E0000
#define B0_TX_SELECTOR_TX_SEL_SRC3_SELECTION_FIELD_SRC3_SELECTION_AC_NUM_MASK                                     0x00300000
#define B0_TX_SELECTOR_TX_SEL_SRC3_SELECTION_FIELD_SRC3_SELECTION_BF_MASK                                         0x03000000
#define B0_TX_SELECTOR_TX_SEL_SRC3_SELECTION_FIELD_SRC3_SELECTION_SRC_NUM_MASK                                    0x1C000000
#define B0_TX_SELECTOR_TX_SEL_SRC3_SELECTION_FIELD_SRC3_SELECTION_EMPTY_MASK                                      0x40000000
#define B0_TX_SELECTOR_TX_SEL_SRC3_SELECTION_FIELD_SRC3_SELECTION_VALID_MASK                                      0x80000000
//========================================
/* REG_TX_SELECTOR_TX_SEL_SELECTED_SRC3_BITMAP 0x7C */
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC3_BITMAP_FIELD_SELECTED_SRC3_BITMAP_REQ0_MASK                           0x00000001
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC3_BITMAP_FIELD_SELECTED_SRC3_BITMAP_PS_TYPE0_MASK                       0x00000002
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC3_BITMAP_FIELD_SELECTED_SRC3_BITMAP_PS_MODE0_MASK                       0x00000004
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC3_BITMAP_FIELD_SELECTED_SRC3_BITMAP_DATA_IN_Q0_MASK                     0x00000008
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC3_BITMAP_FIELD_SELECTED_SRC3_BITMAP_REQ1_MASK                           0x00000010
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC3_BITMAP_FIELD_SELECTED_SRC3_BITMAP_PS_TYPE1_MASK                       0x00000020
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC3_BITMAP_FIELD_SELECTED_SRC3_BITMAP_PS_MODE1_MASK                       0x00000040
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC3_BITMAP_FIELD_SELECTED_SRC3_BITMAP_DATA_IN_Q1_MASK                     0x00000080
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC3_BITMAP_FIELD_SELECTED_SRC3_BITMAP_REQ2_MASK                           0x00000100
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC3_BITMAP_FIELD_SELECTED_SRC3_BITMAP_PS_TYPE2_MASK                       0x00000200
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC3_BITMAP_FIELD_SELECTED_SRC3_BITMAP_PS_MODE2_MASK                       0x00000400
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC3_BITMAP_FIELD_SELECTED_SRC3_BITMAP_DATA_IN_Q2_MASK                     0x00000800
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC3_BITMAP_FIELD_SELECTED_SRC3_BITMAP_REQ3_MASK                           0x00001000
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC3_BITMAP_FIELD_SELECTED_SRC3_BITMAP_PS_TYPE3_MASK                       0x00002000
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC3_BITMAP_FIELD_SELECTED_SRC3_BITMAP_PS_MODE3_MASK                       0x00004000
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC3_BITMAP_FIELD_SELECTED_SRC3_BITMAP_DATA_IN_Q3_MASK                     0x00008000
//========================================
/* REG_TX_SELECTOR_TX_SEL_SRC4_SELECTION 0x80 */
#define B0_TX_SELECTOR_TX_SEL_SRC4_SELECTION_FIELD_SRC4_SELECTION_TID_NUM_MASK                                    0x0000000F
#define B0_TX_SELECTOR_TX_SEL_SRC4_SELECTION_FIELD_SRC4_SELECTION_STA_NUM_MASK                                    0x00000FF0
#define B0_TX_SELECTOR_TX_SEL_SRC4_SELECTION_FIELD_SRC4_SELECTION_VAP_NUM_MASK                                    0x0001F000
#define B0_TX_SELECTOR_TX_SEL_SRC4_SELECTION_FIELD_SRC4_SELECTION_TXQ_ID_MASK                                     0x000E0000
#define B0_TX_SELECTOR_TX_SEL_SRC4_SELECTION_FIELD_SRC4_SELECTION_AC_NUM_MASK                                     0x00300000
#define B0_TX_SELECTOR_TX_SEL_SRC4_SELECTION_FIELD_SRC4_SELECTION_BF_MASK                                         0x03000000
#define B0_TX_SELECTOR_TX_SEL_SRC4_SELECTION_FIELD_SRC4_SELECTION_SRC_NUM_MASK                                    0x1C000000
#define B0_TX_SELECTOR_TX_SEL_SRC4_SELECTION_FIELD_SRC4_SELECTION_EMPTY_MASK                                      0x40000000
#define B0_TX_SELECTOR_TX_SEL_SRC4_SELECTION_FIELD_SRC4_SELECTION_VALID_MASK                                      0x80000000
//========================================
/* REG_TX_SELECTOR_TX_SEL_SELECTED_SRC4_BITMAP 0x84 */
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC4_BITMAP_FIELD_SELECTED_SRC4_BITMAP_DATA_IN_Q_MASK                      0x00000008
//========================================
/* REG_TX_SELECTOR_TX_SEL_SRC5_SELECTION 0x88 */
#define B0_TX_SELECTOR_TX_SEL_SRC5_SELECTION_FIELD_SRC5_SELECTION_TID_NUM_MASK                                    0x0000000F
#define B0_TX_SELECTOR_TX_SEL_SRC5_SELECTION_FIELD_SRC5_SELECTION_STA_NUM_MASK                                    0x00000FF0
#define B0_TX_SELECTOR_TX_SEL_SRC5_SELECTION_FIELD_SRC5_SELECTION_VAP_NUM_MASK                                    0x0001F000
#define B0_TX_SELECTOR_TX_SEL_SRC5_SELECTION_FIELD_SRC5_SELECTION_TXQ_ID_MASK                                     0x000E0000
#define B0_TX_SELECTOR_TX_SEL_SRC5_SELECTION_FIELD_SRC5_SELECTION_AC_NUM_MASK                                     0x00300000
#define B0_TX_SELECTOR_TX_SEL_SRC5_SELECTION_FIELD_SRC5_SELECTION_BF_MASK                                         0x03000000
#define B0_TX_SELECTOR_TX_SEL_SRC5_SELECTION_FIELD_SRC5_SELECTION_SRC_NUM_MASK                                    0x1C000000
#define B0_TX_SELECTOR_TX_SEL_SRC5_SELECTION_FIELD_SRC5_SELECTION_EMPTY_MASK                                      0x40000000
#define B0_TX_SELECTOR_TX_SEL_SRC5_SELECTION_FIELD_SRC5_SELECTION_VALID_MASK                                      0x80000000
//========================================
/* REG_TX_SELECTOR_TX_SEL_SELECTED_SRC5_BITMAP 0x8C */
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC5_BITMAP_FIELD_SELECTED_SRC5_BITMAP_DATA_IN_Q_MASK                      0x00000008
//========================================
/* REG_TX_SELECTOR_TX_SEL_SRC6_SELECTION 0x90 */
#define B0_TX_SELECTOR_TX_SEL_SRC6_SELECTION_FIELD_SRC6_SELECTION_TID_NUM_MASK                                    0x0000000F
#define B0_TX_SELECTOR_TX_SEL_SRC6_SELECTION_FIELD_SRC6_SELECTION_STA_NUM_MASK                                    0x00000FF0
#define B0_TX_SELECTOR_TX_SEL_SRC6_SELECTION_FIELD_SRC6_SELECTION_VAP_NUM_MASK                                    0x0001F000
#define B0_TX_SELECTOR_TX_SEL_SRC6_SELECTION_FIELD_SRC6_SELECTION_TXQ_ID_MASK                                     0x000E0000
#define B0_TX_SELECTOR_TX_SEL_SRC6_SELECTION_FIELD_SRC6_SELECTION_AC_NUM_MASK                                     0x00300000
#define B0_TX_SELECTOR_TX_SEL_SRC6_SELECTION_FIELD_SRC6_SELECTION_BF_MASK                                         0x03000000
#define B0_TX_SELECTOR_TX_SEL_SRC6_SELECTION_FIELD_SRC6_SELECTION_SRC_NUM_MASK                                    0x1C000000
#define B0_TX_SELECTOR_TX_SEL_SRC6_SELECTION_FIELD_SRC6_SELECTION_EMPTY_MASK                                      0x40000000
#define B0_TX_SELECTOR_TX_SEL_SRC6_SELECTION_FIELD_SRC6_SELECTION_VALID_MASK                                      0x80000000
//========================================
/* REG_TX_SELECTOR_TX_SEL_SELECTED_SRC6_BITMAP 0x94 */
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC6_BITMAP_FIELD_SELECTED_SRC6_BITMAP_REQ0_MASK                           0x00000001
#define B0_TX_SELECTOR_TX_SEL_SELECTED_SRC6_BITMAP_FIELD_SELECTED_SRC6_BITMAP_DATA_IN_Q0_MASK                     0x00000008
//========================================
/* REG_TX_SELECTOR_TX_SEL_STATUS 0x98 */
#define B0_TX_SELECTOR_TX_SEL_STATUS_FIELD_STATUS_SW_FIFO_POP_WHILE_EMPTY_MASK                                    0x00000001
#define B0_TX_SELECTOR_TX_SEL_STATUS_FIELD_STATUS_SW_FIFO_PUSH_WHILE_FULL_MASK                                    0x00000002
#define B0_TX_SELECTOR_TX_SEL_STATUS_FIELD_STATUS_SW_FIFO_EMPTY_MASK                                              0x00000004
#define B0_TX_SELECTOR_TX_SEL_STATUS_FIELD_STATUS_SW_FIFO_FULL_MASK                                               0x00000008
#define B0_TX_SELECTOR_TX_SEL_STATUS_FIELD_STATUS_SW_FIFO_HFULL_MASK                                              0x00000010
#define B0_TX_SELECTOR_TX_SEL_STATUS_FIELD_STATUS_DATA_IN_Q_FIFO_EMPTY_MASK                                       0x00000020
#define B0_TX_SELECTOR_TX_SEL_STATUS_FIELD_STATUS_DATA_IN_Q_FIFO_FULL_MASK                                        0x00000040
#define B0_TX_SELECTOR_TX_SEL_STATUS_FIELD_DATA_IN_Q_FIFO_WAS_FULL_MASK                                           0x00000080
#define B0_TX_SELECTOR_TX_SEL_STATUS_FIELD_STATUS_INSTRUCTION_MASK                                                0x00000300
#define B0_TX_SELECTOR_TX_SEL_STATUS_FIELD_STATUS_INSTR_SRC_MASK                                                  0x00000400
//========================================
/* REG_TX_SELECTOR_TX_SEL_ERR 0x9C */
#define B0_TX_SELECTOR_TX_SEL_ERR_FIELD_ERR_SW_INSTR_WHILE_BUSY_MASK                                              0x00000001
#define B0_TX_SELECTOR_TX_SEL_ERR_FIELD_ERR_HW_INSTR_WHILE_BUSY_MASK                                              0x00000002
#define B0_TX_SELECTOR_TX_SEL_ERR_FIELD_ERR_DATA_LOST_MASK                                                        0x00000004
#define B0_TX_SELECTOR_TX_SEL_ERR_FIELD_ERR_DATA_IN_Q_LOST_MASK                                                   0x00100000
#define B0_TX_SELECTOR_TX_SEL_ERR_FIELD_WRONG_BITMAP_ACCESS_ERR_MASK                                              0x01000000
#define B0_TX_SELECTOR_TX_SEL_ERR_FIELD_WRONG_BITMAP_ACCESS_ERR_BITMAP_MASK                                       0x0E000000
#define B0_TX_SELECTOR_TX_SEL_ERR_FIELD_TIM_WRONG_VAP_IRQ_MASK                                                    0x10000000
//========================================
/* REG_TX_SELECTOR_TX_SEL_ERR_CLR 0xA0 */
#define B0_TX_SELECTOR_TX_SEL_ERR_CLR_FIELD_ERR_CLR_SW_INSTR_WHILE_BUSY_CLR_MASK                                  0x00000001
#define B0_TX_SELECTOR_TX_SEL_ERR_CLR_FIELD_ERR_CLR_HW_INSTR_WHILE_BUSY_CLR_MASK                                  0x00000002
#define B0_TX_SELECTOR_TX_SEL_ERR_CLR_FIELD_ERR_CLR_DATA_LOST_CLR_MASK                                            0x00000004
#define B0_TX_SELECTOR_TX_SEL_ERR_CLR_FIELD_ERR_CLR_DATA_IN_Q_LOST_CLR_MASK                                       0x00100000
#define B0_TX_SELECTOR_TX_SEL_ERR_CLR_FIELD_WRONG_BITMAP_ACCESS_ERR_CLR_MASK                                      0x01000000
//========================================
/* REG_TX_SELECTOR_TX_SEL_DL_VAP_AC_MATRIX_0_3 0xA4 */
#define B0_TX_SELECTOR_TX_SEL_DL_VAP_AC_MATRIX_0_3_FIELD_DL_VAP0_AC_MASK                                          0x0000007F
#define B0_TX_SELECTOR_TX_SEL_DL_VAP_AC_MATRIX_0_3_FIELD_DL_VAP1_AC_MASK                                          0x00007F00
#define B0_TX_SELECTOR_TX_SEL_DL_VAP_AC_MATRIX_0_3_FIELD_DL_VAP2_AC_MASK                                          0x007F0000
#define B0_TX_SELECTOR_TX_SEL_DL_VAP_AC_MATRIX_0_3_FIELD_DL_VAP3_AC_MASK                                          0x7F000000
//========================================
/* REG_TX_SELECTOR_TX_SEL_DL_VAP_AC_MATRIX_4_7 0xA8 */
#define B0_TX_SELECTOR_TX_SEL_DL_VAP_AC_MATRIX_4_7_FIELD_DL_VAP4_AC_MASK                                          0x0000007F
#define B0_TX_SELECTOR_TX_SEL_DL_VAP_AC_MATRIX_4_7_FIELD_DL_VAP5_AC_MASK                                          0x00007F00
#define B0_TX_SELECTOR_TX_SEL_DL_VAP_AC_MATRIX_4_7_FIELD_DL_VAP6_AC_MASK                                          0x007F0000
#define B0_TX_SELECTOR_TX_SEL_DL_VAP_AC_MATRIX_4_7_FIELD_DL_VAP7_AC_MASK                                          0x7F000000
//========================================
/* REG_TX_SELECTOR_TX_SEL_DL_VAP_AC_MATRIX_8_11 0xAC */
#define B0_TX_SELECTOR_TX_SEL_DL_VAP_AC_MATRIX_8_11_FIELD_DL_VAP8_AC_MASK                                         0x0000007F
#define B0_TX_SELECTOR_TX_SEL_DL_VAP_AC_MATRIX_8_11_FIELD_DL_VAP9_AC_MASK                                         0x00007F00
#define B0_TX_SELECTOR_TX_SEL_DL_VAP_AC_MATRIX_8_11_FIELD_DL_VAP10_AC_MASK                                        0x007F0000
#define B0_TX_SELECTOR_TX_SEL_DL_VAP_AC_MATRIX_8_11_FIELD_DL_VAP11_AC_MASK                                        0x7F000000
//========================================
/* REG_TX_SELECTOR_TX_SEL_DL_VAP_AC_MATRIX_12_15 0xB0 */
#define B0_TX_SELECTOR_TX_SEL_DL_VAP_AC_MATRIX_12_15_FIELD_DL_VAP12_AC_MASK                                       0x0000007F
#define B0_TX_SELECTOR_TX_SEL_DL_VAP_AC_MATRIX_12_15_FIELD_DL_VAP13_AC_MASK                                       0x00007F00
#define B0_TX_SELECTOR_TX_SEL_DL_VAP_AC_MATRIX_12_15_FIELD_DL_VAP14_AC_MASK                                       0x007F0000
#define B0_TX_SELECTOR_TX_SEL_DL_VAP_AC_MATRIX_12_15_FIELD_DL_VAP15_AC_MASK                                       0x7F000000
//========================================
/* REG_TX_SELECTOR_TX_SEL_DL_VAP_AC_MATRIX_16_19 0xB4 */
#define B0_TX_SELECTOR_TX_SEL_DL_VAP_AC_MATRIX_16_19_FIELD_DL_VAP16_AC_MASK                                       0x0000007F
#define B0_TX_SELECTOR_TX_SEL_DL_VAP_AC_MATRIX_16_19_FIELD_DL_VAP17_AC_MASK                                       0x00007F00
#define B0_TX_SELECTOR_TX_SEL_DL_VAP_AC_MATRIX_16_19_FIELD_DL_VAP18_AC_MASK                                       0x007F0000
#define B0_TX_SELECTOR_TX_SEL_DL_VAP_AC_MATRIX_16_19_FIELD_DL_VAP19_AC_MASK                                       0x7F000000
//========================================
/* REG_TX_SELECTOR_TX_SEL_DL_VAP_AC_MATRIX_20_23 0xB8 */
#define B0_TX_SELECTOR_TX_SEL_DL_VAP_AC_MATRIX_20_23_FIELD_DL_VAP20_AC_MASK                                       0x0000007F
#define B0_TX_SELECTOR_TX_SEL_DL_VAP_AC_MATRIX_20_23_FIELD_DL_VAP21_AC_MASK                                       0x00007F00
#define B0_TX_SELECTOR_TX_SEL_DL_VAP_AC_MATRIX_20_23_FIELD_DL_VAP22_AC_MASK                                       0x007F0000
#define B0_TX_SELECTOR_TX_SEL_DL_VAP_AC_MATRIX_20_23_FIELD_DL_VAP23_AC_MASK                                       0x7F000000
//========================================
/* REG_TX_SELECTOR_TX_SEL_DL_VAP_AC_MATRIX_24_27 0xBC */
#define B0_TX_SELECTOR_TX_SEL_DL_VAP_AC_MATRIX_24_27_FIELD_DL_VAP24_AC_MASK                                       0x0000007F
#define B0_TX_SELECTOR_TX_SEL_DL_VAP_AC_MATRIX_24_27_FIELD_DL_VAP25_AC_MASK                                       0x00007F00
#define B0_TX_SELECTOR_TX_SEL_DL_VAP_AC_MATRIX_24_27_FIELD_DL_VAP26_AC_MASK                                       0x007F0000
#define B0_TX_SELECTOR_TX_SEL_DL_VAP_AC_MATRIX_24_27_FIELD_DL_VAP27_AC_MASK                                       0x7F000000
//========================================
/* REG_TX_SELECTOR_TX_SEL_DL_VAP_AC_MATRIX_28_31 0xC0 */
#define B0_TX_SELECTOR_TX_SEL_DL_VAP_AC_MATRIX_28_31_FIELD_DL_VAP28_AC_MASK                                       0x0000007F
#define B0_TX_SELECTOR_TX_SEL_DL_VAP_AC_MATRIX_28_31_FIELD_DL_VAP29_AC_MASK                                       0x00007F00
#define B0_TX_SELECTOR_TX_SEL_DL_VAP_AC_MATRIX_28_31_FIELD_DL_VAP30_AC_MASK                                       0x007F0000
#define B0_TX_SELECTOR_TX_SEL_DL_VAP_AC_MATRIX_28_31_FIELD_DL_VAP31_AC_MASK                                       0x7F000000
//========================================
/* REG_TX_SELECTOR_TX_SEL_UL_VAP_AC_MATRIX_0_7 0xC4 */
#define B0_TX_SELECTOR_TX_SEL_UL_VAP_AC_MATRIX_0_7_FIELD_UL_VAP0_AC_MASK                                          0x0000000F
#define B0_TX_SELECTOR_TX_SEL_UL_VAP_AC_MATRIX_0_7_FIELD_UL_VAP1_AC_MASK                                          0x000000F0
#define B0_TX_SELECTOR_TX_SEL_UL_VAP_AC_MATRIX_0_7_FIELD_UL_VAP2_AC_MASK                                          0x00000F00
#define B0_TX_SELECTOR_TX_SEL_UL_VAP_AC_MATRIX_0_7_FIELD_UL_VAP3_AC_MASK                                          0x0000F000
#define B0_TX_SELECTOR_TX_SEL_UL_VAP_AC_MATRIX_0_7_FIELD_UL_VAP4_AC_MASK                                          0x000F0000
#define B0_TX_SELECTOR_TX_SEL_UL_VAP_AC_MATRIX_0_7_FIELD_UL_VAP5_AC_MASK                                          0x00F00000
#define B0_TX_SELECTOR_TX_SEL_UL_VAP_AC_MATRIX_0_7_FIELD_UL_VAP6_AC_MASK                                          0x0F000000
#define B0_TX_SELECTOR_TX_SEL_UL_VAP_AC_MATRIX_0_7_FIELD_UL_VAP7_AC_MASK                                          0xF0000000
//========================================
/* REG_TX_SELECTOR_TX_SEL_UL_VAP_AC_MATRIX_8_15 0xC8 */
#define B0_TX_SELECTOR_TX_SEL_UL_VAP_AC_MATRIX_8_15_FIELD_UL_VAP8_AC_MASK                                         0x0000000F
#define B0_TX_SELECTOR_TX_SEL_UL_VAP_AC_MATRIX_8_15_FIELD_UL_VAP9_AC_MASK                                         0x000000F0
#define B0_TX_SELECTOR_TX_SEL_UL_VAP_AC_MATRIX_8_15_FIELD_UL_VAP10_AC_MASK                                        0x00000F00
#define B0_TX_SELECTOR_TX_SEL_UL_VAP_AC_MATRIX_8_15_FIELD_UL_VAP11_AC_MASK                                        0x0000F000
#define B0_TX_SELECTOR_TX_SEL_UL_VAP_AC_MATRIX_8_15_FIELD_UL_VAP12_AC_MASK                                        0x000F0000
#define B0_TX_SELECTOR_TX_SEL_UL_VAP_AC_MATRIX_8_15_FIELD_UL_VAP13_AC_MASK                                        0x00F00000
#define B0_TX_SELECTOR_TX_SEL_UL_VAP_AC_MATRIX_8_15_FIELD_UL_VAP14_AC_MASK                                        0x0F000000
#define B0_TX_SELECTOR_TX_SEL_UL_VAP_AC_MATRIX_8_15_FIELD_UL_VAP15_AC_MASK                                        0xF0000000
//========================================
/* REG_TX_SELECTOR_TX_SEL_UL_VAP_AC_MATRIX_16_23 0xCC */
#define B0_TX_SELECTOR_TX_SEL_UL_VAP_AC_MATRIX_16_23_FIELD_UL_VAP16_AC_MASK                                       0x0000000F
#define B0_TX_SELECTOR_TX_SEL_UL_VAP_AC_MATRIX_16_23_FIELD_UL_VAP17_AC_MASK                                       0x000000F0
#define B0_TX_SELECTOR_TX_SEL_UL_VAP_AC_MATRIX_16_23_FIELD_UL_VAP18_AC_MASK                                       0x00000F00
#define B0_TX_SELECTOR_TX_SEL_UL_VAP_AC_MATRIX_16_23_FIELD_UL_VAP19_AC_MASK                                       0x0000F000
#define B0_TX_SELECTOR_TX_SEL_UL_VAP_AC_MATRIX_16_23_FIELD_UL_VAP20_AC_MASK                                       0x000F0000
#define B0_TX_SELECTOR_TX_SEL_UL_VAP_AC_MATRIX_16_23_FIELD_UL_VAP21_AC_MASK                                       0x00F00000
#define B0_TX_SELECTOR_TX_SEL_UL_VAP_AC_MATRIX_16_23_FIELD_UL_VAP22_AC_MASK                                       0x0F000000
#define B0_TX_SELECTOR_TX_SEL_UL_VAP_AC_MATRIX_16_23_FIELD_UL_VAP23_AC_MASK                                       0xF0000000
//========================================
/* REG_TX_SELECTOR_TX_SEL_UL_VAP_AC_MATRIX_24_31 0xD0 */
#define B0_TX_SELECTOR_TX_SEL_UL_VAP_AC_MATRIX_24_31_FIELD_UL_VAP24_AC_MASK                                       0x0000000F
#define B0_TX_SELECTOR_TX_SEL_UL_VAP_AC_MATRIX_24_31_FIELD_UL_VAP25_AC_MASK                                       0x000000F0
#define B0_TX_SELECTOR_TX_SEL_UL_VAP_AC_MATRIX_24_31_FIELD_UL_VAP26_AC_MASK                                       0x00000F00
#define B0_TX_SELECTOR_TX_SEL_UL_VAP_AC_MATRIX_24_31_FIELD_UL_VAP27_AC_MASK                                       0x0000F000
#define B0_TX_SELECTOR_TX_SEL_UL_VAP_AC_MATRIX_24_31_FIELD_UL_VAP28_AC_MASK                                       0x000F0000
#define B0_TX_SELECTOR_TX_SEL_UL_VAP_AC_MATRIX_24_31_FIELD_UL_VAP29_AC_MASK                                       0x00F00000
#define B0_TX_SELECTOR_TX_SEL_UL_VAP_AC_MATRIX_24_31_FIELD_UL_VAP30_AC_MASK                                       0x0F000000
#define B0_TX_SELECTOR_TX_SEL_UL_VAP_AC_MATRIX_24_31_FIELD_UL_VAP31_AC_MASK                                       0xF0000000
//========================================
/* REG_TX_SELECTOR_TX_SEL_STA_ASSOCIATE 0xD4 */
#define B0_TX_SELECTOR_TX_SEL_STA_ASSOCIATE_FIELD_STA_ASSOCIATE_STATION_MASK                                      0x000000FF
#define B0_TX_SELECTOR_TX_SEL_STA_ASSOCIATE_FIELD_STA_ASSOCIATE_VAP_MASK                                          0x00001F00
#define B0_TX_SELECTOR_TX_SEL_STA_ASSOCIATE_FIELD_STA_ASSOCIATE_VAP_STA_BIT_VALUE_MASK                            0x00010000
#define B0_TX_SELECTOR_TX_SEL_STA_ASSOCIATE_FIELD_VAP_ZERO_MASK                                                   0x01000000
//========================================
/* REG_TX_SELECTOR_TX_SEL_VAP_AC_BYPASS 0xD8 */
#define B0_TX_SELECTOR_TX_SEL_VAP_AC_BYPASS_FIELD_VAP_AC_BYPASS_AC_SEL_MASK                                       0x00000007
#define B0_TX_SELECTOR_TX_SEL_VAP_AC_BYPASS_FIELD_VAP_AC_BYPASS_VAP_SEL_MASK                                      0x00001F00
#define B0_TX_SELECTOR_TX_SEL_VAP_AC_BYPASS_FIELD_VAP_AC_BYPASS_VAILD_MASK                                        0x00010000
//========================================
/* REG_TX_SELECTOR_TX_SEL_VAP_PS_STATE_EN 0xDC */
#define B0_TX_SELECTOR_TX_SEL_VAP_PS_STATE_EN_FIELD_VAP_PS_STATE_VAP_IN_PS_AUTO_EN_MASK                           0x00010000
//========================================
/* REG_TX_SELECTOR_TX_SEL_VAP_PS_STATE 0xE0 */
#define B0_TX_SELECTOR_TX_SEL_VAP_PS_STATE_FIELD_VAP_PS_STATE_VAP_PS_STATE_MASK                                   0xFFFFFFFF
//========================================
/* REG_TX_SELECTOR_TX_SEL_VAP_PS_NEW_IND_PS 0xE4 */
#define B0_TX_SELECTOR_TX_SEL_VAP_PS_NEW_IND_PS_FIELD_VAP_PS_NEW_IND_PS_MASK                                      0xFFFFFFFF
//========================================
/* REG_TX_SELECTOR_TX_SEL_VAP_PS_NEW_IND_ACTIVE 0xE8 */
#define B0_TX_SELECTOR_TX_SEL_VAP_PS_NEW_IND_ACTIVE_FIELD_VAP_PS_NEW_IND_ACTIVE_MASK                              0xFFFFFFFF
//========================================
/* REG_TX_SELECTOR_TX_SEL_STA_ACTIVE_SEL_COUNT 0xEC */
#define B0_TX_SELECTOR_TX_SEL_STA_ACTIVE_SEL_COUNT_FIELD_STA_ACTIVE_COUNT_MASK                                    0x00000FFF
//========================================
/* REG_TX_SELECTOR_TX_SEL_STA_PS_SEL_COUNT 0xF0 */
#define B0_TX_SELECTOR_TX_SEL_STA_PS_SEL_COUNT_FIELD_STA_PS_COUNT_MASK                                            0x00000FFF
//========================================
/* REG_TX_SELECTOR_TX_SEL_VAP_ACTIVE_SEL_COUNT 0xF4 */
#define B0_TX_SELECTOR_TX_SEL_VAP_ACTIVE_SEL_COUNT_FIELD_VAP_ACTIVE_COUNT_MASK                                    0x00000FFF
//========================================
/* REG_TX_SELECTOR_TX_SEL_VAP_PS_SEL_COUNT 0xF8 */
#define B0_TX_SELECTOR_TX_SEL_VAP_PS_SEL_COUNT_FIELD_VAP_PS_COUNT_MASK                                            0x00000FFF
//========================================
/* REG_TX_SELECTOR_TX_SEL_GLOBAL_SEL_COUNT 0xFC */
#define B0_TX_SELECTOR_TX_SEL_GLOBAL_SEL_COUNT_FIELD_GLOBAL_COUNT_MASK                                            0x00000FFF
//========================================
/* REG_TX_SELECTOR_TX_SEL_GPLP_SEL_COUNT 0x100 */
#define B0_TX_SELECTOR_TX_SEL_GPLP_SEL_COUNT_FIELD_GPLP_COUNT_MASK                                                0x00000FFF
//========================================
/* REG_TX_SELECTOR_TX_SEL_GPHP_SEL_COUNT 0x104 */
#define B0_TX_SELECTOR_TX_SEL_GPHP_SEL_COUNT_FIELD_GPHP_COUNT_MASK                                                0x00000FFF
//========================================
/* REG_TX_SELECTOR_TX_SEL_BEACON_SEL_COUNT 0x108 */
#define B0_TX_SELECTOR_TX_SEL_BEACON_SEL_COUNT_FIELD_BEACON_COUNT_MASK                                            0x00000FFF
//========================================
/* REG_TX_SELECTOR_TX_SEL_EMPTY_SEL_COUNT 0x10C */
#define B0_TX_SELECTOR_TX_SEL_EMPTY_SEL_COUNT_FIELD_EMPTY_COUNT_MASK                                              0x00000FFF
//========================================
/* REG_TX_SELECTOR_TX_SEL_COUNT_CLR 0x110 */
#define B0_TX_SELECTOR_TX_SEL_COUNT_CLR_FIELD_COUNTERS_CLR_MASK                                                   0x00000001
//========================================
/* REG_TX_SELECTOR_TX_SEL_LOGGER_CFG 0x114 */
#define B0_TX_SELECTOR_TX_SEL_LOGGER_CFG_FIELD_TX_SEL_LOGGER_SELECTION_LOCKER_PRIORITY_MASK                       0x00000003
#define B0_TX_SELECTOR_TX_SEL_LOGGER_CFG_FIELD_TX_SEL_LOGGER_SELECTION_LOCKER_EN_MASK                             0x00000004
#define B0_TX_SELECTOR_TX_SEL_LOGGER_CFG_FIELD_TX_SEL_LOGGER_SELECTION_LOCKER_SEL_MASK                            0x00000008
#define B0_TX_SELECTOR_TX_SEL_LOGGER_CFG_FIELD_TX_SEL_LOGGER_BITMAP_PRIORITY_MASK                                 0x00000030
#define B0_TX_SELECTOR_TX_SEL_LOGGER_CFG_FIELD_TX_SEL_LOGGER_BITMAP_UPDATE_EN_MASK                                0x00000040
#define B0_TX_SELECTOR_TX_SEL_LOGGER_CFG_FIELD_TX_SEL_LOGGER_MU_LOCKER_PRIORITY_MASK                              0x00000300
#define B0_TX_SELECTOR_TX_SEL_LOGGER_CFG_FIELD_TX_SEL_LOGGER_MU_LOCKER_EN_MASK                                    0x00000400
#define B0_TX_SELECTOR_TX_SEL_LOGGER_CFG_FIELD_TX_SEL_LOGGER_MU_BITMAP_PRIORITY_MASK                              0x00003000
#define B0_TX_SELECTOR_TX_SEL_LOGGER_CFG_FIELD_TX_SEL_LOGGER_MU_BITMAP_UPDATE_EN_MASK                             0x00004000
#define B0_TX_SELECTOR_TX_SEL_LOGGER_CFG_FIELD_TX_SEL_LOGGER_MU_SELECTION_EN_MASK                                 0x00010000
#define B0_TX_SELECTOR_TX_SEL_LOGGER_CFG_FIELD_TX_SEL_LOGGER_BEACON_SELECTION_EN_MASK                             0x00020000
#define B0_TX_SELECTOR_TX_SEL_LOGGER_CFG_FIELD_TX_SEL_LOGGER_PLAN_LOCKER_PRIORITY_MASK                            0x00300000
#define B0_TX_SELECTOR_TX_SEL_LOGGER_CFG_FIELD_TX_SEL_LOGGER_PLAN_LOCKER_EN_MASK                                  0x00400000
//========================================
/* REG_TX_SELECTOR_TX_SEL_LOGGER_BUSY 0x118 */
#define B0_TX_SELECTOR_TX_SEL_LOGGER_BUSY_FIELD_TX_SEL_LOGGER_SELECTION_LOCKER_BUSY_MASK                          0x00000001
#define B0_TX_SELECTOR_TX_SEL_LOGGER_BUSY_FIELD_TX_SEL_LOGGER_MU_LOCKER_BUSY_MASK                                 0x00000010
#define B0_TX_SELECTOR_TX_SEL_LOGGER_BUSY_FIELD_TX_SEL_LOGGER_BITMAP_UPDATE_BUSY_MASK                             0x00000100
#define B0_TX_SELECTOR_TX_SEL_LOGGER_BUSY_FIELD_TX_SEL_LOGGER_MU_BITMAP_UPDATE_BUSY_MASK                          0x00001000
#define B0_TX_SELECTOR_TX_SEL_LOGGER_BUSY_FIELD_TX_SEL_LOGGER_PLAN_LOCKER_BUSY_MASK                               0x00010000
//========================================
/* REG_TX_SELECTOR_TX_SEL_LOCKER_EN 0x1A0 */
#define B0_TX_SELECTOR_TX_SEL_LOCKER_EN_FIELD_LOCKER_EN_MASK                                                      0x00000001
#define B0_TX_SELECTOR_TX_SEL_LOCKER_EN_FIELD_LOCKER_SM_MASK                                                      0x00000300
//========================================
/* REG_TX_SELECTOR_TX_SEL_SW_MASTER0_LOCKER 0x1A4 */
#define B0_TX_SELECTOR_TX_SEL_SW_MASTER0_LOCKER_FIELD_MASTER0_LOCKER_STA_SEL_MASK                                 0x000000FF
#define B0_TX_SELECTOR_TX_SEL_SW_MASTER0_LOCKER_FIELD_MASTER0_LOCKER_TID_SEL_MASK                                 0x00000F00
#define B0_TX_SELECTOR_TX_SEL_SW_MASTER0_LOCKER_FIELD_MASTER0_LOCKER_BITMAP_SEL_MASK                              0x00070000
#define B0_TX_SELECTOR_TX_SEL_SW_MASTER0_LOCKER_FIELD_MASTER0_LOCKER_BIT_TYPE_SEL_MASK                            0x03000000
#define B0_TX_SELECTOR_TX_SEL_SW_MASTER0_LOCKER_FIELD_MASTER0_LOCKER_ALL_TIDS_MASK                                0x80000000
//========================================
/* REG_TX_SELECTOR_TX_SEL_SW_MASTER1_LOCKER 0x1A8 */
#define B0_TX_SELECTOR_TX_SEL_SW_MASTER1_LOCKER_FIELD_MASTER1_LOCKER_STA_SEL_MASK                                 0x000000FF
#define B0_TX_SELECTOR_TX_SEL_SW_MASTER1_LOCKER_FIELD_MASTER1_LOCKER_TID_SEL_MASK                                 0x00000F00
#define B0_TX_SELECTOR_TX_SEL_SW_MASTER1_LOCKER_FIELD_MASTER1_LOCKER_BITMAP_SEL_MASK                              0x00070000
#define B0_TX_SELECTOR_TX_SEL_SW_MASTER1_LOCKER_FIELD_MASTER1_LOCKER_BIT_TYPE_SEL_MASK                            0x03000000
#define B0_TX_SELECTOR_TX_SEL_SW_MASTER1_LOCKER_FIELD_MASTER1_LOCKER_ALL_TIDS_MASK                                0x80000000
//========================================
/* REG_TX_SELECTOR_TX_SEL_SW_MASTER2_LOCKER 0x1AC */
#define B0_TX_SELECTOR_TX_SEL_SW_MASTER2_LOCKER_FIELD_MASTER2_LOCKER_STA_SEL_MASK                                 0x000000FF
#define B0_TX_SELECTOR_TX_SEL_SW_MASTER2_LOCKER_FIELD_MASTER2_LOCKER_TID_SEL_MASK                                 0x00000F00
#define B0_TX_SELECTOR_TX_SEL_SW_MASTER2_LOCKER_FIELD_MASTER2_LOCKER_BITMAP_SEL_MASK                              0x00070000
#define B0_TX_SELECTOR_TX_SEL_SW_MASTER2_LOCKER_FIELD_MASTER2_LOCKER_BIT_TYPE_SEL_MASK                            0x03000000
#define B0_TX_SELECTOR_TX_SEL_SW_MASTER2_LOCKER_FIELD_MASTER2_LOCKER_ALL_TIDS_MASK                                0x80000000
//========================================
/* REG_TX_SELECTOR_TX_SEL_SW_MASTER3_LOCKER 0x1B0 */
#define B0_TX_SELECTOR_TX_SEL_SW_MASTER3_LOCKER_FIELD_MASTER3_LOCKER_STA_SEL_MASK                                 0x000000FF
#define B0_TX_SELECTOR_TX_SEL_SW_MASTER3_LOCKER_FIELD_MASTER3_LOCKER_TID_SEL_MASK                                 0x00000F00
#define B0_TX_SELECTOR_TX_SEL_SW_MASTER3_LOCKER_FIELD_MASTER3_LOCKER_BITMAP_SEL_MASK                              0x00070000
#define B0_TX_SELECTOR_TX_SEL_SW_MASTER3_LOCKER_FIELD_MASTER3_LOCKER_BIT_TYPE_SEL_MASK                            0x03000000
#define B0_TX_SELECTOR_TX_SEL_SW_MASTER3_LOCKER_FIELD_MASTER3_LOCKER_ALL_TIDS_MASK                                0x80000000
//========================================
/* REG_TX_SELECTOR_TX_SEL_SW_MASTER0_LOCKER_STATUS 0x1B4 */
#define B0_TX_SELECTOR_TX_SEL_SW_MASTER0_LOCKER_STATUS_FIELD_SW_MASTER0_REQ_STATUS_MASK                           0x0000001F
#define B0_TX_SELECTOR_TX_SEL_SW_MASTER0_LOCKER_STATUS_FIELD_MASTER0_LOCKER_DONE_INT_MASK                         0x00000100
//========================================
/* REG_TX_SELECTOR_TX_SEL_SW_MASTER1_LOCKER_STATUS 0x1B8 */
#define B0_TX_SELECTOR_TX_SEL_SW_MASTER1_LOCKER_STATUS_FIELD_SW_MASTER1_REQ_STATUS_MASK                           0x0000001F
#define B0_TX_SELECTOR_TX_SEL_SW_MASTER1_LOCKER_STATUS_FIELD_MASTER1_LOCKER_DONE_INT_MASK                         0x00000100
//========================================
/* REG_TX_SELECTOR_TX_SEL_SW_MASTER2_LOCKER_STATUS 0x1BC */
#define B0_TX_SELECTOR_TX_SEL_SW_MASTER2_LOCKER_STATUS_FIELD_SW_MASTER2_REQ_STATUS_MASK                           0x0000001F
#define B0_TX_SELECTOR_TX_SEL_SW_MASTER2_LOCKER_STATUS_FIELD_MASTER2_LOCKER_DONE_INT_MASK                         0x00000100
//========================================
/* REG_TX_SELECTOR_TX_SEL_SW_MASTER3_LOCKER_STATUS 0x1C0 */
#define B0_TX_SELECTOR_TX_SEL_SW_MASTER3_LOCKER_STATUS_FIELD_SW_MASTER3_REQ_STATUS_MASK                           0x0000001F
#define B0_TX_SELECTOR_TX_SEL_SW_MASTER3_LOCKER_STATUS_FIELD_MASTER3_LOCKER_DONE_INT_MASK                         0x00000100
//========================================
/* REG_TX_SELECTOR_TX_SEL_LOCKER_INT_CLR 0x1C4 */
#define B0_TX_SELECTOR_TX_SEL_LOCKER_INT_CLR_FIELD_SW_MASTER0_LOCKER_INT_CLR_MASK                                 0x00000001
#define B0_TX_SELECTOR_TX_SEL_LOCKER_INT_CLR_FIELD_SW_MASTER1_LOCKER_INT_CLR_MASK                                 0x00000002
#define B0_TX_SELECTOR_TX_SEL_LOCKER_INT_CLR_FIELD_SW_MASTER2_LOCKER_INT_CLR_MASK                                 0x00000004
#define B0_TX_SELECTOR_TX_SEL_LOCKER_INT_CLR_FIELD_SW_MASTER3_LOCKER_INT_CLR_MASK                                 0x00000008
//========================================
/* REG_TX_SELECTOR_TX_SEL_LOCKER_PUSH_WHILE_BUSY 0x1C8 */
#define B0_TX_SELECTOR_TX_SEL_LOCKER_PUSH_WHILE_BUSY_FIELD_MASTER0_PUSH_WHILE_BUSY_MASK                           0x00000001
#define B0_TX_SELECTOR_TX_SEL_LOCKER_PUSH_WHILE_BUSY_FIELD_MASTER1_PUSH_WHILE_BUSY_MASK                           0x00000002
#define B0_TX_SELECTOR_TX_SEL_LOCKER_PUSH_WHILE_BUSY_FIELD_MASTER2_PUSH_WHILE_BUSY_MASK                           0x00000004
#define B0_TX_SELECTOR_TX_SEL_LOCKER_PUSH_WHILE_BUSY_FIELD_MASTER3_PUSH_WHILE_BUSY_MASK                           0x00000008
//========================================
/* REG_TX_SELECTOR_TX_SEL_LOCKER_PUSH_WHILE_BUSY_CLR 0x1CC */
#define B0_TX_SELECTOR_TX_SEL_LOCKER_PUSH_WHILE_BUSY_CLR_FIELD_MASTER0_PUSH_WHILE_BUSY_CLR_MASK                   0x00000001
#define B0_TX_SELECTOR_TX_SEL_LOCKER_PUSH_WHILE_BUSY_CLR_FIELD_MASTER1_PUSH_WHILE_BUSY_CLR_MASK                   0x00000002
#define B0_TX_SELECTOR_TX_SEL_LOCKER_PUSH_WHILE_BUSY_CLR_FIELD_MASTER2_PUSH_WHILE_BUSY_CLR_MASK                   0x00000004
#define B0_TX_SELECTOR_TX_SEL_LOCKER_PUSH_WHILE_BUSY_CLR_FIELD_MASTER3_PUSH_WHILE_BUSY_CLR_MASK                   0x00000008
//========================================
/* REG_TX_SELECTOR_TX_SELECTOR_SPARE 0x1D0 */
#define B0_TX_SELECTOR_TX_SELECTOR_SPARE_FIELD_TX_SELECTOR_SPARE_MASK                                             0xFFFFFFFF
//========================================
/* REG_TX_SELECTOR_TX_SEL_MU_SELECT_CFG 0x200 */
#define B0_TX_SELECTOR_TX_SEL_MU_SELECT_CFG_FIELD_MU_EN_MASK                                                      0x00000001
#define B0_TX_SELECTOR_TX_SEL_MU_SELECT_CFG_FIELD_MU_SECONDARY_SEL_MASK                                           0x00000010
#define B0_TX_SELECTOR_TX_SEL_MU_SELECT_CFG_FIELD_MAX_GROUP_NUM_MASK                                              0x00007F00
#define B0_TX_SELECTOR_TX_SEL_MU_SELECT_CFG_FIELD_SELECTOR_MU_LOCK_EN_MASK                                        0x00010000
#define B0_TX_SELECTOR_TX_SEL_MU_SELECT_CFG_FIELD_VHT_MU_SU_SEL_BYPASS_MASK                                       0x00080000
#define B0_TX_SELECTOR_TX_SEL_MU_SELECT_CFG_FIELD_SU_SEL_EN_1SEC_MASK                                             0x00100000
#define B0_TX_SELECTOR_TX_SEL_MU_SELECT_CFG_FIELD_MU_THRESH_1SEC_MASK                                             0x00400000
#define B0_TX_SELECTOR_TX_SEL_MU_SELECT_CFG_FIELD_SU_SEL_EN_2SEC_MASK                                             0x01000000
#define B0_TX_SELECTOR_TX_SEL_MU_SELECT_CFG_FIELD_MU_THRESH_2SEC_MASK                                             0x0C000000
#define B0_TX_SELECTOR_TX_SEL_MU_SELECT_CFG_FIELD_SU_SEL_EN_3SEC_MASK                                             0x10000000
#define B0_TX_SELECTOR_TX_SEL_MU_SELECT_CFG_FIELD_MU_THRESH_3SEC_MASK                                             0xC0000000
//========================================
/* REG_TX_SELECTOR_TX_SEL_MU_BITMAP_UPDATE 0x204 */
#define B0_TX_SELECTOR_TX_SEL_MU_BITMAP_UPDATE_FIELD_MEM_LINE_NUM_MASK                                            0x000000FF
#define B0_TX_SELECTOR_TX_SEL_MU_BITMAP_UPDATE_FIELD_USER_POSITION_IDX_MASK                                       0x00000300
#define B0_TX_SELECTOR_TX_SEL_MU_BITMAP_UPDATE_FIELD_MU_BIT_TYPE_SEL_MASK                                         0x00007000
#define B0_TX_SELECTOR_TX_SEL_MU_BITMAP_UPDATE_FIELD_ALL_GRP_MASK                                                 0x00008000
#define B0_TX_SELECTOR_TX_SEL_MU_BITMAP_UPDATE_FIELD_MU_BITMAP_UPDATE_VALUE_MASK                                  0x00FF0000
#define B0_TX_SELECTOR_TX_SEL_MU_BITMAP_UPDATE_FIELD_ALL_GRP_VALID_UP_MASK                                        0x0F000000
#define B0_TX_SELECTOR_TX_SEL_MU_BITMAP_UPDATE_FIELD_MU_BITMAP_UPDATE_FREE_MASK                                   0x80000000
//========================================
/* REG_TX_SELECTOR_TX_SEL_MU_BITMAP_UPDATE_ALL_STA_WDATA 0x208 */
#define B0_TX_SELECTOR_TX_SEL_MU_BITMAP_UPDATE_ALL_STA_WDATA_FIELD_ALL_GRP_WDATA_MASK                             0xFFFFFFFF
//========================================
/* REG_TX_SELECTOR_TX_SEL_MU_PRIMARY_THRES 0x20C */
#define B0_TX_SELECTOR_TX_SEL_MU_PRIMARY_THRES_FIELD_PD_PRIMARY_THRES_MASK                                        0x000FFFFF
//========================================
/* REG_TX_SELECTOR_TX_SEL_MU_SECONDARY_THRES 0x210 */
#define B0_TX_SELECTOR_TX_SEL_MU_SECONDARY_THRES_FIELD_PD_SECONDARY_THRES_MASK                                    0x000FFFFF
//========================================
/* REG_TX_SELECTOR_TX_SEL_MU_ERR 0x214 */
#define B0_TX_SELECTOR_TX_SEL_MU_ERR_FIELD_PRIMARY_NOT_VALID_ERR_MASK                                             0x00000001
#define B0_TX_SELECTOR_TX_SEL_MU_ERR_FIELD_NO_PRIMARY_IN_GRP_ERR_MASK                                             0x00000002
#define B0_TX_SELECTOR_TX_SEL_MU_ERR_FIELD_PRIMARY_GRP_THRES_ERR_MASK                                             0x00000004
//========================================
/* REG_TX_SELECTOR_TX_SEL_MU_ERR_CLR 0x218 */
#define B0_TX_SELECTOR_TX_SEL_MU_ERR_CLR_FIELD_PRIMARY_NOT_VALID_ERR_CLR_MASK                                     0x00000001
#define B0_TX_SELECTOR_TX_SEL_MU_ERR_CLR_FIELD_NO_PRIMARY_IN_GRP_ERR_CLR_MASK                                     0x00000002
#define B0_TX_SELECTOR_TX_SEL_MU_ERR_CLR_FIELD_PRIMARY_GRP_THRES_ERR_CLR_MASK                                     0x00000004
//========================================
/* REG_TX_SELECTOR_TX_SEL_MU_LOCKER_EN 0x220 */
#define B0_TX_SELECTOR_TX_SEL_MU_LOCKER_EN_FIELD_MU_LOCKER_EN_MASK                                                0x00000001
//========================================
/* REG_TX_SELECTOR_TX_SEL_MU_SW_MASTER0_LOCKER 0x224 */
#define B0_TX_SELECTOR_TX_SEL_MU_SW_MASTER0_LOCKER_FIELD_MASTER0_MU_LOCKER_GRP_SEL_MASK                           0x0000007F
#define B0_TX_SELECTOR_TX_SEL_MU_SW_MASTER0_LOCKER_FIELD_MASTER0_MU_LOCKER_BIT_TYPE_SEL_MASK                      0x03000000
//========================================
/* REG_TX_SELECTOR_TX_SEL_MU_SW_MASTER1_LOCKER 0x228 */
#define B0_TX_SELECTOR_TX_SEL_MU_SW_MASTER1_LOCKER_FIELD_MASTER1_MU_LOCKER_GRP_SEL_MASK                           0x0000007F
#define B0_TX_SELECTOR_TX_SEL_MU_SW_MASTER1_LOCKER_FIELD_MASTER1_MU_LOCKER_BIT_TYPE_SEL_MASK                      0x03000000
//========================================
/* REG_TX_SELECTOR_TX_SEL_MU_SW_MASTER2_LOCKER 0x22C */
#define B0_TX_SELECTOR_TX_SEL_MU_SW_MASTER2_LOCKER_FIELD_MASTER2_MU_LOCKER_GRP_SEL_MASK                           0x0000007F
#define B0_TX_SELECTOR_TX_SEL_MU_SW_MASTER2_LOCKER_FIELD_MASTER2_MU_LOCKER_BIT_TYPE_SEL_MASK                      0x03000000
//========================================
/* REG_TX_SELECTOR_TX_SEL_MU_SW_MASTER3_LOCKER 0x230 */
#define B0_TX_SELECTOR_TX_SEL_MU_SW_MASTER3_LOCKER_FIELD_MASTER3_MU_LOCKER_GRP_SEL_MASK                           0x0000007F
#define B0_TX_SELECTOR_TX_SEL_MU_SW_MASTER3_LOCKER_FIELD_MASTER3_MU_LOCKER_BIT_TYPE_SEL_MASK                      0x03000000
//========================================
/* REG_TX_SELECTOR_TX_SEL_MU_SW_MASTER0_LOCKER_STATUS 0x234 */
#define B0_TX_SELECTOR_TX_SEL_MU_SW_MASTER0_LOCKER_STATUS_FIELD_SW_MASTER0_MU_REQ_STATUS_MASK                     0x0000001F
#define B0_TX_SELECTOR_TX_SEL_MU_SW_MASTER0_LOCKER_STATUS_FIELD_MASTER0_MU_LOCKER_DONE_INT_MASK                   0x00000100
//========================================
/* REG_TX_SELECTOR_TX_SEL_MU_SW_MASTER1_LOCKER_STATUS 0x238 */
#define B0_TX_SELECTOR_TX_SEL_MU_SW_MASTER1_LOCKER_STATUS_FIELD_SW_MASTER1_MU_REQ_STATUS_MASK                     0x0000001F
#define B0_TX_SELECTOR_TX_SEL_MU_SW_MASTER1_LOCKER_STATUS_FIELD_MASTER1_MU_LOCKER_DONE_INT_MASK                   0x00000100
//========================================
/* REG_TX_SELECTOR_TX_SEL_MU_SW_MASTER2_LOCKER_STATUS 0x23C */
#define B0_TX_SELECTOR_TX_SEL_MU_SW_MASTER2_LOCKER_STATUS_FIELD_SW_MASTER2_MU_REQ_STATUS_MASK                     0x0000001F
#define B0_TX_SELECTOR_TX_SEL_MU_SW_MASTER2_LOCKER_STATUS_FIELD_MASTER2_MU_LOCKER_DONE_INT_MASK                   0x00000100
//========================================
/* REG_TX_SELECTOR_TX_SEL_MU_SW_MASTER3_LOCKER_STATUS 0x240 */
#define B0_TX_SELECTOR_TX_SEL_MU_SW_MASTER3_LOCKER_STATUS_FIELD_SW_MASTER3_MU_REQ_STATUS_MASK                     0x0000001F
#define B0_TX_SELECTOR_TX_SEL_MU_SW_MASTER3_LOCKER_STATUS_FIELD_MASTER3_MU_LOCKER_DONE_INT_MASK                   0x00000100
//========================================
/* REG_TX_SELECTOR_TX_SEL_MU_LOCKER_INT_CLR 0x244 */
#define B0_TX_SELECTOR_TX_SEL_MU_LOCKER_INT_CLR_FIELD_SW_MASTER0_MU_LOCKER_INT_CLR_MASK                           0x00000001
#define B0_TX_SELECTOR_TX_SEL_MU_LOCKER_INT_CLR_FIELD_SW_MASTER1_MU_LOCKER_INT_CLR_MASK                           0x00000002
#define B0_TX_SELECTOR_TX_SEL_MU_LOCKER_INT_CLR_FIELD_SW_MASTER2_MU_LOCKER_INT_CLR_MASK                           0x00000004
#define B0_TX_SELECTOR_TX_SEL_MU_LOCKER_INT_CLR_FIELD_SW_MASTER3_MU_LOCKER_INT_CLR_MASK                           0x00000008
//========================================
/* REG_TX_SELECTOR_TX_SEL_MU_LOCKER_PUSH_WHILE_BUSY 0x248 */
#define B0_TX_SELECTOR_TX_SEL_MU_LOCKER_PUSH_WHILE_BUSY_FIELD_MASTER0_MU_PUSH_WHILE_BUSY_MASK                     0x00000001
#define B0_TX_SELECTOR_TX_SEL_MU_LOCKER_PUSH_WHILE_BUSY_FIELD_MASTER1_MU_PUSH_WHILE_BUSY_MASK                     0x00000002
#define B0_TX_SELECTOR_TX_SEL_MU_LOCKER_PUSH_WHILE_BUSY_FIELD_MASTER2_MU_PUSH_WHILE_BUSY_MASK                     0x00000004
#define B0_TX_SELECTOR_TX_SEL_MU_LOCKER_PUSH_WHILE_BUSY_FIELD_MASTER3_MU_PUSH_WHILE_BUSY_MASK                     0x00000008
//========================================
/* REG_TX_SELECTOR_TX_SEL_MU_LOCKER_PUSH_WHILE_BUSY_CLR 0x24C */
#define B0_TX_SELECTOR_TX_SEL_MU_LOCKER_PUSH_WHILE_BUSY_CLR_FIELD_MASTER0_MU_PUSH_WHILE_BUSY_CLR_MASK             0x00000001
#define B0_TX_SELECTOR_TX_SEL_MU_LOCKER_PUSH_WHILE_BUSY_CLR_FIELD_MASTER1_MU_PUSH_WHILE_BUSY_CLR_MASK             0x00000002
#define B0_TX_SELECTOR_TX_SEL_MU_LOCKER_PUSH_WHILE_BUSY_CLR_FIELD_MASTER2_MU_PUSH_WHILE_BUSY_CLR_MASK             0x00000004
#define B0_TX_SELECTOR_TX_SEL_MU_LOCKER_PUSH_WHILE_BUSY_CLR_FIELD_MASTER3_MU_PUSH_WHILE_BUSY_CLR_MASK             0x00000008
//========================================
/* REG_TX_SELECTOR_TX_SEL_MU_UP0_RESULT 0x250 */
#define B0_TX_SELECTOR_TX_SEL_MU_UP0_RESULT_FIELD_MU_UP0_TID_MASK                                                 0x00000007
#define B0_TX_SELECTOR_TX_SEL_MU_UP0_RESULT_FIELD_MU_UP0_STA_MASK                                                 0x000007F8
#define B0_TX_SELECTOR_TX_SEL_MU_UP0_RESULT_FIELD_MU_UP0_VAP_MASK                                                 0x0001F000
#define B0_TX_SELECTOR_TX_SEL_MU_UP0_RESULT_FIELD_MU_UP0_TXQ_ID_MASK                                              0x000E0000
#define B0_TX_SELECTOR_TX_SEL_MU_UP0_RESULT_FIELD_MU_UP0_AC_ID_MASK                                               0x00300000
#define B0_TX_SELECTOR_TX_SEL_MU_UP0_RESULT_FIELD_MU_UP0_BF_REQ_MASK                                              0x03000000
#define B0_TX_SELECTOR_TX_SEL_MU_UP0_RESULT_FIELD_MU_UP0_VALID_MASK                                               0x10000000
//========================================
/* REG_TX_SELECTOR_TX_SEL_MU_UP1_RESULT 0x254 */
#define B0_TX_SELECTOR_TX_SEL_MU_UP1_RESULT_FIELD_MU_UP1_TID_MASK                                                 0x00000007
#define B0_TX_SELECTOR_TX_SEL_MU_UP1_RESULT_FIELD_MU_UP1_STA_MASK                                                 0x000007F8
#define B0_TX_SELECTOR_TX_SEL_MU_UP1_RESULT_FIELD_MU_UP1_VAP_MASK                                                 0x0001F000
#define B0_TX_SELECTOR_TX_SEL_MU_UP1_RESULT_FIELD_MU_UP1_TXQ_ID_MASK                                              0x000E0000
#define B0_TX_SELECTOR_TX_SEL_MU_UP1_RESULT_FIELD_MU_UP1_AC_ID_MASK                                               0x00300000
#define B0_TX_SELECTOR_TX_SEL_MU_UP1_RESULT_FIELD_MU_UP1_BF_REQ_MASK                                              0x03000000
#define B0_TX_SELECTOR_TX_SEL_MU_UP1_RESULT_FIELD_MU_UP1_VALID_MASK                                               0x10000000
//========================================
/* REG_TX_SELECTOR_TX_SEL_MU_UP2_RESULT 0x258 */
#define B0_TX_SELECTOR_TX_SEL_MU_UP2_RESULT_FIELD_MU_UP2_TID_MASK                                                 0x00000007
#define B0_TX_SELECTOR_TX_SEL_MU_UP2_RESULT_FIELD_MU_UP2_STA_MASK                                                 0x000007F8
#define B0_TX_SELECTOR_TX_SEL_MU_UP2_RESULT_FIELD_MU_UP2_VAP_MASK                                                 0x0001F000
#define B0_TX_SELECTOR_TX_SEL_MU_UP2_RESULT_FIELD_MU_UP2_TXQ_ID_MASK                                              0x000E0000
#define B0_TX_SELECTOR_TX_SEL_MU_UP2_RESULT_FIELD_MU_UP2_AC_ID_MASK                                               0x00300000
#define B0_TX_SELECTOR_TX_SEL_MU_UP2_RESULT_FIELD_MU_UP2_BF_REQ_MASK                                              0x03000000
#define B0_TX_SELECTOR_TX_SEL_MU_UP2_RESULT_FIELD_MU_UP2_VALID_MASK                                               0x10000000
//========================================
/* REG_TX_SELECTOR_TX_SEL_MU_UP3_RESULT 0x25C */
#define B0_TX_SELECTOR_TX_SEL_MU_UP3_RESULT_FIELD_MU_UP3_TID_MASK                                                 0x00000007
#define B0_TX_SELECTOR_TX_SEL_MU_UP3_RESULT_FIELD_MU_UP3_STA_MASK                                                 0x000007F8
#define B0_TX_SELECTOR_TX_SEL_MU_UP3_RESULT_FIELD_MU_UP3_VAP_MASK                                                 0x0001F000
#define B0_TX_SELECTOR_TX_SEL_MU_UP3_RESULT_FIELD_MU_UP3_TXQ_ID_MASK                                              0x000E0000
#define B0_TX_SELECTOR_TX_SEL_MU_UP3_RESULT_FIELD_MU_UP3_AC_ID_MASK                                               0x00300000
#define B0_TX_SELECTOR_TX_SEL_MU_UP3_RESULT_FIELD_MU_UP3_BF_REQ_MASK                                              0x03000000
#define B0_TX_SELECTOR_TX_SEL_MU_UP3_RESULT_FIELD_MU_UP3_VALID_MASK                                               0x10000000
//========================================
/* REG_TX_SELECTOR_TX_SEL_MU_UP0_BITMAP 0x260 */
#define B0_TX_SELECTOR_TX_SEL_MU_UP0_BITMAP_FIELD_MU_UP0_BITMAP_REQ0_MASK                                         0x00000001
#define B0_TX_SELECTOR_TX_SEL_MU_UP0_BITMAP_FIELD_MU_UP0_BITMAP_PS_TYPE0_MASK                                     0x00000002
#define B0_TX_SELECTOR_TX_SEL_MU_UP0_BITMAP_FIELD_MU_UP0_BITMAP_PS_MODE0_MASK                                     0x00000004
#define B0_TX_SELECTOR_TX_SEL_MU_UP0_BITMAP_FIELD_MU_UP0_BITMAP_DATA_IN_Q0_MASK                                   0x00000008
#define B0_TX_SELECTOR_TX_SEL_MU_UP0_BITMAP_FIELD_MU_UP0_BITMAP_REQ1_MASK                                         0x00000010
#define B0_TX_SELECTOR_TX_SEL_MU_UP0_BITMAP_FIELD_MU_UP0_BITMAP_PS_TYPE1_MASK                                     0x00000020
#define B0_TX_SELECTOR_TX_SEL_MU_UP0_BITMAP_FIELD_MU_UP0_BITMAP_PS_MODE1_MASK                                     0x00000040
#define B0_TX_SELECTOR_TX_SEL_MU_UP0_BITMAP_FIELD_MU_UP0_BITMAP_DATA_IN_Q1_MASK                                   0x00000080
#define B0_TX_SELECTOR_TX_SEL_MU_UP0_BITMAP_FIELD_MU_UP0_BITMAP_REQ2_MASK                                         0x00000100
#define B0_TX_SELECTOR_TX_SEL_MU_UP0_BITMAP_FIELD_MU_UP0_BITMAP_PS_TYPE2_MASK                                     0x00000200
#define B0_TX_SELECTOR_TX_SEL_MU_UP0_BITMAP_FIELD_MU_UP0_BITMAP_PS_MODE2_MASK                                     0x00000400
#define B0_TX_SELECTOR_TX_SEL_MU_UP0_BITMAP_FIELD_MU_UP0_BITMAP_DATA_IN_Q2_MASK                                   0x00000800
#define B0_TX_SELECTOR_TX_SEL_MU_UP0_BITMAP_FIELD_MU_UP0_BITMAP_REQ3_MASK                                         0x00001000
#define B0_TX_SELECTOR_TX_SEL_MU_UP0_BITMAP_FIELD_MU_UP0_BITMAP_PS_TYPE3_MASK                                     0x00002000
#define B0_TX_SELECTOR_TX_SEL_MU_UP0_BITMAP_FIELD_MU_UP0_BITMAP_PS_MODE3_MASK                                     0x00004000
#define B0_TX_SELECTOR_TX_SEL_MU_UP0_BITMAP_FIELD_MU_UP0_BITMAP_DATA_IN_Q3_MASK                                   0x00008000
#define B0_TX_SELECTOR_TX_SEL_MU_UP0_BITMAP_FIELD_MU_UP0_BITMAP_REQ4_MASK                                         0x00010000
#define B0_TX_SELECTOR_TX_SEL_MU_UP0_BITMAP_FIELD_MU_UP0_BITMAP_PS_TYPE4_MASK                                     0x00020000
#define B0_TX_SELECTOR_TX_SEL_MU_UP0_BITMAP_FIELD_MU_UP0_BITMAP_PS_MODE4_MASK                                     0x00040000
#define B0_TX_SELECTOR_TX_SEL_MU_UP0_BITMAP_FIELD_MU_UP0_BITMAP_DATA_IN_Q4_MASK                                   0x00080000
#define B0_TX_SELECTOR_TX_SEL_MU_UP0_BITMAP_FIELD_MU_UP0_BITMAP_REQ5_MASK                                         0x00100000
#define B0_TX_SELECTOR_TX_SEL_MU_UP0_BITMAP_FIELD_MU_UP0_BITMAP_PS_TYPE5_MASK                                     0x00200000
#define B0_TX_SELECTOR_TX_SEL_MU_UP0_BITMAP_FIELD_MU_UP0_BITMAP_PS_MODE5_MASK                                     0x00400000
#define B0_TX_SELECTOR_TX_SEL_MU_UP0_BITMAP_FIELD_MU_UP0_BITMAP_DATA_IN_Q5_MASK                                   0x00800000
#define B0_TX_SELECTOR_TX_SEL_MU_UP0_BITMAP_FIELD_MU_UP0_BITMAP_REQ6_MASK                                         0x01000000
#define B0_TX_SELECTOR_TX_SEL_MU_UP0_BITMAP_FIELD_MU_UP0_BITMAP_PS_TYPE6_MASK                                     0x02000000
#define B0_TX_SELECTOR_TX_SEL_MU_UP0_BITMAP_FIELD_MU_UP0_BITMAP_PS_MODE6_MASK                                     0x04000000
#define B0_TX_SELECTOR_TX_SEL_MU_UP0_BITMAP_FIELD_MU_UP0_BITMAP_DATA_IN_Q6_MASK                                   0x08000000
#define B0_TX_SELECTOR_TX_SEL_MU_UP0_BITMAP_FIELD_MU_UP0_BITMAP_REQ7_MASK                                         0x10000000
#define B0_TX_SELECTOR_TX_SEL_MU_UP0_BITMAP_FIELD_MU_UP0_BITMAP_PS_TYPE7_MASK                                     0x20000000
#define B0_TX_SELECTOR_TX_SEL_MU_UP0_BITMAP_FIELD_MU_UP0_BITMAP_PS_MODE7_MASK                                     0x40000000
#define B0_TX_SELECTOR_TX_SEL_MU_UP0_BITMAP_FIELD_MU_UP0_BITMAP_DATA_IN_Q7_MASK                                   0x80000000
//========================================
/* REG_TX_SELECTOR_TX_SEL_HE_MU_UP0_BITMAP 0x264 */
#define B0_TX_SELECTOR_TX_SEL_HE_MU_UP0_BITMAP_FIELD_HE_MU_UP0_BITMAP_REQ8_MASK                                   0x00000001
#define B0_TX_SELECTOR_TX_SEL_HE_MU_UP0_BITMAP_FIELD_HE_MU_UP0_BITMAP_PS_TYPE8_MASK                               0x00000002
#define B0_TX_SELECTOR_TX_SEL_HE_MU_UP0_BITMAP_FIELD_HE_MU_UP0_BITMAP_PS_MODE8_MASK                               0x00000004
#define B0_TX_SELECTOR_TX_SEL_HE_MU_UP0_BITMAP_FIELD_HE_MU_UP0_BITMAP_DATA_IN_Q8_MASK                             0x00000008
#define B0_TX_SELECTOR_TX_SEL_HE_MU_UP0_BITMAP_FIELD_HE_MU_UP0_BITMAP_STATUS_MASK                                 0x1FFFFFF0
//========================================
/* REG_TX_SELECTOR_TX_SEL_HE_PRE_AGG_UP0_BITMAP 0x268 */
#define B0_TX_SELECTOR_TX_SEL_HE_PRE_AGG_UP0_BITMAP_FIELD_HE_SEL_UP0_PRE_AGG_BITMAP_MASK                          0x00003FFF
//========================================
/* REG_TX_SELECTOR_TX_SEL_MU_UP1_BITMAP 0x26C */
#define B0_TX_SELECTOR_TX_SEL_MU_UP1_BITMAP_FIELD_MU_UP1_BITMAP_PS_TYPE2_MASK                                     0x00000200
#define B0_TX_SELECTOR_TX_SEL_MU_UP1_BITMAP_FIELD_MU_UP1_BITMAP_PS_MODE2_MASK                                     0x00000400
#define B0_TX_SELECTOR_TX_SEL_MU_UP1_BITMAP_FIELD_MU_UP1_BITMAP_DATA_IN_Q2_MASK                                   0x00000800
#define B0_TX_SELECTOR_TX_SEL_MU_UP1_BITMAP_FIELD_MU_UP1_BITMAP_REQ3_MASK                                         0x00001000
#define B0_TX_SELECTOR_TX_SEL_MU_UP1_BITMAP_FIELD_MU_UP1_BITMAP_PS_TYPE3_MASK                                     0x00002000
#define B0_TX_SELECTOR_TX_SEL_MU_UP1_BITMAP_FIELD_MU_UP1_BITMAP_PS_MODE3_MASK                                     0x00004000
#define B0_TX_SELECTOR_TX_SEL_MU_UP1_BITMAP_FIELD_MU_UP1_BITMAP_DATA_IN_Q3_MASK                                   0x00008000
#define B0_TX_SELECTOR_TX_SEL_MU_UP1_BITMAP_FIELD_MU_UP1_BITMAP_REQ4_MASK                                         0x00010000
#define B0_TX_SELECTOR_TX_SEL_MU_UP1_BITMAP_FIELD_MU_UP1_BITMAP_PS_TYPE4_MASK                                     0x00020000
#define B0_TX_SELECTOR_TX_SEL_MU_UP1_BITMAP_FIELD_MU_UP1_BITMAP_PS_MODE4_MASK                                     0x00040000
#define B0_TX_SELECTOR_TX_SEL_MU_UP1_BITMAP_FIELD_MU_UP1_BITMAP_DATA_IN_Q4_MASK                                   0x00080000
#define B0_TX_SELECTOR_TX_SEL_MU_UP1_BITMAP_FIELD_MU_UP1_BITMAP_REQ5_MASK                                         0x00100000
#define B0_TX_SELECTOR_TX_SEL_MU_UP1_BITMAP_FIELD_MU_UP1_BITMAP_PS_TYPE5_MASK                                     0x00200000
#define B0_TX_SELECTOR_TX_SEL_MU_UP1_BITMAP_FIELD_MU_UP1_BITMAP_PS_MODE5_MASK                                     0x00400000
#define B0_TX_SELECTOR_TX_SEL_MU_UP1_BITMAP_FIELD_MU_UP1_BITMAP_DATA_IN_Q5_MASK                                   0x00800000
#define B0_TX_SELECTOR_TX_SEL_MU_UP1_BITMAP_FIELD_MU_UP1_BITMAP_REQ6_MASK                                         0x01000000
#define B0_TX_SELECTOR_TX_SEL_MU_UP1_BITMAP_FIELD_MU_UP1_BITMAP_PS_TYPE6_MASK                                     0x02000000
#define B0_TX_SELECTOR_TX_SEL_MU_UP1_BITMAP_FIELD_MU_UP1_BITMAP_PS_MODE6_MASK                                     0x04000000
#define B0_TX_SELECTOR_TX_SEL_MU_UP1_BITMAP_FIELD_MU_UP1_BITMAP_DATA_IN_Q6_MASK                                   0x08000000
#define B0_TX_SELECTOR_TX_SEL_MU_UP1_BITMAP_FIELD_MU_UP1_BITMAP_REQ7_MASK                                         0x10000000
#define B0_TX_SELECTOR_TX_SEL_MU_UP1_BITMAP_FIELD_MU_UP1_BITMAP_PS_TYPE7_MASK                                     0x20000000
#define B0_TX_SELECTOR_TX_SEL_MU_UP1_BITMAP_FIELD_MU_UP1_BITMAP_PS_MODE7_MASK                                     0x40000000
#define B0_TX_SELECTOR_TX_SEL_MU_UP1_BITMAP_FIELD_MU_UP1_BITMAP_DATA_IN_Q7_MASK                                   0x80000000
//========================================
/* REG_TX_SELECTOR_TX_SEL_HE_MU_UP1_BITMAP 0x270 */
#define B0_TX_SELECTOR_TX_SEL_HE_MU_UP1_BITMAP_FIELD_HE_MU_UP1_BITMAP_REQ8_MASK                                   0x00000001
#define B0_TX_SELECTOR_TX_SEL_HE_MU_UP1_BITMAP_FIELD_HE_MU_UP1_BITMAP_PS_TYPE8_MASK                               0x00000002
#define B0_TX_SELECTOR_TX_SEL_HE_MU_UP1_BITMAP_FIELD_HE_MU_UP1_BITMAP_PS_MODE8_MASK                               0x00000004
#define B0_TX_SELECTOR_TX_SEL_HE_MU_UP1_BITMAP_FIELD_HE_MU_UP1_BITMAP_DATA_IN_Q8_MASK                             0x00000008
#define B0_TX_SELECTOR_TX_SEL_HE_MU_UP1_BITMAP_FIELD_HE_MU_UP1_BITMAP_STATUS_MASK                                 0x1FFFFFF0
//========================================
/* REG_TX_SELECTOR_TX_SEL_HE_PRE_AGG_UP1_BITMAP 0x274 */
#define B0_TX_SELECTOR_TX_SEL_HE_PRE_AGG_UP1_BITMAP_FIELD_HE_SEL_UP1_PRE_AGG_BITMAP_MASK                          0x00003FFF
//========================================
/* REG_TX_SELECTOR_TX_SEL_MU_UP2_BITMAP 0x278 */
#define B0_TX_SELECTOR_TX_SEL_MU_UP2_BITMAP_FIELD_MU_UP2_BITMAP_DATA_IN_Q1_MASK                                   0x00000080
#define B0_TX_SELECTOR_TX_SEL_MU_UP2_BITMAP_FIELD_MU_UP2_BITMAP_REQ2_MASK                                         0x00000100
#define B0_TX_SELECTOR_TX_SEL_MU_UP2_BITMAP_FIELD_MU_UP2_BITMAP_PS_TYPE2_MASK                                     0x00000200
#define B0_TX_SELECTOR_TX_SEL_MU_UP2_BITMAP_FIELD_MU_UP2_BITMAP_PS_MODE2_MASK                                     0x00000400
#define B0_TX_SELECTOR_TX_SEL_MU_UP2_BITMAP_FIELD_MU_UP2_BITMAP_DATA_IN_Q2_MASK                                   0x00000800
#define B0_TX_SELECTOR_TX_SEL_MU_UP2_BITMAP_FIELD_MU_UP2_BITMAP_REQ3_MASK                                         0x00001000
#define B0_TX_SELECTOR_TX_SEL_MU_UP2_BITMAP_FIELD_MU_UP2_BITMAP_PS_TYPE3_MASK                                     0x00002000
#define B0_TX_SELECTOR_TX_SEL_MU_UP2_BITMAP_FIELD_MU_UP2_BITMAP_PS_MODE3_MASK                                     0x00004000
#define B0_TX_SELECTOR_TX_SEL_MU_UP2_BITMAP_FIELD_MU_UP2_BITMAP_DATA_IN_Q3_MASK                                   0x00008000
#define B0_TX_SELECTOR_TX_SEL_MU_UP2_BITMAP_FIELD_MU_UP2_BITMAP_REQ4_MASK                                         0x00010000
#define B0_TX_SELECTOR_TX_SEL_MU_UP2_BITMAP_FIELD_MU_UP2_BITMAP_PS_TYPE4_MASK                                     0x00020000
#define B0_TX_SELECTOR_TX_SEL_MU_UP2_BITMAP_FIELD_MU_UP2_BITMAP_PS_MODE4_MASK                                     0x00040000
#define B0_TX_SELECTOR_TX_SEL_MU_UP2_BITMAP_FIELD_MU_UP2_BITMAP_DATA_IN_Q4_MASK                                   0x00080000
#define B0_TX_SELECTOR_TX_SEL_MU_UP2_BITMAP_FIELD_MU_UP2_BITMAP_REQ5_MASK                                         0x00100000
#define B0_TX_SELECTOR_TX_SEL_MU_UP2_BITMAP_FIELD_MU_UP2_BITMAP_PS_TYPE5_MASK                                     0x00200000
#define B0_TX_SELECTOR_TX_SEL_MU_UP2_BITMAP_FIELD_MU_UP2_BITMAP_PS_MODE5_MASK                                     0x00400000
#define B0_TX_SELECTOR_TX_SEL_MU_UP2_BITMAP_FIELD_MU_UP2_BITMAP_DATA_IN_Q5_MASK                                   0x00800000
#define B0_TX_SELECTOR_TX_SEL_MU_UP2_BITMAP_FIELD_MU_UP2_BITMAP_REQ6_MASK                                         0x01000000
#define B0_TX_SELECTOR_TX_SEL_MU_UP2_BITMAP_FIELD_MU_UP2_BITMAP_PS_TYPE6_MASK                                     0x02000000
#define B0_TX_SELECTOR_TX_SEL_MU_UP2_BITMAP_FIELD_MU_UP2_BITMAP_PS_MODE6_MASK                                     0x04000000
#define B0_TX_SELECTOR_TX_SEL_MU_UP2_BITMAP_FIELD_MU_UP2_BITMAP_DATA_IN_Q6_MASK                                   0x08000000
#define B0_TX_SELECTOR_TX_SEL_MU_UP2_BITMAP_FIELD_MU_UP2_BITMAP_REQ7_MASK                                         0x10000000
#define B0_TX_SELECTOR_TX_SEL_MU_UP2_BITMAP_FIELD_MU_UP2_BITMAP_PS_TYPE7_MASK                                     0x20000000
#define B0_TX_SELECTOR_TX_SEL_MU_UP2_BITMAP_FIELD_MU_UP2_BITMAP_PS_MODE7_MASK                                     0x40000000
#define B0_TX_SELECTOR_TX_SEL_MU_UP2_BITMAP_FIELD_MU_UP2_BITMAP_DATA_IN_Q7_MASK                                   0x80000000
//========================================
/* REG_TX_SELECTOR_TX_SEL_HE_MU_UP2_BITMAP 0x27C */
#define B0_TX_SELECTOR_TX_SEL_HE_MU_UP2_BITMAP_FIELD_HE_MU_UP2_BITMAP_REQ8_MASK                                   0x00000001
#define B0_TX_SELECTOR_TX_SEL_HE_MU_UP2_BITMAP_FIELD_HE_MU_UP2_BITMAP_PS_TYPE8_MASK                               0x00000002
#define B0_TX_SELECTOR_TX_SEL_HE_MU_UP2_BITMAP_FIELD_HE_MU_UP2_BITMAP_PS_MODE8_MASK                               0x00000004
#define B0_TX_SELECTOR_TX_SEL_HE_MU_UP2_BITMAP_FIELD_HE_MU_UP2_BITMAP_DATA_IN_Q8_MASK                             0x00000008
#define B0_TX_SELECTOR_TX_SEL_HE_MU_UP2_BITMAP_FIELD_HE_MU_UP2_BITMAP_STATUS_MASK                                 0x1FFFFFF0
//========================================
/* REG_TX_SELECTOR_TX_SEL_HE_PRE_AGG_UP2_BITMAP 0x280 */
#define B0_TX_SELECTOR_TX_SEL_HE_PRE_AGG_UP2_BITMAP_FIELD_HE_SEL_UP2_PRE_AGG_BITMAP_MASK                          0x00003FFF
//========================================
/* REG_TX_SELECTOR_TX_SEL_MU_UP3_BITMAP 0x284 */
#define B0_TX_SELECTOR_TX_SEL_MU_UP3_BITMAP_FIELD_MU_UP3_BITMAP_REQ0_MASK                                         0x00000001
#define B0_TX_SELECTOR_TX_SEL_MU_UP3_BITMAP_FIELD_MU_UP3_BITMAP_PS_TYPE0_MASK                                     0x00000002
#define B0_TX_SELECTOR_TX_SEL_MU_UP3_BITMAP_FIELD_MU_UP3_BITMAP_PS_MODE0_MASK                                     0x00000004
#define B0_TX_SELECTOR_TX_SEL_MU_UP3_BITMAP_FIELD_MU_UP3_BITMAP_DATA_IN_Q0_MASK                                   0x00000008
#define B0_TX_SELECTOR_TX_SEL_MU_UP3_BITMAP_FIELD_MU_UP3_BITMAP_REQ1_MASK                                         0x00000010
#define B0_TX_SELECTOR_TX_SEL_MU_UP3_BITMAP_FIELD_MU_UP3_BITMAP_PS_TYPE1_MASK                                     0x00000020
#define B0_TX_SELECTOR_TX_SEL_MU_UP3_BITMAP_FIELD_MU_UP3_BITMAP_PS_MODE1_MASK                                     0x00000040
#define B0_TX_SELECTOR_TX_SEL_MU_UP3_BITMAP_FIELD_MU_UP3_BITMAP_DATA_IN_Q1_MASK                                   0x00000080
#define B0_TX_SELECTOR_TX_SEL_MU_UP3_BITMAP_FIELD_MU_UP3_BITMAP_REQ2_MASK                                         0x00000100
#define B0_TX_SELECTOR_TX_SEL_MU_UP3_BITMAP_FIELD_MU_UP3_BITMAP_PS_TYPE2_MASK                                     0x00000200
#define B0_TX_SELECTOR_TX_SEL_MU_UP3_BITMAP_FIELD_MU_UP3_BITMAP_PS_MODE2_MASK                                     0x00000400
#define B0_TX_SELECTOR_TX_SEL_MU_UP3_BITMAP_FIELD_MU_UP3_BITMAP_DATA_IN_Q2_MASK                                   0x00000800
#define B0_TX_SELECTOR_TX_SEL_MU_UP3_BITMAP_FIELD_MU_UP3_BITMAP_REQ3_MASK                                         0x00001000
#define B0_TX_SELECTOR_TX_SEL_MU_UP3_BITMAP_FIELD_MU_UP3_BITMAP_PS_TYPE3_MASK                                     0x00002000
#define B0_TX_SELECTOR_TX_SEL_MU_UP3_BITMAP_FIELD_MU_UP3_BITMAP_PS_MODE3_MASK                                     0x00004000
#define B0_TX_SELECTOR_TX_SEL_MU_UP3_BITMAP_FIELD_MU_UP3_BITMAP_DATA_IN_Q3_MASK                                   0x00008000
#define B0_TX_SELECTOR_TX_SEL_MU_UP3_BITMAP_FIELD_MU_UP3_BITMAP_REQ4_MASK                                         0x00010000
#define B0_TX_SELECTOR_TX_SEL_MU_UP3_BITMAP_FIELD_MU_UP3_BITMAP_PS_TYPE4_MASK                                     0x00020000
#define B0_TX_SELECTOR_TX_SEL_MU_UP3_BITMAP_FIELD_MU_UP3_BITMAP_PS_MODE4_MASK                                     0x00040000
#define B0_TX_SELECTOR_TX_SEL_MU_UP3_BITMAP_FIELD_MU_UP3_BITMAP_DATA_IN_Q4_MASK                                   0x00080000
#define B0_TX_SELECTOR_TX_SEL_MU_UP3_BITMAP_FIELD_MU_UP3_BITMAP_REQ5_MASK                                         0x00100000
#define B0_TX_SELECTOR_TX_SEL_MU_UP3_BITMAP_FIELD_MU_UP3_BITMAP_PS_TYPE5_MASK                                     0x00200000
#define B0_TX_SELECTOR_TX_SEL_MU_UP3_BITMAP_FIELD_MU_UP3_BITMAP_PS_MODE5_MASK                                     0x00400000
#define B0_TX_SELECTOR_TX_SEL_MU_UP3_BITMAP_FIELD_MU_UP3_BITMAP_DATA_IN_Q5_MASK                                   0x00800000
#define B0_TX_SELECTOR_TX_SEL_MU_UP3_BITMAP_FIELD_MU_UP3_BITMAP_REQ6_MASK                                         0x01000000
#define B0_TX_SELECTOR_TX_SEL_MU_UP3_BITMAP_FIELD_MU_UP3_BITMAP_PS_TYPE6_MASK                                     0x02000000
#define B0_TX_SELECTOR_TX_SEL_MU_UP3_BITMAP_FIELD_MU_UP3_BITMAP_PS_MODE6_MASK                                     0x04000000
#define B0_TX_SELECTOR_TX_SEL_MU_UP3_BITMAP_FIELD_MU_UP3_BITMAP_DATA_IN_Q6_MASK                                   0x08000000
#define B0_TX_SELECTOR_TX_SEL_MU_UP3_BITMAP_FIELD_MU_UP3_BITMAP_REQ7_MASK                                         0x10000000
#define B0_TX_SELECTOR_TX_SEL_MU_UP3_BITMAP_FIELD_MU_UP3_BITMAP_PS_TYPE7_MASK                                     0x20000000
#define B0_TX_SELECTOR_TX_SEL_MU_UP3_BITMAP_FIELD_MU_UP3_BITMAP_PS_MODE7_MASK                                     0x40000000
#define B0_TX_SELECTOR_TX_SEL_MU_UP3_BITMAP_FIELD_MU_UP3_BITMAP_DATA_IN_Q7_MASK                                   0x80000000
//========================================
/* REG_TX_SELECTOR_TX_SEL_HE_MU_UP3_BITMAP 0x288 */
#define B0_TX_SELECTOR_TX_SEL_HE_MU_UP3_BITMAP_FIELD_HE_MU_UP3_BITMAP_REQ8_MASK                                   0x00000001
#define B0_TX_SELECTOR_TX_SEL_HE_MU_UP3_BITMAP_FIELD_HE_MU_UP3_BITMAP_PS_TYPE8_MASK                               0x00000002
#define B0_TX_SELECTOR_TX_SEL_HE_MU_UP3_BITMAP_FIELD_HE_MU_UP3_BITMAP_PS_MODE8_MASK                               0x00000004
#define B0_TX_SELECTOR_TX_SEL_HE_MU_UP3_BITMAP_FIELD_HE_MU_UP3_BITMAP_DATA_IN_Q8_MASK                             0x00000008
#define B0_TX_SELECTOR_TX_SEL_HE_MU_UP3_BITMAP_FIELD_HE_MU_UP3_BITMAP_STATUS_MASK                                 0x1FFFFFF0
//========================================
/* REG_TX_SELECTOR_TX_SEL_HE_PRE_AGG_UP3_BITMAP 0x28C */
#define B0_TX_SELECTOR_TX_SEL_HE_PRE_AGG_UP3_BITMAP_FIELD_HE_SEL_UP3_PRE_AGG_BITMAP_MASK                          0x00003FFF
//========================================
/* REG_TX_SELECTOR_TX_SEL_MU_GRP_BITMAP 0x290 */
#define B0_TX_SELECTOR_TX_SEL_MU_GRP_BITMAP_FIELD_MU_GRP_VALID_MASK                                               0x00000001
#define B0_TX_SELECTOR_TX_SEL_MU_GRP_BITMAP_FIELD_PRIMARY_GRP_POINTER_MASK                                        0x000000FE
#define B0_TX_SELECTOR_TX_SEL_MU_GRP_BITMAP_FIELD_PRIMARY_IDX_MASK                                                0x00000300
#define B0_TX_SELECTOR_TX_SEL_MU_GRP_BITMAP_FIELD_TX_MODE_MASK                                                    0x00000C00
#define B0_TX_SELECTOR_TX_SEL_MU_GRP_BITMAP_FIELD_UP_LOCK_DONE_MASK                                               0x0000F000
#define B0_TX_SELECTOR_TX_SEL_MU_GRP_BITMAP_FIELD_GRP_LOCK_DONE_MASK                                              0x00010000
//========================================
/* REG_TX_SELECTOR_TX_SEL_HE_GRP_BITMAP 0x294 */
#define B0_TX_SELECTOR_TX_SEL_HE_GRP_BITMAP_FIELD_SEL_DL_SEL_MASK                                                 0x00000001
#define B0_TX_SELECTOR_TX_SEL_HE_GRP_BITMAP_FIELD_SEL_HE_MU_SU_MASK                                               0x00000030
#define B0_TX_SELECTOR_TX_SEL_HE_GRP_BITMAP_FIELD_SELECTION_TYPE_MASK                                             0x00003F00
#define B0_TX_SELECTOR_TX_SEL_HE_GRP_BITMAP_FIELD_SEL_HE_PLAN_ENTRY_MASK                                          0x007F0000
#define B0_TX_SELECTOR_TX_SEL_HE_GRP_BITMAP_FIELD_SEL_HE_PLAN_LOCK_DONE_MASK                                      0x01000000
//========================================
/* REG_TX_SELECTOR_TX_SEL_MU_SM_STATUS 0x298 */
#define B0_TX_SELECTOR_TX_SEL_MU_SM_STATUS_FIELD_MU_SEL_SM_MASK                                                   0x0000000F
#define B0_TX_SELECTOR_TX_SEL_MU_SM_STATUS_FIELD_MU_SEL_LOCK_SM_MASK                                              0x00000070
#define B0_TX_SELECTOR_TX_SEL_MU_SM_STATUS_FIELD_SEC_PD_COUNT_RD_SM_MASK                                          0x00000700
#define B0_TX_SELECTOR_TX_SEL_MU_SM_STATUS_FIELD_MU_LOCKER_SM_MASK                                                0x00003000
//========================================
/* REG_TX_SELECTOR_TX_SEL_MU_SU_REPORT_STATUS 0x29C */
#define B0_TX_SELECTOR_TX_SEL_MU_SU_REPORT_STATUS_FIELD_TX_SEL_MU_SU_SECONDARY0_MASK                              0x0000000F
#define B0_TX_SELECTOR_TX_SEL_MU_SU_REPORT_STATUS_FIELD_TX_SEL_MU_SU_SECONDARY1_MASK                              0x000000F0
#define B0_TX_SELECTOR_TX_SEL_MU_SU_REPORT_STATUS_FIELD_TX_SEL_MU_SU_SECONDARY2_MASK                              0x00000F00
#define B0_TX_SELECTOR_TX_SEL_MU_SU_REPORT_STATUS_FIELD_TX_SEL_MU_SU_SECONDARY3_MASK                              0x0000F000
#define B0_TX_SELECTOR_TX_SEL_MU_SU_REPORT_STATUS_FIELD_TX_SEL_MU_SU_PRIMARY_MASK                                 0x007F0000
#define B0_TX_SELECTOR_TX_SEL_MU_SU_REPORT_STATUS_FIELD_TX_SEL_MU_SU_VALID_MASK                                   0x01000000
//========================================
/* REG_TX_SELECTOR_TX_SEL_SELECT_MODE_CFG 0x2A0 */
#define B0_TX_SELECTOR_TX_SEL_SELECT_MODE_CFG_FIELD_DL_WITH_UL_EN_MASK                                            0x00000001
#define B0_TX_SELECTOR_TX_SEL_SELECT_MODE_CFG_FIELD_MGMT_BYPASS_EN_MASK                                           0x00000010
#define B0_TX_SELECTOR_TX_SEL_SELECT_MODE_CFG_FIELD_MGMT_AC_NUM_MASK                                              0x00000300
#define B0_TX_SELECTOR_TX_SEL_SELECT_MODE_CFG_FIELD_HE_MU_FALLBACK_SU_MASK                                        0x00001000
#define B0_TX_SELECTOR_TX_SEL_SELECT_MODE_CFG_FIELD_HE_MU_OMI_DIS_FALLBACK_SU_MASK                                0x00002000
#define B0_TX_SELECTOR_TX_SEL_SELECT_MODE_CFG_FIELD_DL_HE_PS_WITHOUT_REQ_EN_MASK                                  0x00010000
//========================================
/* REG_TX_SELECTOR_TX_SEL_PLAN_LOCKER_EN 0x2A4 */
#define B0_TX_SELECTOR_TX_SEL_PLAN_LOCKER_EN_FIELD_PLAN_LOCKER_EN_MASK                                            0x00000001
#define B0_TX_SELECTOR_TX_SEL_PLAN_LOCKER_EN_FIELD_PLAN_LOCKER_SM_MASK                                            0x00000300
//========================================
/* REG_TX_SELECTOR_TX_SEL_PLAN_SW_MASTER0_LOCKER 0x300 */
#define B0_TX_SELECTOR_TX_SEL_PLAN_SW_MASTER0_LOCKER_FIELD_MASTER0_PLAN_LOCKER_IDX_SEL_MASK                       0x0000007F
#define B0_TX_SELECTOR_TX_SEL_PLAN_SW_MASTER0_LOCKER_FIELD_MASTER0_PLAN_LOCKER_BIT_TYPE_SEL_MASK                  0x03000000
//========================================
/* REG_TX_SELECTOR_TX_SEL_PLAN_SW_MASTER1_LOCKER 0x304 */
#define B0_TX_SELECTOR_TX_SEL_PLAN_SW_MASTER1_LOCKER_FIELD_MASTER1_PLAN_LOCKER_IDX_SEL_MASK                       0x0000007F
#define B0_TX_SELECTOR_TX_SEL_PLAN_SW_MASTER1_LOCKER_FIELD_MASTER1_PLAN_LOCKER_BIT_TYPE_SEL_MASK                  0x03000000
//========================================
/* REG_TX_SELECTOR_TX_SEL_PLAN_SW_MASTER2_LOCKER 0x308 */
#define B0_TX_SELECTOR_TX_SEL_PLAN_SW_MASTER2_LOCKER_FIELD_MASTER2_PLAN_LOCKER_IDX_SEL_MASK                       0x0000007F
#define B0_TX_SELECTOR_TX_SEL_PLAN_SW_MASTER2_LOCKER_FIELD_MASTER2_PLAN_LOCKER_BIT_TYPE_SEL_MASK                  0x03000000
//========================================
/* REG_TX_SELECTOR_TX_SEL_PLAN_SW_MASTER3_LOCKER 0x30C */
#define B0_TX_SELECTOR_TX_SEL_PLAN_SW_MASTER3_LOCKER_FIELD_MASTER3_PLAN_LOCKER_IDX_SEL_MASK                       0x0000007F
#define B0_TX_SELECTOR_TX_SEL_PLAN_SW_MASTER3_LOCKER_FIELD_MASTER3_PLAN_LOCKER_BIT_TYPE_SEL_MASK                  0x03000000
//========================================
/* REG_TX_SELECTOR_TX_SEL_PLAN_SW_MASTER0_LOCKER_STATUS 0x310 */
#define B0_TX_SELECTOR_TX_SEL_PLAN_SW_MASTER0_LOCKER_STATUS_FIELD_SW_MASTER0_PLAN_REQ_STATUS_MASK                 0x0000001F
#define B0_TX_SELECTOR_TX_SEL_PLAN_SW_MASTER0_LOCKER_STATUS_FIELD_MASTER0_PLAN_LOCKER_DONE_INT_MASK               0x00000100
//========================================
/* REG_TX_SELECTOR_TX_SEL_PLAN_SW_MASTER1_LOCKER_STATUS 0x314 */
#define B0_TX_SELECTOR_TX_SEL_PLAN_SW_MASTER1_LOCKER_STATUS_FIELD_SW_MASTER1_PLAN_REQ_STATUS_MASK                 0x0000001F
#define B0_TX_SELECTOR_TX_SEL_PLAN_SW_MASTER1_LOCKER_STATUS_FIELD_MASTER1_PLAN_LOCKER_DONE_INT_MASK               0x00000100
//========================================
/* REG_TX_SELECTOR_TX_SEL_PLAN_SW_MASTER2_LOCKER_STATUS 0x318 */
#define B0_TX_SELECTOR_TX_SEL_PLAN_SW_MASTER2_LOCKER_STATUS_FIELD_SW_MASTER2_PLAN_REQ_STATUS_MASK                 0x0000001F
#define B0_TX_SELECTOR_TX_SEL_PLAN_SW_MASTER2_LOCKER_STATUS_FIELD_MASTER2_PLAN_LOCKER_DONE_INT_MASK               0x00000100
//========================================
/* REG_TX_SELECTOR_TX_SEL_PLAN_SW_MASTER3_LOCKER_STATUS 0x31C */
#define B0_TX_SELECTOR_TX_SEL_PLAN_SW_MASTER3_LOCKER_STATUS_FIELD_SW_MASTER3_PLAN_REQ_STATUS_MASK                 0x0000001F
#define B0_TX_SELECTOR_TX_SEL_PLAN_SW_MASTER3_LOCKER_STATUS_FIELD_MASTER3_PLAN_LOCKER_DONE_INT_MASK               0x00000100
//========================================
/* REG_TX_SELECTOR_TX_SEL_PLAN_LOCKER_INT_CLR 0x320 */
#define B0_TX_SELECTOR_TX_SEL_PLAN_LOCKER_INT_CLR_FIELD_SW_MASTER0_PLAN_LOCKER_INT_CLR_MASK                       0x00000001
#define B0_TX_SELECTOR_TX_SEL_PLAN_LOCKER_INT_CLR_FIELD_SW_MASTER1_PLAN_LOCKER_INT_CLR_MASK                       0x00000002
#define B0_TX_SELECTOR_TX_SEL_PLAN_LOCKER_INT_CLR_FIELD_SW_MASTER2_PLAN_LOCKER_INT_CLR_MASK                       0x00000004
#define B0_TX_SELECTOR_TX_SEL_PLAN_LOCKER_INT_CLR_FIELD_SW_MASTER3_PLAN_LOCKER_INT_CLR_MASK                       0x00000008
//========================================
/* REG_TX_SELECTOR_TX_SEL_PLAN_LOCKER_PUSH_WHILE_BUSY 0x324 */
#define B0_TX_SELECTOR_TX_SEL_PLAN_LOCKER_PUSH_WHILE_BUSY_FIELD_MASTER0_PLAN_PUSH_WHILE_BUSY_MASK                 0x00000001
#define B0_TX_SELECTOR_TX_SEL_PLAN_LOCKER_PUSH_WHILE_BUSY_FIELD_MASTER1_PLAN_PUSH_WHILE_BUSY_MASK                 0x00000002
#define B0_TX_SELECTOR_TX_SEL_PLAN_LOCKER_PUSH_WHILE_BUSY_FIELD_MASTER2_PLAN_PUSH_WHILE_BUSY_MASK                 0x00000004
#define B0_TX_SELECTOR_TX_SEL_PLAN_LOCKER_PUSH_WHILE_BUSY_FIELD_MASTER3_PLAN_PUSH_WHILE_BUSY_MASK                 0x00000008
//========================================
/* REG_TX_SELECTOR_TX_SEL_PLAN_LOCKER_PUSH_WHILE_BUSY_CLR 0x328 */
#define B0_TX_SELECTOR_TX_SEL_PLAN_LOCKER_PUSH_WHILE_BUSY_CLR_FIELD_MASTER0_PLAN_PUSH_WHILE_BUSY_CLR_MASK         0x00000001
#define B0_TX_SELECTOR_TX_SEL_PLAN_LOCKER_PUSH_WHILE_BUSY_CLR_FIELD_MASTER1_PLAN_PUSH_WHILE_BUSY_CLR_MASK         0x00000002
#define B0_TX_SELECTOR_TX_SEL_PLAN_LOCKER_PUSH_WHILE_BUSY_CLR_FIELD_MASTER2_PLAN_PUSH_WHILE_BUSY_CLR_MASK         0x00000004
#define B0_TX_SELECTOR_TX_SEL_PLAN_LOCKER_PUSH_WHILE_BUSY_CLR_FIELD_MASTER3_PLAN_PUSH_WHILE_BUSY_CLR_MASK         0x00000008
//========================================
/* REG_TX_SELECTOR_TX_SEL_PRE_AGG_SEL_REQ_BITS0 0x380 */
#define B0_TX_SELECTOR_TX_SEL_PRE_AGG_SEL_REQ_BITS0_FIELD_PRE_AGG_DL_MASK                                         0x00000001
#define B0_TX_SELECTOR_TX_SEL_PRE_AGG_SEL_REQ_BITS0_FIELD_PRE_AGG_MGMT_ONLY_MASK                                  0x00000002
#define B0_TX_SELECTOR_TX_SEL_PRE_AGG_SEL_REQ_BITS0_FIELD_PRE_AGG_POLICY_MASK                                     0x0000000C
#define B0_TX_SELECTOR_TX_SEL_PRE_AGG_SEL_REQ_BITS0_FIELD_PRE_AGG_LEGACY_PS_EN_MASK                               0x00000010
#define B0_TX_SELECTOR_TX_SEL_PRE_AGG_SEL_REQ_BITS0_FIELD_PRE_AGG_HE_MU_ONLY_MASK                                 0x00000020
#define B0_TX_SELECTOR_TX_SEL_PRE_AGG_SEL_REQ_BITS0_FIELD_PRE_AGG_OMI_CONTROL_MASK                                0x000000C0
#define B0_TX_SELECTOR_TX_SEL_PRE_AGG_SEL_REQ_BITS0_FIELD_PRE_AGG_STA_NUM_MASK                                    0x0000FF00
#define B0_TX_SELECTOR_TX_SEL_PRE_AGG_SEL_REQ_BITS0_FIELD_PRE_AGG_TID_EN_MASK                                     0x01FF0000
#define B0_TX_SELECTOR_TX_SEL_PRE_AGG_SEL_REQ_BITS0_FIELD_PRE_AGG_MAX_SEL_TID_NUM_MASK                            0x0E000000
#define B0_TX_SELECTOR_TX_SEL_PRE_AGG_SEL_REQ_BITS0_FIELD_PRE_AGG_PRIMARY_TID_MASK                                0xE0000000
//========================================
/* REG_TX_SELECTOR_TX_SEL_PRE_AGG_SEL_REQ_BITS1 0x384 */
#define B0_TX_SELECTOR_TX_SEL_PRE_AGG_SEL_REQ_BITS1_FIELD_PRE_AGG_TID_IN_RETRY_MASK                               0x000001FF
#define B0_TX_SELECTOR_TX_SEL_PRE_AGG_SEL_REQ_BITS1_FIELD_PRE_AGG_HW_LOCKED_TID_MASK                              0x001FF000
#define B0_TX_SELECTOR_TX_SEL_PRE_AGG_SEL_REQ_BITS1_FIELD_VALIDATE_DL_HE_MU_EN_MASK                               0x00400000
#define B0_TX_SELECTOR_TX_SEL_PRE_AGG_SEL_REQ_BITS1_FIELD_PRE_AGG_DL_HE_PS_WITHOUT_REQ_EN_MASK                    0x00800000
#define B0_TX_SELECTOR_TX_SEL_PRE_AGG_SEL_REQ_BITS1_FIELD_PRE_AGG_LOCK_SELECTED_MODE_MASK                         0x03000000
#define B0_TX_SELECTOR_TX_SEL_PRE_AGG_SEL_REQ_BITS1_FIELD_PRE_AGG_VAP_IDX_MASK                                    0xF8000000
//========================================
/* REG_TX_SELECTOR_TX_SEL_PRE_AGG_SEL_RESULT_BITS0 0x388 */
#define B0_TX_SELECTOR_TX_SEL_PRE_AGG_SEL_RESULT_BITS0_FIELD_PRE_AGG_RESULT_SELECTED_TIDS_MASK                    0x000001FF
#define B0_TX_SELECTOR_TX_SEL_PRE_AGG_SEL_RESULT_BITS0_FIELD_PRE_AGG_RESULT_SELECTOR_LOCKED_TIDS_MASK             0x0003FE00
#define B0_TX_SELECTOR_TX_SEL_PRE_AGG_SEL_RESULT_BITS0_FIELD_PRE_AGG_STA_NOT_SELECTED_REASON_MASK                 0x7FF00000
//========================================
/* REG_TX_SELECTOR_TX_SEL_PRE_AGG_SEL_RESULT_BITS1 0x38C */
#define B0_TX_SELECTOR_TX_SEL_PRE_AGG_SEL_RESULT_BITS1_FIELD_PRE_AGG_SEL_STATUS0_31_MASK                          0xFFFFFFFF
//========================================
/* REG_TX_SELECTOR_TX_SEL_PRE_AGG_SEL_RESULT_BITS2 0x390 */
#define B0_TX_SELECTOR_TX_SEL_PRE_AGG_SEL_RESULT_BITS2_FIELD_PRE_AGG_UL_STA_TID_DATA_IN_Q_MASK                    0x000000FF
#define B0_TX_SELECTOR_TX_SEL_PRE_AGG_SEL_RESULT_BITS2_FIELD_PRE_AGG_TWT_AVAIL_MASK                               0x00000100
#define B0_TX_SELECTOR_TX_SEL_PRE_AGG_SEL_RESULT_BITS2_FIELD_PRE_AGG_TWT_SP_MASK                                  0x00000200
#define B0_TX_SELECTOR_TX_SEL_PRE_AGG_SEL_RESULT_BITS2_FIELD_PRE_AGG_TWT_ANN_MASK                                 0x00000400
#define B0_TX_SELECTOR_TX_SEL_PRE_AGG_SEL_RESULT_BITS2_FIELD_PRE_AGG_OMI_VALID_MASK                               0x00000800
#define B0_TX_SELECTOR_TX_SEL_PRE_AGG_SEL_RESULT_BITS2_FIELD_PRE_AGG_OMI_SUPP_MASK                                0x00001000
#define B0_TX_SELECTOR_TX_SEL_PRE_AGG_SEL_RESULT_BITS2_FIELD_PRE_AGG_TOMI_DIS_MASK                                0x00002000
#define B0_TX_SELECTOR_TX_SEL_PRE_AGG_SEL_RESULT_BITS2_FIELD_PRE_AGG_OMI_BW_MASK                                  0x0001C000
#define B0_TX_SELECTOR_TX_SEL_PRE_AGG_SEL_RESULT_BITS2_FIELD_PRE_AGG_TOMI_MAX_NSS_MASK                            0x000E0000
#define B0_TX_SELECTOR_TX_SEL_PRE_AGG_SEL_RESULT_BITS2_FIELD_PRE_AGG_ROMI_MAX_NSS_MASK                            0x00700000
#define B0_TX_SELECTOR_TX_SEL_PRE_AGG_SEL_RESULT_BITS2_FIELD_PRE_AGG_DISABLE_HE_SU_ER_MASK                        0x00800000
#define B0_TX_SELECTOR_TX_SEL_PRE_AGG_SEL_RESULT_BITS2_FIELD_PRE_AGG_INSERT_A_CONTROL_MASK                        0x01000000
#define B0_TX_SELECTOR_TX_SEL_PRE_AGG_SEL_RESULT_BITS2_FIELD_PRE_AGG_UL_HE_MU_EN_MASK                             0x02000000
#define B0_TX_SELECTOR_TX_SEL_PRE_AGG_SEL_RESULT_BITS2_FIELD_PRE_AGG_SEL_STATUS32_35_MASK                         0x3C000000
#define B0_TX_SELECTOR_TX_SEL_PRE_AGG_SEL_RESULT_BITS2_FIELD_PRE_AGG_SP_VALID_IN_VAP_MASK                         0x40000000
//========================================
/* REG_TX_SELECTOR_TX_SEL_CHICKEN_BITS 0x394 */
#define B0_TX_SELECTOR_TX_SEL_CHICKEN_BITS_FIELD_CR_KEEP_SEL_BITMAP_ORDER_MASK                                    0x00000001
#define B0_TX_SELECTOR_TX_SEL_CHICKEN_BITS_FIELD_CR_IGNORE_PS_REQ_IN_UL_SEL_STA_PS_LEGACY_MASK                    0x00000002
#define B0_TX_SELECTOR_TX_SEL_CHICKEN_BITS_FIELD_CR_IGNORE_PS_REQ_IN_UL_SEL_STA_PS_WMM_MASK                       0x00000004
#define B0_TX_SELECTOR_TX_SEL_CHICKEN_BITS_FIELD_CR_IGNORE_RECALC_MASK                                            0x00000008
#define B0_TX_SELECTOR_TX_SEL_CHICKEN_BITS_FIELD_CR_IGNORE_TGEN_BI_UL_DATA_IN_Q_MASK                              0x00000010
//========================================
/* REG_TX_SELECTOR_TX_SEL_MLD_VAP_PS 0x398 */
#define B0_TX_SELECTOR_TX_SEL_MLD_VAP_PS_FIELD_CR_VAP_PS_MASK                                                     0xFFFFFFFF


#endif // _TX_SELECTOR_REGS_H_
