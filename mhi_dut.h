/******************************************************************************

                               Copyright (c) 2012
                            Lantiq Deutschland GmbH

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/
/***************************************************************
 File:	mhi_dut.h
 Module:	Dut
 Purpose: 	
 Description: 
***************************************************************/
#ifndef __MHI_DUT_INCLUDED_H
#define __MHI_DUT_INCLUDED_H
//---------------------------------------------------------------------------------
//						Includes									
//---------------------------------------------------------------------------------
#include "mhi_ieee_address.h"
#include "mhi_global_definitions.h"
#include "mhi_hdk.h"

#define   MTLK_PACK_ON
#include "mtlkpack.h"

//---------------------------------------------------------------------------------
//						Defines									
//---------------------------------------------------------------------------------
#define TPC_ARRAY_SIZE					(3)
#define DUT_3ANT_ARRAY					(3)
#define DUT_4ANT_ARRAY					(4)
#define DUT_RXANT_ARRAY  				(5)


#define DUT_NUMBER_OF_CROSSING_POINTS	(7)

#define BIST_RESULTS_ARRAY_SIZE			(7) 

#define UMI_DEBUG_BLOCK_SIZE            (384)
#define UMI_C100_DATA_SIZE   			(UMI_DEBUG_BLOCK_SIZE - (sizeof(uint16) + sizeof(uint16) + sizeof(C100_MSG_HEADER)))
#define UMI_DEBUG_DATA_SIZE  			(UMI_DEBUG_BLOCK_SIZE - (sizeof(uint16) + sizeof(uint16)))
#define DUT_MSG_HEADER_LENGTH			(8)
#define DUT_MSG_DATA_LENGTH				(504)	
#define DUT_MSG_DATA_LENGTH_IN_WORDS	(DUT_MSG_DATA_LENGTH/4)	
#define MAX_DUT_EEPROM_ACCESS_SIZE 		(DUT_MSG_DATA_LENGTH-20)
#define DUT_MSG_DRIVER_FW_GENERAL_HEADER_LENGTH	(4*sizeof(uint32))

// Efuse power limits
#define EFUSE_POWER_LIMITS_START_ADDRESS 		(0x41)
#define EFUSE_POWER_LIMITS_CIS_HEADER_OFFSET 	(2)
#define EFUSE_POWER_LIMITS_FLIPPING_CONSTANT 	(128)
#define EFUSE_POWER_LIMITS_VERSION_LENGTH	 	(1)
#define EFUSE_POWER_LIMITS_VERSION			 	(1)	// Must be equal to the value which is written in eFuse


#define RFIC_NUM_OF_SHORT_REGS_WRX600B	4
#define RFIC_NUM_OF_LONG_REGS_WRX600B   64
#define RFIC_NUM_OF_SHORT_REGS_WRX600D2	4
#define RFIC_NUM_OF_LONG_REGS_WRX600D2  58
#define RFIC_NUM_OF_SHORT_REGS_WRX700	0
#define RFIC_NUM_OF_LONG_REGS_WRX700  128

#define DPD_TOTAL_CALIBRATION_POINTS (15) //This value needs to be aligned with TOTAL_CALIBRATION_POINTS in DpdDefinitions.h

/* BF matrix header masks - Jira WLANRTSYS-53879 */
#define BF_HEADER_PACKET_FORMAT_MASK		0x00000007		// Bits 2:0 - PHY mode
#define WAVE600_BF_HEADER_HT_BW_MASK		0x00100000		// Bit 20 for HT mode
#define WAVE600_BF_HEADER_VHT_HE_BW_MASK	0x00C00000		// Bits 23:22 for VHT/HE modes
#define WAVE700_BF_HEADER_BW_RU_MASK		0x01E00000		// Bits 24:21 - Bandwidth/RU

typedef enum dutChipModule
{
	DUT_CHIP_MODULE_UMAC_MEM	= 1,
	DUT_CHIP_MODULE_LMAC_MEM,
	DUT_CHIP_MODULE_PHY,
	DUT_CHIP_MODULE_RF,
	DUT_CHIP_MODULE_AFE,
	DUT_CHIP_MODULE_REGISTER
} dutChipModule_e;

typedef enum dutNvMemoryType
{
	DUT_NV_MEMORY_EEPROM		= 1,
	DUT_NV_MEMORY_FLASH			= 2,
	DUT_NV_MEMORY_EFUSE			= 3,
} dutNvMemoryType_e;

typedef enum dutChipVar
{
	DUT_CHIP_VAR_TPC_FREQ		= 31,

} dutChipVar_e;

typedef enum dutRiscMode
{
	DUT_RISC_STOP  = 0,
	DUT_RISC_START,
	DUT_RISC_LAST
} dutRiscMode_e;

typedef enum dutBand
{
	DUT_BAND_5000MHZ = 0,
	DUT_BAND_2400MHZ = 1,
	DUT_BAND_6000MHZ = 2,
	DUT_BAND_INVALID = 0xff
}dutBand_e;

typedef struct dutAddVap
{
	uint8 vapIndex;		/* vapNumber_e */
} dutAddVap_t;

typedef struct dutRemoveVap
{
	uint8 vapIndex;		/* vapNumber_e */
} dutRemoveVap_t;

typedef enum dutCalibrationType
{
	DUT_CALIBRATION_TYPE_ONLINE,
	DUT_CALIBRATION_TYPE_OFFLINE,
	DUT_CALIBRATION_TYPE_INVALID
} dutCalibrationType_e;

