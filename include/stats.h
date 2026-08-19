/******************************************************************************

         Copyright (c) 2020, MaxLinear, Inc.
         Copyright 2016 - 2020 Intel Corporation

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

*******************************************************************************/

#ifndef __STATS_H__
#define __STATS_H__

#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
extern void CSI_parsing(const int Data[788], int output_CSI_matrix_data[],
                        int output_CSI_matrix_size[3], int *time_stamp_1,
                        int *time_stamp_2, int *error_flag);

#ifdef __cplusplus
}
#endif

#define HAL_NUM_OF_ANTS		(4)
/* Length of IEEE address (bytes) */
#define IEEE_ADDR_LEN			(6)

#define MAX_NUM_OF_BW			(5)

#define MAX_REDUCED_MCS		(4)
#define MAX_REDUCED_NSS		(2)


/* This type is used for Source and Destination MAC addresses and also as   */
/* unique identifiers for Stations and Networks.                            */

typedef struct _IEEE_ADDR
{
    unsigned char au8Addr[IEEE_ADDR_LEN]; /* WARNING: Special case! No padding here! This structure must be padded externally! */
} IEEE_ADDR;


#define MAC_PRINTF_FMT "%02X:%02X:%02X:%02X:%02X:%02X"

#define __BYTE_ARG(b,i)            (((unsigned char *)(b))[i])
#define __BYTE_ARG_TYPE(t,b,i)     ((t)__BYTE_ARG((b),(i)))
#define _BYTE_ARG_U(b,i)            __BYTE_ARG_TYPE(unsigned int,(b),(i))
#define MAC_PRINTF_ARG(x) \
  _BYTE_ARG_U((x), 0),_BYTE_ARG_U((x), 1),_BYTE_ARG_U((x), 2),_BYTE_ARG_U((x), 3),_BYTE_ARG_U((x), 4),_BYTE_ARG_U((x), 5)

#define MAX_NL_REPLY 8192
#define MAX_LEN_VALID_VALUE 1024
#define MAX_USAGE_LEN 128
#define MAX_COMMAND_LEN 64
#define num_stats(X) (sizeof(X)/sizeof(stats))
#define PRINT_DESCRIPTION(x)	printf("%s\n",x.description);
#define INDENTATION(x)		if(x>0)\
					{while(x) { printf("  "); x--;}};

#define MAC2STR(a)    (a)[0]&0xff, (a)[1]&0xff, (a)[2]&0xff, (a)[3]&0xff, (a)[4]&0xff, (a)[5]&0xff
#define STR2MAC(a)    &((a)[0]), &((a)[1]), &((a)[2]), &((a)[3]), &((a)[4]), &((a)[5])
#define MACSTR_LOWER  "%02x:%02x:%02x:%02x:%02x:%02x"
#define MACSTR_UPPER  "%02X:%02X:%02X:%02X:%02X:%02X"
#define MACSTR2       "%02x%02x%02x%02x%02x%02x"
#define MACSTR        MACSTR_LOWER

/*
reset_stats request type
*/
typedef enum
{
	RESET_FULL = 0,
	RESET_RADIO,
	RESET_VAP,
	RESET_STA,
	RESET_INVALID = 0xFF,
} stat_reset_category_e;

enum Ru_size_map {
	RU26          =  0, //   2 MHz
	RU52          =  1, //   5 MHz
	RU106         =  2, //  10 MHz
	RU242         =  3, //  20 MHz
	RU484         =  4, //  40 MHz
	RU996         =  5, //  80 MHz
	RU996X2       =  6, // 160 MHz
	MAX_NUM_RU_SIZES,
	RU996X4       =  MAX_NUM_RU_SIZES, // 320 MHz
	RU52_26       =  8, //   7 MHz
	RU106_26      =  9, //  12 MHz
	RU484_242     = 10, //  60 MHz
	RU996_484     = 11, // 120 MHz
	RU996_484_242 = 12, // 140 MHz
	RU996X2_484   = 13, // 200 MHz
	RU996X3       = 14, // 240 MHz
	RU996X3_484   = 15, // 280 MHz
	MAX_NUM_RU_SIZES_GEN7
};

enum cw_width {
	CH_WIDTH_20,      //channel width 20
	CH_WIDTH_40,     //channel width 40
	CH_WIDTH_80,    //channel width 80
	CH_WIDTH_160,  //channel width 160
	CH_WIDTH_320,  //Channel width 320
};

typedef enum {
	REMOVAL_REASON_DLOFDMA_MINXPUT,
	REMOVAL_REASON_DLOFDMA_SU_VALIDATION,
	REMOVAL_REASON_DLOFDMA_ULPCALGO,
	REMOVAL_REASON_DLOFDMA_LA_CONVERGENCE,
	REMOVAL_REASON_DLOFDMA_SHORT_TX,
	REMOVAL_REASON_DLOFDMA_MOVED_STA,
	REMOVAL_REASON_DLOFDMA_ATR,
	REMOVAL_REASON_DLOFDMA_PAYLOAD_EFF,
	REMOVAL_REASON_DLOFDMA_LATENCY,
	REMOVAL_REASON_DLOFDMA_CONGESTION,
	REMOVAL_REASON_DLOFDMA_MCS_DEGRADATION,

	REMOVAL_REASON_ULOFDMA_CONGESTION,
	REMOVAL_REASON_ULOFDMA_MINXPUT,
	REMOVAL_REASON_ULOFDMA_PAYLOAD_EFF,
	REMOVAL_REASON_ULOFDMA_ULPCALGO,
	REMOVAL_REASON_ULOFDMA_UPHALGO,

	REMOVAL_REASON_DLMIMO_MINXPUT,
	REMOVAL_REASON_DLMIMO_MCS_DEGRADATION,
	REMOVAL_REASON_DLMIMO_TXOP_USAGE,
	REMOVAL_REASON_DLMIMO_PAYLOAD_EFF,
	REMOVAL_REASON_DLMIMO_SUMRATE,
	REMOVAL_REASON_DLMIMO_ULPCALGO,

	REMOVAL_REASON_REQUESTOR_DISABLE_PROCESS,
	REMOVAL_REASON_REQUESTOR_DISABLE_DYNAMIC_MU_PROCESS,
	REMOVAL_REASON_REQUESTOR_REMOVE_STATIC,
	REMOVAL_REASON_REQUESTOR_STATION_REMOVAL,
	REMOVAL_REASON_REQUESTOR_CREATE_STATIC,
	REMOVAL_REASON_REQUESTOR_SET_CHANNEL,
	REMOVAL_REASON_REQUESTOR_SET_ANTENNA,
	REMOVAL_REASON_REQUESTOR_REMOVE_GROUPS_FOR_STATION_TID,
	REMOVAL_REASON_REQUESTOR_CHANGE_FROM_STATIC_TO_DYNAMIC,
	REMOVAL_REASON_REQUESTOR_STA_CAPABILITY_CHANGED,
	REMOVAL_REASON_REQUESTOR_STA_UPH_OUT_OF_BOUNDS,
	REMOVAL_REASON_REQUESTOR_ML_SWITCH_LINK,
	REMOVAL_REASON_REQUESTOR_REBUILD_ALL_GROUPS,
	REMOVAL_REASON_REQUESTOR_POWER_CHANGE,
	REMOVAL_REASON_REQUESTOR_CREATE_GROUP_FAILED,
	REMOVAL_REASON_REQUESTOR_ULPC_ALGO_ERROR,
	REMOVAL_REASON_REQUESTOR_DISABLE_DYNAMIC_MU_BY_TYPE_PROCESS,
	REMOVAL_REASON_REQUESTOR_GROUP_SIMILAR_CHARACTERISTICS,
	REMOVAL_REASON_REQUESTOR_STA_IN_PS_MODE,
	REMOVAL_REASON_REQUESTOR_TWT_AGREEMENT,
	REMOVAL_REASON_REQUESTOR_MBFM_ERRORS,
	REMOVAL_REASON_REQUESTOR_INVALID_UL_PSDU_LENGTH,

	REMOVAL_REASON_OTHER,

	REMOVAL_REASON_ULMIMO_MINXPUT,
	REMOVAL_REASON_ULMIMO_MCS_DEGRADATION,
	REMOVAL_REASON_ULMIMO_PAYLOAD_EFF,
	REMOVAL_REASON_ULMIMO_TXOP_USAGE,
	REMOVAL_REASON_ULMIMO_ULPCALGO,
	REMOVAL_REASON_ULMIMO_UPHALGO,

	REMOVAL_REASON_MAX,
} group_removal_reason_t;

typedef struct
{
	unsigned char  mcs;
	unsigned char  nss;
	unsigned char  gi;
	unsigned char  reserved1;
	unsigned short bw;
	unsigned short reserved2;
	unsigned int   per;
} packet_error_rate_t;

typedef struct peer_list{
	IEEE_ADDR addr;
	unsigned int is_sta_auth;
}peer_list_t;

typedef struct link_adapt_mu_ofdma_stats{
	uint16_t sta_sid;
	IEEE_ADDR addr;
	uint32_t staRemovedNbiqBelowThresholdCounter;
	uint32_t staRemovedFavoringSuCounter;
	uint32_t staNotAddedNbiqBelowThresholdCounter;
}link_adapt_mu_ofdma_stats_t;

typedef struct pdu_percent_stats
{
	uint32_t ru_he_ppdus_percent;
	uint32_t ru_mpdus_count_percent;
	uint32_t ru_mpdus_err_count_percent;
} pdu_percent_stats_t;

/* The struct link_adapt_ru_mpdu_ofdma_stats_t should match with */
/* the driver wave_mhi_la_ofdma_ru_mpdus_stats_t struct */
typedef struct link_adapt_ru_mpdu_ofdma_stats {
	uint64_t ru_total_mpdus_count;
	uint64_t ru_mpdus_err_count;
} link_adapt_ru_mpdu_ofdma_stats_t;

/* The struct link_adapt_ru_ppdu_ofdma_stats_t should match with */
/* the driver wave_mhi_la_ofdma_ru_ppdu_stats_t struct */
typedef struct link_adapt_ru_ppdu_ofdma_stats {
	uint64_t ru_he_ppdus_count;
} link_adapt_ru_ppdu_ofdma_stats_t;

/* The struct link_adapt_ppdu_stats_t should match with */
/* driver wave_mhi_la_ofdma_ppdus_stats struct */
typedef struct link_adapt_ppdu_stats {
	uint64_t total_ppdus_count;
	uint64_t mu_ofdma_ppdus_count;
	uint64_t mu_mimo_ppdus_count;
	uint64_t mu_ppdus_count;
	uint64_t he_ppdus_count;
	uint64_t total_mu_ru_ppdus;
	link_adapt_ru_ppdu_ofdma_stats_t ru_ppdu_stats[MAX_NUM_RU_SIZES_GEN7];
} link_adapt_ppdu_stats_t;

/* The struct link_adapt_pppdu_sta_stats_t should match with */
/* driver wave_mhi_la_ofdma_sta_ppdu_stats_t struct */
typedef struct link_adapt_ppdu_sta_stats {
	link_adapt_ppdu_stats_t ppdu_stats;
	uint64_t su_ppdus_count[MAX_NUM_OF_BW];
} link_adapt_ppdu_sta_stats_t;

/* The struct link_adapt_mppdu_sta_stats_t should match with */
/* driver wave_mhi_la_ofdma_mpdu_stats_t struct */
typedef struct link_adapt_mpdu_sta_stats {
	link_adapt_ru_mpdu_ofdma_stats_t ru_mpdu_stats[MAX_NUM_RU_SIZES_GEN7];
} link_adapt_mpdu_stats_t;

/* The struct link_adapt_ofdma_stats_t should match with */
/* driver wave_wssa_ofdma_pdu_sta_stats_t struct */
typedef struct link_adapt_ofdma_stats {
	uint16_t sta_sid;
	IEEE_ADDR addr;
	uint8_t no_of_ru_sizes;
	uint8_t no_of_bw;
	link_adapt_ppdu_sta_stats_t sta_ppdus;
	link_adapt_mpdu_stats_t sta_mpdus;
} link_adapt_ofdma_stats_t;

