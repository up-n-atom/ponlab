
/***********************************************************************************
File:				B2MacAddr2IndexRegs.h
Module:				b2MacAddr2Index
SOC Revision:		latest
Generated at:       2024-06-26 12:54:49
Purpose:
Description:		This File was auto generated using SOC Online

************************************************************************************/
#ifndef _B2_MAC_ADDR2INDEX_REGS_H_
#define _B2_MAC_ADDR2INDEX_REGS_H_

/*---------------------------------------------------------------------------------
/						Registers Addresses													 
/----------------------------------------------------------------------------------*/
#include "HwMemoryMap.h"

#define B2_MAC_ADDR2INDEX_BASE_ADDRESS                    MEMORY_MAP_UNIT_20022_BASE_ADDRESS
#define	REG_B2_MAC_ADDR2INDEX_QUEUE_STA_DB_BASE_ADDR              (B2_MAC_ADDR2INDEX_BASE_ADDRESS + 0x0)
#define	REG_B2_MAC_ADDR2INDEX_QUEUE_VAP_DB_BASE_ADDR              (B2_MAC_ADDR2INDEX_BASE_ADDRESS + 0x4)
#define	REG_B2_MAC_ADDR2INDEX_QUEUE_GLOBAL_DB_BASE_ADDR           (B2_MAC_ADDR2INDEX_BASE_ADDRESS + 0x8)
#define	REG_B2_MAC_ADDR2INDEX_QUEUE_GPLP_DB_BASE_ADDR             (B2_MAC_ADDR2INDEX_BASE_ADDRESS + 0xC)
#define	REG_B2_MAC_ADDR2INDEX_QUEUE_GPHP_DB_BASE_ADDR             (B2_MAC_ADDR2INDEX_BASE_ADDRESS + 0x10)
#define	REG_B2_MAC_ADDR2INDEX_QUEUE_BEACON_DB_BASE_ADDR           (B2_MAC_ADDR2INDEX_BASE_ADDRESS + 0x14)
#define	REG_B2_MAC_ADDR2INDEX_QUEUE_STA_DB_SIZE                   (B2_MAC_ADDR2INDEX_BASE_ADDRESS + 0x18)
#define	REG_B2_MAC_ADDR2INDEX_QUEUE_VAP_DB_SIZE                   (B2_MAC_ADDR2INDEX_BASE_ADDRESS + 0x20)
#define	REG_B2_MAC_ADDR2INDEX_QUEUE_GLOBAL_DB_SIZE                (B2_MAC_ADDR2INDEX_BASE_ADDRESS + 0x24)
#define	REG_B2_MAC_ADDR2INDEX_QUEUE_GPLP_DB_SIZE                  (B2_MAC_ADDR2INDEX_BASE_ADDRESS + 0x28)
#define	REG_B2_MAC_ADDR2INDEX_QUEUE_GPHP_DB_SIZE                  (B2_MAC_ADDR2INDEX_BASE_ADDRESS + 0x2C)
#define	REG_B2_MAC_ADDR2INDEX_QUEUE_BEACON_DB_SIZE                (B2_MAC_ADDR2INDEX_BASE_ADDRESS + 0x30)
#define	REG_B2_MAC_ADDR2INDEX_MAC_ADDR2INDEX_COMMAND              (B2_MAC_ADDR2INDEX_BASE_ADDRESS + 0x34)
#define	REG_B2_MAC_ADDR2INDEX_MAC_ADDR2INDEX_ADDR_0TO31           (B2_MAC_ADDR2INDEX_BASE_ADDRESS + 0x38)
#define	REG_B2_MAC_ADDR2INDEX_MAC_ADDR2INDEX_ADDR_32TO47          (B2_MAC_ADDR2INDEX_BASE_ADDRESS + 0x3C)
#define	REG_B2_MAC_ADDR2INDEX_MAC_ADDR2INDEX_INIT_PARAMS          (B2_MAC_ADDR2INDEX_BASE_ADDRESS + 0x40)
#define	REG_B2_MAC_ADDR2INDEX_MAC_ADDR2INDEX_STATUS               (B2_MAC_ADDR2INDEX_BASE_ADDRESS + 0x44)
#define	REG_B2_MAC_ADDR2INDEX_MAC_ADDR2INDEX_VALID_STA_0TO31      (B2_MAC_ADDR2INDEX_BASE_ADDRESS + 0x48)
#define	REG_B2_MAC_ADDR2INDEX_MAC_ADDR2INDEX_VALID_STA_32TO63     (B2_MAC_ADDR2INDEX_BASE_ADDRESS + 0x4C)
#define	REG_B2_MAC_ADDR2INDEX_MAC_ADDR2INDEX_VALID_STA_64TO95     (B2_MAC_ADDR2INDEX_BASE_ADDRESS + 0x50)
#define	REG_B2_MAC_ADDR2INDEX_MAC_ADDR2INDEX_VALID_STA_96TO127    (B2_MAC_ADDR2INDEX_BASE_ADDRESS + 0x54)
#define	REG_B2_MAC_ADDR2INDEX_MAC_ADDR2INDEX_STATUS_2             (B2_MAC_ADDR2INDEX_BASE_ADDRESS + 0x68)
#define	REG_B2_MAC_ADDR2INDEX_MAC_ADDR2INDEX_STATUS_2_CLR         (B2_MAC_ADDR2INDEX_BASE_ADDRESS + 0x6C)
#define	REG_B2_MAC_ADDR2INDEX_MAC_ADDR2INDEX_SPARE_REGISTER       (B2_MAC_ADDR2INDEX_BASE_ADDRESS + 0x70)
/*---------------------------------------------------------------------------------
/						Data Type Definition										
/----------------------------------------------------------------------------------*/
/*REG_B2_MAC_ADDR2INDEX_QUEUE_STA_DB_BASE_ADDR 0x0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 queueStaDbBaseAddr : 24; //Queue STA DB base addr, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegB2MacAddr2IndexQueueStaDbBaseAddr_u;

/*REG_B2_MAC_ADDR2INDEX_QUEUE_VAP_DB_BASE_ADDR 0x4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 queueVapDbBaseAddr : 24; //Queue VAP DB base addr, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegB2MacAddr2IndexQueueVapDbBaseAddr_u;

/*REG_B2_MAC_ADDR2INDEX_QUEUE_GLOBAL_DB_BASE_ADDR 0x8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 queueGlobalDbBaseAddr : 24; //Queue Global DB base addr, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegB2MacAddr2IndexQueueGlobalDbBaseAddr_u;

/*REG_B2_MAC_ADDR2INDEX_QUEUE_GPLP_DB_BASE_ADDR 0xC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 queueGplpDbBaseAddr : 24; //Queue GPLP DB base addr, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegB2MacAddr2IndexQueueGplpDbBaseAddr_u;

/*REG_B2_MAC_ADDR2INDEX_QUEUE_GPHP_DB_BASE_ADDR 0x10 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 queueGphpDbBaseAddr : 24; //Queue GPHP DB base addr, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegB2MacAddr2IndexQueueGphpDbBaseAddr_u;

/*REG_B2_MAC_ADDR2INDEX_QUEUE_BEACON_DB_BASE_ADDR 0x14 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 queueBeaconDbBaseAddr : 24; //Queue Beacon DB base addr, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegB2MacAddr2IndexQueueBeaconDbBaseAddr_u;

/*REG_B2_MAC_ADDR2INDEX_QUEUE_STA_DB_SIZE 0x18 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 queueStaDbEntry4BSize : 8; //Queue STA DB entry 4Bytes size, reset value: 0x0, access type: RW
		uint32 queueStaSecondaryDbEntry4BOffset : 8; //Queue STA secondary DB entry 4Bytes offset, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB2MacAddr2IndexQueueStaDbSize_u;

/*REG_B2_MAC_ADDR2INDEX_QUEUE_VAP_DB_SIZE 0x20 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 queueVapDbEntry4BSize : 8; //Queue VAP DB entry 4Bytes size, reset value: 0x0, access type: RW
		uint32 queueVapSecondaryDbEntry4BOffset : 8; //Queue VAP secondary DB entry 4Bytes offset, reset value: 0x0, access type: RW
		uint32 queueVapDbBssid4BOffset : 8; //Queue VAP BSSID 4Bytes offset, reset value: 0x0, access type: RW
		uint32 queueVapDbBeacon4BOffset : 8; //Queue VAP Beacon 4Bytes offset, reset value: 0x0, access type: RW
	} bitFields;
} RegB2MacAddr2IndexQueueVapDbSize_u;

/*REG_B2_MAC_ADDR2INDEX_QUEUE_GLOBAL_DB_SIZE 0x24 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 queueGlobalDbEntry4BSize : 8; //Queue Global DB entry 4Bytes size, reset value: 0x0, access type: RW
		uint32 queueGlobalSecondaryDbEntry4BOffset : 8; //Queue Global secondary DB entry 4Bytes offset, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB2MacAddr2IndexQueueGlobalDbSize_u;

/*REG_B2_MAC_ADDR2INDEX_QUEUE_GPLP_DB_SIZE 0x28 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 queueGplpDbEntry4BSize : 8; //Queue GPLP DB entry 4Bytes size, reset value: 0x0, access type: RW
		uint32 queueGplpSecondaryDbEntry4BOffset : 8; //Queue GPLP secondary DB entry 4Bytes offset, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB2MacAddr2IndexQueueGplpDbSize_u;

/*REG_B2_MAC_ADDR2INDEX_QUEUE_GPHP_DB_SIZE 0x2C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 queueGphpDbEntry4BSize : 8; //Queue GPHP DB entry 4Bytes size, reset value: 0x0, access type: RW
		uint32 queueGphpSecondaryDbEntry4BOffset : 8; //Queue GPHP secondary DB entry 4Bytes offset, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB2MacAddr2IndexQueueGphpDbSize_u;

/*REG_B2_MAC_ADDR2INDEX_QUEUE_BEACON_DB_SIZE 0x30 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 queueBeaconDbEntry4BSize : 8; //Queue Beacon DB entry 4Bytes size, reset value: 0x0, access type: RW
		uint32 queueBeaconSecondaryDbEntry4BOffset : 8; //Queue Beacon secondary DB entry 4Bytes offset, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB2MacAddr2IndexQueueBeaconDbSize_u;

/*REG_B2_MAC_ADDR2INDEX_MAC_ADDR2INDEX_COMMAND 0x34 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 macAddr2IndexCommand : 3; //MAC addr2index command, reset value: 0x0, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB2MacAddr2IndexMacAddr2IndexCommand_u;

/*REG_B2_MAC_ADDR2INDEX_MAC_ADDR2INDEX_ADDR_0TO31 0x38 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 macAddr2IndexAddr0To31 : 32; //MAC addr2index addr 0to31, reset value: 0x0, access type: RW
	} bitFields;
} RegB2MacAddr2IndexMacAddr2IndexAddr0To31_u;

/*REG_B2_MAC_ADDR2INDEX_MAC_ADDR2INDEX_ADDR_32TO47 0x3C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 macAddr2IndexAddr32To47 : 16; //MAC addr2index addr 32to47, reset value: 0x0, access type: RW
		uint32 removeDbIndex : 8; //MAC addr2index remove DB index, reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegB2MacAddr2IndexMacAddr2IndexAddr32To47_u;

/*REG_B2_MAC_ADDR2INDEX_MAC_ADDR2INDEX_INIT_PARAMS 0x40 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 firstIndex : 8; //MAC addr2index first index, reset value: 0x0, access type: RW
		uint32 maxNumOfIndexes : 9; //MAC addr2index maximal number of indexes, reset value: 0x80, access type: RW
		uint32 reserved0 : 15;
	} bitFields;
} RegB2MacAddr2IndexMacAddr2IndexInitParams_u;

/*REG_B2_MAC_ADDR2INDEX_MAC_ADDR2INDEX_STATUS 0x44 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 macAddr2IndexDone : 1; //MAC addr2index done, reset value: 0x1, access type: RO
		uint32 macAddr2IndexFail : 3; //MAC addr2index fail status.  [0] - Search/Remove operation --- MAC address not found.  [1] - Add operation --- MAC address already exists.  [2] - Add operation --- database is full., reset value: 0x0, access type: RO
		uint32 reserved0 : 4;
		uint32 macAddr2IndexDbIndex : 8; //MAC addr2index DB index, reset value: 0x0, access type: RO
		uint32 macAddr2IndexNumValidIndexes : 9; //MAC addr2index num valid indexes, reset value: 0x0, access type: RO
		uint32 macAddr2IndexSm0 : 3; //MAC addr2index state machine 0, reset value: 0x0, access type: RO
		uint32 macAddr2IndexSm1 : 1; //MAC addr2index state machine 1 (Search Only SM), reset value: 0x0, access type: RO
		uint32 macAddr2IndexArbSm : 3; //MAC addr2index arbiter state machine, reset value: 0x0, access type: RO
	} bitFields;
} RegB2MacAddr2IndexMacAddr2IndexStatus_u;

/*REG_B2_MAC_ADDR2INDEX_MAC_ADDR2INDEX_VALID_STA_0TO31 0x48 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 macAddr2IndexValidSta0To31 : 32; //MAC addr2index valid STAs 0to31, reset value: 0x0, access type: RO
	} bitFields;
} RegB2MacAddr2IndexMacAddr2IndexValidSta0To31_u;

/*REG_B2_MAC_ADDR2INDEX_MAC_ADDR2INDEX_VALID_STA_32TO63 0x4C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 macAddr2IndexValidSta32To63 : 32; //MAC addr2index valid STAs 32to63, reset value: 0x0, access type: RO
	} bitFields;
} RegB2MacAddr2IndexMacAddr2IndexValidSta32To63_u;

/*REG_B2_MAC_ADDR2INDEX_MAC_ADDR2INDEX_VALID_STA_64TO95 0x50 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 macAddr2IndexValidSta64To95 : 32; //MAC addr2index valid STAs 64to95, reset value: 0x0, access type: RO
	} bitFields;
} RegB2MacAddr2IndexMacAddr2IndexValidSta64To95_u;

/*REG_B2_MAC_ADDR2INDEX_MAC_ADDR2INDEX_VALID_STA_96TO127 0x54 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 macAddr2IndexValidSta96To127 : 32; //MAC addr2index valid STAs 96to127, reset value: 0x0, access type: RO
	} bitFields;
} RegB2MacAddr2IndexMacAddr2IndexValidSta96To127_u;

/*REG_B2_MAC_ADDR2INDEX_MAC_ADDR2INDEX_STATUS_2 0x68 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 a2INumOfHwCmdFifoReachedFull : 12; //Number of times that HW_CMD_FIFO has reached full (Clear on Write), reset value: 0x0, access type: RO
		uint32 reserved0 : 4;
		uint32 a2IHwCmdFifoOccupancyMax : 4; //Maximal Occupancy of HW_CMD_FIFO (Clear on Write), reset value: 0x0, access type: RO
		uint32 a2IHwRespFifoOccupancyMax : 5; //Maximal Occupancy of HW_RESP_FIFO (Clear on Write), reset value: 0x0, access type: RO
		uint32 reserved1 : 7;
	} bitFields;
} RegB2MacAddr2IndexMacAddr2IndexStatus2_u;

/*REG_B2_MAC_ADDR2INDEX_MAC_ADDR2INDEX_STATUS_2_CLR 0x6C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 a2INumOfHwCmdFifoReachedFullClr : 1; //Write 1 to this field in order to clear num_of_hw_cmd_fifo_reached_full counter, reset value: 0x0, access type: WO
		uint32 a2IHwCmdFifoOccupancyMaxClr : 1; //Write 1 to this field in order to clear hw_cmd_fifo_occupancy_max, reset value: 0x0, access type: WO
		uint32 a2IHwRespFifoOccupancyMaxClr : 1; //Write 1 to this field in order to clear hw_resp_fifo_occupancy_max, reset value: 0x0, access type: WO
		uint32 reserved0 : 29;
	} bitFields;
} RegB2MacAddr2IndexMacAddr2IndexStatus2Clr_u;

/*REG_B2_MAC_ADDR2INDEX_MAC_ADDR2INDEX_SPARE_REGISTER 0x70 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 spareRegister : 16; //no description, reset value: 0x8888, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB2MacAddr2IndexMacAddr2IndexSpareRegister_u;

//========================================
/* REG_MAC_ADDR2INDEX_QUEUE_STA_DB_BASE_ADDR 0x0 */
#define B2_MAC_ADDR2INDEX_QUEUE_STA_DB_BASE_ADDR_FIELD_QUEUE_STA_DB_BASE_ADDR_MASK                                0x00FFFFFF
//========================================
/* REG_MAC_ADDR2INDEX_QUEUE_VAP_DB_BASE_ADDR 0x4 */
#define B2_MAC_ADDR2INDEX_QUEUE_VAP_DB_BASE_ADDR_FIELD_QUEUE_VAP_DB_BASE_ADDR_MASK                                0x00FFFFFF
//========================================
/* REG_MAC_ADDR2INDEX_QUEUE_GLOBAL_DB_BASE_ADDR 0x8 */
#define B2_MAC_ADDR2INDEX_QUEUE_GLOBAL_DB_BASE_ADDR_FIELD_QUEUE_GLOBAL_DB_BASE_ADDR_MASK                          0x00FFFFFF
//========================================
/* REG_MAC_ADDR2INDEX_QUEUE_GPLP_DB_BASE_ADDR 0xC */
#define B2_MAC_ADDR2INDEX_QUEUE_GPLP_DB_BASE_ADDR_FIELD_QUEUE_GPLP_DB_BASE_ADDR_MASK                              0x00FFFFFF
//========================================
/* REG_MAC_ADDR2INDEX_QUEUE_GPHP_DB_BASE_ADDR 0x10 */
#define B2_MAC_ADDR2INDEX_QUEUE_GPHP_DB_BASE_ADDR_FIELD_QUEUE_GPHP_DB_BASE_ADDR_MASK                              0x00FFFFFF
//========================================
/* REG_MAC_ADDR2INDEX_QUEUE_BEACON_DB_BASE_ADDR 0x14 */
#define B2_MAC_ADDR2INDEX_QUEUE_BEACON_DB_BASE_ADDR_FIELD_QUEUE_BEACON_DB_BASE_ADDR_MASK                          0x00FFFFFF
//========================================
/* REG_MAC_ADDR2INDEX_QUEUE_STA_DB_SIZE 0x18 */
#define B2_MAC_ADDR2INDEX_QUEUE_STA_DB_SIZE_FIELD_QUEUE_STA_DB_ENTRY_4B_SIZE_MASK                                 0x000000FF
#define B2_MAC_ADDR2INDEX_QUEUE_STA_DB_SIZE_FIELD_QUEUE_STA_SECONDARY_DB_ENTRY_4B_OFFSET_MASK                     0x0000FF00
//========================================
/* REG_MAC_ADDR2INDEX_QUEUE_VAP_DB_SIZE 0x20 */
#define B2_MAC_ADDR2INDEX_QUEUE_VAP_DB_SIZE_FIELD_QUEUE_VAP_DB_ENTRY_4B_SIZE_MASK                                 0x000000FF
#define B2_MAC_ADDR2INDEX_QUEUE_VAP_DB_SIZE_FIELD_QUEUE_VAP_SECONDARY_DB_ENTRY_4B_OFFSET_MASK                     0x0000FF00
#define B2_MAC_ADDR2INDEX_QUEUE_VAP_DB_SIZE_FIELD_QUEUE_VAP_DB_BSSID_4B_OFFSET_MASK                               0x00FF0000
#define B2_MAC_ADDR2INDEX_QUEUE_VAP_DB_SIZE_FIELD_QUEUE_VAP_DB_BEACON_4B_OFFSET_MASK                              0xFF000000
//========================================
/* REG_MAC_ADDR2INDEX_QUEUE_GLOBAL_DB_SIZE 0x24 */
#define B2_MAC_ADDR2INDEX_QUEUE_GLOBAL_DB_SIZE_FIELD_QUEUE_GLOBAL_DB_ENTRY_4B_SIZE_MASK                           0x000000FF
#define B2_MAC_ADDR2INDEX_QUEUE_GLOBAL_DB_SIZE_FIELD_QUEUE_GLOBAL_SECONDARY_DB_ENTRY_4B_OFFSET_MASK               0x0000FF00
//========================================
/* REG_MAC_ADDR2INDEX_QUEUE_GPLP_DB_SIZE 0x28 */
#define B2_MAC_ADDR2INDEX_QUEUE_GPLP_DB_SIZE_FIELD_QUEUE_GPLP_DB_ENTRY_4B_SIZE_MASK                               0x000000FF
#define B2_MAC_ADDR2INDEX_QUEUE_GPLP_DB_SIZE_FIELD_QUEUE_GPLP_SECONDARY_DB_ENTRY_4B_OFFSET_MASK                   0x0000FF00
//========================================
/* REG_MAC_ADDR2INDEX_QUEUE_GPHP_DB_SIZE 0x2C */
#define B2_MAC_ADDR2INDEX_QUEUE_GPHP_DB_SIZE_FIELD_QUEUE_GPHP_DB_ENTRY_4B_SIZE_MASK                               0x000000FF
#define B2_MAC_ADDR2INDEX_QUEUE_GPHP_DB_SIZE_FIELD_QUEUE_GPHP_SECONDARY_DB_ENTRY_4B_OFFSET_MASK                   0x0000FF00
//========================================
/* REG_MAC_ADDR2INDEX_QUEUE_BEACON_DB_SIZE 0x30 */
#define B2_MAC_ADDR2INDEX_QUEUE_BEACON_DB_SIZE_FIELD_QUEUE_BEACON_DB_ENTRY_4B_SIZE_MASK                           0x000000FF
#define B2_MAC_ADDR2INDEX_QUEUE_BEACON_DB_SIZE_FIELD_QUEUE_BEACON_SECONDARY_DB_ENTRY_4B_OFFSET_MASK               0x0000FF00
//========================================
/* REG_MAC_ADDR2INDEX_MAC_ADDR2INDEX_COMMAND 0x34 */
#define B2_MAC_ADDR2INDEX_MAC_ADDR2INDEX_COMMAND_FIELD_MAC_ADDR2INDEX_COMMAND_MASK                                0x00000007
//========================================
/* REG_MAC_ADDR2INDEX_MAC_ADDR2INDEX_ADDR_0TO31 0x38 */
#define B2_MAC_ADDR2INDEX_MAC_ADDR2INDEX_ADDR_0TO31_FIELD_MAC_ADDR2INDEX_ADDR_0TO31_MASK                          0xFFFFFFFF
//========================================
/* REG_MAC_ADDR2INDEX_MAC_ADDR2INDEX_ADDR_32TO47 0x3C */
#define B2_MAC_ADDR2INDEX_MAC_ADDR2INDEX_ADDR_32TO47_FIELD_MAC_ADDR2INDEX_ADDR_32TO47_MASK                        0x0000FFFF
#define B2_MAC_ADDR2INDEX_MAC_ADDR2INDEX_ADDR_32TO47_FIELD_REMOVE_DB_INDEX_MASK                                   0x00FF0000
//========================================
/* REG_MAC_ADDR2INDEX_MAC_ADDR2INDEX_INIT_PARAMS 0x40 */
#define B2_MAC_ADDR2INDEX_MAC_ADDR2INDEX_INIT_PARAMS_FIELD_FIRST_INDEX_MASK                                       0x000000FF
#define B2_MAC_ADDR2INDEX_MAC_ADDR2INDEX_INIT_PARAMS_FIELD_MAX_NUM_OF_INDEXES_MASK                                0x0001FF00
//========================================
/* REG_MAC_ADDR2INDEX_MAC_ADDR2INDEX_STATUS 0x44 */
#define B2_MAC_ADDR2INDEX_MAC_ADDR2INDEX_STATUS_FIELD_MAC_ADDR2INDEX_DONE_MASK                                    0x00000001
#define B2_MAC_ADDR2INDEX_MAC_ADDR2INDEX_STATUS_FIELD_MAC_ADDR2INDEX_FAIL_MASK                                    0x0000000E
#define B2_MAC_ADDR2INDEX_MAC_ADDR2INDEX_STATUS_FIELD_MAC_ADDR2INDEX_DB_INDEX_MASK                                0x0000FF00
#define B2_MAC_ADDR2INDEX_MAC_ADDR2INDEX_STATUS_FIELD_MAC_ADDR2INDEX_NUM_VALID_INDEXES_MASK                       0x01FF0000
#define B2_MAC_ADDR2INDEX_MAC_ADDR2INDEX_STATUS_FIELD_MAC_ADDR2INDEX_SM_0_MASK                                    0x0E000000
#define B2_MAC_ADDR2INDEX_MAC_ADDR2INDEX_STATUS_FIELD_MAC_ADDR2INDEX_SM_1_MASK                                    0x10000000
#define B2_MAC_ADDR2INDEX_MAC_ADDR2INDEX_STATUS_FIELD_MAC_ADDR2INDEX_ARB_SM_MASK                                  0xE0000000
//========================================
/* REG_MAC_ADDR2INDEX_MAC_ADDR2INDEX_VALID_STA_0TO31 0x48 */
#define B2_MAC_ADDR2INDEX_MAC_ADDR2INDEX_VALID_STA_0TO31_FIELD_MAC_ADDR2INDEX_VALID_STA_0TO31_MASK                0xFFFFFFFF
//========================================
/* REG_MAC_ADDR2INDEX_MAC_ADDR2INDEX_VALID_STA_32TO63 0x4C */
#define B2_MAC_ADDR2INDEX_MAC_ADDR2INDEX_VALID_STA_32TO63_FIELD_MAC_ADDR2INDEX_VALID_STA_32TO63_MASK              0xFFFFFFFF
//========================================
/* REG_MAC_ADDR2INDEX_MAC_ADDR2INDEX_VALID_STA_64TO95 0x50 */
#define B2_MAC_ADDR2INDEX_MAC_ADDR2INDEX_VALID_STA_64TO95_FIELD_MAC_ADDR2INDEX_VALID_STA_64TO95_MASK              0xFFFFFFFF
//========================================
/* REG_MAC_ADDR2INDEX_MAC_ADDR2INDEX_VALID_STA_96TO127 0x54 */
#define B2_MAC_ADDR2INDEX_MAC_ADDR2INDEX_VALID_STA_96TO127_FIELD_MAC_ADDR2INDEX_VALID_STA_96TO127_MASK            0xFFFFFFFF
//========================================
/* REG_MAC_ADDR2INDEX_MAC_ADDR2INDEX_STATUS_2 0x68 */
#define B2_MAC_ADDR2INDEX_MAC_ADDR2INDEX_STATUS_2_FIELD_A2I_NUM_OF_HW_CMD_FIFO_REACHED_FULL_MASK                  0x00000FFF
#define B2_MAC_ADDR2INDEX_MAC_ADDR2INDEX_STATUS_2_FIELD_A2I_HW_CMD_FIFO_OCCUPANCY_MAX_MASK                        0x000F0000
#define B2_MAC_ADDR2INDEX_MAC_ADDR2INDEX_STATUS_2_FIELD_A2I_HW_RESP_FIFO_OCCUPANCY_MAX_MASK                       0x01F00000
//========================================
/* REG_MAC_ADDR2INDEX_MAC_ADDR2INDEX_STATUS_2_CLR 0x6C */
#define B2_MAC_ADDR2INDEX_MAC_ADDR2INDEX_STATUS_2_CLR_FIELD_A2I_NUM_OF_HW_CMD_FIFO_REACHED_FULL_CLR_MASK          0x00000001
#define B2_MAC_ADDR2INDEX_MAC_ADDR2INDEX_STATUS_2_CLR_FIELD_A2I_HW_CMD_FIFO_OCCUPANCY_MAX_CLR_MASK                0x00000002
#define B2_MAC_ADDR2INDEX_MAC_ADDR2INDEX_STATUS_2_CLR_FIELD_A2I_HW_RESP_FIFO_OCCUPANCY_MAX_CLR_MASK               0x00000004
//========================================
/* REG_MAC_ADDR2INDEX_MAC_ADDR2INDEX_SPARE_REGISTER 0x70 */
#define B2_MAC_ADDR2INDEX_MAC_ADDR2INDEX_SPARE_REGISTER_FIELD_SPARE_REGISTER_MASK                                 0x0000FFFF


#endif // _MAC_ADDR2INDEX_REGS_H_
