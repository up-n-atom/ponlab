
/***********************************************************************************
File:				HostTbRegs.h
Module:				hostTb
SOC Revision:		latest
Generated at:       2024-06-26 12:55:06
Purpose:
Description:		This File was auto generated using SOC Online

************************************************************************************/
#ifndef _HOST_TB_REGS_H_
#define _HOST_TB_REGS_H_

/*---------------------------------------------------------------------------------
/						Registers Addresses													 
/----------------------------------------------------------------------------------*/
#include "HwMemoryMap.h"

#define HOST_TB_BASE_ADDRESS       MEMORY_MAP_UNIT__BASE_ADDRESS
#define	REG_HOST_TB_CONST_VAL    (HOST_TB_BASE_ADDRESS + 0x0)
#define	REG_HOST_TB_INIT         (HOST_TB_BASE_ADDRESS + 0x4)
#define	REG_HOST_TB_SEED         (HOST_TB_BASE_ADDRESS + 0x8)
#define	REG_HOST_TB_SHIFT_NUM    (HOST_TB_BASE_ADDRESS + 0xC)
#define	REG_HOST_TB_ECW          (HOST_TB_BASE_ADDRESS + 0x10)
#define	REG_HOST_TB_SDL_CTRL     (HOST_TB_BASE_ADDRESS + 0x14)
/*---------------------------------------------------------------------------------
/						Data Type Definition										
/----------------------------------------------------------------------------------*/
/*REG_HOST_TB_CONST_VAL 0x0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 constRdVal : 5; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 15;
		uint32 constWrVal : 5; //no description, reset value: 0x0, access type: RW
		uint32 reserved1 : 7;
	} bitFields;
} RegHostTbConstVal_u;

/*REG_HOST_TB_INIT 0x4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 initBlock : 1; //no description, reset value: 0x0, access type: WO
		uint32 enPrbs : 1; //no description, reset value: 0x0, access type: WO
		uint32 reserved0 : 30;
	} bitFields;
} RegHostTbInit_u;

/*REG_HOST_TB_SEED 0x8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 seed : 32; //no description, reset value: 0x12345678, access type: RW
	} bitFields;
} RegHostTbSeed_u;

/*REG_HOST_TB_SHIFT_NUM 0xC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 shiftNum : 5; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 27;
	} bitFields;
} RegHostTbShiftNum_u;

/*REG_HOST_TB_ECW 0x10 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ecw : 4; //no description, reset value: 0x5, access type: RW
		uint32 reserved0 : 28;
	} bitFields;
} RegHostTbEcw_u;

/*REG_HOST_TB_SDL_CTRL 0x14 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 firewallOn : 1; //no description, reset value: 0x0, access type: RW
		uint32 spiDisable : 1; //SPI disable indication (security provision), reset value: 0x0, access type: RW
		uint32 jtagDisable : 1; //no description, reset value: 0x0, access type: RW
		uint32 secureBootOn : 1; //no description, reset value: 0x0, access type: RW
		uint32 chipModeSetting : 3; //Chip mode setting from efuse, reset value: 0x0, access type: RW
		uint32 rfId : 8; //RF ID from efuse, reset value: 0x0, access type: RW
		uint32 hwType : 8; // HW type from efuse, reset value: 0x0, access type: RW
		uint32 hwRevision : 8; //HW revision from efuse, reset value: 0x0, access type: RW
		uint32 reserved0 : 1;
	} bitFields;
} RegHostTbSdlCtrl_u;

//========================================
/* REG_HOST_TB_CONST_VAL 0x0 */
#define HOST_TB_CONST_VAL_FIELD_CONST_RD_VAL_MASK                                                                 0x0000001F
#define HOST_TB_CONST_VAL_FIELD_CONST_WR_VAL_MASK                                                                 0x01F00000
//========================================
/* REG_HOST_TB_INIT 0x4 */
#define HOST_TB_INIT_FIELD_INIT_BLOCK_MASK                                                                        0x00000001
#define HOST_TB_INIT_FIELD_EN_PRBS_MASK                                                                           0x00000002
//========================================
/* REG_HOST_TB_SEED 0x8 */
#define HOST_TB_SEED_FIELD_SEED_MASK                                                                              0xFFFFFFFF
//========================================
/* REG_HOST_TB_SHIFT_NUM 0xC */
#define HOST_TB_SHIFT_NUM_FIELD_SHIFT_NUM_MASK                                                                    0x0000001F
//========================================
/* REG_HOST_TB_ECW 0x10 */
#define HOST_TB_ECW_FIELD_ECW_MASK                                                                                0x0000000F
//========================================
/* REG_HOST_TB_SDL_CTRL 0x14 */
#define HOST_TB_SDL_CTRL_FIELD_FIREWALL_ON_MASK                                                                   0x00000001
#define HOST_TB_SDL_CTRL_FIELD_SPI_DISABLE_MASK                                                                   0x00000002
#define HOST_TB_SDL_CTRL_FIELD_JTAG_DISABLE_MASK                                                                  0x00000004
#define HOST_TB_SDL_CTRL_FIELD_SECURE_BOOT_ON_MASK                                                                0x00000008
#define HOST_TB_SDL_CTRL_FIELD_CHIP_MODE_SETTING_MASK                                                             0x00000070
#define HOST_TB_SDL_CTRL_FIELD_RF_ID_MASK                                                                         0x00007F80
#define HOST_TB_SDL_CTRL_FIELD_HW_TYPE_MASK                                                                       0x007F8000
#define HOST_TB_SDL_CTRL_FIELD_HW_REVISION_MASK                                                                   0x7F800000


#endif // _HOST_TB_REGS_H_