/* The struct link_adapt_radio_ofdma_stats_t should match with */
/* driver wave_wssa_la_ofdma_radio_pdu_stats_t struct */
typedef struct link_adapt_radio_ofdma_stats {
	uint8_t no_of_ru_sizes;
	uint8_t no_of_bw;
	uint32_t bw_ru_in_ppdus[MAX_NUM_OF_BW][MAX_NUM_RU_SIZES_GEN7];
	link_adapt_ppdu_stats_t radio_ppdus;
	link_adapt_mpdu_stats_t radio_mpdus;
	uint64_t su_ppdus_count;
} link_adapt_radio_ofdma_stats_t;

/* HE MIMO stats */
/* The struct link_adapt_mimo_rate_stats_t should match with */
/* driver wave_mhi_mimo_rate_stats_t struct */
typedef struct link_adapt_mimo_rate_stats {
	uint32_t nss_ppdus_percent[MAX_REDUCED_NSS];
	uint32_t mcs_ppdus_percent[MAX_REDUCED_MCS];
	uint64_t nss_ppdus[MAX_REDUCED_NSS];
	uint64_t mcs_ppdus[MAX_REDUCED_MCS];
} link_adapt_mimo_rate_stats_t;

/* The struct link_adapt_su_mimo_stats_t should match with */
/* driver wave_mhi_su_mimo_stats_t struct */
typedef struct link_adapt_su_mimo_stats {
	link_adapt_mimo_rate_stats_t su_mimo_rate;
} link_adapt_su_mimo_stats_t;

/* The struct link_adapt_mu_mimo_stats should match with */
/* driver wave_mhi_mu_mimo_stats_t struct */
typedef struct link_adapt_mu_mimo_stats {
	uint32_t succeeded_sounding_percent;
	uint32_t attemptedsounding_per_mcs_percent[MAX_REDUCED_MCS];
	link_adapt_mimo_rate_stats_t mu_mimo_rate;
	uint64_t succeeded_sounding;
	uint64_t attemptedsounding_per_mcs[MAX_REDUCED_MCS];
	uint64_t sum_attemptedsounding;
} link_adapt_mu_mimo_stats_t;

/* The struct link_adapt_mimo_stats_t should match with */
/* driver wave_wssa_mimo_stats_t struct */
typedef struct link_adapt_mimo_stats {
	link_adapt_su_mimo_stats_t su_mimo;
	link_adapt_mu_mimo_stats_t mu_mimo;
} link_adapt_mimo_stats_t;

/* The struct link_adapt_mimo_sta_stats_t should match with */
/* driver wave_wssa_mimo_sta_stats_t struct */
typedef struct link_adapt_mimo_sta_stats {
	uint16_t sta_sid;
	IEEE_ADDR addr;
	uint8_t no_of_nss;
	uint8_t no_of_mcs;
	link_adapt_mimo_stats_t sta_stats;
} link_adapt_mimo_sta_stats_t;

/* The struct link_adapt_mimo_stats_t should match with */
/* driver wave_wssa_mimo_radio_stats_t struct */
typedef struct link_adapt_mimo_radio_stats {
	uint8_t no_of_nss;
	uint8_t no_of_mcs;
	link_adapt_mimo_stats_t mimo_radio_stats;
} link_adapt_mimo_radio_stats_t;

/* HE MIMO sta stats ends */

/* This structure must match with drivers mtlk_wssa_drv_la_mu_he_eht_input_t */
typedef struct la_mu_eh_eht_input
{
  int group_id;
  int reset_stats;
} la_mu_he_eht_input_t;

/* This enum must match with drivers enum wave_la_mu_he_eht_reset */
enum la_mu_he_eht_reset
{
  LA_MU_HE_EHT_NO_RESET,
  LA_MU_HE_EHT_RESET_GROUP,
  LA_MU_HE_EHT_RESET_ALL,
};

/* This enum must match with drivers wave_mu_la_group_type_e */
enum mu_la_group_type
{
  MU_GROUP_INVALID,
  MU_GROUP_DL,
  MU_GROUP_UL,
};

#define MAX_NUM_OF_HE_GROUPS 26
#define MAX_STA_IN_GROUP      8
#define MAX_MCS              14
/* This struct must match with drivers mtlk_wssa_drv_la_mu_he_eht_stats_t */
typedef struct la_mu_he_eht_stats
{
  uint8_t type;
  uint8_t active;
  uint32_t creation_toggle_bitmap;

  union {
    struct {
        uint32_t protectionSentCnt;
        uint32_t protectionSucceededCnt;
        uint32_t dlPhaseAllocFailCnt;
        uint32_t heMuFastProbeTriggeredCnt[MAX_STA_IN_GROUP];
        uint32_t heMuFastProbeNotFinishedCnt[MAX_STA_IN_GROUP];
        uint32_t heMuCollisionDetectedCnt[MAX_STA_IN_GROUP];
        uint32_t mcsHistogram[MAX_STA_IN_GROUP][MAX_MCS];

        uint16_t staId[MAX_STA_IN_GROUP];
        int8_t   targetRssi[MAX_STA_IN_GROUP];
        int8_t   txPower[MAX_STA_IN_GROUP];
    } dl;

    struct {
        uint32_t ulPCStabilityCnt;
        uint32_t fastDropCnt[MAX_STA_IN_GROUP];
        uint32_t invalidReportsCnt[MAX_STA_IN_GROUP];
        uint32_t rxPPDUsCnt[MAX_STA_IN_GROUP];
        uint32_t changedWpCnt[MAX_STA_IN_GROUP];
        uint32_t collisionDetectedCnt[MAX_STA_IN_GROUP];
        uint32_t mcsHistogram[MAX_STA_IN_GROUP][MAX_MCS];

        uint16_t staId[MAX_STA_IN_GROUP];
        int8_t   targetRSSI[MAX_STA_IN_GROUP];
        uint8_t  longTermErrorRate[MAX_STA_IN_GROUP];
        uint8_t  workingPointErrorRate[MAX_STA_IN_GROUP];
    } ul;
  };
} la_mu_he_eht_stats_t;

typedef struct mu_groups_counters {
	uint32_t groupsCreated;
	uint32_t groupsRemoved;
	uint16_t removalReason[REMOVAL_REASON_MAX];
} mu_groups_counters_t;

typedef struct la_mu_groups_counters_stats {
	bool is_gen6;
	mu_groups_counters_t dlOfdmaGroupsCounters;
	mu_groups_counters_t ulOfdmaGroupsCounters;
	mu_groups_counters_t dlMimoGroupsCounters;
	mu_groups_counters_t ulMimoGroupsCounters;
} la_mu_groups_counters_stats_t;

/* CSI Stats */
#define MAX_NR               5
#define MAX_NR_700           4
#define CSI_RAW_DATA_SIZE    788
#define CSI_MATRIX_DATA_SIZE 768

/* OFDMA stats */
#define PARAM_LIMIT                 1
/*for OFDMA ppdus stats */
#define PARAM_UL_OFDMA              0
#define PARAM_DL_OFDMA              1
#define GET_UL_OFDMA_CATEGORY       2
#define GET_DL_OFDMA_CATEGORY       3
#define GET_UL_RADIO_OFDMA_CATEGORY 0xF0
#define GET_DL_RADIO_OFDMA_CATEGORY 0xF1


typedef signed short wifi_streams_rssi_t [MAX_NR];

typedef struct _wifi_frame_info
{
	unsigned char bw_mode;
	unsigned char mcs;
	unsigned char Nr;
	unsigned char Nc;
	wifi_streams_rssi_t nr_rssi;
	uint32_t channel;
	unsigned short valid_mask;
	unsigned short phy_bw;
	unsigned short cap_bw;
	uint32_t num_sc;
	unsigned char decimation;
	int16_t frequency_offset;
	uint64_t time_stamp;
} __attribute__((aligned(1), packed)) wifi_frame_info_t;

typedef struct _wifi_csi_driver_data_t {
	wifi_frame_info_t frame_info;
	int csi_raw_data[CSI_RAW_DATA_SIZE];
} wifi_csi_driver_data_t;

typedef enum {
	PEER_FLOW_STATS=0,
	PEER_TRAFFIC_STAT,
	RETRANS_STAT,
	TR181_PEER_STATS,
	NETWORK_BITFIELD,
	PEER_CAPABILITY,
	VENDOR_ENUM,
	PEER_RATE_INFO,
	PEER_RATE_INFO1,
	PHY_ENUM,
	RECOVERY_STAT,
	HW_TXM_STAT,
	TRAFFIC_STATS,
	MGMT_STATS,
	HW_FLOW_STATUS,
	TR181_ERROR_STATS,
	TR181_WLAN_STATS,
	TR181_HW_STATS,
	PACKET_ERROR_RATE,
	GI_ENUM,
	PTA_STATS,
	PEER_HOST_IF_QOS,
	PEER_HOST_IF,
	PEER_RX_STATS,
	PEER_UL_BSRC_TID_STATS,
	PEER_BAA_STATS,
	LINK_ADAPTION_STATS,
	PLAN_MANAGER_STATS,
	TWT_STATS,
	PER_CLIENT_STATS,
	PEER_PHY_RX_STATUS,
	PEER_INFO,
	WLAN_HOST_IF_QOS,
	WLAN_HOST_IF,
	WLAN_RX_STATS,
	WLAN_BAA_STATS,
	RADIO_RX_STATS,
	RADIO_BAA_STATS,
	TS_INIT_TID_GL,
	TS_INIT_STA_GL,
	TS_RCPT_TID_GL,
	TS_RCPT_STA_GL,
	RADIO_LINK_ADAPT_STATS,
	MULTICAST_STATS,
	TRAIN_MAN_STATS,
	GROUP_MAN_STATS,
	GENERAL_STATS,
	CHANNEL_STATS,
	RADIO_PHY_RX_STATS,
	DYNAMIC_BW_STATS,
	LA_MU_VHT_STATS,
	CCA_STATS,
	PEER_DIAG_RESULT3,
	PEER_MIXED_STATS,
	CSI_COUNTERS,
	ML_PEER_FLOW_STATS,
	ML_SINGLE_LINK_PEER_FLOW_STATS,

	PEER_LIST,
	RESET_STATISTICS,
	LINK_ADAPT_MU_GROUPS_COUNTERS_STATS,
	LINK_ADAPT_SU_MU_RU_OFDMA_STATS,
	CSI_STATS,
	LINK_ADAPT_MIMO_OFDMA_STATS,
	LA_MU_HE_EHT_STATS,
}stat_id;

typedef struct 
{
	char cmd[MAX_COMMAND_LEN]; //command name
	enum ltq_nl80211_vendor_subcmds id; //NL command
	int num_arg; // number of arguments expected
	char usage[MAX_USAGE_LEN];
	stat_id c; //enum for each cmd
}stats_cmd;

typedef enum
{
	UCHAR=0,
	CHAR,
	STRING,
	BOOLEAN,
	MACADDR,
	BYTE,
	HEXBYTE,
	UINT,
	INT,
	USHORT,
	LONG,
	HEXLONG,
	SLONG,
	SLONGARRAY,
	SSHORTARRAY,
	SBYTEARRAY,
	BYTEARRAY,
	FLAG,
	BITFIELD,
	ENUM,
	TIMESTAMP,
	LONGFRACT,
	SLONGFRACT,
	HUGE,
	LONGARRAY,
	RESERVED,
	NONE	
}type;

typedef struct
{
	stat_id c;
	char description[MAX_LEN_VALID_VALUE];
	type t;
	int element;
}stats;

struct print_struct {
	stat_id st;
	stats *sts;
	int size;
};

stats network_bitfield[] = {
{NETWORK_BITFIELD,	"802.11a",		BITFIELD,	0}, 
{NETWORK_BITFIELD,	"802.11b",		BITFIELD,	1}, 
{NETWORK_BITFIELD,	"802.11g",		BITFIELD,	2}, 
{NETWORK_BITFIELD,	"802.11n",		BITFIELD,	3}, 
{NETWORK_BITFIELD,	"802.11ac",		BITFIELD,	4},
{NETWORK_BITFIELD,	"802.11ax",		BITFIELD,	5},
{NETWORK_BITFIELD,	"802.11be",		BITFIELD,	6}
};

