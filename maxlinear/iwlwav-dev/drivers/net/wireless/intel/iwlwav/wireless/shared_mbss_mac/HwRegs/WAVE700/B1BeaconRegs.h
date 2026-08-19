
/***********************************************************************************
File:				B1BeaconRegs.h
Module:				b1Beacon
SOC Revision:		latest
Generated at:       2022-05-04 10:57:33
Purpose:
Description:		This File was auto generated using SOC Online

************************************************************************************/
#ifndef _B1_BEACON_REGS_H_
#define _B1_BEACON_REGS_H_

/*---------------------------------------------------------------------------------
/						Registers Addresses													 
/----------------------------------------------------------------------------------*/
#include "HwMemoryMap.h"

#define B1_BEACON_BASE_ADDRESS                             MEMORY_MAP_UNIT_10015_BASE_ADDRESS
#define	REG_B1_BEACON_BEACON_LCM                   (B1_BEACON_BASE_ADDRESS + 0x0)
#define	REG_B1_BEACON_VAP0_BEACON_ACTIVATE         (B1_BEACON_BASE_ADDRESS + 0x4)
#define	REG_B1_BEACON_VAP0_BEACON_OFFSET           (B1_BEACON_BASE_ADDRESS + 0x8)
#define	REG_B1_BEACON_VAP0_BEACON_CSA              (B1_BEACON_BASE_ADDRESS + 0xC)
#define	REG_B1_BEACON_VAP1_BEACON_ACTIVATE         (B1_BEACON_BASE_ADDRESS + 0x10)
#define	REG_B1_BEACON_VAP1_BEACON_OFFSET           (B1_BEACON_BASE_ADDRESS + 0x14)
#define	REG_B1_BEACON_VAP1_BEACON_CSA              (B1_BEACON_BASE_ADDRESS + 0x18)
#define	REG_B1_BEACON_VAP2_BEACON_ACTIVATE         (B1_BEACON_BASE_ADDRESS + 0x1C)
#define	REG_B1_BEACON_VAP2_BEACON_OFFSET           (B1_BEACON_BASE_ADDRESS + 0x20)
#define	REG_B1_BEACON_VAP2_BEACON_CSA              (B1_BEACON_BASE_ADDRESS + 0x24)
#define	REG_B1_BEACON_VAP3_BEACON_ACTIVATE         (B1_BEACON_BASE_ADDRESS + 0x28)
#define	REG_B1_BEACON_VAP3_BEACON_OFFSET           (B1_BEACON_BASE_ADDRESS + 0x2C)
#define	REG_B1_BEACON_VAP3_BEACON_CSA              (B1_BEACON_BASE_ADDRESS + 0x30)
#define	REG_B1_BEACON_VAP4_BEACON_ACTIVATE         (B1_BEACON_BASE_ADDRESS + 0x34)
#define	REG_B1_BEACON_VAP4_BEACON_OFFSET           (B1_BEACON_BASE_ADDRESS + 0x38)
#define	REG_B1_BEACON_VAP4_BEACON_CSA              (B1_BEACON_BASE_ADDRESS + 0x3C)
#define	REG_B1_BEACON_VAP5_BEACON_ACTIVATE         (B1_BEACON_BASE_ADDRESS + 0x40)
#define	REG_B1_BEACON_VAP5_BEACON_OFFSET           (B1_BEACON_BASE_ADDRESS + 0x44)
#define	REG_B1_BEACON_VAP5_BEACON_CSA              (B1_BEACON_BASE_ADDRESS + 0x48)
#define	REG_B1_BEACON_VAP6_BEACON_ACTIVATE         (B1_BEACON_BASE_ADDRESS + 0x4C)
#define	REG_B1_BEACON_VAP6_BEACON_OFFSET           (B1_BEACON_BASE_ADDRESS + 0x50)
#define	REG_B1_BEACON_VAP6_BEACON_CSA              (B1_BEACON_BASE_ADDRESS + 0x54)
#define	REG_B1_BEACON_VAP7_BEACON_ACTIVATE         (B1_BEACON_BASE_ADDRESS + 0x58)
#define	REG_B1_BEACON_VAP7_BEACON_OFFSET           (B1_BEACON_BASE_ADDRESS + 0x5C)
#define	REG_B1_BEACON_VAP7_BEACON_CSA              (B1_BEACON_BASE_ADDRESS + 0x60)
#define	REG_B1_BEACON_VAP8_BEACON_ACTIVATE         (B1_BEACON_BASE_ADDRESS + 0x64)
#define	REG_B1_BEACON_VAP8_BEACON_OFFSET           (B1_BEACON_BASE_ADDRESS + 0x68)
#define	REG_B1_BEACON_VAP8_BEACON_CSA              (B1_BEACON_BASE_ADDRESS + 0x6C)
#define	REG_B1_BEACON_VAP9_BEACON_ACTIVATE         (B1_BEACON_BASE_ADDRESS + 0x70)
#define	REG_B1_BEACON_VAP9_BEACON_OFFSET           (B1_BEACON_BASE_ADDRESS + 0x74)
#define	REG_B1_BEACON_VAP9_BEACON_CSA              (B1_BEACON_BASE_ADDRESS + 0x78)
#define	REG_B1_BEACON_VAP10_BEACON_ACTIVATE        (B1_BEACON_BASE_ADDRESS + 0x7C)
#define	REG_B1_BEACON_VAP10_BEACON_OFFSET          (B1_BEACON_BASE_ADDRESS + 0x80)
#define	REG_B1_BEACON_VAP10_BEACON_CSA             (B1_BEACON_BASE_ADDRESS + 0x84)
#define	REG_B1_BEACON_VAP11_BEACON_ACTIVATE        (B1_BEACON_BASE_ADDRESS + 0x88)
#define	REG_B1_BEACON_VAP11_BEACON_OFFSET          (B1_BEACON_BASE_ADDRESS + 0x8C)
#define	REG_B1_BEACON_VAP11_BEACON_CSA             (B1_BEACON_BASE_ADDRESS + 0x90)
#define	REG_B1_BEACON_VAP12_BEACON_ACTIVATE        (B1_BEACON_BASE_ADDRESS + 0x94)
#define	REG_B1_BEACON_VAP12_BEACON_OFFSET          (B1_BEACON_BASE_ADDRESS + 0x98)
#define	REG_B1_BEACON_VAP12_BEACON_CSA             (B1_BEACON_BASE_ADDRESS + 0x9C)
#define	REG_B1_BEACON_VAP13_BEACON_ACTIVATE        (B1_BEACON_BASE_ADDRESS + 0xA0)
#define	REG_B1_BEACON_VAP13_BEACON_OFFSET          (B1_BEACON_BASE_ADDRESS + 0xA4)
#define	REG_B1_BEACON_VAP13_BEACON_CSA             (B1_BEACON_BASE_ADDRESS + 0xA8)
#define	REG_B1_BEACON_VAP14_BEACON_ACTIVATE        (B1_BEACON_BASE_ADDRESS + 0xAC)
#define	REG_B1_BEACON_VAP14_BEACON_OFFSET          (B1_BEACON_BASE_ADDRESS + 0xB0)
#define	REG_B1_BEACON_VAP14_BEACON_CSA             (B1_BEACON_BASE_ADDRESS + 0xB4)
#define	REG_B1_BEACON_VAP15_BEACON_ACTIVATE        (B1_BEACON_BASE_ADDRESS + 0xB8)
#define	REG_B1_BEACON_VAP15_BEACON_OFFSET          (B1_BEACON_BASE_ADDRESS + 0xBC)
#define	REG_B1_BEACON_VAP15_BEACON_CSA             (B1_BEACON_BASE_ADDRESS + 0xC0)
#define	REG_B1_BEACON_VAP16_BEACON_ACTIVATE        (B1_BEACON_BASE_ADDRESS + 0xC4)
#define	REG_B1_BEACON_VAP16_BEACON_OFFSET          (B1_BEACON_BASE_ADDRESS + 0xC8)
#define	REG_B1_BEACON_VAP16_BEACON_CSA             (B1_BEACON_BASE_ADDRESS + 0xCC)
#define	REG_B1_BEACON_VAP17_BEACON_ACTIVATE        (B1_BEACON_BASE_ADDRESS + 0xD0)
#define	REG_B1_BEACON_VAP17_BEACON_OFFSET          (B1_BEACON_BASE_ADDRESS + 0xD4)
#define	REG_B1_BEACON_VAP17_BEACON_CSA             (B1_BEACON_BASE_ADDRESS + 0xD8)
#define	REG_B1_BEACON_VAP18_BEACON_ACTIVATE        (B1_BEACON_BASE_ADDRESS + 0xDC)
#define	REG_B1_BEACON_VAP18_BEACON_OFFSET          (B1_BEACON_BASE_ADDRESS + 0xE0)
#define	REG_B1_BEACON_VAP18_BEACON_CSA             (B1_BEACON_BASE_ADDRESS + 0xE4)
#define	REG_B1_BEACON_VAP19_BEACON_ACTIVATE        (B1_BEACON_BASE_ADDRESS + 0xE8)
#define	REG_B1_BEACON_VAP19_BEACON_OFFSET          (B1_BEACON_BASE_ADDRESS + 0xEC)
#define	REG_B1_BEACON_VAP19_BEACON_CSA             (B1_BEACON_BASE_ADDRESS + 0xF0)
#define	REG_B1_BEACON_VAP20_BEACON_ACTIVATE        (B1_BEACON_BASE_ADDRESS + 0xF4)
#define	REG_B1_BEACON_VAP20_BEACON_OFFSET          (B1_BEACON_BASE_ADDRESS + 0xF8)
#define	REG_B1_BEACON_VAP20_BEACON_CSA             (B1_BEACON_BASE_ADDRESS + 0xFC)
#define	REG_B1_BEACON_VAP21_BEACON_ACTIVATE        (B1_BEACON_BASE_ADDRESS + 0x100)
#define	REG_B1_BEACON_VAP21_BEACON_OFFSET          (B1_BEACON_BASE_ADDRESS + 0x104)
#define	REG_B1_BEACON_VAP21_BEACON_CSA             (B1_BEACON_BASE_ADDRESS + 0x108)
#define	REG_B1_BEACON_VAP22_BEACON_ACTIVATE        (B1_BEACON_BASE_ADDRESS + 0x10C)
#define	REG_B1_BEACON_VAP22_BEACON_OFFSET          (B1_BEACON_BASE_ADDRESS + 0x110)
#define	REG_B1_BEACON_VAP22_BEACON_CSA             (B1_BEACON_BASE_ADDRESS + 0x114)
#define	REG_B1_BEACON_VAP23_BEACON_ACTIVATE        (B1_BEACON_BASE_ADDRESS + 0x118)
#define	REG_B1_BEACON_VAP23_BEACON_OFFSET          (B1_BEACON_BASE_ADDRESS + 0x11C)
#define	REG_B1_BEACON_VAP23_BEACON_CSA             (B1_BEACON_BASE_ADDRESS + 0x120)
#define	REG_B1_BEACON_VAP24_BEACON_ACTIVATE        (B1_BEACON_BASE_ADDRESS + 0x124)
#define	REG_B1_BEACON_VAP24_BEACON_OFFSET          (B1_BEACON_BASE_ADDRESS + 0x128)
#define	REG_B1_BEACON_VAP24_BEACON_CSA             (B1_BEACON_BASE_ADDRESS + 0x12C)
#define	REG_B1_BEACON_VAP25_BEACON_ACTIVATE        (B1_BEACON_BASE_ADDRESS + 0x130)
#define	REG_B1_BEACON_VAP25_BEACON_OFFSET          (B1_BEACON_BASE_ADDRESS + 0x134)
#define	REG_B1_BEACON_VAP25_BEACON_CSA             (B1_BEACON_BASE_ADDRESS + 0x138)
#define	REG_B1_BEACON_VAP26_BEACON_ACTIVATE        (B1_BEACON_BASE_ADDRESS + 0x13C)
#define	REG_B1_BEACON_VAP26_BEACON_OFFSET          (B1_BEACON_BASE_ADDRESS + 0x140)
#define	REG_B1_BEACON_VAP26_BEACON_CSA             (B1_BEACON_BASE_ADDRESS + 0x144)
#define	REG_B1_BEACON_VAP27_BEACON_ACTIVATE        (B1_BEACON_BASE_ADDRESS + 0x148)
#define	REG_B1_BEACON_VAP27_BEACON_OFFSET          (B1_BEACON_BASE_ADDRESS + 0x14C)
#define	REG_B1_BEACON_VAP27_BEACON_CSA             (B1_BEACON_BASE_ADDRESS + 0x150)
#define	REG_B1_BEACON_VAP28_BEACON_ACTIVATE        (B1_BEACON_BASE_ADDRESS + 0x154)
#define	REG_B1_BEACON_VAP28_BEACON_OFFSET          (B1_BEACON_BASE_ADDRESS + 0x158)
#define	REG_B1_BEACON_VAP28_BEACON_CSA             (B1_BEACON_BASE_ADDRESS + 0x15C)
#define	REG_B1_BEACON_VAP29_BEACON_ACTIVATE        (B1_BEACON_BASE_ADDRESS + 0x160)
#define	REG_B1_BEACON_VAP29_BEACON_OFFSET          (B1_BEACON_BASE_ADDRESS + 0x164)
#define	REG_B1_BEACON_VAP29_BEACON_CSA             (B1_BEACON_BASE_ADDRESS + 0x168)
#define	REG_B1_BEACON_VAP30_BEACON_ACTIVATE        (B1_BEACON_BASE_ADDRESS + 0x16C)
#define	REG_B1_BEACON_VAP30_BEACON_OFFSET          (B1_BEACON_BASE_ADDRESS + 0x170)
#define	REG_B1_BEACON_VAP30_BEACON_CSA             (B1_BEACON_BASE_ADDRESS + 0x174)
#define	REG_B1_BEACON_VAP31_BEACON_ACTIVATE        (B1_BEACON_BASE_ADDRESS + 0x178)
#define	REG_B1_BEACON_VAP31_BEACON_OFFSET          (B1_BEACON_BASE_ADDRESS + 0x17C)
#define	REG_B1_BEACON_VAP31_BEACON_CSA             (B1_BEACON_BASE_ADDRESS + 0x180)
#define	REG_B1_BEACON_BEACON_CSA_EN                (B1_BEACON_BASE_ADDRESS + 0x184)
#define	REG_B1_BEACON_VAP_ACTIVE_STATUS            (B1_BEACON_BASE_ADDRESS + 0x188)
#define	REG_B1_BEACON_BEACON_COUNTER               (B1_BEACON_BASE_ADDRESS + 0x18C)
#define	REG_B1_BEACON_VERIFICATION_MODE            (B1_BEACON_BASE_ADDRESS + 0x190)
#define	REG_B1_BEACON_BEACON_LOGGER_CFG            (B1_BEACON_BASE_ADDRESS + 0x194)
#define	REG_B1_BEACON_BEACON_LOGGER_BUSY           (B1_BEACON_BASE_ADDRESS + 0x198)
#define	REG_B1_BEACON_VAP01_TSF_OFFSET             (B1_BEACON_BASE_ADDRESS + 0x19C)
#define	REG_B1_BEACON_VAP23_TSF_OFFSET             (B1_BEACON_BASE_ADDRESS + 0x1A0)
#define	REG_B1_BEACON_VAP45_TSF_OFFSET             (B1_BEACON_BASE_ADDRESS + 0x1A4)
#define	REG_B1_BEACON_VAP67_TSF_OFFSET             (B1_BEACON_BASE_ADDRESS + 0x1A8)
#define	REG_B1_BEACON_VAP89_TSF_OFFSET             (B1_BEACON_BASE_ADDRESS + 0x1AC)
#define	REG_B1_BEACON_VAP1011_TSF_OFFSET           (B1_BEACON_BASE_ADDRESS + 0x1B0)
#define	REG_B1_BEACON_VAP1213_TSF_OFFSET           (B1_BEACON_BASE_ADDRESS + 0x1B4)
#define	REG_B1_BEACON_VAP1415_TSF_OFFSET           (B1_BEACON_BASE_ADDRESS + 0x1B8)
#define	REG_B1_BEACON_VAP1617_TSF_OFFSET           (B1_BEACON_BASE_ADDRESS + 0x1BC)
#define	REG_B1_BEACON_VAP1819_TSF_OFFSET           (B1_BEACON_BASE_ADDRESS + 0x1C0)
#define	REG_B1_BEACON_VAP2021_TSF_OFFSET           (B1_BEACON_BASE_ADDRESS + 0x1C4)
#define	REG_B1_BEACON_VAP2223_TSF_OFFSET           (B1_BEACON_BASE_ADDRESS + 0x1C8)
#define	REG_B1_BEACON_VAP2425_TSF_OFFSET           (B1_BEACON_BASE_ADDRESS + 0x1CC)
#define	REG_B1_BEACON_VAP2627_TSF_OFFSET           (B1_BEACON_BASE_ADDRESS + 0x1D0)
#define	REG_B1_BEACON_VAP2829_TSF_OFFSET           (B1_BEACON_BASE_ADDRESS + 0x1D4)
#define	REG_B1_BEACON_VAP3031_TSF_OFFSET           (B1_BEACON_BASE_ADDRESS + 0x1D8)
#define	REG_B1_BEACON_MULTI_BSSID_PRIMARY_VAP      (B1_BEACON_BASE_ADDRESS + 0x1DC)
#define	REG_B1_BEACON_MULTI_BSSID_VAPS_IN_GROUP    (B1_BEACON_BASE_ADDRESS + 0x1E0)
#define	REG_B1_BEACON_VAP0_BEACON_RO_COUNTERS      (B1_BEACON_BASE_ADDRESS + 0x1E4)
#define	REG_B1_BEACON_VAP1_BEACON_RO_COUNTERS      (B1_BEACON_BASE_ADDRESS + 0x1E8)
#define	REG_B1_BEACON_VAP2_BEACON_RO_COUNTERS      (B1_BEACON_BASE_ADDRESS + 0x1EC)
#define	REG_B1_BEACON_VAP3_BEACON_RO_COUNTERS      (B1_BEACON_BASE_ADDRESS + 0x1F0)
#define	REG_B1_BEACON_VAP4_BEACON_RO_COUNTERS      (B1_BEACON_BASE_ADDRESS + 0x1F4)
#define	REG_B1_BEACON_VAP5_BEACON_RO_COUNTERS      (B1_BEACON_BASE_ADDRESS + 0x1F8)
#define	REG_B1_BEACON_VAP6_BEACON_RO_COUNTERS      (B1_BEACON_BASE_ADDRESS + 0x1FC)
#define	REG_B1_BEACON_VAP7_BEACON_RO_COUNTERS      (B1_BEACON_BASE_ADDRESS + 0x200)
#define	REG_B1_BEACON_VAP8_BEACON_RO_COUNTERS      (B1_BEACON_BASE_ADDRESS + 0x204)
#define	REG_B1_BEACON_VAP9_BEACON_RO_COUNTERS      (B1_BEACON_BASE_ADDRESS + 0x208)
#define	REG_B1_BEACON_VAP10_BEACON_RO_COUNTERS     (B1_BEACON_BASE_ADDRESS + 0x20C)
#define	REG_B1_BEACON_VAP11_BEACON_RO_COUNTERS     (B1_BEACON_BASE_ADDRESS + 0x210)
#define	REG_B1_BEACON_VAP12_BEACON_RO_COUNTERS     (B1_BEACON_BASE_ADDRESS + 0x214)
#define	REG_B1_BEACON_VAP13_BEACON_RO_COUNTERS     (B1_BEACON_BASE_ADDRESS + 0x218)
#define	REG_B1_BEACON_VAP14_BEACON_RO_COUNTERS     (B1_BEACON_BASE_ADDRESS + 0x21C)
#define	REG_B1_BEACON_VAP15_BEACON_RO_COUNTERS     (B1_BEACON_BASE_ADDRESS + 0x220)
#define	REG_B1_BEACON_VAP16_BEACON_RO_COUNTERS     (B1_BEACON_BASE_ADDRESS + 0x224)
#define	REG_B1_BEACON_VAP17_BEACON_RO_COUNTERS     (B1_BEACON_BASE_ADDRESS + 0x228)
#define	REG_B1_BEACON_VAP18_BEACON_RO_COUNTERS     (B1_BEACON_BASE_ADDRESS + 0x22C)
#define	REG_B1_BEACON_VAP19_BEACON_RO_COUNTERS     (B1_BEACON_BASE_ADDRESS + 0x230)
#define	REG_B1_BEACON_VAP20_BEACON_RO_COUNTERS     (B1_BEACON_BASE_ADDRESS + 0x234)
#define	REG_B1_BEACON_VAP21_BEACON_RO_COUNTERS     (B1_BEACON_BASE_ADDRESS + 0x238)
#define	REG_B1_BEACON_VAP22_BEACON_RO_COUNTERS     (B1_BEACON_BASE_ADDRESS + 0x23C)
#define	REG_B1_BEACON_VAP23_BEACON_RO_COUNTERS     (B1_BEACON_BASE_ADDRESS + 0x240)
#define	REG_B1_BEACON_VAP24_BEACON_RO_COUNTERS     (B1_BEACON_BASE_ADDRESS + 0x244)
#define	REG_B1_BEACON_VAP25_BEACON_RO_COUNTERS     (B1_BEACON_BASE_ADDRESS + 0x248)
#define	REG_B1_BEACON_VAP26_BEACON_RO_COUNTERS     (B1_BEACON_BASE_ADDRESS + 0x24C)
#define	REG_B1_BEACON_VAP27_BEACON_RO_COUNTERS     (B1_BEACON_BASE_ADDRESS + 0x250)
#define	REG_B1_BEACON_VAP28_BEACON_RO_COUNTERS     (B1_BEACON_BASE_ADDRESS + 0x254)
#define	REG_B1_BEACON_VAP29_BEACON_RO_COUNTERS     (B1_BEACON_BASE_ADDRESS + 0x258)
#define	REG_B1_BEACON_VAP30_BEACON_RO_COUNTERS     (B1_BEACON_BASE_ADDRESS + 0x25C)
#define	REG_B1_BEACON_VAP31_BEACON_RO_COUNTERS     (B1_BEACON_BASE_ADDRESS + 0x260)
#define	REG_B1_BEACON_BEACON_PERSISTENCE_EN        (B1_BEACON_BASE_ADDRESS + 0x264)
#define	REG_B1_BEACON_BEACON_COLOR_EN              (B1_BEACON_BASE_ADDRESS + 0x268)
#define	REG_B1_BEACON_BEACON_START_FIELD_CFG       (B1_BEACON_BASE_ADDRESS + 0x26C)
/*---------------------------------------------------------------------------------
/						Data Type Definition										
/----------------------------------------------------------------------------------*/
/*REG_B1_BEACON_BEACON_LCM 0x0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 lcm : 16; //Greatest common division, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB1BeaconBeaconLcm_u;

/*REG_B1_BEACON_VAP0_BEACON_ACTIVATE 0x4 */
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
} RegB1BeaconVap0BeaconActivate_u;

