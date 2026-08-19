
/***********************************************************************************
File:				MlDinQRegs.h
Module:				mlDinQ
SOC Revision:		latest
Generated at:       2024-06-26 12:55:07
Purpose:
Description:		This File was auto generated using SOC Online

************************************************************************************/
#ifndef _ML_DIN_Q_REGS_H_
#define _ML_DIN_Q_REGS_H_

/*---------------------------------------------------------------------------------
/						Registers Addresses													 
/----------------------------------------------------------------------------------*/
#include "HwMemoryMap.h"

#define ML_DIN_Q_BASE_ADDRESS           MEMORY_MAP_UNIT_67_BASE_ADDRESS
#define	REG_ML_DIN_Q_ADD_STA           (ML_DIN_Q_BASE_ADDRESS + 0x0)
#define	REG_ML_DIN_Q_REMOVE_STA        (ML_DIN_Q_BASE_ADDRESS + 0x4)
#define	REG_ML_DIN_Q_TID_MAP           (ML_DIN_Q_BASE_ADDRESS + 0x8)
#define	REG_ML_DIN_Q_VLD_FOR_CALC      (ML_DIN_Q_BASE_ADDRESS + 0xC)
#define	REG_ML_DIN_Q_DIS_MLD_0_31      (ML_DIN_Q_BASE_ADDRESS + 0x10)
#define	REG_ML_DIN_Q_DIS_MLD_32_63     (ML_DIN_Q_BASE_ADDRESS + 0x14)
#define	REG_ML_DIN_Q_DIS_MLD_64_95     (ML_DIN_Q_BASE_ADDRESS + 0x18)
#define	REG_ML_DIN_Q_DIS_MLD_96_127    (ML_DIN_Q_BASE_ADDRESS + 0x1C)
#define	REG_ML_DIN_Q_IRQ_STATUS        (ML_DIN_Q_BASE_ADDRESS + 0x20)
#define	REG_ML_DIN_Q_IRQ_CLR           (ML_DIN_Q_BASE_ADDRESS + 0x24)
#define	REG_ML_DIN_Q_IRQ_MASK          (ML_DIN_Q_BASE_ADDRESS + 0x28)
#define	REG_ML_DIN_Q_LOGGER_CFG        (ML_DIN_Q_BASE_ADDRESS + 0x2C)
#define	REG_ML_DIN_Q_LOGGER_BUSY       (ML_DIN_Q_BASE_ADDRESS + 0x30)
#define	REG_ML_DIN_Q_DEBUG             (ML_DIN_Q_BASE_ADDRESS + 0x40)
/*---------------------------------------------------------------------------------
/						Data Type Definition										
/----------------------------------------------------------------------------------*/
/*REG_ML_DIN_Q_ADD_STA 0x0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 addStaMldIdx : 7; //MLD index to add affiliated STA to, reset value: 0x0, access type: WO
		uint32 addStaBand : 2; //band ID of the added affiliated STA, reset value: 0x0, access type: WO
		uint32 addStaInbandStaId : 7; //inband STA ID (0..127), reset value: 0x0, access type: WO
		uint32 addStaTidBitMap : 9; //Mapped TIDs ('1' = 'mapped'), reset value: 0x0, access type: WO
		uint32 reserved0 : 7;
	} bitFields;
} RegMlDinQAddSta_u;

/*REG_ML_DIN_Q_REMOVE_STA 0x4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 removeStaMldIdx : 7; //MLD index to remove STA from, reset value: 0x0, access type: WO
		uint32 removeStaBand : 2; //Band ID of the STA being removed, reset value: 0x0, access type: WO
		uint32 reserved0 : 23;
	} bitFields;
} RegMlDinQRemoveSta_u;

/*REG_ML_DIN_Q_TID_MAP 0x8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 tidMapMldIdx : 7; //MLD index to change TID mapping for, reset value: 0x0, access type: WO
		uint32 tidMapTid : 4; //TID index (0..8) to change mapping for, reset value: 0x0, access type: WO
		uint32 tidMapIsMapped : 3; //bit per Link - where to set/reset the TID mapping. NOTE: each MLD must have at least 1 TID mapped., reset value: 0x0, access type: WO
		uint32 reserved0 : 18;
	} bitFields;
} RegMlDinQTidMap_u;

/*REG_ML_DIN_Q_VLD_FOR_CALC 0xC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 vldForCalcMldIdx : 7; //MLD index to change "valid for calculation" bitmap for, reset value: 0x0, access type: WO
		uint32 vldForCalcTid : 4; //TID index (0..8) to set "valid for calc." for, reset value: 0x0, access type: WO
		uint32 vldForCalcBitValue : 1; //Value of "valid for calc." bit, per TID, reset value: 0x0, access type: WO
		uint32 reserved0 : 20;
	} bitFields;
} RegMlDinQVldForCalc_u;

/*REG_ML_DIN_Q_DIS_MLD_0_31 0x10 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 disMld031 : 32; //"disabled MLD" bitmap, bits[31:0], reset value: 0xFFFFFFFF, access type: RW
	} bitFields;
} RegMlDinQDisMld031_u;

/*REG_ML_DIN_Q_DIS_MLD_32_63 0x14 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 disMld3263 : 32; //no description, reset value: 0xFFFFFFFF, access type: RW
	} bitFields;
} RegMlDinQDisMld3263_u;

/*REG_ML_DIN_Q_DIS_MLD_64_95 0x18 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 disMld6495 : 32; //no description, reset value: 0xFFFFFFFF, access type: RW
	} bitFields;
} RegMlDinQDisMld6495_u;

/*REG_ML_DIN_Q_DIS_MLD_96_127 0x1C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 disMld96127 : 32; //no description, reset value: 0xFFFFFFFF, access type: RW
	} bitFields;
} RegMlDinQDisMld96127_u;

/*REG_ML_DIN_Q_IRQ_STATUS 0x20 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mldTxqErr : 1; //d-in-q update was issued to MLD with TQX other than STA_TID (='0'), reset value: 0x0, access type: RO
		uint32 fifoOvfErr : 9; //sticky overflow bit per block's FIFO instance, reset value: 0x0, access type: RO
		uint32 tidMapErr : 1; //no valid TID mapping (fatal - requires reset)., reset value: 0x0, access type: RO
		uint32 reserved0 : 21;
	} bitFields;
} RegMlDinQIrqStatus_u;

/*REG_ML_DIN_Q_IRQ_CLR 0x24 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 fifoOvfClr : 1; //clears all FIFO's overflow sticky bits, reset value: 0x0, access type: WO
		uint32 mldTxqErrClr : 1; //clears TXQ type error sticky bit., reset value: 0x0, access type: WO
		uint32 tidMapErrClr : 1; //clears illegal TID map (=zeros) IRQ, reset value: 0x0, access type: WO
		uint32 reserved0 : 29;
	} bitFields;
} RegMlDinQIrqClr_u;

/*REG_ML_DIN_Q_IRQ_MASK 0x28 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 fifosOvfIrqMask : 1; //'1' = 'do not produce IRQ', reset value: 0x0, access type: RW
		uint32 mldTxqErrIrqMask : 1; //'1' = 'do not produce IRQ', reset value: 0x0, access type: RW
		uint32 tidMapErrIrqMask : 1; //'1' = 'do not produce IRQ', reset value: 0x0, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegMlDinQIrqMask_u;

/*REG_ML_DIN_Q_LOGGER_CFG 0x2C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 loggerEna : 1; //no description, reset value: 0x0, access type: RW
		uint32 loggerMsgEna : 3; //no description, reset value: 0x0, access type: RW
		uint32 loggerPrio : 2; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 26;
	} bitFields;
} RegMlDinQLoggerCfg_u;

/*REG_ML_DIN_Q_LOGGER_BUSY 0x30 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 loggerBusy : 1; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegMlDinQLoggerBusy_u;

/*REG_ML_DIN_Q_DEBUG 0x40 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dbgStop : 1; //disables the SM leaving the IDLE state., reset value: 0x0, access type: RW
		uint32 dbgSm : 3; //current SM state, reset value: 0x0, access type: RO
		uint32 dbgFifoFull : 9; //bit per FIFO instance, live indication, reset value: 0x0, access type: RO
		uint32 dbgFifoEmpty : 9; //bit per FIFO instance, live indication, reset value: 0x0, access type: RO
		uint32 reserved0 : 10;
	} bitFields;
} RegMlDinQDebug_u;

//========================================
/* REG_ML_DIN_Q_ADD_STA 0x0 */
#define ML_DIN_Q_ADD_STA_FIELD_ADD_STA_MLD_IDX_MASK                                                               0x0000007F
#define ML_DIN_Q_ADD_STA_FIELD_ADD_STA_BAND_MASK                                                                  0x00000180
#define ML_DIN_Q_ADD_STA_FIELD_ADD_STA_INBAND_STA_ID_MASK                                                         0x0000FE00
#define ML_DIN_Q_ADD_STA_FIELD_ADD_STA_TID_BIT_MAP_MASK                                                           0x01FF0000
//========================================
/* REG_ML_DIN_Q_REMOVE_STA 0x4 */
#define ML_DIN_Q_REMOVE_STA_FIELD_REMOVE_STA_MLD_IDX_MASK                                                         0x0000007F
#define ML_DIN_Q_REMOVE_STA_FIELD_REMOVE_STA_BAND_MASK                                                            0x00000180
//========================================
/* REG_ML_DIN_Q_TID_MAP 0x8 */
#define ML_DIN_Q_TID_MAP_FIELD_TID_MAP_MLD_IDX_MASK                                                               0x0000007F
#define ML_DIN_Q_TID_MAP_FIELD_TID_MAP_TID_MASK                                                                   0x00000780
#define ML_DIN_Q_TID_MAP_FIELD_TID_MAP_IS_MAPPED_MASK                                                             0x00003800
//========================================
/* REG_ML_DIN_Q_VLD_FOR_CALC 0xC */
#define ML_DIN_Q_VLD_FOR_CALC_FIELD_VLD_FOR_CALC_MLD_IDX_MASK                                                     0x0000007F
#define ML_DIN_Q_VLD_FOR_CALC_FIELD_VLD_FOR_CALC_TID_MASK                                                         0x00000780
#define ML_DIN_Q_VLD_FOR_CALC_FIELD_VLD_FOR_CALC_BIT_VALUE_MASK                                                   0x00000800
//========================================
/* REG_ML_DIN_Q_DIS_MLD_0_31 0x10 */
#define ML_DIN_Q_DIS_MLD_0_31_FIELD_DIS_MLD_0_31_MASK                                                             0xFFFFFFFF
//========================================
/* REG_ML_DIN_Q_DIS_MLD_32_63 0x14 */
#define ML_DIN_Q_DIS_MLD_32_63_FIELD_DIS_MLD_32_63_MASK                                                           0xFFFFFFFF
//========================================
/* REG_ML_DIN_Q_DIS_MLD_64_95 0x18 */
#define ML_DIN_Q_DIS_MLD_64_95_FIELD_DIS_MLD_64_95_MASK                                                           0xFFFFFFFF
//========================================
/* REG_ML_DIN_Q_DIS_MLD_96_127 0x1C */
#define ML_DIN_Q_DIS_MLD_96_127_FIELD_DIS_MLD_96_127_MASK                                                         0xFFFFFFFF
//========================================
/* REG_ML_DIN_Q_IRQ_STATUS 0x20 */
#define ML_DIN_Q_IRQ_STATUS_FIELD_MLD_TXQ_ERR_MASK                                                                0x00000001
#define ML_DIN_Q_IRQ_STATUS_FIELD_FIFO_OVF_ERR_MASK                                                               0x000003FE
#define ML_DIN_Q_IRQ_STATUS_FIELD_TID_MAP_ERR_MASK                                                                0x00000400
//========================================
/* REG_ML_DIN_Q_IRQ_CLR 0x24 */
#define ML_DIN_Q_IRQ_CLR_FIELD_FIFO_OVF_CLR_MASK                                                                  0x00000001
#define ML_DIN_Q_IRQ_CLR_FIELD_MLD_TXQ_ERR_CLR_MASK                                                               0x00000002
#define ML_DIN_Q_IRQ_CLR_FIELD_TID_MAP_ERR_CLR_MASK                                                               0x00000004
//========================================
/* REG_ML_DIN_Q_IRQ_MASK 0x28 */
#define ML_DIN_Q_IRQ_MASK_FIELD_FIFOS_OVF_IRQ_MASK_MASK                                                           0x00000001
#define ML_DIN_Q_IRQ_MASK_FIELD_MLD_TXQ_ERR_IRQ_MASK_MASK                                                         0x00000002
#define ML_DIN_Q_IRQ_MASK_FIELD_TID_MAP_ERR_IRQ_MASK_MASK                                                         0x00000004
//========================================
/* REG_ML_DIN_Q_LOGGER_CFG 0x2C */
#define ML_DIN_Q_LOGGER_CFG_FIELD_LOGGER_ENA_MASK                                                                 0x00000001
#define ML_DIN_Q_LOGGER_CFG_FIELD_LOGGER_MSG_ENA_MASK                                                             0x0000000E
#define ML_DIN_Q_LOGGER_CFG_FIELD_LOGGER_PRIO_MASK                                                                0x00000030
//========================================
/* REG_ML_DIN_Q_LOGGER_BUSY 0x30 */
#define ML_DIN_Q_LOGGER_BUSY_FIELD_LOGGER_BUSY_MASK                                                               0x00000001
//========================================
/* REG_ML_DIN_Q_DEBUG 0x40 */
#define ML_DIN_Q_DEBUG_FIELD_DBG_STOP_MASK                                                                        0x00000001
#define ML_DIN_Q_DEBUG_FIELD_DBG_SM_MASK                                                                          0x0000000E
#define ML_DIN_Q_DEBUG_FIELD_DBG_FIFO_FULL_MASK                                                                   0x00001FF0
#define ML_DIN_Q_DEBUG_FIELD_DBG_FIFO_EMPTY_MASK                                                                  0x003FE000


#endif // _ML_DIN_Q_REGS_H_