stats phy_enum[] = {
{PHY_ENUM,	"802.11a/g",	ENUM,	0}, 
{PHY_ENUM,	"802.11b",	ENUM,	1}, 
{PHY_ENUM,	"802.11n",	ENUM,	2}, 
{PHY_ENUM,	"802.11ac",	ENUM,	3},
{PHY_ENUM,	"802.11ax",	ENUM,	4},
{PHY_ENUM,	"802.11be",	ENUM,	8}
};

stats vendor_enum[] = {
{VENDOR_ENUM,	"Unknown",	ENUM,	0},
{VENDOR_ENUM,	"Lantiq",	ENUM,	1},
{VENDOR_ENUM,	"W101",		ENUM,	2}
};

stats gi_enum[] = {
{GI_ENUM,       "Short",        ENUM,   0},
{GI_ENUM,       "Long",         ENUM,   1},
};

stats peer_traffic_stat[] = {
{PEER_TRAFFIC_STAT, "BytesSent                - Number of bytes sent successfully",		HUGE,	0},
{PEER_TRAFFIC_STAT, "BytesReceived            - Number of bytes received",				HUGE,	0},
{PEER_TRAFFIC_STAT, "PacketsSent              - Number of packets transmitted",			HUGE,	0},
{PEER_TRAFFIC_STAT, "PacketsReceived          - Number of packets received",			HUGE,	0},
{PEER_TRAFFIC_STAT, "UnicastPacketsSent       - Number of unicast packets sent",		LONG,	0},
{PEER_TRAFFIC_STAT, "UnicastBytesSent         - Number of unicast bytes sent",			LONG,	0},
{PEER_TRAFFIC_STAT, "UnicastPacketsReceived   - Number of unicast packets received",	LONG,	0},
{PEER_TRAFFIC_STAT, "UnicastBytesReceived     - Number of unicast bytes received",		LONG,	0},
{PEER_TRAFFIC_STAT, "MulticastPacketsSent     - Number of multicast packets sent",		LONG,	0},
{PEER_TRAFFIC_STAT, "MulticastBytesSent       - Number of multicast bytes received",	LONG,	0},
{PEER_TRAFFIC_STAT, "BroadcastPacketsReceived - Number of broadcast packets received",	LONG,	0},
{PEER_TRAFFIC_STAT, "BroadcastBytesReceived   - Number of broadcast bytes received",	LONG,	0}
};

stats peer_retrans_stat[] = {
{RETRANS_STAT, "Re-transmissions         - Number of re-transmitted, from the last 100 packets sent",			LONG,	0},
{RETRANS_STAT, "RetransCount             - Total number of transmitted packets which were re-transmissions",	LONG,	0},
{RETRANS_STAT, "RetryCount               - Total number of re-transmitted packets",	LONG,	0},
{RETRANS_STAT, "MultipleRetryCount       - Number of Tx packets succeeded after more than one re-transmission",	LONG,	0},
{RETRANS_STAT, "FailedRetransCount       - Number of Tx packets dropped because of retry limit exceeded",		LONG,	0}
};

stats tr181_peer_stat[] = {
{TR181_PEER_STATS,	"StationID",																			LONG,	0},
{NETWORK_BITFIELD,	"OperatingStandard        - Supported network modes",									NONE,	0},
{PEER_TRAFFIC_STAT,	"Traffic statistics",																	NONE,	0},
{RETRANS_STAT,		"Re-transmission statistics",															NONE,	0},
{TR181_PEER_STATS,	"General statistics",																	NONE,	0},
{TR181_PEER_STATS,	"ErrorsSent                 - Number of Tx packets not transmitted because of errors",	LONG,	0},
{TR181_PEER_STATS,	"LastDataDownlinkRate       - Last data transmit rate (to peer) [kbps]",				LONG,	0},
{TR181_PEER_STATS,	"LastDataUplinkRate         - Last data receive rate (from peer) [kbps]",				LONG,	0},
{TR181_PEER_STATS,	"RF statistics",																		NONE,	0},
{TR181_PEER_STATS,	"SignalStrength             - Radio signal strength of the uplink [dBm]",				SLONG,	0}
};

stats peer_flow_stats[] = {
{PEER_FLOW_STATS,	"Peer packets flow statistics",												NONE,		0},
{TR181_PEER_STATS,	"TR-181 statistics",														NONE,		0},
{PEER_FLOW_STATS,	"AirtimeEfficiency            - Efficiency of used air-time [bytes/sec]",	LONG,		0},
{PEER_FLOW_STATS,	"AirtimeUsage                 - Air-time used by Tx/Rx to/from peer [%]",	BYTE,		0},
{PEER_FLOW_STATS,	"ShortTermRSSI                - Short-term RSSI average per antenna [dBm]",	SBYTEARRAY,	5},
{PEER_FLOW_STATS,	"SNR                          - Signal to Noise Ratio per antenna [dB]",	SBYTEARRAY,	5},
{PEER_FLOW_STATS,	"LongTermNoise                - Long-term Noise average per antenna [dBm]",	SBYTEARRAY,	5},
{PEER_FLOW_STATS,	"AverageRSSI                  - Average RSSI of all antennas [dBm]",		SBYTEARRAY,	1}
};

stats ml_peer_flow_stats[] = {
{ML_PEER_FLOW_STATS,	"ML peer packets flow statistics",											NONE,		0},
{ML_PEER_FLOW_STATS,	"LinkAddress",																MACADDR,	0},
{TR181_PEER_STATS,		"TR-181 statistics",														NONE,		0},
{ML_PEER_FLOW_STATS,	"AirtimeEfficiency            - Efficiency of used air-time [bytes/sec]",	LONG,		0},
{ML_PEER_FLOW_STATS,	"AirtimeUsage                 - Air-time used by Tx/Rx to/from peer [%]",	BYTE,		0},
{ML_PEER_FLOW_STATS,	"ShortTermRSSI                - Short-term RSSI average per antenna [dBm]",	SBYTEARRAY,	5},
{ML_PEER_FLOW_STATS,	"SNR                          - Signal to Noise Ratio per antenna [dB]",	SBYTEARRAY,	5},
{ML_PEER_FLOW_STATS,	"LongTermNoise                - Long-term Noise average per antenna [dBm]",	SBYTEARRAY,	5},
{ML_PEER_FLOW_STATS,	"AverageRSSI                  - Average RSSI of all antennas [dBm]",		SBYTEARRAY,	1},
{ML_PEER_FLOW_STATS,	"\n",																		NONE,		0},
{ML_PEER_FLOW_STATS,	"LinkAddress",																MACADDR,	0},
{TR181_PEER_STATS,		"TR-181 statistics",														NONE,		0},
{ML_PEER_FLOW_STATS,	"AirtimeEfficiency            - Efficiency of used air-time [bytes/sec]",	LONG,		0},
{ML_PEER_FLOW_STATS,	"AirtimeUsage                 - Air-time used by Tx/Rx to/from peer [%]",	BYTE,		0},
{ML_PEER_FLOW_STATS,	"ShortTermRSSI                - Short-term RSSI average per antenna [dBm]",	SBYTEARRAY,	5},
{ML_PEER_FLOW_STATS,	"SNR                          - Signal to Noise Ratio per antenna [dB]",	SBYTEARRAY,	5},
{ML_PEER_FLOW_STATS,	"LongTermNoise                - Long-term Noise average per antenna [dBm]",	SBYTEARRAY,	5},
{ML_PEER_FLOW_STATS,	"AverageRSSI                  - Average RSSI of all antennas [dBm]",		SBYTEARRAY,	1}
};

stats ml_single_link_peer_flow_stats[] = {
{ML_SINGLE_LINK_PEER_FLOW_STATS,	"ML single peer packets flow statistics",									NONE,		0},
{ML_SINGLE_LINK_PEER_FLOW_STATS,	"LinkAddress",																MACADDR,	0},
{TR181_PEER_STATS,					"TR-181 statistics",														NONE,		0},
{ML_SINGLE_LINK_PEER_FLOW_STATS,	"AirtimeEfficiency            - Efficiency of used air-time [bytes/sec]",	LONG,		0},
{ML_SINGLE_LINK_PEER_FLOW_STATS,	"AirtimeUsage                 - Air-time used by Tx/Rx to/from peer [%]",	BYTE,		0},
{ML_SINGLE_LINK_PEER_FLOW_STATS,	"ShortTermRSSI                - Short-term RSSI average per antenna [dBm]",	SBYTEARRAY,	5},
{ML_SINGLE_LINK_PEER_FLOW_STATS,	"SNR                          - Signal to Noise Ratio per antenna [dB]",	SBYTEARRAY,	5},
{ML_SINGLE_LINK_PEER_FLOW_STATS,	"LongTermNoise                - Long-term Noise average per antenna [dBm]",	SBYTEARRAY,	5},
{ML_SINGLE_LINK_PEER_FLOW_STATS,	"AverageRSSI                  - Average RSSI of all antennas [dBm]",		SBYTEARRAY,	1}
};

stats peer_rate_info1[] = {
{PEER_RATE_INFO1,	"Rate info is valid",	  FLAG,	0},
{PHY_ENUM,		"Network (Phy) Mode",	  NONE,	0},
{PEER_RATE_INFO1,	"BW index",		        SLONG,	0},
{PEER_RATE_INFO1,	"BW [MHz]",		        SLONG,	0},
{PEER_RATE_INFO1,	"SGI",			          SLONG,	0},
{PEER_RATE_INFO1,	"MCS index",		      SLONG,	0},
{PEER_RATE_INFO1,	"NSS",			          SLONG,	0}
};

stats peer_rate_info[] = {
{PEER_RATE_INFO,	"Peer TX/RX info",			NONE,		0},
{PEER_RATE_INFO1,	"Mgmt uplink rate info",		NONE,		0},
{PEER_RATE_INFO,	"Last mgmt uplink rate [Mbps]",		LONGFRACT,	1},
{PEER_RATE_INFO1,	"Data uplink rate info",		NONE,		0},
{PEER_RATE_INFO,	"Last data uplink rate [Mbps]",		LONGFRACT,	1},
{PEER_RATE_INFO1,	"Data downlink rate info",		NONE,		0},
{PEER_RATE_INFO,	"Last data downlink rate [Mbps]",	LONGFRACT,	1},
{PEER_RATE_INFO,	"Beamforming mode",			LONG,		0},
{PEER_RATE_INFO,	"STBC mode",				LONG,		0},
{PEER_RATE_INFO,	"TX power for current rate [dBm]",	LONGFRACT,	2},
{PEER_RATE_INFO,	"TX management power       [dBm]",	LONGFRACT,	2}
};

stats peer_capability[] = {
{PEER_CAPABILITY,	"Peer capabilities",			NONE,		0},
{NETWORK_BITFIELD,	"Supported network modes",		NONE,		0},
{PEER_CAPABILITY,	"WMM is supported",			FLAG,		0},
{PEER_CAPABILITY,	"Channel bonding supported",		FLAG,		0},
{PEER_CAPABILITY,	"SGI20 supported",			FLAG,		0},
{PEER_CAPABILITY,	"SGI40 supported",			FLAG,		0},
{PEER_CAPABILITY,	"STBC supported",	                FLAG,		0}, 
{PEER_CAPABILITY,	"LDPC supported",	                FLAG,		0},
{PEER_CAPABILITY,	"Explicit beam forming supported",	FLAG,		0},
{PEER_CAPABILITY,	"40MHz intolerant",			FLAG,		0},
{VENDOR_ENUM,		"Vendor",				NONE,		0},
{PEER_CAPABILITY,	"Tx STBC support",	        	LONG,		0},
{PEER_CAPABILITY,	"Rx STBC support",      		LONG,		0},
{PEER_CAPABILITY,	"Maximum A-MPDU Length Exponent",	LONG,		0},
{PEER_CAPABILITY,	"Minimum MPDU Start Spacing",		LONG,		0},
{PEER_CAPABILITY,	"Timestamp of station association",	TIMESTAMP,	0}
};

stats recovery_stat[] =
{
{RECOVERY_STAT,		"Recovery statistics",					NONE,	0},
{RECOVERY_STAT,		"Number of FAST recovery processed successfully",	LONG,	0},
{RECOVERY_STAT,		"Number of FULL recovery processed successfully",	LONG,	0},
{RECOVERY_STAT,		"Number of FAST recovery failed",			LONG,	0},
{RECOVERY_STAT,		"Number of FULL recovery failed",			LONG,	0},
{RECOVERY_STAT,		"Number of FAST recovery processed in current timer slot",	LONG,	0},
{RECOVERY_STAT,		"Number of FULL recovery processed in current timer slot",	LONG,	0}
};