/*REG_B1_BEACON_VAP0_BEACON_OFFSET 0x8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap0BeaconOffset : 16; //Beacon offset relative to virtual zero point, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB1BeaconVap0BeaconOffset_u;

/*REG_B1_BEACON_VAP0_BEACON_CSA 0xC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 16;
		uint32 vap0CsaCounter : 8; //Current Channel Switch Announcement count, reset value: 0x0, access type: RO
		uint32 reserved1 : 8;
	} bitFields;
} RegB1BeaconVap0BeaconCsa_u;

/*REG_B1_BEACON_VAP1_BEACON_ACTIVATE 0x10 */
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
} RegB1BeaconVap1BeaconActivate_u;

/*REG_B1_BEACON_VAP1_BEACON_OFFSET 0x14 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap1BeaconOffset : 16; //Beacon offset relative to virtual zero point, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB1BeaconVap1BeaconOffset_u;

/*REG_B1_BEACON_VAP1_BEACON_CSA 0x18 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 16;
		uint32 vap1CsaCounter : 8; //Current Channel Switch Announcement count, reset value: 0x0, access type: RO
		uint32 reserved1 : 8;
	} bitFields;
} RegB1BeaconVap1BeaconCsa_u;

/*REG_B1_BEACON_VAP2_BEACON_ACTIVATE 0x1C */
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
} RegB1BeaconVap2BeaconActivate_u;