typedef enum dutCalibrationMaskType
{
	DUT_CALIBRATION_MASK_TYPE_DEFAULT,
	DUT_CALIBRATION_MASK_TYPE_CONFIGURABLE
} dutCalibrationMaskType_e;

typedef enum calibrationReturnStatusType
{
	CALIBRATION_RETURN_STATUS_OK,
	CALIBRATION_RETURN_STATUS_CHANNEL_NOT_SET,
	CALIBRATION_RETURN_STATUS_CALIBRATION_IN_PROCESS,
} calibrationReturnStatusType_e;

typedef enum bandRadioType
{
	RADIO0,
	RADIO1
} bandRadioType_e;

typedef enum cvPrefixIndex
{
	/*byte->version name conversion*/
	CV_PREFIX_CV610 = 0, //"06.01.00"
	CV_PREFIX_CV610_ER2 = 1,  //"06.01.00_ER2"
	CV_PREFIX_CV610_ER4 = 2,  //"06.01.00_ER4"
	CV_PREFIX_CV610_ER4_FR1 = 3, //"06.01.00_ER4.FR1"
	CV_PREFIX_CV610_ER5 = 4, //"06.01.00_ER5"
	CV_PREFIX_CV610_ER6 = 5, //"06.01.00_ER6"
	CV_PREFIX_CV610_ER7 = 6, //"06.01.00_ER8"
	CV_PREFIX_CV610_REL20 = 7, //"06.01.00_REL2.0"
	CV_PREFIX_CV610_REL30_DPHY = 8, //"06.01.00_REL3.0_dPhy"
	CV_PREFIX_MAX = 9
}cvPrefixIndex_e;

typedef enum dutDebugInfoType
{
	DUT_DEBUGINFO_DEFAULT,
	DUT_DEBUGINFO_FWASSERT,
	DUT_DEBUGINFO_EXTENSION
} dutDebugInfoType_e;