stats hw_txm_stat[] = {
{HW_TXM_STAT,		"HW TXM Statistics",					NONE,	0},
{HW_TXM_STAT,		"Number of FW MAN messages sent",			LONG,	0},
{HW_TXM_STAT,		"Number of FW MAN messages confirmed",			LONG,	0},
{HW_TXM_STAT,		"Peak number of FW MAN messages sent simultaneously",	LONG,	0},
{HW_TXM_STAT,		"Number of FW DBG messages sent",			LONG,	0},
{HW_TXM_STAT,		"Number of FW DBG messages confirmed",			LONG,	0},
{HW_TXM_STAT,		"Peak number of FW DBG messages sent simultaneously",	LONG,	0}
};

stats traffic_stats[] = {
{TRAFFIC_STATS,		"Traffic Statistics",				                                NONE,	0},
{TRAFFIC_STATS,		"BytesSent                - Number of bytes sent successfully (64-bit)",	HUGE,	0},
{TRAFFIC_STATS,		"BytesReceived            - Number of bytes received (64-bit)",			HUGE,	0},
{TRAFFIC_STATS,		"PacketsSent              - Number of packets transmitted (64-bit)",		HUGE,	0},
{TRAFFIC_STATS,		"PacketsReceived          - Number of packets received (64-bit)",		HUGE,	0},
{TRAFFIC_STATS,		"UnicastPacketsSent       - Number of unicast packets transmitted",		LONG,	0},
{TRAFFIC_STATS,		"UnicastPacketsReceived   - Number of unicast packets received",		LONG,	0},
{TRAFFIC_STATS,		"MulticastPacketsSent     - Number of multicast packets transmitted",		LONG,	0},
{TRAFFIC_STATS,		"MulticastPacketsReceived - Number of multicast packets received",		LONG,	0},
{TRAFFIC_STATS,		"BroadcastPacketsSent     - Number of broadcast packets transmitted",		LONG,	0},
{TRAFFIC_STATS,		"BroadcastPacketsReceived - Number of broadcast packets received",		LONG,	0},
{TRAFFIC_STATS,		"PD Traffic Statistics",							NONE,	0},
{TRAFFIC_STATS,		"PacketsPDSent            - Number of PD packet sent",				HUGE,	0},
{TRAFFIC_STATS,		"PacketPDByteSent         - Number of PD packet byte sent",			HUGE,	0},
{TRAFFIC_STATS,		"UnicastPacketsPDByteSent - Number of PD packet sent",				HUGE,	0},
{TRAFFIC_STATS,		"UnicastPacketsPDSent     - Number of PD packet sent",				HUGE,	0}
};

stats mgmt_stats[] = {
{MGMT_STATS,		"Management frames statistics",					NONE,	0},
{MGMT_STATS,		"Number of management frames in reserved queue",		LONG,	0},
{MGMT_STATS,		"Number of management frames sent",		                LONG,	0},
{MGMT_STATS,		"Number of management frames confirmed",			LONG,	0},
{MGMT_STATS,		"Number of management frames received",				LONG,	0},
{MGMT_STATS,		"Number of management frames dropped due to retries",		LONG,	0},
{MGMT_STATS,		"Number of management frames dropped due to TX que full",	LONG,	0},
{MGMT_STATS,		"Number of probe responses sent",                         	LONG,0},
{MGMT_STATS,		"Number of probe responses dropped",                      	LONG,0}
};

stats hw_flow_status[] = {
{RECOVERY_STAT,		"HW Recovery Statistics",		NONE,	0},
{HW_TXM_STAT,		"HW TXM statistics",			NONE,	0},
{TRAFFIC_STATS,		"Radio Traffic statistics",		NONE,	0},
{MGMT_STATS,		"Radio MGMT statistics",		NONE,	0},
{HW_FLOW_STATUS,	"Radars detected",				LONG,	0},
{HW_FLOW_STATUS,    "zwdfs_Radars detected",		LONG,	0},
{HW_FLOW_STATUS,	"ZWDFS Antenna RSSI [dBm]",		SLONG, 0},
{HW_FLOW_STATUS,	"Total Airtime Efficiency [bytes/sec]",	LONG,	0},
{HW_FLOW_STATUS,	"Channel Load [%]",				BYTE,	0},
{HW_FLOW_STATUS,	"Channel Utilization [%]",		BYTE,	0},
{HW_FLOW_STATUS,	"Total Airtime [%]",			BYTE,	0}
};

stats tr181_error_stats[] = {
{TR181_ERROR_STATS,	"TR-181 Errors",			                                                NONE,	0},
{TR181_ERROR_STATS,	"ErrorsSent               - Number of Tx packets not transmitted because of errors",	LONG,	0},
{TR181_ERROR_STATS,	"ErrorsReceived           - Number of Rx packets that contained errors",		LONG,	0},
{TR181_ERROR_STATS,	"DiscardPacketsSent       - Number of Tx packets discarded",				LONG,	0},
{TR181_ERROR_STATS,	"DiscardPacketsReceived   - Number of Rx packets discarded",				LONG,	0}
};

stats tr181_wlan_stats[] = {
{TR181_WLAN_STATS,  	"TR-181 Device.WiFi.SSID.{i}.Stats",							NONE,	0},
{TRAFFIC_STATS,		"Traffic Statistics",			                                                NONE,	0},
{TR181_ERROR_STATS,	"Erros Statistics",									NONE,	0},
{RETRANS_STAT,		"Retransmission statistics",								NONE,	0},
{TR181_WLAN_STATS,	"ACKFailureCount             - Number of expected ACKs never received",			LONG,	0},
{TR181_WLAN_STATS,	"AggregatedPacketCount       - Number of aggregated packets transmitted",		LONG,	0},
{TR181_WLAN_STATS,	"UnknownProtoPacketsReceived - Number of Rx packets unknown or unsupported protocol",	LONG,	0}
};

stats tr181_hw_stats[] = {
{TR181_HW_STATS,	"TR-181 Device.WiFi.Radio.{i}.Stats",						NONE,	0},
{TRAFFIC_STATS,		"Traffic Statistics",			                                        NONE,	0},
{TR181_ERROR_STATS,	"Erros Statistics",			                                        NONE,	0},
{TR181_HW_STATS,	"FCSErrorCount            - Number of Rx packets with detected FCS error",	LONG,	0},
{TR181_HW_STATS,	"Noise                    - Average noise strength received [dBm]",		SLONG,	0}
};

stats packet_error_rate[] = {
{PACKET_ERROR_RATE,     "PER per rate",                                                                 NONE,   0},
{PACKET_ERROR_RATE,     "MCS",                                                                          BYTE,   0},
{PACKET_ERROR_RATE,     "NSS",                                                                          BYTE,   0},
{GI_ENUM,               "GI",                                                                           ENUM,   0},
{PACKET_ERROR_RATE,     "BW",                                                                           LONG,   0},
{PACKET_ERROR_RATE,     "PER",                                                                          LONG,   0}
};

stats pta_stats[] = {
{PTA_STATS,		"PTA Stats: Priority GPIO",							NONE,   0},
{PTA_STATS,		"zigbeeStarvationTimerExpired",							LONG,   0},
{PTA_STATS,		"btStarvationTimerExpired",							LONG,   0},
{PTA_STATS,		"zigbeeDenyOverTime",								LONG,   0},
{PTA_STATS,		"btDenyOverTime",								LONG,   0},
{PTA_STATS,		"gpio0TxDemandLow",								LONG,   0},
{PTA_STATS,		"zigbeeGrnatNotUsed",								LONG,   0},
{PTA_STATS,		"btGrnatNotUsed",								LONG,   0},
{PTA_STATS,		"zigbeeGrnatUsed",								LONG,   0},
{PTA_STATS,		"btGrnatUsed",									LONG,   0},
{PTA_STATS,		"zigbeeTxOngoing",								LONG,   0},
{PTA_STATS,		"btTxOngoing",									LONG,   0},
{PTA_STATS,		"gpio",										LONG,   0},
{PTA_STATS,		"PriorityIsHigh",								LONG,   0},
{PTA_STATS,		"PriorityIsLow",								LONG,   0}
};

stats peer_host_if_qos[] = {
{PEER_HOST_IF_QOS,	"QoS Byte Count Sta",								LONGARRAY,   8},
{PEER_HOST_IF_QOS,	"QoS Tx Sta",									LONGARRAY,   8},
};

stats peer_host_if[] = {
{PEER_HOST_IF,		"rxOutStaNumOfBytes",								HUGE,   0},
{PEER_HOST_IF,		"agerPdNoTransmitCountSta ",							HUGE,   0},
};

stats peer_rx_stats[] = {
{PEER_RX_STATS,		"rddelayed",									HUGE,   0},
{PEER_RX_STATS,		"swUpdateDrop",									HUGE,   0},
{PEER_RX_STATS,		"rdDuplicateDrop",								HUGE,   0},
{PEER_RX_STATS,		"missingSn",									HUGE,   0},
};

stats peer_ul_bsrc_tid_stats[] = {
{PEER_UL_BSRC_TID_STATS,"bufStsCnt0",									LONG,   0},
{PEER_UL_BSRC_TID_STATS,"bufStsCnt1",									LONG,   0},
{PEER_UL_BSRC_TID_STATS,"bufStsCnt2",									LONG,   0},
{PEER_UL_BSRC_TID_STATS,"bufStsCnt3",									LONG,   0},
{PEER_UL_BSRC_TID_STATS,"bufStsCnt4",									LONG,   0},
{PEER_UL_BSRC_TID_STATS,"bufStsCnt5",									LONG,   0},
{PEER_UL_BSRC_TID_STATS,"bufStsCnt6",									LONG,   0},
{PEER_UL_BSRC_TID_STATS,"bufStsCnt7",									LONG,   0},
};

stats peer_baa_stats[] = {
{PEER_BAA_STATS,	"mpduFirstRetransmission",							HUGE,   0},
{PEER_BAA_STATS,	"mpduTransmitted",								HUGE,	0},
{PEER_BAA_STATS,	"mpduByteTransmitted",								HUGE,   0},
{PEER_BAA_STATS,	"mpduRetransmission",								HUGE,   0},
{PEER_BAA_STATS,	"channelTransmitTime",								HUGE,   0},
};

stats link_adaption_stats[] = {
{LINK_ADAPTION_STATS,	"DataPhyMode",									LONG,		0},
{LINK_ADAPTION_STATS,	"ManagementPhyMode",								LONG,   	0},
{LINK_ADAPTION_STATS,	"powerData",									LONG,   	0},
{LINK_ADAPTION_STATS,	"powerManagement",								LONG,   	0},
{LINK_ADAPTION_STATS,	"antennaSelectionData",								LONG,   	0},
{LINK_ADAPTION_STATS,	"antennaSelectionManagement",							LONG,   	0},
{LINK_ADAPTION_STATS,	"scpData",									LONG,   	0},
{LINK_ADAPTION_STATS,	"stationCapabilities",								LONG,   	0},
{LINK_ADAPTION_STATS,	"dataBwLimit",									LONG,   	0},
{LINK_ADAPTION_STATS,	"bfModeData",									LONG,   	0},
{LINK_ADAPTION_STATS,	"stbcModeData",									LONG,   	0},
{LINK_ADAPTION_STATS,	"raState",									LONG,   	0},
{LINK_ADAPTION_STATS,	"raStability",									LONG,   	0},
{LINK_ADAPTION_STATS,	"nssData",									LONG,   	0},
{LINK_ADAPTION_STATS,	"nssManagement",								LONG,   	0},
{LINK_ADAPTION_STATS,	"mcsData",									LONG,   	0},
{LINK_ADAPTION_STATS,	"mcsManagement",								LONG,   	0},
{LINK_ADAPTION_STATS,	"bw",										LONG,   	0},
{LINK_ADAPTION_STATS,	"transmittedAmpdu",								LONG,   	0},
{LINK_ADAPTION_STATS,	"totalTxTime",									LONG,   	0},
{LINK_ADAPTION_STATS,	"mpduInAmpdu",									LONG,   	0},
{LINK_ADAPTION_STATS,	"ewmaTimeNsec",									LONG,   	0},
{LINK_ADAPTION_STATS,	"sumTimeNsec",									LONG,   	0},
{LINK_ADAPTION_STATS,	"numMsdus",									LONG,   	0},
{LINK_ADAPTION_STATS,	"per",										LONG,   	0},
{LINK_ADAPTION_STATS,	"totalPsduLength",								LONG,   	0},
{LINK_ADAPTION_STATS,	"totalZld",									LONG,   	0},
{LINK_ADAPTION_STATS,	"ratesMask",									LONGARRAY,	2},
};