/*REG_B1_BEACON_VAP2_BEACON_OFFSET 0x20 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap2BeaconOffset : 16; //Beacon offset relative to virtual zero point, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB1BeaconVap2BeaconOffset_u;

/*REG_B1_BEACON_VAP2_BEACON_CSA 0x24 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 16;
		uint32 vap2CsaCounter : 8; //Current Channel Switch Announcement count, reset value: 0x0, access type: RO
		uint32 reserved1 : 8;
	} bitFields;
} RegB1BeaconVap2BeaconCsa_u;

/*REG_B1_BEACON_VAP3_BEACON_ACTIVATE 0x28 */
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
} RegB1BeaconVap3BeaconActivate_u;

/*REG_B1_BEACON_VAP3_BEACON_OFFSET 0x2C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap3BeaconOffset : 16; //Beacon offset relative to virtual zero point, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB1BeaconVap3BeaconOffset_u;

/*REG_B1_BEACON_VAP3_BEACON_CSA 0x30 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 16;
		uint32 vap3CsaCounter : 8; //Current Channel Switch Announcement count, reset value: 0x0, access type: RO
		uint32 reserved1 : 8;
	} bitFields;
} RegB1BeaconVap3BeaconCsa_u;

/*REG_B1_BEACON_VAP4_BEACON_ACTIVATE 0x34 */
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
} RegB1BeaconVap4BeaconActivate_u;

