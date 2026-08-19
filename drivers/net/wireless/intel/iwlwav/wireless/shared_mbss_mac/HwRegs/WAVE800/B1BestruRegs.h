
/***********************************************************************************
File:				B1BestruRegs.h
Module:				b1Bestru
SOC Revision:		latest
Generated at:       2024-06-26 12:54:35
Purpose:
Description:		This File was auto generated using SOC Online

************************************************************************************/
#ifndef _B1_BESTRU_REGS_H_
#define _B1_BESTRU_REGS_H_

/*---------------------------------------------------------------------------------
/						Registers Addresses													 
/----------------------------------------------------------------------------------*/
#include "HwMemoryMap.h"

#define B1_BESTRU_BASE_ADDRESS                  MEMORY_MAP_UNIT_10066_BASE_ADDRESS
#define	REG_B1_BESTRU_SW_CONFIG                 (B1_BESTRU_BASE_ADDRESS + 0x0)
#define	REG_B1_BESTRU_SW_REQUEST                (B1_BESTRU_BASE_ADDRESS + 0x4)
#define	REG_B1_BESTRU_SW_REQ_RU_MASK_L          (B1_BESTRU_BASE_ADDRESS + 0x8)
#define	REG_B1_BESTRU_SW_REQ_RU_MASK_H          (B1_BESTRU_BASE_ADDRESS + 0xC)
#define	REG_B1_BESTRU_SW_RESULT_RESP            (B1_BESTRU_BASE_ADDRESS + 0x10)
#define	REG_B1_BESTRU_SW_DBG                    (B1_BESTRU_BASE_ADDRESS + 0x14)
#define	REG_B1_BESTRU_SW_DBG_USR_VEC_L          (B1_BESTRU_BASE_ADDRESS + 0x18)
#define	REG_B1_BESTRU_SW_DBG_USR_VEC_H          (B1_BESTRU_BASE_ADDRESS + 0x1C)
#define	REG_B1_BESTRU_SW_DBG_CURR_PROC          (B1_BESTRU_BASE_ADDRESS + 0x20)
#define	REG_B1_BESTRU_SW_LUT1_MCS0              (B1_BESTRU_BASE_ADDRESS + 0x24)
#define	REG_B1_BESTRU_SW_LUT1_MCS1              (B1_BESTRU_BASE_ADDRESS + 0x28)
#define	REG_B1_BESTRU_SW_LUT1_MCS2              (B1_BESTRU_BASE_ADDRESS + 0x2C)
#define	REG_B1_BESTRU_SW_LUT1_MCS3              (B1_BESTRU_BASE_ADDRESS + 0x30)
#define	REG_B1_BESTRU_SW_LUT1_MCS4              (B1_BESTRU_BASE_ADDRESS + 0x34)
#define	REG_B1_BESTRU_SW_LUT1_MCS5              (B1_BESTRU_BASE_ADDRESS + 0x38)
#define	REG_B1_BESTRU_SW_LUT1_MCS6              (B1_BESTRU_BASE_ADDRESS + 0x3C)
#define	REG_B1_BESTRU_SW_LUT1_MCS7              (B1_BESTRU_BASE_ADDRESS + 0x40)
#define	REG_B1_BESTRU_SW_LUT1_MCS8              (B1_BESTRU_BASE_ADDRESS + 0x44)
#define	REG_B1_BESTRU_SW_LUT1_MCS9              (B1_BESTRU_BASE_ADDRESS + 0x48)
#define	REG_B1_BESTRU_SW_LUT1_MCS10             (B1_BESTRU_BASE_ADDRESS + 0x4C)
#define	REG_B1_BESTRU_SW_LUT1_MCS11             (B1_BESTRU_BASE_ADDRESS + 0x50)
#define	REG_B1_BESTRU_SW_LUT2_MCS0              (B1_BESTRU_BASE_ADDRESS + 0x54)
#define	REG_B1_BESTRU_SW_LUT2_MCS1              (B1_BESTRU_BASE_ADDRESS + 0x58)
#define	REG_B1_BESTRU_SW_LUT2_MCS2              (B1_BESTRU_BASE_ADDRESS + 0x5C)
#define	REG_B1_BESTRU_SW_LUT2_MCS3              (B1_BESTRU_BASE_ADDRESS + 0x60)
#define	REG_B1_BESTRU_SW_LUT2_MCS4              (B1_BESTRU_BASE_ADDRESS + 0x64)
#define	REG_B1_BESTRU_SW_LUT2_MCS5              (B1_BESTRU_BASE_ADDRESS + 0x68)
#define	REG_B1_BESTRU_SW_LUT2_MCS6              (B1_BESTRU_BASE_ADDRESS + 0x6C)
#define	REG_B1_BESTRU_SW_LUT2_MCS7              (B1_BESTRU_BASE_ADDRESS + 0x70)
#define	REG_B1_BESTRU_SW_LUT2_MCS8              (B1_BESTRU_BASE_ADDRESS + 0x74)
#define	REG_B1_BESTRU_SW_LUT2_MCS9              (B1_BESTRU_BASE_ADDRESS + 0x78)
#define	REG_B1_BESTRU_SW_LUT2_MCS10             (B1_BESTRU_BASE_ADDRESS + 0x7C)
#define	REG_B1_BESTRU_SW_LUT2_MCS11             (B1_BESTRU_BASE_ADDRESS + 0x80)
#define	REG_B1_BESTRU_SW_CALC_ASNR_DBG          (B1_BESTRU_BASE_ADDRESS + 0x84)
#define	REG_B1_BESTRU_SW_ASNR2MCS_LUT_SEL       (B1_BESTRU_BASE_ADDRESS + 0x88)
#define	REG_B1_BESTRU_SW_DBG_CALC_RES_L         (B1_BESTRU_BASE_ADDRESS + 0x8C)
#define	REG_B1_BESTRU_SW_DBG_CALC_RES_H         (B1_BESTRU_BASE_ADDRESS + 0x90)
#define	REG_B1_BESTRU_SW_DBG_FIFOS              (B1_BESTRU_BASE_ADDRESS + 0x94)
#define	REG_B1_BESTRU_BESTRU_LOGGER_CFG         (B1_BESTRU_BASE_ADDRESS + 0x98)
#define	REG_B1_BESTRU_BESTRU_LOGGER_BUSY        (B1_BESTRU_BASE_ADDRESS + 0x9C)
#define	REG_B1_BESTRU_SW_ABORT_CNT_RST_PULSE    (B1_BESTRU_BASE_ADDRESS + 0xA0)
/*---------------------------------------------------------------------------------
/						Data Type Definition										
/----------------------------------------------------------------------------------*/
/*REG_B1_BESTRU_SW_CONFIG 0x0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swBestruEna : 1; //"1" enables the block. "0" disables the block after finishing curr user., reset value: 0x0, access type: RW
		uint32 swTsfShft : 3; //# of bits to shift right TSF10 (then 5 LSB taken) , tsf5_stamp      <= 5'(tsf10_stamp >> sw_tsf_shft);, reset value: 0x0, access type: RW
		uint32 reserved0 : 28;
	} bitFields;
} RegB1BestruSwConfig_u;

/*REG_B1_BESTRU_SW_REQUEST 0x4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swReq : 1; //Requests BRU result according to the "mode" (next reg). , Poll sw_resp_ready to know when done., reset value: 0x0, access type: WO
		uint32 swReqMode : 2; //"0"=provide single BestRU result in SW regs. "1"=copy STA entry to mirror. "2"=invalidate HDR (set 0) of the specified STA (@ disconnect or outdated TSF). , , reset value: 0x0, access type: RW
		uint32 swReqStaId : 8; //Requested STA_ID. , reset value: 0x0, access type: RW
		uint32 swReqRuSizeIdx : 3; //RU size index for ---result_request--- mode. , (0 = RU26, 1 = RU52, 2 = RU106, 3 = RU242, 4 = RU484, 5 = RU996) , , reset value: 0x0, access type: RW
		uint32 reserved0 : 18;
	} bitFields;
} RegB1BestruSwRequest_u;

/*REG_B1_BESTRU_SW_REQ_RU_MASK_L 0x8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swReqRuMaskL : 32; //bit_X="1" means the RU_X is occupied and shall be excluded from bestRU search., reset value: 0x0, access type: RW
	} bitFields;
} RegB1BestruSwReqRuMaskL_u;

/*REG_B1_BESTRU_SW_REQ_RU_MASK_H 0xC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swReqRuMaskH : 4; //bit_X="1" means the RU_(X+32) is occupied and shall be excluded from bestRU search., reset value: 0x0, access type: RW
		uint32 reserved0 : 28;
	} bitFields;
} RegB1BestruSwReqRuMaskH_u;

/*REG_B1_BESTRU_SW_RESULT_RESP 0x10 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swRespReady : 1; //The result is ready to read. Poll this reg and check valid flag, next bit., reset value: 0x0, access type: RO
		uint32 swResultValid : 1; //If ---0---: STA DB entry is not valid (never written or reset @ STA disconnect)., reset value: 0x0, access type: RO
		uint32 swResultBw : 2; //Bandwidth of the reported STA. , (0 = 20MHz, 1 = 40MHz, 2 = 80MHz, 3 = 160MHz) , , reset value: 0x0, access type: RO
		uint32 swResultRuIdx : 6; //---Best--- RU index, allowing max rate, in the specific RU size tone plan., reset value: 0x0, access type: RO
		uint32 swResultMaxMcs : 4; //maxMCS related to the ---best--- RU., reset value: 0x0, access type: RO
		uint32 swResultMaxNss : 2; //maxNss related to the ---best--- RU., reset value: 0x0, access type: RO
		uint32 swResultTsf5 : 5; //Time-stamp of the last update to this STA entry., reset value: 0x0, access type: RO
		uint32 swBwRusizeError : 1; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 10;
	} bitFields;
} RegB1BestruSwResultResp_u;

/*REG_B1_BESTRU_SW_DBG 0x14 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mainSm : 2; //Main (top) state-machine, reset value: 0x0, access type: RO
		uint32 coreSm : 3; //Core state-machine (trig per user), reset value: 0x0, access type: RO
		uint32 swApiSm : 3; //SW API Server state-machine (trig by sw req), reset value: 0x0, access type: RO
		uint32 swApiBruSm : 2; //BestRU look-up state-machine, reset value: 0x0, access type: RO
		uint32 bfrRdSm : 2; //State of the BFR read logic block (SM), reset value: 0x0, access type: RO
		uint32 bruActive : 1; //"1" = the block isn't idle., reset value: 0x0, access type: RO
		uint32 swApiActive : 1; //"1" = SW request processing is active., reset value: 0x0, access type: RO
		uint32 swPending : 1; //this flag shows if there is a pending SW request, delayed cos of HW working on the same STA., reset value: 0x0, access type: RO
		uint32 hwPending : 1; //this flag shows if there is a pending HW process, delayed cos of copy the same STA entry for the SW., reset value: 0x0, access type: RO
		uint32 initAsnrSm : 2; //State of the ScratchPad initialization logic block (SM), reset value: 0x0, access type: RO
		uint32 abortCnt : 8; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 6;
	} bitFields;
} RegB1BestruSwDbg_u;

/*REG_B1_BESTRU_SW_DBG_USR_VEC_L 0x18 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swDbgUsrVecL : 32; //aSNR-to-maxMCS LUT configuration, reset value: 0x0, access type: RO
	} bitFields;
} RegB1BestruSwDbgUsrVecL_u;

/*REG_B1_BESTRU_SW_DBG_USR_VEC_H 0x1C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swDbgUsrVecH : 4; //aSNR-to-maxMCS LUT configuration, reset value: 0x0, access type: RO
		uint32 reserved0 : 28;
	} bitFields;
} RegB1BestruSwDbgUsrVecH_u;

/*REG_B1_BESTRU_SW_DBG_CURR_PROC 0x20 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swDbgCurrStaId : 8; //aSNR-to-maxMCS LUT configuration, reset value: 0x0, access type: RO
		uint32 swDbgCurrUserId : 6; //aSNR-to-maxMCS LUT configuration, reset value: 0x0, access type: RO
		uint32 reserved0 : 18;
	} bitFields;
} RegB1BestruSwDbgCurrProc_u;

/*REG_B1_BESTRU_SW_LUT1_MCS0 0x24 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swLut1Mcs0 : 32; //aSNR-to-maxMCS LUT configuration, reset value: 0x0, access type: RW
	} bitFields;
} RegB1BestruSwLut1Mcs0_u;

/*REG_B1_BESTRU_SW_LUT1_MCS1 0x28 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swLut1Mcs1 : 32; //aSNR-to-maxMCS LUT configuration, reset value: 0x0, access type: RW
	} bitFields;
} RegB1BestruSwLut1Mcs1_u;

/*REG_B1_BESTRU_SW_LUT1_MCS2 0x2C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swLut1Mcs2 : 32; //aSNR-to-maxMCS LUT configuration, reset value: 0x0, access type: RW
	} bitFields;
} RegB1BestruSwLut1Mcs2_u;

/*REG_B1_BESTRU_SW_LUT1_MCS3 0x30 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swLut1Mcs3 : 32; //aSNR-to-maxMCS LUT configuration, reset value: 0x0, access type: RW
	} bitFields;
} RegB1BestruSwLut1Mcs3_u;

/*REG_B1_BESTRU_SW_LUT1_MCS4 0x34 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swLut1Mcs4 : 32; //aSNR-to-maxMCS LUT configuration, reset value: 0x0, access type: RW
	} bitFields;
} RegB1BestruSwLut1Mcs4_u;

/*REG_B1_BESTRU_SW_LUT1_MCS5 0x38 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swLut1Mcs5 : 32; //aSNR-to-maxMCS LUT configuration, reset value: 0x0, access type: RW
	} bitFields;
} RegB1BestruSwLut1Mcs5_u;

/*REG_B1_BESTRU_SW_LUT1_MCS6 0x3C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swLut1Mcs6 : 32; //aSNR-to-maxMCS LUT configuration, reset value: 0x0, access type: RW
	} bitFields;
} RegB1BestruSwLut1Mcs6_u;

/*REG_B1_BESTRU_SW_LUT1_MCS7 0x40 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swLut1Mcs7 : 32; //aSNR-to-maxMCS LUT configuration, reset value: 0x0, access type: RW
	} bitFields;
} RegB1BestruSwLut1Mcs7_u;

/*REG_B1_BESTRU_SW_LUT1_MCS8 0x44 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swLut1Mcs8 : 32; //aSNR-to-maxMCS LUT configuration, reset value: 0x0, access type: RW
	} bitFields;
} RegB1BestruSwLut1Mcs8_u;

/*REG_B1_BESTRU_SW_LUT1_MCS9 0x48 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swLut1Mcs9 : 32; //aSNR-to-maxMCS LUT configuration, reset value: 0x0, access type: RW
	} bitFields;
} RegB1BestruSwLut1Mcs9_u;

/*REG_B1_BESTRU_SW_LUT1_MCS10 0x4C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swLut1Mcs10 : 32; //aSNR-to-maxMCS LUT configuration, reset value: 0x0, access type: RW
	} bitFields;
} RegB1BestruSwLut1Mcs10_u;

/*REG_B1_BESTRU_SW_LUT1_MCS11 0x50 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swLut1Mcs11 : 32; //aSNR-to-maxMCS LUT configuration, reset value: 0x0, access type: RW
	} bitFields;
} RegB1BestruSwLut1Mcs11_u;

/*REG_B1_BESTRU_SW_LUT2_MCS0 0x54 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swLut2Mcs0 : 32; //aSNR-to-maxMCS LUT configuration, reset value: 0x0, access type: RW
	} bitFields;
} RegB1BestruSwLut2Mcs0_u;

/*REG_B1_BESTRU_SW_LUT2_MCS1 0x58 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swLut2Mcs1 : 32; //aSNR-to-maxMCS LUT configuration, reset value: 0x0, access type: RW
	} bitFields;
} RegB1BestruSwLut2Mcs1_u;

/*REG_B1_BESTRU_SW_LUT2_MCS2 0x5C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swLut2Mcs2 : 32; //aSNR-to-maxMCS LUT configuration, reset value: 0x0, access type: RW
	} bitFields;
} RegB1BestruSwLut2Mcs2_u;

/*REG_B1_BESTRU_SW_LUT2_MCS3 0x60 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swLut2Mcs3 : 32; //aSNR-to-maxMCS LUT configuration, reset value: 0x0, access type: RW
	} bitFields;
} RegB1BestruSwLut2Mcs3_u;

/*REG_B1_BESTRU_SW_LUT2_MCS4 0x64 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swLut2Mcs4 : 32; //aSNR-to-maxMCS LUT configuration, reset value: 0x0, access type: RW
	} bitFields;
} RegB1BestruSwLut2Mcs4_u;

/*REG_B1_BESTRU_SW_LUT2_MCS5 0x68 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swLut2Mcs5 : 32; //aSNR-to-maxMCS LUT configuration, reset value: 0x0, access type: RW
	} bitFields;
} RegB1BestruSwLut2Mcs5_u;

/*REG_B1_BESTRU_SW_LUT2_MCS6 0x6C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swLut2Mcs6 : 32; //aSNR-to-maxMCS LUT configuration, reset value: 0x0, access type: RW
	} bitFields;
} RegB1BestruSwLut2Mcs6_u;

/*REG_B1_BESTRU_SW_LUT2_MCS7 0x70 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swLut2Mcs7 : 32; //aSNR-to-maxMCS LUT configuration, reset value: 0x0, access type: RW
	} bitFields;
} RegB1BestruSwLut2Mcs7_u;

/*REG_B1_BESTRU_SW_LUT2_MCS8 0x74 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swLut2Mcs8 : 32; //aSNR-to-maxMCS LUT configuration, reset value: 0x0, access type: RW
	} bitFields;
} RegB1BestruSwLut2Mcs8_u;

/*REG_B1_BESTRU_SW_LUT2_MCS9 0x78 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swLut2Mcs9 : 32; //aSNR-to-maxMCS LUT configuration, reset value: 0x0, access type: RW
	} bitFields;
} RegB1BestruSwLut2Mcs9_u;

/*REG_B1_BESTRU_SW_LUT2_MCS10 0x7C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swLut2Mcs10 : 32; //aSNR-to-maxMCS LUT configuration, reset value: 0x0, access type: RW
	} bitFields;
} RegB1BestruSwLut2Mcs10_u;

/*REG_B1_BESTRU_SW_LUT2_MCS11 0x80 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swLut2Mcs11 : 32; //aSNR-to-maxMCS LUT configuration, reset value: 0x0, access type: RW
	} bitFields;
} RegB1BestruSwLut2Mcs11_u;

/*REG_B1_BESTRU_SW_CALC_ASNR_DBG 0x84 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 calcAsnrSm : 2; //no description, reset value: 0x0, access type: RO
		uint32 ruRdCnt : 6; //no description, reset value: 0x0, access type: RO
		uint32 wrAddrCnt : 6; //no description, reset value: 0x0, access type: RO
		uint32 wrRuSizeCnt : 3; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 15;
	} bitFields;
} RegB1BestruSwCalcAsnrDbg_u;

/*REG_B1_BESTRU_SW_ASNR2MCS_LUT_SEL 0x88 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swAsnr2McsLutSel : 6; //Bit0 - LUT select for RU26 , Bit1 - LUT select for RU52 , Bit2 - LUT select for RU106 , Bit3 - LUT select for RU242 , Bit4 - LUT select for RU484 , Bit5 - LUT select for RU996, reset value: 0x0, access type: RW
		uint32 reserved0 : 26;
	} bitFields;
} RegB1BestruSwAsnr2McsLutSel_u;

/*REG_B1_BESTRU_SW_DBG_CALC_RES_L 0x8C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swDbgCalcResSm : 3; //no description, reset value: 0x0, access type: RO
		uint32 swDbgCalcResRuSizeCnt : 3; //no description, reset value: 0x0, access type: RO
		uint32 swDbgCalcResRuCnt : 6; //no description, reset value: 0x0, access type: RO
		uint32 swDbgCalcResWrCnt : 4; //no description, reset value: 0x0, access type: RO
		uint32 swDbgCalcResLastRdAddr : 6; //no description, reset value: 0x0, access type: RO
		uint32 swDbgCalcResScratchRdAddr : 6; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 4;
	} bitFields;
} RegB1BestruSwDbgCalcResL_u;

/*REG_B1_BESTRU_SW_DBG_CALC_RES_H 0x90 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swDbgCalcResDbWrAddr : 11; //no description, reset value: 0x0, access type: RO
		uint32 swDbgCalcResLowSnr : 1; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 20;
	} bitFields;
} RegB1BestruSwDbgCalcResH_u;

/*REG_B1_BESTRU_SW_DBG_FIFOS 0x94 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swDbgBfrRdFifo : 8; //{smc_ena,started,start_pending,was_pop_empty,was_push_full,full,empty}, reset value: 0x01, access type: RO
		uint32 swDbgCalcResFifo : 8; //{smc_ena,started,start_pending,was_pop_empty,was_push_full,full,empty}, reset value: 0x01, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegB1BestruSwDbgFifos_u;

/*REG_B1_BESTRU_BESTRU_LOGGER_CFG 0x98 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swLoggerEna : 1; //no description, reset value: 0x0, access type: RW
		uint32 swLoggerPriority : 2; //no description, reset value: 0x0, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB1BestruBestruLoggerCfg_u;

/*REG_B1_BESTRU_BESTRU_LOGGER_BUSY 0x9C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swLoggerBusy : 1; //no description, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB1BestruBestruLoggerBusy_u;

/*REG_B1_BESTRU_SW_ABORT_CNT_RST_PULSE 0xA0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 swAbortCntRstPulse : 1; //no description, reset value: 0x0, access type: WO
		uint32 reserved0 : 31;
	} bitFields;
} RegB1BestruSwAbortCntRstPulse_u;

//========================================
/* REG_BESTRU_SW_CONFIG 0x0 */
#define B1_BESTRU_SW_CONFIG_FIELD_SW_BESTRU_ENA_MASK                                                              0x00000001
#define B1_BESTRU_SW_CONFIG_FIELD_SW_TSF_SHFT_MASK                                                                0x0000000E
//========================================
/* REG_BESTRU_SW_REQUEST 0x4 */
#define B1_BESTRU_SW_REQUEST_FIELD_SW_REQ_MASK                                                                    0x00000001
#define B1_BESTRU_SW_REQUEST_FIELD_SW_REQ_MODE_MASK                                                               0x00000006
#define B1_BESTRU_SW_REQUEST_FIELD_SW_REQ_STA_ID_MASK                                                             0x000007F8
#define B1_BESTRU_SW_REQUEST_FIELD_SW_REQ_RU_SIZE_IDX_MASK                                                        0x00003800
//========================================
/* REG_BESTRU_SW_REQ_RU_MASK_L 0x8 */
#define B1_BESTRU_SW_REQ_RU_MASK_L_FIELD_SW_REQ_RU_MASK_L_MASK                                                    0xFFFFFFFF
//========================================
/* REG_BESTRU_SW_REQ_RU_MASK_H 0xC */
#define B1_BESTRU_SW_REQ_RU_MASK_H_FIELD_SW_REQ_RU_MASK_H_MASK                                                    0x0000000F
//========================================
/* REG_BESTRU_SW_RESULT_RESP 0x10 */
#define B1_BESTRU_SW_RESULT_RESP_FIELD_SW_RESP_READY_MASK                                                         0x00000001
#define B1_BESTRU_SW_RESULT_RESP_FIELD_SW_RESULT_VALID_MASK                                                       0x00000002
#define B1_BESTRU_SW_RESULT_RESP_FIELD_SW_RESULT_BW_MASK                                                          0x0000000C
#define B1_BESTRU_SW_RESULT_RESP_FIELD_SW_RESULT_RU_IDX_MASK                                                      0x000003F0
#define B1_BESTRU_SW_RESULT_RESP_FIELD_SW_RESULT_MAX_MCS_MASK                                                     0x00003C00
#define B1_BESTRU_SW_RESULT_RESP_FIELD_SW_RESULT_MAX_NSS_MASK                                                     0x0000C000
#define B1_BESTRU_SW_RESULT_RESP_FIELD_SW_RESULT_TSF5_MASK                                                        0x001F0000
#define B1_BESTRU_SW_RESULT_RESP_FIELD_SW_BW_RUSIZE_ERROR_MASK                                                    0x00200000
//========================================
/* REG_BESTRU_SW_DBG 0x14 */
#define B1_BESTRU_SW_DBG_FIELD_MAIN_SM_MASK                                                                       0x00000003
#define B1_BESTRU_SW_DBG_FIELD_CORE_SM_MASK                                                                       0x0000001C
#define B1_BESTRU_SW_DBG_FIELD_SW_API_SM_MASK                                                                     0x000000E0
#define B1_BESTRU_SW_DBG_FIELD_SW_API_BRU_SM_MASK                                                                 0x00000300
#define B1_BESTRU_SW_DBG_FIELD_BFR_RD_SM_MASK                                                                     0x00000C00
#define B1_BESTRU_SW_DBG_FIELD_BRU_ACTIVE_MASK                                                                    0x00001000
#define B1_BESTRU_SW_DBG_FIELD_SW_API_ACTIVE_MASK                                                                 0x00002000
#define B1_BESTRU_SW_DBG_FIELD_SW_PENDING_MASK                                                                    0x00004000
#define B1_BESTRU_SW_DBG_FIELD_HW_PENDING_MASK                                                                    0x00008000
#define B1_BESTRU_SW_DBG_FIELD_INIT_ASNR_SM_MASK                                                                  0x00030000
#define B1_BESTRU_SW_DBG_FIELD_ABORT_CNT_MASK                                                                     0x03FC0000
//========================================
/* REG_BESTRU_SW_DBG_USR_VEC_L 0x18 */
#define B1_BESTRU_SW_DBG_USR_VEC_L_FIELD_SW_DBG_USR_VEC_L_MASK                                                    0xFFFFFFFF
//========================================
/* REG_BESTRU_SW_DBG_USR_VEC_H 0x1C */
#define B1_BESTRU_SW_DBG_USR_VEC_H_FIELD_SW_DBG_USR_VEC_H_MASK                                                    0x0000000F
//========================================
/* REG_BESTRU_SW_DBG_CURR_PROC 0x20 */
#define B1_BESTRU_SW_DBG_CURR_PROC_FIELD_SW_DBG_CURR_STA_ID_MASK                                                  0x000000FF
#define B1_BESTRU_SW_DBG_CURR_PROC_FIELD_SW_DBG_CURR_USER_ID_MASK                                                 0x00003F00
//========================================
/* REG_BESTRU_SW_LUT1_MCS0 0x24 */
#define B1_BESTRU_SW_LUT1_MCS0_FIELD_SW_LUT1_MCS0_MASK                                                            0xFFFFFFFF
//========================================
/* REG_BESTRU_SW_LUT1_MCS1 0x28 */
#define B1_BESTRU_SW_LUT1_MCS1_FIELD_SW_LUT1_MCS1_MASK                                                            0xFFFFFFFF
//========================================
/* REG_BESTRU_SW_LUT1_MCS2 0x2C */
#define B1_BESTRU_SW_LUT1_MCS2_FIELD_SW_LUT1_MCS2_MASK                                                            0xFFFFFFFF
//========================================
/* REG_BESTRU_SW_LUT1_MCS3 0x30 */
#define B1_BESTRU_SW_LUT1_MCS3_FIELD_SW_LUT1_MCS3_MASK                                                            0xFFFFFFFF
//========================================
/* REG_BESTRU_SW_LUT1_MCS4 0x34 */
#define B1_BESTRU_SW_LUT1_MCS4_FIELD_SW_LUT1_MCS4_MASK                                                            0xFFFFFFFF
//========================================
/* REG_BESTRU_SW_LUT1_MCS5 0x38 */
#define B1_BESTRU_SW_LUT1_MCS5_FIELD_SW_LUT1_MCS5_MASK                                                            0xFFFFFFFF
//========================================
/* REG_BESTRU_SW_LUT1_MCS6 0x3C */
#define B1_BESTRU_SW_LUT1_MCS6_FIELD_SW_LUT1_MCS6_MASK                                                            0xFFFFFFFF
//========================================
/* REG_BESTRU_SW_LUT1_MCS7 0x40 */
#define B1_BESTRU_SW_LUT1_MCS7_FIELD_SW_LUT1_MCS7_MASK                                                            0xFFFFFFFF
//========================================
/* REG_BESTRU_SW_LUT1_MCS8 0x44 */
#define B1_BESTRU_SW_LUT1_MCS8_FIELD_SW_LUT1_MCS8_MASK                                                            0xFFFFFFFF
//========================================
/* REG_BESTRU_SW_LUT1_MCS9 0x48 */
#define B1_BESTRU_SW_LUT1_MCS9_FIELD_SW_LUT1_MCS9_MASK                                                            0xFFFFFFFF
//========================================
/* REG_BESTRU_SW_LUT1_MCS10 0x4C */
#define B1_BESTRU_SW_LUT1_MCS10_FIELD_SW_LUT1_MCS10_MASK                                                          0xFFFFFFFF
//========================================
/* REG_BESTRU_SW_LUT1_MCS11 0x50 */
#define B1_BESTRU_SW_LUT1_MCS11_FIELD_SW_LUT1_MCS11_MASK                                                          0xFFFFFFFF
//========================================
/* REG_BESTRU_SW_LUT2_MCS0 0x54 */
#define B1_BESTRU_SW_LUT2_MCS0_FIELD_SW_LUT2_MCS0_MASK                                                            0xFFFFFFFF
//========================================
/* REG_BESTRU_SW_LUT2_MCS1 0x58 */
#define B1_BESTRU_SW_LUT2_MCS1_FIELD_SW_LUT2_MCS1_MASK                                                            0xFFFFFFFF
//========================================
/* REG_BESTRU_SW_LUT2_MCS2 0x5C */
#define B1_BESTRU_SW_LUT2_MCS2_FIELD_SW_LUT2_MCS2_MASK                                                            0xFFFFFFFF
//========================================
/* REG_BESTRU_SW_LUT2_MCS3 0x60 */
#define B1_BESTRU_SW_LUT2_MCS3_FIELD_SW_LUT2_MCS3_MASK                                                            0xFFFFFFFF
//========================================
/* REG_BESTRU_SW_LUT2_MCS4 0x64 */
#define B1_BESTRU_SW_LUT2_MCS4_FIELD_SW_LUT2_MCS4_MASK                                                            0xFFFFFFFF
//========================================
/* REG_BESTRU_SW_LUT2_MCS5 0x68 */
#define B1_BESTRU_SW_LUT2_MCS5_FIELD_SW_LUT2_MCS5_MASK                                                            0xFFFFFFFF
//========================================
/* REG_BESTRU_SW_LUT2_MCS6 0x6C */
#define B1_BESTRU_SW_LUT2_MCS6_FIELD_SW_LUT2_MCS6_MASK                                                            0xFFFFFFFF
//========================================
/* REG_BESTRU_SW_LUT2_MCS7 0x70 */
#define B1_BESTRU_SW_LUT2_MCS7_FIELD_SW_LUT2_MCS7_MASK                                                            0xFFFFFFFF
//========================================
/* REG_BESTRU_SW_LUT2_MCS8 0x74 */
#define B1_BESTRU_SW_LUT2_MCS8_FIELD_SW_LUT2_MCS8_MASK                                                            0xFFFFFFFF
//========================================
/* REG_BESTRU_SW_LUT2_MCS9 0x78 */
#define B1_BESTRU_SW_LUT2_MCS9_FIELD_SW_LUT2_MCS9_MASK                                                            0xFFFFFFFF
//========================================
/* REG_BESTRU_SW_LUT2_MCS10 0x7C */
#define B1_BESTRU_SW_LUT2_MCS10_FIELD_SW_LUT2_MCS10_MASK                                                          0xFFFFFFFF
//========================================
/* REG_BESTRU_SW_LUT2_MCS11 0x80 */
#define B1_BESTRU_SW_LUT2_MCS11_FIELD_SW_LUT2_MCS11_MASK                                                          0xFFFFFFFF
//========================================
/* REG_BESTRU_SW_CALC_ASNR_DBG 0x84 */
#define B1_BESTRU_SW_CALC_ASNR_DBG_FIELD_CALC_ASNR_SM_MASK                                                        0x00000003
#define B1_BESTRU_SW_CALC_ASNR_DBG_FIELD_RU_RD_CNT_MASK                                                           0x000000FC
#define B1_BESTRU_SW_CALC_ASNR_DBG_FIELD_WR_ADDR_CNT_MASK                                                         0x00003F00
#define B1_BESTRU_SW_CALC_ASNR_DBG_FIELD_WR_RU_SIZE_CNT_MASK                                                      0x0001C000
//========================================
/* REG_BESTRU_SW_ASNR2MCS_LUT_SEL 0x88 */
#define B1_BESTRU_SW_ASNR2MCS_LUT_SEL_FIELD_SW_ASNR2MCS_LUT_SEL_MASK                                              0x0000003F
//========================================
/* REG_BESTRU_SW_DBG_CALC_RES_L 0x8C */
#define B1_BESTRU_SW_DBG_CALC_RES_L_FIELD_SW_DBG_CALC_RES_SM_MASK                                                 0x00000007
#define B1_BESTRU_SW_DBG_CALC_RES_L_FIELD_SW_DBG_CALC_RES_RU_SIZE_CNT_MASK                                        0x00000038
#define B1_BESTRU_SW_DBG_CALC_RES_L_FIELD_SW_DBG_CALC_RES_RU_CNT_MASK                                             0x00000FC0
#define B1_BESTRU_SW_DBG_CALC_RES_L_FIELD_SW_DBG_CALC_RES_WR_CNT_MASK                                             0x0000F000
#define B1_BESTRU_SW_DBG_CALC_RES_L_FIELD_SW_DBG_CALC_RES_LAST_RD_ADDR_MASK                                       0x003F0000
#define B1_BESTRU_SW_DBG_CALC_RES_L_FIELD_SW_DBG_CALC_RES_SCRATCH_RD_ADDR_MASK                                    0x0FC00000
//========================================
/* REG_BESTRU_SW_DBG_CALC_RES_H 0x90 */
#define B1_BESTRU_SW_DBG_CALC_RES_H_FIELD_SW_DBG_CALC_RES_DB_WR_ADDR_MASK                                         0x000007FF
#define B1_BESTRU_SW_DBG_CALC_RES_H_FIELD_SW_DBG_CALC_RES_LOW_SNR_MASK                                            0x00000800
//========================================
/* REG_BESTRU_SW_DBG_FIFOS 0x94 */
#define B1_BESTRU_SW_DBG_FIFOS_FIELD_SW_DBG_BFR_RD_FIFO_MASK                                                      0x000000FF
#define B1_BESTRU_SW_DBG_FIFOS_FIELD_SW_DBG_CALC_RES_FIFO_MASK                                                    0x0000FF00
//========================================
/* REG_BESTRU_BESTRU_LOGGER_CFG 0x98 */
#define B1_BESTRU_BESTRU_LOGGER_CFG_FIELD_SW_LOGGER_ENA_MASK                                                      0x00000001
#define B1_BESTRU_BESTRU_LOGGER_CFG_FIELD_SW_LOGGER_PRIORITY_MASK                                                 0x00000006
//========================================
/* REG_BESTRU_BESTRU_LOGGER_BUSY 0x9C */
#define B1_BESTRU_BESTRU_LOGGER_BUSY_FIELD_SW_LOGGER_BUSY_MASK                                                    0x00000001
//========================================
/* REG_BESTRU_SW_ABORT_CNT_RST_PULSE 0xA0 */
#define B1_BESTRU_SW_ABORT_CNT_RST_PULSE_FIELD_SW_ABORT_CNT_RST_PULSE_MASK                                        0x00000001


#endif // _BESTRU_REGS_H_