stats plan_manager_stats[] = {
{PLAN_MANAGER_STATS,	"uplinkMuType",									LONG,   0},
{PLAN_MANAGER_STATS,	"ulRuSubChannels",								LONG,	0},
{PLAN_MANAGER_STATS,	"ulRuType",									LONG,   0},
{PLAN_MANAGER_STATS,	"downlinkMuType",								LONG,   0},
{PLAN_MANAGER_STATS,	"dlRuSubChannels",								LONG,   0},
{PLAN_MANAGER_STATS,	"dlRuType",									LONG,   0},
};

stats twt_stats[] = {
{TWT_STATS,		"numOfAgreementsForSta",							LONG,	0},
{TWT_STATS,		"state",									LONG,   0},
{TWT_STATS,		"agreementType",								LONG,   0},
{TWT_STATS,		"implicit",									LONG,   0},
{TWT_STATS,		"announced",									LONG,   0},
{TWT_STATS,		"trigger enabled",								LONG,   0},
{TWT_STATS,		"individual wakeTime",								LONG,   0},
{TWT_STATS,		"individual wakeInterval",							LONG,   0},
{TWT_STATS,		"individual minWakeDuration",							LONG,   0},
{TWT_STATS,		"individual channel",								LONG,   0},
};

stats per_client_stats[] = {
{PER_CLIENT_STATS,	"retryCount",									HUGE,   0},
{PER_CLIENT_STATS,	"successCount",									HUGE,	0},
{PER_CLIENT_STATS,	"exhaustedCount",								HUGE,   0},
{PER_CLIENT_STATS,	"clonedCount",									HUGE,   0},
{PER_CLIENT_STATS,	"oneOrMoreRetryCount",								HUGE,   0},
{PER_CLIENT_STATS,	"packetRetransCount",								HUGE,   0},
{PER_CLIENT_STATS,	"dropCntReasonClassifier",							HUGE,   0},
{PER_CLIENT_STATS,	"dropCntReasonDisconnect",							HUGE,   0},
{PER_CLIENT_STATS,	"dropCntReasonATF",								HUGE,   0},
{PER_CLIENT_STATS,	"dropCntReasonTSFlush",								HUGE,   0},
{PER_CLIENT_STATS,	"dropCntReasonReKey",								HUGE,   0},
{PER_CLIENT_STATS,	"dropCntReasonSetKey",								HUGE,   0},
{PER_CLIENT_STATS,	"dropCntReasonDiscard",								HUGE,   0},
{PER_CLIENT_STATS,	"dropCntReasonDsabled",								HUGE,   0},
{PER_CLIENT_STATS,	"dropCntReasonAggError",							HUGE,   0},
{PER_CLIENT_STATS,	"mpduRetryCount",								HUGE,   0},
{PER_CLIENT_STATS,	"mpduInAmpdu",									HUGE,   0},
{PER_CLIENT_STATS,	"ampdu",									HUGE,   0},
};

stats peer_phy_rx_status[] = {
{PEER_PHY_RX_STATUS,	"rssi",										SLONG,		0},
{PEER_PHY_RX_STATUS,	"rcpi_avg",									SLONG,		0},
{PEER_PHY_RX_STATUS,	"phyRate",									LONG,		0},
{PEER_PHY_RX_STATUS,	"irad",										LONG,		0},
{PEER_PHY_RX_STATUS,	"lastTsf",									LONG,		0},
{PEER_PHY_RX_STATUS,	"perClientRxtimeUsage",								LONG,		0},
{PEER_PHY_RX_STATUS,	"psduRate",									LONG,		0},
{PEER_PHY_RX_STATUS,	"phyRateSynchedToPsduRate",							LONG,		0},
{PEER_PHY_RX_STATUS,	"noise",									SBYTEARRAY,	5},
{PEER_PHY_RX_STATUS,	"gain",										BYTEARRAY,	5},
{PEER_PHY_RX_STATUS,	"rcpi",										SBYTEARRAY,	5},
};

stats peer_info[] = {
{PEER_HOST_IF_QOS,	"Peer Host If Qos",								NONE,	0},
{PEER_HOST_IF,		"Peer Host If",									NONE,	0},
{PEER_RX_STATS,		"Peer Rx Stats",								NONE,	0},
{PEER_UL_BSRC_TID_STATS,"Peer UL BSRC TID Stats",							NONE,	0},
{PEER_BAA_STATS,	"Peer Baa Stats",								NONE,	0},
{PLAN_MANAGER_STATS,	"Plan Manager Stats",								NONE,	0},
{TWT_STATS,		"TWT Stats",									NONE,	0},
{PER_CLIENT_STATS,	"Per Client Status",								NONE,	0},
{PEER_PHY_RX_STATUS,	"Phy Rx status",								NONE,	0},
};

stats wlan_host_if_qos[] = {
{WLAN_HOST_IF_QOS,	"qosTxVap",									LONGARRAY,	4},
};

stats wlan_host_if[] = {
{WLAN_HOST_IF,		"txInUnicastHd",								LONG,	0},
{WLAN_HOST_IF,		"txInMulticastHd",								LONG,	0},
{WLAN_HOST_IF,		"txInBroadcastHd",								LONG,	0},
{WLAN_HOST_IF,		"txInUnicastNumOfBytes",							LONG,	0},
{WLAN_HOST_IF,		"txInMulticastNumOfBytes",							LONG,	0},
{WLAN_HOST_IF,		"txInBroadcastNumOfBytes",							LONG,	0},
{WLAN_HOST_IF,		"rxOutUnicastHd",								LONG,	0},
{WLAN_HOST_IF,		"rxOutMulticastHd",								LONG,	0},
{WLAN_HOST_IF,		"rxOutBroadcastHd",								LONG,	0},
{WLAN_HOST_IF,		"rxOutUnicastNumOfBytes",							LONG,	0},
{WLAN_HOST_IF,		"rxOutMulticastNumOfBytes",							LONG,	0},
{WLAN_HOST_IF,		"rxOutBroadcastNumOfBytes",							LONG,	0},
{WLAN_HOST_IF,		"agerCount",									LONG,	0},
};

stats wlan_rx_stats[] = {
{WLAN_RX_STATS,		"amsdu",									LONG,	0},
{WLAN_RX_STATS,		"amsduBytes",									LONG,	0},
{WLAN_RX_STATS,		"dropCount",									LONG,	0},
{WLAN_RX_STATS,		"mpduInAmpdu",									LONG,	0},
{WLAN_RX_STATS,		"octetsInAmpdu",								LONG,	0},
{WLAN_RX_STATS,		"rxCoorSecurityMismatch",							LONG,	0},
{WLAN_RX_STATS,		"tkipCount",									LONG,	0},
{WLAN_RX_STATS,		"securityFailure",								LONG,	0},
{WLAN_RX_STATS,		"mpduUnicastOrMngmnt",								LONG,	0},
{WLAN_RX_STATS,		"mpduRetryCount",								LONG,	0},
{WLAN_RX_STATS,		"dropMpdu",									LONG,	0},
{WLAN_RX_STATS,		"ampdu",									LONG,	0},
{WLAN_RX_STATS,		"mpduTypeNotSupported",								LONG,	0},
{WLAN_RX_STATS,		"replayData",									LONG,	0},
{WLAN_RX_STATS,		"replayMngmnt",									LONG,	0},
{WLAN_RX_STATS,		"bcMcCountVap",									LONG,	0},
};

stats wlan_baa_stats[] = {
{WLAN_BAA_STATS,	"rtsSuccessCount",								LONG,	0},
{WLAN_BAA_STATS,	"rtsFailure",									LONG,	0},
{WLAN_BAA_STATS,	"transmitStreamRprtMSDUFailed",							LONG,	0},
{WLAN_BAA_STATS,	"qosTransmittedFrames",								LONG,	0},
{WLAN_BAA_STATS,	"transmittedAmsdu",								LONG,	0},
{WLAN_BAA_STATS,	"transmittedOctetsInAmsdu",							LONG,	0},
{WLAN_BAA_STATS,	"transmittedAmpdu",								LONG,	0},
{WLAN_BAA_STATS,	"transmittedMpduInAmpdu",							LONG,	0},
{WLAN_BAA_STATS,	"transmittedOctetsInAmpdu",							LONG,	0},
{WLAN_BAA_STATS,	"beamformingFrames",								LONG,	0},
{WLAN_BAA_STATS,	"ackFailure",									LONG,	0},
{WLAN_BAA_STATS,	"failedAmsdu",									LONG,	0},
{WLAN_BAA_STATS,	"retryAmsdu",									LONG,	0},
{WLAN_BAA_STATS,	"multipleRetryAmsdu",								LONG,	0},
{WLAN_BAA_STATS,	"amsduAckFailure",								LONG,	0},
{WLAN_BAA_STATS,	"implicitBarFailure",								LONG,	0},
{WLAN_BAA_STATS,	"explicitBarFailure",								LONG,	0},
{WLAN_BAA_STATS,	"transmitStreamRprtMultipleRetryCount",						LONG,	0},
{WLAN_BAA_STATS,	"transmitBw20",									LONG,	0},
{WLAN_BAA_STATS,	"transmitBw40",									LONG,	0},
{WLAN_BAA_STATS,	"transmitBw80",									LONG,	0},
{WLAN_BAA_STATS,	"transmitBw160",								LONG,	0},
{WLAN_BAA_STATS,	"transmitBw320",								LONG,	0},
{WLAN_BAA_STATS,	"rxGroupFrame",									LONG,	0},
{WLAN_BAA_STATS,	"txSenderError",								LONG,	0},
};

stats radio_rx_stats[] = {
{RADIO_RX_STATS,	"barMpduCount",									LONG,	0},
{RADIO_RX_STATS,	"crcErrorCount",								LONG,	0},
{RADIO_RX_STATS,	"delCrcError",									LONG,	0},
};

stats radio_baa_stats[] = {
{RADIO_BAA_STATS,	"retryCount",									LONG,	0},
{RADIO_BAA_STATS,	"multipleRetryCount",								LONG,	0},
{RADIO_BAA_STATS,	"fwMngmntframesSent",								LONG,	0},
{RADIO_BAA_STATS,	"fwMngmntFramesConfirmed",							LONG,	0},
{RADIO_BAA_STATS,	"fwMngmntframesRecieved",							LONG,	0},
{RADIO_BAA_STATS,	"fwctrlFramesSent",								LONG,	0},
{RADIO_BAA_STATS,	"fwctrlFramesRecieved",								LONG,	0},
{RADIO_BAA_STATS,	"fwMulticastReplayedPackets",							LONG,	0},
{RADIO_BAA_STATS,	"fwPairWiseMicFailurePackets",							LONG,	0},
{RADIO_BAA_STATS,	"groupMicFailurePackets",							LONG,	0},
{RADIO_BAA_STATS,	"beaconProbeResponsePhyType",							LONG,	0},
{RADIO_BAA_STATS,	"rxBasicReport",								LONG,	0},
{RADIO_BAA_STATS,	"txFrameErrorCount",								LONG,	0},
};