/*REG_B1_BEACON_VAP4_BEACON_OFFSET 0x38 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap4BeaconOffset : 16; //Beacon offset relative to virtual zero point, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB1BeaconVap4BeaconOffset_u;

/*REG_B1_BEACON_VAP4_BEACON_CSA 0x3C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 16;
		uint32 vap4CsaCounter : 8; //Current Channel Switch Announcement count, reset value: 0x0, access type: RO
		uint32 reserved1 : 8;
	} bitFields;
} RegB1BeaconVap4BeaconCsa_u;

/*REG_B1_BEACON_VAP5_BEACON_ACTIVATE 0x40 */
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
} RegB1BeaconVap5BeaconActivate_u;

/*REG_B1_BEACON_VAP5_BEACON_OFFSET 0x44 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap5BeaconOffset : 16; //Beacon offset relative to virtual zero point, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB1BeaconVap5BeaconOffset_u;

/*REG_B1_BEACON_VAP5_BEACON_CSA 0x48 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 16;
		uint32 vap5CsaCounter : 8; //Current Channel Switch Announcement count, reset value: 0x0, access type: RO
		uint32 reserved1 : 8;
	} bitFields;
} RegB1BeaconVap5BeaconCsa_u;

/*REG_B1_BEACON_VAP6_BEACON_ACTIVATE 0x4C */
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
} RegB1BeaconVap6BeaconActivate_u;

/*REG_B1_BEACON_VAP6_BEACON_OFFSET 0x50 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap6BeaconOffset : 16; //Beacon offset relative to virtual zero point, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB1BeaconVap6BeaconOffset_u;

/*REG_B1_BEACON_VAP6_BEACON_CSA 0x54 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 16;
		uint32 vap6CsaCounter : 8; //Current Channel Switch Announcement count, reset value: 0x0, access type: RO
		uint32 reserved1 : 8;
	} bitFields;
} RegB1BeaconVap6BeaconCsa_u;

/*REG_B1_BEACON_VAP7_BEACON_ACTIVATE 0x58 */
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
} RegB1BeaconVap7BeaconActivate_u;

/*REG_B1_BEACON_VAP7_BEACON_OFFSET 0x5C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap7BeaconOffset : 16; //Beacon offset relative to virtual zero point, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB1BeaconVap7BeaconOffset_u;

/*REG_B1_BEACON_VAP7_BEACON_CSA 0x60 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 16;
		uint32 vap7CsaCounter : 8; //Current Channel Switch Announcement count, reset value: 0x0, access type: RO
		uint32 reserved1 : 8;
	} bitFields;
} RegB1BeaconVap7BeaconCsa_u;

/*REG_B1_BEACON_VAP8_BEACON_ACTIVATE 0x64 */
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
} RegB1BeaconVap8BeaconActivate_u;

/*REG_B1_BEACON_VAP8_BEACON_OFFSET 0x68 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap8BeaconOffset : 16; //Beacon offset relative to virtual zero point, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB1BeaconVap8BeaconOffset_u;

/*REG_B1_BEACON_VAP8_BEACON_CSA 0x6C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 16;
		uint32 vap8CsaCounter : 8; //Current Channel Switch Announcement count, reset value: 0x0, access type: RO
		uint32 reserved1 : 8;
	} bitFields;
} RegB1BeaconVap8BeaconCsa_u;

/*REG_B1_BEACON_VAP9_BEACON_ACTIVATE 0x70 */
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
} RegB1BeaconVap9BeaconActivate_u;

/*REG_B1_BEACON_VAP9_BEACON_OFFSET 0x74 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap9BeaconOffset : 16; //Beacon offset relative to virtual zero point, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB1BeaconVap9BeaconOffset_u;

/*REG_B1_BEACON_VAP9_BEACON_CSA 0x78 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 16;
		uint32 vap9CsaCounter : 8; //Current Channel Switch Announcement count, reset value: 0x0, access type: RO
		uint32 reserved1 : 8;
	} bitFields;
} RegB1BeaconVap9BeaconCsa_u;

/*REG_B1_BEACON_VAP10_BEACON_ACTIVATE 0x7C */
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
} RegB1BeaconVap10BeaconActivate_u;

/*REG_B1_BEACON_VAP10_BEACON_OFFSET 0x80 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap10BeaconOffset : 16; //Beacon offset relative to virtual zero point, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB1BeaconVap10BeaconOffset_u;

/*REG_B1_BEACON_VAP10_BEACON_CSA 0x84 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 16;
		uint32 vap10CsaCounter : 8; //Current Channel Switch Announcement count, reset value: 0x0, access type: RO
		uint32 reserved1 : 8;
	} bitFields;
} RegB1BeaconVap10BeaconCsa_u;

/*REG_B1_BEACON_VAP11_BEACON_ACTIVATE 0x88 */
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
} RegB1BeaconVap11BeaconActivate_u;

/*REG_B1_BEACON_VAP11_BEACON_OFFSET 0x8C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap11BeaconOffset : 16; //Beacon offset relative to virtual zero point, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB1BeaconVap11BeaconOffset_u;

/*REG_B1_BEACON_VAP11_BEACON_CSA 0x90 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 16;
		uint32 vap11CsaCounter : 8; //Current Channel Switch Announcement count, reset value: 0x0, access type: RO
		uint32 reserved1 : 8;
	} bitFields;
} RegB1BeaconVap11BeaconCsa_u;

/*REG_B1_BEACON_VAP12_BEACON_ACTIVATE 0x94 */
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
} RegB1BeaconVap12BeaconActivate_u;

/*REG_B1_BEACON_VAP12_BEACON_OFFSET 0x98 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap12BeaconOffset : 16; //Beacon offset relative to virtual zero point, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB1BeaconVap12BeaconOffset_u;

/*REG_B1_BEACON_VAP12_BEACON_CSA 0x9C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 16;
		uint32 vap12CsaCounter : 8; //Current Channel Switch Announcement count, reset value: 0x0, access type: RO
		uint32 reserved1 : 8;
	} bitFields;
} RegB1BeaconVap12BeaconCsa_u;

/*REG_B1_BEACON_VAP13_BEACON_ACTIVATE 0xA0 */
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
} RegB1BeaconVap13BeaconActivate_u;

/*REG_B1_BEACON_VAP13_BEACON_OFFSET 0xA4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap13BeaconOffset : 16; //Beacon offset relative to virtual zero point, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB1BeaconVap13BeaconOffset_u;

/*REG_B1_BEACON_VAP13_BEACON_CSA 0xA8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 16;
		uint32 vap13CsaCounter : 8; //Current Channel Switch Announcement count, reset value: 0x0, access type: RO
		uint32 reserved1 : 8;
	} bitFields;
} RegB1BeaconVap13BeaconCsa_u;

/*REG_B1_BEACON_VAP14_BEACON_ACTIVATE 0xAC */
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
} RegB1BeaconVap14BeaconActivate_u;

/*REG_B1_BEACON_VAP14_BEACON_OFFSET 0xB0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap14BeaconOffset : 16; //Beacon offset relative to virtual zero point, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB1BeaconVap14BeaconOffset_u;

/*REG_B1_BEACON_VAP14_BEACON_CSA 0xB4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 16;
		uint32 vap14CsaCounter : 8; //Current Channel Switch Announcement count, reset value: 0x0, access type: RO
		uint32 reserved1 : 8;
	} bitFields;
} RegB1BeaconVap14BeaconCsa_u;

/*REG_B1_BEACON_VAP15_BEACON_ACTIVATE 0xB8 */
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
} RegB1BeaconVap15BeaconActivate_u;