//---------------------------------------------
//		Dut Message Definition									
//---------------------------------------------
typedef enum{
	// Dut Mngmnt Module Messages
	DUT_HW_DEPENDENT_CONFIG_REQ = 0x02,
	DUT_HW_DEPENDENT_CONFIG_CFM,
	DUT_CONFIGURE_REQ,
	DUT_CONFIGURE_CFM,
	DUT_SET_CHANNEL_REQ,
	DUT_SET_CHANNEL_CFM,
	DUT_READ_EEPROM_REQ,
	DUT_READ_EEPROM_CFM,
	DUT_WRITE_EEPROM_REQ,// = 0x0A
	DUT_WRITE_EEPROM_CFM,
	DUT_READ_BIST_RESULTS_REQ,
	DUT_READ_BIST_RESULTS_CFM,
	DUT_READ_MEMORY_REQ,
	DUT_READ_MEMORY_CFM,
	DUT_WRITE_MEMORY_REQ,// = 0x10
	DUT_WRITE_MEMORY_CFM,
	DUT_WRITE_SECURED_REGISTER_REQ,
	DUT_WRITE_SECURED_REGISTER_CFM,
	DUT_READ_FREQ_TPC_STRUCTS_REQ,
	DUT_READ_FREQ_TPC_STRUCTS_CFM,
	DUT_START_CALIBRATION_REQ,		   
	DUT_START_CALIBRATION_CFM,   
	DUT_RUN_CALIBRATION_REQ,
	DUT_RUN_CALIBRATION_CFM,
	DUT_STOP_FW_REQ,// = 0x1A
	DUT_STOP_FW_CFM,
	DUT_SET_TPC_ANT_PARAMS_REQ,
	DUT_SET_TPC_ANT_PARAMS_CFM,
	DUT_SET_TPC_CONFIG_REQ,
	DUT_SET_TPC_CONFIG_CFM,
	// Dut Hw Acces Module Messages
	DUT_READ_CHIP_VERSION_REQ,// = 0x20
	DUT_READ_CHIP_VERSION_CFM,
	DUT_READ_GENRISC_VERSION_REQ ,	  		
	DUT_READ_GENRISC_VERSION_CFM,
	DUT_ENABLE_TX_ANTENNA_REQ,		
	DUT_ENABLE_TX_ANTENNA_CFM,
	DUT_ENABLE_RX_ANTENNA_REQ,
	DUT_ENABLE_RX_ANTENNA_CFM,
	DUT_SET_DEFAULT_ANTENNA_SET_REQ,	
	DUT_SET_DEFAULT_ANTENNA_SET_CFM,
	DUT_TX_TONE_REQ,// = 0x2A
	DUT_TX_TONE_CFM,
	DUT_TX_SPACELESS_REQ,
	DUT_TX_SPACELESS_CFM,
	DUT_SET_SCRAMBLER_MODE_REQ,
	DUT_SET_SCRAMBLER_MODE_CFM,
	DUT_SET_IFS_REQ,// = 0x30
	DUT_SET_IFS_CFM,
	DUT_SET_POWER_OUT_VEC_REQ,
	DUT_SET_POWER_OUT_VEC_CFM,
	DUT_GET_POWER_OUT_VEC_REQ,
	DUT_GET_POWER_OUT_VEC_CFM,
	DUT_READ_PACKET_COUNTERS_REQ,
	DUT_READ_PACKET_COUNTERS_CFM,
    DUT_SET_TX_POWER_LIMIT_REQ,
    DUT_SET_TX_POWER_LIMIT_CFM,
	DUT_SET_RISC_MODE_REQ,// = 0x3A
	DUT_SET_RISC_MODE_CFM,
	DUT_GET_RX_EVM_REQ,
	DUT_GET_RX_EVM_CFM,
	//RFIC
	DUT_GET_TX_POWER_FEEDBACK_REQ,
	DUT_GET_TX_POWER_FEEDBACK_CFM,
	DUT_GET_RX_GAINS_REQ,// = 0x40
	DUT_GET_RX_GAINS_CFM,
	DUT_GET_XTAL_VALUE_REQ,
	DUT_GET_XTAL_VALUE_CFM,
	DUT_SET_XTAL_VALUE_REQ,
	DUT_SET_XTAL_VALUE_CFM,
	DUT_GET_RSSI_REQ,
	DUT_GET_RSSI_CFM,
	DUT_GET_TEMPERATURE_SENSOR_REQ,
	DUT_GET_TEMPERATURE_SENSOR_CFM,
	DUT_SET_RFM_MODE_REQ,// = 0x4A
	DUT_SET_RFM_MODE_CFM,
	DUT_SET_RF_LOOPBACK_REQ,
	DUT_SET_RF_LOOPBACK_CFM,
	DUT_SET_GIPO_OUTPUT_LEVEL_REQ,
	DUT_SET_GIPO_OUTPUT_LEVEL_CFM,
	DUT_CANCEL_TX_PHASE_REQ,// = 0x50
	DUT_CANCEL_TX_PHASE_CFM,
	DUT_SET_CDD_SET_REQ,
	DUT_SET_CDD_SET_CFM,
	DUT_SET_QBF_REQ,
	DUT_SET_QBF_CFM,
	DUT_GET_LNA_PARAMS_REQ,
	DUT_GET_LNA_PARAMS_CFM,
	DUT_SET_LNA_PARAMS_REQ,
	DUT_SET_LNA_PARAMS_CFM,
	DUT_SET_RX_COUNTER_REQ,// = 0x5A
	DUT_SET_RX_COUNTER_CFM,
	DUT_GET_RX_COUNTER_REQ,
	DUT_GET_RX_COUNTER_CFM,
	DUT_SET_TX_BOOST_REQ,
	DUT_SET_TX_BOOST_CFM,
	DUT_LOGGER_SET_SEVERITY_REQ,// = 0x60
	DUT_LOGGER_SET_SEVERITY_CFM,
	DUT_LOGGER_SET_FILTER_REQ,
	DUT_LOGGER_SET_FILTER_CFM,
	DUT_LOGGER_FLUSH_BUF_REQ,
	DUT_LOGGER_FLUSH_BUF_CFM,
	DUT_SET_RATE_REQ,
	DUT_SET_RATE_CFM,
	DUT_GET_FW_INFO_REQ,
	DUT_GET_FW_INFO_CFM,
	DUT_SET_POWER_AND_PACKET_LENGTH_REQ,// = 0x6A
	DUT_SET_POWER_AND_PACKET_LENGTH_CFM,
	DUT_SET_S2D_GAIN_AND_OFFSET_REQ,
	DUT_SET_S2D_GAIN_AND_OFFSET_CFM,
	DUT_GET_RATE_REQ,
	DUT_GET_RATE_CFM,
	// RSSI 
	DUT_SET_RSSI_GAIN_BLOCK_REQ,// = 0x70
	DUT_SET_RSSI_GAIN_BLOCK_CFM,
	DUT_GET_CW_POWER_REQ,
	DUT_GET_CW_POWER_CFM,
	DUT_SET_RSSI_S2D_GAIN_AND_OFFSET_REQ,
	DUT_SET_RSSI_S2D_GAIN_AND_OFFSET_CFM,
	DUT_GET_AUX_ADC_RSSI_REQ,
	DUT_GET_AUX_ADC_RSSI_CFM,
	DUT_GET_RSSI_DB_REQ,
	DUT_GET_RSSI_DB_CFM,
	DUT_GET_CLIPPING_REQ,// = 0x7A
	DUT_GET_CLIPPING_CFM,
	// TPC
	DUT_SHIFT_POWER_OUT_VECTOR_REQ,
	DUT_SHIFT_POWER_OUT_VECTOR_CFM,
	DUT_GET_TX_GAINS_REQ,
	DUT_GET_TX_GAINS_CFM,
	DUT_START_RSSI_CALIBRATION_REQ,// = 0x80
	DUT_START_RSSI_CALIBRATION_CFM,
	DUT_END_RSSI_CALIBRATION_REQ,
	DUT_END_RSSI_CALIBRATION_CFM,	
	DUT_SET_LNA_MID_GAIN_REQ,
	DUT_SET_LNA_MID_GAIN_CFM,
	DUT_SET_RX_BAND_REQ,
	DUT_SET_RX_BAND_CFM,
	DUT_SET_RX_BAND_LUT_REQ,
	DUT_SET_RX_BAND_LUT_CFM,
	DUT_GET_POUT_TABLE_OFFSET_REQ,// = 0x8A
	DUT_GET_POUT_TABLE_OFFSET_CFM,
	DUT_SET_POUT_TABLE_OFFSET_REQ,
	DUT_SET_POUT_TABLE_OFFSET_CFM,
	DUT_ENABLE_RX_AGGREGATE_REQ,
	DUT_ENABLE_RX_AGGREGATE_CFM,
	DUT_SET_TX_BAND_REQ,// = 0x90
	DUT_SET_TX_BAND_CFM,
	DUT_SET_TX_BAND_LUT_REQ,
	DUT_SET_TX_BAND_LUT_CFM,
	DUT_GET_BBIC_CDD_REQ,
	DUT_GET_BBIC_CDD_CFM,
	DUT_SET_BBIC_CDD_REQ,
	DUT_SET_BBIC_CDD_CFM,
	DUT_GET_RFIC_CHANNEL_COUNT_REQ,
	DUT_GET_RFIC_CHANNEL_COUNT_CFM,
	DUT_GET_RFIC_REGISTERS_REQ,// = 0x9A
	DUT_GET_RFIC_REGISTERS_CFM,
	DUT_GET_RF_RSSI_POWER_REQ,
	DUT_GET_RF_RSSI_POWER_CFM,
	DUT_GET_FEM_TYPE_REQ,
	DUT_GET_FEM_TYPE_CFM,
	DUT_GET_DPD_NMSE_MEASURES_REQ,// = 0xA0
	DUT_GET_DPD_NMSE_MEASURES_CFM,
	DUT_SET_RU_PARAMS_REQ,
	DUT_SET_RU_PARAMS_CFM,
	DUT_RX_PACKET_LIMIT_REQ,
	DUT_RX_PACKET_LIMIT_CFM,
	DUT_RX_PER_COUNTERS_REQ,
	DUT_RX_PER_COUNTERS_CFM,
	DUT_RX_MEASURE_PARAMS_REQ,
	DUT_RX_MEASURE_PARAMS_CFM,
	DUT_GET_RX_RATE_REQ,
	DUT_GET_RX_RATE_CFM,
	DUT_SET_CLIPPER_REQ,
	DUT_SET_CLIPPER_CFM,
	// Dut Tx Module Messages
	DUT_START_TX_REQ	= 0x200,
	DUT_START_TX_CFM,
	DUT_STOP_TX_REQ,
	DUT_STOP_TX_CFM,
	DUT_START_TX_VECTOR_REQ,
	DUT_START_TX_VECTOR_CFM,
	DUT_GET_TX_VECTOR_ANT_REQ,
	DUT_GET_TX_VECTOR_ANT_CFM,


	DUT_UNKNOWN_MSG = 0x300,
	DUT_MAX_VAL = 0xFFFF,
} dutMessagesId_e;