stats ts_init_tid_gl[] = {
{TS_INIT_TID_GL,	"InitTidStatsEventAddbaRequestWasSent",						LONG,	0},
{TS_INIT_TID_GL,	"InitTidStatsEventAddbaRequestConfiremd",					LONG,	0},
{TS_INIT_TID_GL,	"InitTidStatsEventAddbaResponseReceivedWithStatusSuccessAndValid",		LONG,	0},
{TS_INIT_TID_GL,	"InitTidStatsEventAddbaResponseReceivedWithStatusSuccessAndInvalid",		LONG,	0},
{TS_INIT_TID_GL,	"InitTidStatsEventAddbaResponseReceivedWithStatusFailure",			LONG,	0},
{TS_INIT_TID_GL,	"InitTidStatsEventAddbaResponseTimeout",					LONG,	0},
{TS_INIT_TID_GL,	"InitTidStatsEventBaAgreementOpened",						LONG,	0},
{TS_INIT_TID_GL,	"InitTidStatsEventBarWasSent",							LONG,	0},
{TS_INIT_TID_GL,	"InitTidStatsEventBarWasDiscarded",						LONG,	0},
{TS_INIT_TID_GL,	"InitTidStatsEventBarWasAnsweredWithAck",					LONG,	0},
{TS_INIT_TID_GL,	"InitTidStatsEventBarWasAnsweredWithBa",					LONG,	0},
{TS_INIT_TID_GL,	"InitTidStatsEventInactivityTimeout",						LONG,	0},
{TS_INIT_TID_GL,	"InitTidStatsEventDelbaWasSent",						LONG,	0},
{TS_INIT_TID_GL,	"InitTidStatsEventDelbaReceived",						LONG,	0},
{TS_INIT_TID_GL,	"InitTidStatsEventCloseTid",							LONG,	0},
{TS_INIT_TID_GL,	"InitTidStatsEventOpenTid",							LONG,	0},
};

stats ts_init_sta_gl[] = {
{TS_INIT_STA_GL,	"InitStaStatsEventRemove",							LONG,	0},
{TS_INIT_STA_GL,	"InitStaStatsEventAdd",								LONG,	0},
{TS_INIT_STA_GL,	"InitStaStatsEventAddbaResponseReceivedWithIllegalTid",				LONG,	0},
};

stats ts_rcpt_tid_gl[] = {
{TS_RCPT_TID_GL,	"RcpTidStatsEventAddbaRequestAccepted",						LONG,	0},
{TS_RCPT_TID_GL,	"RcpTidStatsEventAddbaRequestWasReceivedWithInvalidParameters",			LONG,	0},
{TS_RCPT_TID_GL,	"RcpTidStatsEventAddbaResponseWasSentWithStatusSuccess",			LONG,	0},
{TS_RCPT_TID_GL,	"RcpTidStatsEventAddbaResponseWasSentWithStatusFailure",			LONG,	0},
{TS_RCPT_TID_GL,	"RcpTidStatsEventBaAgreementOpened",						LONG,	0},
{TS_RCPT_TID_GL,	"RcpTidStatsEventBaAgreementClosed",						LONG,	0},
{TS_RCPT_TID_GL,	"RcpTidStatsEventBarWasReceivedWithBaAgreementOpened",				LONG,	0},
{TS_RCPT_TID_GL,	"RcpTidStatsEventInactivityTimeout",						LONG,	0},
{TS_RCPT_TID_GL,	"RcpTidStatsEventDelbaReceived",						LONG,	0},
{TS_RCPT_TID_GL,	"RcpTidStatsEventDelbaWasSent",							LONG,	0},
{TS_RCPT_TID_GL,	"RcpTidStatsEventIllegalAggregationWasReceived",				LONG,	0},
{TS_RCPT_TID_GL,	"RcpTidStatsEventCloseTid",							LONG,	0},
};

stats ts_rcpt_sta_gl[] = {
{TS_RCPT_STA_GL,	"RcptStaStatsEventRemove",							LONG,	0},
{TS_RCPT_STA_GL,	"RcptStaStatsEventAdd",								LONG,	0},
{TS_RCPT_STA_GL,	"RcptStaStatsEventAddbaRequestReceivedWithIllegalTid",				LONG,	0},
};

stats radio_link_adapt_stats[] = {
{RADIO_LINK_ADAPT_STATS,"minPower",									LONGARRAY,	4},
{RADIO_LINK_ADAPT_STATS,"maxPower",									LONGARRAY,	4},
{RADIO_LINK_ADAPT_STATS,"bfReportReceivedCounter",							LONG,		0},
{RADIO_LINK_ADAPT_STATS,"protectionSentCounter",							LONG,		0},
{RADIO_LINK_ADAPT_STATS,"protectionSucceededCounter",							LONG,		0},
{RADIO_LINK_ADAPT_STATS,"rxDc",										LONGARRAY,	4},
{RADIO_LINK_ADAPT_STATS,"txLo",										LONGARRAY,	4},
{RADIO_LINK_ADAPT_STATS,"rxIq",										LONGARRAY,	4},
{RADIO_LINK_ADAPT_STATS,"txIq",										LONGARRAY,	4},
};

stats multicast_stats[] = {
{MULTICAST_STATS,	"notEnoughClonePds",								LONG,	0},
{MULTICAST_STATS,	"allClonesFinishedTransmission",						LONG,	0},
{MULTICAST_STATS,	"noStationsInGroup",								LONG,	0},
};

stats train_man_stats[] = {
{TRAIN_MAN_STATS,	"trainingTimerExpiredCounter",							LONG,	0},
{TRAIN_MAN_STATS,	"trainingStartedCounter",							LONG,	0},
{TRAIN_MAN_STATS,	"trainingFinishedSuccessfullyCounter",						LONG,	0},
{TRAIN_MAN_STATS,	"trainingFinishedUnsuccessfullyCounter",					LONG,	0},
{TRAIN_MAN_STATS,	"trainingNotStartedCounter",							LONG,	0},
};

stats group_man_stats[] = {
{GROUP_MAN_STATS,	"groupInfoDb",									LONG,	0},
};

stats general_stats[] = {
{GENERAL_STATS,		"calibrationMask",								LONG,	0},
{GENERAL_STATS,		"onlineCalibrationMask",							LONG,	0},
{GENERAL_STATS,		"dropReasonClassifier",								LONG,	0},
{GENERAL_STATS,		"dropReasonDisconnect",								LONG,	0},
{GENERAL_STATS,		"dropReasonATF",								LONG,	0},
{GENERAL_STATS,		"dropReasonTSFlush",								LONG,	0},
{GENERAL_STATS,		"dropReasonReKey",								LONG,	0},
{GENERAL_STATS,		"dropReasonSetKey",								LONG,	0},
{GENERAL_STATS,		"dropReasonDiscard",								LONG,	0},
{GENERAL_STATS,		"dropReasonDsabled",								LONG,	0},
{GENERAL_STATS,		"dropReasonAggError",								LONG,	0},
{GENERAL_STATS,		"defragStart",									LONG,	0},
{GENERAL_STATS,		"defragEnd",									LONG,	0},
{GENERAL_STATS,		"defragTimeout",								LONG,	0},
{GENERAL_STATS,		"classViolationErrors",								LONG,	0},
{GENERAL_STATS,		"rxhErrors",									LONG,	0},
{GENERAL_STATS,		"rxSmps",									LONG,	0},
{GENERAL_STATS,		"rxOmn",									LONG,	0},
{GENERAL_STATS,		"txMngFromHost",								LONG,	0},
{GENERAL_STATS,		"txDataFromHos",								LONG,	0},
{GENERAL_STATS,		"cyclicBufferOverflow",								LONG,	0},
{GENERAL_STATS,		"beaconsTransmitted",								LONG,	0},
{GENERAL_STATS,		"debugStatistics",								LONG,	0},
{GENERAL_STATS,		"exceedsETSIMaxSizeUnicast",								LONG,	0},
{GENERAL_STATS,		"exceedsETSIMaxSizeMulticast",								LONG,	0},
};

stats channel_stats[] = {
{CHANNEL_STATS,		"chUtilizationActive",								HUGE,	0},
{CHANNEL_STATS,		"chUtilizationBusy",								HUGE,	0},
{CHANNEL_STATS,		"chUtilizationBusyTx",								HUGE,	0},
{CHANNEL_STATS,		"chUtilizationBusyRx",								HUGE,	0},
{CHANNEL_STATS,		"chUtilizationBusySelf",							HUGE,	0},
{CHANNEL_STATS,		"chUtilizationInterf",								HUGE,	0},
{CHANNEL_STATS,		"chUtilizationIdle",								HUGE,	0},
{CHANNEL_STATS,		"chUtilizationBusyExt",								HUGE,	0},
{CHANNEL_STATS,		"chUtilization [%]",								LONG,	0},
{CHANNEL_STATS,		"channelMax80211Rssi [dBm]",							SLONG,	0},
{CHANNEL_STATS,		"channelNum",									LONG,	0},
};

stats radio_phy_rx_stats[] = {
{RADIO_PHY_RX_STATS,	"channel_load",									LONG,		0},
{RADIO_PHY_RX_STATS,	"chNon80211Noise",								SLONG,		0},
{RADIO_PHY_RX_STATS,	"CWIvalue",									SLONG,		0},
{RADIO_PHY_RX_STATS,	"channelNum",									LONG,		0},
{RADIO_PHY_RX_STATS,	"txPower",									SLONG,		0},
{RADIO_PHY_RX_STATS,	"irad",										LONG,		0},
{RADIO_PHY_RX_STATS,	"tsf",										LONG,		0},
{RADIO_PHY_RX_STATS,	"extStaRx",									LONG,		0},
{RADIO_PHY_RX_STATS,	"noise",									SBYTEARRAY,	5},
{RADIO_PHY_RX_STATS,	"rf_gain",									BYTEARRAY,	5},
{RADIO_PHY_RX_STATS,	"Background Noise [dBm]",							SBYTEARRAY,	5},
{RADIO_PHY_RX_STATS,	"Active Antenna Mask",								HEXBYTE,	0},
};

stats dynamic_bw_stats[] = {
{DYNAMIC_BW_STATS,	"dynamicBW20MHz",								LONG,	0},
{DYNAMIC_BW_STATS,	"dynamicBW40MHz",								LONG,	0},
{DYNAMIC_BW_STATS,	"dynamicBW80MHz",								LONG,	0},
{DYNAMIC_BW_STATS,	"dynamicBW160MHz",								LONG,	0},
};

stats la_mu_vht_stats[] = {
{LA_MU_VHT_STATS,	"protectionSentCounter",							LONG,		0},
{LA_MU_VHT_STATS,	"protectionSucceededCounter",							LONG,		0},
{LA_MU_VHT_STATS,	"DataPhyMode",									LONG,		0},
{LA_MU_VHT_STATS,	"powerData",									LONGARRAY,	4},
{LA_MU_VHT_STATS,	"scpData",									LONGARRAY,	4},
{LA_MU_VHT_STATS,	"dataBwLimit",									LONG,		0},
{LA_MU_VHT_STATS,	"groupGoodput",									LONG,		0},
{LA_MU_VHT_STATS,	"raState",									LONG,		0},
{LA_MU_VHT_STATS,	"raStability",									LONG,		0},
{LA_MU_VHT_STATS,	"nssData",									LONGARRAY,	4},
{LA_MU_VHT_STATS,	"mcsData",									LONGARRAY,	4},
};

stats cca_stats[] = {
{CCA_STATS,	"overflowOccurred",									LONG,		0},
{CCA_STATS,	"ccaActive",										LONG,		0},
{CCA_STATS,	"ccaTx",										LONG,		0},
{CCA_STATS,	"ccaRx",										LONG,		0},
{CCA_STATS,	"ccaIntf",										LONG,		0},
{CCA_STATS,	"ccaIdle",										LONG,		0},
};


/* Associated Device Diagnostic Result3 */