/*REG_B1_BEACON_VAP15_BEACON_OFFSET 0xBC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap15BeaconOffset : 16; //Beacon offset relative to virtual zero point, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB1BeaconVap15BeaconOffset_u;

/*REG_B1_BEACON_VAP15_BEACON_CSA 0xC0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 16;
		uint32 vap15CsaCounter : 8; //Current Channel Switch Announcement count, reset value: 0x0, access type: RO
		uint32 reserved1 : 8;
	} bitFields;
} RegB1BeaconVap15BeaconCsa_u;

/*REG_B1_BEACON_VAP16_BEACON_ACTIVATE 0xC4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap16BeaconEn : 1; //Beacon enable bit, reset value: 0x0, access type: RW
		uint32 reserved0 : 7;
		uint32 vap16BeaconInterval : 16; //Beacon interval in TUs , reset value: 0x0, access type: RW
		uint32 vap16DtimInterval : 8; //DTIM interval in TIM units, reset value: 0x0, access type: RW
	} bitFields;
} RegB1BeaconVap16BeaconActivate_u;

/*REG_B1_BEACON_VAP16_BEACON_OFFSET 0xC8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap16BeaconOffset : 16; //Beacon offset relative to virtual zero point, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB1BeaconVap16BeaconOffset_u;

/*REG_B1_BEACON_VAP16_BEACON_CSA 0xCC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 16;
		uint32 vap16CsaCounter : 8; //Current Channel Switch Announcement count, reset value: 0x0, access type: RO
		uint32 reserved1 : 8;
	} bitFields;
} RegB1BeaconVap16BeaconCsa_u;

/*REG_B1_BEACON_VAP17_BEACON_ACTIVATE 0xD0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap17BeaconEn : 1; //Beacon enable bit, reset value: 0x0, access type: RW
		uint32 reserved0 : 7;
		uint32 vap17BeaconInterval : 16; //Beacon interval in TUs , reset value: 0x0, access type: RW
		uint32 vap17DtimInterval : 8; //DTIM interval in TIM units, reset value: 0x0, access type: RW
	} bitFields;
} RegB1BeaconVap17BeaconActivate_u;

/*REG_B1_BEACON_VAP17_BEACON_OFFSET 0xD4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap17BeaconOffset : 16; //Beacon offset relative to virtual zero point, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB1BeaconVap17BeaconOffset_u;

/*REG_B1_BEACON_VAP17_BEACON_CSA 0xD8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 16;
		uint32 vap17CsaCounter : 8; //Current Channel Switch Announcement count, reset value: 0x0, access type: RO
		uint32 reserved1 : 8;
	} bitFields;
} RegB1BeaconVap17BeaconCsa_u;

/*REG_B1_BEACON_VAP18_BEACON_ACTIVATE 0xDC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap18BeaconEn : 1; //Beacon enable bit, reset value: 0x0, access type: RW
		uint32 reserved0 : 7;
		uint32 vap18BeaconInterval : 16; //Beacon interval in TUs , reset value: 0x0, access type: RW
		uint32 vap18DtimInterval : 8; //DTIM interval in TIM units, reset value: 0x0, access type: RW
	} bitFields;
} RegB1BeaconVap18BeaconActivate_u;

/*REG_B1_BEACON_VAP18_BEACON_OFFSET 0xE0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap18BeaconOffset : 16; //Beacon offset relative to virtual zero point, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB1BeaconVap18BeaconOffset_u;

/*REG_B1_BEACON_VAP18_BEACON_CSA 0xE4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 16;
		uint32 vap18CsaCounter : 8; //Current Channel Switch Announcement count, reset value: 0x0, access type: RO
		uint32 reserved1 : 8;
	} bitFields;
} RegB1BeaconVap18BeaconCsa_u;

/*REG_B1_BEACON_VAP19_BEACON_ACTIVATE 0xE8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap19BeaconEn : 1; //Beacon enable bit, reset value: 0x0, access type: RW
		uint32 reserved0 : 7;
		uint32 vap19BeaconInterval : 16; //Beacon interval in TUs , reset value: 0x0, access type: RW
		uint32 vap19DtimInterval : 8; //DTIM interval in TIM units, reset value: 0x0, access type: RW
	} bitFields;
} RegB1BeaconVap19BeaconActivate_u;

/*REG_B1_BEACON_VAP19_BEACON_OFFSET 0xEC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap19BeaconOffset : 16; //Beacon offset relative to virtual zero point, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB1BeaconVap19BeaconOffset_u;

/*REG_B1_BEACON_VAP19_BEACON_CSA 0xF0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 16;
		uint32 vap19CsaCounter : 8; //Current Channel Switch Announcement count, reset value: 0x0, access type: RO
		uint32 reserved1 : 8;
	} bitFields;
} RegB1BeaconVap19BeaconCsa_u;

/*REG_B1_BEACON_VAP20_BEACON_ACTIVATE 0xF4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap20BeaconEn : 1; //Beacon enable bit, reset value: 0x0, access type: RW
		uint32 reserved0 : 7;
		uint32 vap20BeaconInterval : 16; //Beacon interval in TUs , reset value: 0x0, access type: RW
		uint32 vap20DtimInterval : 8; //DTIM interval in TIM units, reset value: 0x0, access type: RW
	} bitFields;
} RegB1BeaconVap20BeaconActivate_u;

/*REG_B1_BEACON_VAP20_BEACON_OFFSET 0xF8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap20BeaconOffset : 16; //Beacon offset relative to virtual zero point, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB1BeaconVap20BeaconOffset_u;

/*REG_B1_BEACON_VAP20_BEACON_CSA 0xFC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 16;
		uint32 vap20CsaCounter : 8; //Current Channel Switch Announcement count, reset value: 0x0, access type: RO
		uint32 reserved1 : 8;
	} bitFields;
} RegB1BeaconVap20BeaconCsa_u;

/*REG_B1_BEACON_VAP21_BEACON_ACTIVATE 0x100 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap21BeaconEn : 1; //Beacon enable bit, reset value: 0x0, access type: RW
		uint32 reserved0 : 7;
		uint32 vap21BeaconInterval : 16; //Beacon interval in TUs , reset value: 0x0, access type: RW
		uint32 vap21DtimInterval : 8; //DTIM interval in TIM units, reset value: 0x0, access type: RW
	} bitFields;
} RegB1BeaconVap21BeaconActivate_u;

/*REG_B1_BEACON_VAP21_BEACON_OFFSET 0x104 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap21BeaconOffset : 16; //Beacon offset relative to virtual zero point, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB1BeaconVap21BeaconOffset_u;

/*REG_B1_BEACON_VAP21_BEACON_CSA 0x108 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 16;
		uint32 vap21CsaCounter : 8; //Current Channel Switch Announcement count, reset value: 0x0, access type: RO
		uint32 reserved1 : 8;
	} bitFields;
} RegB1BeaconVap21BeaconCsa_u;

/*REG_B1_BEACON_VAP22_BEACON_ACTIVATE 0x10C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap22BeaconEn : 1; //Beacon enable bit, reset value: 0x0, access type: RW
		uint32 reserved0 : 7;
		uint32 vap22BeaconInterval : 16; //Beacon interval in TUs , reset value: 0x0, access type: RW
		uint32 vap22DtimInterval : 8; //DTIM interval in TIM units, reset value: 0x0, access type: RW
	} bitFields;
} RegB1BeaconVap22BeaconActivate_u;

/*REG_B1_BEACON_VAP22_BEACON_OFFSET 0x110 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap22BeaconOffset : 16; //Beacon offset relative to virtual zero point, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB1BeaconVap22BeaconOffset_u;

/*REG_B1_BEACON_VAP22_BEACON_CSA 0x114 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 16;
		uint32 vap22CsaCounter : 8; //Current Channel Switch Announcement count, reset value: 0x0, access type: RO
		uint32 reserved1 : 8;
	} bitFields;
} RegB1BeaconVap22BeaconCsa_u;

/*REG_B1_BEACON_VAP23_BEACON_ACTIVATE 0x118 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap23BeaconEn : 1; //Beacon enable bit, reset value: 0x0, access type: RW
		uint32 reserved0 : 7;
		uint32 vap23BeaconInterval : 16; //Beacon interval in TUs , reset value: 0x0, access type: RW
		uint32 vap23DtimInterval : 8; //DTIM interval in TIM units, reset value: 0x0, access type: RW
	} bitFields;
} RegB1BeaconVap23BeaconActivate_u;

/*REG_B1_BEACON_VAP23_BEACON_OFFSET 0x11C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap23BeaconOffset : 16; //Beacon offset relative to virtual zero point, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB1BeaconVap23BeaconOffset_u;

/*REG_B1_BEACON_VAP23_BEACON_CSA 0x120 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 16;
		uint32 vap23CsaCounter : 8; //Current Channel Switch Announcement count, reset value: 0x0, access type: RO
		uint32 reserved1 : 8;
	} bitFields;
} RegB1BeaconVap23BeaconCsa_u;

/*REG_B1_BEACON_VAP24_BEACON_ACTIVATE 0x124 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap24BeaconEn : 1; //Beacon enable bit, reset value: 0x0, access type: RW
		uint32 reserved0 : 7;
		uint32 vap24BeaconInterval : 16; //Beacon interval in TUs , reset value: 0x0, access type: RW
		uint32 vap24DtimInterval : 8; //DTIM interval in TIM units, reset value: 0x0, access type: RW
	} bitFields;
} RegB1BeaconVap24BeaconActivate_u;

/*REG_B1_BEACON_VAP24_BEACON_OFFSET 0x128 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap24BeaconOffset : 16; //Beacon offset relative to virtual zero point, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB1BeaconVap24BeaconOffset_u;

/*REG_B1_BEACON_VAP24_BEACON_CSA 0x12C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 16;
		uint32 vap24CsaCounter : 8; //Current Channel Switch Announcement count, reset value: 0x0, access type: RO
		uint32 reserved1 : 8;
	} bitFields;
} RegB1BeaconVap24BeaconCsa_u;

/*REG_B1_BEACON_VAP25_BEACON_ACTIVATE 0x130 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap25BeaconEn : 1; //Beacon enable bit, reset value: 0x0, access type: RW
		uint32 reserved0 : 7;
		uint32 vap25BeaconInterval : 16; //Beacon interval in TUs , reset value: 0x0, access type: RW
		uint32 vap25DtimInterval : 8; //DTIM interval in TIM units, reset value: 0x0, access type: RW
	} bitFields;
} RegB1BeaconVap25BeaconActivate_u;

/*REG_B1_BEACON_VAP25_BEACON_OFFSET 0x134 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap25BeaconOffset : 16; //Beacon offset relative to virtual zero point, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB1BeaconVap25BeaconOffset_u;

/*REG_B1_BEACON_VAP25_BEACON_CSA 0x138 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 16;
		uint32 vap25CsaCounter : 8; //Current Channel Switch Announcement count, reset value: 0x0, access type: RO
		uint32 reserved1 : 8;
	} bitFields;
} RegB1BeaconVap25BeaconCsa_u;

/*REG_B1_BEACON_VAP26_BEACON_ACTIVATE 0x13C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap26BeaconEn : 1; //Beacon enable bit, reset value: 0x0, access type: RW
		uint32 reserved0 : 7;
		uint32 vap26BeaconInterval : 16; //Beacon interval in TUs , reset value: 0x0, access type: RW
		uint32 vap26DtimInterval : 8; //DTIM interval in TIM units, reset value: 0x0, access type: RW
	} bitFields;
} RegB1BeaconVap26BeaconActivate_u;

/*REG_B1_BEACON_VAP26_BEACON_OFFSET 0x140 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap26BeaconOffset : 16; //Beacon offset relative to virtual zero point, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB1BeaconVap26BeaconOffset_u;

/*REG_B1_BEACON_VAP26_BEACON_CSA 0x144 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 16;
		uint32 vap26CsaCounter : 8; //Current Channel Switch Announcement count, reset value: 0x0, access type: RO
		uint32 reserved1 : 8;
	} bitFields;
} RegB1BeaconVap26BeaconCsa_u;

/*REG_B1_BEACON_VAP27_BEACON_ACTIVATE 0x148 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap27BeaconEn : 1; //Beacon enable bit, reset value: 0x0, access type: RW
		uint32 reserved0 : 7;
		uint32 vap27BeaconInterval : 16; //Beacon interval in TUs , reset value: 0x0, access type: RW
		uint32 vap27DtimInterval : 8; //DTIM interval in TIM units, reset value: 0x0, access type: RW
	} bitFields;
} RegB1BeaconVap27BeaconActivate_u;

/*REG_B1_BEACON_VAP27_BEACON_OFFSET 0x14C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap27BeaconOffset : 16; //Beacon offset relative to virtual zero point, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB1BeaconVap27BeaconOffset_u;

/*REG_B1_BEACON_VAP27_BEACON_CSA 0x150 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 16;
		uint32 vap27CsaCounter : 8; //Current Channel Switch Announcement count, reset value: 0x0, access type: RO
		uint32 reserved1 : 8;
	} bitFields;
} RegB1BeaconVap27BeaconCsa_u;

/*REG_B1_BEACON_VAP28_BEACON_ACTIVATE 0x154 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap28BeaconEn : 1; //Beacon enable bit, reset value: 0x0, access type: RW
		uint32 reserved0 : 7;
		uint32 vap28BeaconInterval : 16; //Beacon interval in TUs , reset value: 0x0, access type: RW
		uint32 vap28DtimInterval : 8; //DTIM interval in TIM units, reset value: 0x0, access type: RW
	} bitFields;
} RegB1BeaconVap28BeaconActivate_u;

/*REG_B1_BEACON_VAP28_BEACON_OFFSET 0x158 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap28BeaconOffset : 16; //Beacon offset relative to virtual zero point, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB1BeaconVap28BeaconOffset_u;

/*REG_B1_BEACON_VAP28_BEACON_CSA 0x15C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 16;
		uint32 vap28CsaCounter : 8; //Current Channel Switch Announcement count, reset value: 0x0, access type: RO
		uint32 reserved1 : 8;
	} bitFields;
} RegB1BeaconVap28BeaconCsa_u;

/*REG_B1_BEACON_VAP29_BEACON_ACTIVATE 0x160 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap29BeaconEn : 1; //Beacon enable bit, reset value: 0x0, access type: RW
		uint32 reserved0 : 7;
		uint32 vap29BeaconInterval : 16; //Beacon interval in TUs , reset value: 0x0, access type: RW
		uint32 vap29DtimInterval : 8; //DTIM interval in TIM units, reset value: 0x0, access type: RW
	} bitFields;
} RegB1BeaconVap29BeaconActivate_u;

/*REG_B1_BEACON_VAP29_BEACON_OFFSET 0x164 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap29BeaconOffset : 16; //Beacon offset relative to virtual zero point, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB1BeaconVap29BeaconOffset_u;

/*REG_B1_BEACON_VAP29_BEACON_CSA 0x168 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 16;
		uint32 vap29CsaCounter : 8; //Current Channel Switch Announcement count, reset value: 0x0, access type: RO
		uint32 reserved1 : 8;
	} bitFields;
} RegB1BeaconVap29BeaconCsa_u;

/*REG_B1_BEACON_VAP30_BEACON_ACTIVATE 0x16C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap30BeaconEn : 1; //Beacon enable bit, reset value: 0x0, access type: RW
		uint32 reserved0 : 7;
		uint32 vap30BeaconInterval : 16; //Beacon interval in TUs , reset value: 0x0, access type: RW
		uint32 vap30DtimInterval : 8; //DTIM interval in TIM units, reset value: 0x0, access type: RW
	} bitFields;
} RegB1BeaconVap30BeaconActivate_u;

/*REG_B1_BEACON_VAP30_BEACON_OFFSET 0x170 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap30BeaconOffset : 16; //Beacon offset relative to virtual zero point, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB1BeaconVap30BeaconOffset_u;

/*REG_B1_BEACON_VAP30_BEACON_CSA 0x174 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 16;
		uint32 vap30CsaCounter : 8; //Current Channel Switch Announcement count, reset value: 0x0, access type: RO
		uint32 reserved1 : 8;
	} bitFields;
} RegB1BeaconVap30BeaconCsa_u;

/*REG_B1_BEACON_VAP31_BEACON_ACTIVATE 0x178 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap31BeaconEn : 1; //Beacon enable bit, reset value: 0x0, access type: RW
		uint32 reserved0 : 7;
		uint32 vap31BeaconInterval : 16; //Beacon interval in TUs , reset value: 0x0, access type: RW
		uint32 vap31DtimInterval : 8; //DTIM interval in TIM units, reset value: 0x0, access type: RW
	} bitFields;
} RegB1BeaconVap31BeaconActivate_u;

/*REG_B1_BEACON_VAP31_BEACON_OFFSET 0x17C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap31BeaconOffset : 16; //Beacon offset relative to virtual zero point, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB1BeaconVap31BeaconOffset_u;

/*REG_B1_BEACON_VAP31_BEACON_CSA 0x180 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 16;
		uint32 vap31CsaCounter : 8; //Current Channel Switch Announcement count, reset value: 0x0, access type: RO
		uint32 reserved1 : 8;
	} bitFields;
} RegB1BeaconVap31BeaconCsa_u;

/*REG_B1_BEACON_BEACON_CSA_EN 0x184 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 csaBeaconCountEn : 32; //Activate all enabled CSA counters, reset value: 0x0, access type: RW
	} bitFields;
} RegB1BeaconBeaconCsaEn_u;

/*REG_B1_BEACON_VAP_ACTIVE_STATUS 0x188 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vapActiveStatus : 32; //List of active VAPs, reset value: 0x0, access type: RO
	} bitFields;
} RegB1BeaconVapActiveStatus_u;

/*REG_B1_BEACON_BEACON_COUNTER 0x18C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 beaconReqCounter : 20; //number of beacons transmitted in the system, reset value: 0x0, access type: RO
		uint32 reserved0 : 11;
		uint32 beaconReqCounterClr : 1; //Clears Beacon counter, reset value: 0x0, access type: WO
	} bitFields;
} RegB1BeaconBeaconCounter_u;

/*REG_B1_BEACON_VERIFICATION_MODE 0x190 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 verificationMode : 1; //Verification mode to shift tsf counter, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB1BeaconVerificationMode_u;

/*REG_B1_BEACON_BEACON_LOGGER_CFG 0x194 */
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
} RegB1BeaconBeaconLoggerCfg_u;