typedef enum dutDriverMessagesIdEnum_t
{
	DUT_SERVER_MSG_RESET_MAC			= 0x01,
	DUT_SERVER_MSG_UPLOAD_PROGMODEL		= 0x02,
	DUT_SERVER_MSG_MAC_C100				= 0x03,
	DUT_SERVER_MSG_RESEND_NOT_IN_USE	= 0x04,
	DUT_SERVER_MSG_DRIVER_GENERAL		= 0x05,
	DUT_SERVER_MSG_ADD_VAP		 		= 0x06,
	DUT_SERVER_MSG_REMOVE_VAP		 	= 0x07,	
	DUT_SERVER_MSG_SET_BSS			 	= 0x08,	
	DUT_SERVER_MSG_SET_WMM_PARAMETERS 	= 0x09,	
	DUT_SERVER_MSG_STOP_VAP_TRAFFIC	 	= 0x0A,
	DUT_SERVER_MSG_DRIVER_FW_GENERAL	= 0x0B,	
	DUT_SERVER_MSG_PLATFORM_DATA_FIELDS = 0x0C,
 	DUT_SERVER_MSG_PLATFORM_GENERAL     = 0x0D,
	DUT_SERVER_MSG_CNT                  = 0x0E
} dutDriverMessagesId_e;


typedef enum
{
	// DGM - driver general message
	DUT_DGM_READ_NV_MEMORY_REQ = 0x2,
	DUT_DGM_READ_NV_MEMORY_CFM,
	DUT_DGM_WRITE_NV_MEMORY_REQ,
	DUT_DGM_WRITE_NV_MEMORY_CFM,
	DUT_DGM_FLUSH_NV_MEMORY_REQ,
	DUT_DGM_FLUSH_NV_MEMORY_CFM,
	DUT_DGM_GET_NV_FLASH_NAME_REQ,
	DUT_DGM_GET_NV_FLASH_NAME_CFM,
	DUT_DGM_CHANGE_BAND_REQ,
	DUT_DGM_CHANGE_BAND_CFM,
	DUT_DGM_RECOVERY_REQ
} dutDriverGeneralMsgId_e;

typedef enum
{
	/* PGM - platform general message */
	DUT_PGM_GET_CV_REQ = 0x2,
	DUT_PGM_GET_CV_CFM,
	DUT_PGM_COLLECT_DEBUG_INFO_REQ,
	DUT_PGM_COLLECT_DEBUG_INFO_CFM,
} dutPlatformGeneralMsgId_e;

typedef enum
{
	DUT_STATUS_PASS		= 1,
	DUT_STATUS_FAIL     = 2,
	DUT_STATUS_TX_ALREADY_STOPPED = 3,
	DUT_STATUS_TX_STOP_IS_ONGOING = 4,
	DUT_STATUS_TX_START_IS_ONGOING = 5,
	DUT_STATUS_MAX_VAL = MAX_UINT16,
} dutStatus_e ;

typedef enum counterType
{
	DUT_PHY_COUNTER,
	DUT_MPDU_COUNTER
} counterType_e;

//---------------------------------------------------------------------------------
//						Data Type Definition									
//---------------------------------------------------------------------------------

/*DUT_SERVER_MSG_DRIVER_FW_GENERAL*/
typedef struct dutDriverFwGeneralMsg
{
	uint32 umiReqId;
	uint32 umiLen;
	uint32 reserved0;
	uint32 reserved1;
	uint8  param[MTLK_PAD4(DUT_MSG_DATA_LENGTH + DUT_MSG_HEADER_LENGTH)];
} dutDriverFwGeneralMsg_t;