stats peer_diag_result3[] = {
/* wifiAssociatedDevDiagnostic3_t */
{PEER_DIAG_RESULT3,	"Associated Device Diagnostic Result3",	NONE,	0},
{PEER_DIAG_RESULT3,	"PacketsSent        (64-bit)",	HUGE,		0},
{PEER_DIAG_RESULT3,	"PacketsReceived    (64-bit)",	HUGE,		0},
{PEER_DIAG_RESULT3,	"ErrorsSent         (64-bit)",	HUGE,		0},
{PEER_DIAG_RESULT3,	"RetransCount       (64-bit)",	HUGE,		0},
{PEER_DIAG_RESULT3,	"FailedRetransCoun  (64-bit)",	HUGE,		0},
{PEER_DIAG_RESULT3,	"RetryCount         (64-bit)",	HUGE,		0},
{PEER_DIAG_RESULT3,	"MultipleRetryCount (64-bit)",	HUGE,		0},
{PEER_DIAG_RESULT3,	"TxFrames           (64-bit)",	HUGE,		0},
{PEER_DIAG_RESULT3,	"RxRetries          (64-bit)",	HUGE,		0},
{PEER_DIAG_RESULT3,	"RxErrors           (64-bit)",	HUGE,		0},
{PEER_DIAG_RESULT3,	"MaxDownlinkRate",				LONG,		0},
{PEER_DIAG_RESULT3,	"MaxUplinkRate",				LONG,		0},
{PEER_DIAG_RESULT3,	"ActiveNumSpatialStreams",		LONG,		0},

/* wifiAssociatedDevDiagnostic2_t is part of wifiAssociatedDevDiagnostic3_t */
{PEER_DIAG_RESULT3,	"DataFramesSentAck",		LONG,		0},
{PEER_DIAG_RESULT3,	"DataFramesSentNoAck",		LONG,		0},
{PEER_DIAG_RESULT3,	"BytesSent",			LONG,		0},
{PEER_DIAG_RESULT3,	"BytesReceived",		LONG,		0},
{PEER_DIAG_RESULT3,	"LastDataDownlinkRate",		LONG,		0},
{PEER_DIAG_RESULT3,	"LastDataUplinkRate",		LONG,		0},
{PEER_DIAG_RESULT3,	"Retransmissions",		LONG,		0},
{PEER_DIAG_RESULT3,	"Disassociations",		LONG,		0},
{PEER_DIAG_RESULT3,	"AuthenticationFailures",	LONG,		0},
{PEER_DIAG_RESULT3,	"Associations",			LONG,		0},
{PEER_DIAG_RESULT3,	"SignalStrength",		SLONG,		0},
{PEER_DIAG_RESULT3,	"RSSI",				SLONGARRAY,	4},
{PEER_DIAG_RESULT3,	"MinRSSI",			SLONG,		0},
{PEER_DIAG_RESULT3,	"MaxRSSI",			SLONG,		0},
{PEER_DIAG_RESULT3,	"IPAddress",			STRING,		64},
{PEER_DIAG_RESULT3,	"OperatingStandard",		STRING,		64},
{PEER_DIAG_RESULT3,	"OperatingChannelBandwidth",	STRING,		64},
{PEER_DIAG_RESULT3,	"InterferenceSources",		STRING,		64},
{PEER_DIAG_RESULT3,	"SNR",				SBYTEARRAY,	4},
{PEER_DIAG_RESULT3,	"MACAddress",			MACADDR,	0},
{PEER_DIAG_RESULT3,	"AuthenticationState",		BOOLEAN,	0},
{PEER_DIAG_RESULT3,	"Active",			BOOLEAN,	0},

/* Not filled yet */
#if 0
{PEER_DIAG_RESULT3,	"UL MU statistic",		RESERVED,	856},
{PEER_DIAG_RESULT3,	"DL MU statistic",		RESERVED,	856},
{PEER_DIAG_RESULT3,	"TWT Params",			RESERVED,	24},
#endif
};

stats peer_mixed_stats[] = {
{PEER_MIXED_STATS,	"num_rates",		LONG,		0},
{PEER_MIXED_STATS,	"rates_kbps",		LONGARRAY,	32},
{PEER_MIXED_STATS,	"rate_max",			LONG,		0},
{PEER_MIXED_STATS,	"rate_1ss",			LONG,		0},
{PEER_MIXED_STATS,	"ant_max",			LONG,		0},
{NETWORK_BITFIELD,	"net_modes",		NONE,		0},
{PEER_MIXED_STATS,	"asleep",			LONG,		0},
{PEER_MIXED_STATS,	"in_network_sec",	LONG,		0},
{PEER_MIXED_STATS,	"idle_sec",			LONG,		0},
{PEER_MIXED_STATS,	"tx_packets",		HUGE,		0},
{PEER_MIXED_STATS,	"rx_packets",		HUGE,		0},
};

stats csi_counters[] = {
{CSI_COUNTERS,	"CSI_Send_QoS_NULL_Count",		HUGE,		0},
{CSI_COUNTERS,	"CSI_Recv_Frame_Count",			HUGE,		0},
{CSI_COUNTERS,	"CSI_Send_NL_CSI_DATA",			HUGE,		0},
{CSI_COUNTERS,	"CSI_Req_Info_Count",			HUGE,		0},
};

struct print_struct gStat[] = {
{PEER_FLOW_STATS,	peer_flow_stats,	num_stats(peer_flow_stats)},
{PEER_TRAFFIC_STAT,	peer_traffic_stat,	num_stats(peer_traffic_stat)},
{RETRANS_STAT,		peer_retrans_stat,	num_stats(peer_retrans_stat)},
{TR181_PEER_STATS,	tr181_peer_stat,	num_stats(tr181_peer_stat)},
{NETWORK_BITFIELD,	network_bitfield,	num_stats(network_bitfield)},
{PEER_CAPABILITY,	peer_capability,	num_stats(peer_capability)},
{VENDOR_ENUM,		vendor_enum,		num_stats(vendor_enum)},
{PEER_RATE_INFO,	peer_rate_info,		num_stats(peer_rate_info)},
{PEER_RATE_INFO1,	peer_rate_info1,	num_stats(peer_rate_info1)},
{PHY_ENUM,		phy_enum,		num_stats(phy_enum)},
{RECOVERY_STAT,		recovery_stat,		num_stats(recovery_stat)},
{HW_TXM_STAT,		hw_txm_stat,		num_stats(hw_txm_stat)},
{TRAFFIC_STATS,		traffic_stats,		num_stats(traffic_stats)},
{MGMT_STATS,		mgmt_stats,		num_stats(mgmt_stats)},
{HW_FLOW_STATUS,	hw_flow_status,		num_stats(hw_flow_status)},
{TR181_ERROR_STATS,	tr181_error_stats,	num_stats(tr181_error_stats)},
{TR181_WLAN_STATS,	tr181_wlan_stats,	num_stats(tr181_wlan_stats)},
{TR181_HW_STATS,	tr181_hw_stats,		num_stats(tr181_hw_stats)},
{PACKET_ERROR_RATE,	packet_error_rate,	num_stats(packet_error_rate)},
{GI_ENUM,			gi_enum,			num_stats(gi_enum)},
{PTA_STATS,			pta_stats,			num_stats(pta_stats)},
{PEER_HOST_IF_QOS,	peer_host_if_qos,	num_stats(peer_host_if_qos)},
{PEER_HOST_IF,		peer_host_if,		num_stats(peer_host_if)},
{PEER_RX_STATS,		peer_rx_stats,		num_stats(peer_rx_stats)},
{PEER_UL_BSRC_TID_STATS,peer_ul_bsrc_tid_stats,	num_stats(peer_ul_bsrc_tid_stats)},
{PEER_BAA_STATS,	peer_baa_stats,		num_stats(peer_baa_stats)},
{LINK_ADAPTION_STATS,	link_adaption_stats,	num_stats(link_adaption_stats)},
{PLAN_MANAGER_STATS,	plan_manager_stats,	num_stats(plan_manager_stats)},
{TWT_STATS,		twt_stats,		num_stats(twt_stats)},
{PER_CLIENT_STATS,	per_client_stats,	num_stats(per_client_stats)},
{PEER_PHY_RX_STATUS,	peer_phy_rx_status,	num_stats(peer_phy_rx_status)},
{PEER_INFO,		peer_info,		num_stats(peer_info)},
{WLAN_HOST_IF_QOS,	wlan_host_if_qos,	num_stats(wlan_host_if_qos)},
{WLAN_HOST_IF,		wlan_host_if,		num_stats(wlan_host_if)},
{WLAN_RX_STATS,		wlan_rx_stats,		num_stats(wlan_rx_stats)},
{WLAN_BAA_STATS,	wlan_baa_stats,		num_stats(wlan_baa_stats)},
{RADIO_RX_STATS,	radio_rx_stats,		num_stats(radio_rx_stats)},
{RADIO_BAA_STATS,	radio_baa_stats,	num_stats(radio_baa_stats)},
{TS_INIT_TID_GL,	ts_init_tid_gl,		num_stats(ts_init_tid_gl)},
{TS_INIT_STA_GL,	ts_init_sta_gl,		num_stats(ts_init_sta_gl)},
{TS_RCPT_TID_GL,	ts_rcpt_tid_gl,		num_stats(ts_rcpt_tid_gl)},
{TS_RCPT_STA_GL,	ts_rcpt_sta_gl,		num_stats(ts_rcpt_sta_gl)},
{RADIO_LINK_ADAPT_STATS,radio_link_adapt_stats,	num_stats(radio_link_adapt_stats)},
{MULTICAST_STATS,       multicast_stats,	num_stats(multicast_stats)},
{TRAIN_MAN_STATS,       train_man_stats,	num_stats(train_man_stats)},
{GROUP_MAN_STATS,       group_man_stats,	num_stats(group_man_stats)},
{GENERAL_STATS,		general_stats,		num_stats(general_stats)},
{CHANNEL_STATS,		channel_stats,		num_stats(channel_stats)},
{RADIO_PHY_RX_STATS,	radio_phy_rx_stats,	num_stats(radio_phy_rx_stats)},
{DYNAMIC_BW_STATS,	dynamic_bw_stats,	num_stats(dynamic_bw_stats)},
{LA_MU_VHT_STATS,	la_mu_vht_stats,	num_stats(la_mu_vht_stats)},
{CCA_STATS,		cca_stats,		num_stats(cca_stats)},
{PEER_DIAG_RESULT3,	peer_diag_result3,	num_stats(peer_diag_result3)},
{PEER_MIXED_STATS,	peer_mixed_stats,	num_stats(peer_mixed_stats)},
{CSI_COUNTERS,	csi_counters,	num_stats(csi_counters)},
{ML_PEER_FLOW_STATS,	ml_peer_flow_stats,	num_stats(ml_peer_flow_stats)},
{ML_SINGLE_LINK_PEER_FLOW_STATS,	ml_single_link_peer_flow_stats,	num_stats(ml_single_link_peer_flow_stats)},
};