/*REG_B1_BEACON_BEACON_LOGGER_BUSY 0x198 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 beaconTimersLoggerBusy : 1; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB1BeaconBeaconLoggerBusy_u;

/*REG_B1_BEACON_VAP01_TSF_OFFSET 0x19C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap0BeaconFinalOffset : 16; //no description, reset value: 0x0, access type: RO
		uint32 vap1BeaconFinalOffset : 16; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB1BeaconVap01TsfOffset_u;

/*REG_B1_BEACON_VAP23_TSF_OFFSET 0x1A0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap2BeaconFinalOffset : 16; //no description, reset value: 0x0, access type: RO
		uint32 vap3BeaconFinalOffset : 16; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB1BeaconVap23TsfOffset_u;

/*REG_B1_BEACON_VAP45_TSF_OFFSET 0x1A4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap4BeaconFinalOffset : 16; //no description, reset value: 0x0, access type: RO
		uint32 vap5BeaconFinalOffset : 16; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB1BeaconVap45TsfOffset_u;

/*REG_B1_BEACON_VAP67_TSF_OFFSET 0x1A8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap6BeaconFinalOffset : 16; //no description, reset value: 0x0, access type: RO
		uint32 vap7BeaconFinalOffset : 16; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB1BeaconVap67TsfOffset_u;

/*REG_B1_BEACON_VAP89_TSF_OFFSET 0x1AC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap8BeaconFinalOffset : 16; //no description, reset value: 0x0, access type: RO
		uint32 vap9BeaconFinalOffset : 16; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB1BeaconVap89TsfOffset_u;

/*REG_B1_BEACON_VAP1011_TSF_OFFSET 0x1B0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap10BeaconFinalOffset : 16; //no description, reset value: 0x0, access type: RO
		uint32 vap11BeaconFinalOffset : 16; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB1BeaconVap1011TsfOffset_u;

/*REG_B1_BEACON_VAP1213_TSF_OFFSET 0x1B4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap12BeaconFinalOffset : 16; //no description, reset value: 0x0, access type: RO
		uint32 vap13BeaconFinalOffset : 16; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB1BeaconVap1213TsfOffset_u;

/*REG_B1_BEACON_VAP1415_TSF_OFFSET 0x1B8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap14BeaconFinalOffset : 16; //no description, reset value: 0x0, access type: RO
		uint32 vap15BeaconFinalOffset : 16; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB1BeaconVap1415TsfOffset_u;

/*REG_B1_BEACON_VAP1617_TSF_OFFSET 0x1BC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap16BeaconFinalOffset : 16; //no description, reset value: 0x0, access type: RO
		uint32 vap17BeaconFinalOffset : 16; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB1BeaconVap1617TsfOffset_u;

/*REG_B1_BEACON_VAP1819_TSF_OFFSET 0x1C0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap18BeaconFinalOffset : 16; //no description, reset value: 0x0, access type: RO
		uint32 vap19BeaconFinalOffset : 16; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB1BeaconVap1819TsfOffset_u;

/*REG_B1_BEACON_VAP2021_TSF_OFFSET 0x1C4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap20BeaconFinalOffset : 16; //no description, reset value: 0x0, access type: RO
		uint32 vap21BeaconFinalOffset : 16; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB1BeaconVap2021TsfOffset_u;

/*REG_B1_BEACON_VAP2223_TSF_OFFSET 0x1C8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap22BeaconFinalOffset : 16; //no description, reset value: 0x0, access type: RO
		uint32 vap23BeaconFinalOffset : 16; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB1BeaconVap2223TsfOffset_u;

/*REG_B1_BEACON_VAP2425_TSF_OFFSET 0x1CC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap24BeaconFinalOffset : 16; //no description, reset value: 0x0, access type: RO
		uint32 vap25BeaconFinalOffset : 16; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB1BeaconVap2425TsfOffset_u;

/*REG_B1_BEACON_VAP2627_TSF_OFFSET 0x1D0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap26BeaconFinalOffset : 16; //no description, reset value: 0x0, access type: RO
		uint32 vap27BeaconFinalOffset : 16; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB1BeaconVap2627TsfOffset_u;

/*REG_B1_BEACON_VAP2829_TSF_OFFSET 0x1D4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap28BeaconFinalOffset : 16; //no description, reset value: 0x0, access type: RO
		uint32 vap29BeaconFinalOffset : 16; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB1BeaconVap2829TsfOffset_u;

/*REG_B1_BEACON_VAP3031_TSF_OFFSET 0x1D8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vap30BeaconFinalOffset : 16; //no description, reset value: 0x0, access type: RO
		uint32 vap31BeaconFinalOffset : 16; //no description, reset value: 0x0, access type: RO
	} bitFields;
} RegB1BeaconVap3031TsfOffset_u;

/*REG_B1_BEACON_MULTI_BSSID_PRIMARY_VAP 0x1DC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 multiBssidPrimaryVapIdx : 1; //Primary VAP for Multi BSSID group: , 0: Primary VAP is 0. , 1: Primary VAP is 16., reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB1BeaconMultiBssidPrimaryVap_u;

/*REG_B1_BEACON_MULTI_BSSID_VAPS_IN_GROUP 0x1E0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 multiBssidVapsInGroupEn : 32; //VAPs in the group. Valid fields are according to SW work assumptions., reset value: 0x0, access type: RW
	} bitFields;
} RegB1BeaconMultiBssidVapsInGroup_u;

/*REG_B1_BEACON_VAP0_BEACON_RO_COUNTERS 0x1E4 */
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
} RegB1BeaconVap0BeaconRoCounters_u;