typedef struct _dutMessage
{
	uint16 	msgLength;
	uint16 	status; /* dutStatus_e */
	uint8   wlanIndex;
	uint8 	reserved[1];
	uint16 	msgId;	/* dutMessagesId_e */
	uint32 	data[DUT_MSG_DATA_LENGTH_IN_WORDS];
} __MTLK_PACKED dutMessage_t;

// To Align with mhi_umi convention
typedef  dutMessage_t UMI_DUT;

#define dutDriverMessageHeaderLength 8

typedef struct _C100_MSG_HEADER
{	
	uint16  u16MsgId;
	uint8   u8Task;
	uint8   u8Instance;
} __MTLK_PACKED C100_MSG_HEADER;

typedef struct _UMI_C100
{
	uint16 u16Length;
	uint16 u16stream;
	C100_MSG_HEADER sC100hdr;
	uint8  au8Data[MTLK_PAD4(UMI_C100_DATA_SIZE)];
} __MTLK_PACKED UMI_C100;

//----- Hw Dependent Params----------//
typedef struct dutHwDependentConfParams
{
	uint32	xtalValue;
	int8   	extLNAbypass;
	int8   	extLNAhigh;
} dutHwDependentConfParams_t;

typedef enum
{
	PRIMARY_LOW_40_LOW_20,
	PRIMARY_LOW_40_HIGH_20,
	PRIMARY_HIGH_40_LOW_20,
	PRIMARY_HIGH_40_HIGH_20,
	PRIMARY_MAX_VAL = MAX_UINT8,
} PrimaryChannel_e;

//----- FW Configure Params----------//
typedef struct dutConfigureParams
{
	uint32		txLifeTime;
	uint16		erpRates11g;
	uint16		calibrationAlgoMask;
	IEEE_ADDR	sIEEE_ADDR;
	uint8		phyType;
	uint8		numOfRxChains;
	uint16		spectrumMode; /* Bandwidth_e */
	int8		forceTpc[TPC_ARRAY_SIZE];
	uint8		tpcModeIsInClosedLoop; //  1 - Close loop\ 0 - Open Loop
} dutConfigureParams_t;

//----- FW SetChannel Params----------//
typedef struct dutSetChannelParams
{
	uint32 dummy;
} dutSetChannelParams_t;

//----- Eeprom Params----------//


typedef struct dutEepromParams
{
	uint32 address;
	uint32 eepromSize;
	uint32 length;
	uint8  data[MTLK_PAD4(MAX_DUT_EEPROM_ACCESS_SIZE)];
} dutEepromParams_t;

//----- NV memory Acces Params (Dut Driver Structure)----------//
typedef struct dutNvMemoryAccessParams
{
	uint32 nvMemoryType;
	uint32 address;
	uint32 eepromSize;
	uint32 length;
	uint32 fileType;
	uint8  data[MTLK_PAD4(MAX_DUT_EEPROM_ACCESS_SIZE)];
} __MTLK_PACKED dutNvMemoryAccessParams_t;


typedef struct dutNvMemoryFlushParams
{
	uint32 nvMemoryType;
	uint32 fileType;
	uint8 verifyNvmData;
    int8 verifyResult;
	uint8 reserved[2];
} __MTLK_PACKED dutNvMemoryFlushParams_t;

typedef struct dutResetParams
{
  uint32 nvMemoryType;
  uint32 eepromSize;
  uint8  doReset; // 0 - Enable DUT mode on the fly, 1 - EnableDUT mode via acripts (using rmmode/insmode dut=1)
  uint8  reserved[3];
} __MTLK_PACKED dutResetParams_t;



//----- Memory access Params DUT_READ_MEMORY_REQ\CFM----------//
//----- Memory access Params DUT_WRITE_MEMORY_REQ\CFM----------//
//----- Memory access Params DUT_WRITE_SECURED_REGISTER_REQ\CFM----------//
#define MAX_DUT_MEMORY_ACCESS_SIZE (256)
#define MAX_DUT_MEMORY_ACCESS_SIZE_IN_WORDS (MAX_DUT_MEMORY_ACCESS_SIZE/4)

		



typedef struct dutMemoryAccessParams
{
	uint16 module;
	uint16 length;
	uint32 address;
	uint32 data[MAX_DUT_MEMORY_ACCESS_SIZE_IN_WORDS];
} dutMemoryAccessParams_t;

typedef struct dutSecuredWriteParams
{
	/* modify secured register: */
	/* (*address) = (*address) & ~mask | data */
	uint32	address;
	uint32	mask;
	uint32	data;
} dutSecuredWriteParams_t;

//----- Frequency TPC Params DUT_READ_FREQ_TPC_STRUCTS_REQ\CFM----------//,
typedef union dutFreqTpcParams
{
   TPC_FREQ			 tpcFreqXYcal[DUT_3ANT_ARRAY * TPC_FREQ_XY_STR_NUM];
   TPC_FREQ			 tpcFreq[DUT_4ANT_ARRAY * TPC_FREQ_STR_NUM];
} dutFreqTpcParams_t;

typedef struct dutTpcConfig
{
	tpcConfig_t	tpcConfig;
} dutTpcConfig_t;

//----- Frequency TPC Params DUT_RUN_CALIBRATION_REQ\CFM----------//,
typedef struct dutCalibrationParams
{
	uint16 channel;
//	uint16 calibrationMask;
//	uint8		tpcModeIsInClosedLoop; //  1 - Close loop\ 0 - Open Loop
} dutCalibrationParams_t;

