
/***********************************************************************************
File:				BeaconRegs.h
Module:				beacon
SOC Revision:		latest
Generated at:       2024-06-26 12:55:12
Purpose:
Description:		This File was auto generated using SOC Online

************************************************************************************/
#ifndef _BEACON_REGS_H_
#define _BEACON_REGS_H_

/*---------------------------------------------------------------------------------
/						Registers Addresses													 
/----------------------------------------------------------------------------------*/
#include "HwMemoryMap.h"

#define BEACON_BASE_ADDRESS                        MEMORY_MAP_UNIT_15_BASE_ADDRESS
#define	REG_BEACON_BEACON_LCM                   (BEACON_BASE_ADDRESS + 0x0)
#define	REG_BEACON_VAP0_BEACON_ACTIVATE         (BEACON_BASE_ADDRESS + 0x4)
#define	REG_BEACON_VAP0_BEACON_OFFSET           (BEACON_BASE_ADDRESS + 0x8)
#define	REG_BEACON_VAP0_BEACON_CSA              (BEACON_BASE_ADDRESS + 0xC)
#define	REG_BEACON_VAP1_BEACON_ACTIVATE         (BEACON_BASE_ADDRESS + 0x10)
#define	REG_BEACON_VAP1_BEACON_OFFSET           (BEACON_BASE_ADDRESS + 0x14)
#define	REG_BEACON_VAP1_BEACON_CSA              (BEACON_BASE_ADDRESS + 0x18)
#define	REG_BEACON_VAP2_BEACON_ACTIVATE         (BEACON_BASE_ADDRESS + 0x1C)
#define	REG_BEACON_VAP2_BEACON_OFFSET           (BEACON_BASE_ADDRESS + 0x20)
#define	REG_BEACON_VAP2_BEACON_CSA              (BEACON_BASE_ADDRESS + 0x24)
#define	REG_BEACON_VAP3_BEACON_ACTIVATE         (BEACON_BASE_ADDRESS + 0x28)
#define	REG_BEACON_VAP3_BEACON_OFFSET           (BEACON_BASE_ADDRESS + 0x2C)
#define	REG_BEACON_VAP3_BEACON_CSA              (BEACON_BASE_ADDRESS + 0x30)
#define	REG_BEACON_VAP4_BEACON_ACTIVATE         (BEACON_BASE_ADDRESS + 0x34)
#define	REG_BEACON_VAP4_BEACON_OFFSET           (BEACON_BASE_ADDRESS + 0x38)
#define	REG_BEACON_VAP4_BEACON_CSA              (BEACON_BASE_ADDRESS + 0x3C)
#define	REG_BEACON_VAP5_BEACON_ACTIVATE         (BEACON_BASE_ADDRESS + 0x40)
#define	REG_BEACON_VAP5_BEACON_OFFSET           (BEACON_BASE_ADDRESS + 0x44)
#define	REG_BEACON_VAP5_BEACON_CSA              (BEACON_BASE_ADDRESS + 0x48)
#define	REG_BEACON_VAP6_BEACON_ACTIVATE         (BEACON_BASE_ADDRESS + 0x4C)
#define	REG_BEACON_VAP6_BEACON_OFFSET           (BEACON_BASE_ADDRESS + 0x50)
#define	REG_BEACON_VAP6_BEACON_CSA              (BEACON_BASE_ADDRESS + 0x54)
#define	REG_BEACON_VAP7_BEACON_ACTIVATE         (BEACON_BASE_ADDRESS + 0x58)
#define	REG_BEACON_VAP7_BEACON_OFFSET           (BEACON_BASE_ADDRESS + 0x5C)
#define	REG_BEACON_VAP7_BEACON_CSA              (BEACON_BASE_ADDRESS + 0x60)
#define	REG_BEACON_VAP8_BEACON_ACTIVATE         (BEACON_BASE_ADDRESS + 0x64)
#define	REG_BEACON_VAP8_BEACON_OFFSET           (BEACON_BASE_ADDRESS + 0x68)
#define	REG_BEACON_VAP8_BEACON_CSA              (BEACON_BASE_ADDRESS + 0x6C)
#define	REG_BEACON_VAP9_BEACON_ACTIVATE         (BEACON_BASE_ADDRESS + 0x70)
#define	REG_BEACON_VAP9_BEACON_OFFSET           (BEACON_BASE_ADDRESS + 0x74)
#define	REG_BEACON_VAP9_BEACON_CSA              (BEACON_BASE_ADDRESS + 0x78)
#define	REG_BEACON_VAP10_BEACON_ACTIVATE        (BEACON_BASE_ADDRESS + 0x7C)
#define	REG_BEACON_VAP10_BEACON_OFFSET          (BEACON_BASE_ADDRESS + 0x80)
#define	REG_BEACON_VAP10_BEACON_CSA             (BEACON_BASE_ADDRESS + 0x84)
#define	REG_BEACON_VAP11_BEACON_ACTIVATE        (BEACON_BASE_ADDRESS + 0x88)
#define	REG_BEACON_VAP11_BEACON_OFFSET          (BEACON_BASE_ADDRESS + 0x8C)
#define	REG_BEACON_VAP11_BEACON_CSA             (BEACON_BASE_ADDRESS + 0x90)
#define	REG_BEACON_VAP12_BEACON_ACTIVATE        (BEACON_BASE_ADDRESS + 0x94)
#define	REG_BEACON_VAP12_BEACON_OFFSET          (BEACON_BASE_ADDRESS + 0x98)
#define	REG_BEACON_VAP12_BEACON_CSA             (BEACON_BASE_ADDRESS + 0x9C)
#define	REG_BEACON_VAP13_BEACON_ACTIVATE        (BEACON_BASE_ADDRESS + 0xA0)
#define	REG_BEACON_VAP13_BEACON_OFFSET          (BEACON_BASE_ADDRESS + 0xA4)
#define	REG_BEACON_VAP13_BEACON_CSA             (BEACON_BASE_ADDRESS + 0xA8)
#define	REG_BEACON_VAP14_BEACON_ACTIVATE        (BEACON_BASE_ADDRESS + 0xAC)
#define	REG_BEACON_VAP14_BEACON_OFFSET          (BEACON_BASE_ADDRESS + 0xB0)
#define	REG_BEACON_VAP14_BEACON_CSA             (BEACON_BASE_ADDRESS + 0xB4)
#define	REG_BEACON_VAP15_BEACON_ACTIVATE        (BEACON_BASE_ADDRESS + 0xB8)
#define	REG_BEACON_VAP15_BEACON_OFFSET          (BEACON_BASE_ADDRESS + 0xBC)
#define	REG_BEACON_VAP15_BEACON_CSA             (BEACON_BASE_ADDRESS + 0xC0)
#define	REG_BEACON_BEACON_CSA_EN                (BEACON_BASE_ADDRESS + 0x184)
#define	REG_BEACON_VAP_ACTIVE_STATUS            (BEACON_BASE_ADDRESS + 0x188)
#define	REG_BEACON_BEACON_COUNTER               (BEACON_BASE_ADDRESS + 0x18C)
#define	REG_BEACON_VERIFICATION_MODE            (BEACON_BASE_ADDRESS + 0x190)
#define	REG_BEACON_BEACON_LOGGER_CFG            (BEACON_BASE_ADDRESS + 0x194)
#define	REG_BEACON_BEACON_LOGGER_BUSY           (BEACON_BASE_ADDRESS + 0x198)
#define	REG_BEACON_VAP01_TSF_OFFSET             (BEACON_BASE_ADDRESS + 0x19C)
#define	REG_BEACON_VAP23_TSF_OFFSET             (BEACON_BASE_ADDRESS + 0x1A0)
#define	REG_BEACON_VAP45_TSF_OFFSET             (BEACON_BASE_ADDRESS + 0x1A4)
#define	REG_BEACON_VAP67_TSF_OFFSET             (BEACON_BASE_ADDRESS + 0x1A8)
#define	REG_BEACON_VAP89_TSF_OFFSET             (BEACON_BASE_ADDRESS + 0x1AC)
#define	REG_BEACON_VAP1011_TSF_OFFSET           (BEACON_BASE_ADDRESS + 0x1B0)
#define	REG_BEACON_VAP1213_TSF_OFFSET           (BEACON_BASE_ADDRESS + 0x1B4)
#define	REG_BEACON_VAP1415_TSF_OFFSET           (BEACON_BASE_ADDRESS + 0x1B8)
#define	REG_BEACON_MULTI_BSSID_PRIMARY_VAP      (BEACON_BASE_ADDRESS + 0x1DC)
#define	REG_BEACON_MULTI_BSSID_VAPS_IN_GROUP    (BEACON_BASE_ADDRESS + 0x1E0)
#define	REG_BEACON_VAP0_BEACON_RO_COUNTERS      (BEACON_BASE_ADDRESS + 0x1E4)
#define	REG_BEACON_VAP1_BEACON_RO_COUNTERS      (BEACON_BASE_ADDRESS + 0x1E8)
#define	REG_BEACON_VAP2_BEACON_RO_COUNTERS      (BEACON_BASE_ADDRESS + 0x1EC)
#define	REG_BEACON_VAP3_BEACON_RO_COUNTERS      (BEACON_BASE_ADDRESS + 0x1F0)
#define	REG_BEACON_VAP4_BEACON_RO_COUNTERS      (BEACON_BASE_ADDRESS + 0x1F4)
#define	REG_BEACON_VAP5_BEACON_RO_COUNTERS      (BEACON_BASE_ADDRESS + 0x1F8)
#define	REG_BEACON_VAP6_BEACON_RO_COUNTERS      (BEACON_BASE_ADDRESS + 0x1FC)
#define	REG_BEACON_VAP7_BEACON_RO_COUNTERS      (BEACON_BASE_ADDRESS + 0x200)
#define	REG_BEACON_VAP8_BEACON_RO_COUNTERS      (BEACON_BASE_ADDRESS + 0x204)
#define	REG_BEACON_VAP9_BEACON_RO_COUNTERS      (BEACON_BASE_ADDRESS + 0x208)
#define	REG_BEACON_VAP10_BEACON_RO_COUNTERS     (BEACON_BASE_ADDRESS + 0x20C)
#define	REG_BEACON_VAP11_BEACON_RO_COUNTERS     (BEACON_BASE_ADDRESS + 0x210)
#define	REG_BEACON_VAP12_BEACON_RO_COUNTERS     (BEACON_BASE_ADDRESS + 0x214)
#define	REG_BEACON_VAP13_BEACON_RO_COUNTERS     (BEACON_BASE_ADDRESS + 0x218)
#define	REG_BEACON_VAP14_BEACON_RO_COUNTERS     (BEACON_BASE_ADDRESS + 0x21C)
#define	REG_BEACON_VAP15_BEACON_RO_COUNTERS     (BEACON_BASE_ADDRESS + 0x220)
#define	REG_BEACON_BEACON_PERSISTENCE_EN        (BEACON_BASE_ADDRESS + 0x264)
#define	REG_BEACON_BEACON_COLOR_EN              (BEACON_BASE_ADDRESS + 0x268)
#define	REG_BEACON_BEACON_START_FIELD_CFG       (BEACON_BASE_ADDRESS + 0x26C)
/*---------------------------------------------------------------------------------
/						Data Type Definition										
/----------------------------------------------------------------------------------*/
/*REG_BEACON_BEACON_LCM 0x0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 lcm : 16; //Greatest common division, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegBeaconBeaconLcm_u;

/*REG_BEACON_VAP0_BEACON_ACTIVATE 0x4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap0BeaconEn : 1; //Beacon enable bit, reset value: 0x0, access type: RW
		uint32 reserved0 : 7;
		uint32 vap0BeaconInterval : 16; //Beacon interval in TUs , reset value: 0x0, access type: RW
		uint32 vap0DtimInterval : 8; //DTIM interval in TIM units, reset value: 0x0, access type: RW
	} bitFields;
} RegBeaconVap0BeaconActivate_u;

/*REG_BEACON_VAP0_BEACON_OFFSET 0x8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap0BeaconOffset : 16; //Beacon offset relative to virtual zero point, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegBeaconVap0BeaconOffset_u;

/*REG_BEACON_VAP0_BEACON_CSA 0xC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 16;
		uint32 vap0CsaCounter : 8; //Current Channel Switch Announcement count, reset value: 0x0, access type: RO
		uint32 reserved1 : 8;
	} bitFields;
} RegBeaconVap0BeaconCsa_u;

/*REG_BEACON_VAP1_BEACON_ACTIVATE 0x10 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap1BeaconEn : 1; //Beacon enable bit, reset value: 0x0, access type: RW
		uint32 reserved0 : 7;
		uint32 vap1BeaconInterval : 16; //Beacon interval in TUs , reset value: 0x0, access type: RW
		uint32 vap1DtimInterval : 8; //DTIM interval in TIM units, reset value: 0x0, access type: RW
	} bitFields;
} RegBeaconVap1BeaconActivate_u;

/*REG_BEACON_VAP1_BEACON_OFFSET 0x14 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap1BeaconOffset : 16; //Beacon offset relative to virtual zero point, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegBeaconVap1BeaconOffset_u;

/*REG_BEACON_VAP1_BEACON_CSA 0x18 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 16;
		uint32 vap1CsaCounter : 8; //Current Channel Switch Announcement count, reset value: 0x0, access type: RO
		uint32 reserved1 : 8;
	} bitFields;
} RegBeaconVap1BeaconCsa_u;

/*REG_BEACON_VAP2_BEACON_ACTIVATE 0x1C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap2BeaconEn : 1; //Beacon enable bit, reset value: 0x0, access type: RW
		uint32 reserved0 : 7;
		uint32 vap2BeaconInterval : 16; //Beacon interval in TUs , reset value: 0x0, access type: RW
		uint32 vap2DtimInterval : 8; //DTIM interval in TIM units, reset value: 0x0, access type: RW
	} bitFields;
} RegBeaconVap2BeaconActivate_u;

/*REG_BEACON_VAP2_BEACON_OFFSET 0x20 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap2BeaconOffset : 16; //Beacon offset relative to virtual zero point, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegBeaconVap2BeaconOffset_u;

/*REG_BEACON_VAP2_BEACON_CSA 0x24 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 16;
		uint32 vap2CsaCounter : 8; //Current Channel Switch Announcement count, reset value: 0x0, access type: RO
		uint32 reserved1 : 8;
	} bitFields;
} RegBeaconVap2BeaconCsa_u;

/*REG_BEACON_VAP3_BEACON_ACTIVATE 0x28 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap3BeaconEn : 1; //Beacon enable bit, reset value: 0x0, access type: RW
		uint32 reserved0 : 7;
		uint32 vap3BeaconInterval : 16; //Beacon interval in TUs , reset value: 0x0, access type: RW
		uint32 vap3DtimInterval : 8; //DTIM interval in TIM units, reset value: 0x0, access type: RW
	} bitFields;
} RegBeaconVap3BeaconActivate_u;

/*REG_BEACON_VAP3_BEACON_OFFSET 0x2C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap3BeaconOffset : 16; //Beacon offset relative to virtual zero point, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegBeaconVap3BeaconOffset_u;

/*REG_BEACON_VAP3_BEACON_CSA 0x30 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 16;
		uint32 vap3CsaCounter : 8; //Current Channel Switch Announcement count, reset value: 0x0, access type: RO
		uint32 reserved1 : 8;
	} bitFields;
} RegBeaconVap3BeaconCsa_u;

/*REG_BEACON_VAP4_BEACON_ACTIVATE 0x34 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap4BeaconEn : 1; //Beacon enable bit, reset value: 0x0, access type: RW
		uint32 reserved0 : 7;
		uint32 vap4BeaconInterval : 16; //Beacon interval in TUs , reset value: 0x0, access type: RW
		uint32 vap4DtimInterval : 8; //DTIM interval in TIM units, reset value: 0x0, access type: RW
	} bitFields;
} RegBeaconVap4BeaconActivate_u;

/*REG_BEACON_VAP4_BEACON_OFFSET 0x38 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap4BeaconOffset : 16; //Beacon offset relative to virtual zero point, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegBeaconVap4BeaconOffset_u;

/*REG_BEACON_VAP4_BEACON_CSA 0x3C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 16;
		uint32 vap4CsaCounter : 8; //Current Channel Switch Announcement count, reset value: 0x0, access type: RO
		uint32 reserved1 : 8;
	} bitFields;
} RegBeaconVap4BeaconCsa_u;

/*REG_BEACON_VAP5_BEACON_ACTIVATE 0x40 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap5BeaconEn : 1; //Beacon enable bit, reset value: 0x0, access type: RW
		uint32 reserved0 : 7;
		uint32 vap5BeaconInterval : 16; //Beacon interval in TUs , reset value: 0x0, access type: RW
		uint32 vap5DtimInterval : 8; //DTIM interval in TIM units, reset value: 0x0, access type: RW
	} bitFields;
} RegBeaconVap5BeaconActivate_u;

/*REG_BEACON_VAP5_BEACON_OFFSET 0x44 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap5BeaconOffset : 16; //Beacon offset relative to virtual zero point, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegBeaconVap5BeaconOffset_u;

/*REG_BEACON_VAP5_BEACON_CSA 0x48 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 16;
		uint32 vap5CsaCounter : 8; //Current Channel Switch Announcement count, reset value: 0x0, access type: RO
		uint32 reserved1 : 8;
	} bitFields;
} RegBeaconVap5BeaconCsa_u;

/*REG_BEACON_VAP6_BEACON_ACTIVATE 0x4C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap6BeaconEn : 1; //Beacon enable bit, reset value: 0x0, access type: RW
		uint32 reserved0 : 7;
		uint32 vap6BeaconInterval : 16; //Beacon interval in TUs , reset value: 0x0, access type: RW
		uint32 vap6DtimInterval : 8; //DTIM interval in TIM units, reset value: 0x0, access type: RW
	} bitFields;
} RegBeaconVap6BeaconActivate_u;

/*REG_BEACON_VAP6_BEACON_OFFSET 0x50 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap6BeaconOffset : 16; //Beacon offset relative to virtual zero point, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegBeaconVap6BeaconOffset_u;

/*REG_BEACON_VAP6_BEACON_CSA 0x54 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 16;
		uint32 vap6CsaCounter : 8; //Current Channel Switch Announcement count, reset value: 0x0, access type: RO
		uint32 reserved1 : 8;
	} bitFields;
} RegBeaconVap6BeaconCsa_u;

/*REG_BEACON_VAP7_BEACON_ACTIVATE 0x58 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap7BeaconEn : 1; //Beacon enable bit, reset value: 0x0, access type: RW
		uint32 reserved0 : 7;
		uint32 vap7BeaconInterval : 16; //Beacon interval in TUs , reset value: 0x0, access type: RW
		uint32 vap7DtimInterval : 8; //DTIM interval in TIM units, reset value: 0x0, access type: RW
	} bitFields;
} RegBeaconVap7BeaconActivate_u;

/*REG_BEACON_VAP7_BEACON_OFFSET 0x5C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap7BeaconOffset : 16; //Beacon offset relative to virtual zero point, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegBeaconVap7BeaconOffset_u;

/*REG_BEACON_VAP7_BEACON_CSA 0x60 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 16;
		uint32 vap7CsaCounter : 8; //Current Channel Switch Announcement count, reset value: 0x0, access type: RO
		uint32 reserved1 : 8;
	} bitFields;
} RegBeaconVap7BeaconCsa_u;

/*REG_BEACON_VAP8_BEACON_ACTIVATE 0x64 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap8BeaconEn : 1; //Beacon enable bit, reset value: 0x0, access type: RW
		uint32 reserved0 : 7;
		uint32 vap8BeaconInterval : 16; //Beacon interval in TUs , reset value: 0x0, access type: RW
		uint32 vap8DtimInterval : 8; //DTIM interval in TIM units, reset value: 0x0, access type: RW
	} bitFields;
} RegBeaconVap8BeaconActivate_u;

/*REG_BEACON_VAP8_BEACON_OFFSET 0x68 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap8BeaconOffset : 16; //Beacon offset relative to virtual zero point, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegBeaconVap8BeaconOffset_u;

/*REG_BEACON_VAP8_BEACON_CSA 0x6C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 16;
		uint32 vap8CsaCounter : 8; //Current Channel Switch Announcement count, reset value: 0x0, access type: RO
		uint32 reserved1 : 8;
	} bitFields;
} RegBeaconVap8BeaconCsa_u;

/*REG_BEACON_VAP9_BEACON_ACTIVATE 0x70 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap9BeaconEn : 1; //Beacon enable bit, reset value: 0x0, access type: RW
		uint32 reserved0 : 7;
		uint32 vap9BeaconInterval : 16; //Beacon interval in TUs , reset value: 0x0, access type: RW
		uint32 vap9DtimInterval : 8; //DTIM interval in TIM units, reset value: 0x0, access type: RW
	} bitFields;
} RegBeaconVap9BeaconActivate_u;

/*REG_BEACON_VAP9_BEACON_OFFSET 0x74 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap9BeaconOffset : 16; //Beacon offset relative to virtual zero point, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegBeaconVap9BeaconOffset_u;

/*REG_BEACON_VAP9_BEACON_CSA 0x78 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 16;
		uint32 vap9CsaCounter : 8; //Current Channel Switch Announcement count, reset value: 0x0, access type: RO
		uint32 reserved1 : 8;
	} bitFields;
} RegBeaconVap9BeaconCsa_u;

/*REG_BEACON_VAP10_BEACON_ACTIVATE 0x7C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap10BeaconEn : 1; //Beacon enable bit, reset value: 0x0, access type: RW
		uint32 reserved0 : 7;
		uint32 vap10BeaconInterval : 16; //Beacon interval in TUs , reset value: 0x0, access type: RW
		uint32 vap10DtimInterval : 8; //DTIM interval in TIM units, reset value: 0x0, access type: RW
	} bitFields;
} RegBeaconVap10BeaconActivate_u;

/*REG_BEACON_VAP10_BEACON_OFFSET 0x80 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap10BeaconOffset : 16; //Beacon offset relative to virtual zero point, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegBeaconVap10BeaconOffset_u;

/*REG_BEACON_VAP10_BEACON_CSA 0x84 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 16;
		uint32 vap10CsaCounter : 8; //Current Channel Switch Announcement count, reset value: 0x0, access type: RO
		uint32 reserved1 : 8;
	} bitFields;
} RegBeaconVap10BeaconCsa_u;

/*REG_BEACON_VAP11_BEACON_ACTIVATE 0x88 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap11BeaconEn : 1; //Beacon enable bit, reset value: 0x0, access type: RW
		uint32 reserved0 : 7;
		uint32 vap11BeaconInterval : 16; //Beacon interval in TUs , reset value: 0x0, access type: RW
		uint32 vap11DtimInterval : 8; //DTIM interval in TIM units, reset value: 0x0, access type: RW
	} bitFields;
} RegBeaconVap11BeaconActivate_u;

/*REG_BEACON_VAP11_BEACON_OFFSET 0x8C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap11BeaconOffset : 16; //Beacon offset relative to virtual zero point, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegBeaconVap11BeaconOffset_u;

/*REG_BEACON_VAP11_BEACON_CSA 0x90 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 16;
		uint32 vap11CsaCounter : 8; //Current Channel Switch Announcement count, reset value: 0x0, access type: RO
		uint32 reserved1 : 8;
	} bitFields;
} RegBeaconVap11BeaconCsa_u;

/*REG_BEACON_VAP12_BEACON_ACTIVATE 0x94 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap12BeaconEn : 1; //Beacon enable bit, reset value: 0x0, access type: RW
		uint32 reserved0 : 7;
		uint32 vap12BeaconInterval : 16; //Beacon interval in TUs , reset value: 0x0, access type: RW
		uint32 vap12DtimInterval : 8; //DTIM interval in TIM units, reset value: 0x0, access type: RW
	} bitFields;
} RegBeaconVap12BeaconActivate_u;

/*REG_BEACON_VAP12_BEACON_OFFSET 0x98 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap12BeaconOffset : 16; //Beacon offset relative to virtual zero point, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegBeaconVap12BeaconOffset_u;

/*REG_BEACON_VAP12_BEACON_CSA 0x9C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 16;
		uint32 vap12CsaCounter : 8; //Current Channel Switch Announcement count, reset value: 0x0, access type: RO
		uint32 reserved1 : 8;
	} bitFields;
} RegBeaconVap12BeaconCsa_u;

/*REG_BEACON_VAP13_BEACON_ACTIVATE 0xA0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap13BeaconEn : 1; //Beacon enable bit, reset value: 0x0, access type: RW
		uint32 reserved0 : 7;
		uint32 vap13BeaconInterval : 16; //Beacon interval in TUs , reset value: 0x0, access type: RW
		uint32 vap13DtimInterval : 8; //DTIM interval in TIM units, reset value: 0x0, access type: RW
	} bitFields;
} RegBeaconVap13BeaconActivate_u;

/*REG_BEACON_VAP13_BEACON_OFFSET 0xA4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap13BeaconOffset : 16; //Beacon offset relative to virtual zero point, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegBeaconVap13BeaconOffset_u;

/*REG_BEACON_VAP13_BEACON_CSA 0xA8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 16;
		uint32 vap13CsaCounter : 8; //Current Channel Switch Announcement count, reset value: 0x0, access type: RO
		uint32 reserved1 : 8;
	} bitFields;
} RegBeaconVap13BeaconCsa_u;

/*REG_BEACON_VAP14_BEACON_ACTIVATE 0xAC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap14BeaconEn : 1; //Beacon enable bit, reset value: 0x0, access type: RW
		uint32 reserved0 : 7;
		uint32 vap14BeaconInterval : 16; //Beacon interval in TUs , reset value: 0x0, access type: RW
		uint32 vap14DtimInterval : 8; //DTIM interval in TIM units, reset value: 0x0, access type: RW
	} bitFields;
} RegBeaconVap14BeaconActivate_u;

/*REG_BEACON_VAP14_BEACON_OFFSET 0xB0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap14BeaconOffset : 16; //Beacon offset relative to virtual zero point, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegBeaconVap14BeaconOffset_u;

/*REG_BEACON_VAP14_BEACON_CSA 0xB4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 16;
		uint32 vap14CsaCounter : 8; //Current Channel Switch Announcement count, reset value: 0x0, access type: RO
		uint32 reserved1 : 8;
	} bitFields;
} RegBeaconVap14BeaconCsa_u;

/*REG_BEACON_VAP15_BEACON_ACTIVATE 0xB8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap15BeaconEn : 1; //Beacon enable bit, reset value: 0x0, access type: RW
		uint32 reserved0 : 7;
		uint32 vap15BeaconInterval : 16; //Beacon interval in TUs , reset value: 0x0, access type: RW
		uint32 vap15DtimInterval : 8; //DTIM interval in TIM units, reset value: 0x0, access type: RW
	} bitFields;
} RegBeaconVap15BeaconActivate_u;

/*REG_BEACON_VAP15_BEACON_OFFSET 0xBC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap15BeaconOffset : 16; //Beacon offset relative to virtual zero point, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegBeaconVap15BeaconOffset_u;

/*REG_BEACON_VAP15_BEACON_CSA 0xC0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 16;
		uint32 vap15CsaCounter : 8; //Current Channel Switch Announcement count, reset value: 0x0, access type: RO
		uint32 reserved1 : 8;
	} bitFields;
} RegBeaconVap15BeaconCsa_u;

/*REG_BEACON_BEACON_CSA_EN 0x184 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 csaBeaconCountEn : 16; //Activate all enabled CSA counters, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegBeaconBeaconCsaEn_u;

/*REG_BEACON_VAP_ACTIVE_STATUS 0x188 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vapActiveStatus : 16; //List of active VAPs, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegBeaconVapActiveStatus_u;

/*REG_BEACON_BEACON_COUNTER 0x18C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 beaconReqCounter : 20; //number of beacons transmitted in the system, reset value: 0x0, access type: RO
		uint32 reserved0 : 11;
		uint32 beaconReqCounterClr : 1; //Clears Beacon counter, reset value: 0x0, access type: WO
	} bitFields;
} RegBeaconBeaconCounter_u;

/*REG_BEACON_VERIFICATION_MODE 0x190 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 verificationMode : 1; //Verification mode to shift tsf counter, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegBeaconVerificationMode_u;

/*REG_BEACON_BEACON_LOGGER_CFG 0x194 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 beaconTimersLoggerPriority : 2; //no description, reset value: 0x0, access type: RW
		uint32 beaconTimersLoggerEn : 1; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
		uint32 beaconLoggerMessageSel : 1; //0: Beacon request message type , 1: Beacon activate message type, reset value: 0x0, access type: RW
		uint32 reserved1 : 27;
	} bitFields;
} RegBeaconBeaconLoggerCfg_u;

/*REG_BEACON_BEACON_LOGGER_BUSY 0x198 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 beaconTimersLoggerBusy : 1; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegBeaconBeaconLoggerBusy_u;

/*REG_BEACON_VAP01_TSF_OFFSET 0x19C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap0BeaconFinalOffset : 16; //no description, reset value: 0x0, access type: RO
		uint32 vap1BeaconFinalOffset : 16; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegBeaconVap01TsfOffset_u;

/*REG_BEACON_VAP23_TSF_OFFSET 0x1A0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap2BeaconFinalOffset : 16; //no description, reset value: 0x0, access type: RO
		uint32 vap3BeaconFinalOffset : 16; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegBeaconVap23TsfOffset_u;

/*REG_BEACON_VAP45_TSF_OFFSET 0x1A4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap4BeaconFinalOffset : 16; //no description, reset value: 0x0, access type: RO
		uint32 vap5BeaconFinalOffset : 16; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegBeaconVap45TsfOffset_u;

/*REG_BEACON_VAP67_TSF_OFFSET 0x1A8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap6BeaconFinalOffset : 16; //no description, reset value: 0x0, access type: RO
		uint32 vap7BeaconFinalOffset : 16; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegBeaconVap67TsfOffset_u;

/*REG_BEACON_VAP89_TSF_OFFSET 0x1AC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap8BeaconFinalOffset : 16; //no description, reset value: 0x0, access type: RO
		uint32 vap9BeaconFinalOffset : 16; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegBeaconVap89TsfOffset_u;

/*REG_BEACON_VAP1011_TSF_OFFSET 0x1B0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap10BeaconFinalOffset : 16; //no description, reset value: 0x0, access type: RO
		uint32 vap11BeaconFinalOffset : 16; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegBeaconVap1011TsfOffset_u;

/*REG_BEACON_VAP1213_TSF_OFFSET 0x1B4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap12BeaconFinalOffset : 16; //no description, reset value: 0x0, access type: RO
		uint32 vap13BeaconFinalOffset : 16; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegBeaconVap1213TsfOffset_u;

/*REG_BEACON_VAP1415_TSF_OFFSET 0x1B8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap14BeaconFinalOffset : 16; //no description, reset value: 0x0, access type: RO
		uint32 vap15BeaconFinalOffset : 16; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegBeaconVap1415TsfOffset_u;

/*REG_BEACON_MULTI_BSSID_PRIMARY_VAP 0x1DC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 multiBssidPrimaryVapIdx : 1; //Primary VAP for Multi BSSID group: , 0: Primary VAP is 0. , 1: Primary VAP is 16., reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegBeaconMultiBssidPrimaryVap_u;

/*REG_BEACON_MULTI_BSSID_VAPS_IN_GROUP 0x1E0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 multiBssidVapsInGroupEn : 16; //VAPs in the group. Valid fields are according to SW work assumptions., reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegBeaconMultiBssidVapsInGroup_u;

/*REG_BEACON_VAP0_BEACON_RO_COUNTERS 0x1E4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 8;
		uint32 dtim0Counter : 8; //Current Dtim counter, reset value: 0x0, access type: RO
		uint32 vap0ColorCounter : 8; //Current Color change count, reset value: 0x0, access type: RO
		uint32 vap0PersistenceCounter : 8; //Current Persistence count, reset value: 0x0, access type: RO
	} bitFields;
} RegBeaconVap0BeaconRoCounters_u;

/*REG_BEACON_VAP1_BEACON_RO_COUNTERS 0x1E8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 8;
		uint32 dtim1Counter : 8; //Current Dtim counter, reset value: 0x0, access type: RO
		uint32 vap1ColorCounter : 8; //Current Color change count, reset value: 0x0, access type: RO
		uint32 vap1PersistenceCounter : 8; //Current Persistence count, reset value: 0x0, access type: RO
	} bitFields;
} RegBeaconVap1BeaconRoCounters_u;

/*REG_BEACON_VAP2_BEACON_RO_COUNTERS 0x1EC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 8;
		uint32 dtim2Counter : 8; //Current Dtim counter, reset value: 0x0, access type: RO
		uint32 vap2ColorCounter : 8; //Current Color change count, reset value: 0x0, access type: RO
		uint32 vap2PersistenceCounter : 8; //Current Persistence count, reset value: 0x0, access type: RO
	} bitFields;
} RegBeaconVap2BeaconRoCounters_u;

/*REG_BEACON_VAP3_BEACON_RO_COUNTERS 0x1F0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 8;
		uint32 dtim3Counter : 8; //Current Dtim counter, reset value: 0x0, access type: RO
		uint32 vap3ColorCounter : 8; //Current Color change count, reset value: 0x0, access type: RO
		uint32 vap3PersistenceCounter : 8; //Current Persistence count, reset value: 0x0, access type: RO
	} bitFields;
} RegBeaconVap3BeaconRoCounters_u;

/*REG_BEACON_VAP4_BEACON_RO_COUNTERS 0x1F4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 8;
		uint32 dtim4Counter : 8; //Current Dtim counter, reset value: 0x0, access type: RO
		uint32 vap4ColorCounter : 8; //Current Color change count, reset value: 0x0, access type: RO
		uint32 vap4PersistenceCounter : 8; //Current Persistence count, reset value: 0x0, access type: RO
	} bitFields;
} RegBeaconVap4BeaconRoCounters_u;

/*REG_BEACON_VAP5_BEACON_RO_COUNTERS 0x1F8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 8;
		uint32 dtim5Counter : 8; //Current Dtim counter, reset value: 0x0, access type: RO
		uint32 vap5ColorCounter : 8; //Current Color change count, reset value: 0x0, access type: RO
		uint32 vap5PersistenceCounter : 8; //Current Persistence count, reset value: 0x0, access type: RO
	} bitFields;
} RegBeaconVap5BeaconRoCounters_u;

/*REG_BEACON_VAP6_BEACON_RO_COUNTERS 0x1FC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 8;
		uint32 dtim6Counter : 8; //Current Dtim counter, reset value: 0x0, access type: RO
		uint32 vap6ColorCounter : 8; //Current Color change count, reset value: 0x0, access type: RO
		uint32 vap6PersistenceCounter : 8; //Current Persistence count, reset value: 0x0, access type: RO
	} bitFields;
} RegBeaconVap6BeaconRoCounters_u;

/*REG_BEACON_VAP7_BEACON_RO_COUNTERS 0x200 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 8;
		uint32 dtim7Counter : 8; //Current Dtim counter, reset value: 0x0, access type: RO
		uint32 vap7ColorCounter : 8; //Current Color change count, reset value: 0x0, access type: RO
		uint32 vap7PersistenceCounter : 8; //Current Persistence count, reset value: 0x0, access type: RO
	} bitFields;
} RegBeaconVap7BeaconRoCounters_u;

/*REG_BEACON_VAP8_BEACON_RO_COUNTERS 0x204 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 8;
		uint32 dtim8Counter : 8; //Current Dtim counter, reset value: 0x0, access type: RO
		uint32 vap8ColorCounter : 8; //Current Color change count, reset value: 0x0, access type: RO
		uint32 vap8PersistenceCounter : 8; //Current Persistence count, reset value: 0x0, access type: RO
	} bitFields;
} RegBeaconVap8BeaconRoCounters_u;

/*REG_BEACON_VAP9_BEACON_RO_COUNTERS 0x208 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 8;
		uint32 dtim9Counter : 8; //Current Dtim counter, reset value: 0x0, access type: RO
		uint32 vap9ColorCounter : 8; //Current Color change count, reset value: 0x0, access type: RO
		uint32 vap9PersistenceCounter : 8; //Current Persistence count, reset value: 0x0, access type: RO
	} bitFields;
} RegBeaconVap9BeaconRoCounters_u;

/*REG_BEACON_VAP10_BEACON_RO_COUNTERS 0x20C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 8;
		uint32 dtim10Counter : 8; //Current Dtim counter, reset value: 0x0, access type: RO
		uint32 vap10ColorCounter : 8; //Current Color change count, reset value: 0x0, access type: RO
		uint32 vap10PersistenceCounter : 8; //Current Persistence count, reset value: 0x0, access type: RO
	} bitFields;
} RegBeaconVap10BeaconRoCounters_u;

/*REG_BEACON_VAP11_BEACON_RO_COUNTERS 0x210 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 8;
		uint32 dtim11Counter : 8; //Current Dtim counter, reset value: 0x0, access type: RO
		uint32 vap11ColorCounter : 8; //Current Color change count, reset value: 0x0, access type: RO
		uint32 vap11PersistenceCounter : 8; //Current Persistence count, reset value: 0x0, access type: RO
	} bitFields;
} RegBeaconVap11BeaconRoCounters_u;

/*REG_BEACON_VAP12_BEACON_RO_COUNTERS 0x214 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 8;
		uint32 dtim12Counter : 8; //Current Dtim counter, reset value: 0x0, access type: RO
		uint32 vap12ColorCounter : 8; //Current Color change count, reset value: 0x0, access type: RO
		uint32 vap12PersistenceCounter : 8; //Current Persistence count, reset value: 0x0, access type: RO
	} bitFields;
} RegBeaconVap12BeaconRoCounters_u;

/*REG_BEACON_VAP13_BEACON_RO_COUNTERS 0x218 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 8;
		uint32 dtim13Counter : 8; //Current Dtim counter, reset value: 0x0, access type: RO
		uint32 vap13ColorCounter : 8; //Current Color change count, reset value: 0x0, access type: RO
		uint32 vap13PersistenceCounter : 8; //Current Persistence count, reset value: 0x0, access type: RO
	} bitFields;
} RegBeaconVap13BeaconRoCounters_u;

/*REG_BEACON_VAP14_BEACON_RO_COUNTERS 0x21C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 8;
		uint32 dtim14Counter : 8; //Current Dtim counter, reset value: 0x0, access type: RO
		uint32 vap14ColorCounter : 8; //Current Color change count, reset value: 0x0, access type: RO
		uint32 vap14PersistenceCounter : 8; //Current Persistence count, reset value: 0x0, access type: RO
	} bitFields;
} RegBeaconVap14BeaconRoCounters_u;

/*REG_BEACON_VAP15_BEACON_RO_COUNTERS 0x220 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 8;
		uint32 dtim15Counter : 8; //Current Dtim counter, reset value: 0x0, access type: RO
		uint32 vap15ColorCounter : 8; //Current Color change count, reset value: 0x0, access type: RO
		uint32 vap15PersistenceCounter : 8; //Current Persistence count, reset value: 0x0, access type: RO
	} bitFields;
} RegBeaconVap15BeaconRoCounters_u;

/*REG_BEACON_BEACON_PERSISTENCE_EN 0x264 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 persistenceBeaconCountEn : 16; //Activate all enabled PERSISTENCE counters, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegBeaconBeaconPersistenceEn_u;

/*REG_BEACON_BEACON_COLOR_EN 0x268 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 colorBeaconCountEn : 16; //Activate all enabled COLOR counters, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegBeaconBeaconColorEn_u;

/*REG_BEACON_BEACON_START_FIELD_CFG 0x26C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 fieldUpdateValue : 8; //For DTIM first period: DTIM start count value in TIM units , For CSA and Color counters: number of TTIMs to count, reset value: 0x0, access type: RW
		uint32 fieldUpdateVapIdx : 4; //VAP index to update the DTIM start period, reset value: 0x0, access type: RW
		uint32 reserved0 : 4;
		uint32 fieldUpdateType : 4; //Field update type: , 0001: Dtim first period , 0010: CSA counter , 0100: Color counter , 1000: Persistence Counter, reset value: 0x0, access type: RW
		uint32 reserved1 : 12;
	} bitFields;
} RegBeaconBeaconStartFieldCfg_u;

//========================================
/* REG_BEACON_BEACON_LCM 0x0 */
#define B0_BEACON_BEACON_LCM_FIELD_LCM_MASK                                                                       0x0000FFFF
//========================================
/* REG_BEACON_VAP0_BEACON_ACTIVATE 0x4 */
#define B0_BEACON_VAP0_BEACON_ACTIVATE_FIELD_VAP0_BEACON_EN_MASK                                                  0x00000001
#define B0_BEACON_VAP0_BEACON_ACTIVATE_FIELD_VAP0_BEACON_INTERVAL_MASK                                            0x00FFFF00
#define B0_BEACON_VAP0_BEACON_ACTIVATE_FIELD_VAP0_DTIM_INTERVAL_MASK                                              0xFF000000
//========================================
/* REG_BEACON_VAP0_BEACON_OFFSET 0x8 */
#define B0_BEACON_VAP0_BEACON_OFFSET_FIELD_VAP0_BEACON_OFFSET_MASK                                                0x0000FFFF
//========================================
/* REG_BEACON_VAP0_BEACON_CSA 0xC */
#define B0_BEACON_VAP0_BEACON_CSA_FIELD_VAP0_CSA_COUNTER_MASK                                                     0x00FF0000
//========================================
/* REG_BEACON_VAP1_BEACON_ACTIVATE 0x10 */
#define B0_BEACON_VAP1_BEACON_ACTIVATE_FIELD_VAP1_BEACON_EN_MASK                                                  0x00000001
#define B0_BEACON_VAP1_BEACON_ACTIVATE_FIELD_VAP1_BEACON_INTERVAL_MASK                                            0x00FFFF00
#define B0_BEACON_VAP1_BEACON_ACTIVATE_FIELD_VAP1_DTIM_INTERVAL_MASK                                              0xFF000000
//========================================
/* REG_BEACON_VAP1_BEACON_OFFSET 0x14 */
#define B0_BEACON_VAP1_BEACON_OFFSET_FIELD_VAP1_BEACON_OFFSET_MASK                                                0x0000FFFF
//========================================
/* REG_BEACON_VAP1_BEACON_CSA 0x18 */
#define B0_BEACON_VAP1_BEACON_CSA_FIELD_VAP1_CSA_COUNTER_MASK                                                     0x00FF0000
//========================================
/* REG_BEACON_VAP2_BEACON_ACTIVATE 0x1C */
#define B0_BEACON_VAP2_BEACON_ACTIVATE_FIELD_VAP2_BEACON_EN_MASK                                                  0x00000001
#define B0_BEACON_VAP2_BEACON_ACTIVATE_FIELD_VAP2_BEACON_INTERVAL_MASK                                            0x00FFFF00
#define B0_BEACON_VAP2_BEACON_ACTIVATE_FIELD_VAP2_DTIM_INTERVAL_MASK                                              0xFF000000
//========================================
/* REG_BEACON_VAP2_BEACON_OFFSET 0x20 */
#define B0_BEACON_VAP2_BEACON_OFFSET_FIELD_VAP2_BEACON_OFFSET_MASK                                                0x0000FFFF
//========================================
/* REG_BEACON_VAP2_BEACON_CSA 0x24 */
#define B0_BEACON_VAP2_BEACON_CSA_FIELD_VAP2_CSA_COUNTER_MASK                                                     0x00FF0000
//========================================
/* REG_BEACON_VAP3_BEACON_ACTIVATE 0x28 */
#define B0_BEACON_VAP3_BEACON_ACTIVATE_FIELD_VAP3_BEACON_EN_MASK                                                  0x00000001
#define B0_BEACON_VAP3_BEACON_ACTIVATE_FIELD_VAP3_BEACON_INTERVAL_MASK                                            0x00FFFF00
#define B0_BEACON_VAP3_BEACON_ACTIVATE_FIELD_VAP3_DTIM_INTERVAL_MASK                                              0xFF000000
//========================================
/* REG_BEACON_VAP3_BEACON_OFFSET 0x2C */
#define B0_BEACON_VAP3_BEACON_OFFSET_FIELD_VAP3_BEACON_OFFSET_MASK                                                0x0000FFFF
//========================================
/* REG_BEACON_VAP3_BEACON_CSA 0x30 */
#define B0_BEACON_VAP3_BEACON_CSA_FIELD_VAP3_CSA_COUNTER_MASK                                                     0x00FF0000
//========================================
/* REG_BEACON_VAP4_BEACON_ACTIVATE 0x34 */
#define B0_BEACON_VAP4_BEACON_ACTIVATE_FIELD_VAP4_BEACON_EN_MASK                                                  0x00000001
#define B0_BEACON_VAP4_BEACON_ACTIVATE_FIELD_VAP4_BEACON_INTERVAL_MASK                                            0x00FFFF00
#define B0_BEACON_VAP4_BEACON_ACTIVATE_FIELD_VAP4_DTIM_INTERVAL_MASK                                              0xFF000000
//========================================
/* REG_BEACON_VAP4_BEACON_OFFSET 0x38 */
#define B0_BEACON_VAP4_BEACON_OFFSET_FIELD_VAP4_BEACON_OFFSET_MASK                                                0x0000FFFF
//========================================
/* REG_BEACON_VAP4_BEACON_CSA 0x3C */
#define B0_BEACON_VAP4_BEACON_CSA_FIELD_VAP4_CSA_COUNTER_MASK                                                     0x00FF0000
//========================================
/* REG_BEACON_VAP5_BEACON_ACTIVATE 0x40 */
#define B0_BEACON_VAP5_BEACON_ACTIVATE_FIELD_VAP5_BEACON_EN_MASK                                                  0x00000001
#define B0_BEACON_VAP5_BEACON_ACTIVATE_FIELD_VAP5_BEACON_INTERVAL_MASK                                            0x00FFFF00
#define B0_BEACON_VAP5_BEACON_ACTIVATE_FIELD_VAP5_DTIM_INTERVAL_MASK                                              0xFF000000
//========================================
/* REG_BEACON_VAP5_BEACON_OFFSET 0x44 */
#define B0_BEACON_VAP5_BEACON_OFFSET_FIELD_VAP5_BEACON_OFFSET_MASK                                                0x0000FFFF
//========================================
/* REG_BEACON_VAP5_BEACON_CSA 0x48 */
#define B0_BEACON_VAP5_BEACON_CSA_FIELD_VAP5_CSA_COUNTER_MASK                                                     0x00FF0000
//========================================
/* REG_BEACON_VAP6_BEACON_ACTIVATE 0x4C */
#define B0_BEACON_VAP6_BEACON_ACTIVATE_FIELD_VAP6_BEACON_EN_MASK                                                  0x00000001
#define B0_BEACON_VAP6_BEACON_ACTIVATE_FIELD_VAP6_BEACON_INTERVAL_MASK                                            0x00FFFF00
#define B0_BEACON_VAP6_BEACON_ACTIVATE_FIELD_VAP6_DTIM_INTERVAL_MASK                                              0xFF000000
//========================================
/* REG_BEACON_VAP6_BEACON_OFFSET 0x50 */
#define B0_BEACON_VAP6_BEACON_OFFSET_FIELD_VAP6_BEACON_OFFSET_MASK                                                0x0000FFFF
//========================================
/* REG_BEACON_VAP6_BEACON_CSA 0x54 */
#define B0_BEACON_VAP6_BEACON_CSA_FIELD_VAP6_CSA_COUNTER_MASK                                                     0x00FF0000
//========================================
/* REG_BEACON_VAP7_BEACON_ACTIVATE 0x58 */
#define B0_BEACON_VAP7_BEACON_ACTIVATE_FIELD_VAP7_BEACON_EN_MASK                                                  0x00000001
#define B0_BEACON_VAP7_BEACON_ACTIVATE_FIELD_VAP7_BEACON_INTERVAL_MASK                                            0x00FFFF00
#define B0_BEACON_VAP7_BEACON_ACTIVATE_FIELD_VAP7_DTIM_INTERVAL_MASK                                              0xFF000000
//========================================
/* REG_BEACON_VAP7_BEACON_OFFSET 0x5C */
#define B0_BEACON_VAP7_BEACON_OFFSET_FIELD_VAP7_BEACON_OFFSET_MASK                                                0x0000FFFF
//========================================
/* REG_BEACON_VAP7_BEACON_CSA 0x60 */
#define B0_BEACON_VAP7_BEACON_CSA_FIELD_VAP7_CSA_COUNTER_MASK                                                     0x00FF0000
//========================================
/* REG_BEACON_VAP8_BEACON_ACTIVATE 0x64 */
#define B0_BEACON_VAP8_BEACON_ACTIVATE_FIELD_VAP8_BEACON_EN_MASK                                                  0x00000001
#define B0_BEACON_VAP8_BEACON_ACTIVATE_FIELD_VAP8_BEACON_INTERVAL_MASK                                            0x00FFFF00
#define B0_BEACON_VAP8_BEACON_ACTIVATE_FIELD_VAP8_DTIM_INTERVAL_MASK                                              0xFF000000
//========================================
/* REG_BEACON_VAP8_BEACON_OFFSET 0x68 */
#define B0_BEACON_VAP8_BEACON_OFFSET_FIELD_VAP8_BEACON_OFFSET_MASK                                                0x0000FFFF
//========================================
/* REG_BEACON_VAP8_BEACON_CSA 0x6C */
#define B0_BEACON_VAP8_BEACON_CSA_FIELD_VAP8_CSA_COUNTER_MASK                                                     0x00FF0000
//========================================
/* REG_BEACON_VAP9_BEACON_ACTIVATE 0x70 */
#define B0_BEACON_VAP9_BEACON_ACTIVATE_FIELD_VAP9_BEACON_EN_MASK                                                  0x00000001
#define B0_BEACON_VAP9_BEACON_ACTIVATE_FIELD_VAP9_BEACON_INTERVAL_MASK                                            0x00FFFF00
#define B0_BEACON_VAP9_BEACON_ACTIVATE_FIELD_VAP9_DTIM_INTERVAL_MASK                                              0xFF000000
//========================================
/* REG_BEACON_VAP9_BEACON_OFFSET 0x74 */
#define B0_BEACON_VAP9_BEACON_OFFSET_FIELD_VAP9_BEACON_OFFSET_MASK                                                0x0000FFFF
//========================================
/* REG_BEACON_VAP9_BEACON_CSA 0x78 */
#define B0_BEACON_VAP9_BEACON_CSA_FIELD_VAP9_CSA_COUNTER_MASK                                                     0x00FF0000
//========================================
/* REG_BEACON_VAP10_BEACON_ACTIVATE 0x7C */
#define B0_BEACON_VAP10_BEACON_ACTIVATE_FIELD_VAP10_BEACON_EN_MASK                                                0x00000001
#define B0_BEACON_VAP10_BEACON_ACTIVATE_FIELD_VAP10_BEACON_INTERVAL_MASK                                          0x00FFFF00
#define B0_BEACON_VAP10_BEACON_ACTIVATE_FIELD_VAP10_DTIM_INTERVAL_MASK                                            0xFF000000
//========================================
/* REG_BEACON_VAP10_BEACON_OFFSET 0x80 */
#define B0_BEACON_VAP10_BEACON_OFFSET_FIELD_VAP10_BEACON_OFFSET_MASK                                              0x0000FFFF
//========================================
/* REG_BEACON_VAP10_BEACON_CSA 0x84 */
#define B0_BEACON_VAP10_BEACON_CSA_FIELD_VAP10_CSA_COUNTER_MASK                                                   0x00FF0000
//========================================
/* REG_BEACON_VAP11_BEACON_ACTIVATE 0x88 */
#define B0_BEACON_VAP11_BEACON_ACTIVATE_FIELD_VAP11_BEACON_EN_MASK                                                0x00000001
#define B0_BEACON_VAP11_BEACON_ACTIVATE_FIELD_VAP11_BEACON_INTERVAL_MASK                                          0x00FFFF00
#define B0_BEACON_VAP11_BEACON_ACTIVATE_FIELD_VAP11_DTIM_INTERVAL_MASK                                            0xFF000000
//========================================
/* REG_BEACON_VAP11_BEACON_OFFSET 0x8C */
#define B0_BEACON_VAP11_BEACON_OFFSET_FIELD_VAP11_BEACON_OFFSET_MASK                                              0x0000FFFF
//========================================
/* REG_BEACON_VAP11_BEACON_CSA 0x90 */
#define B0_BEACON_VAP11_BEACON_CSA_FIELD_VAP11_CSA_COUNTER_MASK                                                   0x00FF0000
//========================================
/* REG_BEACON_VAP12_BEACON_ACTIVATE 0x94 */
#define B0_BEACON_VAP12_BEACON_ACTIVATE_FIELD_VAP12_BEACON_EN_MASK                                                0x00000001
#define B0_BEACON_VAP12_BEACON_ACTIVATE_FIELD_VAP12_BEACON_INTERVAL_MASK                                          0x00FFFF00
#define B0_BEACON_VAP12_BEACON_ACTIVATE_FIELD_VAP12_DTIM_INTERVAL_MASK                                            0xFF000000
//========================================
/* REG_BEACON_VAP12_BEACON_OFFSET 0x98 */
#define B0_BEACON_VAP12_BEACON_OFFSET_FIELD_VAP12_BEACON_OFFSET_MASK                                              0x0000FFFF
//========================================
/* REG_BEACON_VAP12_BEACON_CSA 0x9C */
#define B0_BEACON_VAP12_BEACON_CSA_FIELD_VAP12_CSA_COUNTER_MASK                                                   0x00FF0000
//========================================
/* REG_BEACON_VAP13_BEACON_ACTIVATE 0xA0 */
#define B0_BEACON_VAP13_BEACON_ACTIVATE_FIELD_VAP13_BEACON_EN_MASK                                                0x00000001
#define B0_BEACON_VAP13_BEACON_ACTIVATE_FIELD_VAP13_BEACON_INTERVAL_MASK                                          0x00FFFF00
#define B0_BEACON_VAP13_BEACON_ACTIVATE_FIELD_VAP13_DTIM_INTERVAL_MASK                                            0xFF000000
//========================================
/* REG_BEACON_VAP13_BEACON_OFFSET 0xA4 */
#define B0_BEACON_VAP13_BEACON_OFFSET_FIELD_VAP13_BEACON_OFFSET_MASK                                              0x0000FFFF
//========================================
/* REG_BEACON_VAP13_BEACON_CSA 0xA8 */
#define B0_BEACON_VAP13_BEACON_CSA_FIELD_VAP13_CSA_COUNTER_MASK                                                   0x00FF0000
//========================================
/* REG_BEACON_VAP14_BEACON_ACTIVATE 0xAC */
#define B0_BEACON_VAP14_BEACON_ACTIVATE_FIELD_VAP14_BEACON_EN_MASK                                                0x00000001
#define B0_BEACON_VAP14_BEACON_ACTIVATE_FIELD_VAP14_BEACON_INTERVAL_MASK                                          0x00FFFF00
#define B0_BEACON_VAP14_BEACON_ACTIVATE_FIELD_VAP14_DTIM_INTERVAL_MASK                                            0xFF000000
//========================================
/* REG_BEACON_VAP14_BEACON_OFFSET 0xB0 */
#define B0_BEACON_VAP14_BEACON_OFFSET_FIELD_VAP14_BEACON_OFFSET_MASK                                              0x0000FFFF
//========================================
/* REG_BEACON_VAP14_BEACON_CSA 0xB4 */
#define B0_BEACON_VAP14_BEACON_CSA_FIELD_VAP14_CSA_COUNTER_MASK                                                   0x00FF0000
//========================================
/* REG_BEACON_VAP15_BEACON_ACTIVATE 0xB8 */
#define B0_BEACON_VAP15_BEACON_ACTIVATE_FIELD_VAP15_BEACON_EN_MASK                                                0x00000001
#define B0_BEACON_VAP15_BEACON_ACTIVATE_FIELD_VAP15_BEACON_INTERVAL_MASK                                          0x00FFFF00
#define B0_BEACON_VAP15_BEACON_ACTIVATE_FIELD_VAP15_DTIM_INTERVAL_MASK                                            0xFF000000
//========================================
/* REG_BEACON_VAP15_BEACON_OFFSET 0xBC */
#define B0_BEACON_VAP15_BEACON_OFFSET_FIELD_VAP15_BEACON_OFFSET_MASK                                              0x0000FFFF
//========================================
/* REG_BEACON_VAP15_BEACON_CSA 0xC0 */
#define B0_BEACON_VAP15_BEACON_CSA_FIELD_VAP15_CSA_COUNTER_MASK                                                   0x00FF0000
//========================================
/* REG_BEACON_BEACON_CSA_EN 0x184 */
#define B0_BEACON_BEACON_CSA_EN_FIELD_CSA_BEACON_COUNT_EN_MASK                                                    0x0000FFFF
//========================================
/* REG_BEACON_VAP_ACTIVE_STATUS 0x188 */
#define B0_BEACON_VAP_ACTIVE_STATUS_FIELD_VAP_ACTIVE_STATUS_MASK                                                  0x0000FFFF
//========================================
/* REG_BEACON_BEACON_COUNTER 0x18C */
#define B0_BEACON_BEACON_COUNTER_FIELD_BEACON_REQ_COUNTER_MASK                                                    0x000FFFFF
#define B0_BEACON_BEACON_COUNTER_FIELD_BEACON_REQ_COUNTER_CLR_MASK                                                0x80000000
//========================================
/* REG_BEACON_VERIFICATION_MODE 0x190 */
#define B0_BEACON_VERIFICATION_MODE_FIELD_VERIFICATION_MODE_MASK                                                  0x00000001
//========================================
/* REG_BEACON_BEACON_LOGGER_CFG 0x194 */
#define B0_BEACON_BEACON_LOGGER_CFG_FIELD_BEACON_TIMERS_LOGGER_PRIORITY_MASK                                      0x00000003
#define B0_BEACON_BEACON_LOGGER_CFG_FIELD_BEACON_TIMERS_LOGGER_EN_MASK                                            0x00000004
#define B0_BEACON_BEACON_LOGGER_CFG_FIELD_BEACON_LOGGER_MESSAGE_SEL_MASK                                          0x00000010
//========================================
/* REG_BEACON_BEACON_LOGGER_BUSY 0x198 */
#define B0_BEACON_BEACON_LOGGER_BUSY_FIELD_BEACON_TIMERS_LOGGER_BUSY_MASK                                         0x00000001
//========================================
/* REG_BEACON_VAP01_TSF_OFFSET 0x19C */
#define B0_BEACON_VAP01_TSF_OFFSET_FIELD_VAP0_BEACON_FINAL_OFFSET_MASK                                            0x0000FFFF
#define B0_BEACON_VAP01_TSF_OFFSET_FIELD_VAP1_BEACON_FINAL_OFFSET_MASK                                            0xFFFF0000
//========================================
/* REG_BEACON_VAP23_TSF_OFFSET 0x1A0 */
#define B0_BEACON_VAP23_TSF_OFFSET_FIELD_VAP2_BEACON_FINAL_OFFSET_MASK                                            0x0000FFFF
#define B0_BEACON_VAP23_TSF_OFFSET_FIELD_VAP3_BEACON_FINAL_OFFSET_MASK                                            0xFFFF0000
//========================================
/* REG_BEACON_VAP45_TSF_OFFSET 0x1A4 */
#define B0_BEACON_VAP45_TSF_OFFSET_FIELD_VAP4_BEACON_FINAL_OFFSET_MASK                                            0x0000FFFF
#define B0_BEACON_VAP45_TSF_OFFSET_FIELD_VAP5_BEACON_FINAL_OFFSET_MASK                                            0xFFFF0000
//========================================
/* REG_BEACON_VAP67_TSF_OFFSET 0x1A8 */
#define B0_BEACON_VAP67_TSF_OFFSET_FIELD_VAP6_BEACON_FINAL_OFFSET_MASK                                            0x0000FFFF
#define B0_BEACON_VAP67_TSF_OFFSET_FIELD_VAP7_BEACON_FINAL_OFFSET_MASK                                            0xFFFF0000
//========================================
/* REG_BEACON_VAP89_TSF_OFFSET 0x1AC */
#define B0_BEACON_VAP89_TSF_OFFSET_FIELD_VAP8_BEACON_FINAL_OFFSET_MASK                                            0x0000FFFF
#define B0_BEACON_VAP89_TSF_OFFSET_FIELD_VAP9_BEACON_FINAL_OFFSET_MASK                                            0xFFFF0000
//========================================
/* REG_BEACON_VAP1011_TSF_OFFSET 0x1B0 */
#define B0_BEACON_VAP1011_TSF_OFFSET_FIELD_VAP10_BEACON_FINAL_OFFSET_MASK                                         0x0000FFFF
#define B0_BEACON_VAP1011_TSF_OFFSET_FIELD_VAP11_BEACON_FINAL_OFFSET_MASK                                         0xFFFF0000
//========================================
/* REG_BEACON_VAP1213_TSF_OFFSET 0x1B4 */
#define B0_BEACON_VAP1213_TSF_OFFSET_FIELD_VAP12_BEACON_FINAL_OFFSET_MASK                                         0x0000FFFF
#define B0_BEACON_VAP1213_TSF_OFFSET_FIELD_VAP13_BEACON_FINAL_OFFSET_MASK                                         0xFFFF0000
//========================================
/* REG_BEACON_VAP1415_TSF_OFFSET 0x1B8 */
#define B0_BEACON_VAP1415_TSF_OFFSET_FIELD_VAP14_BEACON_FINAL_OFFSET_MASK                                         0x0000FFFF
#define B0_BEACON_VAP1415_TSF_OFFSET_FIELD_VAP15_BEACON_FINAL_OFFSET_MASK                                         0xFFFF0000
//========================================
/* REG_BEACON_MULTI_BSSID_PRIMARY_VAP 0x1DC */
#define B0_BEACON_MULTI_BSSID_PRIMARY_VAP_FIELD_MULTI_BSSID_PRIMARY_VAP_IDX_MASK                                  0x00000001
//========================================
/* REG_BEACON_MULTI_BSSID_VAPS_IN_GROUP 0x1E0 */
#define B0_BEACON_MULTI_BSSID_VAPS_IN_GROUP_FIELD_MULTI_BSSID_VAPS_IN_GROUP_EN_MASK                               0x0000FFFF
//========================================
/* REG_BEACON_VAP0_BEACON_RO_COUNTERS 0x1E4 */
#define B0_BEACON_VAP0_BEACON_RO_COUNTERS_FIELD_DTIM0_COUNTER_MASK                                                0x0000FF00
#define B0_BEACON_VAP0_BEACON_RO_COUNTERS_FIELD_VAP0_COLOR_COUNTER_MASK                                           0x00FF0000
#define B0_BEACON_VAP0_BEACON_RO_COUNTERS_FIELD_VAP0_PERSISTENCE_COUNTER_MASK                                     0xFF000000
//========================================
/* REG_BEACON_VAP1_BEACON_RO_COUNTERS 0x1E8 */
#define B0_BEACON_VAP1_BEACON_RO_COUNTERS_FIELD_DTIM1_COUNTER_MASK                                                0x0000FF00
#define B0_BEACON_VAP1_BEACON_RO_COUNTERS_FIELD_VAP1_COLOR_COUNTER_MASK                                           0x00FF0000
#define B0_BEACON_VAP1_BEACON_RO_COUNTERS_FIELD_VAP1_PERSISTENCE_COUNTER_MASK                                     0xFF000000
//========================================
/* REG_BEACON_VAP2_BEACON_RO_COUNTERS 0x1EC */
#define B0_BEACON_VAP2_BEACON_RO_COUNTERS_FIELD_DTIM2_COUNTER_MASK                                                0x0000FF00
#define B0_BEACON_VAP2_BEACON_RO_COUNTERS_FIELD_VAP2_COLOR_COUNTER_MASK                                           0x00FF0000
#define B0_BEACON_VAP2_BEACON_RO_COUNTERS_FIELD_VAP2_PERSISTENCE_COUNTER_MASK                                     0xFF000000
//========================================
/* REG_BEACON_VAP3_BEACON_RO_COUNTERS 0x1F0 */
#define B0_BEACON_VAP3_BEACON_RO_COUNTERS_FIELD_DTIM3_COUNTER_MASK                                                0x0000FF00
#define B0_BEACON_VAP3_BEACON_RO_COUNTERS_FIELD_VAP3_COLOR_COUNTER_MASK                                           0x00FF0000
#define B0_BEACON_VAP3_BEACON_RO_COUNTERS_FIELD_VAP3_PERSISTENCE_COUNTER_MASK                                     0xFF000000
//========================================
/* REG_BEACON_VAP4_BEACON_RO_COUNTERS 0x1F4 */
#define B0_BEACON_VAP4_BEACON_RO_COUNTERS_FIELD_DTIM4_COUNTER_MASK                                                0x0000FF00
#define B0_BEACON_VAP4_BEACON_RO_COUNTERS_FIELD_VAP4_COLOR_COUNTER_MASK                                           0x00FF0000
#define B0_BEACON_VAP4_BEACON_RO_COUNTERS_FIELD_VAP4_PERSISTENCE_COUNTER_MASK                                     0xFF000000
//========================================
/* REG_BEACON_VAP5_BEACON_RO_COUNTERS 0x1F8 */
#define B0_BEACON_VAP5_BEACON_RO_COUNTERS_FIELD_DTIM5_COUNTER_MASK                                                0x0000FF00
#define B0_BEACON_VAP5_BEACON_RO_COUNTERS_FIELD_VAP5_COLOR_COUNTER_MASK                                           0x00FF0000
#define B0_BEACON_VAP5_BEACON_RO_COUNTERS_FIELD_VAP5_PERSISTENCE_COUNTER_MASK                                     0xFF000000
//========================================
/* REG_BEACON_VAP6_BEACON_RO_COUNTERS 0x1FC */
#define B0_BEACON_VAP6_BEACON_RO_COUNTERS_FIELD_DTIM6_COUNTER_MASK                                                0x0000FF00
#define B0_BEACON_VAP6_BEACON_RO_COUNTERS_FIELD_VAP6_COLOR_COUNTER_MASK                                           0x00FF0000
#define B0_BEACON_VAP6_BEACON_RO_COUNTERS_FIELD_VAP6_PERSISTENCE_COUNTER_MASK                                     0xFF000000
//========================================
/* REG_BEACON_VAP7_BEACON_RO_COUNTERS 0x200 */
#define B0_BEACON_VAP7_BEACON_RO_COUNTERS_FIELD_DTIM7_COUNTER_MASK                                                0x0000FF00
#define B0_BEACON_VAP7_BEACON_RO_COUNTERS_FIELD_VAP7_COLOR_COUNTER_MASK                                           0x00FF0000
#define B0_BEACON_VAP7_BEACON_RO_COUNTERS_FIELD_VAP7_PERSISTENCE_COUNTER_MASK                                     0xFF000000
//========================================
/* REG_BEACON_VAP8_BEACON_RO_COUNTERS 0x204 */
#define B0_BEACON_VAP8_BEACON_RO_COUNTERS_FIELD_DTIM8_COUNTER_MASK                                                0x0000FF00
#define B0_BEACON_VAP8_BEACON_RO_COUNTERS_FIELD_VAP8_COLOR_COUNTER_MASK                                           0x00FF0000
#define B0_BEACON_VAP8_BEACON_RO_COUNTERS_FIELD_VAP8_PERSISTENCE_COUNTER_MASK                                     0xFF000000
//========================================
/* REG_BEACON_VAP9_BEACON_RO_COUNTERS 0x208 */
#define B0_BEACON_VAP9_BEACON_RO_COUNTERS_FIELD_DTIM9_COUNTER_MASK                                                0x0000FF00
#define B0_BEACON_VAP9_BEACON_RO_COUNTERS_FIELD_VAP9_COLOR_COUNTER_MASK                                           0x00FF0000
#define B0_BEACON_VAP9_BEACON_RO_COUNTERS_FIELD_VAP9_PERSISTENCE_COUNTER_MASK                                     0xFF000000
//========================================
/* REG_BEACON_VAP10_BEACON_RO_COUNTERS 0x20C */
#define B0_BEACON_VAP10_BEACON_RO_COUNTERS_FIELD_DTIM10_COUNTER_MASK                                              0x0000FF00
#define B0_BEACON_VAP10_BEACON_RO_COUNTERS_FIELD_VAP10_COLOR_COUNTER_MASK                                         0x00FF0000
#define B0_BEACON_VAP10_BEACON_RO_COUNTERS_FIELD_VAP10_PERSISTENCE_COUNTER_MASK                                   0xFF000000
//========================================
/* REG_BEACON_VAP11_BEACON_RO_COUNTERS 0x210 */
#define B0_BEACON_VAP11_BEACON_RO_COUNTERS_FIELD_DTIM11_COUNTER_MASK                                              0x0000FF00
#define B0_BEACON_VAP11_BEACON_RO_COUNTERS_FIELD_VAP11_COLOR_COUNTER_MASK                                         0x00FF0000
#define B0_BEACON_VAP11_BEACON_RO_COUNTERS_FIELD_VAP11_PERSISTENCE_COUNTER_MASK                                   0xFF000000
//========================================
/* REG_BEACON_VAP12_BEACON_RO_COUNTERS 0x214 */
#define B0_BEACON_VAP12_BEACON_RO_COUNTERS_FIELD_DTIM12_COUNTER_MASK                                              0x0000FF00
#define B0_BEACON_VAP12_BEACON_RO_COUNTERS_FIELD_VAP12_COLOR_COUNTER_MASK                                         0x00FF0000
#define B0_BEACON_VAP12_BEACON_RO_COUNTERS_FIELD_VAP12_PERSISTENCE_COUNTER_MASK                                   0xFF000000
//========================================
/* REG_BEACON_VAP13_BEACON_RO_COUNTERS 0x218 */
#define B0_BEACON_VAP13_BEACON_RO_COUNTERS_FIELD_DTIM13_COUNTER_MASK                                              0x0000FF00
#define B0_BEACON_VAP13_BEACON_RO_COUNTERS_FIELD_VAP13_COLOR_COUNTER_MASK                                         0x00FF0000
#define B0_BEACON_VAP13_BEACON_RO_COUNTERS_FIELD_VAP13_PERSISTENCE_COUNTER_MASK                                   0xFF000000
//========================================
/* REG_BEACON_VAP14_BEACON_RO_COUNTERS 0x21C */
#define B0_BEACON_VAP14_BEACON_RO_COUNTERS_FIELD_DTIM14_COUNTER_MASK                                              0x0000FF00
#define B0_BEACON_VAP14_BEACON_RO_COUNTERS_FIELD_VAP14_COLOR_COUNTER_MASK                                         0x00FF0000
#define B0_BEACON_VAP14_BEACON_RO_COUNTERS_FIELD_VAP14_PERSISTENCE_COUNTER_MASK                                   0xFF000000
//========================================
/* REG_BEACON_VAP15_BEACON_RO_COUNTERS 0x220 */
#define B0_BEACON_VAP15_BEACON_RO_COUNTERS_FIELD_DTIM15_COUNTER_MASK                                              0x0000FF00
#define B0_BEACON_VAP15_BEACON_RO_COUNTERS_FIELD_VAP15_COLOR_COUNTER_MASK                                         0x00FF0000
#define B0_BEACON_VAP15_BEACON_RO_COUNTERS_FIELD_VAP15_PERSISTENCE_COUNTER_MASK                                   0xFF000000
//========================================
/* REG_BEACON_BEACON_PERSISTENCE_EN 0x264 */
#define B0_BEACON_BEACON_PERSISTENCE_EN_FIELD_PERSISTENCE_BEACON_COUNT_EN_MASK                                    0x0000FFFF
//========================================
/* REG_BEACON_BEACON_COLOR_EN 0x268 */
#define B0_BEACON_BEACON_COLOR_EN_FIELD_COLOR_BEACON_COUNT_EN_MASK                                                0x0000FFFF
//========================================
/* REG_BEACON_BEACON_START_FIELD_CFG 0x26C */
#define B0_BEACON_BEACON_START_FIELD_CFG_FIELD_FIELD_UPDATE_VALUE_MASK                                            0x000000FF
#define B0_BEACON_BEACON_START_FIELD_CFG_FIELD_FIELD_UPDATE_VAP_IDX_MASK                                          0x00000F00
#define B0_BEACON_BEACON_START_FIELD_CFG_FIELD_FIELD_UPDATE_TYPE_MASK                                             0x000F0000


#endif // _BEACON_REGS_H_