/*REG_B1_BEACON_VAP1_BEACON_RO_COUNTERS 0x1E8 */
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
} RegB1BeaconVap1BeaconRoCounters_u;

/*REG_B1_BEACON_VAP2_BEACON_RO_COUNTERS 0x1EC */
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
} RegB1BeaconVap2BeaconRoCounters_u;

/*REG_B1_BEACON_VAP3_BEACON_RO_COUNTERS 0x1F0 */
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
} RegB1BeaconVap3BeaconRoCounters_u;

/*REG_B1_BEACON_VAP4_BEACON_RO_COUNTERS 0x1F4 */
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
} RegB1BeaconVap4BeaconRoCounters_u;

/*REG_B1_BEACON_VAP5_BEACON_RO_COUNTERS 0x1F8 */
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
} RegB1BeaconVap5BeaconRoCounters_u;

/*REG_B1_BEACON_VAP6_BEACON_RO_COUNTERS 0x1FC */
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
} RegB1BeaconVap6BeaconRoCounters_u;

/*REG_B1_BEACON_VAP7_BEACON_RO_COUNTERS 0x200 */
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
} RegB1BeaconVap7BeaconRoCounters_u;

/*REG_B1_BEACON_VAP8_BEACON_RO_COUNTERS 0x204 */
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
} RegB1BeaconVap8BeaconRoCounters_u;

/*REG_B1_BEACON_VAP9_BEACON_RO_COUNTERS 0x208 */
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
} RegB1BeaconVap9BeaconRoCounters_u;

/*REG_B1_BEACON_VAP10_BEACON_RO_COUNTERS 0x20C */
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
} RegB1BeaconVap10BeaconRoCounters_u;

/*REG_B1_BEACON_VAP11_BEACON_RO_COUNTERS 0x210 */
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
} RegB1BeaconVap11BeaconRoCounters_u;

/*REG_B1_BEACON_VAP12_BEACON_RO_COUNTERS 0x214 */
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
} RegB1BeaconVap12BeaconRoCounters_u;

/*REG_B1_BEACON_VAP13_BEACON_RO_COUNTERS 0x218 */
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
} RegB1BeaconVap13BeaconRoCounters_u;

/*REG_B1_BEACON_VAP14_BEACON_RO_COUNTERS 0x21C */
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
} RegB1BeaconVap14BeaconRoCounters_u;

/*REG_B1_BEACON_VAP15_BEACON_RO_COUNTERS 0x220 */
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
} RegB1BeaconVap15BeaconRoCounters_u;