//----- Frequency TPC Params DUT_STOP_FW_REQ\CFM----------//,
typedef struct dutStopFwParams
{
	uint32 dummy;
} dutStopFwParams_t;


//----- Bist Results----------//
typedef struct dutBistResults
{
	int8 bistResults[BIST_RESULTS_ARRAY_SIZE];
} dutBistResults_t;

//----- Chip Version----------//
typedef struct dutChipVersion
{
	uint32 bbChipId;
	uint32 rfChipId;
} dutChipVersion_t;

typedef struct dutRxCount
{
	uint32 val;
} dutRxCount;
//--------------------------------------//
//	Tx Data Types						//
//--------------------------------------//
typedef struct dutTxParams
{
	uint32		packetLength;
	uint16		repeats;
	uint8		isTxEndless;
	uint8		isDataLong;
    uint8       isBeamforming;
    uint8       reserved[3];
} dutTxParams_t;

//--------------------------------------//
//	Hw Configuration Data Types			//
//--------------------------------------//
//-----  Read Hw Params DUT_READ_HW_VERSION_REQ\CFM----------//
typedef struct dutHwParams
{
	uint32	revision;
	uint32	minor;
	uint8	branchId;
	uint8	released;
	uint8	modified;
	uint8	Tx3Enabled;
} dutHwParams_t;

//-----  Set Tpc Table Params (DUT_SET_TPC_REQ\CFM)----------//
typedef struct dutPowerOutVecParams
{
	uint8	ant;
	uint8	bandwidth;
	uint8	powerVectorIndex;
	uint8	powerVectorWord;
} dutPowerOutVecParams_t;

typedef struct dutSetGain
{
	uint8	gainTableIndex;
	uint8	gainWord;
} dutSetGain_t;

//-----  Rfic Rssi Params DUT_GET_RSSI_REQ\CFM----------//
typedef struct dutRssiParams
{
	uint8 rssi[DUT_RXANT_ARRAY];
	uint8 N;
} dutRssiParams_t;

//-----  Tpc Feedback params Params(DUT_GET_TPC_FEEDBACK_REQ\CFM) ----------//
typedef struct dutTpcFeedbackparams
{
	uint32	numOfSamples;
	uint32	actualNumOfSamples;
	uint32  valueGain;
	uint32	valueSum[DUT_4ANT_ARRAY];
} dutTpcFeedbackparams_t;
//-----  Tx Tone Params (DUT_SET_TX_TONE_REQ\CFM) ----------//
typedef struct dutTxToneParams
{
	uint8 onOff;
	int8 amplitude;
	int16 binNum;
} dutTxToneParams_t;

//-----  Tx Spaceless Params (DUT_SET_SPACELESS_TX_REQ\CFM) ----------//
typedef struct dutTxSpacelessParams
{
	uint8 onOff;
} dutTxSpacelessParams_t;

//-----  Scrambler Mode Params (DUT_TX_SPACELESS_REQ\CFM) ----------//
typedef struct dutScramblerParams
{
	uint8 mode;
} dutScramblerParams_t;

//-----  Set IFS Params (DUT_SET_IFS_REQ\CFM) ----------//
typedef struct dutIfsParams
{
	uint32 spacingUsec;
} dutIfsParams_t;

//-----  Counters Params DUT_READ_PACKET_COUNTERS_REQ\CFM----------//
typedef struct packetCountersParams
{
	uint32 rxPacketCounter;
	uint32 rxPacketErrorCounter;
	union
	{
		uint32 fwPacketCounter;
		uint32 type; // PHY = 0/ MPDU = 1, /* counterType_e */
	};
	uint8  doReset;
} packetCountersParams_t;

//-----  Set Tx Power Limit  Params (DUT_SET_TX_POWER_LIMIT_REQ\CFM)----------//
typedef struct powerLimitParams
{
    uint8 powerLimit;
} powerLimitParams_t;

//-----  Counters Params DUT_SET_RISC_MODE_REQ\CFM----------//
typedef struct riscModeParams
{
	dutRiscMode_e riscMode; // 0- Off 1 - On
} riscModeParams_t;

//-----  Set Antenna Params Params DUT_ENABLE_TX_ANTENNA_REQ\CFM----------//
//-----  Set Antenna Params Params DUT_ENABLE_RX_ANTENNA_REQ\CFM----------//
typedef struct dutSetAntennaParams
{
	uint8 enabledAntMask; // bit 0-3: Ants 0-3 ON (3 - ants 0,1 ON, 7 - ants 0,1,2 ON, 0xF - ants 0,1,2,3 ON, 0 - all off)
} dutSetAntennaParams_t;

//-----  Set Antenna Params Params (DUT_SET_DEFAULT_ANTENNA_SET_REQ\CFM) ----------//
typedef struct antSelectionParams
{
	uint8	antSet;
} antSelectionParams_t;
//-----  Rfic Gains Params DUT_GET_TX_GAINS_REQ\CFM ----------//
typedef struct dutAntTxGains
{
	uint8	bbGain;
	uint8	paDriverGain;		// 3bit 0-High 1- Medium 2-Low
	uint8	mixedGain;
} dutAntTxGains_t;

typedef struct dutRficTxGainsParams
{
	dutAntTxGains_t antTxGains[DUT_4ANT_ARRAY]; 
} dutRficTxGainsParams_t;

//-----  Rfic Gains Params DUT_GET_RX_GAINS_REQ\CFM ----------//
typedef struct dutAntRxGains
{
	int16	bbGain;
	int8	lnaGain;		// 3bit 0-High 1- Medium 2-Low
	uint8	type;			// 0-ignore 1-OnlyBbGainIsValid
} dutAntRxGains_t;