stats_cmd gCmd[] =
{
{"peerlist", LTQ_NL80211_VENDOR_SUBCMD_GET_PEER_LIST, 1, 
		"usage: dwpal_cli [INTERFACENAME] PeerList",PEER_LIST},
{"peerflowstatus", LTQ_NL80211_VENDOR_SUBCMD_GET_PEER_FLOW_STATUS, 2, 
		"usage: dwpal_cli [INTERFACENAME] PeerFlowStatus [MACADDR]", PEER_FLOW_STATS},
{"peercapabilities", LTQ_NL80211_VENDOR_SUBCMD_GET_PEER_CAPABILITIES, 2, 
		"usage: dwpal_cli [INTERFACENAME] PeerCapabilities [MACADDR]", PEER_CAPABILITY},
{"peerratesinfo", LTQ_NL80211_VENDOR_SUBCMD_GET_PEER_RATE_INFO, 2, 
		"usage: dwpal_cli [INTERFACENAME] PeerRatesInfo [MACADDR]", PEER_RATE_INFO},
{"recoverystats", LTQ_NL80211_VENDOR_SUBCMD_GET_RECOVERY_STATS, 1,
		"usage: dwpal_cli [INTERFACENAME] RecoveryStats", RECOVERY_STAT},
{"hwflowstatus", LTQ_NL80211_VENDOR_SUBCMD_GET_HW_FLOW_STATUS, 1,
		"usage: dwpal_cli [INTERFACENAME] HWFlowStatus", HW_FLOW_STATUS},
{"tr181wlanstat", LTQ_NL80211_VENDOR_SUBCMD_GET_TR181_WLAN_STATS, 1,
		"usage: dwpal_cli [INTERFACENAME] TR181WLANStat", TR181_WLAN_STATS},
{"tr181hwstat", LTQ_NL80211_VENDOR_SUBCMD_GET_TR181_HW_STATS, 1,
		"usage: dwpal_cli [INTERFACENAME] TR181HWStat", TR181_HW_STATS},
{"tr181peerstat", LTQ_NL80211_VENDOR_SUBCMD_GET_TR181_PEER_STATS, 2,
		"usage: dwpal_cli [INTERFACENAME] TR181PeerStat [MACADDR]", TR181_PEER_STATS},
{"packeterrorrate", -1, 2,
		"usage: dwpal_cli [INTERFACENAME] PacketErrorRate [MACADDR]", PACKET_ERROR_RATE},
{"ptastats", LTQ_NL80211_VENDOR_SUBCMD_GET_PRIORITY_GPIO, 1,
		"usage: dwpal_cli [INTERFACENAME] PTAStats", PTA_STATS},
{"peerhostifqos", LTQ_NL80211_VENDOR_SUBCMD_GET_PEER_HOST_IF_QOS, 2,
		"usage: dwpal_cli [INTERFACENAME] PeerHostIfQos [MACADDR]", PEER_HOST_IF_QOS},
{"peerhostif", LTQ_NL80211_VENDOR_SUBCMD_GET_PEER_HOST_IF, 2,
		"usage: dwpal_cli [INTERFACENAME] PeerHostIf [MACADDR]", PEER_HOST_IF},
{"peerrxstats", LTQ_NL80211_VENDOR_SUBCMD_GET_PEER_RX_STATS, 2,
		"usage: dwpal_cli [INTERFACENAME] PeerRxStats [MACADDR]", PEER_RX_STATS},
{"peerulbsrctidstats", LTQ_NL80211_VENDOR_SUBCMD_GET_PEER_UL_BSRC_TID_STATS, 2,
		"usage: dwpal_cli [INTERFACENAME] PeerUlBsrcTidStats[MACADDR]", PEER_UL_BSRC_TID_STATS},
{"peerbaastats", LTQ_NL80211_VENDOR_SUBCMD_GET_PEER_BAA_STATS, 2,
		"usage: dwpal_cli [INTERFACENAME] PeerBaaStats [MACADDR]", PEER_BAA_STATS},
{"linkadaptionstats", LTQ_NL80211_VENDOR_SUBCMD_GET_LINK_ADAPTION_STATS, 2,
		"usage: dwpal_cli [INTERFACENAME] LinkAdaptionStats [MACADDR]", LINK_ADAPTION_STATS},
{"planmanagerstats", LTQ_NL80211_VENDOR_SUBCMD_GET_PLAN_MANAGER_STATS, 2,
		"usage: dwpal_cli [INTERFACENAME] PlanManagerStats [MACADDR]", PLAN_MANAGER_STATS},
{"twtstats", LTQ_NL80211_VENDOR_SUBCMD_GET_PEER_TWT_STATS, 2,
		"usage: dwpal_cli [INTERFACENAME] TwtStats [MACADDR]", TWT_STATS},
{"perclientstats", LTQ_NL80211_VENDOR_SUBCMD_GET_PER_CLIENT_STATS, 2,
		"usage: dwpal_cli [INTERFACENAME] PerClientStats [MACADDR]", PER_CLIENT_STATS},
{"peerphyrxstatus", LTQ_NL80211_VENDOR_SUBCMD_GET_PEER_PHY_RX_STATUS, 2,
		"usage: dwpal_cli [INTERFACENAME] PeerPhyRxStatus [MACADDR]", PEER_PHY_RX_STATUS},
{"peerinfo", LTQ_NL80211_VENDOR_SUBCMD_GET_PEER_INFO, 2,
		"usage: dwpal_cli [INTERFACENAME] PeerInfo [MACADDR]", PEER_INFO},
{"wlanhostifqos", LTQ_NL80211_VENDOR_SUBCMD_GET_WLAN_HOST_IF_QOS, 1,
		"usage: dwpal_cli [INTERFACENAME] WLANHostIfQos", WLAN_HOST_IF_QOS},
{"wlanhostif", LTQ_NL80211_VENDOR_SUBCMD_GET_WLAN_HOST_IF, 1,
		"usage: dwpal_cli [INTERFACENAME] WLANHostIf", WLAN_HOST_IF},
{"wlanrxstats", LTQ_NL80211_VENDOR_SUBCMD_GET_WLAN_RX_STATS, 1,
		"usage: dwpal_cli [INTERFACENAME] WLANRxStats", WLAN_RX_STATS},
{"wlanbaastats", LTQ_NL80211_VENDOR_SUBCMD_GET_WLAN_BAA_STATS, 1,
		"usage: dwpal_cli [INTERFACENAME] WLANBaaStats", WLAN_BAA_STATS},
{"radiorxstats", LTQ_NL80211_VENDOR_SUBCMD_GET_RADIO_RX_STATS, 1,
		"usage: dwpal_cli [INTERFACENAME] RadioRxStats", RADIO_RX_STATS},
{"radiobaastats", LTQ_NL80211_VENDOR_SUBCMD_GET_RADIO_BAA_STATS, 1,
		"usage: dwpal_cli [INTERFACENAME] RadioBaaStats", RADIO_BAA_STATS},
{"tsmaninittidglstats", LTQ_NL80211_VENDOR_SUBCMD_GET_TSMAN_INIT_TID_GL, 1,
		"usage: dwpal_cli [INTERFACENAME] TsManInitTidGlStats", TS_INIT_TID_GL},
{"tsmaninitstaglstats", LTQ_NL80211_VENDOR_SUBCMD_GET_TSMAN_INIT_STA_GL, 1,
		"usage: dwpal_cli [INTERFACENAME] TsManInitStaGlStats", TS_INIT_STA_GL},
{"tsmanrcpttidglstats", LTQ_NL80211_VENDOR_SUBCMD_GET_TSMAN_RCPT_TID_GL, 1,
		"usage: dwpal_cli [INTERFACENAME] TsManRcptTidGlStats", TS_RCPT_TID_GL},
{"tsmanrcptstaglstats", LTQ_NL80211_VENDOR_SUBCMD_GET_TSMAN_RCPT_STA_GL, 1,
		"usage: dwpal_cli [INTERFACENAME] TsManRcptStaGlStats", TS_RCPT_STA_GL},
{"radiolinkadaptionstats", LTQ_NL80211_VENDOR_SUBCMD_GET_RADIO_LINK_ADAPT_STATS, 1,
		"usage: dwpal_cli [INTERFACENAME] RadioLinkAdaptionStats", RADIO_LINK_ADAPT_STATS},
{"multicaststats", LTQ_NL80211_VENDOR_SUBCMD_GET_MULTICAST_STATS, 1,
		"usage: dwpal_cli [INTERFACENAME] MulticastStats", MULTICAST_STATS},
{"trainingmanstats", LTQ_NL80211_VENDOR_SUBCMD_GET_TRAINING_MAN_STATS, 1,
		"usage: dwpal_cli [INTERFACENAME] TrainingManStats", TRAIN_MAN_STATS},
{"groupmanstats", LTQ_NL80211_VENDOR_SUBCMD_GET_GROUP_MAN_STATS, 1,
		"usage: dwpal_cli [INTERFACENAME] GroupManStats", GROUP_MAN_STATS},
{"generalstats", LTQ_NL80211_VENDOR_SUBCMD_GET_GENERAL_STATS, 1,
		"usage: dwpal_cli [INTERFACENAME] GeneralStats", GENERAL_STATS},
{"currentchannelstats", LTQ_NL80211_VENDOR_SUBCMD_GET_CUR_CHANNEL_STATS, 1,
		"usage: dwpal_cli [INTERFACENAME] CurrentChannelStats", CHANNEL_STATS},
{"radiophyrxstats", LTQ_NL80211_VENDOR_SUBCMD_GET_RADIO_PHY_RX_STATS, 1,
		"usage: dwpal_cli [INTERFACENAME] RadioPhyRxStats", RADIO_PHY_RX_STATS},
{"dynamicbwstats", LTQ_NL80211_VENDOR_SUBCMD_GET_DYNAMIC_BW_STATS, 1,
		"usage: dwpal_cli [INTERFACENAME] DynamicBwStats", DYNAMIC_BW_STATS},
{"linkadaptationmuvhtstats", LTQ_NL80211_VENDOR_SUBCMD_GET_LA_MU_VHT_STATS, 2,
		"usage: dwpal_cli [INTERFACENAME] LinkAdaptationMuVhtStats [MU GROUP]", LA_MU_VHT_STATS},
{"getccastats", LTQ_NL80211_VENDOR_SUBCMD_GET_CCA_STATS_CURRENT_CHAN, 1,
		"usage: dwpal_cli [INTERFACENAME] GetCcaStats", CCA_STATS},
{"getccameasurement", LTQ_NL80211_VENDOR_SUBCMD_GET_CCA_MSR_OFF_CHAN, 2,
		"usage: dwpal_cli [INTERFACENAME] GetCcaMeasurement [CHANNEL]", CCA_STATS},
{"startccameasurement", LTQ_NL80211_VENDOR_SUBCMD_SET_START_CCA_MSR_OFF_CHAN, 3,
		"usage: dwpal_cli [INTERFACENAME] StartCcaMeasurement [CHANNEL] [DWELL_TIME_MS]", CCA_STATS},
{"reset_statistics", LTQ_NL80211_VENDOR_SUBCMD_RESET_STATISTICS, 2,
		"usage: dwpal_cli [INTERFACENAME] reset_statistics [reset_catagory] <MACADDR>", RESET_STATISTICS},
/* Two commands (short and long) to get the same statistics GET_DEV_DIAG_RESULT3 */
{"peerdiagresult3", LTQ_NL80211_VENDOR_SUBCMD_GET_DEV_DIAG_RESULT3, 2,
		"usage: dwpal_cli [INTERFACENAME] PeerDiagResult3 <MACADDR>", PEER_DIAG_RESULT3},
{"getapassociateddevicediagnosticresult3", LTQ_NL80211_VENDOR_SUBCMD_GET_DEV_DIAG_RESULT3, 2,
		"usage: dwpal_cli [INTERFACENAME] getApAssociatedDeviceDiagnosticResult3 <MACADDR>", PEER_DIAG_RESULT3},
{"getpeermixedstats", LTQ_NL80211_VENDOR_SUBCMD_GET_PEER_MIXED_STATS, 2,
		"usage: dwpal_cli [INTERFACENAME] GetPeerMixedStats <MACADDR>", PEER_MIXED_STATS},
{"lamugroupscounters", LTQ_NL80211_VENDOR_SUBCMD_GET_LINK_ADAPT_MU_GROUPS_COUNTERS_STATS, 1,
		"usage: dwpal_cli [INTERFACENAME] LaMuGroupsCounters", LINK_ADAPT_MU_GROUPS_COUNTERS_STATS},
/* CSI stats */
{"csistats", LTQ_NL80211_VENDOR_SUBCMD_GET_CSI_STATS, 2,
		"usage: dwpal_cli [INTERFACENAME] CSIStats <MACADDR>", CSI_STATS},
{"csicounters", LTQ_NL80211_VENDOR_SUBCMD_GET_CSI_COUNTERS, 2,
		"usage: dwpal_cli [INTERFACENAME] CSICounters <MACADDR>", CSI_COUNTERS},
/* OFDMA SuMuRu stats */
{"musustatistics", LTQ_NL80211_VENDOR_SUBCMD_GET_LINK_ADAPT_SU_MU_RU_OFDMA_STATS, 2,
		"usage: dwpal_cli [INTERFACENAME] MuSuStatistics [stats_category]", LINK_ADAPT_SU_MU_RU_OFDMA_STATS},
/* DL MIMO OFDMA stats */
{"dlmimostatistics", LTQ_NL80211_VENDOR_SUBCMD_GET_LINK_ADAPT_MIMO_OFDMA_STATS, 1,
		"usage: dwpal_cli [INTERFACENAME] DLMimoStatistics", LINK_ADAPT_MIMO_OFDMA_STATS},
{"linkadaptationmuheehtstats", LTQ_NL80211_VENDOR_SUBCMD_GET_LA_MU_HE_EHT_STATS, 1,
		"usage: dwpal_cli [INTERFACENAME] LinkAdaptationMuHeEhtStats [MU Group] [RST]", LA_MU_HE_EHT_STATS},
{"mlpeerflowstatus", LTQ_NL80211_VENDOR_SUBCMD_GET_ML_PEER_FLOW_STATUS, 2,
		"usage: dwpal_cli [INTERFACENAME] MLPeerFlowStatus [ML_STA_MACADDR]", ML_PEER_FLOW_STATS},
};


int check_and_dump_packet_error_rate(char *cmd[]);
int check_stats_cmd(int argc,char *argv[]);
#endif