/*REG_B1_BEACON_VAP16_BEACON_RO_COUNTERS 0x224 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 8;
		uint32 dtim16Counter : 8; //Current Dtim counter, reset value: 0x0, access type: RO
		uint32 vap16ColorCounter : 8; //Current Color change count, reset value: 0x0, access type: RO
		uint32 vap16PersistenceCounter : 8; //Current Persistence count, reset value: 0x0, access type: RO
	} bitFields;
} RegB1BeaconVap16BeaconRoCounters_u;

/*REG_B1_BEACON_VAP17_BEACON_RO_COUNTERS 0x228 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 8;
		uint32 dtim17Counter : 8; //Current Dtim counter, reset value: 0x0, access type: RO
		uint32 vap17ColorCounter : 8; //Current Color change count, reset value: 0x0, access type: RO
		uint32 vap17PersistenceCounter : 8; //Current Persistence count, reset value: 0x0, access type: RO
	} bitFields;
} RegB1BeaconVap17BeaconRoCounters_u;

/*REG_B1_BEACON_VAP18_BEACON_RO_COUNTERS 0x22C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 8;
		uint32 dtim18Counter : 8; //Current Dtim counter, reset value: 0x0, access type: RO
		uint32 vap18ColorCounter : 8; //Current Color change count, reset value: 0x0, access type: RO
		uint32 vap18PersistenceCounter : 8; //Current Persistence count, reset value: 0x0, access type: RO
	} bitFields;
} RegB1BeaconVap18BeaconRoCounters_u;

/*REG_B1_BEACON_VAP19_BEACON_RO_COUNTERS 0x230 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 8;
		uint32 dtim19Counter : 8; //Current Dtim counter, reset value: 0x0, access type: RO
		uint32 vap19ColorCounter : 8; //Current Color change count, reset value: 0x0, access type: RO
		uint32 vap19PersistenceCounter : 8; //Current Persistence count, reset value: 0x0, access type: RO
	} bitFields;
} RegB1BeaconVap19BeaconRoCounters_u;

/*REG_B1_BEACON_VAP20_BEACON_RO_COUNTERS 0x234 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 8;
		uint32 dtim20Counter : 8; //Current Dtim counter, reset value: 0x0, access type: RO
		uint32 vap20ColorCounter : 8; //Current Color change count, reset value: 0x0, access type: RO
		uint32 vap20PersistenceCounter : 8; //Current Persistence count, reset value: 0x0, access type: RO
	} bitFields;
} RegB1BeaconVap20BeaconRoCounters_u;

/*REG_B1_BEACON_VAP21_BEACON_RO_COUNTERS 0x238 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 8;
		uint32 dtim21Counter : 8; //Current Dtim counter, reset value: 0x0, access type: RO
		uint32 vap21ColorCounter : 8; //Current Color change count, reset value: 0x0, access type: RO
		uint32 vap21PersistenceCounter : 8; //Current Persistence count, reset value: 0x0, access type: RO
	} bitFields;
} RegB1BeaconVap21BeaconRoCounters_u;

/*REG_B1_BEACON_VAP22_BEACON_RO_COUNTERS 0x23C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 8;
		uint32 dtim22Counter : 8; //Current Dtim counter, reset value: 0x0, access type: RO
		uint32 vap22ColorCounter : 8; //Current Color change count, reset value: 0x0, access type: RO
		uint32 vap22PersistenceCounter : 8; //Current Persistence count, reset value: 0x0, access type: RO
	} bitFields;
} RegB1BeaconVap22BeaconRoCounters_u;

/*REG_B1_BEACON_VAP23_BEACON_RO_COUNTERS 0x240 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 8;
		uint32 dtim23Counter : 8; //Current Dtim counter, reset value: 0x0, access type: RO
		uint32 vap23ColorCounter : 8; //Current Color change count, reset value: 0x0, access type: RO
		uint32 vap23PersistenceCounter : 8; //Current Persistence count, reset value: 0x0, access type: RO
	} bitFields;
} RegB1BeaconVap23BeaconRoCounters_u;

/*REG_B1_BEACON_VAP24_BEACON_RO_COUNTERS 0x244 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 8;
		uint32 dtim24Counter : 8; //Current Dtim counter, reset value: 0x0, access type: RO
		uint32 vap24ColorCounter : 8; //Current Color change count, reset value: 0x0, access type: RO
		uint32 vap24PersistenceCounter : 8; //Current Persistence count, reset value: 0x0, access type: RO
	} bitFields;
} RegB1BeaconVap24BeaconRoCounters_u;

/*REG_B1_BEACON_VAP25_BEACON_RO_COUNTERS 0x248 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 8;
		uint32 dtim25Counter : 8; //Current Dtim counter, reset value: 0x0, access type: RO
		uint32 vap25ColorCounter : 8; //Current Color change count, reset value: 0x0, access type: RO
		uint32 vap25PersistenceCounter : 8; //Current Persistence count, reset value: 0x0, access type: RO
	} bitFields;
} RegB1BeaconVap25BeaconRoCounters_u;

/*REG_B1_BEACON_VAP26_BEACON_RO_COUNTERS 0x24C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 8;
		uint32 dtim26Counter : 8; //Current Dtim counter, reset value: 0x0, access type: RO
		uint32 vap26ColorCounter : 8; //Current Color change count, reset value: 0x0, access type: RO
		uint32 vap26PersistenceCounter : 8; //Current Persistence count, reset value: 0x0, access type: RO
	} bitFields;
} RegB1BeaconVap26BeaconRoCounters_u;

/*REG_B1_BEACON_VAP27_BEACON_RO_COUNTERS 0x250 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 8;
		uint32 dtim27Counter : 8; //Current Dtim counter, reset value: 0x0, access type: RO
		uint32 vap27ColorCounter : 8; //Current Color change count, reset value: 0x0, access type: RO
		uint32 vap27PersistenceCounter : 8; //Current Persistence count, reset value: 0x0, access type: RO
	} bitFields;
} RegB1BeaconVap27BeaconRoCounters_u;

/*REG_B1_BEACON_VAP28_BEACON_RO_COUNTERS 0x254 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 8;
		uint32 dtim28Counter : 8; //Current Dtim counter, reset value: 0x0, access type: RO
		uint32 vap28ColorCounter : 8; //Current Color change count, reset value: 0x0, access type: RO
		uint32 vap28PersistenceCounter : 8; //Current Persistence count, reset value: 0x0, access type: RO
	} bitFields;
} RegB1BeaconVap28BeaconRoCounters_u;

/*REG_B1_BEACON_VAP29_BEACON_RO_COUNTERS 0x258 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 8;
		uint32 dtim29Counter : 8; //Current Dtim counter, reset value: 0x0, access type: RO
		uint32 vap29ColorCounter : 8; //Current Color change count, reset value: 0x0, access type: RO
		uint32 vap29PersistenceCounter : 8; //Current Persistence count, reset value: 0x0, access type: RO
	} bitFields;
} RegB1BeaconVap29BeaconRoCounters_u;

/*REG_B1_BEACON_VAP30_BEACON_RO_COUNTERS 0x25C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 8;
		uint32 dtim30Counter : 8; //Current Dtim counter, reset value: 0x0, access type: RO
		uint32 vap30ColorCounter : 8; //Current Color change count, reset value: 0x0, access type: RO
		uint32 vap30PersistenceCounter : 8; //Current Persistence count, reset value: 0x0, access type: RO
	} bitFields;
} RegB1BeaconVap30BeaconRoCounters_u;

/*REG_B1_BEACON_VAP31_BEACON_RO_COUNTERS 0x260 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 8;
		uint32 dtim31Counter : 8; //no description, reset value: 0x0, access type: RO
		uint32 vap31ColorCounter : 8; //Current Color change count, reset value: 0x0, access type: RO
		uint32 vap31PersistenceCounter : 8; //Current Persistence count, reset value: 0x0, access type: RO
	} bitFields;
} RegB1BeaconVap31BeaconRoCounters_u;

/*REG_B1_BEACON_BEACON_PERSISTENCE_EN 0x264 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 persistenceBeaconCountEn : 32; //Activate all enabled PERSISTENCE counters, reset value: 0x0, access type: RW
	} bitFields;
} RegB1BeaconBeaconPersistenceEn_u;

/*REG_B1_BEACON_BEACON_COLOR_EN 0x268 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 colorBeaconCountEn : 32; //Activate all enabled COLOR counters, reset value: 0x0, access type: RW
	} bitFields;
} RegB1BeaconBeaconColorEn_u;

/*REG_B1_BEACON_BEACON_START_FIELD_CFG 0x26C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 fieldUpdateValue : 8; //For DTIM first period: DTIM start count value in TIM units , For CSA and Color counters: number of TTIMs to count, reset value: 0x0, access type: RW
		uint32 fieldUpdateVapIdx : 5; //VAP index to update the DTIM start period, reset value: 0x0, access type: RW
		uint32 reserved0 : 3;
		uint32 fieldUpdateType : 4; //Field update type: , 0001: Dtim first period , 0010: CSA counter , 0100: Color counter , 1000: Persistence Counter, reset value: 0x0, access type: RW
		uint32 reserved1 : 12;
	} bitFields;
} RegB1BeaconBeaconStartFieldCfg_u;



#endif // _B1_BEACON_REGS_H_