typedef struct dutRficRxGainsParams
{
	dutAntRxGains_t antRxGains[DUT_RXANT_ARRAY]; 
} dutRficRxGainsParams_t;

//-----  Xtal Params DUT_GET_XTAL_VALUE_REQ\CFM----------//
typedef struct dutXtalParams
{
	uint32 xtalValue;
} dutXtalParams_t;

typedef struct dutLnaParams
{
	int8	extLNAbypass;
	int8	extLNAhigh;
} dutLnaParam_t;

typedef struct dutTxPhaseParams
{
	uint32 TxPhaseValue;
} dutTxPhaseParams_t;

typedef struct dutCddParams
{
	uint32 CDDvalue;
} dutCddParams_t;

typedef struct dutQbfParams
{
	uint32 QBFvalue;
} dutQbfParams_t;

//-----  Rx Counter Params DUT_GET_RX_COUNTER_REQ\CFM----------//
typedef struct dutRxCounter
{
	uint32 rxCounter;
} dutRxCounter_t;

//-----  Tx power boost Params DUT_SET_TX_BOOST_REQ\CFM----------//
typedef struct dutTxPowerBoost
{
	uint8 ant0;
	uint8 ant1;
	uint8 ant2;
	uint8 ant3;
} dutTxPowerBoost_t;

//-----  start calibration Params DUT_START_CALIBRATION_REQ\CFM----------//
typedef struct dutStartCalibrationParams
{
	uint32	mask;		// calibration mask, see ClbrProcBit_e (only if maskType is configurable)
	uint32	results;	// results as per mask
	uint8	maskType;	// configurable = 0/ default = 1, /* dutCalibrationMaskType_e */
	uint8 	type;		// offline = 0/ online = 1 /* dutCalibrationType_e */
	uint8	status; /* calibrationReturnStatusType_e */
	uint8 	reserved;
} dutStartCalibrationParams_t;


// DUT_SET_RATE_REQ
typedef struct dutSetRate_tag
{
	uint16		txRate;// NSS+MCS
	uint8	 	phyMode; /* PHYMode */
	uint8		cpMode; // values from CyclicPrefixmode_e
	uint8		signalBw;
} dutSetRate_t;

//DUT_GET_FW_INFO_REQ
typedef struct dutGetFwInfo_tag
{
	uint8			vapId;
	uint8			operationMode;
	uint16			spectrumMode; /* Bandwidth_e */
	uint16			primaryChannel;
	uint8			phyMode;
	uint8 			operationalTxAntMask; 	//Band associated antenna mask.
	uint8 			operationalRxAntMask; 	//Band associated antenna mask.
	uint8			riscMode; 				// dutRiscMode_e
	uint8			supportedBandsPsd; 		//Bitmask of supported bands: bit0 set if 5GHz, bit1 set if 2.4GHz and bit2 set if 6GHz.
	uint8 			currentRfBandPsd;		//which one of the bands is the default one.
	uint8 			zwdfsAntMask;			//Zwdfs dedicated antenna mask.
	uint8 			isZwdfsEn;				//ZWDFS enable flag from configuration manager.
	uint8 			psdMaxTxAntMask; 		//Maximal antenna mask from PSD
	uint8 			psdMaxRxAntMask; 		//Maximal antenna mask from PSD
} dutGetFwInfo_t;

typedef struct dutPowerAndPacketLength_tag
{
	uint16 packetLength;
	uint8 txPower;
	uint8 reserved;
} dutPowerAndPacketLength_t;

typedef struct s2dParams
{
    uint8 gain;
    uint8 offset;
    uint8 region; //1-3
    uint8 ant;
    uint8 bw; 
	uint8 pThreshold;
} tpcS2dParams_t;

//-----  DUT_SET_LNA_MID_GAIN_REQ\CFM----------//
typedef struct dutLnaMidGain_tag
{
	uint8 lnaMidGainValue[DUT_RXANT_ARRAY];
} dutLnaMidGain_t;

typedef struct dutSetTransmitPowerVector_tag
{
	uint16 lengthOfPowerVector;
	uint16 startingPacketLength;
	uint16 packetLengthIncrementPerIndex;
	uint8 powerIndexArray[64];
	uint8 numOfTransmitionPerPower[64];
	//uint16 measuredVoltagePerPower[16];
} dutSetTransmitPowerVector_t;

typedef struct dutGetTransmitPowerVectorAntenna_tag
{
	uint32 antennaIndex;	 
	uint16 measuredVoltagePerPower[64];
} dutGetTransmitPowerVectorAntenna_t;

typedef struct dutSetRxBand_tag
{
	uint8 rxBand;
} dutSetRxBand_t;

typedef struct dutSetRxBandLUT_tag
{
	uint8 antMask;
	uint8 firstTuningIndex[DUT_RXANT_ARRAY];
	uint8 reserved[2];
	uint16 rxBandLUT[DUT_RXANT_ARRAY][DUT_NUMBER_OF_CROSSING_POINTS];
} __MTLK_PACKED dutSetRxBandLUT_t;

typedef struct dutSetTxBand_tag
{
	uint8 txBand;
} dutSetTxBand_t;

typedef struct dutSetTxBandLUT_tag
{
	uint16 txBandLUT[DUT_4ANT_ARRAY][DUT_NUMBER_OF_CROSSING_POINTS];
} dutSetTxBandLUT_t;

typedef struct dutGetRxEvm_tag
{
	uint8 rxEvm[DUT_RXANT_ARRAY];
} dutGetRxEvm_t;


