
/***********************************************************************************
File:				TimGenRegs.h
Module:				timGen
SOC Revision:		latest
Generated at:       2024-06-26 12:55:09
Purpose:
Description:		This File was auto generated using SOC Online

************************************************************************************/
#ifndef _TIM_GEN_REGS_H_
#define _TIM_GEN_REGS_H_

/*---------------------------------------------------------------------------------
/						Registers Addresses													 
/----------------------------------------------------------------------------------*/
#include "HwMemoryMap.h"

#define TIM_GEN_BASE_ADDRESS                      MEMORY_MAP_UNIT_1080_BASE_ADDRESS
#define	REG_TIM_GEN_BAND0_TIM_GEN_CTRL          (TIM_GEN_BASE_ADDRESS + 0x0)
#define	REG_TIM_GEN_BAND0_TIM_GEN_DBG           (TIM_GEN_BASE_ADDRESS + 0x4)
#define	REG_TIM_GEN_BAND0_START_VAP_ID          (TIM_GEN_BASE_ADDRESS + 0xC)
#define	REG_TIM_GEN_BAND0_SM_BUSY               (TIM_GEN_BASE_ADDRESS + 0x10)
#define	REG_TIM_GEN_BAND0_VAP_ID                (TIM_GEN_BASE_ADDRESS + 0x14)
#define	REG_TIM_GEN_BAND0_B_VAR                 (TIM_GEN_BASE_ADDRESS + 0x18)
#define	REG_TIM_GEN_BAND0_DTIM_PARAM            (TIM_GEN_BASE_ADDRESS + 0x1C)
#define	REG_TIM_GEN_BAND0_TIM_HDR_BITMAP0       (TIM_GEN_BASE_ADDRESS + 0x20)
#define	REG_TIM_GEN_BAND0_TIM_HDR_BITMAP1       (TIM_GEN_BASE_ADDRESS + 0x24)
#define	REG_TIM_GEN_BAND0_TIM_HDR_BITMAP2       (TIM_GEN_BASE_ADDRESS + 0x28)
#define	REG_TIM_GEN_BAND0_TIM_HDR_BITMAP3       (TIM_GEN_BASE_ADDRESS + 0x2C)
#define	REG_TIM_GEN_BAND0_TIM_HDR_BITMAP4       (TIM_GEN_BASE_ADDRESS + 0x30)
#define	REG_TIM_GEN_BAND0_TIM_HDR_BITMAP5       (TIM_GEN_BASE_ADDRESS + 0x34)
#define	REG_TIM_GEN_BAND0_TIM_HDR_BITMAP6       (TIM_GEN_BASE_ADDRESS + 0x38)
#define	REG_TIM_GEN_BAND0_TIM_HDR_BITMAP7       (TIM_GEN_BASE_ADDRESS + 0x3C)
#define	REG_TIM_GEN_BAND0_TIM_HDR_BITMAP8       (TIM_GEN_BASE_ADDRESS + 0x40)
#define	REG_TIM_GEN_BAND0_TIM_HDR_BITMAP9       (TIM_GEN_BASE_ADDRESS + 0x44)
#define	REG_TIM_GEN_BAND0_TIM_HDR_BITMAP10      (TIM_GEN_BASE_ADDRESS + 0x48)
#define	REG_TIM_GEN_BAND0_TIM_HDR_BITMAP11      (TIM_GEN_BASE_ADDRESS + 0x4C)
#define	REG_TIM_GEN_BAND0_TIM_HDR_BITMAP12      (TIM_GEN_BASE_ADDRESS + 0x50)
#define	REG_TIM_GEN_BAND0_TIM_HDR_BITMAP13      (TIM_GEN_BASE_ADDRESS + 0x54)
#define	REG_TIM_GEN_BAND0_TIM_HDR_BITMAP14      (TIM_GEN_BASE_ADDRESS + 0x58)
#define	REG_TIM_GEN_BAND0_TIM_HDR_BITMAP15      (TIM_GEN_BASE_ADDRESS + 0x5C)
#define	REG_TIM_GEN_BAND0_TIM_CASE              (TIM_GEN_BASE_ADDRESS + 0x60)
#define	REG_TIM_GEN_BAND0_MLTI_HDR_LISTOUT0     (TIM_GEN_BASE_ADDRESS + 0x80)
#define	REG_TIM_GEN_BAND0_MLTI_HDR_LISTOUT1     (TIM_GEN_BASE_ADDRESS + 0x84)
#define	REG_TIM_GEN_BAND0_MLTI_HDR_LISTOUT2     (TIM_GEN_BASE_ADDRESS + 0x88)
#define	REG_TIM_GEN_BAND0_MLTI_HDR_LISTOUT3     (TIM_GEN_BASE_ADDRESS + 0x8C)
#define	REG_TIM_GEN_BAND0_MLTI_HDR_LISTOUT4     (TIM_GEN_BASE_ADDRESS + 0x90)
#define	REG_TIM_GEN_BAND0_MLTI_HDR_LISTOUT5     (TIM_GEN_BASE_ADDRESS + 0x94)
#define	REG_TIM_GEN_BAND0_MLTI_HDR_LISTOUT6     (TIM_GEN_BASE_ADDRESS + 0x98)
#define	REG_TIM_GEN_BAND0_MLTI_HDR_LISTOUT7     (TIM_GEN_BASE_ADDRESS + 0x9C)
#define	REG_TIM_GEN_BAND0_MLTI_HDR_LISTOUT8     (TIM_GEN_BASE_ADDRESS + 0xA0)
#define	REG_TIM_GEN_BAND0_MLTI_HDR_LISTOUT9     (TIM_GEN_BASE_ADDRESS + 0xA4)
#define	REG_TIM_GEN_BAND0_MLTI_HDR_LISTOUT10    (TIM_GEN_BASE_ADDRESS + 0xA8)
#define	REG_TIM_GEN_BAND0_MLTI_HDR_LISTOUT11    (TIM_GEN_BASE_ADDRESS + 0xAC)
#define	REG_TIM_GEN_BAND0_MLTI_HDR_LISTOUT12    (TIM_GEN_BASE_ADDRESS + 0xB0)
#define	REG_TIM_GEN_BAND0_MLTI_HDR_LISTOUT13    (TIM_GEN_BASE_ADDRESS + 0xB4)
#define	REG_TIM_GEN_BAND0_TI_MLD_COPY0          (TIM_GEN_BASE_ADDRESS + 0xB8)
#define	REG_TIM_GEN_BAND0_TI_MLD_COPY1          (TIM_GEN_BASE_ADDRESS + 0xBC)
#define	REG_TIM_GEN_BAND0_TI_MLD_COPY2          (TIM_GEN_BASE_ADDRESS + 0xC0)
#define	REG_TIM_GEN_BAND0_TI_MLD_COPY3          (TIM_GEN_BASE_ADDRESS + 0xC4)
#define	REG_TIM_GEN_BAND1_TIM_GEN_CTRL          (TIM_GEN_BASE_ADDRESS + 0x100)
#define	REG_TIM_GEN_BAND1_TIM_GEN_DBG           (TIM_GEN_BASE_ADDRESS + 0x104)
#define	REG_TIM_GEN_BAND1_START_VAP_ID          (TIM_GEN_BASE_ADDRESS + 0x10C)
#define	REG_TIM_GEN_BAND1_SM_BUSY               (TIM_GEN_BASE_ADDRESS + 0x110)
#define	REG_TIM_GEN_BAND1_VAP_ID                (TIM_GEN_BASE_ADDRESS + 0x114)
#define	REG_TIM_GEN_BAND1_B_VAR                 (TIM_GEN_BASE_ADDRESS + 0x118)
#define	REG_TIM_GEN_BAND1_DTIM_PARAM            (TIM_GEN_BASE_ADDRESS + 0x11C)
#define	REG_TIM_GEN_BAND1_TIM_HDR_BITMAP0       (TIM_GEN_BASE_ADDRESS + 0x120)
#define	REG_TIM_GEN_BAND1_TIM_HDR_BITMAP1       (TIM_GEN_BASE_ADDRESS + 0x124)
#define	REG_TIM_GEN_BAND1_TIM_HDR_BITMAP2       (TIM_GEN_BASE_ADDRESS + 0x128)
#define	REG_TIM_GEN_BAND1_TIM_HDR_BITMAP3       (TIM_GEN_BASE_ADDRESS + 0x12C)
#define	REG_TIM_GEN_BAND1_TIM_HDR_BITMAP4       (TIM_GEN_BASE_ADDRESS + 0x130)
#define	REG_TIM_GEN_BAND1_TIM_HDR_BITMAP5       (TIM_GEN_BASE_ADDRESS + 0x134)
#define	REG_TIM_GEN_BAND1_TIM_HDR_BITMAP6       (TIM_GEN_BASE_ADDRESS + 0x138)
#define	REG_TIM_GEN_BAND1_TIM_HDR_BITMAP7       (TIM_GEN_BASE_ADDRESS + 0x13C)
#define	REG_TIM_GEN_BAND1_TIM_HDR_BITMAP8       (TIM_GEN_BASE_ADDRESS + 0x140)
#define	REG_TIM_GEN_BAND1_TIM_HDR_BITMAP9       (TIM_GEN_BASE_ADDRESS + 0x144)
#define	REG_TIM_GEN_BAND1_TIM_HDR_BITMAP10      (TIM_GEN_BASE_ADDRESS + 0x148)
#define	REG_TIM_GEN_BAND1_TIM_HDR_BITMAP11      (TIM_GEN_BASE_ADDRESS + 0x14C)
#define	REG_TIM_GEN_BAND1_TIM_HDR_BITMAP12      (TIM_GEN_BASE_ADDRESS + 0x150)
#define	REG_TIM_GEN_BAND1_TIM_HDR_BITMAP13      (TIM_GEN_BASE_ADDRESS + 0x154)
#define	REG_TIM_GEN_BAND1_TIM_HDR_BITMAP14      (TIM_GEN_BASE_ADDRESS + 0x158)
#define	REG_TIM_GEN_BAND1_TIM_HDR_BITMAP15      (TIM_GEN_BASE_ADDRESS + 0x15C)
#define	REG_TIM_GEN_BAND1_TIM_CASE              (TIM_GEN_BASE_ADDRESS + 0x160)
#define	REG_TIM_GEN_BAND1_MLTI_HDR_LISTOUT0     (TIM_GEN_BASE_ADDRESS + 0x180)
#define	REG_TIM_GEN_BAND1_MLTI_HDR_LISTOUT1     (TIM_GEN_BASE_ADDRESS + 0x184)
#define	REG_TIM_GEN_BAND1_MLTI_HDR_LISTOUT2     (TIM_GEN_BASE_ADDRESS + 0x188)
#define	REG_TIM_GEN_BAND1_MLTI_HDR_LISTOUT3     (TIM_GEN_BASE_ADDRESS + 0x18C)
#define	REG_TIM_GEN_BAND1_MLTI_HDR_LISTOUT4     (TIM_GEN_BASE_ADDRESS + 0x190)
#define	REG_TIM_GEN_BAND1_MLTI_HDR_LISTOUT5     (TIM_GEN_BASE_ADDRESS + 0x194)
#define	REG_TIM_GEN_BAND1_MLTI_HDR_LISTOUT6     (TIM_GEN_BASE_ADDRESS + 0x198)
#define	REG_TIM_GEN_BAND1_MLTI_HDR_LISTOUT7     (TIM_GEN_BASE_ADDRESS + 0x19C)
#define	REG_TIM_GEN_BAND1_MLTI_HDR_LISTOUT8     (TIM_GEN_BASE_ADDRESS + 0x1A0)
#define	REG_TIM_GEN_BAND1_MLTI_HDR_LISTOUT9     (TIM_GEN_BASE_ADDRESS + 0x1A4)
#define	REG_TIM_GEN_BAND1_MLTI_HDR_LISTOUT10    (TIM_GEN_BASE_ADDRESS + 0x1A8)
#define	REG_TIM_GEN_BAND1_MLTI_HDR_LISTOUT11    (TIM_GEN_BASE_ADDRESS + 0x1AC)
#define	REG_TIM_GEN_BAND1_MLTI_HDR_LISTOUT12    (TIM_GEN_BASE_ADDRESS + 0x1B0)
#define	REG_TIM_GEN_BAND1_MLTI_HDR_LISTOUT13    (TIM_GEN_BASE_ADDRESS + 0x1B4)
#define	REG_TIM_GEN_BAND1_TI_MLD_COPY0          (TIM_GEN_BASE_ADDRESS + 0x1B8)
#define	REG_TIM_GEN_BAND1_TI_MLD_COPY1          (TIM_GEN_BASE_ADDRESS + 0x1BC)
#define	REG_TIM_GEN_BAND1_TI_MLD_COPY2          (TIM_GEN_BASE_ADDRESS + 0x1C0)
#define	REG_TIM_GEN_BAND1_TI_MLD_COPY3          (TIM_GEN_BASE_ADDRESS + 0x1C4)
#define	REG_TIM_GEN_BAND2_TIM_GEN_CTRL          (TIM_GEN_BASE_ADDRESS + 0x200)
#define	REG_TIM_GEN_BAND2_TIM_GEN_DBG           (TIM_GEN_BASE_ADDRESS + 0x204)
#define	REG_TIM_GEN_BAND2_START_VAP_ID          (TIM_GEN_BASE_ADDRESS + 0x20C)
#define	REG_TIM_GEN_BAND2_SM_BUSY               (TIM_GEN_BASE_ADDRESS + 0x210)
#define	REG_TIM_GEN_BAND2_VAP_ID                (TIM_GEN_BASE_ADDRESS + 0x214)
#define	REG_TIM_GEN_BAND2_B_VAR                 (TIM_GEN_BASE_ADDRESS + 0x218)
#define	REG_TIM_GEN_BAND2_DTIM_PARAM            (TIM_GEN_BASE_ADDRESS + 0x21C)
#define	REG_TIM_GEN_BAND2_TIM_HDR_BITMAP0       (TIM_GEN_BASE_ADDRESS + 0x220)
#define	REG_TIM_GEN_BAND2_TIM_HDR_BITMAP1       (TIM_GEN_BASE_ADDRESS + 0x224)
#define	REG_TIM_GEN_BAND2_TIM_HDR_BITMAP2       (TIM_GEN_BASE_ADDRESS + 0x228)
#define	REG_TIM_GEN_BAND2_TIM_HDR_BITMAP3       (TIM_GEN_BASE_ADDRESS + 0x22C)
#define	REG_TIM_GEN_BAND2_TIM_HDR_BITMAP4       (TIM_GEN_BASE_ADDRESS + 0x230)
#define	REG_TIM_GEN_BAND2_TIM_HDR_BITMAP5       (TIM_GEN_BASE_ADDRESS + 0x234)
#define	REG_TIM_GEN_BAND2_TIM_HDR_BITMAP6       (TIM_GEN_BASE_ADDRESS + 0x238)
#define	REG_TIM_GEN_BAND2_TIM_HDR_BITMAP7       (TIM_GEN_BASE_ADDRESS + 0x23C)
#define	REG_TIM_GEN_BAND2_TIM_HDR_BITMAP8       (TIM_GEN_BASE_ADDRESS + 0x240)
#define	REG_TIM_GEN_BAND2_TIM_HDR_BITMAP9       (TIM_GEN_BASE_ADDRESS + 0x244)
#define	REG_TIM_GEN_BAND2_TIM_HDR_BITMAP10      (TIM_GEN_BASE_ADDRESS + 0x248)
#define	REG_TIM_GEN_BAND2_TIM_HDR_BITMAP11      (TIM_GEN_BASE_ADDRESS + 0x24C)
#define	REG_TIM_GEN_BAND2_TIM_HDR_BITMAP12      (TIM_GEN_BASE_ADDRESS + 0x250)
#define	REG_TIM_GEN_BAND2_TIM_HDR_BITMAP13      (TIM_GEN_BASE_ADDRESS + 0x254)
#define	REG_TIM_GEN_BAND2_TIM_HDR_BITMAP14      (TIM_GEN_BASE_ADDRESS + 0x258)
#define	REG_TIM_GEN_BAND2_TIM_HDR_BITMAP15      (TIM_GEN_BASE_ADDRESS + 0x25C)
#define	REG_TIM_GEN_BAND2_TIM_CASE              (TIM_GEN_BASE_ADDRESS + 0x260)
#define	REG_TIM_GEN_BAND2_MLTI_HDR_LISTOUT0     (TIM_GEN_BASE_ADDRESS + 0x280)
#define	REG_TIM_GEN_BAND2_MLTI_HDR_LISTOUT1     (TIM_GEN_BASE_ADDRESS + 0x284)
#define	REG_TIM_GEN_BAND2_MLTI_HDR_LISTOUT2     (TIM_GEN_BASE_ADDRESS + 0x288)
#define	REG_TIM_GEN_BAND2_MLTI_HDR_LISTOUT3     (TIM_GEN_BASE_ADDRESS + 0x28C)
#define	REG_TIM_GEN_BAND2_MLTI_HDR_LISTOUT4     (TIM_GEN_BASE_ADDRESS + 0x290)
#define	REG_TIM_GEN_BAND2_MLTI_HDR_LISTOUT5     (TIM_GEN_BASE_ADDRESS + 0x294)
#define	REG_TIM_GEN_BAND2_MLTI_HDR_LISTOUT6     (TIM_GEN_BASE_ADDRESS + 0x298)
#define	REG_TIM_GEN_BAND2_MLTI_HDR_LISTOUT7     (TIM_GEN_BASE_ADDRESS + 0x29C)
#define	REG_TIM_GEN_BAND2_MLTI_HDR_LISTOUT8     (TIM_GEN_BASE_ADDRESS + 0x2A0)
#define	REG_TIM_GEN_BAND2_MLTI_HDR_LISTOUT9     (TIM_GEN_BASE_ADDRESS + 0x2A4)
#define	REG_TIM_GEN_BAND2_MLTI_HDR_LISTOUT10    (TIM_GEN_BASE_ADDRESS + 0x2A8)
#define	REG_TIM_GEN_BAND2_MLTI_HDR_LISTOUT11    (TIM_GEN_BASE_ADDRESS + 0x2AC)
#define	REG_TIM_GEN_BAND2_MLTI_HDR_LISTOUT12    (TIM_GEN_BASE_ADDRESS + 0x2B0)
#define	REG_TIM_GEN_BAND2_MLTI_HDR_LISTOUT13    (TIM_GEN_BASE_ADDRESS + 0x2B4)
#define	REG_TIM_GEN_BAND2_TI_MLD_COPY0          (TIM_GEN_BASE_ADDRESS + 0x2B8)
#define	REG_TIM_GEN_BAND2_TI_MLD_COPY1          (TIM_GEN_BASE_ADDRESS + 0x2BC)
#define	REG_TIM_GEN_BAND2_TI_MLD_COPY2          (TIM_GEN_BASE_ADDRESS + 0x2C0)
#define	REG_TIM_GEN_BAND2_TI_MLD_COPY3          (TIM_GEN_BASE_ADDRESS + 0x2C4)
#define	REG_TIM_GEN_DTIM0                       (TIM_GEN_BASE_ADDRESS + 0x300)
#define	REG_TIM_GEN_DTIM1                       (TIM_GEN_BASE_ADDRESS + 0x304)
#define	REG_TIM_GEN_VAP_BI0                     (TIM_GEN_BASE_ADDRESS + 0x308)
#define	REG_TIM_GEN_VAP_BI1                     (TIM_GEN_BASE_ADDRESS + 0x30C)
#define	REG_TIM_GEN_VALID_STA0                  (TIM_GEN_BASE_ADDRESS + 0x310)
#define	REG_TIM_GEN_VALID_STA1                  (TIM_GEN_BASE_ADDRESS + 0x314)
#define	REG_TIM_GEN_VALID_STA2                  (TIM_GEN_BASE_ADDRESS + 0x318)
#define	REG_TIM_GEN_VALID_STA3                  (TIM_GEN_BASE_ADDRESS + 0x31C)
#define	REG_TIM_GEN_VALID_STA4                  (TIM_GEN_BASE_ADDRESS + 0x320)
#define	REG_TIM_GEN_VALID_STA5                  (TIM_GEN_BASE_ADDRESS + 0x324)
#define	REG_TIM_GEN_VALID_STA6                  (TIM_GEN_BASE_ADDRESS + 0x328)
#define	REG_TIM_GEN_VALID_STA7                  (TIM_GEN_BASE_ADDRESS + 0x32C)
#define	REG_TIM_GEN_VALID_STA8                  (TIM_GEN_BASE_ADDRESS + 0x330)
#define	REG_TIM_GEN_VALID_STA9                  (TIM_GEN_BASE_ADDRESS + 0x334)
#define	REG_TIM_GEN_VALID_STA10                 (TIM_GEN_BASE_ADDRESS + 0x338)
#define	REG_TIM_GEN_VALID_STA11                 (TIM_GEN_BASE_ADDRESS + 0x33C)
#define	REG_TIM_GEN_VALID_STA12                 (TIM_GEN_BASE_ADDRESS + 0x340)
#define	REG_TIM_GEN_VALID_STA13                 (TIM_GEN_BASE_ADDRESS + 0x344)
#define	REG_TIM_GEN_VALID_STA14                 (TIM_GEN_BASE_ADDRESS + 0x348)
#define	REG_TIM_GEN_VALID_STA15                 (TIM_GEN_BASE_ADDRESS + 0x34C)
#define	REG_TIM_GEN_STA_PS0                     (TIM_GEN_BASE_ADDRESS + 0x350)
#define	REG_TIM_GEN_STA_PS1                     (TIM_GEN_BASE_ADDRESS + 0x354)
#define	REG_TIM_GEN_STA_PS2                     (TIM_GEN_BASE_ADDRESS + 0x358)
#define	REG_TIM_GEN_STA_PS3                     (TIM_GEN_BASE_ADDRESS + 0x35C)
#define	REG_TIM_GEN_STA_PS4                     (TIM_GEN_BASE_ADDRESS + 0x360)
#define	REG_TIM_GEN_STA_PS5                     (TIM_GEN_BASE_ADDRESS + 0x364)
#define	REG_TIM_GEN_STA_PS6                     (TIM_GEN_BASE_ADDRESS + 0x368)
#define	REG_TIM_GEN_STA_PS7                     (TIM_GEN_BASE_ADDRESS + 0x36C)
#define	REG_TIM_GEN_STA_PS8                     (TIM_GEN_BASE_ADDRESS + 0x370)
#define	REG_TIM_GEN_STA_PS9                     (TIM_GEN_BASE_ADDRESS + 0x374)
#define	REG_TIM_GEN_STA_PS10                    (TIM_GEN_BASE_ADDRESS + 0x378)
#define	REG_TIM_GEN_STA_PS11                    (TIM_GEN_BASE_ADDRESS + 0x37C)
#define	REG_TIM_GEN_STA_PS12                    (TIM_GEN_BASE_ADDRESS + 0x380)
#define	REG_TIM_GEN_STA_PS13                    (TIM_GEN_BASE_ADDRESS + 0x384)
#define	REG_TIM_GEN_STA_PS14                    (TIM_GEN_BASE_ADDRESS + 0x388)
#define	REG_TIM_GEN_STA_PS15                    (TIM_GEN_BASE_ADDRESS + 0x38C)
#define	REG_TIM_GEN_TI_STA0                     (TIM_GEN_BASE_ADDRESS + 0x390)
#define	REG_TIM_GEN_TI_STA1                     (TIM_GEN_BASE_ADDRESS + 0x394)
#define	REG_TIM_GEN_TI_STA2                     (TIM_GEN_BASE_ADDRESS + 0x398)
#define	REG_TIM_GEN_TI_STA3                     (TIM_GEN_BASE_ADDRESS + 0x39C)
#define	REG_TIM_GEN_TI_STA4                     (TIM_GEN_BASE_ADDRESS + 0x3A0)
#define	REG_TIM_GEN_TI_STA5                     (TIM_GEN_BASE_ADDRESS + 0x3A4)
#define	REG_TIM_GEN_TI_STA6                     (TIM_GEN_BASE_ADDRESS + 0x3A8)
#define	REG_TIM_GEN_TI_STA7                     (TIM_GEN_BASE_ADDRESS + 0x3AC)
#define	REG_TIM_GEN_TI_STA8                     (TIM_GEN_BASE_ADDRESS + 0x3B0)
#define	REG_TIM_GEN_TI_STA9                     (TIM_GEN_BASE_ADDRESS + 0x3B4)
#define	REG_TIM_GEN_TI_STA10                    (TIM_GEN_BASE_ADDRESS + 0x3B8)
#define	REG_TIM_GEN_TI_STA11                    (TIM_GEN_BASE_ADDRESS + 0x3BC)
#define	REG_TIM_GEN_TI_STA12                    (TIM_GEN_BASE_ADDRESS + 0x3C0)
#define	REG_TIM_GEN_TI_STA13                    (TIM_GEN_BASE_ADDRESS + 0x3C4)
#define	REG_TIM_GEN_TI_STA14                    (TIM_GEN_BASE_ADDRESS + 0x3C8)
#define	REG_TIM_GEN_TI_STA15                    (TIM_GEN_BASE_ADDRESS + 0x3CC)
#define	REG_TIM_GEN_MLTI_LISTIN0                (TIM_GEN_BASE_ADDRESS + 0x3D0)
#define	REG_TIM_GEN_MLTI_LISTIN1                (TIM_GEN_BASE_ADDRESS + 0x3D4)
#define	REG_TIM_GEN_MLTI_LISTIN2                (TIM_GEN_BASE_ADDRESS + 0x3D8)
#define	REG_TIM_GEN_MLTI_LISTIN3                (TIM_GEN_BASE_ADDRESS + 0x3DC)
#define	REG_TIM_GEN_MLTI_LISTIN4                (TIM_GEN_BASE_ADDRESS + 0x3E0)
#define	REG_TIM_GEN_MLTI_LISTIN5                (TIM_GEN_BASE_ADDRESS + 0x3E4)
#define	REG_TIM_GEN_MLTI_LISTIN6                (TIM_GEN_BASE_ADDRESS + 0x3E8)
#define	REG_TIM_GEN_MLTI_LISTIN7                (TIM_GEN_BASE_ADDRESS + 0x3EC)
#define	REG_TIM_GEN_MLTI_LISTIN8                (TIM_GEN_BASE_ADDRESS + 0x3F0)
#define	REG_TIM_GEN_MLTI_LISTIN9                (TIM_GEN_BASE_ADDRESS + 0x3F4)
#define	REG_TIM_GEN_MLTI_LISTIN10               (TIM_GEN_BASE_ADDRESS + 0x3F8)
#define	REG_TIM_GEN_MLTI_LISTIN11               (TIM_GEN_BASE_ADDRESS + 0x3FC)
/*---------------------------------------------------------------------------------
/						Data Type Definition										
/----------------------------------------------------------------------------------*/
/*REG_TIM_GEN_BAND0_TIM_GEN_CTRL 0x0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band0TimGenEnable : 1; //Enable TIM generation BAND0    , reset value: 0x0, access type: RW
		uint32 band0MltiGenEnable : 1; //Enable MLTI generation BAND0    , reset value: 0x0, access type: RW
		uint32 band0SegBZeroI : 1; //Indicates to set Segment B with value all 0, reset value: 0x0, access type: RW
		uint32 band0SegBDtimI : 1; //Indicates to ignore the DTIM of the reported AP MLDs, reset value: 0x0, access type: RW
		uint32 band0SegBBitmapSizeI : 2; //Indicates the size (in bits) of the reported APs bitmap.Possible values: 1, 3., reset value: 0x3, access type: RW
		uint32 reserved0 : 10;
		uint32 band0MaxNumOfBssidsI : 5; //Used by FW to set the maximum number of members in multiple BSSID set. If M-BSSID is supported then possible values are 2, 4, 8, 16, 32; else the value is 1., reset value: 0x0, access type: RW
		uint32 reserved1 : 11;
	} bitFields;
} RegTimGenBand0TimGenCtrl_u;

/*REG_TIM_GEN_BAND0_TIM_GEN_DBG 0x4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band0InvalidStaInt : 1; //Error event - Access to non valid MLD DB entry , reset value: 0x0, access type: RO
		uint32 band0InvalidVapInt : 1; //Error event - Access to non valid VAP CFG table entry , reset value: 0x0, access type: RO
		uint32 band0FalseStartInt : 1; //Error event - Atempt to start during busy , reset value: 0x0, access type: RO
		uint32 reserved0 : 1;
		uint32 band0InvalidStaClr : 1; //Error event Clear (Write 1 to clear) - Access to non valid MLD DB entry , reset value: 0x0, access type: WO
		uint32 band0InvalidVapClr : 1; //Error event Clear (Write 1 to clear) - Access to non valid VAP CFG table entry , reset value: 0x0, access type: WO
		uint32 band0FalseStartClr : 1; //Error event Clear (Write 1 to clear) - Atempt to start during busy , reset value: 0x0, access type: WO
		uint32 reserved1 : 9;
		uint32 band0FsmState : 4; //FSM State, reset value: 0x0, access type: RO
		uint32 reserved2 : 12;
	} bitFields;
} RegTimGenBand0TimGenDbg_u;

/*REG_TIM_GEN_BAND0_START_VAP_ID 0xC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band0StartVapIdI : 6; //Can be used by Sender to trigger the SM. Start includes the VAP_ID (6b) of the transmitted BSSID , (WR transaction to this register starting TIM GEN FSM of corresponding BAND) , reset value: 0x0, access type: RW
		uint32 reserved0 : 26;
	} bitFields;
} RegTimGenBand0StartVapId_u;

/*REG_TIM_GEN_BAND0_SM_BUSY 0x10 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band0SmBusyI : 1; //sm_busy indicates SM is in progress, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegTimGenBand0SmBusy_u;

/*REG_TIM_GEN_BAND0_VAP_ID 0x14 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band0VapIdI : 6; //The VAP_ID provided by the Selector, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegTimGenBand0VapId_u;

/*REG_TIM_GEN_BAND0_B_VAR 0x18 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band0B0VarI : 9; //Intermediate variable B0, 9b Result of SM, reset value: 0x0, access type: RO
		uint32 band0B1VarI : 9; //Intermediate variable B1, 9b Result of SM, reset value: 0x0, access type: RO
		uint32 band0B2VarI : 9; //Intermediate variable B2, 9b Result of SM, reset value: 0x0, access type: RO
		uint32 reserved0 : 5;
	} bitFields;
} RegTimGenBand0BVar_u;

/*REG_TIM_GEN_BAND0_DTIM_PARAM 0x1C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band0DtimCountI : 8; //Reflects the DTIM Count  from selector, reset value: 0x0, access type: RO
		uint32 reserved0 : 8;
		uint32 band0DtimPeriodI : 8; //Reflects the DTIM Period from selector, reset value: 0x0, access type: RO
		uint32 reserved1 : 8;
	} bitFields;
} RegTimGenBand0DtimParam_u;

/*REG_TIM_GEN_BAND0_TIM_HDR_BITMAP0 0x20 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band0TimHdrBitmap0I : 32; //The last generated TIM header (bytes 0-3), including: Element ID, Length, DTIM Count, DTIM Period, Bitmap Control. Result of SM, reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand0TimHdrBitmap0_u;

/*REG_TIM_GEN_BAND0_TIM_HDR_BITMAP1 0x24 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band0TimHdrBitmap1I : 32; //The last generated TIM header (byte 4) + TIM bitmap (bytes 0-2) concatenated , including: Element ID, Length, DTIM Count, DTIM Period, Bitmap Control. Result of SM, reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand0TimHdrBitmap1_u;

/*REG_TIM_GEN_BAND0_TIM_HDR_BITMAP2 0x28 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band0TimHdrBitmap2I : 32; //The last generated TIM bitmap. Result of SM (bytes 3-6), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand0TimHdrBitmap2_u;

/*REG_TIM_GEN_BAND0_TIM_HDR_BITMAP3 0x2C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band0TimHdrBitmap3I : 32; //The last generated TIM bitmap. Result of SM (bytes 7-10), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand0TimHdrBitmap3_u;

/*REG_TIM_GEN_BAND0_TIM_HDR_BITMAP4 0x30 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band0TimHdrBitmap4I : 32; //The last generated TIM bitmap. Result of SM (bytes 11-14), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand0TimHdrBitmap4_u;

/*REG_TIM_GEN_BAND0_TIM_HDR_BITMAP5 0x34 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band0TimHdrBitmap5I : 32; //The last generated TIM bitmap. Result of SM (bytes 15-18), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand0TimHdrBitmap5_u;

/*REG_TIM_GEN_BAND0_TIM_HDR_BITMAP6 0x38 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band0TimHdrBitmap6I : 32; //The last generated TIM bitmap. Result of SM (bytes 19-22), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand0TimHdrBitmap6_u;

/*REG_TIM_GEN_BAND0_TIM_HDR_BITMAP7 0x3C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band0TimHdrBitmap7I : 32; //The last generated TIM bitmap. Result of SM (bytes 23-26), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand0TimHdrBitmap7_u;

/*REG_TIM_GEN_BAND0_TIM_HDR_BITMAP8 0x40 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band0TimHdrBitmap8I : 32; //The last generated TIM bitmap. Result of SM (bytes 27-30), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand0TimHdrBitmap8_u;

/*REG_TIM_GEN_BAND0_TIM_HDR_BITMAP9 0x44 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band0TimHdrBitmap9I : 32; //The last generated TIM bitmap. Result of SM (bytes 31-34), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand0TimHdrBitmap9_u;

/*REG_TIM_GEN_BAND0_TIM_HDR_BITMAP10 0x48 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band0TimHdrBitmap10I : 32; //The last generated TIM bitmap. Result of SM (bytes 35-38), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand0TimHdrBitmap10_u;

/*REG_TIM_GEN_BAND0_TIM_HDR_BITMAP11 0x4C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band0TimHdrBitmap11I : 32; //The last generated TIM bitmap. Result of SM (bytes 39-42), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand0TimHdrBitmap11_u;

/*REG_TIM_GEN_BAND0_TIM_HDR_BITMAP12 0x50 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band0TimHdrBitmap12I : 32; //The last generated TIM bitmap. Result of SM (bytes 43-46), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand0TimHdrBitmap12_u;

/*REG_TIM_GEN_BAND0_TIM_HDR_BITMAP13 0x54 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band0TimHdrBitmap13I : 32; //The last generated TIM bitmap. Result of SM (bytes 47-50), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand0TimHdrBitmap13_u;

/*REG_TIM_GEN_BAND0_TIM_HDR_BITMAP14 0x58 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band0TimHdrBitmap14I : 32; //The last generated TIM bitmap. Result of SM (bytes 51-54), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand0TimHdrBitmap14_u;

/*REG_TIM_GEN_BAND0_TIM_HDR_BITMAP15 0x5C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band0TimHdrBitmap15I : 32; //The last generated TIM bitmap. Result of SM (byte 55), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand0TimHdrBitmap15_u;

/*REG_TIM_GEN_BAND0_TIM_CASE 0x60 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band0TimCaseI : 3; //Result of SM (Table 8), used by Sender, reset value: 0x0, access type: RO
		uint32 reserved0 : 29;
	} bitFields;
} RegTimGenBand0TimCase_u;

/*REG_TIM_GEN_BAND0_MLTI_HDR_LISTOUT0 0x80 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band0MltiHdrListout0I : 32; //The generated MLTI header (bytes 0-3), including: Element ID, Length, DTIM Count, DTIM Period, Bitmap Control. Result of SM, reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand0MltiHdrListout0_u;

/*REG_TIM_GEN_BAND0_MLTI_HDR_LISTOUT1 0x84 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band0MltiHdrListout1I : 32; //The generated MLTI header (byte 4) + list out(bytes 0-2) concatenated , including: Element ID, Length, DTIM Count, DTIM Period, Bitmap Control. Result of SM, reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand0MltiHdrListout1_u;

/*REG_TIM_GEN_BAND0_MLTI_HDR_LISTOUT2 0x88 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band0MltiHdrListout2I : 32; //The generated MLTI list out. Result of SM (bytes 3-6), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand0MltiHdrListout2_u;

/*REG_TIM_GEN_BAND0_MLTI_HDR_LISTOUT3 0x8C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band0MltiHdrListout3I : 32; //The last generated MLTI list out. Result of SM (bytes 7-10), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand0MltiHdrListout3_u;

/*REG_TIM_GEN_BAND0_MLTI_HDR_LISTOUT4 0x90 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band0MltiHdrListout4I : 32; //The last generated MLTI list out. Result of SM (bytes 11-14), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand0MltiHdrListout4_u;

/*REG_TIM_GEN_BAND0_MLTI_HDR_LISTOUT5 0x94 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band0MltiHdrListout5I : 32; //The last generated MLTI list out. Result of SM (bytes 15-18), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand0MltiHdrListout5_u;

/*REG_TIM_GEN_BAND0_MLTI_HDR_LISTOUT6 0x98 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band0MltiHdrListout6I : 32; //The last generated MLTI list out. Result of SM (bytes 19-22), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand0MltiHdrListout6_u;

/*REG_TIM_GEN_BAND0_MLTI_HDR_LISTOUT7 0x9C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band0MltiHdrListout7I : 32; //The last generated MLTI list out. Result of SM (bytes 23-26), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand0MltiHdrListout7_u;

/*REG_TIM_GEN_BAND0_MLTI_HDR_LISTOUT8 0xA0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band0MltiHdrListout8I : 32; //The last generated MLTI list out. Result of SM (bytes 27-30), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand0MltiHdrListout8_u;

/*REG_TIM_GEN_BAND0_MLTI_HDR_LISTOUT9 0xA4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band0MltiHdrListout9I : 32; //The last generated MLTI list out. Result of SM (bytes 31-34), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand0MltiHdrListout9_u;

/*REG_TIM_GEN_BAND0_MLTI_HDR_LISTOUT10 0xA8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band0MltiHdrListout10I : 32; //The last generated MLTI list out. Result of SM (bytes 35-38), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand0MltiHdrListout10_u;

/*REG_TIM_GEN_BAND0_MLTI_HDR_LISTOUT11 0xAC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band0MltiHdrListout11I : 32; //The last generated MLTI list out. Result of SM (bytes 39-42), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand0MltiHdrListout11_u;

/*REG_TIM_GEN_BAND0_MLTI_HDR_LISTOUT12 0xB0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band0MltiHdrListout12I : 32; //The last generated MLTI list out. Result of SM (bytes 43-46), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand0MltiHdrListout12_u;

/*REG_TIM_GEN_BAND0_MLTI_HDR_LISTOUT13 0xB4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band0MltiHdrListout13I : 32; //The last generated MLTI list out. Result of SM (byte 47), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand0MltiHdrListout13_u;

/*REG_TIM_GEN_BAND0_TI_MLD_COPY0 0xB8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band0TiMldCopy0I : 32; //Reflects the TI_MLD copy of the SM (bits 31-0), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand0TiMldCopy0_u;

/*REG_TIM_GEN_BAND0_TI_MLD_COPY1 0xBC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band0TiMldCopy1I : 32; //Reflects the TI_MLD copy of the SM (bits 63-32), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand0TiMldCopy1_u;

/*REG_TIM_GEN_BAND0_TI_MLD_COPY2 0xC0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band0TiMldCopy2I : 32; //Reflects the TI_MLD copy of the SM (bits 95-64), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand0TiMldCopy2_u;

/*REG_TIM_GEN_BAND0_TI_MLD_COPY3 0xC4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band0TiMldCopy3I : 32; //Reflects the TI_MLD copy of the SM (bits 127-96), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand0TiMldCopy3_u;

/*REG_TIM_GEN_BAND1_TIM_GEN_CTRL 0x100 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band1TimGenEnable : 1; //Enable TIM GEN BAND1, reset value: 0x0, access type: RW
		uint32 band1MltiGenEnable : 1; //Enable MLTI generation BAND1    , reset value: 0x0, access type: RW
		uint32 band1SegBZeroI : 1; //Indicates to set Segment B with value all 0, reset value: 0x0, access type: RW
		uint32 band1SegBDtimI : 1; //Indicates to ignore the DTIM of the reported AP MLDs, reset value: 0x0, access type: RW
		uint32 band1SegBBitmapSizeI : 2; //Indicates the size (in bits) of the reported APs bitmap.Possible values: 1, 3., reset value: 0x3, access type: RW
		uint32 reserved0 : 10;
		uint32 band1MaxNumOfBssidsI : 5; //Used by FW to set the maximum number of members in multiple BSSID set. If M-BSSID is supported then possible values are 2, 4, 8, 16, 32; else the value is 1., reset value: 0x0, access type: RW
		uint32 reserved1 : 11;
	} bitFields;
} RegTimGenBand1TimGenCtrl_u;

/*REG_TIM_GEN_BAND1_TIM_GEN_DBG 0x104 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band1InvalidStaInt : 1; //Error event - Access to non valid MLD DB entry , reset value: 0x0, access type: RO
		uint32 band1InvalidVapInt : 1; //Error event - Access to non valid VAP CFG table entry , reset value: 0x0, access type: RO
		uint32 band1FalseStartInt : 1; //Error event - Atempt to start during busy , reset value: 0x0, access type: RO
		uint32 reserved0 : 1;
		uint32 band1InvalidStaClr : 1; //Error event Clear (Write 1 to clear) - Access to non valid MLD DB entry , reset value: 0x0, access type: WO
		uint32 band1InvalidVapClr : 1; //Error event Clear (Write 1 to clear) - Access to non valid VAP CFG table entry , reset value: 0x0, access type: WO
		uint32 band1FalseStartClr : 1; //Error event Clear (Write 1 to clear) - Atempt to start during busy , reset value: 0x0, access type: WO
		uint32 reserved1 : 9;
		uint32 band1FsmState : 4; //FSM State, reset value: 0x0, access type: RO
		uint32 reserved2 : 12;
	} bitFields;
} RegTimGenBand1TimGenDbg_u;

/*REG_TIM_GEN_BAND1_START_VAP_ID 0x10C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band1StartVapIdI : 6; //Can be used by Sender to trigger the SM. Start includes the VAP_ID (6b) of the transmitted BSSID, reset value: 0x0, access type: RW
		uint32 reserved0 : 26;
	} bitFields;
} RegTimGenBand1StartVapId_u;

/*REG_TIM_GEN_BAND1_SM_BUSY 0x110 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band1SmBusyI : 1; //sm_busy indicates SM is in progress, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegTimGenBand1SmBusy_u;

/*REG_TIM_GEN_BAND1_VAP_ID 0x114 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band1VapIdI : 6; //The VAP_ID provided by the Selector, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegTimGenBand1VapId_u;

/*REG_TIM_GEN_BAND1_B_VAR 0x118 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band1B0VarI : 9; //Intermediate variable B0, 9b Result of SM, reset value: 0x0, access type: RO
		uint32 band1B1VarI : 9; //Intermediate variable B1, 9b Result of SM, reset value: 0x0, access type: RO
		uint32 band1B2VarI : 9; //Intermediate variable B2, 9b Result of SM, reset value: 0x0, access type: RO
		uint32 reserved0 : 5;
	} bitFields;
} RegTimGenBand1BVar_u;

/*REG_TIM_GEN_BAND1_DTIM_PARAM 0x11C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band1DtimCountI : 8; //Reflects the DTIM Count  from selector, reset value: 0x0, access type: RO
		uint32 reserved0 : 8;
		uint32 band1DtimPeriodI : 8; //Reflects the DTIM Period from selector, reset value: 0x0, access type: RO
		uint32 reserved1 : 8;
	} bitFields;
} RegTimGenBand1DtimParam_u;

/*REG_TIM_GEN_BAND1_TIM_HDR_BITMAP0 0x120 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band1TimHdrBitmap0I : 32; //The last generated TIM header (bytes 0-3), including: Element ID, Length, DTIM Count, DTIM Period, Bitmap Control. Result of SM, reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand1TimHdrBitmap0_u;

/*REG_TIM_GEN_BAND1_TIM_HDR_BITMAP1 0x124 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band1TimHdrBitmap1I : 32; //The last generated TIM header (byte 4) + TIM bitmap (bytes 0-2) concatenated , including: Element ID, Length, DTIM Count, DTIM Period, Bitmap Control. Result of SM, reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand1TimHdrBitmap1_u;

/*REG_TIM_GEN_BAND1_TIM_HDR_BITMAP2 0x128 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band1TimHdrBitmap2I : 32; //The last generated TIM bitmap. Result of SM (bytes 3-6), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand1TimHdrBitmap2_u;

/*REG_TIM_GEN_BAND1_TIM_HDR_BITMAP3 0x12C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band1TimHdrBitmap3I : 32; //The last generated TIM bitmap. Result of SM (bytes 7-10), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand1TimHdrBitmap3_u;

/*REG_TIM_GEN_BAND1_TIM_HDR_BITMAP4 0x130 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band1TimHdrBitmap4I : 32; //The last generated TIM bitmap. Result of SM (bytes 11-14), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand1TimHdrBitmap4_u;

/*REG_TIM_GEN_BAND1_TIM_HDR_BITMAP5 0x134 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band1TimHdrBitmap5I : 32; //The last generated TIM bitmap. Result of SM (bytes 15-18), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand1TimHdrBitmap5_u;

/*REG_TIM_GEN_BAND1_TIM_HDR_BITMAP6 0x138 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band1TimHdrBitmap6I : 32; //The last generated TIM bitmap. Result of SM (bytes 19-22), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand1TimHdrBitmap6_u;

/*REG_TIM_GEN_BAND1_TIM_HDR_BITMAP7 0x13C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band1TimHdrBitmap7I : 32; //The last generated TIM bitmap. Result of SM (bytes 23-26), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand1TimHdrBitmap7_u;

/*REG_TIM_GEN_BAND1_TIM_HDR_BITMAP8 0x140 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band1TimHdrBitmap8I : 32; //The last generated TIM bitmap. Result of SM (bytes 27-30), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand1TimHdrBitmap8_u;

/*REG_TIM_GEN_BAND1_TIM_HDR_BITMAP9 0x144 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band1TimHdrBitmap9I : 32; //The last generated TIM bitmap. Result of SM (bytes 31-34), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand1TimHdrBitmap9_u;

/*REG_TIM_GEN_BAND1_TIM_HDR_BITMAP10 0x148 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band1TimHdrBitmap10I : 32; //The last generated TIM bitmap. Result of SM (bytes 35-38), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand1TimHdrBitmap10_u;

/*REG_TIM_GEN_BAND1_TIM_HDR_BITMAP11 0x14C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band1TimHdrBitmap11I : 32; //The last generated TIM bitmap. Result of SM (bytes 39-42), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand1TimHdrBitmap11_u;

/*REG_TIM_GEN_BAND1_TIM_HDR_BITMAP12 0x150 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band1TimHdrBitmap12I : 32; //The last generated TIM bitmap. Result of SM (bytes 43-46), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand1TimHdrBitmap12_u;

/*REG_TIM_GEN_BAND1_TIM_HDR_BITMAP13 0x154 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band1TimHdrBitmap13I : 32; //The last generated TIM bitmap. Result of SM (bytes 47-50), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand1TimHdrBitmap13_u;

/*REG_TIM_GEN_BAND1_TIM_HDR_BITMAP14 0x158 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band1TimHdrBitmap14I : 32; //The last generated TIM bitmap. Result of SM (bytes 51-54), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand1TimHdrBitmap14_u;

/*REG_TIM_GEN_BAND1_TIM_HDR_BITMAP15 0x15C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band1TimHdrBitmap15I : 32; //The last generated TIM bitmap. Result of SM (byte 55), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand1TimHdrBitmap15_u;

/*REG_TIM_GEN_BAND1_TIM_CASE 0x160 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band1TimCaseI : 3; //Result of SM (Table 8), used by Sender, reset value: 0x0, access type: RO
		uint32 reserved0 : 29;
	} bitFields;
} RegTimGenBand1TimCase_u;

/*REG_TIM_GEN_BAND1_MLTI_HDR_LISTOUT0 0x180 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band1MltiHdrListout0I : 32; //The generated MLTI header (bytes 0-3), including: Element ID, Length, DTIM Count, DTIM Period, Bitmap Control. Result of SM, reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand1MltiHdrListout0_u;

/*REG_TIM_GEN_BAND1_MLTI_HDR_LISTOUT1 0x184 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band1MltiHdrListout1I : 32; //The generated MLTI header (byte 4) + list out(bytes 0-2) concatenated , including: Element ID, Length, DTIM Count, DTIM Period, Bitmap Control. Result of SM, reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand1MltiHdrListout1_u;

/*REG_TIM_GEN_BAND1_MLTI_HDR_LISTOUT2 0x188 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band1MltiHdrListout2I : 32; //The generated MLTI list out. Result of SM (bytes 3-6), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand1MltiHdrListout2_u;

/*REG_TIM_GEN_BAND1_MLTI_HDR_LISTOUT3 0x18C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band1MltiHdrListout3I : 32; //The last generated MLTI list out. Result of SM (bytes 7-10), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand1MltiHdrListout3_u;

/*REG_TIM_GEN_BAND1_MLTI_HDR_LISTOUT4 0x190 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band1MltiHdrListout4I : 32; //The last generated MLTI list out. Result of SM (bytes 11-14), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand1MltiHdrListout4_u;

/*REG_TIM_GEN_BAND1_MLTI_HDR_LISTOUT5 0x194 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band1MltiHdrListout5I : 32; //The last generated MLTI list out. Result of SM (bytes 15-18), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand1MltiHdrListout5_u;

/*REG_TIM_GEN_BAND1_MLTI_HDR_LISTOUT6 0x198 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band1MltiHdrListout6I : 32; //The last generated MLTI list out. Result of SM (bytes 19-22), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand1MltiHdrListout6_u;

/*REG_TIM_GEN_BAND1_MLTI_HDR_LISTOUT7 0x19C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band1MltiHdrListout7I : 32; //The last generated MLTI list out. Result of SM (bytes 23-26), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand1MltiHdrListout7_u;

/*REG_TIM_GEN_BAND1_MLTI_HDR_LISTOUT8 0x1A0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band1MltiHdrListout8I : 32; //The last generated MLTI list out. Result of SM (bytes 27-30), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand1MltiHdrListout8_u;

/*REG_TIM_GEN_BAND1_MLTI_HDR_LISTOUT9 0x1A4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band1MltiHdrListout9I : 32; //The last generated MLTI list out. Result of SM (bytes 31-34), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand1MltiHdrListout9_u;

/*REG_TIM_GEN_BAND1_MLTI_HDR_LISTOUT10 0x1A8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band1MltiHdrListout10I : 32; //The last generated MLTI list out. Result of SM (bytes 35-38), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand1MltiHdrListout10_u;

/*REG_TIM_GEN_BAND1_MLTI_HDR_LISTOUT11 0x1AC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band1MltiHdrListout11I : 32; //The last generated MLTI list out. Result of SM (bytes 39-42), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand1MltiHdrListout11_u;

/*REG_TIM_GEN_BAND1_MLTI_HDR_LISTOUT12 0x1B0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band1MltiHdrListout12I : 32; //The last generated MLTI list out. Result of SM (bytes 43-46), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand1MltiHdrListout12_u;

/*REG_TIM_GEN_BAND1_MLTI_HDR_LISTOUT13 0x1B4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band1MltiHdrListout13I : 32; //The last generated MLTI list out. Result of SM (byte 47), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand1MltiHdrListout13_u;

/*REG_TIM_GEN_BAND1_TI_MLD_COPY0 0x1B8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band1TiMldCopy0I : 32; //Reflects the TI_MLD copy of the SM (bits 31-0), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand1TiMldCopy0_u;

/*REG_TIM_GEN_BAND1_TI_MLD_COPY1 0x1BC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band1TiMldCopy1I : 32; //Reflects the TI_MLD copy of the SM (bits 63-32), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand1TiMldCopy1_u;

/*REG_TIM_GEN_BAND1_TI_MLD_COPY2 0x1C0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band1TiMldCopy2I : 32; //Reflects the TI_MLD copy of the SM (bits 95-64), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand1TiMldCopy2_u;

/*REG_TIM_GEN_BAND1_TI_MLD_COPY3 0x1C4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band1TiMldCopy3I : 32; //Reflects the TI_MLD copy of the SM (bits 127-96), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand1TiMldCopy3_u;

/*REG_TIM_GEN_BAND2_TIM_GEN_CTRL 0x200 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band2TimGenEnable : 1; //Enable TIM GEN BAND2, reset value: 0x0, access type: RW
		uint32 band2MltiGenEnable : 1; //Enable MLTI generation BAND2    , reset value: 0x0, access type: RW
		uint32 band2SegBZeroI : 1; //Indicates to set Segment B with value all 0, reset value: 0x0, access type: RW
		uint32 band2SegBDtimI : 1; //Indicates to ignore the DTIM of the reported AP MLDs, reset value: 0x0, access type: RW
		uint32 band2SegBBitmapSizeI : 2; //Indicates the size (in bits) of the reported APs bitmap.Possible values: 1, 3., reset value: 0x3, access type: RW
		uint32 reserved0 : 10;
		uint32 band2MaxNumOfBssidsI : 5; //Used by FW to set the maximum number of members in multiple BSSID set. If M-BSSID is supported then possible values are 2, 4, 8, 16, 32; else the value is 1., reset value: 0x0, access type: RW
		uint32 reserved1 : 11;
	} bitFields;
} RegTimGenBand2TimGenCtrl_u;

/*REG_TIM_GEN_BAND2_TIM_GEN_DBG 0x204 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band2InvalidStaInt : 1; //Error event - Access to non valid MLD DB entry , reset value: 0x0, access type: RO
		uint32 band2InvalidVapInt : 1; //Error event - Access to non valid VAP CFG table entry , reset value: 0x0, access type: RO
		uint32 band2FalseStartInt : 1; //Error event - Atempt to start during busy , reset value: 0x0, access type: RO
		uint32 reserved0 : 1;
		uint32 band2InvalidStaClr : 1; //Error event Clear (Write 1 to clear) - Access to non valid MLD DB entry , reset value: 0x0, access type: WO
		uint32 band2InvalidVapClr : 1; //Error event Clear (Write 1 to clear) - Access to non valid VAP CFG table entry , reset value: 0x0, access type: WO
		uint32 band2FalseStartClr : 1; //Error event Clear (Write 1 to clear) - Atempt to start during busy , reset value: 0x0, access type: WO
		uint32 reserved1 : 9;
		uint32 band2FsmState : 4; //FSM State, reset value: 0x0, access type: RO
		uint32 reserved2 : 12;
	} bitFields;
} RegTimGenBand2TimGenDbg_u;

/*REG_TIM_GEN_BAND2_START_VAP_ID 0x20C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band2StartVapIdI : 6; //Can be used by Sender to trigger the SM. Start includes the VAP_ID (6b) of the transmitted BSSID, reset value: 0x0, access type: RW
		uint32 reserved0 : 26;
	} bitFields;
} RegTimGenBand2StartVapId_u;

/*REG_TIM_GEN_BAND2_SM_BUSY 0x210 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band2SmBusyI : 1; //sm_busy indicates SM is in progress, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegTimGenBand2SmBusy_u;

/*REG_TIM_GEN_BAND2_VAP_ID 0x214 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band2VapIdI : 6; //The VAP_ID provided by the Selector, reset value: 0x0, access type: RO
		uint32 reserved0 : 26;
	} bitFields;
} RegTimGenBand2VapId_u;

/*REG_TIM_GEN_BAND2_B_VAR 0x218 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band2B0VarI : 9; //Intermediate variable B0, 9b Result of SM, reset value: 0x0, access type: RO
		uint32 band2B1VarI : 9; //Intermediate variable B1, 9b Result of SM, reset value: 0x0, access type: RO
		uint32 band2B2VarI : 9; //Intermediate variable B2, 9b Result of SM, reset value: 0x0, access type: RO
		uint32 reserved0 : 5;
	} bitFields;
} RegTimGenBand2BVar_u;

/*REG_TIM_GEN_BAND2_DTIM_PARAM 0x21C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band2DtimCountI : 8; //Reflects the DTIM Count  from selector, reset value: 0x0, access type: RO
		uint32 reserved0 : 8;
		uint32 band2DtimPeriodI : 8; //Reflects the DTIM Period from selector, reset value: 0x0, access type: RO
		uint32 reserved1 : 8;
	} bitFields;
} RegTimGenBand2DtimParam_u;

/*REG_TIM_GEN_BAND2_TIM_HDR_BITMAP0 0x220 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band2TimHdrBitmap0I : 32; //The last generated TIM header (bytes 0-3), including: Element ID, Length, DTIM Count, DTIM Period, Bitmap Control. Result of SM, reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand2TimHdrBitmap0_u;

/*REG_TIM_GEN_BAND2_TIM_HDR_BITMAP1 0x224 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band2TimHdrBitmap1I : 32; //The last generated TIM header (byte 4) + TIM bitmap (bytes 0-2) concatenated , including: Element ID, Length, DTIM Count, DTIM Period, Bitmap Control. Result of SM, reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand2TimHdrBitmap1_u;

/*REG_TIM_GEN_BAND2_TIM_HDR_BITMAP2 0x228 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band2TimHdrBitmap2I : 32; //The last generated TIM bitmap. Result of SM (bytes 3-6), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand2TimHdrBitmap2_u;

/*REG_TIM_GEN_BAND2_TIM_HDR_BITMAP3 0x22C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band2TimHdrBitmap3I : 32; //The last generated TIM bitmap. Result of SM (bytes 7-10), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand2TimHdrBitmap3_u;

/*REG_TIM_GEN_BAND2_TIM_HDR_BITMAP4 0x230 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band2TimHdrBitmap4I : 32; //The last generated TIM bitmap. Result of SM (bytes 11-14), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand2TimHdrBitmap4_u;

/*REG_TIM_GEN_BAND2_TIM_HDR_BITMAP5 0x234 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band2TimHdrBitmap5I : 32; //The last generated TIM bitmap. Result of SM (bytes 15-18), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand2TimHdrBitmap5_u;

/*REG_TIM_GEN_BAND2_TIM_HDR_BITMAP6 0x238 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band2TimHdrBitmap6I : 32; //The last generated TIM bitmap. Result of SM (bytes 19-22), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand2TimHdrBitmap6_u;

/*REG_TIM_GEN_BAND2_TIM_HDR_BITMAP7 0x23C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band2TimHdrBitmap7I : 32; //The last generated TIM bitmap. Result of SM (bytes 23-26), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand2TimHdrBitmap7_u;

/*REG_TIM_GEN_BAND2_TIM_HDR_BITMAP8 0x240 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band2TimHdrBitmap8I : 32; //The last generated TIM bitmap. Result of SM (bytes 27-30), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand2TimHdrBitmap8_u;

/*REG_TIM_GEN_BAND2_TIM_HDR_BITMAP9 0x244 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band2TimHdrBitmap9I : 32; //The last generated TIM bitmap. Result of SM (bytes 31-34), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand2TimHdrBitmap9_u;

/*REG_TIM_GEN_BAND2_TIM_HDR_BITMAP10 0x248 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band2TimHdrBitmap10I : 32; //The last generated TIM bitmap. Result of SM (bytes 35-38), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand2TimHdrBitmap10_u;

/*REG_TIM_GEN_BAND2_TIM_HDR_BITMAP11 0x24C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band2TimHdrBitmap11I : 32; //The last generated TIM bitmap. Result of SM (bytes 39-42), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand2TimHdrBitmap11_u;

/*REG_TIM_GEN_BAND2_TIM_HDR_BITMAP12 0x250 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band2TimHdrBitmap12I : 32; //The last generated TIM bitmap. Result of SM (bytes 43-46), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand2TimHdrBitmap12_u;

/*REG_TIM_GEN_BAND2_TIM_HDR_BITMAP13 0x254 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band2TimHdrBitmap13I : 32; //The last generated TIM bitmap. Result of SM (bytes 47-50), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand2TimHdrBitmap13_u;

/*REG_TIM_GEN_BAND2_TIM_HDR_BITMAP14 0x258 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band2TimHdrBitmap14I : 32; //The last generated TIM bitmap. Result of SM (bytes 51-54), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand2TimHdrBitmap14_u;

/*REG_TIM_GEN_BAND2_TIM_HDR_BITMAP15 0x25C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band2TimHdrBitmap15I : 32; //The last generated TIM bitmap. Result of SM (byte 55), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand2TimHdrBitmap15_u;

/*REG_TIM_GEN_BAND2_TIM_CASE 0x260 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band2TimCaseI : 3; //Result of SM (Table 8), used by Sender, reset value: 0x0, access type: RO
		uint32 reserved0 : 29;
	} bitFields;
} RegTimGenBand2TimCase_u;

/*REG_TIM_GEN_BAND2_MLTI_HDR_LISTOUT0 0x280 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band2MltiHdrListout0I : 32; //The generated MLTI header (bytes 0-3), including: Element ID, Length, DTIM Count, DTIM Period, Bitmap Control. Result of SM, reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand2MltiHdrListout0_u;

/*REG_TIM_GEN_BAND2_MLTI_HDR_LISTOUT1 0x284 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band2MltiHdrListout1I : 32; //The generated MLTI header (byte 4) + list out(bytes 0-2) concatenated , including: Element ID, Length, DTIM Count, DTIM Period, Bitmap Control. Result of SM, reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand2MltiHdrListout1_u;

/*REG_TIM_GEN_BAND2_MLTI_HDR_LISTOUT2 0x288 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band2MltiHdrListout2I : 32; //The generated MLTI list out. Result of SM (bytes 3-6), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand2MltiHdrListout2_u;

/*REG_TIM_GEN_BAND2_MLTI_HDR_LISTOUT3 0x28C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band2MltiHdrListout3I : 32; //The last generated MLTI list out. Result of SM (bytes 7-10), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand2MltiHdrListout3_u;

/*REG_TIM_GEN_BAND2_MLTI_HDR_LISTOUT4 0x290 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band2MltiHdrListout4I : 32; //The last generated MLTI list out. Result of SM (bytes 11-14), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand2MltiHdrListout4_u;

/*REG_TIM_GEN_BAND2_MLTI_HDR_LISTOUT5 0x294 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band2MltiHdrListout5I : 32; //The last generated MLTI list out. Result of SM (bytes 15-18), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand2MltiHdrListout5_u;

/*REG_TIM_GEN_BAND2_MLTI_HDR_LISTOUT6 0x298 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band2MltiHdrListout6I : 32; //The last generated MLTI list out. Result of SM (bytes 19-22), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand2MltiHdrListout6_u;

/*REG_TIM_GEN_BAND2_MLTI_HDR_LISTOUT7 0x29C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band2MltiHdrListout7I : 32; //The last generated MLTI list out. Result of SM (bytes 23-26), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand2MltiHdrListout7_u;

/*REG_TIM_GEN_BAND2_MLTI_HDR_LISTOUT8 0x2A0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band2MltiHdrListout8I : 32; //The last generated MLTI list out. Result of SM (bytes 27-30), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand2MltiHdrListout8_u;

/*REG_TIM_GEN_BAND2_MLTI_HDR_LISTOUT9 0x2A4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band2MltiHdrListout9I : 32; //The last generated MLTI list out. Result of SM (bytes 31-34), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand2MltiHdrListout9_u;

/*REG_TIM_GEN_BAND2_MLTI_HDR_LISTOUT10 0x2A8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band2MltiHdrListout10I : 32; //The last generated MLTI list out. Result of SM (bytes 35-38), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand2MltiHdrListout10_u;

/*REG_TIM_GEN_BAND2_MLTI_HDR_LISTOUT11 0x2AC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band2MltiHdrListout11I : 32; //The last generated MLTI list out. Result of SM (bytes 39-42), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand2MltiHdrListout11_u;

/*REG_TIM_GEN_BAND2_MLTI_HDR_LISTOUT12 0x2B0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band2MltiHdrListout12I : 32; //The last generated MLTI list out. Result of SM (bytes 43-46), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand2MltiHdrListout12_u;

/*REG_TIM_GEN_BAND2_MLTI_HDR_LISTOUT13 0x2B4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band2MltiHdrListout13I : 32; //The last generated MLTI list out. Result of SM (byte 47), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand2MltiHdrListout13_u;

/*REG_TIM_GEN_BAND2_TI_MLD_COPY0 0x2B8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band2TiMldCopy0I : 32; //Reflects the TI_MLD copy of the SM (bits 31-0), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand2TiMldCopy0_u;

/*REG_TIM_GEN_BAND2_TI_MLD_COPY1 0x2BC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band2TiMldCopy1I : 32; //Reflects the TI_MLD copy of the SM (bits 63-32), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand2TiMldCopy1_u;

/*REG_TIM_GEN_BAND2_TI_MLD_COPY2 0x2C0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band2TiMldCopy2I : 32; //Reflects the TI_MLD copy of the SM (bits 95-64), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand2TiMldCopy2_u;

/*REG_TIM_GEN_BAND2_TI_MLD_COPY3 0x2C4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 band2TiMldCopy3I : 32; //Reflects the TI_MLD copy of the SM (bits 127-96), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenBand2TiMldCopy3_u;

/*REG_TIM_GEN_DTIM0 0x300 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dtim0I : 32; //Reflects the DTIM bitmap provided by the Selectors (bits 31-0), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenDtim0_u;

/*REG_TIM_GEN_DTIM1 0x304 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dtim1I : 32; //Reflects the DTIM bitmap provided by the Selectors (bits 63-32), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenDtim1_u;

/*REG_TIM_GEN_VAP_BI0 0x308 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vapBi0I : 32; //Reflects the VAP buffering indication provided by the Selectors (bits 31-0), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenVapBi0_u;

/*REG_TIM_GEN_VAP_BI1 0x30C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vapBi1I : 32; //Reflects the VAP buffering indication provided by the Selectors (bits 63-32), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenVapBi1_u;

/*REG_TIM_GEN_VALID_STA0 0x310 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 validSta0I : 32; //Reflects the Valid STA indication provided by the Selectors (bits 31-0), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenValidSta0_u;

/*REG_TIM_GEN_VALID_STA1 0x314 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 validSta1I : 32; //Reflects the Valid STA indication provided by the Selectors (bits 63-32), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenValidSta1_u;

/*REG_TIM_GEN_VALID_STA2 0x318 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 validSta2I : 32; //Reflects the Valid STA indication provided by the Selectors (bits 95-64), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenValidSta2_u;

/*REG_TIM_GEN_VALID_STA3 0x31C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 validSta3I : 32; //Reflects the Valid STA indication provided by the Selectors (bits 127-96), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenValidSta3_u;

/*REG_TIM_GEN_VALID_STA4 0x320 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 validSta4I : 32; //Reflects the Valid STA indication provided by the Selectors (bits 159-128), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenValidSta4_u;

/*REG_TIM_GEN_VALID_STA5 0x324 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 validSta5I : 32; //Reflects the Valid STA indication provided by the Selectors (bits 191-160), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenValidSta5_u;

/*REG_TIM_GEN_VALID_STA6 0x328 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 validSta6I : 32; //Reflects the Valid STA indication provided by the Selectors (bits 223-192), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenValidSta6_u;

/*REG_TIM_GEN_VALID_STA7 0x32C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 validSta7I : 32; //Reflects the Valid STA indication provided by the Selectors (bits 255-224), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenValidSta7_u;

/*REG_TIM_GEN_VALID_STA8 0x330 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 validSta8I : 32; //Reflects the Valid STA indication provided by the Selectors (bits 287-256), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenValidSta8_u;

/*REG_TIM_GEN_VALID_STA9 0x334 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 validSta9I : 32; //Reflects the Valid STA indication provided by the Selectors (bits 319-288), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenValidSta9_u;

/*REG_TIM_GEN_VALID_STA10 0x338 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 validSta10I : 32; //Reflects the Valid STA indication provided by the Selectors (bits 351-320), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenValidSta10_u;

/*REG_TIM_GEN_VALID_STA11 0x33C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 validSta11I : 32; //Reflects the Valid STA indication provided by the Selectors (bits 383-352), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenValidSta11_u;

/*REG_TIM_GEN_VALID_STA12 0x340 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 validSta12I : 32; //Reflects the Valid STA indication provided by the Selectors (bits 415-384), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenValidSta12_u;

/*REG_TIM_GEN_VALID_STA13 0x344 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 validSta13I : 32; //Reflects the Valid STA indication provided by the Selectors (bits 447-416), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenValidSta13_u;

/*REG_TIM_GEN_VALID_STA14 0x348 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 validSta14I : 32; //Reflects the Valid STA indication provided by the Selectors (bits 479-448), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenValidSta14_u;

/*REG_TIM_GEN_VALID_STA15 0x34C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 validSta15I : 32; //Reflects the Valid STA indication provided by the Selectors (bits 511-480), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenValidSta15_u;

/*REG_TIM_GEN_STA_PS0 0x350 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 staPs0I : 32; //Reflects the STA power state provided by the Selectors (bits 31-0), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenStaPs0_u;

/*REG_TIM_GEN_STA_PS1 0x354 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 staPs1I : 32; //Reflects the STA power state provided by the Selectors (bits 63-32), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenStaPs1_u;

/*REG_TIM_GEN_STA_PS2 0x358 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 staPs2I : 32; //Reflects the STA power state provided by the Selectors (bits 95-64), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenStaPs2_u;

/*REG_TIM_GEN_STA_PS3 0x35C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 staPs3I : 32; //Reflects the STA power state provided by the Selectors (bits 127-96), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenStaPs3_u;

/*REG_TIM_GEN_STA_PS4 0x360 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 staPs4I : 32; //Reflects the STA power state provided by the Selectors (bits 159-128), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenStaPs4_u;

/*REG_TIM_GEN_STA_PS5 0x364 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 staPs5I : 32; //Reflects the STA power state provided by the Selectors (bits 191-160), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenStaPs5_u;

/*REG_TIM_GEN_STA_PS6 0x368 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 staPs6I : 32; //Reflects the STA power state provided by the Selectors (bits 223-192), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenStaPs6_u;

/*REG_TIM_GEN_STA_PS7 0x36C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 staPs7I : 32; //Reflects the STA power state provided by the Selectors (bits 255-224), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenStaPs7_u;

/*REG_TIM_GEN_STA_PS8 0x370 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 staPs8I : 32; //Reflects the STA power state provided by the Selectors (bits 287-256), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenStaPs8_u;

/*REG_TIM_GEN_STA_PS9 0x374 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 staPs9I : 32; //Reflects the STA power state provided by the Selectors (bits 319-288), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenStaPs9_u;

/*REG_TIM_GEN_STA_PS10 0x378 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 staPs10I : 32; //Reflects the STA power state provided by the Selectors (bits 351-320), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenStaPs10_u;

/*REG_TIM_GEN_STA_PS11 0x37C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 staPs11I : 32; //Reflects the STA power state provided by the Selectors (bits 383-352), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenStaPs11_u;

/*REG_TIM_GEN_STA_PS12 0x380 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 staPs12I : 32; //Reflects the STA power state provided by the Selectors (bits 415-384), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenStaPs12_u;

/*REG_TIM_GEN_STA_PS13 0x384 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 staPs13I : 32; //Reflects the STA power state provided by the Selectors (bits 447-416), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenStaPs13_u;

/*REG_TIM_GEN_STA_PS14 0x388 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 staPs14I : 32; //Reflects the STA power state provided by the Selectors (bits 479-448), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenStaPs14_u;

/*REG_TIM_GEN_STA_PS15 0x38C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 staPs15I : 32; //Reflects the STA power state provided by the Selectors (bits 511-480), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenStaPs15_u;

/*REG_TIM_GEN_TI_STA0 0x390 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tiSta0I : 32; //Reflects the TI STA state provided by the ML_DinQ (bits 31-0), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenTiSta0_u;

/*REG_TIM_GEN_TI_STA1 0x394 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tiSta1I : 32; //Reflects the TI STA state provided by the ML_DinQ (bits 63-32), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenTiSta1_u;

/*REG_TIM_GEN_TI_STA2 0x398 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tiSta2I : 32; //Reflects the TI STA state provided by the ML_DinQ (bits 95-64), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenTiSta2_u;

/*REG_TIM_GEN_TI_STA3 0x39C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tiSta3I : 32; //Reflects the TI STA state provided by the ML_DinQ (bits 127-96), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenTiSta3_u;

/*REG_TIM_GEN_TI_STA4 0x3A0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tiSta4I : 32; //Reflects the TI STA state provided by the ML_DinQ (bits 159-128), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenTiSta4_u;

/*REG_TIM_GEN_TI_STA5 0x3A4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tiSta5I : 32; //Reflects the TI STA state provided by the ML_DinQ (bits 191-160), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenTiSta5_u;

/*REG_TIM_GEN_TI_STA6 0x3A8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tiSta6I : 32; //Reflects the TI STA state provided by the ML_DinQ (bits 223-192), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenTiSta6_u;

/*REG_TIM_GEN_TI_STA7 0x3AC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tiSta7I : 32; //Reflects the TI STA state provided by the ML_DinQ (bits 255-224), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenTiSta7_u;

/*REG_TIM_GEN_TI_STA8 0x3B0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tiSta8I : 32; //Reflects the TI STA state provided by the ML_DinQ (bits 287-256), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenTiSta8_u;

/*REG_TIM_GEN_TI_STA9 0x3B4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tiSta9I : 32; //Reflects the TI STA state provided by the ML_DinQ (bits 319-288), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenTiSta9_u;

/*REG_TIM_GEN_TI_STA10 0x3B8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tiSta10I : 32; //Reflects the TI STA state provided by the ML_DinQ (bits 351-320), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenTiSta10_u;

/*REG_TIM_GEN_TI_STA11 0x3BC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tiSta11I : 32; //Reflects the TI STA state provided by the ML_DinQ (bits 383-352), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenTiSta11_u;

/*REG_TIM_GEN_TI_STA12 0x3C0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tiSta12I : 32; //Reflects the TI STA state provided by the ML_DinQ (bits 415-384), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenTiSta12_u;

/*REG_TIM_GEN_TI_STA13 0x3C4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tiSta13I : 32; //Reflects the TI STA state provided by the ML_DinQ (bits 447-416), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenTiSta13_u;

/*REG_TIM_GEN_TI_STA14 0x3C8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tiSta14I : 32; //Reflects the TI STA state provided by the ML_DinQ (bits 479-448), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenTiSta14_u;

/*REG_TIM_GEN_TI_STA15 0x3CC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tiSta15I : 32; //Reflects the TI STA state provided by the ML_DinQ (bits 511-480), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenTiSta15_u;

/*REG_TIM_GEN_MLTI_LISTIN0 0x3D0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mltiListin0I : 32; //Reflects the MLTI LIST IN state provided by the ML_DinQ (bits 31-0), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenMltiListin0_u;

/*REG_TIM_GEN_MLTI_LISTIN1 0x3D4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mltiListin1I : 32; //Reflects the MLTI LIST IN state provided by the ML_DinQ (bits 63-32), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenMltiListin1_u;

/*REG_TIM_GEN_MLTI_LISTIN2 0x3D8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mltiListin2I : 32; //Reflects the MLTI LIST IN state provided by the ML_DinQ (bits 95-64), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenMltiListin2_u;

/*REG_TIM_GEN_MLTI_LISTIN3 0x3DC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mltiListin3I : 32; //Reflects the MLTI LIST IN state provided by the ML_DinQ (bits 127-96), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenMltiListin3_u;

/*REG_TIM_GEN_MLTI_LISTIN4 0x3E0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mltiListin4I : 32; //Reflects the MLTI LIST IN state provided by the ML_DinQ (bits 159-128), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenMltiListin4_u;

/*REG_TIM_GEN_MLTI_LISTIN5 0x3E4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mltiListin5I : 32; //Reflects the MLTI LIST IN state provided by the ML_DinQ (bits 191-160), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenMltiListin5_u;

/*REG_TIM_GEN_MLTI_LISTIN6 0x3E8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mltiListin6I : 32; //Reflects the MLTI LIST IN state provided by the ML_DinQ (bits 223-192), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenMltiListin6_u;

/*REG_TIM_GEN_MLTI_LISTIN7 0x3EC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mltiListin7I : 32; //Reflects the MLTI LIST IN state provided by the ML_DinQ (bits 255-224), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenMltiListin7_u;

/*REG_TIM_GEN_MLTI_LISTIN8 0x3F0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mltiListin8I : 32; //Reflects the MLTI LIST IN state provided by the ML_DinQ (bits 287-256), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenMltiListin8_u;

/*REG_TIM_GEN_MLTI_LISTIN9 0x3F4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mltiListin9I : 32; //Reflects the MLTI LIST IN state provided by the ML_DinQ (bits 319-288), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenMltiListin9_u;

/*REG_TIM_GEN_MLTI_LISTIN10 0x3F8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mltiListin10I : 32; //Reflects the MLTI LIST IN state provided by the ML_DinQ (bits 351-320), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenMltiListin10_u;

/*REG_TIM_GEN_MLTI_LISTIN11 0x3FC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mltiListin11I : 32; //Reflects the MLTI LIST IN state provided by the ML_DinQ (bits 383-352), reset value: 0x0, access type: RO
	} bitFields;
} RegTimGenMltiListin11_u;

//========================================
/* REG_TIM_GEN_BAND0_TIM_GEN_CTRL 0x0 */
#define TIM_GEN_BAND0_TIM_GEN_CTRL_FIELD_BAND0_TIM_GEN_ENABLE_MASK                                                0x00000001
#define TIM_GEN_BAND0_TIM_GEN_CTRL_FIELD_BAND0_MLTI_GEN_ENABLE_MASK                                               0x00000002
#define TIM_GEN_BAND0_TIM_GEN_CTRL_FIELD_BAND0_SEG_B_ZERO_I_MASK                                                  0x00000004
#define TIM_GEN_BAND0_TIM_GEN_CTRL_FIELD_BAND0_SEG_B_DTIM_I_MASK                                                  0x00000008
#define TIM_GEN_BAND0_TIM_GEN_CTRL_FIELD_BAND0_SEG_B_BITMAP_SIZE_I_MASK                                           0x00000030
#define TIM_GEN_BAND0_TIM_GEN_CTRL_FIELD_BAND0_MAX_NUM_OF_BSSIDS_I_MASK                                           0x001F0000
//========================================
/* REG_TIM_GEN_BAND0_TIM_GEN_DBG 0x4 */
#define TIM_GEN_BAND0_TIM_GEN_DBG_FIELD_BAND0_INVALID_STA_INT_MASK                                                0x00000001
#define TIM_GEN_BAND0_TIM_GEN_DBG_FIELD_BAND0_INVALID_VAP_INT_MASK                                                0x00000002
#define TIM_GEN_BAND0_TIM_GEN_DBG_FIELD_BAND0_FALSE_START_INT_MASK                                                0x00000004
#define TIM_GEN_BAND0_TIM_GEN_DBG_FIELD_BAND0_INVALID_STA_CLR_MASK                                                0x00000010
#define TIM_GEN_BAND0_TIM_GEN_DBG_FIELD_BAND0_INVALID_VAP_CLR_MASK                                                0x00000020
#define TIM_GEN_BAND0_TIM_GEN_DBG_FIELD_BAND0_FALSE_START_CLR_MASK                                                0x00000040
#define TIM_GEN_BAND0_TIM_GEN_DBG_FIELD_BAND0_FSM_STATE_MASK                                                      0x000F0000
//========================================
/* REG_TIM_GEN_BAND0_START_VAP_ID 0xC */
#define TIM_GEN_BAND0_START_VAP_ID_FIELD_BAND0_START_VAP_ID_I_MASK                                                0x0000003F
//========================================
/* REG_TIM_GEN_BAND0_SM_BUSY 0x10 */
#define TIM_GEN_BAND0_SM_BUSY_FIELD_BAND0_SM_BUSY_I_MASK                                                          0x00000001
//========================================
/* REG_TIM_GEN_BAND0_VAP_ID 0x14 */
#define TIM_GEN_BAND0_VAP_ID_FIELD_BAND0_VAP_ID_I_MASK                                                            0x0000003F
//========================================
/* REG_TIM_GEN_BAND0_B_VAR 0x18 */
#define TIM_GEN_BAND0_B_VAR_FIELD_BAND0_B0_VAR_I_MASK                                                             0x000001FF
#define TIM_GEN_BAND0_B_VAR_FIELD_BAND0_B1_VAR_I_MASK                                                             0x0003FE00
#define TIM_GEN_BAND0_B_VAR_FIELD_BAND0_B2_VAR_I_MASK                                                             0x07FC0000
//========================================
/* REG_TIM_GEN_BAND0_DTIM_PARAM 0x1C */
#define TIM_GEN_BAND0_DTIM_PARAM_FIELD_BAND0_DTIM_COUNT_I_MASK                                                    0x000000FF
#define TIM_GEN_BAND0_DTIM_PARAM_FIELD_BAND0_DTIM_PERIOD_I_MASK                                                   0x00FF0000
//========================================
/* REG_TIM_GEN_BAND0_TIM_HDR_BITMAP0 0x20 */
#define TIM_GEN_BAND0_TIM_HDR_BITMAP0_FIELD_BAND0_TIM_HDR_BITMAP0_I_MASK                                          0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND0_TIM_HDR_BITMAP1 0x24 */
#define TIM_GEN_BAND0_TIM_HDR_BITMAP1_FIELD_BAND0_TIM_HDR_BITMAP1_I_MASK                                          0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND0_TIM_HDR_BITMAP2 0x28 */
#define TIM_GEN_BAND0_TIM_HDR_BITMAP2_FIELD_BAND0_TIM_HDR_BITMAP2_I_MASK                                          0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND0_TIM_HDR_BITMAP3 0x2C */
#define TIM_GEN_BAND0_TIM_HDR_BITMAP3_FIELD_BAND0_TIM_HDR_BITMAP3_I_MASK                                          0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND0_TIM_HDR_BITMAP4 0x30 */
#define TIM_GEN_BAND0_TIM_HDR_BITMAP4_FIELD_BAND0_TIM_HDR_BITMAP4_I_MASK                                          0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND0_TIM_HDR_BITMAP5 0x34 */
#define TIM_GEN_BAND0_TIM_HDR_BITMAP5_FIELD_BAND0_TIM_HDR_BITMAP5_I_MASK                                          0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND0_TIM_HDR_BITMAP6 0x38 */
#define TIM_GEN_BAND0_TIM_HDR_BITMAP6_FIELD_BAND0_TIM_HDR_BITMAP6_I_MASK                                          0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND0_TIM_HDR_BITMAP7 0x3C */
#define TIM_GEN_BAND0_TIM_HDR_BITMAP7_FIELD_BAND0_TIM_HDR_BITMAP7_I_MASK                                          0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND0_TIM_HDR_BITMAP8 0x40 */
#define TIM_GEN_BAND0_TIM_HDR_BITMAP8_FIELD_BAND0_TIM_HDR_BITMAP8_I_MASK                                          0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND0_TIM_HDR_BITMAP9 0x44 */
#define TIM_GEN_BAND0_TIM_HDR_BITMAP9_FIELD_BAND0_TIM_HDR_BITMAP9_I_MASK                                          0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND0_TIM_HDR_BITMAP10 0x48 */
#define TIM_GEN_BAND0_TIM_HDR_BITMAP10_FIELD_BAND0_TIM_HDR_BITMAP10_I_MASK                                        0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND0_TIM_HDR_BITMAP11 0x4C */
#define TIM_GEN_BAND0_TIM_HDR_BITMAP11_FIELD_BAND0_TIM_HDR_BITMAP11_I_MASK                                        0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND0_TIM_HDR_BITMAP12 0x50 */
#define TIM_GEN_BAND0_TIM_HDR_BITMAP12_FIELD_BAND0_TIM_HDR_BITMAP12_I_MASK                                        0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND0_TIM_HDR_BITMAP13 0x54 */
#define TIM_GEN_BAND0_TIM_HDR_BITMAP13_FIELD_BAND0_TIM_HDR_BITMAP13_I_MASK                                        0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND0_TIM_HDR_BITMAP14 0x58 */
#define TIM_GEN_BAND0_TIM_HDR_BITMAP14_FIELD_BAND0_TIM_HDR_BITMAP14_I_MASK                                        0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND0_TIM_HDR_BITMAP15 0x5C */
#define TIM_GEN_BAND0_TIM_HDR_BITMAP15_FIELD_BAND0_TIM_HDR_BITMAP15_I_MASK                                        0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND0_TIM_CASE 0x60 */
#define TIM_GEN_BAND0_TIM_CASE_FIELD_BAND0_TIM_CASE_I_MASK                                                        0x00000007
//========================================
/* REG_TIM_GEN_BAND0_MLTI_HDR_LISTOUT0 0x80 */
#define TIM_GEN_BAND0_MLTI_HDR_LISTOUT0_FIELD_BAND0_MLTI_HDR_LISTOUT0_I_MASK                                      0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND0_MLTI_HDR_LISTOUT1 0x84 */
#define TIM_GEN_BAND0_MLTI_HDR_LISTOUT1_FIELD_BAND0_MLTI_HDR_LISTOUT1_I_MASK                                      0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND0_MLTI_HDR_LISTOUT2 0x88 */
#define TIM_GEN_BAND0_MLTI_HDR_LISTOUT2_FIELD_BAND0_MLTI_HDR_LISTOUT2_I_MASK                                      0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND0_MLTI_HDR_LISTOUT3 0x8C */
#define TIM_GEN_BAND0_MLTI_HDR_LISTOUT3_FIELD_BAND0_MLTI_HDR_LISTOUT3_I_MASK                                      0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND0_MLTI_HDR_LISTOUT4 0x90 */
#define TIM_GEN_BAND0_MLTI_HDR_LISTOUT4_FIELD_BAND0_MLTI_HDR_LISTOUT4_I_MASK                                      0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND0_MLTI_HDR_LISTOUT5 0x94 */
#define TIM_GEN_BAND0_MLTI_HDR_LISTOUT5_FIELD_BAND0_MLTI_HDR_LISTOUT5_I_MASK                                      0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND0_MLTI_HDR_LISTOUT6 0x98 */
#define TIM_GEN_BAND0_MLTI_HDR_LISTOUT6_FIELD_BAND0_MLTI_HDR_LISTOUT6_I_MASK                                      0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND0_MLTI_HDR_LISTOUT7 0x9C */
#define TIM_GEN_BAND0_MLTI_HDR_LISTOUT7_FIELD_BAND0_MLTI_HDR_LISTOUT7_I_MASK                                      0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND0_MLTI_HDR_LISTOUT8 0xA0 */
#define TIM_GEN_BAND0_MLTI_HDR_LISTOUT8_FIELD_BAND0_MLTI_HDR_LISTOUT8_I_MASK                                      0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND0_MLTI_HDR_LISTOUT9 0xA4 */
#define TIM_GEN_BAND0_MLTI_HDR_LISTOUT9_FIELD_BAND0_MLTI_HDR_LISTOUT9_I_MASK                                      0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND0_MLTI_HDR_LISTOUT10 0xA8 */
#define TIM_GEN_BAND0_MLTI_HDR_LISTOUT10_FIELD_BAND0_MLTI_HDR_LISTOUT10_I_MASK                                    0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND0_MLTI_HDR_LISTOUT11 0xAC */
#define TIM_GEN_BAND0_MLTI_HDR_LISTOUT11_FIELD_BAND0_MLTI_HDR_LISTOUT11_I_MASK                                    0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND0_MLTI_HDR_LISTOUT12 0xB0 */
#define TIM_GEN_BAND0_MLTI_HDR_LISTOUT12_FIELD_BAND0_MLTI_HDR_LISTOUT12_I_MASK                                    0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND0_MLTI_HDR_LISTOUT13 0xB4 */
#define TIM_GEN_BAND0_MLTI_HDR_LISTOUT13_FIELD_BAND0_MLTI_HDR_LISTOUT13_I_MASK                                    0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND0_TI_MLD_COPY0 0xB8 */
#define TIM_GEN_BAND0_TI_MLD_COPY0_FIELD_BAND0_TI_MLD_COPY0_I_MASK                                                0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND0_TI_MLD_COPY1 0xBC */
#define TIM_GEN_BAND0_TI_MLD_COPY1_FIELD_BAND0_TI_MLD_COPY1_I_MASK                                                0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND0_TI_MLD_COPY2 0xC0 */
#define TIM_GEN_BAND0_TI_MLD_COPY2_FIELD_BAND0_TI_MLD_COPY2_I_MASK                                                0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND0_TI_MLD_COPY3 0xC4 */
#define TIM_GEN_BAND0_TI_MLD_COPY3_FIELD_BAND0_TI_MLD_COPY3_I_MASK                                                0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND1_TIM_GEN_CTRL 0x100 */
#define TIM_GEN_BAND1_TIM_GEN_CTRL_FIELD_BAND1_TIM_GEN_ENABLE_MASK                                                0x00000001
#define TIM_GEN_BAND1_TIM_GEN_CTRL_FIELD_BAND1_MLTI_GEN_ENABLE_MASK                                               0x00000002
#define TIM_GEN_BAND1_TIM_GEN_CTRL_FIELD_BAND1_SEG_B_ZERO_I_MASK                                                  0x00000004
#define TIM_GEN_BAND1_TIM_GEN_CTRL_FIELD_BAND1_SEG_B_DTIM_I_MASK                                                  0x00000008
#define TIM_GEN_BAND1_TIM_GEN_CTRL_FIELD_BAND1_SEG_B_BITMAP_SIZE_I_MASK                                           0x00000030
#define TIM_GEN_BAND1_TIM_GEN_CTRL_FIELD_BAND1_MAX_NUM_OF_BSSIDS_I_MASK                                           0x001F0000
//========================================
/* REG_TIM_GEN_BAND1_TIM_GEN_DBG 0x104 */
#define TIM_GEN_BAND1_TIM_GEN_DBG_FIELD_BAND1_INVALID_STA_INT_MASK                                                0x00000001
#define TIM_GEN_BAND1_TIM_GEN_DBG_FIELD_BAND1_INVALID_VAP_INT_MASK                                                0x00000002
#define TIM_GEN_BAND1_TIM_GEN_DBG_FIELD_BAND1_FALSE_START_INT_MASK                                                0x00000004
#define TIM_GEN_BAND1_TIM_GEN_DBG_FIELD_BAND1_INVALID_STA_CLR_MASK                                                0x00000010
#define TIM_GEN_BAND1_TIM_GEN_DBG_FIELD_BAND1_INVALID_VAP_CLR_MASK                                                0x00000020
#define TIM_GEN_BAND1_TIM_GEN_DBG_FIELD_BAND1_FALSE_START_CLR_MASK                                                0x00000040
#define TIM_GEN_BAND1_TIM_GEN_DBG_FIELD_BAND1_FSM_STATE_MASK                                                      0x000F0000
//========================================
/* REG_TIM_GEN_BAND1_START_VAP_ID 0x10C */
#define TIM_GEN_BAND1_START_VAP_ID_FIELD_BAND1_START_VAP_ID_I_MASK                                                0x0000003F
//========================================
/* REG_TIM_GEN_BAND1_SM_BUSY 0x110 */
#define TIM_GEN_BAND1_SM_BUSY_FIELD_BAND1_SM_BUSY_I_MASK                                                          0x00000001
//========================================
/* REG_TIM_GEN_BAND1_VAP_ID 0x114 */
#define TIM_GEN_BAND1_VAP_ID_FIELD_BAND1_VAP_ID_I_MASK                                                            0x0000003F
//========================================
/* REG_TIM_GEN_BAND1_B_VAR 0x118 */
#define TIM_GEN_BAND1_B_VAR_FIELD_BAND1_B0_VAR_I_MASK                                                             0x000001FF
#define TIM_GEN_BAND1_B_VAR_FIELD_BAND1_B1_VAR_I_MASK                                                             0x0003FE00
#define TIM_GEN_BAND1_B_VAR_FIELD_BAND1_B2_VAR_I_MASK                                                             0x07FC0000
//========================================
/* REG_TIM_GEN_BAND1_DTIM_PARAM 0x11C */
#define TIM_GEN_BAND1_DTIM_PARAM_FIELD_BAND1_DTIM_COUNT_I_MASK                                                    0x000000FF
#define TIM_GEN_BAND1_DTIM_PARAM_FIELD_BAND1_DTIM_PERIOD_I_MASK                                                   0x00FF0000
//========================================
/* REG_TIM_GEN_BAND1_TIM_HDR_BITMAP0 0x120 */
#define TIM_GEN_BAND1_TIM_HDR_BITMAP0_FIELD_BAND1_TIM_HDR_BITMAP0_I_MASK                                          0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND1_TIM_HDR_BITMAP1 0x124 */
#define TIM_GEN_BAND1_TIM_HDR_BITMAP1_FIELD_BAND1_TIM_HDR_BITMAP1_I_MASK                                          0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND1_TIM_HDR_BITMAP2 0x128 */
#define TIM_GEN_BAND1_TIM_HDR_BITMAP2_FIELD_BAND1_TIM_HDR_BITMAP2_I_MASK                                          0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND1_TIM_HDR_BITMAP3 0x12C */
#define TIM_GEN_BAND1_TIM_HDR_BITMAP3_FIELD_BAND1_TIM_HDR_BITMAP3_I_MASK                                          0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND1_TIM_HDR_BITMAP4 0x130 */
#define TIM_GEN_BAND1_TIM_HDR_BITMAP4_FIELD_BAND1_TIM_HDR_BITMAP4_I_MASK                                          0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND1_TIM_HDR_BITMAP5 0x134 */
#define TIM_GEN_BAND1_TIM_HDR_BITMAP5_FIELD_BAND1_TIM_HDR_BITMAP5_I_MASK                                          0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND1_TIM_HDR_BITMAP6 0x138 */
#define TIM_GEN_BAND1_TIM_HDR_BITMAP6_FIELD_BAND1_TIM_HDR_BITMAP6_I_MASK                                          0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND1_TIM_HDR_BITMAP7 0x13C */
#define TIM_GEN_BAND1_TIM_HDR_BITMAP7_FIELD_BAND1_TIM_HDR_BITMAP7_I_MASK                                          0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND1_TIM_HDR_BITMAP8 0x140 */
#define TIM_GEN_BAND1_TIM_HDR_BITMAP8_FIELD_BAND1_TIM_HDR_BITMAP8_I_MASK                                          0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND1_TIM_HDR_BITMAP9 0x144 */
#define TIM_GEN_BAND1_TIM_HDR_BITMAP9_FIELD_BAND1_TIM_HDR_BITMAP9_I_MASK                                          0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND1_TIM_HDR_BITMAP10 0x148 */
#define TIM_GEN_BAND1_TIM_HDR_BITMAP10_FIELD_BAND1_TIM_HDR_BITMAP10_I_MASK                                        0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND1_TIM_HDR_BITMAP11 0x14C */
#define TIM_GEN_BAND1_TIM_HDR_BITMAP11_FIELD_BAND1_TIM_HDR_BITMAP11_I_MASK                                        0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND1_TIM_HDR_BITMAP12 0x150 */
#define TIM_GEN_BAND1_TIM_HDR_BITMAP12_FIELD_BAND1_TIM_HDR_BITMAP12_I_MASK                                        0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND1_TIM_HDR_BITMAP13 0x154 */
#define TIM_GEN_BAND1_TIM_HDR_BITMAP13_FIELD_BAND1_TIM_HDR_BITMAP13_I_MASK                                        0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND1_TIM_HDR_BITMAP14 0x158 */
#define TIM_GEN_BAND1_TIM_HDR_BITMAP14_FIELD_BAND1_TIM_HDR_BITMAP14_I_MASK                                        0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND1_TIM_HDR_BITMAP15 0x15C */
#define TIM_GEN_BAND1_TIM_HDR_BITMAP15_FIELD_BAND1_TIM_HDR_BITMAP15_I_MASK                                        0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND1_TIM_CASE 0x160 */
#define TIM_GEN_BAND1_TIM_CASE_FIELD_BAND1_TIM_CASE_I_MASK                                                        0x00000007
//========================================
/* REG_TIM_GEN_BAND1_MLTI_HDR_LISTOUT0 0x180 */
#define TIM_GEN_BAND1_MLTI_HDR_LISTOUT0_FIELD_BAND1_MLTI_HDR_LISTOUT0_I_MASK                                      0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND1_MLTI_HDR_LISTOUT1 0x184 */
#define TIM_GEN_BAND1_MLTI_HDR_LISTOUT1_FIELD_BAND1_MLTI_HDR_LISTOUT1_I_MASK                                      0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND1_MLTI_HDR_LISTOUT2 0x188 */
#define TIM_GEN_BAND1_MLTI_HDR_LISTOUT2_FIELD_BAND1_MLTI_HDR_LISTOUT2_I_MASK                                      0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND1_MLTI_HDR_LISTOUT3 0x18C */
#define TIM_GEN_BAND1_MLTI_HDR_LISTOUT3_FIELD_BAND1_MLTI_HDR_LISTOUT3_I_MASK                                      0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND1_MLTI_HDR_LISTOUT4 0x190 */
#define TIM_GEN_BAND1_MLTI_HDR_LISTOUT4_FIELD_BAND1_MLTI_HDR_LISTOUT4_I_MASK                                      0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND1_MLTI_HDR_LISTOUT5 0x194 */
#define TIM_GEN_BAND1_MLTI_HDR_LISTOUT5_FIELD_BAND1_MLTI_HDR_LISTOUT5_I_MASK                                      0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND1_MLTI_HDR_LISTOUT6 0x198 */
#define TIM_GEN_BAND1_MLTI_HDR_LISTOUT6_FIELD_BAND1_MLTI_HDR_LISTOUT6_I_MASK                                      0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND1_MLTI_HDR_LISTOUT7 0x19C */
#define TIM_GEN_BAND1_MLTI_HDR_LISTOUT7_FIELD_BAND1_MLTI_HDR_LISTOUT7_I_MASK                                      0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND1_MLTI_HDR_LISTOUT8 0x1A0 */
#define TIM_GEN_BAND1_MLTI_HDR_LISTOUT8_FIELD_BAND1_MLTI_HDR_LISTOUT8_I_MASK                                      0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND1_MLTI_HDR_LISTOUT9 0x1A4 */
#define TIM_GEN_BAND1_MLTI_HDR_LISTOUT9_FIELD_BAND1_MLTI_HDR_LISTOUT9_I_MASK                                      0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND1_MLTI_HDR_LISTOUT10 0x1A8 */
#define TIM_GEN_BAND1_MLTI_HDR_LISTOUT10_FIELD_BAND1_MLTI_HDR_LISTOUT10_I_MASK                                    0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND1_MLTI_HDR_LISTOUT11 0x1AC */
#define TIM_GEN_BAND1_MLTI_HDR_LISTOUT11_FIELD_BAND1_MLTI_HDR_LISTOUT11_I_MASK                                    0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND1_MLTI_HDR_LISTOUT12 0x1B0 */
#define TIM_GEN_BAND1_MLTI_HDR_LISTOUT12_FIELD_BAND1_MLTI_HDR_LISTOUT12_I_MASK                                    0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND1_MLTI_HDR_LISTOUT13 0x1B4 */
#define TIM_GEN_BAND1_MLTI_HDR_LISTOUT13_FIELD_BAND1_MLTI_HDR_LISTOUT13_I_MASK                                    0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND1_TI_MLD_COPY0 0x1B8 */
#define TIM_GEN_BAND1_TI_MLD_COPY0_FIELD_BAND1_TI_MLD_COPY0_I_MASK                                                0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND1_TI_MLD_COPY1 0x1BC */
#define TIM_GEN_BAND1_TI_MLD_COPY1_FIELD_BAND1_TI_MLD_COPY1_I_MASK                                                0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND1_TI_MLD_COPY2 0x1C0 */
#define TIM_GEN_BAND1_TI_MLD_COPY2_FIELD_BAND1_TI_MLD_COPY2_I_MASK                                                0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND1_TI_MLD_COPY3 0x1C4 */
#define TIM_GEN_BAND1_TI_MLD_COPY3_FIELD_BAND1_TI_MLD_COPY3_I_MASK                                                0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND2_TIM_GEN_CTRL 0x200 */
#define TIM_GEN_BAND2_TIM_GEN_CTRL_FIELD_BAND2_TIM_GEN_ENABLE_MASK                                                0x00000001
#define TIM_GEN_BAND2_TIM_GEN_CTRL_FIELD_BAND2_MLTI_GEN_ENABLE_MASK                                               0x00000002
#define TIM_GEN_BAND2_TIM_GEN_CTRL_FIELD_BAND2_SEG_B_ZERO_I_MASK                                                  0x00000004
#define TIM_GEN_BAND2_TIM_GEN_CTRL_FIELD_BAND2_SEG_B_DTIM_I_MASK                                                  0x00000008
#define TIM_GEN_BAND2_TIM_GEN_CTRL_FIELD_BAND2_SEG_B_BITMAP_SIZE_I_MASK                                           0x00000030
#define TIM_GEN_BAND2_TIM_GEN_CTRL_FIELD_BAND2_MAX_NUM_OF_BSSIDS_I_MASK                                           0x001F0000
//========================================
/* REG_TIM_GEN_BAND2_TIM_GEN_DBG 0x204 */
#define TIM_GEN_BAND2_TIM_GEN_DBG_FIELD_BAND2_INVALID_STA_INT_MASK                                                0x00000001
#define TIM_GEN_BAND2_TIM_GEN_DBG_FIELD_BAND2_INVALID_VAP_INT_MASK                                                0x00000002
#define TIM_GEN_BAND2_TIM_GEN_DBG_FIELD_BAND2_FALSE_START_INT_MASK                                                0x00000004
#define TIM_GEN_BAND2_TIM_GEN_DBG_FIELD_BAND2_INVALID_STA_CLR_MASK                                                0x00000010
#define TIM_GEN_BAND2_TIM_GEN_DBG_FIELD_BAND2_INVALID_VAP_CLR_MASK                                                0x00000020
#define TIM_GEN_BAND2_TIM_GEN_DBG_FIELD_BAND2_FALSE_START_CLR_MASK                                                0x00000040
#define TIM_GEN_BAND2_TIM_GEN_DBG_FIELD_BAND2_FSM_STATE_MASK                                                      0x000F0000
//========================================
/* REG_TIM_GEN_BAND2_START_VAP_ID 0x20C */
#define TIM_GEN_BAND2_START_VAP_ID_FIELD_BAND2_START_VAP_ID_I_MASK                                                0x0000003F
//========================================
/* REG_TIM_GEN_BAND2_SM_BUSY 0x210 */
#define TIM_GEN_BAND2_SM_BUSY_FIELD_BAND2_SM_BUSY_I_MASK                                                          0x00000001
//========================================
/* REG_TIM_GEN_BAND2_VAP_ID 0x214 */
#define TIM_GEN_BAND2_VAP_ID_FIELD_BAND2_VAP_ID_I_MASK                                                            0x0000003F
//========================================
/* REG_TIM_GEN_BAND2_B_VAR 0x218 */
#define TIM_GEN_BAND2_B_VAR_FIELD_BAND2_B0_VAR_I_MASK                                                             0x000001FF
#define TIM_GEN_BAND2_B_VAR_FIELD_BAND2_B1_VAR_I_MASK                                                             0x0003FE00
#define TIM_GEN_BAND2_B_VAR_FIELD_BAND2_B2_VAR_I_MASK                                                             0x07FC0000
//========================================
/* REG_TIM_GEN_BAND2_DTIM_PARAM 0x21C */
#define TIM_GEN_BAND2_DTIM_PARAM_FIELD_BAND2_DTIM_COUNT_I_MASK                                                    0x000000FF
#define TIM_GEN_BAND2_DTIM_PARAM_FIELD_BAND2_DTIM_PERIOD_I_MASK                                                   0x00FF0000
//========================================
/* REG_TIM_GEN_BAND2_TIM_HDR_BITMAP0 0x220 */
#define TIM_GEN_BAND2_TIM_HDR_BITMAP0_FIELD_BAND2_TIM_HDR_BITMAP0_I_MASK                                          0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND2_TIM_HDR_BITMAP1 0x224 */
#define TIM_GEN_BAND2_TIM_HDR_BITMAP1_FIELD_BAND2_TIM_HDR_BITMAP1_I_MASK                                          0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND2_TIM_HDR_BITMAP2 0x228 */
#define TIM_GEN_BAND2_TIM_HDR_BITMAP2_FIELD_BAND2_TIM_HDR_BITMAP2_I_MASK                                          0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND2_TIM_HDR_BITMAP3 0x22C */
#define TIM_GEN_BAND2_TIM_HDR_BITMAP3_FIELD_BAND2_TIM_HDR_BITMAP3_I_MASK                                          0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND2_TIM_HDR_BITMAP4 0x230 */
#define TIM_GEN_BAND2_TIM_HDR_BITMAP4_FIELD_BAND2_TIM_HDR_BITMAP4_I_MASK                                          0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND2_TIM_HDR_BITMAP5 0x234 */
#define TIM_GEN_BAND2_TIM_HDR_BITMAP5_FIELD_BAND2_TIM_HDR_BITMAP5_I_MASK                                          0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND2_TIM_HDR_BITMAP6 0x238 */
#define TIM_GEN_BAND2_TIM_HDR_BITMAP6_FIELD_BAND2_TIM_HDR_BITMAP6_I_MASK                                          0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND2_TIM_HDR_BITMAP7 0x23C */
#define TIM_GEN_BAND2_TIM_HDR_BITMAP7_FIELD_BAND2_TIM_HDR_BITMAP7_I_MASK                                          0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND2_TIM_HDR_BITMAP8 0x240 */
#define TIM_GEN_BAND2_TIM_HDR_BITMAP8_FIELD_BAND2_TIM_HDR_BITMAP8_I_MASK                                          0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND2_TIM_HDR_BITMAP9 0x244 */
#define TIM_GEN_BAND2_TIM_HDR_BITMAP9_FIELD_BAND2_TIM_HDR_BITMAP9_I_MASK                                          0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND2_TIM_HDR_BITMAP10 0x248 */
#define TIM_GEN_BAND2_TIM_HDR_BITMAP10_FIELD_BAND2_TIM_HDR_BITMAP10_I_MASK                                        0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND2_TIM_HDR_BITMAP11 0x24C */
#define TIM_GEN_BAND2_TIM_HDR_BITMAP11_FIELD_BAND2_TIM_HDR_BITMAP11_I_MASK                                        0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND2_TIM_HDR_BITMAP12 0x250 */
#define TIM_GEN_BAND2_TIM_HDR_BITMAP12_FIELD_BAND2_TIM_HDR_BITMAP12_I_MASK                                        0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND2_TIM_HDR_BITMAP13 0x254 */
#define TIM_GEN_BAND2_TIM_HDR_BITMAP13_FIELD_BAND2_TIM_HDR_BITMAP13_I_MASK                                        0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND2_TIM_HDR_BITMAP14 0x258 */
#define TIM_GEN_BAND2_TIM_HDR_BITMAP14_FIELD_BAND2_TIM_HDR_BITMAP14_I_MASK                                        0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND2_TIM_HDR_BITMAP15 0x25C */
#define TIM_GEN_BAND2_TIM_HDR_BITMAP15_FIELD_BAND2_TIM_HDR_BITMAP15_I_MASK                                        0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND2_TIM_CASE 0x260 */
#define TIM_GEN_BAND2_TIM_CASE_FIELD_BAND2_TIM_CASE_I_MASK                                                        0x00000007
//========================================
/* REG_TIM_GEN_BAND2_MLTI_HDR_LISTOUT0 0x280 */
#define TIM_GEN_BAND2_MLTI_HDR_LISTOUT0_FIELD_BAND2_MLTI_HDR_LISTOUT0_I_MASK                                      0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND2_MLTI_HDR_LISTOUT1 0x284 */
#define TIM_GEN_BAND2_MLTI_HDR_LISTOUT1_FIELD_BAND2_MLTI_HDR_LISTOUT1_I_MASK                                      0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND2_MLTI_HDR_LISTOUT2 0x288 */
#define TIM_GEN_BAND2_MLTI_HDR_LISTOUT2_FIELD_BAND2_MLTI_HDR_LISTOUT2_I_MASK                                      0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND2_MLTI_HDR_LISTOUT3 0x28C */
#define TIM_GEN_BAND2_MLTI_HDR_LISTOUT3_FIELD_BAND2_MLTI_HDR_LISTOUT3_I_MASK                                      0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND2_MLTI_HDR_LISTOUT4 0x290 */
#define TIM_GEN_BAND2_MLTI_HDR_LISTOUT4_FIELD_BAND2_MLTI_HDR_LISTOUT4_I_MASK                                      0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND2_MLTI_HDR_LISTOUT5 0x294 */
#define TIM_GEN_BAND2_MLTI_HDR_LISTOUT5_FIELD_BAND2_MLTI_HDR_LISTOUT5_I_MASK                                      0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND2_MLTI_HDR_LISTOUT6 0x298 */
#define TIM_GEN_BAND2_MLTI_HDR_LISTOUT6_FIELD_BAND2_MLTI_HDR_LISTOUT6_I_MASK                                      0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND2_MLTI_HDR_LISTOUT7 0x29C */
#define TIM_GEN_BAND2_MLTI_HDR_LISTOUT7_FIELD_BAND2_MLTI_HDR_LISTOUT7_I_MASK                                      0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND2_MLTI_HDR_LISTOUT8 0x2A0 */
#define TIM_GEN_BAND2_MLTI_HDR_LISTOUT8_FIELD_BAND2_MLTI_HDR_LISTOUT8_I_MASK                                      0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND2_MLTI_HDR_LISTOUT9 0x2A4 */
#define TIM_GEN_BAND2_MLTI_HDR_LISTOUT9_FIELD_BAND2_MLTI_HDR_LISTOUT9_I_MASK                                      0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND2_MLTI_HDR_LISTOUT10 0x2A8 */
#define TIM_GEN_BAND2_MLTI_HDR_LISTOUT10_FIELD_BAND2_MLTI_HDR_LISTOUT10_I_MASK                                    0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND2_MLTI_HDR_LISTOUT11 0x2AC */
#define TIM_GEN_BAND2_MLTI_HDR_LISTOUT11_FIELD_BAND2_MLTI_HDR_LISTOUT11_I_MASK                                    0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND2_MLTI_HDR_LISTOUT12 0x2B0 */
#define TIM_GEN_BAND2_MLTI_HDR_LISTOUT12_FIELD_BAND2_MLTI_HDR_LISTOUT12_I_MASK                                    0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND2_MLTI_HDR_LISTOUT13 0x2B4 */
#define TIM_GEN_BAND2_MLTI_HDR_LISTOUT13_FIELD_BAND2_MLTI_HDR_LISTOUT13_I_MASK                                    0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND2_TI_MLD_COPY0 0x2B8 */
#define TIM_GEN_BAND2_TI_MLD_COPY0_FIELD_BAND2_TI_MLD_COPY0_I_MASK                                                0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND2_TI_MLD_COPY1 0x2BC */
#define TIM_GEN_BAND2_TI_MLD_COPY1_FIELD_BAND2_TI_MLD_COPY1_I_MASK                                                0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND2_TI_MLD_COPY2 0x2C0 */
#define TIM_GEN_BAND2_TI_MLD_COPY2_FIELD_BAND2_TI_MLD_COPY2_I_MASK                                                0xFFFFFFFF
//========================================
/* REG_TIM_GEN_BAND2_TI_MLD_COPY3 0x2C4 */
#define TIM_GEN_BAND2_TI_MLD_COPY3_FIELD_BAND2_TI_MLD_COPY3_I_MASK                                                0xFFFFFFFF
//========================================
/* REG_TIM_GEN_DTIM0 0x300 */
#define TIM_GEN_DTIM0_FIELD_DTIM0_I_MASK                                                                          0xFFFFFFFF
//========================================
/* REG_TIM_GEN_DTIM1 0x304 */
#define TIM_GEN_DTIM1_FIELD_DTIM1_I_MASK                                                                          0xFFFFFFFF
//========================================
/* REG_TIM_GEN_VAP_BI0 0x308 */
#define TIM_GEN_VAP_BI0_FIELD_VAP_BI0_I_MASK                                                                      0xFFFFFFFF
//========================================
/* REG_TIM_GEN_VAP_BI1 0x30C */
#define TIM_GEN_VAP_BI1_FIELD_VAP_BI1_I_MASK                                                                      0xFFFFFFFF
//========================================
/* REG_TIM_GEN_VALID_STA0 0x310 */
#define TIM_GEN_VALID_STA0_FIELD_VALID_STA0_I_MASK                                                                0xFFFFFFFF
//========================================
/* REG_TIM_GEN_VALID_STA1 0x314 */
#define TIM_GEN_VALID_STA1_FIELD_VALID_STA1_I_MASK                                                                0xFFFFFFFF
//========================================
/* REG_TIM_GEN_VALID_STA2 0x318 */
#define TIM_GEN_VALID_STA2_FIELD_VALID_STA2_I_MASK                                                                0xFFFFFFFF
//========================================
/* REG_TIM_GEN_VALID_STA3 0x31C */
#define TIM_GEN_VALID_STA3_FIELD_VALID_STA3_I_MASK                                                                0xFFFFFFFF
//========================================
/* REG_TIM_GEN_VALID_STA4 0x320 */
#define TIM_GEN_VALID_STA4_FIELD_VALID_STA4_I_MASK                                                                0xFFFFFFFF
//========================================
/* REG_TIM_GEN_VALID_STA5 0x324 */
#define TIM_GEN_VALID_STA5_FIELD_VALID_STA5_I_MASK                                                                0xFFFFFFFF
//========================================
/* REG_TIM_GEN_VALID_STA6 0x328 */
#define TIM_GEN_VALID_STA6_FIELD_VALID_STA6_I_MASK                                                                0xFFFFFFFF
//========================================
/* REG_TIM_GEN_VALID_STA7 0x32C */
#define TIM_GEN_VALID_STA7_FIELD_VALID_STA7_I_MASK                                                                0xFFFFFFFF
//========================================
/* REG_TIM_GEN_VALID_STA8 0x330 */
#define TIM_GEN_VALID_STA8_FIELD_VALID_STA8_I_MASK                                                                0xFFFFFFFF
//========================================
/* REG_TIM_GEN_VALID_STA9 0x334 */
#define TIM_GEN_VALID_STA9_FIELD_VALID_STA9_I_MASK                                                                0xFFFFFFFF
//========================================
/* REG_TIM_GEN_VALID_STA10 0x338 */
#define TIM_GEN_VALID_STA10_FIELD_VALID_STA10_I_MASK                                                              0xFFFFFFFF
//========================================
/* REG_TIM_GEN_VALID_STA11 0x33C */
#define TIM_GEN_VALID_STA11_FIELD_VALID_STA11_I_MASK                                                              0xFFFFFFFF
//========================================
/* REG_TIM_GEN_VALID_STA12 0x340 */
#define TIM_GEN_VALID_STA12_FIELD_VALID_STA12_I_MASK                                                              0xFFFFFFFF
//========================================
/* REG_TIM_GEN_VALID_STA13 0x344 */
#define TIM_GEN_VALID_STA13_FIELD_VALID_STA13_I_MASK                                                              0xFFFFFFFF
//========================================
/* REG_TIM_GEN_VALID_STA14 0x348 */
#define TIM_GEN_VALID_STA14_FIELD_VALID_STA14_I_MASK                                                              0xFFFFFFFF
//========================================
/* REG_TIM_GEN_VALID_STA15 0x34C */
#define TIM_GEN_VALID_STA15_FIELD_VALID_STA15_I_MASK                                                              0xFFFFFFFF
//========================================
/* REG_TIM_GEN_STA_PS0 0x350 */
#define TIM_GEN_STA_PS0_FIELD_STA_PS0_I_MASK                                                                      0xFFFFFFFF
//========================================
/* REG_TIM_GEN_STA_PS1 0x354 */
#define TIM_GEN_STA_PS1_FIELD_STA_PS1_I_MASK                                                                      0xFFFFFFFF
//========================================
/* REG_TIM_GEN_STA_PS2 0x358 */
#define TIM_GEN_STA_PS2_FIELD_STA_PS2_I_MASK                                                                      0xFFFFFFFF
//========================================
/* REG_TIM_GEN_STA_PS3 0x35C */
#define TIM_GEN_STA_PS3_FIELD_STA_PS3_I_MASK                                                                      0xFFFFFFFF
//========================================
/* REG_TIM_GEN_STA_PS4 0x360 */
#define TIM_GEN_STA_PS4_FIELD_STA_PS4_I_MASK                                                                      0xFFFFFFFF
//========================================
/* REG_TIM_GEN_STA_PS5 0x364 */
#define TIM_GEN_STA_PS5_FIELD_STA_PS5_I_MASK                                                                      0xFFFFFFFF
//========================================
/* REG_TIM_GEN_STA_PS6 0x368 */
#define TIM_GEN_STA_PS6_FIELD_STA_PS6_I_MASK                                                                      0xFFFFFFFF
//========================================
/* REG_TIM_GEN_STA_PS7 0x36C */
#define TIM_GEN_STA_PS7_FIELD_STA_PS7_I_MASK                                                                      0xFFFFFFFF
//========================================
/* REG_TIM_GEN_STA_PS8 0x370 */
#define TIM_GEN_STA_PS8_FIELD_STA_PS8_I_MASK                                                                      0xFFFFFFFF
//========================================
/* REG_TIM_GEN_STA_PS9 0x374 */
#define TIM_GEN_STA_PS9_FIELD_STA_PS9_I_MASK                                                                      0xFFFFFFFF
//========================================
/* REG_TIM_GEN_STA_PS10 0x378 */
#define TIM_GEN_STA_PS10_FIELD_STA_PS10_I_MASK                                                                    0xFFFFFFFF
//========================================
/* REG_TIM_GEN_STA_PS11 0x37C */
#define TIM_GEN_STA_PS11_FIELD_STA_PS11_I_MASK                                                                    0xFFFFFFFF
//========================================
/* REG_TIM_GEN_STA_PS12 0x380 */
#define TIM_GEN_STA_PS12_FIELD_STA_PS12_I_MASK                                                                    0xFFFFFFFF
//========================================
/* REG_TIM_GEN_STA_PS13 0x384 */
#define TIM_GEN_STA_PS13_FIELD_STA_PS13_I_MASK                                                                    0xFFFFFFFF
//========================================
/* REG_TIM_GEN_STA_PS14 0x388 */
#define TIM_GEN_STA_PS14_FIELD_STA_PS14_I_MASK                                                                    0xFFFFFFFF
//========================================
/* REG_TIM_GEN_STA_PS15 0x38C */
#define TIM_GEN_STA_PS15_FIELD_STA_PS15_I_MASK                                                                    0xFFFFFFFF
//========================================
/* REG_TIM_GEN_TI_STA0 0x390 */
#define TIM_GEN_TI_STA0_FIELD_TI_STA0_I_MASK                                                                      0xFFFFFFFF
//========================================
/* REG_TIM_GEN_TI_STA1 0x394 */
#define TIM_GEN_TI_STA1_FIELD_TI_STA1_I_MASK                                                                      0xFFFFFFFF
//========================================
/* REG_TIM_GEN_TI_STA2 0x398 */
#define TIM_GEN_TI_STA2_FIELD_TI_STA2_I_MASK                                                                      0xFFFFFFFF
//========================================
/* REG_TIM_GEN_TI_STA3 0x39C */
#define TIM_GEN_TI_STA3_FIELD_TI_STA3_I_MASK                                                                      0xFFFFFFFF
//========================================
/* REG_TIM_GEN_TI_STA4 0x3A0 */
#define TIM_GEN_TI_STA4_FIELD_TI_STA4_I_MASK                                                                      0xFFFFFFFF
//========================================
/* REG_TIM_GEN_TI_STA5 0x3A4 */
#define TIM_GEN_TI_STA5_FIELD_TI_STA5_I_MASK                                                                      0xFFFFFFFF
//========================================
/* REG_TIM_GEN_TI_STA6 0x3A8 */
#define TIM_GEN_TI_STA6_FIELD_TI_STA6_I_MASK                                                                      0xFFFFFFFF
//========================================
/* REG_TIM_GEN_TI_STA7 0x3AC */
#define TIM_GEN_TI_STA7_FIELD_TI_STA7_I_MASK                                                                      0xFFFFFFFF
//========================================
/* REG_TIM_GEN_TI_STA8 0x3B0 */
#define TIM_GEN_TI_STA8_FIELD_TI_STA8_I_MASK                                                                      0xFFFFFFFF
//========================================
/* REG_TIM_GEN_TI_STA9 0x3B4 */
#define TIM_GEN_TI_STA9_FIELD_TI_STA9_I_MASK                                                                      0xFFFFFFFF
//========================================
/* REG_TIM_GEN_TI_STA10 0x3B8 */
#define TIM_GEN_TI_STA10_FIELD_TI_STA10_I_MASK                                                                    0xFFFFFFFF
//========================================
/* REG_TIM_GEN_TI_STA11 0x3BC */
#define TIM_GEN_TI_STA11_FIELD_TI_STA11_I_MASK                                                                    0xFFFFFFFF
//========================================
/* REG_TIM_GEN_TI_STA12 0x3C0 */
#define TIM_GEN_TI_STA12_FIELD_TI_STA12_I_MASK                                                                    0xFFFFFFFF
//========================================
/* REG_TIM_GEN_TI_STA13 0x3C4 */
#define TIM_GEN_TI_STA13_FIELD_TI_STA13_I_MASK                                                                    0xFFFFFFFF
//========================================
/* REG_TIM_GEN_TI_STA14 0x3C8 */
#define TIM_GEN_TI_STA14_FIELD_TI_STA14_I_MASK                                                                    0xFFFFFFFF
//========================================
/* REG_TIM_GEN_TI_STA15 0x3CC */
#define TIM_GEN_TI_STA15_FIELD_TI_STA15_I_MASK                                                                    0xFFFFFFFF
//========================================
/* REG_TIM_GEN_MLTI_LISTIN0 0x3D0 */
#define TIM_GEN_MLTI_LISTIN0_FIELD_MLTI_LISTIN0_I_MASK                                                            0xFFFFFFFF
//========================================
/* REG_TIM_GEN_MLTI_LISTIN1 0x3D4 */
#define TIM_GEN_MLTI_LISTIN1_FIELD_MLTI_LISTIN1_I_MASK                                                            0xFFFFFFFF
//========================================
/* REG_TIM_GEN_MLTI_LISTIN2 0x3D8 */
#define TIM_GEN_MLTI_LISTIN2_FIELD_MLTI_LISTIN2_I_MASK                                                            0xFFFFFFFF
//========================================
/* REG_TIM_GEN_MLTI_LISTIN3 0x3DC */
#define TIM_GEN_MLTI_LISTIN3_FIELD_MLTI_LISTIN3_I_MASK                                                            0xFFFFFFFF
//========================================
/* REG_TIM_GEN_MLTI_LISTIN4 0x3E0 */
#define TIM_GEN_MLTI_LISTIN4_FIELD_MLTI_LISTIN4_I_MASK                                                            0xFFFFFFFF
//========================================
/* REG_TIM_GEN_MLTI_LISTIN5 0x3E4 */
#define TIM_GEN_MLTI_LISTIN5_FIELD_MLTI_LISTIN5_I_MASK                                                            0xFFFFFFFF
//========================================
/* REG_TIM_GEN_MLTI_LISTIN6 0x3E8 */
#define TIM_GEN_MLTI_LISTIN6_FIELD_MLTI_LISTIN6_I_MASK                                                            0xFFFFFFFF
//========================================
/* REG_TIM_GEN_MLTI_LISTIN7 0x3EC */
#define TIM_GEN_MLTI_LISTIN7_FIELD_MLTI_LISTIN7_I_MASK                                                            0xFFFFFFFF
//========================================
/* REG_TIM_GEN_MLTI_LISTIN8 0x3F0 */
#define TIM_GEN_MLTI_LISTIN8_FIELD_MLTI_LISTIN8_I_MASK                                                            0xFFFFFFFF
//========================================
/* REG_TIM_GEN_MLTI_LISTIN9 0x3F4 */
#define TIM_GEN_MLTI_LISTIN9_FIELD_MLTI_LISTIN9_I_MASK                                                            0xFFFFFFFF
//========================================
/* REG_TIM_GEN_MLTI_LISTIN10 0x3F8 */
#define TIM_GEN_MLTI_LISTIN10_FIELD_MLTI_LISTIN10_I_MASK                                                          0xFFFFFFFF
//========================================
/* REG_TIM_GEN_MLTI_LISTIN11 0x3FC */
#define TIM_GEN_MLTI_LISTIN11_FIELD_MLTI_LISTIN11_I_MASK                                                          0xFFFFFFFF


#endif // _TIM_GEN_REGS_H_
