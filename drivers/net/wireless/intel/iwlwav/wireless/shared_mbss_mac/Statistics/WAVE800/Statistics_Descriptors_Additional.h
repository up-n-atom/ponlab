/************************************************************************************
*
*	File:				Statistics_Descriptors_Additional.h
*	Description:	
*	COPYRIGHT:
*		Copyright(c) 2020, MaxLinear, Inc.
*		Copyright 2016 - 2020 Intel Corporation
*
*		All rights are strictly reserved. Reproduction or divulgence in any
*		form whatsoever is not permitted without written authority from the
*		copyright owner. Issued by Intel Israel Ltd
*
**************************************************************************************/
#ifndef _STATISTICS_DESC_ADD_WAVE700_H_
#define _STATISTICS_DESC_ADD_WAVE700_H_
  

#if (defined (CPU_ARC)) && (!defined(ENET_INC_DPA_RX)) 

#include "HwGlobalDefinitions.h"
#include "linkAdaptation_api.h"
#include "mhi_umi.h"
#include "GroupManager_API.h"
#include "PacketTrafficArbitrator_api.h"
#include "ieee80211.h"
#include "SharedDbTypes.h"
#include "TwtManager_API.h"
#include "Utils_Api.h"

#endif 

#define PHY_STATISTICS_MAX_RX_ANT_700  	 	(5)

#ifdef CPU_ARC
#define PHY_STATISTICS_MAX_RX_ANT      	 	(PHY_STATISTICS_MAX_RX_ANT_700)
#endif // CPU_ARC

#if (!defined(CPU_ARC)) || defined (ENET_INC_DPA_RX)
#define MAX_NUM_OF_ANTENNAS					(4)
#define NUM_OF_BITS_IN_BYTE					(0x8) 
#define NUM_OF_BITS_IN_UINT32 				(32)  
#define NUM_OF_BYTES_IN_WORD				(NUM_OF_BITS_IN_UINT32/NUM_OF_BITS_IN_BYTE) 
#define NUM_OF_EXTENTION_RSSI				(4)

#endif //(!CPU_ARC) || (ENET_INC_DPA_RX)

typedef struct 
{
#ifdef ENET_INC_DPA_RX
	uint32  rssi;      //Last 4 RSSI frames received
#else
	uint8  rssi[4]; 	 //Last 4 RSSI frames received
#endif //ENET_INC_DPA_RX
	uint8  time_s[4];    //Time of when last 4 RSSI were received
	uint16 count;        //Sequence numer of received managemant (bcn, ack) frames 
} rssiSnapshot_t_wave700;
#ifdef CPU_ARC
typedef rssiSnapshot_t_wave700 rssiSnapshot_t;
#endif



typedef struct 
{
	//words 0,1
	int8	noise[PHY_STATISTICS_MAX_RX_ANT_700];
	uint8	reserved0[NUM_OF_BYTES_IN_WORD*2-PHY_STATISTICS_MAX_RX_ANT_700]; 
	//words 2,3
	int8	gain[PHY_STATISTICS_MAX_RX_ANT_700];	
	uint8	reserved1[NUM_OF_BYTES_IN_WORD*2-PHY_STATISTICS_MAX_RX_ANT_700]; 
	//words 4,5
	int8	evm[MAX_NUM_OF_ANTENNAS];					// EVM is NSS related(4), not num of antennas (5)
	uint32	reserved2;									// 8 bit reserved, 21bit alpha factor, 3 bit reserved
	//words 6,7
	int8	rssi[PHY_STATISTICS_MAX_RX_ANT_700];	
	uint8	reserved3[NUM_OF_BYTES_IN_WORD*2-PHY_STATISTICS_MAX_RX_ANT_700]; 
	//words 8,9
	uint32 	avgDeltaT;	
	uint32  reserved4;								
	//word 10
	uint32 	phyRate;
	//word 11
	uint32 	tsf;
	//word 12 		for SW use
	uint32	perClientRxtimeUsage;
	//word 13-15	for SW use
	rssiSnapshot_t_wave700	rssiAckAvarage; //AP_MODE: RSSI from last 4 ack received
	uint8	reserved[2];

	//word 16-19
	uint8 	rssiArray[MAX_NUM_OF_ANTENNAS][NUM_OF_EXTENTION_RSSI];   // 4=antennas, 4=20+20+40+80 extension rssi
	
	/* Per antenna RSSI (above noise floor) for all widths (primary,secondary) 
	  -----------------------------------------------
	  | chain_nun	|  20MHz [pri20                   		]
	  |           	|  40MHZ [pri20,sec20             		] 
	  |          	 	|  80MHz [pri20,sec20,sec40,      	]
	  |           	| 160MHz [pri20,sec20,sec40,sec80 	]
	  -----------------------------------------------
	  |  1        	|  rssi  [pri20,sec20,sec40,sec80 ]
	  |  ...      	|  ...
	  |  8        	|  rssi  [pri20,sec20,sec40,sec80 ]
	  ----------------------------------------------- */

	//word 20
	uint32 phyRateSynchedToPsduRate;		/* Phy rate extraced by RX HANDLER from PHY METRICS (the above phy rate is extracted by PAC Extrapolator) */

	//word 21
	uint32	psduRate;						/* PSDU RATE is copied from phy metrics. It is constructed from MCS & NSS [bits 0..7], PHY MODE [bits 8..10], BW [bits 11..12], SCP [bit 13], HE CP [bits 14..15] */


	//Last possible word num is 14 (total of 15 words that can be configured to pac Extrap for 4 bits parameter
} stationPhyRxStatusDb_t_wave700;
#ifdef CPU_ARC
typedef stationPhyRxStatusDb_t_wave700 stationPhyRxStatusDb_t;
#endif

#endif //_STATISTICS_DESC_ADD_WAVE700_H_