typedef struct Clipping_trigger
{
	int clipping_trigger;
} Clipping_trigger_t;

typedef enum
{
	RSSI_REGION_LOW,		// 0x0		
	RSSI_REGION_HIGH,		// 0x1
	RSSI_REGION_MID,		// 0x2	
	NUMBER_OF_RSSI_REGIONS_GEN6 = RSSI_REGION_MID,
	NUMBER_OF_RSSI_REGIONS	// 0x3
} RssiRegion_e;

typedef struct RssiDB_tag
{
	uint32 alpha[DUT_RXANT_ARRAY][NUMBER_OF_RSSI_REGIONS];
	uint32 beta[DUT_RXANT_ARRAY][NUMBER_OF_RSSI_REGIONS];	
	int32 s2dGain[DUT_RXANT_ARRAY][NUMBER_OF_RSSI_REGIONS];
	int32 s2dOffset[DUT_RXANT_ARRAY][NUMBER_OF_RSSI_REGIONS];
	uint32 gamma[DUT_RXANT_ARRAY][NUMBER_OF_RSSI_REGIONS];
} RssiDBPower_t;


// DUT_GET_RATE_HE_REQ
typedef struct dutGetRate_tag
{
	uint8	 	phyMode; 	
	uint8		nss;
	uint8		mcs;
	uint8		gi;	// 0-2
	uint8		signalBw;
	uint8		reserved[3];
	uint32		rate;
} dutGetRate_t;

typedef struct dutChangeBand
{
   uint8 changeToRadio;
   uint8 reserved[3];
} dutChangeBand_t;

typedef struct dutPoutTableOffsetParams_tag
{
	uint8	 	antNum; 	
	uint8		bw;
	short 		tableOffset;
} dutPoutTableOffsetParams_t;

typedef struct dutRxEnableAggregate
{
	uint8		aggregate;    // 1 enable, 0 disable
	uint8		reserved[3];
} dutRxEnableAggregate_t;


typedef struct dutSetBbicCdd
{
	uint8		antNum;    // range 1,2,3,4
	uint8		reserved[3];
	uint32		offset1_rtrn;
	uint32		offset2_rtrn;
	uint32		offset3_rtrn;
} __MTLK_PACKED dutSetBbicCdd_t;

typedef struct dutGetBbicCdd
{
	uint8		antNum;    // range 1,2,3,4
	uint8		reserved[3];	
	uint32		offset1_rtrn;
	uint32		offset2_rtrn;
	uint32		offset3_rtrn;
} __MTLK_PACKED dutGetBbicCdd_t;

typedef struct dutRficChannelCnt
{
    uint16 channelCnt;
} dutRficChannelCnt_t;

typedef struct dutRficRfRegistersDB
{
	uint16 channelNum;
	uint16 channelShortRegData[RFIC_NUM_OF_SHORT_REGS_WRX600B];	//worst case is 4 for WRX600B/WRX600D2
	uint16 channelLongRegData[RFIC_NUM_OF_LONG_REGS_WRX700];	//worst case is 128 for WRX700
} dutRficRfRegistersDB_t;

typedef struct dutCollectDebugInfoReq
{
	uint8 type;
	uint8 reserved[3];
	uint32 hostIP;
}dutCollectDebugInfoReq_t;
typedef struct dutGetTemperature 
{
	int32 inttempRes;
	int32 dectempRes;
} dutGetTemperature_t;

typedef struct dpdNmseMeasures
{
	int32 values[DUT_4ANT_ARRAY][DPD_TOTAL_CALIBRATION_POINTS];
} dpdNmseMeasures_t;

typedef struct dutRuParams
{
	uint32 userOne;
	uint32 userTwo;
} dutRuParams_t;

typedef struct dutSetClipperParams
{
	uint8 enabled;
} dutSetClipperParams_t;

// DUT_GET_RX_RATE_REQ
typedef struct dutGetRxRate
{
	uint8    mcs;
	uint8    nss;
} dutGetRxRate_t;

// DUT_RX_PACKET_LIMIT_REQ
typedef struct dutRxPacketLimitReq
{
	uint32 packetLimit;
	uint8	protocol; /* PHYMode 	0 for Legacy(11a,b,g),  1 for HT, 3 for VHT, 4 for HE, 5 for EHT */
	uint8 reserved[3];
} __MTLK_PACKED dutRxPacketLimit_t;

// DUT_RX_PER_COUNTERS_REQ
typedef struct dutRxPerCountersReq
{
	uint32 totalPackets;
	uint32 packetCounter;
	uint8 protocol; /* PHYMode  0 for Legacy(11a,b,g),  1 for HT, 3 for VHT, 4 for HE, 5 for EHT */
	uint8 stopRx;
	uint8 reserved[2];
} __MTLK_PACKED dutRxPerCounters_t;

// DUT_RX_MEASURE_PARAMS_REQ
typedef struct dutRxMeasureParamsReq
{
	uint32 numCaptures;
	uint32 interval;
	uint8 protocol;
	uint8 enabled;
	uint8 reserved[2];
} __MTLK_PACKED dutRxMeasureParams_t;

typedef struct _UMI_RX_MEASURE_REPORT
{
	int8 rcpi[4];
	int8 rssi[4];
	int8 noise[4];
	uint32 snr[4];
	uint32 captures_rem;
	uint8 evm[4];
} __MTLK_PACKED UMI_RX_MEASURE_REPORT;
#define MTLK_PACK_OFF
#include "mtlkpack.h"

#endif
















