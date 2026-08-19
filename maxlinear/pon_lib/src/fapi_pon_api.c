/*****************************************************************************
 *
 * Copyright (c) 2020 - 2026 MaxLinear, Inc.
 * Copyright (c) 2017 - 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

/*
 * A simple "get" FAPI call works in this way:
 * 1. The fapi_pon_*_get(struct <type> param) function gets called by external
 * code
 * 2. This function calls the fapi_pon_generic_get() function with the
 *    command ID
 *   a. In addition we also provide a callback handler and the param parameter
 * 3. fapi_pon_generic_get() calls some other methods to create a read message
 *    for the firmware with no payload to read out the specified data
 * 4. This data is send to the pon_mbox driver through generic Netlink and the
 *    forwarded to the firmware.
 * 5. A callback handler is registered which should be called for the answer
 * 6. The nl_recvmsgs() function is called which handles receiving Netlink
 *    messages.
 * 7. When the Firmware answers to the message the pon_mbox driver gets the
 *    message and creates a new Netlink message as a response to the request.
 *   a. The pon_mbox driver matches the request to the response with the
 *      sequence number
 *   b. The answer is only send to the process which send the request
 * 8. The callback handler (cb_valid_handler()) registered in 5. is called by
 *    the Netlink library with the received data.
 * 9. The data is extracted and then given to the callback handler registered
 *    in 2.
 *   a. It gets the payload received from the firmware and the param given to
 *      fapi_pon_generic_get() in 2.
 * 10. The callback handler registered in 2. converts the content received from
 *     the firmware into the format used in the FAPI.
 *   a. It is checked if the size matches the expected size of the message
 *   b. The result is written to the param given by the external caller
 * 11. Both callback functions terminate
 *   a. The Netlink message itself will be freed
 * 12. fapi_pon_generic_get() returns the status code
 * 13. fapi_pon_*_get(struct <type> param) returns with a status code.
 *   a. param was already updated in the callback function
 */

#include <string.h>
#include <math.h>
#include <time.h>
#include "fapi_pon.h"
#include "fapi_pon_core.h"
#include "fapi_pon_debug.h"
#include "fapi_pon_os.h"
#include "pon_ip_msg.h"

#ifdef EXTRA_VERSION
#define pon_extra_ver_str "." EXTRA_VERSION
#else
#define pon_extra_ver_str ""
#endif

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x) (sizeof(x) / sizeof(*(x)))
#endif

/* Assign value and check if there is type overflow */
#define ASSIGN_AND_OVERFLOW_CHECK(fw_param, fapi_param) do { \
			fw_param = fapi_param;	\
			if (fw_param != fapi_param) \
				return PON_STATUS_VALUE_RANGE_ERR; \
			} while (0)

#define CNT_DECODE_U64(COUNTER, DEST) \
	do { \
		if (cnt[PON_MBOX_A_CNT_##COUNTER]) \
			dst_param->DEST = \
				nla_get_u64(cnt[PON_MBOX_A_CNT_##COUNTER]); \
	} while (0)

#define CNT_DECODE_U32(PARAM, DEST) \
	do { \
		if (cnt[PON_MBOX_A_CNT_##PARAM]) \
			dst_param->DEST = \
				nla_get_u32(cnt[PON_MBOX_A_CNT_##PARAM]); \
	} while (0)

#define SRDS_DECODE_U8(PARAM, DEST) \
	do { \
		if (cfg[PON_MBOX_SRDS_##PARAM]) \
			dst_param->DEST = \
				nla_get_u8(cfg[PON_MBOX_SRDS_##PARAM]); \
	} while (0)

#define GLUE_U32(a, b, c, d) \
	((uint32_t)((a) & 0xFF) << 24 | \
	 (uint32_t)((b) & 0xFF) << 16 | \
	 (uint32_t)((c) & 0xFF) << 8 | \
	 (uint32_t)((d) & 0xFF))

#define GLUE_U16(a, b) ((uint16_t)((a) & 0xFF) << 8 | (uint16_t)((b) & 0xFF))

#define TO_MILLISECONDS(a) ((a) / 8)
#define FROM_MILLISECONDS(a) ((a) * 8)

/** Maximum GEM port ID for GPON mode */
#define PON_G984_GEM_PORT_ID_MAX 4095
/** Maximum GEM port ID for XG-PON/XGS-PON/NG-PON2 mode */
#define PON_G987_GEM_PORT_ID_MAX 65534
/** Maximum Allocation ID for GPON mode */
#define PON_G984_ALLOC_ID_MAX 4095
/** Maximum Allocation ID for XG-PON/XGS-PON/NG-PON2 mode */
#define PON_G987_ALLOC_ID_MAX 16383
/** Enable Local Wake-up Software Control */
#define LWI_SW_CONTROL_ENABLE 1
/** Disable Local Wake-up Software Control */
#define LWI_SW_CONTROL_DISABLE 0
/** Enable Local Wake-up Indication */
#define LWI_ON 1
/** Disable Local Wake-up Indication */
#define LWI_OFF 0
/** Maximum Burst Profile Index value */
#define PON_MAX_BURST_PROFILE_INDEX 3
/** Maximum ToD PPS Signal Pulse Width value */
#define PON_TOD_PPSW_MAX 8191
/** Maximum ToD Upstream Correction value */
#define PON_TOD_USC_MAX 1023
/** Maximum ToD Downstream Correction value */
#define PON_TOD_DSC_MAX 1023
/** Maximum ToD Output Sub-sampling value */
#define PON_TOD_OUTSS_MAX 511
/** Maximum ToD Output correction value */
#define PON_TOD_OUTC_MAX 1023
/** Default value for divider deviance threshold in synce config message */
#define SYNCE_CONFIG_DDT 0X00989680
/** Default value for history sample count in synce config message */
#define SYNCE_CONFIG_HIST 0X02
/** Default value for averaging sample count in synce config message */
#define SYNCE_CONFIG_AVG 0x04
/** Maximum value for PON debug clock source */
#define PON_DEBUG_CLOCK_SOURCE_MAX 1
/** Bit error counting time interval, given in multiples of 1 ms,
 *  the default value is 10 s as a compromise between measurement accuracy at
 *  low bit error rates and detection speed. This can be adapted to meet
 *  specific system requirements.
 */
#define BER_COUNT_TIME 10000
/** In GPON mode the number of bits per 125 us time slot is
 * 125 us * 2.48832 GHz:
 * 125e-6 s * 2.48832e9 bit/s = 311040 bit.
 */
#define GPON_BITS_PER_125US 311040
/** Signal fail threshold minimum value */
#define SF_THRESHOLD_MIN_VALUE 3
/** Signal fail threshold maximum value */
#define SF_THRESHOLD_MAX_VALUE 9
/** Signal degrade threshold minimum value */
#define SD_THRESHOLD_MIN_VALUE 4
/** Signal degrade threshold maximum value */
#define SD_THRESHOLD_MAX_VALUE 10

/** The PON operation mode is ITU-T G.984 (GPON). */
#define MODE_984_GPON (1U << PON_MODE_984_GPON)
/** The PON operation mode is ITU-T G.987 (XG-PON). */
#define MODE_987_XGPON (1U << PON_MODE_987_XGPON)
/** The PON operation mode is ITU-T G.9807 (XGS-PON). */
#define	MODE_9807_XGSPON (1U << PON_MODE_9807_XGSPON)
/** The PON operation mode is ITU-T G.989 (NG-PON2 2.5G upstream). */
#define	MODE_989_NGPON2_2G5 (1U << PON_MODE_989_NGPON2_2G5)
/** The PON operation mode is ITU-T G.989 (NG-PON2 10G upstream). */
#define MODE_989_NGPON2_10G (1U << PON_MODE_989_NGPON2_10G)
/** The PON operation mode is AON (Active Ethernet). */
#define MODE_AON (1U << PON_MODE_AON)
/** The PON operation mode belongs to ITU modes. */
#define MODE_ITU_PON (MODE_984_GPON | MODE_987_XGPON | MODE_9807_XGSPON | \
		      MODE_989_NGPON2_10G | MODE_989_NGPON2_2G5)

/*
* Conversion factor for downstream frames from bytes to BIP32 words (4-Byte)
* +------------------+---------------------+--------------------+
* | Protocol         | Bytes (downstream)  | Words (downstream) |
* +------------------+---------------------+--------------------+
* | G-PON (2.5G)     | 38,880              | 9,720              |
* | XG-PON           | 155,520             | 38,880             |
* | XGS-PON          | 155,520             | 38,880             |
* | NG-PON2 (2.5G)   | 38,880              | 9,720              |
* | NG-PON2 (10G)    | 155,520             | 38,880             |
* +------------------+---------------------+--------------------+
*/
#define DS_FRAMES_TO_BIP32_WORDS_MODE_GPON 9720
#define DS_FRAMES_TO_BIP32_WORDS_MODE_XGPON 38880
#define DS_FRAMES_TO_BIP32_WORDS_MODE_XGSPON 38880
#define DS_FRAMES_TO_BIP32_WORDS_MODE_NGPON2_2G5 9720
#define DS_FRAMES_TO_BIP32_WORDS_MODE_NGPON2_10G 38880

/** TWDM_CONFIG.WL_SW_ROUNDS_INIT parameter's minimum allowed value */
#define PONIPFW_TWDM_CONFIG_WL_SW_ROUNDS_MIN (1)

/* Configuration values per threshold in 10G mode.
 * The table is commonly used to implement threshold levels for signal fail
 * and signal degrade alarm detection.
 * Index values 0 - 6 of this array correspond to the values 3 - 9 of the
 * signal fail threshold and index values 1 - 7 correspond to the values 4 - 10
 * of the signal degrade threshold.
 */
static const uint32_t threshold_cfg_10g[8] = {99532800, 9953280, 995328,
				    99533, 9954, 996, 100, 10};

/** what string support, version string */
const char pon_whatversion[] =
	"@(#)MaxLinear PON library, Version " PACKAGE_VERSION pon_extra_ver_str;

static struct nla_policy
pon_mbox_cnt_gtc_policy[PON_MBOX_A_CNT_GTC_MAX + 1] = {
	[PON_MBOX_A_CNT_GTC_BIP_ERRORS] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_GTC_DISC_GEM_FRAMES] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_GTC_GEM_HEC_ERRORS_CORR] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_GTC_GEM_HEC_ERRORS_UNCORR] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_GTC_BWMAP_HEC_ERRORS_CORR] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_GTC_BYTES_CORR] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_GTC_FEC_CODEWORDS_CORR] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_GTC_FEC_COREWORDS_UNCORR] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_GTC_TOTAL_FRAMES] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_GTC_FEC_SEC] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_GTC_GEM_IDLE] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_GTC_LODS_EVENTS] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_GTC_DG_TIME] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_GTC_PLOAM_CRC_ERRORS] = { .type = NLA_U64 },
};

static struct nla_policy
pon_mbox_cnt_xgtc_policy[PON_MBOX_A_CNT_XGTC_MAX + 1] = {
	[PON_MBOX_A_CNT_XGTC_PSBD_HEC_ERR_UNCORR] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_XGTC_PSBD_HEC_ERR_CORR] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_XGTC_FS_HEC_ERR_UNCORR] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_XGTC_FS_HEC_ERR_CORR] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_XGTC_LOST_WORDS] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_XGTC_PLOAM_MIC_ERR] = { .type = NLA_U64 },
};

static struct nla_policy
pon_mbox_cnt_gem_port_policy[PON_MBOX_A_CNT_GEM_PORT_MAX + 1] = {
	[PON_MBOX_A_CNT_GEM_PORT_GEM_PORT_ID] = { .type = NLA_U32 },
	[PON_MBOX_A_CNT_GEM_PORT_TX_FRAMES] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_GEM_PORT_TX_FRAGMENTS] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_GEM_PORT_TX_BYTES] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_GEM_PORT_RX_FRAMES] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_GEM_PORT_RX_FRAGMENTS] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_GEM_PORT_RX_BYTES] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_GEM_PORT_KEY_ERRORS] = { .type = NLA_U64 },
};

static struct nla_policy
pon_mbox_cnt_alloc_policy[PON_MBOX_A_CNT_ALLOC_MAX + 1] = {
	[PON_MBOX_A_CNT_ALLOC_ALLOCATIONS] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_ALLOC_IDLE] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_ALLOC_US_BW] = { .type = NLA_U64 },
};

static struct nla_policy
pon_mbox_cnt_alloc_discard_policy[PON_MBOX_A_CNT_ALLOC_DISCARD_MAX + 1] = {
	[PON_MBOX_A_CNT_ALLOC_DISCARD_DISCS] = { .type = NLA_NESTED },
	[PON_MBOX_A_CNT_ALLOC_DISCARD_RULES] = { .type = NLA_NESTED },
};

static struct nla_policy
pon_mbox_cnt_alloc_discard_item_policy
	[PON_MBOX_A_CNT_ALLOC_DISCARD_ITEM_MAX + 1] = {
	  [PON_MBOX_A_CNT_ALLOC_DISCARD_ITEM] = { .type = NLA_U64 },
};

static struct nla_policy
pon_mbox_cnt_eth_policy[PON_MBOX_A_CNT_ETH_MAX + 1] = {
	[PON_MBOX_A_CNT_ETH_BYTES] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_ETH_FRAMES_LT_64] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_ETH_FRAMES_64] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_ETH_FRAMES_65_127] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_ETH_FRAMES_128_255] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_ETH_FRAMES_256_511] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_ETH_FRAMES_512_1023] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_ETH_FRAMES_1024_1518] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_ETH_FRAMES_GT_1518] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_ETH_FRAMES_FCS_ERR] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_ETH_BYTES_FCS_ERR] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_ETH_FRAMES_TOO_LONG] = { .type = NLA_U64 },
};

static struct nla_policy
serdes_config_policy[PON_MBOX_SRDS_MAX + 1] = {
	[PON_MBOX_SRDS_TX_EQ_MAIN] = { .type = NLA_U8 },
	[PON_MBOX_SRDS_TX_EQ_POST] = { .type = NLA_U8 },
	[PON_MBOX_SRDS_TX_EQ_PRE] = { .type = NLA_U8 },
	[PON_MBOX_SRDS_VBOOST_EN] = { .type = NLA_U8 },
	[PON_MBOX_SRDS_VBOOST_LVL] = { .type = NLA_U8 },
	[PON_MBOX_SRDS_IBOOST_LVL] = { .type = NLA_U8 },
	[PON_MBOX_SRDS_RX_ADAPT_AFE_EN] = { .type = NLA_U8 },
	[PON_MBOX_SRDS_RX_ADAPT_DFE_EN] = { .type = NLA_U8 },
	[PON_MBOX_SRDS_RX_ADAPT_CONT] = { .type = NLA_U8 },
	[PON_MBOX_SRDS_RX_ADAPT_EN] = { .type = NLA_U8 },
	[PON_MBOX_SRDS_RX_EQ_ATT_LVL] = { .type = NLA_U8 },
	[PON_MBOX_SRDS_RX_EQ_ADAPT_MODE] = { .type = NLA_U8 },
	[PON_MBOX_SRDS_RX_EQ_ADAPT_SEL] = { .type = NLA_U8 },
	[PON_MBOX_SRDS_RX_EQ_CTLE_BOOST] = { .type = NLA_U8 },
	[PON_MBOX_SRDS_RX_VCO_TEMP_COMP_EN] = { .type = NLA_U8 },
	[PON_MBOX_SRDS_RX_VCO_STEP_CTRL] = { .type = NLA_U8 },
	[PON_MBOX_SRDS_RX_VCO_FRQBAND] = { .type = NLA_U8 },
	[PON_MBOX_SRDS_RX_MISC] = { .type = NLA_U8 },
	[PON_MBOX_SRDS_RX_DELTA_IQ] = { .type = NLA_U8 },
	[PON_MBOX_SRDS_RX_MARGIN_IQ] = { .type = NLA_U8 },
	[PON_MBOX_SRDS_EQ_CTLE_POLE] = { .type = NLA_U8 },
	[PON_MBOX_SRDS_EQ_DFE_TAP1] = { .type = NLA_U8 },
	[PON_MBOX_SRDS_EQ_DFE_BYPASS] = { .type = NLA_U8 },
	[PON_MBOX_SRDS_EQ_VGA1_GAIN] = { .type = NLA_U8 },
	[PON_MBOX_SRDS_EQ_VGA2_GAIN] = { .type = NLA_U8 },
};

static struct nla_policy dp_config_policy[PON_MBOX_DPCFG_MAX + 1] = {
	[PON_MBOX_A_DP_CONFIG_WITH_RX_FCS] = { .type = NLA_U8 },
	[PON_MBOX_A_DP_CONFIG_WITH_TX_FCS] = { .type = NLA_U8 },
	[PON_MBOX_A_DP_CONFIG_WITHOUT_TIMESTAMP] = { .type = NLA_U8 },
};

static struct nla_policy
pon_mbox_cnt_twdm_lods_policy[PON_MBOX_A_CNT_TWDM_LODS_MAX + 1] = {
	[PON_MBOX_A_CNT_TWDM_LODS_EVENTS_ALL] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_TWDM_LODS_RESTORED_OPER] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_TWDM_LODS_RESTORED_PROT] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_TWDM_LODS_RESTORED_DISK] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_TWDM_LODS_REACTIVATION_OPER] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_TWDM_LODS_REACTIVATION_PROT] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_TWDM_LODS_REACTIVATION_DISC] = { .type = NLA_U64 },
};

static struct nla_policy
pon_mbox_cnt_twdm_optic_pl_policy[PON_MBOX_A_CNT_TWDM_OPTIC_PL_MAX + 1] = {
	[PON_MBOX_A_CNT_TWDM_OPTIC_PL_REJECTED] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_TWDM_OPTIC_PL_INCOMPLETE] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_TWDM_OPTIC_PL_COMPLETE] = { .type = NLA_U64 },
};

static struct nla_policy
pon_mbox_cnt_twdm_tc_policy[PON_MBOX_A_CNT_TWDM_TC_MAX + 1] = {
	[PON_MBOX_A_CNT_TWDM_TC] = { .type = NLA_NESTED },
};

static struct nla_policy
pon_mbox_cnt_twdm_tc_item_policy[PON_MBOX_A_CNT_TWDM_TC_ITEM_MAX + 1] = {
	[PON_MBOX_A_CNT_TWDM_TC_ITEM] = { .type = NLA_U64 },
};

static struct nla_policy
pon_mbox_cnt_tc_ploam_ds_policy[PON_MBOX_A_CNT_TC_PLOAM_DS_MAX + 1] = {
	[PON_MBOX_A_CNT_TC_PLOAM_DS_US_OVERHEAD] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_TC_PLOAM_DS_ENC_PORT_ID] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_TC_PLOAM_DS_REQ_PW] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_TC_PLOAM_DS_NO_MESSAGE] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_TC_PLOAM_DS_POPUP] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_TC_PLOAM_DS_REQ_KEY] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_TC_PLOAM_DS_CONFIG_PORT_ID] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_TC_PLOAM_DS_PEE] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_TC_PLOAM_DS_PST] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_TC_PLOAM_DS_BER_INTERVAL] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_TC_PLOAM_DS_KEY_SWITCHING] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_TC_PLOAM_DS_EXT_BURST] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_TC_PLOAM_DS_PON_ID] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_TC_PLOAM_DS_SWIFT_POPUP] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_TC_PLOAM_DS_RANGING_ADJ] = { .type = NLA_U64 },

	[PON_MBOX_A_CNT_TC_PLOAM_DS_BST_PROFILE] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_TC_PLOAM_DS_ASS_ONU] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_TC_PLOAM_DS_RNG_TIME] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_TC_PLOAM_DS_DEACT_ONU] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_TC_PLOAM_DS_DIS_SER] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_TC_PLOAM_DS_REQ_REG] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_TC_PLOAM_DS_ASS_ALLOC] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_TC_PLOAM_DS_KEY_CTRL] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_TC_PLOAM_DS_SLP_ALLOW] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_TC_PLOAM_DS_CALIB_REQ] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_TC_PLOAM_DS_ADJ_TX_WL] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_TC_PLOAM_DS_TUNE_CTRL] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_TC_PLOAM_DS_SYS_PROFILE] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_TC_PLOAM_DS_CH_PROFILE] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_TC_PLOAM_DS_PROT_CONTROL] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_TC_PLOAM_DS_CHG_PW_LVL] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_TC_PLOAM_DS_PW_CONS] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_TC_PLOAM_DS_RATE_CTRL] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_TC_PLOAM_DS_REBOOT_ONU] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_TC_PLOAM_DS_UNKNOWN] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_TC_PLOAM_DS_ADJ_TX_WL_FAIL] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_TC_PLOAM_DS_TUNE_REQ] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_TC_PLOAM_DS_TUNE_COMPL] = { .type = NLA_U64 },
};

static struct nla_policy
pon_mbox_cnt_tc_ploam_us_policy[PON_MBOX_A_CNT_TC_PLOAM_DS_MAX + 1] = {
	[PON_MBOX_A_CNT_TC_PLOAM_US_SER_ONU] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_TC_PLOAM_US_PASSWORD] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_TC_PLOAM_US_DYG_GASP] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_TC_PLOAM_US_NO_MSG] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_TC_PLOAM_US_ENC_KEY] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_TC_PLOAM_US_PHY_EE] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_TC_PLOAM_US_PST_MSG] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_TC_PLOAM_US_REM_ERR] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_TC_PLOAM_US_ACK] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_TC_PLOAM_US_SLP_REQ] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_TC_PLOAM_US_REG] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_TC_PLOAM_US_KEY_REP] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_TC_PLOAM_US_TUN_RES] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_TC_PLOAM_US_PW_CONS] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_TC_PLOAM_US_RATE_RESP] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_TC_PLOAM_US_CPL_ERR] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_TC_PLOAM_US_TUN_RES_AN] = { .type = NLA_U64 },
	[PON_MBOX_A_CNT_TC_PLOAM_US_TUN_RES_CRB] = { .type = NLA_U64 }
};

/*
 * Function used to check data integrity between source
 * and destination structures.
 *
 * Input:
 * - param: pointer to the destination structure
 * - expect_size: size of the source structure
 * - data_size: size of the destination structure
 *
 * Return value as follows:
 * - PON_STATUS_OK: If successful
 * - Other: An error code in case of error.
 */
static enum fapi_pon_errorcode integrity_check(void *param,
					       size_t src_size,
					       size_t dst_size)
{
	if (src_size != dst_size)
		return PON_STATUS_FW_UNEXPECTED;

	if (!param) {
		PON_DEBUG_ERR("private data not set");
		return PON_STATUS_DATA_SET_ERR;
	}

	return PON_STATUS_OK;
}

void pon_byte_copy(uint8_t *dst, const uint8_t *src, int size)
{
	int i;

#if __BYTE_ORDER == __BIG_ENDIAN
	for (i = 0; i < size; i++)
		dst[i] = src[i];
#else
	/* copy only full 32bit values */
	for (i = 0; i + 3 < size; i += 4) {
		dst[i] = src[i + 3];
		dst[i + 1] = src[i + 2];
		dst[i + 2] = src[i + 1];
		dst[i + 3] = src[i];
	}
#endif
}

static enum fapi_pon_errorcode pon_mode_get_decode(struct pon_ctx *ctx,
						   struct nlattr **attrs,
						   void *priv)
{
	uint8_t *pon_mode = priv;

	if (!attrs[PON_MBOX_A_PON_MODE])
		return PON_STATUS_ERR;

	*pon_mode = nla_get_u8(attrs[PON_MBOX_A_PON_MODE]);

	/* Caching PON mode in PON context */
	ctx->mode = *pon_mode;
	/* Caching PON mode validity in PON context */
	ctx->mode_valid = 1;

	return PON_STATUS_OK;
}

/*
 * Function checks if features given in features param
 * are set in capabilities.
 *
 * Input:
 * - ctx: PON library context created by fapi_pon_open
 * - features: bitmask representing features to be checked
 *
 * Return value as follows:
 * - true if all expected features are active
 * - false in opposite case
 */
static bool caps_features_check_if_all(struct pon_ctx *ctx, uint32_t features)
{
	enum fapi_pon_errorcode ret;
	struct pon_cap caps = {0};

	ret = fapi_pon_cap_get(ctx, &caps);
	if (ret != PON_STATUS_OK)
		return false;

	return (caps.features & features) == features;
}

/*
 * Function checks if at least one of features given in features
 * param is set in capabilities.
 *
 * Input:
 * - ctx: PON library context created by fapi_pon_open
 * - features: bitmask representing features to be checked
 *
 * Return value as follows:
 * - true if at least one of expected features is active
 * - false in opposite case
 */
static bool caps_features_check_if_one(struct pon_ctx *ctx, uint32_t features)
{
	enum fapi_pon_errorcode ret;
	struct pon_cap caps = {0};

	ret = fapi_pon_cap_get(ctx, &caps);
	if (ret != PON_STATUS_OK)
		return false;

	return caps.features & features;
}

/*
 * Retrieves actual Platform type.
 *
 * Input:
 * - ctx: PON library context created by fapi_pon_open
 *   act_plat_type: pointer to an uint32_t to get populated with act_plat_type
 *
 * Return value:
 * - PON_STATUS_OK : all went fine
 *   PON_STATUS_FW_UNEXPECTED : unexpected value found in FW_VERSION.PLATFORM
 *   All other errors potentially raised by fapi_pon_version_get()
 *
 */
static enum fapi_pon_errorcode actual_platform_type_get(struct pon_ctx *ctx,
							uint32_t *act_plat_type)
{
	enum fapi_pon_errorcode ret;
	struct pon_version version;
	static const uint32_t mapper[] = {
		[PONFW_VERSION_PLATFORM_FPGA_PRX_A] = PLAT_T_PRX,
		[PONFW_VERSION_PLATFORM_SOC_PRX_A]  = PLAT_T_PRX,
		[PONFW_VERSION_PLATFORM_FPGA_PRX_B] = PLAT_T_PRX,
		[PONFW_VERSION_PLATFORM_SOC_PRX_B]  = PLAT_T_PRX,
		[PONFW_VERSION_PLATFORM_FPGA_URX_A] = PLAT_T_URX,
		[PONFW_VERSION_PLATFORM_SOC_URX_A]  = PLAT_T_URX,
		[PONFW_VERSION_PLATFORM_FPGA_URX_B] = PLAT_T_URX,
		[PONFW_VERSION_PLATFORM_SOC_URX_B]  = PLAT_T_URX,
		[PONFW_VERSION_PLATFORM_SOC_URX_C]  = PLAT_T_URX,
		/* TODO: when this gets added uncomment it */
		/* [PONFW_VERSION_PLATFORM_SOC_TPZ_A] = PLAT_T_TPZ */
	};

	if (ctx->actual_plat_type)
		goto actual_plat_type_get_exit;

	ret = fapi_pon_version_get(ctx, &version);
	if (ret != PON_STATUS_OK)
		return ret;

	if (version.fw_version_platform >= ARRAY_SIZE(mapper))
		return PON_STATUS_FW_UNEXPECTED;

	ctx->actual_plat_type = 1UL << mapper[version.fw_version_platform];

actual_plat_type_get_exit:
	*act_plat_type = ctx->actual_plat_type;
	return PON_STATUS_OK;
}

/*
 * checks whether the actual platform type is any of platform types specified
 * by plat_types parameter.
 *
 * Input:
 * - ctx: PON library context created by fapi_pon_open
 * - plat_types: Platform type(s) to check against
 *
 * Return value:
 * - true if actual platform type is any of specified platform types
 * - false otherwise
 */
static bool is_plat_type_any_of(struct pon_ctx *ctx, const uint32_t plat_types)
{
	enum fapi_pon_errorcode ret;
	uint32_t act_plat_type;

	ret = actual_platform_type_get(ctx, &act_plat_type);
	if (ret != PON_STATUS_OK) {
		PON_DEBUG_ERR("Actual Platform type cannot be established! Error code: %d", ret);
		return false;
	}

	return !!(act_plat_type & plat_types);
}

/*
 * Function checks if the active mode is one of modes passed
 * in mode param.
 *
 * Input:
 * - ctx: PON library context created by fapi_pon_open
 * - mode: bitmask representing modes to be checked
 *
 * Return value as follows:
 * - true if one of passed mode is active
 * - false in opposite case
 */
static bool pon_mode_check(struct pon_ctx *ctx, uint32_t mode)
{
	enum fapi_pon_errorcode ret;
	uint8_t pon_mode = 0;

	ret = fapi_pon_mode_get(ctx, &pon_mode);
	if (ret != PON_STATUS_OK)
		return false;

	return (1U << pon_mode) & mode;
}

enum fapi_pon_errorcode fapi_pon_mode_get(struct pon_ctx *ctx,
					  uint8_t *pon_mode)
{
	struct read_cmd_cb cb_data;
	struct nl_msg *msg;
	enum fapi_pon_errorcode ret;
	uint32_t seq = NL_AUTO_SEQ;

	if (!ctx || !pon_mode)
		return PON_STATUS_INPUT_ERR;

	if (ctx->mode_valid) {
		*pon_mode = ctx->mode;
		return PON_STATUS_OK;
	}

	ret = fapi_pon_nl_msg_prepare_decode(ctx, &msg, &cb_data, &seq,
					     &pon_mode_get_decode,
					     NULL, pon_mode,
					     PON_MBOX_C_MODE_READ);
	if (ret != PON_STATUS_OK)
		return ret;

	return fapi_pon_nl_msg_send(ctx, &msg, &cb_data, &seq);
}

static enum fapi_pon_errorcode external_calibration_update(struct pon_ctx *ctx)
{
	struct pon_optic_properties optic_properties = {0};
	enum fapi_pon_errorcode ret;

	if (!ctx)
		return PON_STATUS_INPUT_ERR;

	ret = fapi_pon_optic_properties_get(ctx, &optic_properties);
	if (ret != PON_STATUS_OK)
		return ret;

	ctx->ext_calibrated = optic_properties.ext_calibrated;
	ctx->ext_cal_valid = 1;

	return ret;
}

static enum fapi_pon_errorcode debug_support_check(struct pon_ctx *ctx)
{
	/* Debug mode is not supported. */
	if (!caps_features_check_if_one(ctx, PON_FEATURE_DBG |
					PON_FEATURE_SDBG))
		return PON_STATUS_FW_DBG;

	return PON_STATUS_OK;
}

enum fapi_pon_errorcode fapi_pon_limits_get(struct pon_ctx *ctx,
					    struct pon_range_limits *param)
{
	enum fapi_pon_errorcode ret;

	if (!ctx || !param)
		return PON_STATUS_INPUT_ERR;

	if (ctx->limits_valid) {
		if (memcpy_s(param, sizeof(*param),
			     &ctx->limits_data,
			     sizeof(ctx->limits_data))) {
			PON_DEBUG_ERR("memcpy_s failed");
			return PON_STATUS_MEMCPY_ERR;
		}
	} else {
		struct pon_cap caps = {0};

		ret = fapi_pon_cap_get(ctx, &caps);
		if (ret != PON_STATUS_OK)
			return ret;

		/* GPON mode */
		if (pon_mode_check(ctx, MODE_984_GPON)) {
			param->gem_port_id_max = PON_G984_GEM_PORT_ID_MAX;
			param->alloc_id_max = PON_G984_ALLOC_ID_MAX;
		}
		/* XG-PON/XGS-PON/NG-PON2 mode */
		else {
			param->gem_port_id_max = PON_G987_GEM_PORT_ID_MAX;
			param->alloc_id_max = PON_G987_ALLOC_ID_MAX;
		}

		param->gem_port_idx_max = caps.gem_ports - 1;
		param->alloc_idx_max = caps.alloc_ids - 1;

		/* Caching limits data in PON context */
		if (memcpy_s(&ctx->limits_data,
			     sizeof(ctx->limits_data),
			     param, sizeof(*param))) {
			PON_DEBUG_ERR("memcpy_s failed");
			return PON_STATUS_MEMCPY_ERR;
		}
		/* Caching limits validity in PON context */
		ctx->limits_valid = 1;
	}

	return PON_STATUS_OK;
}

static enum fapi_pon_errorcode pon_cap_get_copy(struct pon_ctx *ctx,
						const void *data,
						size_t data_size,
						void *priv)
{
	enum fapi_pon_errorcode ret;
	const struct ponfw_capabilities *src_param = data;
	struct pon_cap *dst_param = priv;

	ret = integrity_check(dst_param, sizeof(*src_param), data_size);
	if (ret != PON_STATUS_OK)
		return ret;

	/* TODO: Fix this for Little endian */
	dst_param->features = ((const uint32_t *)data)[0] & 0x7FFFFF;
	dst_param->gem_ports = src_param->gem;
	dst_param->alloc_ids = src_param->alloc;
	dst_param->itransinit = src_param->itransinit;
	dst_param->itxinit = src_param->itxinit;
	dst_param->qos_max = src_param->qos_max;

	/* Caching capabilities data in PON context */
	if (memcpy_s(&ctx->caps_data, sizeof(ctx->caps_data),
		     dst_param, sizeof(*dst_param))) {
		PON_DEBUG_ERR("memcpy_s failed");
		return PON_STATUS_MEMCPY_ERR;
	}
	/* Caching capabilities validity in PON context */
	ctx->caps_valid = 1;

	return PON_STATUS_OK;
}

enum fapi_pon_errorcode fapi_pon_cap_get(struct pon_ctx *ctx,
					 struct pon_cap *param)
{
	if (!ctx)
		return PON_STATUS_INPUT_ERR;

	if (ctx->caps_valid) {
		if (memcpy_s(param, sizeof(*param),
			     &ctx->caps_data, sizeof(ctx->caps_data))) {
			PON_DEBUG_ERR("memcpy_s failed");
			return PON_STATUS_MEMCPY_ERR;
		}
		return PON_STATUS_OK;
	}

	return fapi_pon_generic_get(ctx,
				    PONFW_CAPABILITIES_CMD_ID,
				    NULL,
				    0,
				    &pon_cap_get_copy,
				    param);
}

static enum fapi_pon_errorcode pon_version_get_copy(struct pon_ctx *ctx,
						    const void *data,
						    size_t data_size,
						    void *priv)
{
	enum fapi_pon_errorcode ret;
	const struct ponfw_version *src_param = data;
	struct pon_version *dst_param = priv;

	ret = integrity_check(dst_param, sizeof(*src_param), data_size);
	if (ret != PON_STATUS_OK)
		return ret;

	dst_param->hw_version = src_param->hwversion;
	dst_param->fw_version_major = src_param->maj;
	dst_param->fw_version_minor = src_param->min;
	dst_param->fw_version_platform = src_param->platform;
	dst_param->fw_version_standard = src_param->standard;
	dst_param->fw_version_patch = src_param->patch;
	dst_param->fw_version_is_test = src_param->tfw;
	dst_param->fw_timestamp = src_param->time;
	dst_param->sw_version = PON_VERSION_CODE;

	/* Caching version data in PON context */
	if (memcpy_s(&ctx->ver_data, sizeof(ctx->ver_data),
		     dst_param, sizeof(*dst_param))) {
		PON_DEBUG_ERR("memcpy_s failed");
		return PON_STATUS_MEMCPY_ERR;
	}
	/* Caching version validity in PON context */
	ctx->ver_valid = 1;

	return PON_STATUS_OK;
}

enum fapi_pon_errorcode fapi_pon_version_get(struct pon_ctx *ctx,
					     struct pon_version *param)
{
	if (!ctx)
		return PON_STATUS_INPUT_ERR;

	if (ctx->ver_valid) {
		if (memcpy_s(param, sizeof(*param),
			     &ctx->ver_data, sizeof(ctx->ver_data))) {
			PON_DEBUG_ERR("memcpy_s failed");
			return PON_STATUS_MEMCPY_ERR;
		}
		return PON_STATUS_OK;
	}

	return fapi_pon_generic_get(ctx,
				    PONFW_VERSION_CMD_ID,
				    NULL,
				    0,
				    &pon_version_get_copy,
				    param);
}

/* External calibration option constants */
#define EXT_START 56
#define EXT_LINE 36
#define EXT_RX_POW_COUNT 5
#define EXT_RX_POW (56-EXT_START)
#define EXT_TX_I_SLOPE (76-EXT_START)
#define EXT_TX_I_OFFSET (78-EXT_START)
#define EXT_TX_POW_SLOPE (80-EXT_START)
#define EXT_TX_POW_OFFSET (82-EXT_START)
#define EXT_T_SLOPE (84-EXT_START)
#define EXT_T_OFFSET (86-EXT_START)
#define EXT_V_SLOPE (88-EXT_START)
#define EXT_V_OFFSET (90-EXT_START)

/* Convert unsigned 32 bit integer value taken from EEPROM file to float */
static float to_float(unsigned char *data)
{
	union {
		uint32_t ival;
		float fval;
	} d;

	d.ival = *data << 24 | *(data + 1) << 16 |
		 *(data + 2) <<  8 | *(data + 3);

	return d.fval;
}

/* Function used to calculate rx_power value when external calibration is set.
 * RX power: Rx_PWR(uW)
 * = Rx_PWR(4) * Rx_PWR_ADe4 (16 bit unsigned integer)
 * + Rx_PWR(3) * Rx_PWR_ADe3 (16 bit unsigned integer)
 * + Rx_PWR(2) * Rx_PWR_ADe2 (16 bit unsigned integer)
 * + Rx_PWR(1) * Rx_PWR_AD (16 bit unsigned integer)
 * + Rx_PWR(0)
 * = 0xA2(56-59) * 0xA2(104-105)^4 + 0xA2(60-63)
 * * 0xA2(104-105)^3 + 0xA2(64-67) * 0xA2(104-105)^2
 * + 0xA2(68-71) * 0xA2(104-105) + 0xA2(72-75)
 */
static float rx_power_get(unsigned char data[], int32_t in_power)
{
	float power_coeff[EXT_RX_POW_COUNT];
	int i, j;

	for (j = (EXT_RX_POW_COUNT - 1), i = 0; i < EXT_RX_POW_COUNT; i++, j--)
		power_coeff[j] = to_float(&data[EXT_RX_POW + 4 * i]);

	return (float)(pow(power_coeff[4], 4) * in_power
		     + pow(power_coeff[3], 3) * in_power
		     + pow(power_coeff[2], 2) * in_power
		     + power_coeff[1] * in_power
		     + power_coeff[0]);
}

/* Function used to calculate tx_power, temperature, voltage and bias values
 * when external calibration is set.
 */
static float dmi_calculate(unsigned char *slope, unsigned char *offset,
			   int32_t value)
{
	float tmp_slope;
	int tmp_offset;

	tmp_slope = *slope + *(slope + 1)/256.0;
	tmp_offset = *offset << 8 | *(offset + 1);

	return tmp_slope * value + tmp_offset;
}

/* DMI - Diagnostic Monitoring Interface */
#define DMI_START 96
#define DMI_LINE 16
#define DMI_TEMP (96-DMI_START)
#define DMI_VOLT (98-DMI_START)
#define DMI_BIAS (100-DMI_START)
#define DMI_TX_POW (102-DMI_START)
#define DMI_RX_POW (104-DMI_START)
#define DMI_STATUS (110-DMI_START)

enum fapi_pon_errorcode
fapi_pon_optic_status_get(struct pon_ctx *ctx, struct pon_optic_status *param,
				  enum pon_tx_power_scale scale)
{
	enum fapi_pon_errorcode ret;
	unsigned char ext_data[EXT_LINE];
	unsigned char dmi_data[DMI_LINE];
	float rx_power;
	float tx_power;

	if (!ctx)
		return PON_STATUS_INPUT_ERR;

	ret = fapi_pon_eeprom_data_get(ctx, PON_DDMI_A2, dmi_data,
				       DMI_START, DMI_LINE);
	if (ret != PON_STATUS_OK)
		return ret;

	param->temperature = (int16_t)(dmi_data[DMI_TEMP] << 8 |
			     dmi_data[DMI_TEMP + 1]);
	param->voltage = (dmi_data[DMI_VOLT] << 8 | dmi_data[DMI_VOLT + 1]);
	param->bias = dmi_data[DMI_BIAS] << 8 | dmi_data[DMI_BIAS + 1];
	param->tx_power = DMI_POWER_ZERO;
	param->rx_power = DMI_POWER_ZERO;
	param->rx_los = (dmi_data[DMI_STATUS] & 0x02) ? 1 : 0;
	param->tx_disable = (dmi_data[DMI_STATUS] & 0x80) ? 1 : 0;
	param->tx_fault = (dmi_data[DMI_STATUS] & 0x04) ? 1 : 0;

	tx_power = dmi_data[DMI_TX_POW] << 8 | dmi_data[DMI_TX_POW + 1];
	rx_power = dmi_data[DMI_RX_POW] << 8 | dmi_data[DMI_RX_POW + 1];

	if (!ctx->ext_cal_valid) {
		ret = external_calibration_update(ctx);
		if (ret != PON_STATUS_OK)
			return ret;
	}

	if (ctx->ext_calibrated) {
		ret = fapi_pon_eeprom_data_get(ctx, PON_DDMI_A2, ext_data,
					       EXT_START, EXT_LINE);
		if (ret != PON_STATUS_OK)
			return ret;

		/* Temperature: T(C) = T_slope * T_AD (16 bit signed twos
		 * complement value) + T_offset
		 * = 0xA2(84-85) * 0xA2(96-97) + 0xA2(86-87)
		 */
		param->temperature = dmi_calculate(&ext_data[EXT_T_SLOPE],
						   &ext_data[EXT_T_OFFSET],
						   param->temperature);

		/* Voltage: V(uV) = V_slope * V_AD (16 bit unsigned integer)
		 * + V_offset
		 * = 0xA2(88-89) * 0xA2(98-99) + 0xA2(90-91)
		 */
		param->voltage = dmi_calculate(&ext_data[EXT_V_SLOPE],
					       &ext_data[EXT_V_OFFSET],
					       param->voltage);

		/* Laser bias current: I(uA) = I_slope * I_AD (16 bit unsigned
		 * integer) + I_offset
		 * = 0xA2(76-77) * 0xA2(100-101) + 0xA2(78-79)
		 */
		param->bias = dmi_calculate(&ext_data[EXT_TX_I_SLOPE],
					    &ext_data[EXT_TX_I_OFFSET],
					    param->bias);

		/* TX power: Tx_PWR(uW) = Tx_PWR_slope * Tx_PWR_AD (16 bit
		 * unsigned integer) + Tx_PWR_offset
		 * = 0xA2(80-81) * 0xA2(102-103) + 0xA2(82-83)
		 */
		param->tx_power = dmi_calculate(&ext_data[EXT_TX_POW_SLOPE],
						&ext_data[EXT_TX_POW_OFFSET],
						tx_power);
		tx_power = param->tx_power;

		/* RX power */
		param->rx_power = rx_power_get(ext_data, rx_power);
		rx_power = param->rx_power;
	}

	/* TX power is given in 0.1 uW/LSB or 0.2 uW/LSB */
	if (tx_power) {
		/* dBm = 10 log (P1/P0) where P1 is given in mW units
		 * and P0 = 1mW
		 */
		if (scale == TX_POWER_SCALE_0_1)
			/* A power scale factor of 0.1 µW/LSB is used.
			 * The power  input value is divided by 10000 to
			 * convert it to mW.
			 */
			tx_power = 10 * log10(tx_power / 10000.0);
		else
			/* A power scale factor of 0.2 µW/LSB is used.
			 * The power  input value is divided by 5000 to
			 * convert it to mW.
			 */
			tx_power = 10 * log10(tx_power / 5000.0);

		/* Division used to get the required granularity of
		 * 0.002 dBm/LSB
		 */
		param->tx_power = tx_power / 0.002;
	}

	/* RX power is given in 0.1 uW/LSB */
	if (rx_power) {
		/* dBm = 10 log (P1/P0) where P1 is given in mW units
		 * and P0 = 1mW
		 */
		rx_power = 10 * log10(rx_power / 10000.0);
		/* Division used to get the required granularity of
		 * 0.002 dBm/LSB
		 */
		param->rx_power = rx_power / 0.002;
	}
	return PON_STATUS_OK;
}

#define SID_PROP_START 0
#define SID_PROP_SIZE 96
#define SID_IDENTIFIER (0-SID_PROP_START)
#define SID_CONNECTOR (2-SID_PROP_START)
#define SID_SIGNALING_RATE (12-SID_PROP_START)
#define SID_VEND_NAME (20-SID_PROP_START)
#define SID_VEND_NAME_SIZE 16
#define SID_VEND_OUI (37-SID_PROP_START)
#define SID_VEND_OUI_SIZE 3
#define SID_PART_NUM (40-SID_PROP_START)
#define SID_PART_NUM_SIZE 16
#define SID_REV_LVL (56-SID_PROP_START)
#define SID_REV_LVL_SIZE 4
#define SID_WAVELEN (60-SID_PROP_START)
#define SID_SERIAL_NUM_SIZE 16
#define SID_SERIAL_NUM (68-SID_PROP_START)
#define SID_OPT_ONE (64-SID_PROP_START)
#define SID_OPT_TWO (65-SID_PROP_START)
#define SID_DATA_CODE (84-SID_PROP_START)
#define SID_DATA_CODE_SIZE 8
#define SID_TYPE (92-SID_PROP_START)
#define SID_OPT_ENH (93-SID_PROP_START)
#define SID_COMPL (94-SID_PROP_START)

enum fapi_pon_errorcode
fapi_pon_optic_properties_get(struct pon_ctx *ctx,
			      struct pon_optic_properties *param)
{
	enum fapi_pon_errorcode ret;
	unsigned char data[SID_PROP_SIZE];

	if (!ctx)
		return PON_STATUS_INPUT_ERR;

	ret = fapi_pon_eeprom_data_get(ctx, PON_DDMI_A0, data,
				       SID_PROP_START, SID_PROP_SIZE);
	if (ret != PON_STATUS_OK)
		return ret;

	memset(param, 0x0, sizeof(*param));

	if (memcpy_s(param->vendor_name, sizeof(param->vendor_name),
		     &data[SID_VEND_NAME], SID_VEND_NAME_SIZE)) {
		PON_DEBUG_ERR("memcpy_s failed");
		return PON_STATUS_MEMCPY_ERR;
	}
	if (memcpy_s(param->vendor_oui, sizeof(param->vendor_oui),
		     &data[SID_VEND_OUI], SID_VEND_OUI_SIZE)) {
		PON_DEBUG_ERR("memcpy_s failed");
		return PON_STATUS_MEMCPY_ERR;
	}
	if (memcpy_s(param->part_number, sizeof(param->part_number),
		     &data[SID_PART_NUM], SID_PART_NUM_SIZE)) {
		PON_DEBUG_ERR("memcpy_s failed");
		return PON_STATUS_MEMCPY_ERR;
	}
	if (memcpy_s(param->revision, sizeof(param->revision),
		     &data[SID_REV_LVL], SID_REV_LVL_SIZE)) {
		PON_DEBUG_ERR("memcpy_s failed");
		return PON_STATUS_MEMCPY_ERR;
	}
	if (memcpy_s(param->serial_number, sizeof(param->serial_number),
		     &data[SID_SERIAL_NUM], SID_SERIAL_NUM_SIZE)) {
		PON_DEBUG_ERR("memcpy_s failed");
		return PON_STATUS_MEMCPY_ERR;
	}
	if (memcpy_s(param->date_code, sizeof(param->date_code),
		     &data[SID_DATA_CODE], SID_DATA_CODE_SIZE)) {
		PON_DEBUG_ERR("memcpy_s failed");
		return PON_STATUS_MEMCPY_ERR;
	}
	param->identifier = data[SID_IDENTIFIER];
	param->connector = data[SID_CONNECTOR];
	param->signaling_rate = data[SID_SIGNALING_RATE];
	param->tx_wavelength = data[SID_WAVELEN] << 8 | data[SID_WAVELEN+1];
	param->high_power_lvl_decl = !!(data[SID_OPT_ONE] & 0x20);
	param->paging_implemented_ind = !!(data[SID_OPT_ONE] & 0x10);
	param->retimer_ind = !!(data[SID_OPT_ONE] & 0x08);
	param->cooled_transceiver_decl = !!(data[SID_OPT_ONE] & 0x04);
	param->power_lvl_decl = !!(data[SID_OPT_ONE] & 0x02);
	param->linear_rx_output_impl = !!(data[SID_OPT_ONE] & 0x01);
	param->rx_decision_thr_impl = !!(data[SID_OPT_TWO] & 0x80);
	param->tunable_transmitter = !!(data[SID_OPT_TWO] & 0x40);
	param->rate_select = !!(data[SID_OPT_TWO] & 0x20);
	param->tx_disable = !!(data[SID_OPT_TWO] & 0x10);
	param->tx_fault = !!(data[SID_OPT_TWO] & 0x08);
	param->signal_detect = !!(data[SID_OPT_TWO] & 0x04);
	param->rx_los = !!(data[SID_OPT_TWO] & 0x02);
	param->digital_monitoring = !!(data[SID_TYPE] & 0x40);
	param->int_calibrated = !!(data[SID_TYPE] & 0x20);
	param->ext_calibrated = !!(data[SID_TYPE] & 0x10);
	param->rx_power_measurement_type = !!(data[SID_TYPE] & 0x08);
	param->address_change_req = !!(data[SID_TYPE] & 0x04);
	param->optional_flags_impl = !!(data[SID_OPT_ENH] & 0x80);
	param->soft_tx_disable_monitor = !!(data[SID_OPT_ENH] & 0x40);
	param->soft_tx_fault_monitor = !!(data[SID_OPT_ENH] & 0x20);
	param->soft_rx_los_monitor = !!(data[SID_OPT_ENH] & 0x10);
	param->soft_rate_select_monitor = !!(data[SID_OPT_ENH] & 0x08);
	param->app_select_impl = !!(data[SID_OPT_ENH] & 0x04);
	param->soft_rate_select_ctrl_impl = !!(data[SID_OPT_ENH] & 0x02);
	param->compliance = data[SID_COMPL];

	/* Set information about external calibration in context */
	ctx->ext_calibrated = param->ext_calibrated;

	return PON_STATUS_OK;
}

static enum fapi_pon_errorcode pon_xpon_cfg_set_copy(struct pon_ctx *ctx,
					const struct pon_gpon_cfg *param)
{
	struct ponfw_xgtc_onu_config fw_param = {0};
	int i;

	pon_byte_copy(fw_param.sn, param->serial_no, PON_SERIAL_NO_SIZE);
	for (i = 0; i < 9; i++) {
		fw_param.id[i] = (uint32_t)param->reg_id[4 * i + 3];
		fw_param.id[i] |= (uint32_t)param->reg_id[4 * i + 2] << 8;
		fw_param.id[i] |= (uint32_t)param->reg_id[4 * i + 1] << 16;
		fw_param.id[i] |= (uint32_t)param->reg_id[4 * i + 0] << 24;
	}

	switch (param->mode) {
	case PON_MODE_9807_XGSPON:
		fw_param.mode = PONFW_XGTC_ONU_CONFIG_MODE_9807;
		break;
	case PON_MODE_987_XGPON:
		fw_param.mode = PONFW_XGTC_ONU_CONFIG_MODE_987;
		break;
	case PON_MODE_989_NGPON2_2G5:
		fw_param.mode = PONFW_XGTC_ONU_CONFIG_MODE_989_2G5;
		break;
	case PON_MODE_989_NGPON2_10G:
		fw_param.mode = PONFW_XGTC_ONU_CONFIG_MODE_989_10G;
		break;
	case PON_MODE_984_GPON:
		/* TODO: Remove this branch once we get rid of emulation */
		fw_param.mode = 0;
		break;
	default:
		return PON_STATUS_VALUE_RANGE_ERR;
	}
	fw_param.emergency = param->stop;
	fw_param.plev_cap = param->plev_cap;
	fw_param.to0 = param->ploam_timeout_0;
	fw_param.to1 = param->ploam_timeout_1;
	fw_param.to2 = param->ploam_timeout_2;
	fw_param.to3 = param->ploam_timeout_3;
	fw_param.to4 = param->ploam_timeout_4;
	fw_param.to5 = param->ploam_timeout_5;
	fw_param.to6 = param->ploam_timeout_6;
	fw_param.to_cpl = param->ploam_timeout_cpl;
	fw_param.to_cpi = param->ploam_timeout_cpi;
	fw_param.to_tpd = param->ploam_timeout_tpd;
	fw_param.vlan_msb = param->ident;
	/* Double negate this parameter to get boolean value */
	fw_param.coex_op = !!param->tdm_coexistence;
	fw_param.dg_dis = param->dg_dis;
	fw_param.ds_fcs_en = param->ds_fcs_en;
	fw_param.ds_ts_dis = param->ds_ts_dis;
	/* TODO: missing fw_param->lt */

	return fapi_pon_generic_set(ctx,
				    PONFW_XGTC_ONU_CONFIG_CMD_ID,
				    &fw_param,
				    sizeof(struct ponfw_xgtc_onu_config));
}

static enum fapi_pon_errorcode pon_gpon_cfg_set_copy(struct pon_ctx *ctx,
					const struct pon_gpon_cfg *param)
{
	struct ponfw_gtc_onu_config fw_param = {0};

	pon_byte_copy(fw_param.sn, param->serial_no, PON_SERIAL_NO_SIZE);

	fw_param.pw10 = param->password[9];
	fw_param.pw9 = param->password[8];
	fw_param.pw8 = param->password[7];
	fw_param.pw7 = param->password[6];
	fw_param.pw6 = param->password[5];
	fw_param.pw5 = param->password[4];
	fw_param.pw4 = param->password[3];
	fw_param.pw3 = param->password[2];
	fw_param.pw2 = param->password[1];
	fw_param.pw1 = param->password[0];

	fw_param.emergency = param->stop;
	fw_param.to1 = param->ploam_timeout_1;
	fw_param.to2 = param->ploam_timeout_2;
	fw_param.vlan_msb = param->ident;
	fw_param.dg_dis = param->dg_dis;
	fw_param.ds_fcs_en = param->ds_fcs_en;
	fw_param.ds_ts_dis = param->ds_ts_dis;

	return fapi_pon_generic_set(ctx,
			    PONFW_GTC_ONU_CONFIG_CMD_ID,
			    &fw_param,
			    sizeof(struct ponfw_gtc_onu_config));
}

enum fapi_pon_errorcode fapi_pon_gpon_cfg_set(struct pon_ctx *ctx,
					      const struct pon_gpon_cfg *param)
{
	if (!pon_mode_check(ctx, MODE_ITU_PON))
		return PON_STATUS_OPERATION_MODE_ERR;

	/* GPON mode only */
	if (pon_mode_check(ctx, MODE_984_GPON))
		return pon_gpon_cfg_set_copy(ctx, param);

	/* XG-PON/XGS-PON/NG-PON2 mode */
	return pon_xpon_cfg_set_copy(ctx, param);
}

static enum fapi_pon_errorcode pon_xpon_cfg_get_copy(struct pon_ctx *ctx,
						      const void *data,
						      size_t data_size,
						      void *priv)
{
	enum fapi_pon_errorcode ret;
	const struct ponfw_xgtc_onu_config *src_param = data;
	struct pon_gpon_cfg *dst_param = priv;
	unsigned int i;

	UNUSED(ctx);

	ret = integrity_check(dst_param, sizeof(*src_param), data_size);
	if (ret != PON_STATUS_OK)
		return ret;

	pon_byte_copy(dst_param->serial_no, src_param->sn, PON_SERIAL_NO_SIZE);
	for (i = 0; i < 9; i++) {
		dst_param->reg_id[4 * i + 3] =
			src_param->id[i] & 0xFF;
		dst_param->reg_id[4 * i + 2] =
			(src_param->id[i] & 0xFF00) >> 8;
		dst_param->reg_id[4 * i + 1] =
			(src_param->id[i] & 0xFF0000) >> 16;
		dst_param->reg_id[4 * i] =
			(src_param->id[i] & 0xFF000000) >> 24;
	}
	memset(dst_param->password, ' ', sizeof(dst_param->password));

	switch (src_param->mode) {
	case PONFW_XGTC_ONU_CONFIG_MODE_9807:
		dst_param->mode = PON_MODE_9807_XGSPON;
		break;
	case PONFW_XGTC_ONU_CONFIG_MODE_987:
		dst_param->mode = PON_MODE_987_XGPON;
		break;
	case PONFW_XGTC_ONU_CONFIG_MODE_989_2G5:
		dst_param->mode = PON_MODE_989_NGPON2_2G5;
		break;
	case PONFW_XGTC_ONU_CONFIG_MODE_989_10G:
		dst_param->mode = PON_MODE_989_NGPON2_10G;
		break;
	default:
		dst_param->mode = PON_MODE_UNKNOWN;
	}
	dst_param->stop = src_param->emergency;
	dst_param->plev_cap = src_param->plev_cap;
	dst_param->ploam_timeout_0 = src_param->to0;
	dst_param->ploam_timeout_1 = src_param->to1;
	dst_param->ploam_timeout_2 = src_param->to2;
	dst_param->ploam_timeout_3 = src_param->to3;
	dst_param->ploam_timeout_4 = src_param->to4;
	dst_param->ploam_timeout_5 = src_param->to5;
	dst_param->ploam_timeout_6 = src_param->to6;
	dst_param->ploam_timeout_cpl = src_param->to_cpl;
	dst_param->ploam_timeout_cpi = src_param->to_cpi;
	dst_param->ploam_timeout_tpd = src_param->to_tpd;
	dst_param->ident = src_param->vlan_msb;
	dst_param->tdm_coexistence = src_param->coex_op;
	dst_param->dg_dis = src_param->dg_dis;
	dst_param->ds_fcs_en = src_param->ds_fcs_en;
	dst_param->ds_ts_dis = src_param->ds_ts_dis;

	return PON_STATUS_OK;
}

static enum fapi_pon_errorcode pon_gpon_cfg_get_copy(struct pon_ctx *ctx,
						     const void *data,
						     size_t data_size,
						     void *priv)
{
	enum fapi_pon_errorcode ret;
	const struct ponfw_gtc_onu_config *src_param = data;
	struct pon_gpon_cfg *dst_param = priv;

	UNUSED(ctx);

	ret = integrity_check(dst_param, sizeof(*src_param), data_size);
	if (ret != PON_STATUS_OK)
		return ret;

	pon_byte_copy(dst_param->serial_no, src_param->sn, PON_SERIAL_NO_SIZE);

	dst_param->password[9] = src_param->pw10;
	dst_param->password[8] = src_param->pw9;
	dst_param->password[7] = src_param->pw8;
	dst_param->password[6] = src_param->pw7;
	dst_param->password[5] = src_param->pw6;
	dst_param->password[4] = src_param->pw5;
	dst_param->password[3] = src_param->pw4;
	dst_param->password[2] = src_param->pw3;
	dst_param->password[1] = src_param->pw2;
	dst_param->password[0] = src_param->pw1;

	memset(dst_param->reg_id, 0, sizeof(dst_param->reg_id));

	dst_param->mode = PON_MODE_984_GPON;
	dst_param->stop = src_param->emergency;
	dst_param->plev_cap = 0;
	dst_param->ploam_timeout_0 = 0;
	dst_param->ploam_timeout_1 = src_param->to1;
	dst_param->ploam_timeout_2 = src_param->to2;
	dst_param->ploam_timeout_3 = 0;
	dst_param->ploam_timeout_4 = 0;
	dst_param->ploam_timeout_5 = 0;
	dst_param->ploam_timeout_6 = 0;
	dst_param->ploam_timeout_cpl = 0;
	dst_param->ploam_timeout_cpi = 0;
	dst_param->ploam_timeout_tpd = 0;
	dst_param->ident = src_param->vlan_msb;
	dst_param->tdm_coexistence = 0;
	dst_param->dg_dis = src_param->dg_dis;
	dst_param->ds_fcs_en = src_param->ds_fcs_en;
	dst_param->ds_ts_dis = src_param->ds_ts_dis;

	return PON_STATUS_OK;
}

enum fapi_pon_errorcode fapi_pon_gpon_cfg_get(struct pon_ctx *ctx,
					      struct pon_gpon_cfg *param)
{
	if (!pon_mode_check(ctx, MODE_ITU_PON))
		return PON_STATUS_OPERATION_MODE_ERR;

	/* GPON mode only */
	if (pon_mode_check(ctx, MODE_984_GPON))
		return fapi_pon_generic_get(ctx,
				    PONFW_GTC_ONU_CONFIG_CMD_ID,
				    NULL,
				    0,
				    &pon_gpon_cfg_get_copy,
				    param);
	/* XG-PON/XGS-PON/NG-PON2 mode */
	return fapi_pon_generic_get(ctx,
			    PONFW_XGTC_ONU_CONFIG_CMD_ID,
			    NULL,
			    0,
			    &pon_xpon_cfg_get_copy,
			    param);
}

static enum fapi_pon_errorcode fapi_pon_cred_set_xgtc(struct pon_ctx *ctx,
					const struct pon_cred_cfg *param)
{
	struct ponfw_xgtc_credentials_config fw_param = {0};
	int i;

	pon_byte_copy(fw_param.sn, param->serial_no, PON_SERIAL_NO_SIZE);

	for (i = 0; i < 9; i++) {
		fw_param.id[i] = (uint32_t)param->reg_id[4 * i + 3];
		fw_param.id[i] |= (uint32_t)param->reg_id[4 * i + 2] << 8;
		fw_param.id[i] |= (uint32_t)param->reg_id[4 * i + 1] << 16;
		fw_param.id[i] |= (uint32_t)param->reg_id[4 * i + 0] << 24;
	}

	return fapi_pon_generic_set(
				ctx,
				PONFW_XGTC_CREDENTIALS_CONFIG_CMD_ID,
				&fw_param,
				sizeof(struct ponfw_xgtc_credentials_config));
}

static enum fapi_pon_errorcode fapi_pon_cred_set_gtc(struct pon_ctx *ctx,
					const struct pon_cred_cfg *param)
{
	struct ponfw_gtc_credentials_config fw_param = {0};

	pon_byte_copy(fw_param.sn, param->serial_no, PON_SERIAL_NO_SIZE);

	fw_param.pw10 = param->password[9];
	fw_param.pw9 = param->password[8];
	fw_param.pw8 = param->password[7];
	fw_param.pw7 = param->password[6];
	fw_param.pw6 = param->password[5];
	fw_param.pw5 = param->password[4];
	fw_param.pw4 = param->password[3];
	fw_param.pw3 = param->password[2];
	fw_param.pw2 = param->password[1];
	fw_param.pw1 = param->password[0];

	return fapi_pon_generic_set(ctx,
				PONFW_GTC_CREDENTIALS_CONFIG_CMD_ID,
				&fw_param,
				sizeof(struct ponfw_gtc_credentials_config));
}

enum fapi_pon_errorcode fapi_pon_cred_set(struct pon_ctx *ctx,
					  const struct pon_cred_cfg *param)
{
	if (!pon_mode_check(ctx, MODE_ITU_PON))
		return PON_STATUS_OPERATION_MODE_ERR;

	/* GPON mode only */
	if (pon_mode_check(ctx, MODE_984_GPON))
		return fapi_pon_cred_set_gtc(ctx, param);
	/* XG-PON/XGS-PON/NG-PON2 mode */
	return fapi_pon_cred_set_xgtc(ctx, param);
}

static enum fapi_pon_errorcode pon_xgtc_cred_get_copy(struct pon_ctx *ctx,
						      const void *data,
						      size_t data_size,
						      void *priv)
{
	enum fapi_pon_errorcode ret;
	const struct ponfw_xgtc_credentials_config *src_param = data;
	struct pon_cred_cfg *dst_param = priv;
	unsigned int i;

	UNUSED(ctx);

	ret = integrity_check(dst_param, sizeof(*src_param), data_size);
	if (ret != PON_STATUS_OK)
		return ret;

	pon_byte_copy(dst_param->serial_no, src_param->sn, PON_SERIAL_NO_SIZE);

	for (i = 0; i < 9; i++) {
		dst_param->reg_id[4 * i + 3] =
			src_param->id[i] & 0xFF;
		dst_param->reg_id[4 * i + 2] =
			(src_param->id[i] & 0xFF00) >> 8;
		dst_param->reg_id[4 * i + 1] =
			(src_param->id[i] & 0xFF0000) >> 16;
		dst_param->reg_id[4 * i] =
			(src_param->id[i] & 0xFF000000) >> 24;
	}

	memset(dst_param->password, ' ', sizeof(dst_param->password));

	return PON_STATUS_OK;
}

static enum fapi_pon_errorcode pon_gtc_cred_get_copy(struct pon_ctx *ctx,
						      const void *data,
						      size_t data_size,
						      void *priv)
{
	enum fapi_pon_errorcode ret;
	const struct ponfw_gtc_credentials_config *src_param = data;
	struct pon_cred_cfg *dst_param = priv;

	UNUSED(ctx);

	ret = integrity_check(dst_param, sizeof(*src_param), data_size);
	if (ret != PON_STATUS_OK)
		return ret;

	pon_byte_copy(dst_param->serial_no, src_param->sn, PON_SERIAL_NO_SIZE);

	dst_param->password[9] = src_param->pw10;
	dst_param->password[8] = src_param->pw9;
	dst_param->password[7] = src_param->pw8;
	dst_param->password[6] = src_param->pw7;
	dst_param->password[5] = src_param->pw6;
	dst_param->password[4] = src_param->pw5;
	dst_param->password[3] = src_param->pw4;
	dst_param->password[2] = src_param->pw3;
	dst_param->password[1] = src_param->pw2;
	dst_param->password[0] = src_param->pw1;

	memset(dst_param->reg_id, 0, sizeof(dst_param->reg_id));

	return PON_STATUS_OK;
}

enum fapi_pon_errorcode fapi_pon_cred_get(struct pon_ctx *ctx,
					  struct pon_cred_cfg *param)
{
	if (!pon_mode_check(ctx, MODE_ITU_PON))
		return PON_STATUS_OPERATION_MODE_ERR;

	/* GPON mode only */
	if (pon_mode_check(ctx, MODE_984_GPON))
		return fapi_pon_generic_get(ctx,
				    PONFW_GTC_CREDENTIALS_CONFIG_CMD_ID,
				    NULL,
				    0,
				    &pon_gtc_cred_get_copy,
				    param);
	/* XG-PON/XGS-PON/NG-PON2 mode */
	return fapi_pon_generic_get(ctx,
			    PONFW_XGTC_CREDENTIALS_CONFIG_CMD_ID,
			    NULL,
			    0,
			    &pon_xgtc_cred_get_copy,
			    param);
}

static enum fapi_pon_errorcode gpon_serial_number_get_copy(struct pon_ctx *ctx,
							   const void *data,
							   size_t data_size,
							   void *priv)
{
	enum fapi_pon_errorcode ret;
	const struct ponfw_gtc_credentials_config *src_param = data;
	struct pon_serial_number *dst_param = priv;

	UNUSED(ctx);

	ret = integrity_check(dst_param, sizeof(*src_param), data_size);
	if (ret != PON_STATUS_OK)
		return ret;

	pon_byte_copy(dst_param->serial_no, src_param->sn, PON_SERIAL_NO_SIZE);

	return PON_STATUS_OK;
}

static enum fapi_pon_errorcode xpon_serial_number_get_copy(struct pon_ctx *ctx,
							   const void *data,
							   size_t data_size,
							   void *priv)
{
	enum fapi_pon_errorcode ret;
	const struct ponfw_xgtc_credentials_config *src_param = data;
	struct pon_serial_number *dst_param = priv;

	UNUSED(ctx);

	ret = integrity_check(dst_param, sizeof(*src_param), data_size);
	if (ret != PON_STATUS_OK)
		return ret;

	pon_byte_copy(dst_param->serial_no, src_param->sn, PON_SERIAL_NO_SIZE);

	return PON_STATUS_OK;
}

enum fapi_pon_errorcode
fapi_pon_serial_number_get(struct pon_ctx *ctx,
			   struct pon_serial_number *param)
{
	enum fapi_pon_errorcode ret;

	if (!pon_mode_check(ctx, MODE_ITU_PON))
		return PON_STATUS_OPERATION_MODE_ERR;

	ret = debug_support_check(ctx);
	if (ret != PON_STATUS_OK)
		return ret;

	/* GPON mode only */
	if (pon_mode_check(ctx, MODE_984_GPON))
		return fapi_pon_generic_get(ctx,
					    PONFW_GTC_CREDENTIALS_CONFIG_CMD_ID,
					    NULL,
					    0,
					    &gpon_serial_number_get_copy,
					    param);
	/* XG-PON/XGS-PON/NG-PON2 mode */
	return fapi_pon_generic_get(ctx,
				    PONFW_XGTC_CREDENTIALS_CONFIG_CMD_ID,
				    NULL,
				    0,
				    &xpon_serial_number_get_copy,
				    param);
}

static enum fapi_pon_errorcode pon_registration_id_get_copy(struct pon_ctx *ctx,
							    const void *data,
							    size_t data_size,
							    void *priv)
{
	enum fapi_pon_errorcode ret;
	const struct ponfw_xgtc_credentials_config *src_param = data;
	struct pon_registration_id *dst_param = priv;
	unsigned int i;

	UNUSED(ctx);

	ret = integrity_check(dst_param, sizeof(*src_param), data_size);
	if (ret != PON_STATUS_OK)
		return ret;

	for (i = 0; i < 9; i++) {
		dst_param->reg_id[4 * i + 3] =
			src_param->id[i] & 0xFF;
		dst_param->reg_id[4 * i + 2] =
			(src_param->id[i] & 0xFF00) >> 8;
		dst_param->reg_id[4 * i + 1] =
			(src_param->id[i] & 0xFF0000) >> 16;
		dst_param->reg_id[4 * i] =
			(src_param->id[i] & 0xFF000000) >> 24;
	}

	return PON_STATUS_OK;
}

enum fapi_pon_errorcode
fapi_pon_registration_id_get(struct pon_ctx *ctx,
			     struct pon_registration_id *param)
{
	enum fapi_pon_errorcode ret;

	if (!pon_mode_check(ctx, MODE_ITU_PON))
		return PON_STATUS_OPERATION_MODE_ERR;

	ret = debug_support_check(ctx);
	if (ret != PON_STATUS_OK)
		return ret;

	/* XG-PON/XGS-PON/NG-PON2 mode only */
	if (pon_mode_check(ctx, MODE_984_GPON))
		return PON_STATUS_OPERATION_MODE_ERR;

	return fapi_pon_generic_get(ctx,
				    PONFW_XGTC_CREDENTIALS_CONFIG_CMD_ID,
				    NULL,
				    0,
				    &pon_registration_id_get_copy,
				    param);
}

static enum fapi_pon_errorcode pon_password_get_copy(struct pon_ctx *ctx,
						     const void *data,
						     size_t data_size,
						     void *priv)
{
	enum fapi_pon_errorcode ret;
	const struct ponfw_gtc_credentials_config *src_param = data;
	struct pon_password *dst_param = priv;

	UNUSED(ctx);

	ret = integrity_check(dst_param, sizeof(*src_param), data_size);
	if (ret != PON_STATUS_OK)
		return ret;

	dst_param->password[9] = src_param->pw10;
	dst_param->password[8] = src_param->pw9;
	dst_param->password[7] = src_param->pw8;
	dst_param->password[6] = src_param->pw7;
	dst_param->password[5] = src_param->pw6;
	dst_param->password[4] = src_param->pw5;
	dst_param->password[3] = src_param->pw4;
	dst_param->password[2] = src_param->pw3;
	dst_param->password[1] = src_param->pw2;
	dst_param->password[0] = src_param->pw1;

	return PON_STATUS_OK;
}

enum fapi_pon_errorcode fapi_pon_password_get(struct pon_ctx *ctx,
					      struct pon_password *param)
{
	enum fapi_pon_errorcode ret;

	/* GPON mode only */
	if (!pon_mode_check(ctx, MODE_984_GPON))
		return PON_STATUS_OPERATION_MODE_ERR;

	ret = debug_support_check(ctx);
	if (ret != PON_STATUS_OK)
		return ret;

	return fapi_pon_generic_get(ctx,
				    PONFW_GTC_CREDENTIALS_CONFIG_CMD_ID,
				    NULL,
				    0,
				    &pon_password_get_copy,
				    param);
}

enum fapi_pon_errorcode fapi_pon_omci_cfg_set(struct pon_ctx *ctx,
					      const struct pon_omci_cfg *param)
{
	struct ponfw_omci_encap_config fw_param = {0};

	if (!param)
		return PON_STATUS_INPUT_ERR;

	if (!pon_mode_check(ctx, MODE_ITU_PON))
		return PON_STATUS_OPERATION_MODE_ERR;

	fw_param.macsa1 = ((uint32_t)param->mac_sa[0] << 8) |
		param->mac_sa[1];
	fw_param.macsa0 = ((uint32_t)param->mac_sa[2] << 24) |
		((uint32_t)param->mac_sa[3] << 16) |
		((uint32_t)param->mac_sa[4] << 8) |
		param->mac_sa[5];

	fw_param.macda1 = ((uint32_t)param->mac_da[0] << 8) |
		param->mac_da[1];
	fw_param.macda0 = ((uint32_t)param->mac_da[2] << 24) |
		((uint32_t)param->mac_da[3] << 16) |
		((uint32_t)param->mac_da[4] << 8) |
		param->mac_da[5];

	fw_param.ethtype = param->ethertype;

	fw_param.omcip1 = param->protocol[0];
	fw_param.omcip0 = ((uint32_t)param->protocol[1] << 24) |
		((uint32_t)param->protocol[2] << 16) |
		((uint32_t)param->protocol[3] << 8) |
		param->protocol[4];

	return fapi_pon_generic_set(ctx,
				PONFW_OMCI_ENCAP_CONFIG_CMD_ID,
				&fw_param,
				sizeof(struct ponfw_omci_encap_config));
}

static enum fapi_pon_errorcode pon_omci_cfg_get_copy(struct pon_ctx *ctx,
						     const void *data,
						     size_t data_size,
						     void *priv)
{
	enum fapi_pon_errorcode ret;
	const struct ponfw_omci_encap_config *src_param = data;
	struct pon_omci_cfg *dst_param = priv;

	UNUSED(ctx);

	ret = integrity_check(dst_param, sizeof(*src_param), data_size);
	if (ret != PON_STATUS_OK)
		return ret;

	dst_param->mac_sa[0] = (src_param->macsa1 & 0xff00) >> 8;
	dst_param->mac_sa[1] = src_param->macsa1 & 0xff;
	dst_param->mac_sa[2] = (src_param->macsa0 & 0xff000000) >> 24;
	dst_param->mac_sa[3] = (src_param->macsa0 & 0xff0000) >> 16;
	dst_param->mac_sa[4] = (src_param->macsa0 & 0xff00) >> 8;
	dst_param->mac_sa[5] = src_param->macsa0 & 0xff;

	dst_param->mac_da[0] = (src_param->macda1 & 0xff00) >> 8;
	dst_param->mac_da[1] = src_param->macda1 & 0xff;
	dst_param->mac_da[2] = (src_param->macda0 & 0xff000000) >> 24;
	dst_param->mac_da[3] = (src_param->macda0 & 0xff0000) >> 16;
	dst_param->mac_da[4] = (src_param->macda0 & 0xff00) >> 8;
	dst_param->mac_da[5] = src_param->macda0 & 0xff;

	dst_param->ethertype = src_param->ethtype;

	dst_param->protocol[0] = src_param->omcip1 & 0xff;
	dst_param->protocol[1] = (src_param->omcip0 & 0xff000000) >> 24;
	dst_param->protocol[2] = (src_param->omcip0 & 0xff0000) >> 16;
	dst_param->protocol[3] = (src_param->omcip0 & 0xff00) >> 8;
	dst_param->protocol[4] = src_param->omcip0 & 0xff;

	return PON_STATUS_OK;
}

enum fapi_pon_errorcode fapi_pon_omci_cfg_get(struct pon_ctx *ctx,
					      struct pon_omci_cfg *param)
{
	if (!pon_mode_check(ctx, MODE_ITU_PON))
		return PON_STATUS_OPERATION_MODE_ERR;

	return fapi_pon_generic_get(ctx,
				    PONFW_OMCI_ENCAP_CONFIG_CMD_ID,
				    NULL,
				    0,
				    &pon_omci_cfg_get_copy,
				    param);
}

static enum fapi_pon_errorcode pon_gem_port_index_get_copy(struct pon_ctx *ctx,
							  const void *data,
							  size_t data_size,
							  void *priv)
{
	enum fapi_pon_errorcode ret;
	const struct ponfw_gem_port_idx *src_param = data;
	struct pon_gem_port *dst_param = priv;

	UNUSED(ctx);

	ret = integrity_check(dst_param, sizeof(*src_param), data_size);
	if (ret != PON_STATUS_OK)
		return ret;

	dst_param->gem_port_index = (uint8_t)src_param->gem_port_idx;
	dst_param->gem_port_id = (uint16_t)src_param->gem_port_id;
	dst_param->encryption_key_ring = (uint8_t)src_param->enc;
	dst_param->is_downstream =
		(uint8_t)(!!(src_param->dir & PONFW_GEM_PORT_IDX_DIR_DS));
	dst_param->is_upstream =
		(uint8_t)(!!(src_param->dir & PONFW_GEM_PORT_IDX_DIR_US));
	dst_param->payload_type = (uint8_t)src_param->tt;
	dst_param->gem_max_size = (uint16_t)src_param->max_gem_size;

	return PON_STATUS_OK;
}

enum fapi_pon_errorcode
fapi_pon_gem_port_index_get(struct pon_ctx *ctx, uint8_t gpix,
			    struct pon_gem_port *param_out)
{
	struct ponfw_gem_port_idx fw_param = {0};
	struct pon_range_limits limits = {0};
	struct pon_allocation_id alloc_param;
	enum fapi_pon_errorcode ret;
	enum fapi_pon_errorcode alloc_ret;

	if (!param_out)
		return PON_STATUS_INPUT_ERR;

	if (!pon_mode_check(ctx, MODE_ITU_PON))
		return PON_STATUS_OPERATION_MODE_ERR;

	ret = fapi_pon_limits_get(ctx, &limits);
	if (ret != PON_STATUS_OK)
		return ret;

	if (gpix > limits.gem_port_idx_max)
		return PON_STATUS_VALUE_RANGE_ERR;

	ASSIGN_AND_OVERFLOW_CHECK(fw_param.gem_port_idx, gpix);

	ret = fapi_pon_generic_get(ctx,
				   PONFW_GEM_PORT_IDX_CMD_ID,
				   &fw_param,
				   PONFW_GEM_PORT_IDX_LENR,
				   &pon_gem_port_index_get_copy,
				   param_out);
	if (ret != PON_STATUS_OK)
		return ret;

	/* not upstream and not downstream means it is disabled. */
	if (param_out->is_downstream == 0 && param_out->is_upstream == 0)
		return PON_STATUS_GEM_PORT_ID_NOT_EXISTS_ERR;

	alloc_ret = fapi_pon_gem_port_alloc_get(ctx, param_out->gem_port_id,
						&alloc_param);

	/*
	 * PON_STATUS_GEM_PORT_ID_NOT_EXISTS_ERR means there is no allocation ID
	 * associated with the GEM port. Only fail in case of another unexpected
	 * error.
	 */
	if (alloc_ret == PON_STATUS_OK) {
		param_out->alloc_valid = PON_ALLOC_VALID;
		param_out->alloc_id = alloc_param.alloc_id;
	} else if (alloc_ret == PON_STATUS_GEM_PORT_ID_NOT_EXISTS_ERR) {
		param_out->alloc_valid = PON_ALLOC_INVALID;
		param_out->alloc_id = UINT16_MAX;
	} else {
		return alloc_ret;
	}

	return ret;
}

static enum fapi_pon_errorcode pon_gem_port_id_get_copy(struct pon_ctx *ctx,
							const void *data,
							size_t data_size,
							void *priv)
{
	enum fapi_pon_errorcode ret;
	const struct ponfw_gem_port_id *src_param = data;
	struct pon_gem_port *dst_param = priv;

	UNUSED(ctx);

	ret = integrity_check(dst_param, sizeof(*src_param), data_size);
	if (ret != PON_STATUS_OK)
		return ret;

	dst_param->gem_port_index = (uint8_t)src_param->gem_port_idx;
	dst_param->gem_port_id = (uint16_t)src_param->gem_port_id;
	dst_param->encryption_key_ring = (uint8_t)src_param->enc;
	dst_param->is_downstream =
		(uint8_t)(!!(src_param->dir & PONFW_GEM_PORT_ID_DIR_DS));
	dst_param->is_upstream =
		(uint8_t)(!!(src_param->dir & PONFW_GEM_PORT_ID_DIR_US));
	dst_param->payload_type = (uint8_t)src_param->tt;
	dst_param->gem_max_size = (uint16_t)src_param->max_gem_size;
	dst_param->alloc_link_ref = src_param->alloc_link_ref;
	if (dst_param->alloc_link_ref) {
		dst_param->alloc_valid = PON_ALLOC_VALID;
		dst_param->alloc_id = (uint16_t)src_param->alloc_id;
	} else {
		dst_param->alloc_valid = PON_ALLOC_INVALID;
		dst_param->alloc_id = UINT16_MAX;
	}

	return PON_STATUS_OK;
}

enum fapi_pon_errorcode fapi_pon_gem_port_id_get(struct pon_ctx *ctx,
	uint16_t gem_port_id, struct pon_gem_port *param_out)
{
	struct ponfw_gem_port_id fw_param = {0};
	struct pon_range_limits limits = {0};
	enum fapi_pon_errorcode ret;

	if (!pon_mode_check(ctx, MODE_ITU_PON))
		return PON_STATUS_OPERATION_MODE_ERR;

	ret = fapi_pon_limits_get(ctx, &limits);
	if (ret != PON_STATUS_OK)
		return ret;

	if (gem_port_id > limits.gem_port_id_max)
		return PON_STATUS_VALUE_RANGE_ERR;

	ASSIGN_AND_OVERFLOW_CHECK(fw_param.gem_port_id, gem_port_id);

	ret = fapi_pon_generic_get(ctx,
				   PONFW_GEM_PORT_ID_CMD_ID,
				   &fw_param,
				   PONFW_GEM_PORT_ID_LENR,
				   &pon_gem_port_id_get_copy,
				   param_out);
	if (ret == PON_STATUS_FW_NACK)
		return PON_STATUS_GEM_PORT_ID_NOT_EXISTS_ERR;

	return ret;
}

static enum fapi_pon_errorcode pon_alloc_id_copy(struct pon_ctx *ctx,
						 const void *data,
						 size_t data_size,
						 void *priv)
{
	enum fapi_pon_errorcode ret;
	const struct ponfw_alloc_id_link *src_param = data;
	struct pon_allocation_index *dst_param = priv;

	UNUSED(ctx);

	ret = integrity_check(dst_param, sizeof(*src_param), data_size);
	if (ret != PON_STATUS_OK)
		return ret;

	dst_param->alloc_index = (uint8_t)src_param->alloc_idx;
	dst_param->alloc_link_ref = src_param->alloc_link_ref;
	dst_param->hw_status = src_param->hw_status;

	return PON_STATUS_OK;
}

static enum fapi_pon_errorcode
pon_alloc_id_get(struct pon_ctx *ctx, uint16_t alloc_id,
		 struct pon_allocation_index *param_out,
		 bool access_control, uint8_t qos_idx)
{
	struct ponfw_alloc_id_link fw_param = {0};
	struct pon_range_limits limits = {0};
	enum fapi_pon_errorcode ret;

	if (!pon_mode_check(ctx, MODE_ITU_PON))
		return PON_STATUS_OPERATION_MODE_ERR;

	ret = fapi_pon_limits_get(ctx, &limits);
	if (ret != PON_STATUS_OK)
		return ret;

	if (alloc_id > limits.alloc_id_max)
		return PON_STATUS_VALUE_RANGE_ERR;

	ASSIGN_AND_OVERFLOW_CHECK(fw_param.alloc_id, alloc_id);
	ASSIGN_AND_OVERFLOW_CHECK(fw_param.qos_idx, qos_idx);

	if (access_control)
		fw_param.ctr = 1;

	ret = fapi_pon_generic_get(ctx,
				   PONFW_ALLOC_ID_LINK_CMD_ID,
				   &fw_param,
				   PONFW_ALLOC_ID_LINK_LENR,
				   &pon_alloc_id_copy,
				   param_out);
	if (ret == PON_STATUS_FW_NACK)
		return PON_STATUS_ALLOC_ID_MISSING;

	return ret;
}

enum fapi_pon_errorcode
fapi_pon_alloc_id_get(struct pon_ctx *ctx, uint16_t alloc_id,
		      struct pon_allocation_index *param_out)
{
	return pon_alloc_id_get(ctx, alloc_id, param_out, false, 0);
}

static enum fapi_pon_errorcode pon_alloc_index_get_copy(struct pon_ctx *ctx,
							const void *data,
							size_t data_size,
							void *priv)
{
	enum fapi_pon_errorcode ret;
	const struct ponfw_debug_alloc_idx *src_param = data;
	struct pon_allocation_id *dst_param = priv;

	UNUSED(ctx);

	ret = integrity_check(dst_param, sizeof(*src_param), data_size);
	if (ret != PON_STATUS_OK)
		return ret;

	if (src_param->status == PONFW_DEBUG_ALLOC_IDX_STATUS_UNUSED)
		return PON_STATUS_ERR;

	dst_param->alloc_id = (uint16_t)src_param->alloc_id;
	dst_param->alloc_link_ref = src_param->alloc_link_ref;
	dst_param->status = src_param->status;

	return PON_STATUS_OK;
}

enum fapi_pon_errorcode
fapi_pon_alloc_index_get(struct pon_ctx *ctx, uint8_t alloc_index,
			 struct pon_allocation_id *param_out)
{
	struct ponfw_debug_alloc_idx fw_param = {0};
	struct pon_range_limits limits = {0};
	enum fapi_pon_errorcode ret;

	if (!pon_mode_check(ctx, MODE_ITU_PON))
		return PON_STATUS_OPERATION_MODE_ERR;

	ret = debug_support_check(ctx);
	if (ret != PON_STATUS_OK)
		return ret;

	ret = fapi_pon_limits_get(ctx, &limits);
	if (ret != PON_STATUS_OK)
		return ret;

	if (alloc_index > limits.alloc_idx_max)
		return PON_STATUS_VALUE_RANGE_ERR;

	ASSIGN_AND_OVERFLOW_CHECK(fw_param.alloc_idx, alloc_index);

	ret = fapi_pon_generic_get(ctx,
				   PONFW_DEBUG_ALLOC_IDX_CMD_ID,
				   &fw_param,
				   PONFW_DEBUG_ALLOC_IDX_LENR,
				   &pon_alloc_index_get_copy,
				   param_out);
	if (ret == PON_STATUS_FW_NACK)
		return PON_STATUS_ALLOC_ID_MISSING;

	return ret;
}

static enum fapi_pon_errorcode pon_status_get_copy_xgtc(struct pon_ctx *ctx,
							const void *data,
							size_t data_size,
							void *priv)
{
	enum fapi_pon_errorcode ret;
	const struct ponfw_onu_status *src_param = data;
	struct pon_gpon_status *dst_param = priv;

	UNUSED(ctx);

	ret = integrity_check(dst_param, sizeof(*src_param), data_size);
	if (ret != PON_STATUS_OK)
		return ret;

	dst_param->gem_ports = src_param->gem_port_cnt;
	dst_param->alloc_id = (uint16_t)src_param->alloc_id_cnt;
	dst_param->onu_resp_time = src_param->onu_resp_time;
	dst_param->gtc_stat = src_param->gtc_stat;
	dst_param->psm_state = src_param->psm_stat;
	dst_param->fec_status_us = (src_param->fec_stat & 0x02) >> 1;
	dst_param->fec_status_ds = src_param->fec_stat & 0x01;
	dst_param->onu_id = src_param->onu_id;
	dst_param->eq_del = src_param->eq_del;
	dst_param->pon_id[6] = (uint8_t)src_param->pon_id_hi;
	dst_param->pon_id[5] = (uint8_t)(src_param->pon_id_hi >> 8);
	dst_param->pon_id[4] = (uint8_t)(src_param->pon_id_hi >> 16);
	dst_param->pon_id[3] = (uint8_t)src_param->pon_id;
	dst_param->pon_id[2] = (uint8_t)(src_param->pon_id >> 8);
	dst_param->pon_id[1] = (uint8_t)(src_param->pon_id >> 16);
	dst_param->pon_id[0] = (uint8_t)(src_param->pon_id >> 24);
	dst_param->oc_tol = src_param->tol;
	dst_param->oc_pit = src_param->pit;
	dst_param->oc_coex = src_param->coex;
	dst_param->ds_ch_index = src_param->dwlch_id;
	dst_param->us_ch_index = src_param->uwlch_id;
	/** TODO:
	 *  The C and R values are not yet supported by the firmware message
	 *  and shall be set to 0.
	 */
	dst_param->oc_c = 0;
	dst_param->oc_r = 0;

	switch (src_param->pon_mode) {
	case PONFW_ONU_STATUS_PON_MODE_984:
		dst_param->pon_mode = PON_MODE_984_GPON;
		break;
	case PONFW_ONU_STATUS_PON_MODE_987:
		dst_param->pon_mode = PON_MODE_987_XGPON;
		break;
	case PONFW_ONU_STATUS_PON_MODE_9807:
		dst_param->pon_mode = PON_MODE_9807_XGSPON;
		break;
	case PONFW_ONU_STATUS_PON_MODE_989_2G5:
		dst_param->pon_mode = PON_MODE_989_NGPON2_2G5;
		break;
	case PONFW_ONU_STATUS_PON_MODE_989_10G:
		dst_param->pon_mode = PON_MODE_989_NGPON2_10G;
		break;
	default:
		dst_param->pon_mode = PON_MODE_UNKNOWN;
		break;
	}

	switch (src_param->ds_rate) {
	case PONFW_ONU_STATUS_DS_RATE_2G4:
		dst_param->pon_ds_rate = PON_DS_RATE_2G4;
		break;
	case PONFW_ONU_STATUS_DS_RATE_9G9:
		dst_param->pon_ds_rate = PON_DS_RATE_9G9;
		break;
	default:
		dst_param->pon_ds_rate = PON_DS_RATE_NONE;
		break;
	}

	switch (src_param->us_rate) {
	case PONFW_ONU_STATUS_US_RATE_1G2:
		dst_param->pon_us_rate = PON_US_RATE_1G2;
		break;
	case PONFW_ONU_STATUS_US_RATE_2G4:
		dst_param->pon_us_rate = PON_US_RATE_2G4;
		break;
	case PONFW_ONU_STATUS_US_RATE_9G9:
		dst_param->pon_us_rate = PON_US_RATE_9G9;
		break;
	default:
		dst_param->pon_us_rate = PON_US_RATE_NONE;
		break;
	}

	return PON_STATUS_OK;
}

static enum fapi_pon_errorcode pon_status_get_copy_gtc(struct pon_ctx *ctx,
						       const void *data,
						       size_t data_size,
						       void *priv)
{
	enum fapi_pon_errorcode ret;
	const struct ponfw_ploam_state *src_param = data;
	struct pon_gpon_status *dst_param = priv;

	UNUSED(ctx);

	ret = integrity_check(dst_param, sizeof(*src_param), data_size);
	if (ret != PON_STATUS_OK)
		return ret;

	dst_param->ploam_state = src_param->ploam_act;
	dst_param->ploam_state_previous = src_param->ploam_prev;
	dst_param->time_prev = src_param->ploam_time /* prev_counter */;
	dst_param->ploam_state_change_reason = src_param->reason;
	dst_param->auth_status = 0;

	return PON_STATUS_OK;
}

static enum fapi_pon_errorcode pon_status_get_copy_xgtc_onu(struct pon_ctx *ctx,
							    const void *data,
							    size_t data_size,
							    void *priv)
{
	enum fapi_pon_errorcode ret;
	const struct ponfw_xgtc_auth_status *src_param = data;
	struct pon_gpon_status *dst_param = priv;

	UNUSED(ctx);

	ret = integrity_check(dst_param, sizeof(*src_param), data_size);
	if (ret != PON_STATUS_OK)
		return ret;

	dst_param->auth_status = src_param->authstat;

	return PON_STATUS_OK;
}

enum fapi_pon_errorcode fapi_pon_gpon_status_get(struct pon_ctx *ctx,
						 struct pon_gpon_status *param)
{
	enum fapi_pon_errorcode ret;

	if (!pon_mode_check(ctx, MODE_ITU_PON))
		return PON_STATUS_OPERATION_MODE_ERR;

	ret = fapi_pon_generic_get(ctx,
				   PONFW_ONU_STATUS_CMD_ID,
				   NULL,
				   0,
				   &pon_status_get_copy_xgtc,
				   param);
	if (ret != PON_STATUS_OK)
		return ret;

	ret = fapi_pon_generic_get(ctx,
				   PONFW_PLOAM_STATE_CMD_ID,
				   NULL,
				   0,
				   &pon_status_get_copy_gtc,
				   param);
	if (ret != PON_STATUS_OK)
		return ret;

	/* For GPON, skip AUTH_STATUS */
	if (pon_mode_check(ctx, MODE_984_GPON))
		return ret;

	return fapi_pon_generic_get(ctx,
				    PONFW_XGTC_AUTH_STATUS_CMD_ID,
				    NULL,
				    0,
				    &pon_status_get_copy_xgtc_onu,
				    param);
}

/**
 * @brief Get clock cycle (GPON_CLOCK_CYCLE)
 *
 * @param ctx pointer to pon_ctx
 *
 * @return 0 on error, GPON_CLOCK_CYCLE
 */
static int get_clock_cycle(struct pon_ctx *ctx)
{
	struct pon_cap caps = {0};
	enum fapi_pon_errorcode ret;

	ret = fapi_pon_cap_get(ctx, &caps);

	if (ret != PON_STATUS_OK)
		return 0;

	return get_clock_cycle_from_caps(&caps);
}

enum fapi_pon_errorcode fapi_pon_tod_cfg_set(struct pon_ctx *ctx,
					     const struct pon_tod_cfg *param)
{
	struct ponfw_onu_tod_config fw_param = {0};
	int clock_cycle;

	if (!param)
		return PON_STATUS_INPUT_ERR;

	if (!pon_mode_check(ctx, MODE_ITU_PON))
		return PON_STATUS_OPERATION_MODE_ERR;

	clock_cycle = get_clock_cycle(ctx);

	if (!clock_cycle) {
		PON_DEBUG_ERR("Can't get clock cycle");
		return PON_STATUS_ERR;
	}

	/* 1PPS free running mode enable should be set to 0 or 1 */
	if (param->pps_debug != PONFW_ONU_TOD_CONFIG_FREN_DIS &&
	    param->pps_debug != PONFW_ONU_TOD_CONFIG_FREN_EN)
		return PON_STATUS_VALUE_RANGE_ERR;
	ASSIGN_AND_OVERFLOW_CHECK(fw_param.fren, param->pps_debug);

	/* 1PPS signal pulse width should be in range from 0 to 8191 */
	if (param->pps_width > PON_TOD_PPSW_MAX)
		return PON_STATUS_VALUE_RANGE_ERR;
	ASSIGN_AND_OVERFLOW_CHECK(fw_param.ppsw, param->pps_width);

	/* 1PPS signal pulse polarity should be set to 0 or 1 */
	if (param->pps_level != PONFW_ONU_TOD_CONFIG_PPSP_POS &&
	    param->pps_level != PONFW_ONU_TOD_CONFIG_PPSP_NEG)
		return PON_STATUS_VALUE_RANGE_ERR;
	ASSIGN_AND_OVERFLOW_CHECK(fw_param.ppsp, !param->pps_level);

	/* Capture signal polarity should be in range from 0 to 2 */
	if (param->capture_edge == PONFW_ONU_TOD_CONFIG_CTES_RISE ||
	    param->capture_edge == PONFW_ONU_TOD_CONFIG_CTES_FALL)
		ASSIGN_AND_OVERFLOW_CHECK(fw_param.ctes, !param->capture_edge);
	else if (param->capture_edge == PONFW_ONU_TOD_CONFIG_CTES_BOTH)
		ASSIGN_AND_OVERFLOW_CHECK(fw_param.ctes, param->capture_edge);
	else
		return PON_STATUS_VALUE_RANGE_ERR;

	/* Upstream time stamping delay correction should be in range
	 * from 0 to 1023
	 */
	if ((param->us_ts_corr / clock_cycle) > PON_TOD_USC_MAX)
		return PON_STATUS_VALUE_RANGE_ERR;
	ASSIGN_AND_OVERFLOW_CHECK(fw_param.usc,
				  param->us_ts_corr / clock_cycle);

	/* Downstream time stamping delay correction should be in range
	 * from 0 to 1023
	 */
	if ((param->ds_ts_corr / clock_cycle) > PON_TOD_DSC_MAX)
		return PON_STATUS_VALUE_RANGE_ERR;
	ASSIGN_AND_OVERFLOW_CHECK(fw_param.dsc,
				  param->ds_ts_corr / clock_cycle);

	/* ToD output sub-sampling factor should be in range from 0 to 511 */
	if ((param->pps_scale - 1) > PON_TOD_OUTSS_MAX)
		return PON_STATUS_VALUE_RANGE_ERR;
	ASSIGN_AND_OVERFLOW_CHECK(fw_param.outss, param->pps_scale - 1);

	/* ToD output delay correction (in ps) should be in range
	 * from 0 to 1023
	 */
	if ((param->pps_delay / clock_cycle) > PON_TOD_OUTC_MAX)
		return PON_STATUS_VALUE_RANGE_ERR;
	ASSIGN_AND_OVERFLOW_CHECK(fw_param.outc,
				  param->pps_delay / clock_cycle);

	return fapi_pon_generic_set(ctx,
				    PONFW_ONU_TOD_CONFIG_CMD_ID,
				    &fw_param,
				    sizeof(struct ponfw_onu_tod_config));
}

static enum fapi_pon_errorcode pon_tod_cfg_get_copy(struct pon_ctx *ctx,
						    const void *data,
						    size_t data_size,
						    void *priv)
{
	enum fapi_pon_errorcode ret;
	const struct ponfw_onu_tod_config *src_param = data;
	struct pon_tod_cfg *dst_param = priv;
	int clock_cycle;

	if (!ctx)
		return PON_STATUS_INPUT_ERR;

	clock_cycle = get_clock_cycle(ctx);

	if (!clock_cycle) {
		PON_DEBUG_ERR("Can't get clock cycle: can't get capabilities");
		return PON_STATUS_ERR;
	}

	ret = integrity_check(dst_param, sizeof(*src_param), data_size);
	if (ret != PON_STATUS_OK)
		return ret;

	dst_param->pps_debug = src_param->fren;
	dst_param->pps_width = src_param->ppsw;
	dst_param->pps_level = !src_param->ppsp;

	if (src_param->ctes == PONFW_ONU_TOD_CONFIG_CTES_RISE ||
	    src_param->ctes == PONFW_ONU_TOD_CONFIG_CTES_FALL)
		dst_param->capture_edge = !src_param->ctes;
	else
		dst_param->capture_edge = src_param->ctes;

	dst_param->us_ts_corr = src_param->usc * clock_cycle;
	dst_param->ds_ts_corr = src_param->dsc * clock_cycle;
	dst_param->pps_scale = src_param->outss + 1;
	dst_param->pps_delay = src_param->outc * clock_cycle;

	return PON_STATUS_OK;
}

enum fapi_pon_errorcode fapi_pon_tod_cfg_get(struct pon_ctx *ctx,
					     struct pon_tod_cfg *param)
{
	if (!pon_mode_check(ctx, MODE_ITU_PON))
		return PON_STATUS_OPERATION_MODE_ERR;

	return fapi_pon_generic_get(ctx,
				    PONFW_ONU_TOD_CONFIG_CMD_ID,
				    NULL,
				    0,
				    &pon_tod_cfg_get_copy,
				    param);
}

enum fapi_pon_errorcode
fapi_pon_gpon_tod_sync_set(struct pon_ctx *ctx,
			   const struct pon_gpon_tod_sync *param)
{
	enum fapi_pon_errorcode ret;
	struct ponfw_onu_tod_sync fw_param = {0};
	struct pon_cap caps = {0};
	int clock_cycle;

	if (!param)
		return PON_STATUS_INPUT_ERR;

	if (!pon_mode_check(ctx, MODE_ITU_PON))
		return PON_STATUS_OPERATION_MODE_ERR;

	ret = fapi_pon_cap_get(ctx, &caps);
	if (ret != PON_STATUS_OK)
		return ret;

	fw_param.mf_count = param->multiframe_count;
	/* Bit 31 to 30 are unused in G.984.3 mode. */
	if (pon_mode_check(ctx, MODE_984_GPON) &&
	    fw_param.mf_count > 0x3FFFFFFF)
		return PON_STATUS_VALUE_RANGE_ERR;

	clock_cycle = get_clock_cycle_from_caps(&caps);

	fw_param.tod_sec = param->tod_seconds;

	/* Time of Day microseconds part is the 32-bit nanoseconds part
	 * in the range from 0 to 9999 * 100 μs = 999.9 ms.
	 */
	fw_param.tod_micro = param->tod_nano_seconds / 100000;

	/* Time of Day nanoseconds part, given in units of clock cycles.
	 * Each clock cycle for GPON is derived from the 311.04 MHz clock
	 * (3.215 ns per bit).
	 */
	fw_param.tod_clocks = (param->tod_nano_seconds % 100000) /
					((float)clock_cycle / 1000);

	/* The value of tod_quality shall be ignored in the 'set' function
	 * and only be used in the 'get' function to report the value provided
	 * by the FW message ONU_TOD_SYNC.
	 */

	return fapi_pon_generic_set(ctx,
				    PONFW_ONU_TOD_SYNC_CMD_ID,
				    &fw_param,
				    sizeof(struct ponfw_onu_tod_sync));
}

static enum fapi_pon_errorcode
fapi_pon_gpon_tod_sync_get_copy(struct pon_ctx *ctx,
				const void *data,
				size_t data_size,
				void *priv)
{
	enum fapi_pon_errorcode ret;
	const struct ponfw_onu_tod_sync *src_param = data;
	struct pon_gpon_tod_sync *dst_param = priv;
	struct pon_cap caps = {0};
	int clock_cycle;

	ret = fapi_pon_cap_get(ctx, &caps);
	if (ret != PON_STATUS_OK)
		return ret;

	ret = integrity_check(dst_param, sizeof(*src_param), data_size);
	if (ret != PON_STATUS_OK)
		return ret;

	clock_cycle = get_clock_cycle_from_caps(&caps);

	dst_param->multiframe_count = src_param->mf_count;

	/* Bit 31 and 30 shall be ignored while reading in G.989.3 mode,
	 * as the multiframe counter is only 30 bit wide.
	 */
	if (caps_features_check_if_all(ctx, PON_FEATURE_G989))
		dst_param->multiframe_count &= 0x3FFFFFFF;

	dst_param->tod_seconds = src_param->tod_sec;

	/* The extended seconds are not handled in this message, as
	 * these are not covered by the hardware and will have no
	 * effect until the year 2106.
	 */
	dst_param->tod_extended_seconds = 0;

	/* Param tod_nano_seconds consists of tod_nano and tod_micro FW params
	 * explained in fapi_pon_gpon_tod_sync_set function above.
	 */
	dst_param->tod_nano_seconds = (src_param->tod_micro * 100000) +
		     (src_param->tod_clocks * ((float)clock_cycle / 1000));

	dst_param->tod_offset_pico_seconds = 0;
	dst_param->tod_quality = src_param->tod_quality;

	return PON_STATUS_OK;
}

enum fapi_pon_errorcode
fapi_pon_gpon_tod_sync_get(struct pon_ctx *ctx,
			   struct pon_gpon_tod_sync *param)
{
	if (!pon_mode_check(ctx, MODE_ITU_PON))
		return PON_STATUS_OPERATION_MODE_ERR;

	return fapi_pon_generic_get(ctx,
				    PONFW_ONU_TOD_SYNC_CMD_ID,
				    NULL,
				    0,
				    &fapi_pon_gpon_tod_sync_get_copy,
				    param);
}

static enum fapi_pon_errorcode pon_tod_get_copy(struct pon_ctx *ctx,
						const void *data,
						size_t data_size,
						void *priv)
{
	enum fapi_pon_errorcode ret;
	const struct ponfw_onu_tod_sync *src_param = data;
	struct pon_tod *dst_param = priv;
	struct tm tm;
	time_t seconds;

	UNUSED(ctx);

	ret = integrity_check(dst_param, sizeof(*src_param), data_size);
	if (ret != PON_STATUS_OK)
		return ret;

	seconds = src_param->tod_sec;

	if (!gmtime_r(&seconds, &tm))
		return PON_STATUS_ERR;

	dst_param->sec = tm.tm_sec;
	dst_param->min = tm.tm_min;
	dst_param->hour = tm.tm_hour;
	dst_param->mday = tm.tm_mday;
	dst_param->mon = 1 + tm.tm_mon;
	dst_param->year = 1900 + tm.tm_year;
	dst_param->wday = tm.tm_wday;
	dst_param->yday = tm.tm_yday;
	dst_param->sec_tai = src_param->tod_sec;
	dst_param->tod_quality = src_param->tod_quality;

	return PON_STATUS_OK;
}

enum fapi_pon_errorcode fapi_pon_tod_get(struct pon_ctx *ctx,
					 struct pon_tod *param)
{
	if (!pon_mode_check(ctx, MODE_ITU_PON))
		return PON_STATUS_OPERATION_MODE_ERR;

	return fapi_pon_generic_get(ctx,
				    PONFW_ONU_TOD_SYNC_CMD_ID,
				    NULL,
				    0,
				    &pon_tod_get_copy,
				    param);
}

enum fapi_pon_errorcode fapi_pon_auth_onu_bc_key_set(struct pon_ctx *ctx,
	const struct pon_onu_bc_key *param)
{
	struct ponfw_xgtc_broadcast_key_table fw_param = {0};

	if (!param || param->size > MAX_AUTH_TABLE_SIZE)
		return PON_STATUS_INPUT_ERR;

	if (!pon_mode_check(ctx, MODE_ITU_PON))
		return PON_STATUS_OPERATION_MODE_ERR;

	/* refer to struct pon_onu_bc_key
	 * 1: This is the first broadcast key definition
	 * 2: This is the second broadcast key definition
	 */
	if (param->index < 1 || param->index > 2)
		return PON_STATUS_VALUE_RANGE_ERR;

	ASSIGN_AND_OVERFLOW_CHECK(fw_param.idx, param->index >> 1);
	pon_byte_copy(fw_param.oltbkt, param->table, param->size);
	/* TODO: table size assignment (fw_param.size) */

	return fapi_pon_generic_set(ctx,
				    PONFW_XGTC_BROADCAST_KEY_TABLE_CMD_ID,
				    &fw_param,
				    sizeof(fw_param));
}

static enum fapi_pon_errorcode
pon_auth_onu_msk_hash_get_copy(struct pon_ctx *ctx,
			       const void *data,
			       size_t data_size,
			       void *priv)
{
	enum fapi_pon_errorcode ret;
	const struct ponfw_xgtc_msk_hash *src_param = data;
	struct pon_onu_msk_hash *dst_param = priv;

	UNUSED(ctx);

	/* TODO: check functionality with new firmware (auth adaptation) */
	ret = integrity_check(dst_param, sizeof(*src_param), data_size);
	if (ret != PON_STATUS_OK)
		return ret;

	/* FIXME: convert to memcpy_s after fw definition adaptation */

	dst_param->type = src_param->type;
	dst_param->hash[0] = src_param->msk_hash1 & 0xFF;
	dst_param->hash[1] = (src_param->msk_hash1 & 0xFF00) >> 8;
	dst_param->hash[2] = (src_param->msk_hash1 & 0xFF0000) >> 16;
	dst_param->hash[3] = (src_param->msk_hash1 & 0xFF000000) >> 24;
	dst_param->hash[4] = src_param->msk_hash2 & 0xFF;
	dst_param->hash[5] = (src_param->msk_hash2 & 0xFF00) >> 8;
	dst_param->hash[6] = (src_param->msk_hash2 & 0xFF0000) >> 16;
	dst_param->hash[7] = (src_param->msk_hash2 & 0xFF000000) >> 24;
	dst_param->hash[8] = src_param->msk_hash3 & 0xFF;
	dst_param->hash[9] = (src_param->msk_hash3 & 0xFF00) >> 8;
	dst_param->hash[10] = (src_param->msk_hash3 & 0xFF0000) >> 16;
	dst_param->hash[11] = (src_param->msk_hash3 & 0xFF000000) >> 24;
	dst_param->hash[12] = src_param->msk_hash4 & 0xFF;
	dst_param->hash[13] = (src_param->msk_hash4 & 0xFF00) >> 8;
	dst_param->hash[14] = (src_param->msk_hash4 & 0xFF0000) >> 16;
	dst_param->hash[15] = (src_param->msk_hash4 & 0xFF000000) >> 24;

	return PON_STATUS_OK;
}

enum fapi_pon_errorcode fapi_pon_auth_onu_msk_hash_get(struct pon_ctx *ctx,
	struct pon_onu_msk_hash *param)
{
	struct ponfw_xgtc_msk_hash fw_param = {0};

	if (!pon_mode_check(ctx, MODE_ITU_PON))
		return PON_STATUS_OPERATION_MODE_ERR;

	fw_param.type = PONFW_XGTC_MSK_HASH_TYPE_AES_CMAC128;

	return fapi_pon_generic_get(ctx,
				    PONFW_XGTC_MSK_HASH_CMD_ID,
				    &fw_param,
				    PONFW_XGTC_MSK_HASH_LENR,
				    &pon_auth_onu_msk_hash_get_copy,
				    param);
}

enum fapi_pon_errorcode fapi_pon_auth_olt_result_set(struct pon_ctx *ctx,
	const struct pon_generic_auth_table *param)
{
	struct ponfw_xgtc_olt_auth_result_table fw_param = {0};

	if (!param || param->size > MAX_AUTH_TABLE_SIZE)
		return PON_STATUS_INPUT_ERR;

	if (!pon_mode_check(ctx, MODE_ITU_PON))
		return PON_STATUS_OPERATION_MODE_ERR;

	pon_byte_copy(fw_param.oltart, param->table, param->size);
	/* TODO: table size assignment (fw_param.size) */

	return fapi_pon_generic_set(ctx,
				    PONFW_XGTC_OLT_AUTH_RESULT_TABLE_CMD_ID,
				    &fw_param,
				    sizeof(fw_param));
}

enum fapi_pon_errorcode fapi_pon_auth_olt_challenge_set(struct pon_ctx *ctx,
	const struct pon_generic_auth_table *param)
{
	struct ponfw_xgtc_olt_rnd_chal_table fw_param = {0};

	if (!param || param->size > MAX_AUTH_TABLE_SIZE)
		return PON_STATUS_INPUT_ERR;

	if (!pon_mode_check(ctx, MODE_ITU_PON))
		return PON_STATUS_OPERATION_MODE_ERR;

	pon_byte_copy(fw_param.oltrct, param->table, param->size);
	/* TODO: table size assignment (fw_param.size) */

	return fapi_pon_generic_set(ctx,
				    PONFW_XGTC_OLT_RND_CHAL_TABLE_CMD_ID,
				    &fw_param,
				    sizeof(fw_param));
}

enum fapi_pon_errorcode fapi_pon_auth_enc_cfg_set(struct pon_ctx *ctx,
				const struct pon_enc_cfg *param)
{
	struct ponfw_xgtc_enc_config fw_param = {0};

	if (!param)
		return PON_STATUS_INPUT_ERR;

	if (!pon_mode_check(ctx, MODE_ITU_PON))
		return PON_STATUS_OPERATION_MODE_ERR;

	/* We support only AES-CMAC-128 */
	if (param->enc_mode != 1)
		return PON_STATUS_INPUT_ERR;
	ASSIGN_AND_OVERFLOW_CHECK(fw_param.encmode, param->enc_mode);

	/* We support only 128 bit */
	if (param->key_size != 128)
		return PON_STATUS_INPUT_ERR;
	ASSIGN_AND_OVERFLOW_CHECK(fw_param.keysize, param->key_size);

	ASSIGN_AND_OVERFLOW_CHECK(fw_param.psk3, GLUE_U32(param->psk[0],
		param->psk[1], param->psk[2], param->psk[3]));
	ASSIGN_AND_OVERFLOW_CHECK(fw_param.psk2, GLUE_U32(param->psk[4],
		param->psk[5], param->psk[6], param->psk[7]));
	ASSIGN_AND_OVERFLOW_CHECK(fw_param.psk1, GLUE_U32(param->psk[8],
		param->psk[9], param->psk[10], param->psk[11]));
	ASSIGN_AND_OVERFLOW_CHECK(fw_param.psk0, GLUE_U32(param->psk[12],
		param->psk[13], param->psk[14], param->psk[15]));

	return fapi_pon_generic_set(ctx,
				    PONFW_XGTC_ENC_CONFIG_CMD_ID,
				    &fw_param,
				    sizeof(fw_param));
}

static enum fapi_pon_errorcode
pon_auth_enc_cfg_get_copy(struct pon_ctx *ctx,
			  const void *data,
			  size_t data_size,
			  void *priv)
{
	enum fapi_pon_errorcode ret;
	const struct ponfw_xgtc_enc_config *src_param = data;
	struct pon_enc_cfg *dst_param = priv;

	UNUSED(ctx);

	ret = integrity_check(dst_param, sizeof(*src_param), data_size);
	if (ret != PON_STATUS_OK)
		return ret;

	dst_param->enc_mode = src_param->encmode;
	dst_param->key_size = src_param->keysize;

	dst_param->psk[0] = src_param->psk0 & 0xFF;
	dst_param->psk[1] = (src_param->psk0 & 0xFF00) >> 8;
	dst_param->psk[2] = (src_param->psk0 & 0xFF0000) >> 16;
	dst_param->psk[3] = (src_param->psk0 & 0xFF000000) >> 24;
	dst_param->psk[4] = src_param->psk1 & 0xFF;
	dst_param->psk[5] = (src_param->psk1 & 0xFF00) >> 8;
	dst_param->psk[6] = (src_param->psk1 & 0xFF0000) >> 16;
	dst_param->psk[7] = (src_param->psk1 & 0xFF000000) >> 24;
	dst_param->psk[8] = src_param->psk2 & 0xFF;
	dst_param->psk[9] = (src_param->psk2 & 0xFF00) >> 8;
	dst_param->psk[10] = (src_param->psk2 & 0xFF0000) >> 16;
	dst_param->psk[11] = (src_param->psk2 & 0xFF000000) >> 24;
	dst_param->psk[12] = src_param->psk3 & 0xFF;
	dst_param->psk[13] = (src_param->psk3 & 0xFF00) >> 8;
	dst_param->psk[14] = (src_param->psk3 & 0xFF0000) >> 16;
	dst_param->psk[15] = (src_param->psk3 & 0xFF000000) >> 24;

	return PON_STATUS_OK;
}

enum fapi_pon_errorcode fapi_pon_auth_enc_cfg_get(struct pon_ctx *ctx,
	struct pon_enc_cfg *param)
{
	if (!pon_mode_check(ctx, MODE_ITU_PON))
		return PON_STATUS_OPERATION_MODE_ERR;

	return fapi_pon_generic_get(ctx,
				    PONFW_XGTC_ENC_CONFIG_CMD_ID,
				    NULL,
				    0,
				    &pon_auth_enc_cfg_get_copy,
				    param);
}

/* GEM ports information.
 * Used by fapi_pon_alloc_gem_port_get.
 */
struct gpid_info {
	/* Size of buffer used to read GEM ports */
	uint32_t *gem_ports_num;
	/* Buffer used to read GEM ports */
	uint32_t *gem_ports;
};

static enum fapi_pon_errorcode pon_alloc_gem_port_get_copy(struct pon_ctx *ctx,
							   const void *data,
							   size_t data_size,
							   void *priv)
{
	const union ponfw_msg *fw = data;
	struct gpid_info *gpid_info = priv;
	/* Number of existing GEM ports received from the FW.
	 * It is calculated by checking FW message size in bytes, subtraction
	 * data included in I1 section (4 bytes, information about allocation
	 * ID) and division by 4 to get number of existing GEM ports.
	 */
	uint32_t num_of_gem_ports = (data_size - 4) / 4;
	/* Size of buffer used to read GEM ports */
	size_t size = *gpid_info->gem_ports_num;
	unsigned int i;

	UNUSED(ctx);

	if (size != 0) {
		for (i = 0; i < size && i < num_of_gem_ports; i++)
#if __BYTE_ORDER == __BIG_ENDIAN
			gpid_info->gem_ports[i] = fw->val[i + 1] & 0xFFFF;
#else
			gpid_info->gem_ports[i]
			       = (fw->val[i + 1] & 0xFFFF0000) >> 16;
#endif

		if (size >= num_of_gem_ports) {
			*gpid_info->gem_ports_num = num_of_gem_ports;
			return PON_STATUS_OK;
		}

		return PON_STATUS_MEM_NOT_ENOUGH;
	}

	return PON_STATUS_INPUT_ERR;
}

enum fapi_pon_errorcode
fapi_pon_alloc_gem_port_get(struct pon_ctx *ctx,
			    uint16_t alloc_id,
			    uint32_t *gem_ports_num,
			    uint32_t *gem_ports)
{
	struct ponfw_alloc_to_gem_map fw_param = {0};
	struct pon_range_limits limits = {0};
	struct gpid_info gpid_info = {0};
	struct pon_allocation_index alloc_idx = {0};
	enum fapi_pon_errorcode ret;

	if (!gem_ports_num || !gem_ports)
		return PON_STATUS_INPUT_ERR;

	if (!pon_mode_check(ctx, MODE_ITU_PON))
		return PON_STATUS_OPERATION_MODE_ERR;

	ret = fapi_pon_limits_get(ctx, &limits);
	if (ret != PON_STATUS_OK)
		return ret;

	if (alloc_id > limits.alloc_id_max)
		return PON_STATUS_VALUE_RANGE_ERR;

	ret = fapi_pon_alloc_id_get(ctx, alloc_id, &alloc_idx);
	if (ret != PON_STATUS_OK) {
		*gem_ports_num = 0;
		return ret;
	}

	ASSIGN_AND_OVERFLOW_CHECK(fw_param.alloc_id, alloc_id);
	ASSIGN_AND_OVERFLOW_CHECK(fw_param.alloc_link_ref,
				  alloc_idx.alloc_link_ref);
	gpid_info.gem_ports_num = gem_ports_num;
	gpid_info.gem_ports = gem_ports;

	ret = fapi_pon_generic_get(ctx,
				   PONFW_ALLOC_TO_GEM_MAP_CMD_ID,
				   &fw_param,
				   PONFW_ALLOC_TO_GEM_MAP_LENR,
				   &pon_alloc_gem_port_get_copy,
				   &gpid_info);
	if (ret == PON_STATUS_FW_NACK) {
		*gem_ports_num = 0;
		return PON_STATUS_ALLOC_GEM_MAP_ERR;
	}

	return ret;
}

static enum fapi_pon_errorcode pon_gem_port_alloc_get_copy(struct pon_ctx *ctx,
							   const void *data,
							   size_t data_size,
							   void *priv)
{
	enum fapi_pon_errorcode ret;
	const struct ponfw_gem_alloc_map *src_param = data;
	struct pon_allocation_id *dst_param = priv;

	UNUSED(ctx);

	ret = integrity_check(dst_param, sizeof(*src_param), data_size);
	if (ret != PON_STATUS_OK)
		return ret;

	dst_param->alloc_id = (uint16_t)src_param->alloc_id;
	dst_param->alloc_link_ref = src_param->alloc_link_ref;

	return PON_STATUS_OK;
}

enum fapi_pon_errorcode
fapi_pon_gem_port_alloc_get(struct pon_ctx *ctx,
			    uint16_t gem_port_id,
			    struct pon_allocation_id *param)
{
	struct ponfw_gem_alloc_map fw_param = {0};
	struct pon_range_limits limits = {0};
	enum fapi_pon_errorcode ret;

	if (!pon_mode_check(ctx, MODE_ITU_PON))
		return PON_STATUS_OPERATION_MODE_ERR;

	ret = fapi_pon_limits_get(ctx, &limits);
	if (ret != PON_STATUS_OK)
		return ret;

	if (gem_port_id > limits.gem_port_id_max)
		return PON_STATUS_VALUE_RANGE_ERR;

	ASSIGN_AND_OVERFLOW_CHECK(fw_param.gem_port_id, gem_port_id);

	ret = fapi_pon_generic_get(ctx,
				   PONFW_GEM_ALLOC_MAP_CMD_ID,
				   &fw_param,
				   PONFW_GEM_ALLOC_MAP_LENR,
				   &pon_gem_port_alloc_get_copy,
				   param);
	if (ret == PON_STATUS_FW_NACK)
		return PON_STATUS_GEM_PORT_ID_NOT_EXISTS_ERR;

	return ret;
}

/** Function to get configuration value per threshold for XGS-PON mode */
static enum fapi_pon_errorcode threshold_config_value_xgspon_get(uint8_t value,
							  uint32_t *threshold,
							  uint8_t mode)
{
	UNUSED(mode);

	if (value < SF_THRESHOLD_MIN_VALUE || value > SD_THRESHOLD_MAX_VALUE)
		return PON_STATUS_VALUE_RANGE_ERR;

	/* The lowest proper value provided to this function corresponds to
	 * minimal value of signal fail threshold (3). To get value from an
	 * array where indexes start from 0, the value 3 should be subtracted
	 * from value provided as a function argument.
	 */
	*threshold = threshold_cfg_10g[value - SF_THRESHOLD_MIN_VALUE];

	return PON_STATUS_OK;
}

/** Function to get configuration value per threshold for GPON mode */
static enum fapi_pon_errorcode
threshold_config_value_gpon_get(uint8_t value,
				uint32_t *threshold,
				uint32_t bip_error_intvl,
				uint8_t mode)
{
	if (value < SF_THRESHOLD_MIN_VALUE || value > SD_THRESHOLD_MAX_VALUE)
		return PON_STATUS_VALUE_RANGE_ERR;

	/* Calculate threshold value only if in GPON mode. */
	if (mode != PON_MODE_984_GPON)
		return PON_STATUS_INPUT_ERR;

	*threshold = (double)bip_error_intvl *
		     GPON_BITS_PER_125US * pow(10.0, -value);

	return PON_STATUS_OK;
}

/** Function to get bip error interval value */
static enum fapi_pon_errorcode bip_err_intvl_get(struct pon_ctx *ctx,
						 const void *data,
						 size_t data_size,
						 void *priv)
{
	const struct ponfw_bip_err_config *src_param = data;
	uint32_t *bip_error_intvl = priv;

	UNUSED(ctx);
	UNUSED(data_size);

	*bip_error_intvl = src_param->bip_err_intvl;

	return PON_STATUS_OK;
}

/** Configure the BIP error thresholds which are needed to trigger the Signal
 *  Fail (SF) and Signal Degrade (SD) alarms. The alarms are detected by the
 *  PON IP firmware based on this configuration.
 */
enum fapi_pon_errorcode fapi_pon_gtc_cfg_set(struct pon_ctx *ctx,
					     const struct pon_gtc_cfg *param)
{
	struct ponfw_bip_err_config fw_param = {0};
	enum fapi_pon_errorcode ret;
	uint32_t bip_error_intvl = 0;
	uint32_t sigfail_th = 0;
	uint32_t sigdeg_th = 0;
	uint8_t pon_mode = 0;

	if (!param)
		return PON_STATUS_INPUT_ERR;

	/* Check the PON operation mode because the handling differs between
	 * GPON mode and XG(S)-PON/NG-PON2 operation modes.
	 */
	ret = fapi_pon_mode_get(ctx, &pon_mode);
	if (ret != PON_STATUS_OK)
		return ret;
	if (pon_mode != PON_MODE_984_GPON &&
		pon_mode != PON_MODE_987_XGPON &&
		pon_mode != PON_MODE_9807_XGSPON &&
		pon_mode != PON_MODE_989_NGPON2_10G &&
		pon_mode != PON_MODE_989_NGPON2_2G5)
		return PON_STATUS_OPERATION_MODE_ERR;

	/* The "signal fail" bit error threshold is calculated from the input
	 * value range (3 to 9) depending on the selected counting interval time
	 * and the downstream data rate (2.5 Gbit/s for GPON, 10 Gbit/s else).
	 */
	if (param->sf_threshold < SF_THRESHOLD_MIN_VALUE ||
	    param->sf_threshold > SF_THRESHOLD_MAX_VALUE)
		return PON_STATUS_VALUE_RANGE_ERR;

	/* The "signal degrade" bit error threshold is calculated from the input
	 * value range (4 to 10) depending on the selected counting interval
	 * time and the downstream data rate (2.5 Gbit/s for GPON, 10 Gbit/s
	 * else).
	 */
	if (param->sd_threshold < SD_THRESHOLD_MIN_VALUE ||
	    param->sd_threshold > SD_THRESHOLD_MAX_VALUE)
		return PON_STATUS_VALUE_RANGE_ERR;

	/* GPON operation mode:
	 * The bit error counting interval is configured by the OLT
	 * through a PLOAM message. The value needs to be read back from the
	 * PON IP firmware and the alarm thresholds need to be calculated based
	 * on this remotely configured value.
	 */
	if (pon_mode == PON_MODE_984_GPON) {
		ret = fapi_pon_generic_get(ctx,
					   PONFW_BIP_ERR_CONFIG_CMD_ID,
					   NULL,
					   0,
					   &bip_err_intvl_get,
					   &bip_error_intvl);
		if (ret != PON_STATUS_OK)
			return ret;

		/* This is the BIP error interval as configured by the OLT. */
		fw_param.bip_err_intvl = bip_error_intvl;

		/* Calculate the threshold for the SF alarm. */
		ret = threshold_config_value_gpon_get(param->sf_threshold,
						 &sigfail_th,
						 bip_error_intvl,
						 pon_mode);
		if (ret != PON_STATUS_OK)
			return ret;

		/* Calculate the threshold for the SD alarm. */
		ret = threshold_config_value_gpon_get(param->sd_threshold,
						 &sigdeg_th,
						 bip_error_intvl,
						 pon_mode);
		if (ret != PON_STATUS_OK)
			return ret;
	} else {
		/* XG(S)-PON or NG-PON2 operation mode:
		 * The bit error counting interval is configured locally by the
		 * software. The value BER_COUNT_TIME shall be selected to
		 * provide a compromise between quick reaction time
		 * (with a short interval) and accuracy (which needs a longer
		 * measurement time and delays the alarm).
		 *
		 * The bit error counting interval is configured
		 * in multiples of 125 us while BER_COUNT_TIME is defined in
		 * units of seconds.
		 */
		fw_param.bip_err_intvl = BER_COUNT_TIME * 8;

		/* Calculate the threshold for the SF alarm. */
		ret = threshold_config_value_xgspon_get(param->sf_threshold,
						 &sigfail_th,
						 pon_mode);
		if (ret != PON_STATUS_OK)
			return ret;

		/* Calculate the threshold for the SD alarm. */
		ret = threshold_config_value_xgspon_get(param->sd_threshold,
						 &sigdeg_th,
						 pon_mode);
		if (ret != PON_STATUS_OK)
			return ret;
	}
	fw_param.sigfail_th = sigfail_th;
	fw_param.sigdeg_th = sigdeg_th;

	/* Write the configuration values to the PON IP by using a dedicated
	 * firmware message.
	 */
	return fapi_pon_generic_set(ctx,
				    PONFW_BIP_ERR_CONFIG_CMD_ID,
				    &fw_param,
				    sizeof(struct ponfw_bip_err_config));
}

static enum fapi_pon_errorcode pon_gtc_cfg_get_copy(struct pon_ctx *ctx,
						    const void *data,
						    size_t data_size,
						    void *priv)
{
	enum fapi_pon_errorcode ret;
	const struct ponfw_bip_err_config *src_param = data;
	struct pon_gtc_cfg *dst_param = priv;
	unsigned int i;
	uint32_t bip_error_intvl;

	if (!ctx)
		return PON_STATUS_INPUT_ERR;

	ret = integrity_check(dst_param, sizeof(*src_param), data_size);
	if (ret != PON_STATUS_OK)
		return ret;

	/* Calculate power of bit error rate based on current threshold values.
	 * The equation depends on pon_mode.
	 */
	if (pon_mode_check(ctx, MODE_987_XGPON |
				MODE_9807_XGSPON |
				MODE_989_NGPON2_2G5 |
				MODE_989_NGPON2_10G)) {
		/* Index of the element in cfg_pon array is 3 lower than
		 * value of power stored in param->sd/sf_threshold
		 */
		for (i = 0; i < ARRAY_SIZE(threshold_cfg_10g); ++i) {
			if (threshold_cfg_10g[i] == src_param->sigdeg_th)
				dst_param->sd_threshold = i + 3;

			if (threshold_cfg_10g[i] == src_param->sigfail_th)
				dst_param->sf_threshold = i + 3;
		}
	} else if (pon_mode_check(ctx, MODE_984_GPON)) {
		/* BIP error interval is necessary in calculations. */
		ret = fapi_pon_generic_get(ctx,
				PONFW_BIP_ERR_CONFIG_CMD_ID,
				NULL,
				0,
				&bip_err_intvl_get,
				&bip_error_intvl);
		if (ret != PON_STATUS_OK)
			return ret;

		dst_param->sd_threshold = -log10(src_param->sigdeg_th / (
						(double)bip_error_intvl *
						GPON_BITS_PER_125US));
		dst_param->sf_threshold = -log10(src_param->sigfail_th / (
						(double)bip_error_intvl *
						GPON_BITS_PER_125US));
	}

	return PON_STATUS_OK;
}

/** Read the BIP error thresholds which are needed to trigger the Signal
 *  Fail (SF) and Signal Degrade (SD) alarms. The alarms are detected by the
 *  PON IP hardware based on this configuration.
 */
enum fapi_pon_errorcode fapi_pon_gtc_cfg_get(struct pon_ctx *ctx,
					     struct pon_gtc_cfg *param)
{
	if (!pon_mode_check(ctx, MODE_ITU_PON))
		return PON_STATUS_OPERATION_MODE_ERR;

	/* Read the configuration values from the PON IP by using a dedicated
	 * firmware message.
	 */
	return fapi_pon_generic_get(ctx,
				    PONFW_BIP_ERR_CONFIG_CMD_ID,
				    NULL,
				    0,
				    &pon_gtc_cfg_get_copy,
				    param);
}

static enum fapi_pon_errorcode
pon_twdm_ploam_us_counters_get_decode(struct pon_ctx *ctx,
				      struct nlattr **attrs,
				      void *priv)
{
	struct pon_twdm_ploam_us_counters *dst_param = priv;
	struct nlattr *cnt[PON_MBOX_A_CNT_TC_PLOAM_US_MAX + 1];

	memset(dst_param, 0, sizeof(*dst_param));

	UNUSED(ctx);

	if (!attrs[PON_MBOX_A_CNT])
		return PON_STATUS_ERR;

	if (nla_parse_nested(cnt, PON_MBOX_A_CNT_TC_PLOAM_US_MAX,
			     attrs[PON_MBOX_A_CNT],
			     pon_mbox_cnt_tc_ploam_us_policy) < 0)
		return PON_STATUS_ERR;

	#define ASSIGN(src, dst) \
		do { \
			CNT_DECODE_U64(src, dst);; \
			dst_param->all += dst_param->dst; \
		} while (0)
	ASSIGN(TC_PLOAM_US_SER_ONU, ser_no);
	ASSIGN(TC_PLOAM_US_REG, reg);
	ASSIGN(TC_PLOAM_US_KEY_REP, key_rep);
	ASSIGN(TC_PLOAM_US_ACK, ack);
	ASSIGN(TC_PLOAM_US_SLP_REQ, sleep_req);
	ASSIGN(TC_PLOAM_US_TUN_RES_AN, tuning_resp_ack_nack);
	ASSIGN(TC_PLOAM_US_TUN_RES_CRB, tuning_resp_complete_rollback);
	ASSIGN(TC_PLOAM_US_PW_CONS, power_rep);
	ASSIGN(TC_PLOAM_US_CPL_ERR, cpl_err);
	#undef ASSIGN

	return PON_STATUS_OK;
}

static enum fapi_pon_errorcode
pon_ploam_us_counters_get_decode(struct pon_ctx *ctx,
				 struct nlattr **attrs,
				 void *priv)
{
	struct pon_ploam_us_counters *dst_param = priv;
	struct nlattr *cnt[PON_MBOX_A_CNT_TC_PLOAM_US_MAX + 1];

	memset(dst_param, 0, sizeof(*dst_param));

	UNUSED(ctx);

	if (!attrs[PON_MBOX_A_CNT])
		return PON_STATUS_ERR;

	if (nla_parse_nested(cnt, PON_MBOX_A_CNT_TC_PLOAM_US_MAX,
			     attrs[PON_MBOX_A_CNT],
			     pon_mbox_cnt_tc_ploam_us_policy) < 0)
		return PON_STATUS_ERR;

	#define ASSIGN(src, dst) \
		do { \
			CNT_DECODE_U64(src, dst);; \
			dst_param->all += dst_param->dst; \
		} while (0)
	ASSIGN(TC_PLOAM_US_SER_ONU, ser_no);
	ASSIGN(TC_PLOAM_US_PASSWORD, passwd);
	ASSIGN(TC_PLOAM_US_DYG_GASP, dying_gasp);
	ASSIGN(TC_PLOAM_US_NO_MSG, no_message);
	ASSIGN(TC_PLOAM_US_ENC_KEY, enc_key);
	ASSIGN(TC_PLOAM_US_PHY_EE, pee);
	ASSIGN(TC_PLOAM_US_PST_MSG, pst);
	ASSIGN(TC_PLOAM_US_REM_ERR, rei);
	ASSIGN(TC_PLOAM_US_ACK, ack);
	ASSIGN(TC_PLOAM_US_SLP_REQ, sleep_req);
	ASSIGN(TC_PLOAM_US_REG, reg);
	ASSIGN(TC_PLOAM_US_KEY_REP, key_rep);
	ASSIGN(TC_PLOAM_US_TUN_RES, tuning_resp);
	ASSIGN(TC_PLOAM_US_PW_CONS, power_rep);
	ASSIGN(TC_PLOAM_US_RATE_RESP, rate_resp);
	#undef ASSIGN

	return PON_STATUS_OK;
}

static enum fapi_pon_errorcode
pon_tc_ploam_us_counters_get(struct pon_ctx *ctx,
			     const uint8_t dswlch_id,
			     fapi_pon_decode decode,
			     void *param)
{
	struct read_cmd_cb cb_data;
	struct nl_msg *msg;
	enum fapi_pon_errorcode ret;
	uint32_t seq = NL_AUTO_SEQ;

	if (!pon_mode_check(ctx, MODE_ITU_PON))
		return PON_STATUS_OPERATION_MODE_ERR;

	/* send netlink message for PONFW_(X)GTC_PLOAM_US_COUNTERS_CMD_ID */
	/* fill the nl message and get the TC_PLOAM_US counters */
	ret = fapi_pon_nl_msg_prepare_decode(ctx, &msg, &cb_data, &seq,
					     decode,
					     NULL, param,
					     PON_MBOX_C_TC_PLOAM_US_COUNTERS);
	if (ret != PON_STATUS_OK)
		return ret;

	ret = nla_put_u8(msg, PON_MBOX_D_DSWLCH_ID, dswlch_id);
	if (ret) {
		PON_DEBUG_ERR("Can't add netlink attribute");
		nlmsg_free(msg);
		return PON_STATUS_NL_ERR;
	}

	ret = fapi_pon_nl_msg_send(ctx, &msg, &cb_data, &seq);

	return ret;
}

enum fapi_pon_errorcode
fapi_pon_ploam_us_counters_get(struct pon_ctx *ctx,
			       struct pon_ploam_us_counters *param)
{
	enum fapi_pon_errorcode ret;

	if (!pon_mode_check(ctx, MODE_ITU_PON))
		return PON_STATUS_OPERATION_MODE_ERR;

	/* GPON-XG-PON/XGS-PON/NG-PON2 mode */
	ret = pon_tc_ploam_us_counters_get(ctx, PON_MBOX_D_DSWLCH_ID_CURR,
			&pon_ploam_us_counters_get_decode, param);

	return ret;

}

static enum fapi_pon_errorcode
pon_ploam_ds_counters_get_decode(struct pon_ctx *ctx,
				 struct nlattr **attrs,
				 void *priv)
{
	struct pon_ploam_ds_counters *dst_param = priv;
	struct nlattr *cnt[PON_MBOX_A_CNT_TC_PLOAM_DS_MAX + 1];

	memset(dst_param, 0, sizeof(*dst_param));

	UNUSED(ctx);

	if (!attrs[PON_MBOX_A_CNT])
		return PON_STATUS_ERR;

	if (nla_parse_nested(cnt, PON_MBOX_A_CNT_TC_PLOAM_DS_MAX,
			     attrs[PON_MBOX_A_CNT],
			     pon_mbox_cnt_tc_ploam_ds_policy) < 0)
		return PON_STATUS_ERR;

	#define ASSIGN(src, dst) \
		do { \
			CNT_DECODE_U64(src, dst);; \
			dst_param->all += dst_param->dst; \
		} while (0)

	ASSIGN(TC_PLOAM_DS_BST_PROFILE, burst_profile);
	ASSIGN(TC_PLOAM_DS_ASS_ONU, assign_onu_id);
	ASSIGN(TC_PLOAM_DS_RNG_TIME, ranging_time);
	ASSIGN(TC_PLOAM_DS_DEACT_ONU, deact_onu);
	ASSIGN(TC_PLOAM_DS_DIS_SER, disable_ser_no);
	ASSIGN(TC_PLOAM_DS_REQ_REG, req_reg);
	ASSIGN(TC_PLOAM_DS_ASS_ALLOC, assign_alloc_id);
	ASSIGN(TC_PLOAM_DS_KEY_CTRL, key_control);
	ASSIGN(TC_PLOAM_DS_SLP_ALLOW, sleep_allow);
	ASSIGN(TC_PLOAM_DS_CALIB_REQ, cal_req);
	ASSIGN(TC_PLOAM_DS_ADJ_TX_WL, tx_wavelength);
	ASSIGN(TC_PLOAM_DS_TUNE_CTRL, tune_ctrl);
	ASSIGN(TC_PLOAM_DS_SYS_PROFILE, system_profile);
	ASSIGN(TC_PLOAM_DS_CH_PROFILE, channel_profile);
	ASSIGN(TC_PLOAM_DS_PROT_CONTROL, protection);
	ASSIGN(TC_PLOAM_DS_CHG_PW_LVL, cpl);
	ASSIGN(TC_PLOAM_DS_PW_CONS, power);
	ASSIGN(TC_PLOAM_DS_RATE_CTRL, rate);
	ASSIGN(TC_PLOAM_DS_REBOOT_ONU, reset);
	ASSIGN(TC_PLOAM_DS_UNKNOWN, unknown);
	ASSIGN(TC_PLOAM_DS_ADJ_TX_WL_FAIL, tx_wavelength_err);
	ASSIGN(TC_PLOAM_DS_TUNE_REQ, tuning_request);
	ASSIGN(TC_PLOAM_DS_TUNE_COMPL, tuning_complete);

	ASSIGN(TC_PLOAM_DS_US_OVERHEAD, us_overhead);
	ASSIGN(TC_PLOAM_DS_ENC_PORT_ID, enc_port_id);
	ASSIGN(TC_PLOAM_DS_REQ_PW, req_passwd);
	ASSIGN(TC_PLOAM_DS_NO_MESSAGE, no_message);
	ASSIGN(TC_PLOAM_DS_POPUP, popup);
	ASSIGN(TC_PLOAM_DS_REQ_KEY, req_key);
	ASSIGN(TC_PLOAM_DS_CONFIG_PORT_ID, config_port_id);
	ASSIGN(TC_PLOAM_DS_PEE, pee);
	ASSIGN(TC_PLOAM_DS_PST, pst);
	ASSIGN(TC_PLOAM_DS_BER_INTERVAL, ber_interval);
	ASSIGN(TC_PLOAM_DS_KEY_SWITCHING, key_switching);
	ASSIGN(TC_PLOAM_DS_EXT_BURST, ext_burst);
	ASSIGN(TC_PLOAM_DS_PON_ID, pon_id);
	ASSIGN(TC_PLOAM_DS_SWIFT_POPUP, swift_popup);
	ASSIGN(TC_PLOAM_DS_RANGING_ADJ, ranging_adj);
	#undef ASSIGN

	return PON_STATUS_OK;
}

static enum fapi_pon_errorcode
pon_twdm_ploam_ds_counters_get_decode(struct pon_ctx *ctx,
				      struct nlattr **attrs,
				      void *priv)
{
	struct pon_twdm_ploam_ds_counters *dst_param = priv;
	struct nlattr *cnt[PON_MBOX_A_CNT_TC_PLOAM_DS_MAX + 1];

	memset(dst_param, 0, sizeof(*dst_param));

	UNUSED(ctx);

	if (!attrs[PON_MBOX_A_CNT])
		return PON_STATUS_ERR;

	if (nla_parse_nested(cnt, PON_MBOX_A_CNT_TC_PLOAM_DS_MAX,
			     attrs[PON_MBOX_A_CNT],
			     pon_mbox_cnt_tc_ploam_ds_policy) < 0)
		return PON_STATUS_ERR;

	#define ASSIGN(src, dst) \
		do { \
			CNT_DECODE_U64(src, dst);; \
			dst_param->all += dst_param->dst; \
		} while (0)

	ASSIGN(TC_PLOAM_DS_BST_PROFILE, burst_profile);
	ASSIGN(TC_PLOAM_DS_ASS_ONU, assign_onu_id);
	ASSIGN(TC_PLOAM_DS_RNG_TIME, ranging_time);
	ASSIGN(TC_PLOAM_DS_DEACT_ONU, deact_onu);
	ASSIGN(TC_PLOAM_DS_DIS_SER, disable_ser_no);
	ASSIGN(TC_PLOAM_DS_REQ_REG, req_reg);
	ASSIGN(TC_PLOAM_DS_ASS_ALLOC, assign_alloc_id);
	ASSIGN(TC_PLOAM_DS_KEY_CTRL, key_control);
	ASSIGN(TC_PLOAM_DS_SLP_ALLOW, sleep_allow);
	ASSIGN(TC_PLOAM_DS_CALIB_REQ, cal_req);
	ASSIGN(TC_PLOAM_DS_ADJ_TX_WL, tx_wavelength);
	ASSIGN(TC_PLOAM_DS_TUNE_CTRL, tune_ctrl);
	ASSIGN(TC_PLOAM_DS_SYS_PROFILE, system_profile);
	ASSIGN(TC_PLOAM_DS_CH_PROFILE, channel_profile);
	ASSIGN(TC_PLOAM_DS_PROT_CONTROL, protection);
	ASSIGN(TC_PLOAM_DS_CHG_PW_LVL, cpl);
	ASSIGN(TC_PLOAM_DS_PW_CONS, power);
	ASSIGN(TC_PLOAM_DS_RATE_CTRL, rate);
	ASSIGN(TC_PLOAM_DS_REBOOT_ONU, reset);
	ASSIGN(TC_PLOAM_DS_UNKNOWN, unknown);
	ASSIGN(TC_PLOAM_DS_ADJ_TX_WL_FAIL, tx_wavelength_err);
	ASSIGN(TC_PLOAM_DS_TUNE_REQ, tuning_request);
	ASSIGN(TC_PLOAM_DS_TUNE_COMPL, tuning_complete);
	#undef ASSIGN

	return PON_STATUS_OK;
}

static enum fapi_pon_errorcode
pon_tc_ploam_ds_counters_get(struct pon_ctx *ctx,
			     const uint8_t dswlch_id,
			     fapi_pon_decode decode,
			     void *param)
{
	struct read_cmd_cb cb_data;
	struct nl_msg *msg;
	enum fapi_pon_errorcode ret;
	uint32_t seq = NL_AUTO_SEQ;

	if (!pon_mode_check(ctx, MODE_ITU_PON))
		return PON_STATUS_OPERATION_MODE_ERR;

	/* send netlink message for PONFW_(X)GTC_PLOAM_DS_COUNTERS_CMD_ID */
	/* fill the nl message and get the TC_PLOAM_DS counters */
	ret = fapi_pon_nl_msg_prepare_decode(ctx, &msg, &cb_data, &seq,
					     decode,
					     NULL, param,
					     PON_MBOX_C_TC_PLOAM_DS_COUNTERS);
	if (ret != PON_STATUS_OK)
		return ret;

	ret = nla_put_u8(msg, PON_MBOX_D_DSWLCH_ID, dswlch_id);
	if (ret) {
		PON_DEBUG_ERR("Can't add netlink attribute");
		nlmsg_free(msg);
		return PON_STATUS_NL_ERR;
	}

	ret = fapi_pon_nl_msg_send(ctx, &msg, &cb_data, &seq);

	return ret;
}

enum fapi_pon_errorcode
fapi_pon_ploamd_cfg_set(struct pon_ctx *ctx,
			const struct pon_ploamd_cfg *param)
{
	struct ponfw_ploam_forward_config fw_param = {0};
	unsigned int i;

	if (!param)
		return PON_STATUS_INPUT_ERR;

	if (!pon_mode_check(ctx, MODE_ITU_PON))
		return PON_STATUS_OPERATION_MODE_ERR;

	for (i = 0; i < ARRAY_SIZE(param->enable); i++)
		fw_param.en |= (uint32_t)param->enable[i] << i;

	if (memcpy_s(fw_param.msg_type_id, sizeof(fw_param.msg_type_id),
		     param->msg_type_id, sizeof(param->msg_type_id))) {
		PON_DEBUG_ERR("memcpy_s failed");
		return PON_STATUS_MEMCPY_ERR;
	}

	return fapi_pon_generic_set(ctx,
				    PONFW_PLOAM_FORWARD_CONFIG_CMD_ID,
				    &fw_param,
				    sizeof(fw_param));
}

static enum fapi_pon_errorcode
pon_ploamd_cfg_get_copy(struct pon_ctx *ctx,
			const void *data,
			size_t data_size,
			void *priv)
{
	enum fapi_pon_errorcode ret;
	const struct ponfw_ploam_forward_config *src_param = data;
	struct pon_ploamd_cfg *dst_param = priv;
	unsigned int i;

	UNUSED(ctx);

	ret = integrity_check(dst_param, sizeof(*src_param), data_size);
	if (ret != PON_STATUS_OK)
		return ret;

	memset(dst_param, 0x0, sizeof(*dst_param));

	for (i = 0; i < ARRAY_SIZE(dst_param->enable); i++)
		dst_param->enable[i] = (uint8_t)(src_param->en >> i) & 1;

	if (memcpy_s(dst_param->msg_type_id, sizeof(dst_param->msg_type_id),
		     src_param->msg_type_id, sizeof(src_param->msg_type_id))) {
		PON_DEBUG_ERR("memcpy_s failed");
		return PON_STATUS_MEMCPY_ERR;
	}

	return PON_STATUS_OK;
}

enum fapi_pon_errorcode fapi_pon_ploamd_cfg_get(struct pon_ctx *ctx,
						struct pon_ploamd_cfg *param)
{
	if (!pon_mode_check(ctx, MODE_ITU_PON))
		return PON_STATUS_OPERATION_MODE_ERR;

	return fapi_pon_generic_get(ctx,
				    PONFW_PLOAM_FORWARD_CONFIG_CMD_ID,
				    NULL,
				    0,
				    &pon_ploamd_cfg_get_copy,
				    param);
}

static enum fapi_pon_errorcode
pon_alloc_counters_get_decode(struct pon_ctx *ctx,
				 struct nlattr **attrs,
				 void *priv)
{
	struct pon_alloc_counters *dst_param = priv;
	struct nlattr *cnt[PON_MBOX_A_CNT_ALLOC_MAX + 1];

	memset(dst_param, 0, sizeof(*dst_param));

	UNUSED(ctx);

	if (!attrs[PON_MBOX_A_CNT])
		return PON_STATUS_ERR;

	if (nla_parse_nested(cnt, PON_MBOX_A_CNT_ALLOC_MAX,
			     attrs[PON_MBOX_A_CNT],
			     pon_mbox_cnt_alloc_policy) < 0)
		return PON_STATUS_ERR;

	CNT_DECODE_U64(ALLOC_IDLE, idle);
	CNT_DECODE_U64(ALLOC_ALLOCATIONS, allocations);
	CNT_DECODE_U64(ALLOC_US_BW, us_bw);

	return PON_STATUS_OK;
}

enum fapi_pon_errorcode
fapi_pon_alloc_counters_get(struct pon_ctx *ctx, uint8_t alloc_index,
			    struct pon_alloc_counters *param)
{
	struct pon_range_limits limits = {0};
	struct read_cmd_cb cb_data;
	struct nl_msg *msg;
	enum fapi_pon_errorcode ret;
	uint32_t seq = NL_AUTO_SEQ;

	if (!pon_mode_check(ctx, MODE_ITU_PON))
		return PON_STATUS_OPERATION_MODE_ERR;

	ret = fapi_pon_limits_get(ctx, &limits);
	if (ret != PON_STATUS_OK)
		return ret;

	if (alloc_index > limits.alloc_idx_max)
		return PON_STATUS_VALUE_RANGE_ERR;

	ret = fapi_pon_nl_msg_prepare_decode(ctx, &msg, &cb_data, &seq,
				      &pon_alloc_counters_get_decode, NULL,
				      param, PON_MBOX_C_ALLOC_ID_COUNTERS);
	if (ret != PON_STATUS_OK)
		return ret;

	ret = nla_put_u8(msg, PON_MBOX_D_ALLOC_IDX, alloc_index);
	if (ret) {
		PON_DEBUG_ERR("Can't add netlink attribute");
		nlmsg_free(msg);
		return PON_STATUS_NL_ERR;
	}

	return fapi_pon_nl_msg_send(ctx, &msg, &cb_data, &seq);
}

static enum fapi_pon_errorcode
pon_xgtc_counters_get_decode(struct pon_ctx *ctx,
			    struct nlattr **attrs,
			    void *priv)
{
	struct pon_xgtc_counters *dst_param = priv;
	struct nlattr *cnt[PON_MBOX_A_CNT_XGTC_MAX + 1];

	memset(dst_param, 0, sizeof(*dst_param));

	UNUSED(ctx);

	if (!attrs[PON_MBOX_A_CNT])
		return PON_STATUS_ERR;

	if (nla_parse_nested(cnt, PON_MBOX_A_CNT_XGTC_MAX,
			     attrs[PON_MBOX_A_CNT],
			     pon_mbox_cnt_xgtc_policy) < 0)
		return PON_STATUS_ERR;

	CNT_DECODE_U64(XGTC_FS_HEC_ERR_CORR, fs_hec_err_corr);
	CNT_DECODE_U64(XGTC_FS_HEC_ERR_UNCORR, fs_hec_err_uncorr);
	CNT_DECODE_U64(XGTC_LOST_WORDS, lost_words);
	CNT_DECODE_U64(XGTC_PLOAM_MIC_ERR, ploam_mic_err);
	CNT_DECODE_U64(XGTC_PSBD_HEC_ERR_CORR, psbd_hec_err_corr);
	CNT_DECODE_U64(XGTC_PSBD_HEC_ERR_UNCORR,  psbd_hec_err_uncorr);

	return PON_STATUS_OK;
}

static enum fapi_pon_errorcode
	pon_xgtc_counters_get(struct pon_ctx *ctx,
			      const uint8_t dswlch_id,
			      struct pon_xgtc_counters *param)
{
	struct read_cmd_cb cb_data;
	struct nl_msg *msg;
	enum fapi_pon_errorcode ret;
	uint32_t seq = NL_AUTO_SEQ;

	if (!param)
		return PON_STATUS_INPUT_ERR;

	ret = fapi_pon_nl_msg_prepare_decode(ctx, &msg, &cb_data, &seq,
					     &pon_xgtc_counters_get_decode,
					     NULL, param,
					     PON_MBOX_C_XGTC_COUNTERS);
	if (ret != PON_STATUS_OK)
		return ret;

	ret = nla_put_u8(msg, PON_MBOX_D_DSWLCH_ID, dswlch_id);
	if (ret) {
		PON_DEBUG_ERR("Can't add netlink attribute");
		nlmsg_free(msg);
		return PON_STATUS_NL_ERR;
	}

	ret = fapi_pon_nl_msg_send(ctx, &msg, &cb_data, &seq);
	if (ret != PON_STATUS_OK)
		return ret;

	/* We get xgem_hec_err_corr, xgem_hec_err_uncorr from
	 * GTC_COUNTERS message, not here
	 */

	return PON_STATUS_OK;
}

enum fapi_pon_errorcode
fapi_pon_ploam_ds_counters_get(struct pon_ctx *ctx,
			       struct pon_ploam_ds_counters *param)
{
	enum fapi_pon_errorcode ret;
	struct pon_xgtc_counters pon_xgtc_cnt = { 0 };

	if (!pon_mode_check(ctx, MODE_ITU_PON))
		return PON_STATUS_OPERATION_MODE_ERR;

	/* GPON mode */
	/* XG-PON/XGS-PON/NG-PON2 mode */
	ret = pon_tc_ploam_ds_counters_get(ctx, PON_MBOX_D_DSWLCH_ID_CURR,
			&pon_ploam_ds_counters_get_decode, param);
	if (ret != PON_STATUS_OK)
		return ret;

	/* get XGTC */
	/* XG-PON/XGS-PON/NG-PON2 mode only */
	if (!pon_mode_check(ctx, MODE_987_XGPON |
				 MODE_9807_XGSPON |
				 MODE_989_NGPON2_10G |
				 MODE_989_NGPON2_2G5))
		goto EXIT;

	ret = pon_xgtc_counters_get(ctx, PON_MBOX_D_DSWLCH_ID_CURR, &pon_xgtc_cnt);
	if (ret != PON_STATUS_OK)
		return ret;
	param->mic_err = pon_xgtc_cnt.ploam_mic_err;
	param->all += param->mic_err;
EXIT:
	return ret;
}

static enum fapi_pon_errorcode
pon_gtc_counters_get_decode(struct pon_ctx *ctx,
			    struct nlattr **attrs,
			    void *priv)
{
	struct pon_gtc_counters *dst_param = priv;
	struct nlattr *cnt[PON_MBOX_A_CNT_GTC_MAX + 1];

	memset(dst_param, 0, sizeof(*dst_param));

	UNUSED(ctx);

	if (!attrs[PON_MBOX_A_CNT])
		return PON_STATUS_ERR;

	if (nla_parse_nested(cnt, PON_MBOX_A_CNT_GTC_MAX, attrs[PON_MBOX_A_CNT],
			     pon_mbox_cnt_gtc_policy) < 0)
		return PON_STATUS_ERR;

	CNT_DECODE_U64(GTC_BWMAP_HEC_ERRORS_CORR, bwmap_hec_errors_corr);
	CNT_DECODE_U64(GTC_BIP_ERRORS, bip_errors);
	CNT_DECODE_U64(GTC_GEM_HEC_ERRORS_CORR, gem_hec_errors_corr);
	CNT_DECODE_U64(GTC_GEM_HEC_ERRORS_UNCORR, gem_hec_errors_uncorr);
	CNT_DECODE_U64(GTC_DISC_GEM_FRAMES, disc_gem_frames);
	CNT_DECODE_U64(GTC_BYTES_CORR, bytes_corr);
	CNT_DECODE_U64(GTC_FEC_CODEWORDS_CORR, fec_codewords_corr);
	CNT_DECODE_U64(GTC_FEC_COREWORDS_UNCORR, fec_codewords_uncorr);
	CNT_DECODE_U64(GTC_TOTAL_FRAMES, total_frames);
	CNT_DECODE_U64(GTC_FEC_SEC, fec_sec);
	CNT_DECODE_U64(GTC_GEM_IDLE, gem_idle);
	CNT_DECODE_U64(GTC_LODS_EVENTS, lods_events);
	CNT_DECODE_U64(GTC_DG_TIME, dg_time);
	CNT_DECODE_U64(GTC_PLOAM_CRC_ERRORS, ploam_crc_errors);

	return PON_STATUS_OK;
}

static enum fapi_pon_errorcode
	pon_gtc_counters_get(struct pon_ctx *ctx,
			     const uint8_t dswlch_id,
			     struct pon_gtc_counters *param)
{
	struct read_cmd_cb cb_data;
	struct nl_msg *msg;
	enum fapi_pon_errorcode ret;
	uint32_t seq = NL_AUTO_SEQ;

	if (!pon_mode_check(ctx, MODE_ITU_PON))
		return PON_STATUS_OPERATION_MODE_ERR;

	ret = fapi_pon_nl_msg_prepare_decode(ctx, &msg, &cb_data, &seq,
					     &pon_gtc_counters_get_decode,
					     NULL, param,
					     PON_MBOX_C_GTC_COUNTERS);
	if (ret != PON_STATUS_OK)
		return ret;

	ret = nla_put_u8(msg, PON_MBOX_D_DSWLCH_ID, dswlch_id);
	if (ret) {
		PON_DEBUG_ERR("Can't add netlink attribute");
		nlmsg_free(msg);
		return PON_STATUS_NL_ERR;
	}

	return fapi_pon_nl_msg_send(ctx, &msg, &cb_data, &seq);
}

enum fapi_pon_errorcode
	fapi_pon_xgtc_counters_get(struct pon_ctx *ctx,
				   struct pon_xgtc_counters *param)
{
	enum fapi_pon_errorcode ret;
	struct pon_gtc_counters gtc_counters = { 0 };

	if (!param)
		return PON_STATUS_INPUT_ERR;

	/* XG-PON/XGS-PON/NG-PON2 mode only */
	if (!pon_mode_check(ctx, MODE_987_XGPON |
				 MODE_9807_XGSPON |
				 MODE_989_NGPON2_10G |
				 MODE_989_NGPON2_2G5))
		return PON_STATUS_OPERATION_MODE_ERR;

	ret = pon_xgtc_counters_get(ctx, PON_MBOX_D_DSWLCH_ID_CURR, param);
	if (ret != PON_STATUS_OK)
		return ret;

	/* We get xgem_hec_err_corr, xgem_hec_err_uncorr from
	 * GTC_COUNTERS message
	 */
	ret = pon_gtc_counters_get(ctx, PON_MBOX_D_DSWLCH_ID_CURR, &gtc_counters);
	if (ret != PON_STATUS_OK)
		return ret;

	param->xgem_hec_err_corr = gtc_counters.gem_hec_errors_corr;
	param->xgem_hec_err_uncorr = gtc_counters.gem_hec_errors_uncorr;

	return PON_STATUS_OK;
}

enum fapi_pon_errorcode
	fapi_pon_gtc_counters_get(struct pon_ctx *ctx,
				  struct pon_gtc_counters *param)
{
	return pon_gtc_counters_get(ctx, PON_MBOX_D_DSWLCH_ID_CURR, param);
}

/* Number of FEC codewords per downstream frame for each standard */
#define DS_FRAMES_TO_FEC_WORDS_MODE_984_GPON_2G5 153
#define DS_FRAMES_TO_FEC_WORDS_MODE_989_NGPON2_2G5 157
#define DS_FRAMES_TO_FEC_WORDS_MODE_ANY_10G 627

enum fapi_pon_errorcode
	fapi_pon_fec_counters_get(struct pon_ctx *ctx,
				  struct pon_fec_counters *param)
{
	enum fapi_pon_errorcode ret;
	struct pon_gpon_status gpon_status = {0};
	struct pon_gtc_counters gtc_counters = {0};
	uint8_t pon_mode;

	if (!ctx || !param)
		return PON_STATUS_INPUT_ERR;

	if (!pon_mode_check(ctx, MODE_ITU_PON))
		return PON_STATUS_OPERATION_MODE_ERR;

	ret = fapi_pon_gpon_status_get(ctx, &gpon_status);
	if (ret != PON_STATUS_OK)
		return ret;

	if (gpon_status.fec_status_ds) {
		/* The DS FEC codewords are calculated out from the total
		 * frames. The total frames are counting continuously and
		 * a value is reported even if the FEC feature is disabled.
		 */
		ret = pon_gtc_counters_get(ctx, PON_MBOX_D_DSWLCH_ID_CURR,
					   &gtc_counters);
		if (ret != PON_STATUS_OK)
			return ret;

		param->bytes_corr = gtc_counters.bytes_corr;
		param->words_corr = gtc_counters.fec_codewords_corr;
		param->words_uncorr = gtc_counters.fec_codewords_uncorr;
		param->seconds = gtc_counters.fec_sec;

		ret = fapi_pon_mode_get(ctx, &pon_mode);
		if (ret != PON_STATUS_OK)
			return ret;
		switch (pon_mode) {
			case PON_MODE_984_GPON:
				param->words = gtc_counters.total_frames
						* DS_FRAMES_TO_FEC_WORDS_MODE_984_GPON_2G5;
				break;
			case PON_MODE_989_NGPON2_2G5:
				param->words = gtc_counters.total_frames
						* DS_FRAMES_TO_FEC_WORDS_MODE_989_NGPON2_2G5;
				break;
			default:
				/* All 10G modes (NG-PON2, XG-PON, XGS-PON) */
				param->words = gtc_counters.total_frames
						* DS_FRAMES_TO_FEC_WORDS_MODE_ANY_10G;
				break;
		}
	}

	return PON_STATUS_OK;
}

enum fapi_pon_errorcode
	fapi_pon_twdm_fec_counters_get(struct pon_ctx *ctx,
				       const uint8_t dswlch_id,
				       struct pon_fec_counters *param)
{
	enum fapi_pon_errorcode ret;
	struct pon_gpon_status gpon_status = {0};
	struct pon_gtc_counters gtc_counters = {0};

	if (!ctx || !param)
		return PON_STATUS_INPUT_ERR;

	if (!pon_mode_check(ctx, MODE_ITU_PON))
		return PON_STATUS_OPERATION_MODE_ERR;

	ret = fapi_pon_gpon_status_get(ctx, &gpon_status);
	if (ret != PON_STATUS_OK)
		return ret;

	/* The DS FEC codewords are calculated out from the total frames.
	 * The total frames are counting continuously and so a value is
	 * reported even if the FEC feature is disabled.
	 */
	if (gpon_status.fec_status_ds) {
		ret = pon_gtc_counters_get(ctx, dswlch_id, &gtc_counters);
		if (ret != PON_STATUS_OK)
			return ret;

		param->bytes_corr = gtc_counters.bytes_corr;
		param->words_corr = gtc_counters.fec_codewords_corr;
		param->words_uncorr = gtc_counters.fec_codewords_uncorr;
		param->seconds = gtc_counters.fec_sec;
		param->words = gtc_counters.total_frames
				* DS_FRAMES_TO_FEC_WORDS_MODE_ANY_10G;
	}

	return PON_STATUS_OK;
}

static enum fapi_pon_errorcode
pon_gem_port_counters_get_decode(struct pon_ctx *ctx,
				 struct nlattr **attrs,
				 void *priv)
{
	struct pon_gem_port_counters *dst_param = priv;
	struct nlattr *cnt[PON_MBOX_A_CNT_GEM_PORT_MAX + 1];

	if (memset_s(dst_param, sizeof(*dst_param), 0, sizeof(*dst_param))) {
		PON_DEBUG_ERR("memset_s failed");
		return PON_STATUS_MEMCPY_ERR;
	}

	UNUSED(ctx);

	if (!attrs[PON_MBOX_A_CNT])
		return PON_STATUS_ERR;

	if (nla_parse_nested(cnt, PON_MBOX_A_CNT_GEM_PORT_MAX,
			     attrs[PON_MBOX_A_CNT],
			     pon_mbox_cnt_gem_port_policy) < 0)
		return PON_STATUS_ERR;

	CNT_DECODE_U64(GEM_PORT_TX_FRAMES, tx_frames);
	CNT_DECODE_U64(GEM_PORT_TX_FRAGMENTS, tx_fragments);
	CNT_DECODE_U64(GEM_PORT_TX_BYTES, tx_bytes);
	CNT_DECODE_U64(GEM_PORT_RX_FRAMES, rx_frames);
	CNT_DECODE_U64(GEM_PORT_RX_FRAGMENTS, rx_fragments);
	CNT_DECODE_U64(GEM_PORT_RX_BYTES, rx_bytes);
	CNT_DECODE_U64(GEM_PORT_KEY_ERRORS, key_errors);

	return PON_STATUS_OK;
}

static enum fapi_pon_errorcode
pon_gem_port_counters_get(struct pon_ctx *ctx,
			  const uint8_t dswlch_id,
			  uint16_t gem_port_id,
			  struct pon_gem_port_counters *param)
{
	struct pon_gem_port gem_port;
	struct pon_range_limits limits = {0};
	struct read_cmd_cb cb_data;
	struct nl_msg *msg;
	uint32_t seq = NL_AUTO_SEQ;
	enum fapi_pon_errorcode ret;

	ret = fapi_pon_limits_get(ctx, &limits);
	if (ret != PON_STATUS_OK)
		return ret;

	if (gem_port_id > limits.gem_port_id_max)
		return PON_STATUS_VALUE_RANGE_ERR;

	/* This is only done to get the GEM port index for the GEM ID. */
	ret = fapi_pon_gem_port_id_get(ctx, gem_port_id, &gem_port);
	if (ret != PON_STATUS_OK)
		return ret;

	ret = fapi_pon_nl_msg_prepare_decode(ctx, &msg, &cb_data, &seq,
					     &pon_gem_port_counters_get_decode,
					     NULL,
					     param,
					     PON_MBOX_C_GEM_PORT_COUNTERS);
	if (ret != PON_STATUS_OK)
		return ret;

	ret = nla_put_u8(msg, PON_MBOX_D_GEM_IDX, gem_port.gem_port_index);
	if (ret) {
		PON_DEBUG_ERR("Can't add netlink attribute");
		nlmsg_free(msg);
		return PON_STATUS_NL_ERR;
	}

	ret = nla_put_u8(msg, PON_MBOX_D_DSWLCH_ID, dswlch_id);
	if (ret) {
		PON_DEBUG_ERR("Can't add netlink attribute DSWLCH_ID");
		nlmsg_free(msg);
		return PON_STATUS_NL_ERR;
	}

	ret = fapi_pon_nl_msg_send(ctx, &msg, &cb_data, &seq);

	/* Set the GEM port id in the result as we do not get it back.
	 */
	param->gem_port_id = gem_port_id;

	return ret;
}

enum fapi_pon_errorcode
fapi_pon_gem_port_counters_get(struct pon_ctx *ctx,
			       uint16_t gem_port_id,
			       struct pon_gem_port_counters *param)
{
	if (!param)
		return PON_STATUS_INPUT_ERR;

	if (!pon_mode_check(ctx, MODE_ITU_PON))
		return PON_STATUS_OPERATION_MODE_ERR;

	return pon_gem_port_counters_get(ctx,
		PON_MBOX_D_DSWLCH_ID_CURR, gem_port_id, param);
}

static enum fapi_pon_errorcode
pon_gem_all_counters_get_decode(struct pon_ctx *ctx,
				struct nlattr **attrs,
				void *priv)
{
	struct pon_gem_port_counters *dst_param = priv;
	struct nlattr *cnt[PON_MBOX_A_CNT_GEM_PORT_MAX + 1];

	if (memset_s(dst_param, sizeof(*dst_param), 0, sizeof(*dst_param))) {
		PON_DEBUG_ERR("memset_s failed");
		return PON_STATUS_MEMCPY_ERR;
	}

	UNUSED(ctx);

	if (!attrs[PON_MBOX_A_CNT])
		return PON_STATUS_ERR;

	if (nla_parse_nested(cnt, PON_MBOX_A_CNT_GEM_PORT_MAX,
			     attrs[PON_MBOX_A_CNT],
			     pon_mbox_cnt_gem_port_policy) < 0)
		return PON_STATUS_ERR;

	CNT_DECODE_U64(GEM_PORT_TX_FRAMES, tx_frames);
	CNT_DECODE_U64(GEM_PORT_TX_FRAGMENTS, tx_fragments);
	CNT_DECODE_U64(GEM_PORT_TX_BYTES, tx_bytes);
	CNT_DECODE_U64(GEM_PORT_RX_FRAMES, rx_frames);
	CNT_DECODE_U64(GEM_PORT_RX_FRAGMENTS, rx_fragments);
	CNT_DECODE_U64(GEM_PORT_RX_BYTES, rx_bytes);
	CNT_DECODE_U64(GEM_PORT_KEY_ERRORS, key_errors);

	return PON_STATUS_OK;
}

static enum fapi_pon_errorcode
pon_gem_all_counters_get(struct pon_ctx *ctx,
			 const uint8_t dswlch_id,
			 struct pon_gem_port_counters *param)
{
	struct read_cmd_cb cb_data;
	struct nl_msg *msg;
	uint32_t seq = NL_AUTO_SEQ;
	enum fapi_pon_errorcode ret;

	ret = fapi_pon_nl_msg_prepare_decode(ctx, &msg, &cb_data, &seq,
					     &pon_gem_all_counters_get_decode,
					     NULL,
					     param,
					     PON_MBOX_C_GEM_ALL_COUNTERS);
	if (ret != PON_STATUS_OK)
		return ret;

	ret = nla_put_u8(msg, PON_MBOX_D_DSWLCH_ID, dswlch_id);
	if (ret) {
		PON_DEBUG_ERR("Can't add netlink attribute DSWLCH_ID");
		nlmsg_free(msg);
		return PON_STATUS_NL_ERR;
	}

	return fapi_pon_nl_msg_send(ctx, &msg, &cb_data, &seq);
}

enum fapi_pon_errorcode
fapi_pon_gem_all_counters_get(struct pon_ctx *ctx,
			      struct pon_gem_port_counters *param)
{
	if (!param)
		return PON_STATUS_INPUT_ERR;

	if (!pon_mode_check(ctx, MODE_ITU_PON))
		return PON_STATUS_OPERATION_MODE_ERR;

	return pon_gem_all_counters_get(ctx,
		PON_MBOX_D_DSWLCH_ID_CURR, param);
}

enum fapi_pon_errorcode
fapi_pon_twdm_xgem_port_counters_get(struct pon_ctx *ctx,
				     const uint8_t dswlch_id,
				     uint16_t gem_port_id,
				     struct pon_gem_port_counters *param)
{
	if (!param)
		return PON_STATUS_INPUT_ERR;

	if (!pon_mode_check(ctx, MODE_989_NGPON2_2G5 |
				 MODE_989_NGPON2_10G))
		return PON_STATUS_OPERATION_MODE_ERR;

	return pon_gem_port_counters_get(ctx,
		dswlch_id, gem_port_id, param);
}

enum fapi_pon_errorcode
fapi_pon_twdm_xgem_all_counters_get(struct pon_ctx *ctx,
				    const uint8_t dswlch_id,
				    struct pon_gem_port_counters *param)
{
	if (!param)
		return PON_STATUS_INPUT_ERR;

	if (!pon_mode_check(ctx, MODE_989_NGPON2_2G5 |
				 MODE_989_NGPON2_10G))
		return PON_STATUS_OPERATION_MODE_ERR;

	return pon_gem_all_counters_get(ctx, dswlch_id, param);
}

enum fapi_pon_errorcode
fapi_pon_debug_alarm_cfg_set(struct pon_ctx *ctx, uint16_t pon_alarm_id,
			     const struct pon_debug_alarm_cfg *param)
{
	struct ponfw_debug_alarm_control_config fw_param = {0};
	enum fapi_pon_errorcode ret;

	if (!param)
		return PON_STATUS_INPUT_ERR;

	if (pon_mode_check(ctx, MODE_AON))
		return PON_STATUS_OPERATION_MODE_ERR;

	ret = debug_support_check(ctx);
	if (ret != PON_STATUS_OK)
		return ret;

	fw_param.aen = param->enable;
	fw_param.logen = param->logging;
	fw_param.alarm_id = pon_alarm_id;

	return fapi_pon_generic_set(ctx,
				    PONFW_DEBUG_ALARM_CONTROL_CONFIG_CMD_ID,
				    &fw_param,
				    sizeof(fw_param));
}

static enum fapi_pon_errorcode
pon_debug_alarm_cfg_get_copy(struct pon_ctx *ctx,
			     const void *data,
			     size_t data_size,
			     void *priv)
{
	enum fapi_pon_errorcode ret;
	const struct ponfw_debug_alarm_control_config *src_param = data;
	struct pon_debug_alarm_cfg *dst_param = priv;

	UNUSED(ctx);

	ret = integrity_check(dst_param, sizeof(*src_param), data_size);
	if (ret != PON_STATUS_OK)
		return ret;

	memset(dst_param, 0x0, sizeof(*dst_param));

	dst_param->enable = src_param->aen;
	dst_param->logging = src_param->logen;

	return PON_STATUS_OK;
}

enum fapi_pon_errorcode
fapi_pon_debug_alarm_cfg_get(struct pon_ctx *ctx, uint16_t pon_alarm_id,
			     struct pon_debug_alarm_cfg *param)
{
	struct ponfw_debug_alarm_control_config fw_param = {0};
	enum fapi_pon_errorcode ret;

	if (pon_mode_check(ctx, MODE_AON))
		return PON_STATUS_OPERATION_MODE_ERR;

	ret = debug_support_check(ctx);
	if (ret != PON_STATUS_OK)
		return ret;

	fw_param.alarm_id = pon_alarm_id;

	return fapi_pon_generic_get(ctx,
				    PONFW_DEBUG_ALARM_CONTROL_CONFIG_CMD_ID,
				    &fw_param,
				    PONFW_DEBUG_ALARM_CONTROL_CONFIG_LEN,
				    &pon_debug_alarm_cfg_get_copy,
				    param);
}

enum fapi_pon_errorcode
fapi_pon_alarm_cfg_set(struct pon_ctx *ctx,
		       const struct pon_alarm_cfg *param)
{
	struct ponfw_alarm_control_config fw_param = {0};

	if (!param)
		return PON_STATUS_INPUT_ERR;

	if (pon_mode_check(ctx, MODE_AON))
		return PON_STATUS_OPERATION_MODE_ERR;

	fw_param.aen = param->enable;
	fw_param.alarm_id = param->alarm_id;

	return fapi_pon_generic_set(ctx,
				    PONFW_ALARM_CONTROL_CONFIG_CMD_ID,
				    &fw_param,
				    sizeof(fw_param));
}

static enum fapi_pon_errorcode
pon_alarm_cfg_get_copy(struct pon_ctx *ctx,
		       const void *data,
		       size_t data_size,
		       void *priv)
{
	enum fapi_pon_errorcode ret;
	const struct ponfw_alarm_control_config *src_param = data;
	struct pon_alarm_cfg *dst_param = priv;

	UNUSED(ctx);

	ret = integrity_check(dst_param, sizeof(*src_param), data_size);
	if (ret != PON_STATUS_OK)
		return ret;

	memset(dst_param, 0x0, sizeof(*dst_param));

	dst_param->enable = src_param->aen;
	dst_param->alarm_id = src_param->alarm_id;

	return PON_STATUS_OK;
}

enum fapi_pon_errorcode
fapi_pon_alarm_cfg_get(struct pon_ctx *ctx,
		       uint16_t pon_alarm_id,
		       struct pon_alarm_cfg *param)
{
	struct ponfw_alarm_control_config fw_param = {0};

	if (pon_mode_check(ctx, MODE_AON))
		return PON_STATUS_OPERATION_MODE_ERR;

	fw_param.alarm_id = pon_alarm_id;

	return fapi_pon_generic_get(ctx,
				    PONFW_ALARM_CONTROL_CONFIG_CMD_ID,
				    &fw_param,
				    PONFW_ALARM_CONTROL_CONFIG_LEN,
				    &pon_alarm_cfg_get_copy,
				    param);
}

static enum fapi_pon_errorcode pon_alarm_status_get_copy(struct pon_ctx *ctx,
							 const void *data,
							 size_t data_size,
							 void *priv)
{
	const union ponfw_msg *fw = data;
	struct pon_alarm_status *dst_param = priv;
	/* Number of alarms reported by the FW.
	 * It is calculated by checking FW message size in bytes and division by
	 * 4 to get number of reported Alarms.
	 */
	uint32_t num_of_alarms = data_size / 4;
	unsigned int i;

	UNUSED(ctx);

	dst_param->alarm_status = PON_ALARM_DIS;

	for (i = 0; i < num_of_alarms; i++) {
		if (fw->val[i] == dst_param->alarm_id) {
			dst_param->alarm_status = PON_ALARM_EN;
			break;
		}
	}

	return PON_STATUS_OK;
}

enum fapi_pon_errorcode
fapi_pon_alarm_status_get(struct pon_ctx *ctx,
			  uint16_t pon_alarm_id,
			  struct pon_alarm_status *param)
{
	if (!param)
		return PON_STATUS_INPUT_ERR;

	if (pon_mode_check(ctx, MODE_AON))
		return PON_STATUS_OPERATION_MODE_ERR;

	memset(param, 0x0, sizeof(*param));

	param->alarm_id = pon_alarm_id;

	return fapi_pon_generic_get(ctx,
				    PONFW_GET_STATIC_ALARM_CMD_ID,
				    NULL,
				    0,
				    &pon_alarm_status_get_copy,
				    param);
}

enum fapi_pon_errorcode
fapi_pon_alarm_status_set(struct pon_ctx *ctx,
			  uint16_t pon_alarm_id,
			  int mode)
{
	enum fapi_pon_errorcode ret;
	struct ponfw_debug_trigger_alarm fw_param = {0};

	if (pon_mode_check(ctx, MODE_AON))
		return PON_STATUS_OPERATION_MODE_ERR;

	ret = debug_support_check(ctx);
	if (ret != PON_STATUS_OK)
		return ret;

	fw_param.alarm_id = pon_alarm_id;
	fw_param.mode = mode;

	return fapi_pon_generic_set(ctx,
				    PONFW_DEBUG_TRIGGER_ALARM_CMD_ID,
				    &fw_param,
				    sizeof(struct ponfw_debug_trigger_alarm));
}

static enum fapi_pon_errorcode pon_ploam_state_get_copy(struct pon_ctx *ctx,
							const void *data,
							size_t data_size,
							void *priv)
{
	enum fapi_pon_errorcode ret;
	const struct ponfw_ploam_state *src_param = data;
	struct pon_ploam_state *dst_param = priv;

	UNUSED(ctx);

	ret = integrity_check(dst_param, sizeof(*src_param), data_size);
	if (ret != PON_STATUS_OK)
		return ret;

	memset(dst_param, 0x0, sizeof(*dst_param));

	dst_param->current = src_param->ploam_act;
	dst_param->previous = src_param->ploam_prev;
	dst_param->time_curr = src_param->ploam_time;
	dst_param->change_reason = src_param->reason;

	return PON_STATUS_OK;
}

enum fapi_pon_errorcode fapi_pon_ploam_state_get(struct pon_ctx *ctx,
						 struct pon_ploam_state *param)
{
	if (!pon_mode_check(ctx, MODE_ITU_PON))
		return PON_STATUS_OPERATION_MODE_ERR;

	return fapi_pon_generic_get(ctx,
				    PONFW_PLOAM_STATE_CMD_ID,
				    NULL,
				    0,
				    &pon_ploam_state_get_copy,
				    param);
}

static enum fapi_pon_errorcode
pon_alloc_discard_counters_get_decode(struct pon_ctx *ctx,
				      struct nlattr **attrs,
				      void *priv)
{
	struct pon_alloc_discard_counters *dst_param = priv;
	struct nlattr *cnt[PON_MBOX_A_CNT_ALLOC_DISCARD_MAX + 1];
	struct nlattr *attr = NULL, *discs, *rules;
	unsigned int i;
	int err;
	int remaining;

	UNUSED(ctx);

	if (!attrs[PON_MBOX_A_CNT])
		return PON_STATUS_ERR;

	if (nla_parse_nested(cnt, PON_MBOX_A_CNT_ALLOC_DISCARD_MAX,
			     attrs[PON_MBOX_A_CNT],
			     pon_mbox_cnt_alloc_discard_policy) < 0)
		return PON_STATUS_ERR;

	discs = cnt[PON_MBOX_A_CNT_ALLOC_DISCARD_DISCS];
	if (discs) {
		err = nla_validate(nla_data(discs),
				   nla_len(discs),
				   PON_MBOX_A_CNT_ALLOC_DISCARD_ITEM_MAX,
				   pon_mbox_cnt_alloc_discard_item_policy);
		if (err)
			return PON_STATUS_ERR;
		i = 0;
		nla_for_each_nested(attr, discs, remaining) {
			if (i >= ARRAY_SIZE(dst_param->disc))
				break;
			if (nla_type(attr) != PON_MBOX_A_CNT_ALLOC_DISCARD_ITEM)
				continue;
			dst_param->disc[i] = nla_get_u64(attr);
			i++;
		}
	}

	rules = cnt[PON_MBOX_A_CNT_ALLOC_DISCARD_RULES];
	if (rules) {
		err = nla_validate(nla_data(rules),
				   nla_len(rules),
				   PON_MBOX_A_CNT_ALLOC_DISCARD_ITEM_MAX,
				   pon_mbox_cnt_alloc_discard_item_policy);
		if (err)
			return PON_STATUS_ERR;
		i = 0;
		nla_for_each_nested(attr, rules, remaining) {
			if (i >= ARRAY_SIZE(dst_param->rule))
				break;
			if (nla_type(attr) != PON_MBOX_A_CNT_ALLOC_DISCARD_ITEM)
				continue;
			dst_param->rule[i] = nla_get_u64(attr);
			i++;
		}
	}

	return PON_STATUS_OK;
}

enum fapi_pon_errorcode fapi_pon_alloc_discard_counters_get(struct pon_ctx *ctx,
				struct pon_alloc_discard_counters *param)
{
	struct read_cmd_cb cb_data;
	struct nl_msg *msg;
	enum fapi_pon_errorcode ret;
	uint32_t seq = NL_AUTO_SEQ;

	if (!pon_mode_check(ctx, MODE_ITU_PON))
		return PON_STATUS_OPERATION_MODE_ERR;

	ret = debug_support_check(ctx);
	if (ret != PON_STATUS_OK)
		return ret;

	ret = fapi_pon_nl_msg_prepare_decode(ctx, &msg, &cb_data, &seq,
				      &pon_alloc_discard_counters_get_decode,
				      NULL, param,
				      PON_MBOX_C_ALLOC_LOST_COUNTERS);
	if (ret != PON_STATUS_OK)
		return ret;

	return fapi_pon_nl_msg_send(ctx, &msg, &cb_data, &seq);
}

static enum fapi_pon_errorcode pon_register_get_copy(struct pon_ctx *ctx,
						     const void *data,
						     size_t data_size,
						     void *priv)
{
	enum fapi_pon_errorcode ret;
	const struct ponfw_debug_data_access *src_param = data;
	struct pon_register *dst_param = priv;

	UNUSED(ctx);

	/* Only data1 is provided, then it should be 12 bytes long */
	ret = integrity_check(dst_param, 12, data_size);
	if (ret != PON_STATUS_OK)
		return ret;

	memset(dst_param, 0x0, sizeof(*dst_param));

	dst_param->addr = src_param->address;
	dst_param->data = src_param->data;

	return PON_STATUS_OK;
}

enum fapi_pon_errorcode fapi_pon_register_get(struct pon_ctx *ctx,
					      uint32_t dst_addr,
					      struct pon_register *param)
{
	enum fapi_pon_errorcode ret;
	struct ponfw_debug_data_access fw_param;

	if (pon_mode_check(ctx, MODE_AON))
		return PON_STATUS_OPERATION_MODE_ERR;

	ret = debug_support_check(ctx);
	if (ret != PON_STATUS_OK)
		return ret;

	memset(&fw_param, 0, sizeof(fw_param));
	fw_param.address = dst_addr;
	fw_param.bus = PONFW_DEBUG_DATA_ACCESS_BUS_IO;
	fw_param.plength = 1; /* 1 x 32bit */

	return fapi_pon_generic_get(ctx,
				    PONFW_DEBUG_DATA_ACCESS_CMD_ID,
				    &fw_param,
				    PONFW_DEBUG_DATA_ACCESS_LENR,
				    &pon_register_get_copy,
				    param);
}

enum fapi_pon_errorcode fapi_pon_register_set(struct pon_ctx *ctx,
					      const struct pon_register *param)
{
	enum fapi_pon_errorcode ret;
	struct ponfw_debug_data_access fw_param = {0};

	if (!param)
		return PON_STATUS_INPUT_ERR;

	if (pon_mode_check(ctx, MODE_AON))
		return PON_STATUS_OPERATION_MODE_ERR;

	ret = debug_support_check(ctx);
	if (ret != PON_STATUS_OK)
		return ret;

	fw_param.address = param->addr;
	fw_param.data = param->data;
	fw_param.bus = PONFW_DEBUG_DATA_ACCESS_BUS_IO;
	fw_param.plength = 1; /* 1 x 32bit */

	return fapi_pon_generic_set(ctx,
				    PONFW_DEBUG_DATA_ACCESS_CMD_ID,
				    &fw_param,
				    sizeof(struct ponfw_debug_data_access));
}

#define DEBUG_ALLOC_GPON_ONU_ID 0
#define DEBUG_ALLOC_GPON_MSG_TYPE_ID 0x0A
#define DEBUG_ALLOC_GPON_ALLOC_ID_MIN 256
#define DEBUG_ALLOC_GPON_ALLOC_ID_TYPE_ASSIGN 0x01
#define DEBUG_ALLOC_GPON_ALLOC_ID_TYPE_DEASSIGN 0xFF

static enum fapi_pon_errorcode pon_gpon_debug_alloc(struct pon_ctx *ctx,
						    uint32_t onu_id,
						    uint16_t alloc_id,
						    bool assign)
{
	struct ponfw_debug_gtc_ploam_send fw_param = {0};

	if (alloc_id < DEBUG_ALLOC_GPON_ALLOC_ID_MIN)
		return PON_STATUS_VALUE_RANGE_ERR;

	ASSIGN_AND_OVERFLOW_CHECK(fw_param.id, onu_id);
	fw_param.mid = DEBUG_ALLOC_GPON_MSG_TYPE_ID;
	fw_param.data1 = alloc_id << 4;

	if (assign)
		fw_param.data2 = DEBUG_ALLOC_GPON_ALLOC_ID_TYPE_ASSIGN << 8;
	else
		fw_param.data2 = DEBUG_ALLOC_GPON_ALLOC_ID_TYPE_DEASSIGN << 8;

	return fapi_pon_generic_set(ctx,
		PONFW_DEBUG_GTC_PLOAM_SEND_CMD_ID,
		&fw_param,
		sizeof(struct ponfw_debug_gtc_ploam_send));
}

#define DEBUG_ALLOC_XPON_ONU_ID 0
#define DEBUG_ALLOC_XPON_MSG_TYPE_ID 0x0A
#define DEBUG_ALLOC_XPON_SEQ_NO 0
#define DEBUG_ALLOC_XPON_ALLOC_ID_MIN 1024
#define DEBUG_ALLOC_XPON_ALLOC_ID_TYPE_ASSIGN 0x01
#define DEBUG_ALLOC_XPON_ALLOC_ID_TYPE_DEASSIGN 0xFF

static enum fapi_pon_errorcode pon_xpon_debug_alloc(struct pon_ctx *ctx,
						    uint16_t alloc_id,
						    bool assign)
{
	struct ponfw_debug_xgtc_ploam_send fw_param = {0};

	if (alloc_id < DEBUG_ALLOC_XPON_ALLOC_ID_MIN)
		return PON_STATUS_VALUE_RANGE_ERR;

	fw_param.id = DEBUG_ALLOC_XPON_ONU_ID;
	fw_param.type = DEBUG_ALLOC_XPON_MSG_TYPE_ID;
	fw_param.seqno = DEBUG_ALLOC_XPON_SEQ_NO;

	if (assign)
		fw_param.msg[0] = (alloc_id << 16) |
				(DEBUG_ALLOC_XPON_ALLOC_ID_TYPE_ASSIGN << 8);
	else
		fw_param.msg[0] = (alloc_id << 16) |
				(DEBUG_ALLOC_XPON_ALLOC_ID_TYPE_DEASSIGN << 8);

	return fapi_pon_generic_set(ctx,
		PONFW_DEBUG_XGTC_PLOAM_SEND_CMD_ID,
		&fw_param,
		sizeof(struct ponfw_debug_xgtc_ploam_send));
}

static enum fapi_pon_errorcode pon_debug_alloc(struct pon_ctx *ctx,
					       uint32_t onu_id,
					       uint16_t alloc_id,
					       bool assign)
{
	enum fapi_pon_errorcode ret;
	struct pon_range_limits limits = {0};

	if (!pon_mode_check(ctx, MODE_ITU_PON))
		return PON_STATUS_OPERATION_MODE_ERR;

	ret = debug_support_check(ctx);
	if (ret != PON_STATUS_OK)
		return ret;

	ret = fapi_pon_limits_get(ctx, &limits);
	if (ret != PON_STATUS_OK)
		return ret;

	if (alloc_id > limits.alloc_id_max)
		return PON_STATUS_VALUE_RANGE_ERR;

	/* GPON mode */
	if (pon_mode_check(ctx, MODE_984_GPON))
		return pon_gpon_debug_alloc(ctx, onu_id, alloc_id, assign);

	/* XG-PON/XGS-PON/NG-PON2 mode */
	return pon_xpon_debug_alloc(ctx, alloc_id, assign);
}

enum fapi_pon_errorcode fapi_pon_debug_alloc_assign(struct pon_ctx *ctx,
						    uint16_t alloc_id)
{
	enum fapi_pon_errorcode ret;
	struct pon_gpon_status gpon_status = {0};

	ret = fapi_pon_gpon_status_get(ctx, &gpon_status);
	if (ret != PON_STATUS_OK)
		return ret;

	return pon_debug_alloc(ctx, gpon_status.onu_id, alloc_id, true);
}

enum fapi_pon_errorcode fapi_pon_debug_alloc_deassign(struct pon_ctx *ctx,
						      uint16_t alloc_id)
{
	enum fapi_pon_errorcode ret;
	struct pon_gpon_status gpon_status = {0};

	ret = fapi_pon_gpon_status_get(ctx, &gpon_status);
	if (ret != PON_STATUS_OK)
		return ret;

	return pon_debug_alloc(ctx, gpon_status.onu_id, alloc_id, false);
}

static enum fapi_pon_errorcode
pon_debug_trace_run_status_get_copy(struct pon_ctx *ctx,
				    const void *data,
				    size_t data_size,
				    void *priv)
{
	enum fapi_pon_errorcode ret;
	const struct ponfw_debug_trace_control *src_param = data;
	struct pon_debug_trace_run_status *dst_param = priv;

	UNUSED(ctx);

	ret = integrity_check(dst_param, sizeof(*src_param), data_size);
	if (ret != PON_STATUS_OK)
		return ret;

	memset(dst_param, 0x0, sizeof(*dst_param));

	dst_param->run = src_param->run;
	dst_param->done = src_param->done;

	return PON_STATUS_OK;
}

enum fapi_pon_errorcode
fapi_pon_debug_trace_run_status_get(struct pon_ctx *ctx,
				struct pon_debug_trace_run_status *param)
{
	enum fapi_pon_errorcode ret;

	if (pon_mode_check(ctx, MODE_AON))
		return PON_STATUS_OPERATION_MODE_ERR;

	ret = debug_support_check(ctx);
	if (ret != PON_STATUS_OK)
		return ret;

	return fapi_pon_generic_get(ctx,
				    PONFW_DEBUG_TRACE_CONTROL_CMD_ID,
				    NULL,
				    0,
				    &pon_debug_trace_run_status_get_copy,
				    param);
}

static enum fapi_pon_errorcode
pon_debug_random_number_get_copy(struct pon_ctx *ctx,
				 const void *data,
				 size_t data_size,
				 void *priv)
{
	enum fapi_pon_errorcode ret;
	const struct ponfw_debug_random_values *src_param = data;
	struct pon_debug_random_number *dst_param = priv;

	UNUSED(ctx);

	ret = integrity_check(dst_param, sizeof(*src_param), data_size);
	if (ret != PON_STATUS_OK)
		return ret;

	memset(dst_param, 0x0, sizeof(*dst_param));

	dst_param->type = src_param->type;
	pon_byte_copy((uint8_t *)dst_param->random_value,
		      src_param->rnd, PON_RAND_VAL_SIZE);

	return PON_STATUS_OK;
}

enum fapi_pon_errorcode
fapi_pon_debug_random_number_get(struct pon_ctx *ctx,
				 uint32_t type,
				 struct pon_debug_random_number *param)
{
	enum fapi_pon_errorcode ret;
	struct ponfw_debug_random_values fw_param = {0};

	if (pon_mode_check(ctx, MODE_AON))
		return PON_STATUS_OPERATION_MODE_ERR;

	ret = debug_support_check(ctx);
	if (ret != PON_STATUS_OK)
		return ret;

	ASSIGN_AND_OVERFLOW_CHECK(fw_param.type, type);

	return fapi_pon_generic_get(ctx,
				    PONFW_DEBUG_RANDOM_VALUES_CMD_ID,
				    &fw_param,
				    PONFW_DEBUG_RANDOM_VALUES_LENR,
				    &pon_debug_random_number_get_copy,
				    param);
}

enum fapi_pon_errorcode
fapi_pon_debug_trace_cfg_set(struct pon_ctx *ctx,
			     const struct pon_debug_trace_cfg *param)
{
	enum fapi_pon_errorcode ret;
	struct ponfw_debug_trace_config fw_param = {0};
	struct pon_debug_trace_run_status status_param;

	if (!param)
		return PON_STATUS_INPUT_ERR;

	if (pon_mode_check(ctx, MODE_AON))
		return PON_STATUS_OPERATION_MODE_ERR;

	ret = debug_support_check(ctx);
	if (ret != PON_STATUS_OK)
		return ret;

	ret = fapi_pon_debug_trace_run_status_get(ctx, &status_param);
	if (ret != PON_STATUS_OK)
		return ret;

	if (status_param.run != 0 || status_param.done != 0)
		return PON_STATUS_TRACE_MODULE_NOT_READY;

	ASSIGN_AND_OVERFLOW_CHECK(fw_param.st, param->suppress_trigger);
	ASSIGN_AND_OVERFLOW_CHECK(fw_param.av, param->assume_valid);
	ASSIGN_AND_OVERFLOW_CHECK(fw_param.src, param->trigger_source);
	ASSIGN_AND_OVERFLOW_CHECK(fw_param.pocs, param->samples);
	ASSIGN_AND_OVERFLOW_CHECK(fw_param.nit, param->trigger_ignore);
	ASSIGN_AND_OVERFLOW_CHECK(fw_param.tss, param->trigger_shift);

	return fapi_pon_generic_set(ctx,
				    PONFW_DEBUG_TRACE_CONFIG_CMD_ID,
				    &fw_param,
				    sizeof(struct ponfw_debug_trace_config));
}

static enum fapi_pon_errorcode pon_debug_trace_cfg_get_copy(struct pon_ctx *ctx,
							    const void *data,
							    size_t data_size,
							    void *priv)
{
	enum fapi_pon_errorcode ret;
	const struct ponfw_debug_trace_config *src_param = data;
	struct pon_debug_trace_cfg *dst_param = priv;

	UNUSED(ctx);

	ret = integrity_check(dst_param, sizeof(*src_param), data_size);
	if (ret != PON_STATUS_OK)
		return ret;

	memset(dst_param, 0x0, sizeof(*dst_param));

	dst_param->suppress_trigger = src_param->st;
	dst_param->assume_valid = src_param->av;
	dst_param->trigger_source = src_param->src;
	dst_param->samples = src_param->pocs;
	dst_param->trigger_ignore = src_param->nit;
	dst_param->trigger_shift = src_param->tss;

	return PON_STATUS_OK;
}

enum fapi_pon_errorcode
fapi_pon_debug_trace_cfg_get(struct pon_ctx *ctx,
			     struct pon_debug_trace_cfg *param)
{
	enum fapi_pon_errorcode ret;

	if (pon_mode_check(ctx, MODE_AON))
		return PON_STATUS_OPERATION_MODE_ERR;

	ret = debug_support_check(ctx);
	if (ret != PON_STATUS_OK)
		return ret;

	return fapi_pon_generic_get(ctx,
				    PONFW_DEBUG_TRACE_CONFIG_CMD_ID,
				    NULL,
				    0,
				    &pon_debug_trace_cfg_get_copy,
				    param);
}

static enum fapi_pon_errorcode
pon_debug_trace_status_get_copy(struct pon_ctx *ctx,
				const void *data,
				size_t data_size,
				void *priv)
{
	enum fapi_pon_errorcode ret;
	const struct ponfw_debug_trace_result *src_param = data;
	struct pon_debug_trace_status *dst_param = priv;

	UNUSED(ctx);

	ret = integrity_check(dst_param, sizeof(*src_param), data_size);
	if (ret != PON_STATUS_OK)
		return ret;

	memset(dst_param, 0x0, sizeof(*dst_param));

	dst_param->address = src_param->ats;
	dst_param->sample_cnt = src_param->tns;
	dst_param->trigger_cnt = src_param->tnt;

	return PON_STATUS_OK;
}

enum fapi_pon_errorcode
fapi_pon_debug_trace_status_get(struct pon_ctx *ctx,
				struct pon_debug_trace_status *param)
{
	enum fapi_pon_errorcode ret;

	if (pon_mode_check(ctx, MODE_AON))
		return PON_STATUS_OPERATION_MODE_ERR;

	ret = debug_support_check(ctx);
	if (ret != PON_STATUS_OK)
		return ret;

	return fapi_pon_generic_get(ctx,
				    PONFW_DEBUG_TRACE_RESULT_CMD_ID,
				    NULL,
				    0,
				    &pon_debug_trace_status_get_copy,
				    param);
}

enum fapi_pon_errorcode fapi_pon_debug_trace_start(struct pon_ctx *ctx)
{
	enum fapi_pon_errorcode ret;
	struct ponfw_debug_trace_control fw_param = {0};
	struct pon_debug_trace_run_status status_param;

	if (pon_mode_check(ctx, MODE_AON))
		return PON_STATUS_OPERATION_MODE_ERR;

	ret = debug_support_check(ctx);
	if (ret != PON_STATUS_OK)
		return ret;

	ret = fapi_pon_debug_trace_run_status_get(ctx, &status_param);
	if (ret != PON_STATUS_OK)
		return ret;

	if (status_param.run != 0 || status_param.done != 0)
		return PON_STATUS_TRACE_MODULE_NOT_READY;

	fw_param.run = 1;
	fw_param.done = 0;

	return fapi_pon_generic_set(ctx,
				    PONFW_DEBUG_TRACE_CONTROL_CMD_ID,
				    &fw_param,
				    sizeof(struct ponfw_debug_trace_control));
}

enum fapi_pon_errorcode fapi_pon_debug_trace_stop(struct pon_ctx *ctx)
{
	enum fapi_pon_errorcode ret;
	struct ponfw_debug_trace_control fw_param = {0};

	if (pon_mode_check(ctx, MODE_AON))
		return PON_STATUS_OPERATION_MODE_ERR;

	ret = debug_support_check(ctx);
	if (ret != PON_STATUS_OK)
		return ret;

	fw_param.run = 0;
	fw_param.done = 0;

	return fapi_pon_generic_set(ctx,
				    PONFW_DEBUG_TRACE_CONTROL_CMD_ID,
				    &fw_param,
				    sizeof(struct ponfw_debug_trace_control));
}

static enum fapi_pon_errorcode pon_gtc_debug_config_copy(struct pon_ctx *ctx,
							 const void *data,
							 size_t data_size,
							 void *priv)
{
	enum fapi_pon_errorcode ret;
	const struct ponfw_debug_config *src_param = data;
	struct ponfw_debug_config *dst_param = priv;

	UNUSED(ctx);

	ret = integrity_check(dst_param, sizeof(*src_param), data_size);
	if (ret != PON_STATUS_OK)
		return ret;

	if (memcpy_s(dst_param, sizeof(*dst_param), src_param,
		     sizeof(*src_param))) {
		PON_DEBUG_ERR("memcpy_s failed");
		return PON_STATUS_MEMCPY_ERR;
	}

	return PON_STATUS_OK;
}

static enum fapi_pon_errorcode
	pon_debug_burst_profile_get_copy(struct pon_ctx *ctx,
					 const void *data,
					 size_t data_size,
					 void *priv)
{
	enum fapi_pon_errorcode ret;
	const struct ponfw_burst_profile_status *src_param = data;
	struct pon_debug_burst_profile *dst_param = priv;

	UNUSED(ctx);

	ret = integrity_check(dst_param, sizeof(*src_param), data_size);
	if (ret != PON_STATUS_OK)
		return ret;

	memset(dst_param, 0x0, sizeof(*dst_param));

	dst_param->version = src_param->version;
	dst_param->us_fec = src_param->fec;
	dst_param->delimiter_length = src_param->del_len;
	dst_param->delimiter_pattern[0] = src_param->del_pat7;
	dst_param->delimiter_pattern[1] = src_param->del_pat6;
	dst_param->delimiter_pattern[2] = src_param->del_pat5;
	dst_param->delimiter_pattern[3] = src_param->del_pat4;
	dst_param->delimiter_pattern[4] = src_param->del_pat3;
	dst_param->delimiter_pattern[5] = src_param->del_pat2;
	dst_param->delimiter_pattern[6] = src_param->del_pat1;
	dst_param->delimiter_pattern[7] = src_param->del_pat0;
	dst_param->preamble_length = src_param->pre_len;
	dst_param->preamble_repeat_count = src_param->pre_rep;
	dst_param->preamble_pattern[0] = src_param->pre_pat7;
	dst_param->preamble_pattern[1] = src_param->pre_pat6;
	dst_param->preamble_pattern[2] = src_param->pre_pat5;
	dst_param->preamble_pattern[3] = src_param->pre_pat4;
	dst_param->preamble_pattern[4] = src_param->pre_pat3;
	dst_param->preamble_pattern[5] = src_param->pre_pat2;
	dst_param->preamble_pattern[6] = src_param->pre_pat1;
	dst_param->preamble_pattern[7] = src_param->pre_pat0;
	dst_param->pon_tag[0] = src_param->pon_tag7;
	dst_param->pon_tag[1] = src_param->pon_tag6;
	dst_param->pon_tag[2] = src_param->pon_tag5;
	dst_param->pon_tag[3] = src_param->pon_tag4;
	dst_param->pon_tag[4] = src_param->pon_tag3;
	dst_param->pon_tag[5] = src_param->pon_tag2;
	dst_param->pon_tag[6] = src_param->pon_tag1;
	dst_param->pon_tag[7] = src_param->pon_tag0;

	return PON_STATUS_OK;
}

enum fapi_pon_errorcode
fapi_pon_debug_burst_profile_get(struct pon_ctx *ctx,
				 uint32_t index,
				 uint32_t us_rate,
				 uint32_t dwlch_id,
				 struct pon_debug_burst_profile *param)
{
	uint32_t wl_sel = 0;
	enum fapi_pon_errorcode ret;
	struct ponfw_burst_profile_status fw_param = {0};

	/* XG-PON/XGS-PON/NG-PON2 mode only */
	if (!pon_mode_check(ctx, MODE_987_XGPON |
				 MODE_9807_XGSPON |
				 MODE_989_NGPON2_10G |
				 MODE_989_NGPON2_2G5))
		return PON_STATUS_OPERATION_MODE_ERR;

	ret = debug_support_check(ctx);
	if (ret != PON_STATUS_OK)
		return ret;

	/* Burst profile data rate must be set to 0 or 1 */
	if (us_rate != PONFW_BURST_PROFILE_STATUS_RATE_SLOW &&
	    us_rate != PONFW_BURST_PROFILE_STATUS_RATE_HIGH)
		return PON_STATUS_BURST_PROFILE_RATE_ERR;

	/* Burst profile index value must be in range 0 .. 3 */
	if (index > PON_MAX_BURST_PROFILE_INDEX)
		return PON_STATUS_VALUE_RANGE_ERR;

	/* Shall be selected if multiple wavelengths are supported by the
	 * selected operation mode
	 */
	if (pon_mode_check(ctx, MODE_989_NGPON2_10G | MODE_989_NGPON2_2G5))
		wl_sel = 1;

	ASSIGN_AND_OVERFLOW_CHECK(fw_param.dwlch_id, dwlch_id);
	ASSIGN_AND_OVERFLOW_CHECK(fw_param.wl_sel, wl_sel);
	ASSIGN_AND_OVERFLOW_CHECK(fw_param.rate, us_rate);
	ASSIGN_AND_OVERFLOW_CHECK(fw_param.bp_idx, index);

	return fapi_pon_generic_get(ctx,
				    PONFW_BURST_PROFILE_STATUS_CMD_ID,
				    &fw_param,
				    PONFW_BURST_PROFILE_STATUS_LENR,
				    &pon_debug_burst_profile_get_copy,
				    param);
}

enum fapi_pon_errorcode fapi_pon_loop_cfg_set(struct pon_ctx *ctx,
					      const struct pon_loop_cfg *param)
{
	enum fapi_pon_errorcode ret;
	struct ponfw_debug_config fw_param = {0};

	if (!ctx)
		return PON_STATUS_INPUT_ERR;

	if (pon_mode_check(ctx, MODE_AON))
		return PON_STATUS_OPERATION_MODE_ERR;

	ret = debug_support_check(ctx);
	if (ret != PON_STATUS_OK)
		return ret;

	/*
	 * read the values which will not be
	 * modified to allow read/modify/write
	 */
	ret = fapi_pon_generic_get(ctx,
				   PONFW_DEBUG_CONFIG_CMD_ID,
				   NULL,
				   0,
				   &pon_gtc_debug_config_copy,
				   &fw_param);
	if (ret != PON_STATUS_OK)
		return ret;

	fw_param.pma_igl = param->pma_ingress;
	fw_param.pma_egl = param->pma_egress;
	fw_param.pcs_egl = param->pcs_egress;
	fw_param.mac_igl = param->mac_ingress;

	return fapi_pon_generic_set(ctx,
				    PONFW_DEBUG_CONFIG_CMD_ID,
				    &fw_param,
				    sizeof(struct ponfw_debug_config));
}

static enum fapi_pon_errorcode pon_loop_cfg_get_copy(struct pon_ctx *ctx,
						     const void *data,
						     size_t data_size,
						     void *priv)
{
	enum fapi_pon_errorcode ret;
	const struct ponfw_debug_config *src_param = data;
	struct pon_loop_cfg *dst_param = priv;

	UNUSED(ctx);

	ret = integrity_check(dst_param, sizeof(*src_param), data_size);
	if (ret != PON_STATUS_OK)
		return ret;

	memset(dst_param, 0x0, sizeof(*dst_param));

	dst_param->pma_ingress = src_param->pma_igl;
	dst_param->pma_egress = src_param->pma_egl;
	dst_param->pcs_egress = src_param->pcs_egl;
	dst_param->mac_ingress = src_param->mac_igl;

	return PON_STATUS_OK;
}

enum fapi_pon_errorcode fapi_pon_loop_cfg_get(struct pon_ctx *ctx,
					      struct pon_loop_cfg *param)
{
	enum fapi_pon_errorcode ret;

	if (!ctx)
		return PON_STATUS_INPUT_ERR;

	if (pon_mode_check(ctx, MODE_AON))
		return PON_STATUS_OPERATION_MODE_ERR;

	ret = debug_support_check(ctx);
	if (ret != PON_STATUS_OK)
		return ret;

	return fapi_pon_generic_get(ctx,
				    PONFW_DEBUG_CONFIG_CMD_ID,
				    NULL,
				    0,
				    &pon_loop_cfg_get_copy,
				    param);
}

static enum fapi_pon_errorcode
fapi_pon_debug_rogue_start_stop(struct pon_ctx *ctx, uint8_t state)
{
	enum fapi_pon_errorcode ret;
	struct ponfw_debug_config fw_param = {0};

	if (!ctx)
		return PON_STATUS_INPUT_ERR;

	if (pon_mode_check(ctx, MODE_AON))
		return PON_STATUS_OPERATION_MODE_ERR;

	ret = debug_support_check(ctx);
	if (ret != PON_STATUS_OK)
		return ret;

	/*
	 * read the values which will not be
	 * modified to allow read/modify/write
	 */
	ret = fapi_pon_generic_get(ctx,
				   PONFW_DEBUG_CONFIG_CMD_ID,
				   NULL,
				   0,
				   &pon_gtc_debug_config_copy,
				   &fw_param);

	if (ret != PON_STATUS_OK)
		return ret;

	fw_param.rogue = state;

	return fapi_pon_generic_set(ctx,
				    PONFW_DEBUG_CONFIG_CMD_ID,
				    &fw_param,
				    sizeof(struct ponfw_debug_config));
}

enum fapi_pon_errorcode fapi_pon_debug_rogue_start(struct pon_ctx *ctx)
{
	return fapi_pon_debug_rogue_start_stop(ctx,
					       PONFW_DEBUG_CONFIG_ROGUE_ON);
}

enum fapi_pon_errorcode fapi_pon_debug_rogue_stop(struct pon_ctx *ctx)
{
	return fapi_pon_debug_rogue_start_stop(ctx,
					       PONFW_DEBUG_CONFIG_ROGUE_DIS);
}

static enum fapi_pon_errorcode
fapi_pon_debug_rogue_internal_start_stop(struct pon_ctx *ctx, uint8_t state)
{
	enum fapi_pon_errorcode ret;
	struct ponfw_debug_config fw_param = {0};

	if (!ctx)
		return PON_STATUS_INPUT_ERR;

	if (pon_mode_check(ctx, MODE_AON))
		return PON_STATUS_OPERATION_MODE_ERR;

	ret = debug_support_check(ctx);
	if (ret != PON_STATUS_OK)
		return ret;

	/*
	 * read the values which will not be
	 * modified to allow read/modify/write
	 */
	ret = fapi_pon_generic_get(ctx,
				   PONFW_DEBUG_CONFIG_CMD_ID,
				   NULL,
				   0,
				   &pon_gtc_debug_config_copy,
				   &fw_param);

	if (ret != PON_STATUS_OK)
		return ret;

	fw_param.rogue_int = state;

	return fapi_pon_generic_set(ctx,
				    PONFW_DEBUG_CONFIG_CMD_ID,
				    &fw_param,
				    sizeof(struct ponfw_debug_config));
}

enum fapi_pon_errorcode fapi_pon_debug_rogue_internal_start(struct pon_ctx *ctx)
{
	return fapi_pon_debug_rogue_internal_start_stop(
					ctx,
					PONFW_DEBUG_CONFIG_ROGUE_INT_TEST);
}

enum fapi_pon_errorcode fapi_pon_debug_rogue_internal_stop(struct pon_ctx *ctx)
{
	return fapi_pon_debug_rogue_internal_start_stop(
					ctx,
					PONFW_DEBUG_CONFIG_ROGUE_INT_DIS);
}

enum fapi_pon_errorcode fapi_pon_req_cfg_set(struct pon_ctx *ctx,
					     const struct pon_req_cfg *param)
{
	enum fapi_pon_errorcode ret;
	struct ponfw_debug_config fw_param = {0};

	if (!ctx)
		return PON_STATUS_INPUT_ERR;

	if (pon_mode_check(ctx, MODE_AON))
		return PON_STATUS_OPERATION_MODE_ERR;

	ret = debug_support_check(ctx);
	if (ret != PON_STATUS_OK)
		return ret;

	/*
	 * read the values which will not be
	 * modified to allow read/modify/write
	 */
	ret = fapi_pon_generic_get(ctx,
				   PONFW_DEBUG_CONFIG_CMD_ID,
				   NULL,
				   0,
				   &pon_gtc_debug_config_copy,
				   &fw_param);
	if (ret != PON_STATUS_OK)
		return ret;

	fw_param.req_act = param->active;

	return fapi_pon_generic_set(ctx,
				    PONFW_DEBUG_CONFIG_CMD_ID,
				    &fw_param,
				    sizeof(struct ponfw_debug_config));
}

static enum fapi_pon_errorcode pon_req_cfg_get_copy(struct pon_ctx *ctx,
						    const void *data,
						    size_t data_size,
						    void *priv)
{
	enum fapi_pon_errorcode ret;
	const struct ponfw_debug_config *src_param = data;
	struct pon_req_cfg *dst_param = priv;

	UNUSED(ctx);

	ret = integrity_check(dst_param, sizeof(*src_param), data_size);
	if (ret != PON_STATUS_OK)
		return ret;

	dst_param->active = src_param->req_act;

	return PON_STATUS_OK;
}

enum fapi_pon_errorcode fapi_pon_req_cfg_get(struct pon_ctx *ctx,
					     struct pon_req_cfg *param)
{
	enum fapi_pon_errorcode ret;

	if (!ctx)
		return PON_STATUS_INPUT_ERR;

	if (pon_mode_check(ctx, MODE_AON))
		return PON_STATUS_OPERATION_MODE_ERR;

	ret = debug_support_check(ctx);
	if (ret != PON_STATUS_OK)
		return ret;

	return fapi_pon_generic_get(ctx,
				    PONFW_DEBUG_CONFIG_CMD_ID,
				    NULL,
				    0,
				    &pon_req_cfg_get_copy,
				    param);
}

static enum fapi_pon_errorcode pon_omci_ik_get_copy(struct pon_ctx *ctx,
						    const void *data,
						    size_t data_size,
						    void *priv)
{
	enum fapi_pon_errorcode ret;
	const struct ponfw_xgtc_omci_ik *src_param = data;
	struct pon_omci_ik *dst_param = priv;

	UNUSED(ctx);

	ret = integrity_check(dst_param, sizeof(*src_param), data_size);
	if (ret != PON_STATUS_OK)
		return ret;

	if (!src_param->valid)
		return PON_STATUS_OMCI_IK_ERR;

	memset(dst_param, 0x0, sizeof(*dst_param));

	dst_param->key[0] = (src_param->omci_ik4 & 0xFF000000) >> 24;
	dst_param->key[1] = (src_param->omci_ik4 & 0xFF0000) >> 16;
	dst_param->key[2] = (src_param->omci_ik4 & 0xFF00) >> 8;
	dst_param->key[3] = src_param->omci_ik4 & 0xFF;
	dst_param->key[4] = (src_param->omci_ik3 & 0xFF000000) >> 24;
	dst_param->key[5] = (src_param->omci_ik3 & 0xFF0000) >> 16;
	dst_param->key[6] = (src_param->omci_ik3 & 0xFF00) >> 8;
	dst_param->key[7] = src_param->omci_ik3 & 0xFF;
	dst_param->key[8] = (src_param->omci_ik2 & 0xFF000000) >> 24;
	dst_param->key[9] = (src_param->omci_ik2 & 0xFF0000) >> 16;
	dst_param->key[10] = (src_param->omci_ik2 & 0xFF00) >> 8;
	dst_param->key[11] = src_param->omci_ik2 & 0xFF;
	dst_param->key[12] = (src_param->omci_ik1 & 0xFF000000) >> 24;
	dst_param->key[13] = (src_param->omci_ik1 & 0xFF0000) >> 16;
	dst_param->key[14] = (src_param->omci_ik1 & 0xFF00) >> 8;
	dst_param->key[15] = src_param->omci_ik1 & 0xFF;

	return PON_STATUS_OK;
}

enum fapi_pon_errorcode fapi_pon_omci_ik_get(struct pon_ctx *ctx,
					     struct pon_omci_ik *param)
{
	/* XG-PON/XGS-PON/NG-PON2 mode only */
	if (!pon_mode_check(ctx, MODE_987_XGPON |
				 MODE_9807_XGSPON |
				 MODE_989_NGPON2_10G |
				 MODE_989_NGPON2_2G5))
		return PON_STATUS_OPERATION_MODE_ERR;

	return fapi_pon_generic_get(ctx,
				    PONFW_XGTC_OMCI_IK_CMD_ID,
				    NULL,
				    0,
				    &pon_omci_ik_get_copy,
				    param);
}

enum fapi_pon_errorcode
fapi_pon_psm_cfg_set(struct pon_ctx *ctx, const struct pon_psm_cfg *param)
{
	struct ponfw_psm_config fw_param = {0};

	if (!param)
		return PON_STATUS_INPUT_ERR;

	if (!pon_mode_check(ctx, MODE_ITU_PON))
		return PON_STATUS_OPERATION_MODE_ERR;

	/* Map fapi values to firmware values */
	switch (param->mode) {
	case PON_PSM_CONFIG_MODE_DOZE:
		fw_param.mode = PONFW_PSM_CONFIG_MODE_DOZE;
		fw_param.max_cyl_int = param->max_doze_interval;
		break;
	case PON_PSM_CONFIG_MODE_CSL:
		fw_param.mode = PONFW_PSM_CONFIG_MODE_CSL;
		fw_param.max_cyl_int = param->max_rx_off_interval;
		break;
	case PON_PSM_CONFIG_MODE_WSL:
		fw_param.mode = PONFW_PSM_CONFIG_MODE_WSL;
		fw_param.max_cyl_int = param->max_rx_off_interval;
		break;
	default:
		/* Enable of PSM is not allowed
		 * when no valid mode is set
		 */
		if (param->enable)
			return PON_STATUS_OPERATION_MODE_ERR;
		break;
	}

	fw_param.en = param->enable ? 1 : 0;
	fw_param.max_slp_int = param->max_sleep_interval;
	fw_param.min_aw_int = param->min_aware_interval;
	fw_param.min_act_int = param->min_active_held_interval;

	return fapi_pon_generic_set(ctx,
				    PONFW_PSM_CONFIG_CMD_ID,
				    &fw_param,
				    sizeof(struct ponfw_psm_config));
}

static enum fapi_pon_errorcode pon_psm_cfg_get_copy(struct pon_ctx *ctx,
						    const void *data,
						    size_t data_size,
						    void *priv)
{
	enum fapi_pon_errorcode ret;
	const struct ponfw_psm_config *src_param = data;
	struct pon_psm_cfg *dst_param = priv;

	UNUSED(ctx);

	ret = integrity_check(dst_param, sizeof(*src_param), data_size);
	if (ret != PON_STATUS_OK)
		return ret;

	memset(dst_param, 0x0, sizeof(*dst_param));

	dst_param->enable = src_param->en;

	/* Map fapi values to firmware values */
	switch (src_param->mode) {
	case PONFW_PSM_CONFIG_MODE_DOZE:
		dst_param->mode = PON_PSM_CONFIG_MODE_DOZE;
		dst_param->max_doze_interval = src_param->max_cyl_int;
		break;
	case PONFW_PSM_CONFIG_MODE_CSL:
		dst_param->mode = PON_PSM_CONFIG_MODE_CSL;
		dst_param->max_rx_off_interval = src_param->max_cyl_int;
		break;
	case PONFW_PSM_CONFIG_MODE_WSL:
		dst_param->mode = PON_PSM_CONFIG_MODE_WSL;
		dst_param->max_rx_off_interval = src_param->max_cyl_int;
		break;
	default:
		break;
	}

	dst_param->max_sleep_interval = src_param->max_slp_int;
	dst_param->min_aware_interval = src_param->min_aw_int;
	dst_param->min_active_held_interval = src_param->min_act_int;

	return PON_STATUS_OK;
}

enum fapi_pon_errorcode
fapi_pon_psm_cfg_get(struct pon_ctx *ctx, struct pon_psm_cfg *param)
{
	if (!pon_mode_check(ctx, MODE_ITU_PON))
		return PON_STATUS_OPERATION_MODE_ERR;

	return fapi_pon_generic_get(ctx,
				    PONFW_PSM_CONFIG_CMD_ID,
				    NULL,
				    0,
				    &pon_psm_cfg_get_copy,
				    param);
}

static enum fapi_pon_errorcode pon_psm_time_get_copy(struct pon_ctx *ctx,
						     const void *data,
						     size_t data_size,
						     void *priv)
{
	enum fapi_pon_errorcode ret;
	const struct ponfw_psm_status *src_param = data;
	struct pon_psm_fsm_time *dst_param = priv;

	UNUSED(ctx);

	ret = integrity_check(dst_param, sizeof(*src_param), data_size);
	if (ret != PON_STATUS_OK)
		return ret;

	memset(dst_param, 0x0, sizeof(*dst_param));

	dst_param->state_idle = src_param->idle;
	dst_param->state_active = src_param->act;
	dst_param->state_active_held = src_param->act_held;
	dst_param->state_active_free = src_param->act_free;
	dst_param->state_asleep = src_param->asleep;
	dst_param->state_listen = src_param->listen;
	dst_param->state_watch = src_param->watch;
	dst_param->state_doze_aware = src_param->doze_aware;
	dst_param->state_watch_aware = src_param->watch_aware;
	dst_param->state_sleep_aware = src_param->sleep_aware;
	dst_param->state_wait = src_param->wait;

	return PON_STATUS_OK;
}

enum fapi_pon_errorcode
fapi_pon_psm_time_get(struct pon_ctx *ctx,
		      struct pon_psm_fsm_time *param)
{
	if (!pon_mode_check(ctx, MODE_ITU_PON))
		return PON_STATUS_OPERATION_MODE_ERR;

	return fapi_pon_generic_get(ctx,
				    PONFW_PSM_STATUS_CMD_ID,
				    NULL,
				    0,
				    &pon_psm_time_get_copy,
				    param);
}

enum fapi_pon_errorcode fapi_pon_psm_enable(struct pon_ctx *ctx)
{
	struct pon_psm_cfg psm_cfg = { 0 };
	enum fapi_pon_errorcode ret;

	if (!pon_mode_check(ctx, MODE_ITU_PON))
		return PON_STATUS_OPERATION_MODE_ERR;

	ret = fapi_pon_psm_cfg_get(ctx, &psm_cfg);

	if (ret != PON_STATUS_OK)
		return ret;

	psm_cfg.enable = 1;

	return fapi_pon_psm_cfg_set(ctx, &psm_cfg);
}

enum fapi_pon_errorcode fapi_pon_psm_disable(struct pon_ctx *ctx)
{
	struct pon_psm_cfg psm_cfg = { 0 };
	enum fapi_pon_errorcode ret;

	if (!pon_mode_check(ctx, MODE_ITU_PON))
		return PON_STATUS_OPERATION_MODE_ERR;

	ret = fapi_pon_psm_cfg_get(ctx, &psm_cfg);

	if (ret != PON_STATUS_OK)
		return ret;

	psm_cfg.enable = 0;

	return fapi_pon_psm_cfg_set(ctx, &psm_cfg);
}

static enum fapi_pon_errorcode pon_psm_counters_get_copy(struct pon_ctx *ctx,
							 const void *data,
							 size_t data_size,
							 void *priv)
{
	enum fapi_pon_errorcode ret;
	const struct ponfw_psm_counters *src_param = data;
	struct pon_psm_counters *dst_param = priv;

	UNUSED(ctx);

	ret = integrity_check(dst_param, sizeof(*src_param), data_size);
	if (ret != PON_STATUS_OK)
		return ret;

	dst_param->doze_time = (uint64_t)src_param->doze_hi << 32;
	dst_param->doze_time |= (uint64_t)src_param->doze_lo;
	dst_param->cyclic_sleep_time = (uint64_t)src_param->cyclic_hi << 32;
	dst_param->cyclic_sleep_time |= (uint64_t)src_param->cyclic_lo;
	dst_param->watchful_sleep_time = (uint64_t)src_param->watchful_hi << 32;
	dst_param->watchful_sleep_time |= (uint64_t)src_param->watchful_lo;

	return PON_STATUS_OK;
}

enum fapi_pon_errorcode
fapi_pon_psm_counters_get(struct pon_ctx *ctx, struct pon_psm_counters *param)
{
	if (!pon_mode_check(ctx, MODE_ITU_PON))
		return PON_STATUS_OPERATION_MODE_ERR;

	return fapi_pon_generic_get(ctx,
				    PONFW_PSM_COUNTERS_CMD_ID,
				    NULL,
				    0,
				    &pon_psm_counters_get_copy,
				    param);
}

static enum fapi_pon_errorcode
	pon_psm_enable_state_get_copy(struct pon_ctx *ctx,
				      const void *data,
				      size_t data_size,
				      void *priv)
{
	enum fapi_pon_errorcode ret;
	const struct ponfw_onu_status *src_param = data;
	struct pon_psm_state *dst_param = priv;

	UNUSED(ctx);

	ret = integrity_check(dst_param, sizeof(*src_param), data_size);
	if (ret != PON_STATUS_OK)
		return ret;

	memset(dst_param, 0x0, sizeof(*dst_param));

	/* It is possible because PSM states in both structures are
	 * in the same order.
	 */
	dst_param->current = src_param->psm_stat;

	return PON_STATUS_OK;
}

enum fapi_pon_errorcode fapi_pon_psm_state_get(struct pon_ctx *ctx,
					       struct pon_psm_state *param)
{
	enum fapi_pon_errorcode ret;
	struct pon_psm_cfg psm_cfg = {0};
	struct pon_ploam_state ploam_state = {0};

	if (!param)
		return PON_STATUS_INPUT_ERR;

	if (!pon_mode_check(ctx, MODE_ITU_PON))
		return PON_STATUS_OPERATION_MODE_ERR;

	ret = fapi_pon_psm_cfg_get(ctx, &psm_cfg);
	if (ret != PON_STATUS_OK)
		return ret;

	if (psm_cfg.enable == PONFW_PSM_CONFIG_EN_EN) {
		return fapi_pon_generic_get(ctx,
					    PONFW_ONU_STATUS_CMD_ID,
					    NULL,
					    0,
					    &pon_psm_enable_state_get_copy,
					    param);
	}

	ret = fapi_pon_ploam_state_get(ctx, &ploam_state);
	if (ret != PON_STATUS_OK)
		return ret;

	if (ploam_state.current == 50)
		param->current = PSM_STATE_ACTIVE;
	else
		param->current = PSM_STATE_IDLE;

	return PON_STATUS_OK;
}

enum fapi_pon_errorcode fapi_pon_synce_cfg_set(struct pon_ctx *ctx,
					const struct pon_synce_cfg *param)
{
	struct ponfw_synce_config fw_param = {0};

	if (!ctx)
		return PON_STATUS_INPUT_ERR;

	if (!is_plat_type_any_of(ctx, PONFW_SYNCE_CONFIG_PLAT_COMPATIBILITY))
		return PON_STATUS_SUPPORT;

	ASSIGN_AND_OVERFLOW_CHECK(fw_param.loop_mode, param->loop_mode);
	ASSIGN_AND_OVERFLOW_CHECK(fw_param.synce_mode, param->synce_mode);
	ASSIGN_AND_OVERFLOW_CHECK(fw_param.synce_src, param->src);
	ASSIGN_AND_OVERFLOW_CHECK(fw_param.synce_frq, param->frq);
	ASSIGN_AND_OVERFLOW_CHECK(fw_param.n0, param->n0_avg);
	ASSIGN_AND_OVERFLOW_CHECK(fw_param.ddt, param->ddt);
	ASSIGN_AND_OVERFLOW_CHECK(fw_param.lim_thr, param->lim_thr);
	ASSIGN_AND_OVERFLOW_CHECK(fw_param.b0, (param->iir & 0xf0000000) >> 28);
	ASSIGN_AND_OVERFLOW_CHECK(fw_param.b1, (param->iir & 0x0f000000) >> 24);
	ASSIGN_AND_OVERFLOW_CHECK(fw_param.b2, (param->iir & 0x00f00000) >> 20);
	ASSIGN_AND_OVERFLOW_CHECK(fw_param.b3, (param->iir & 0x000f0000) >> 16);
	ASSIGN_AND_OVERFLOW_CHECK(fw_param.b4, (param->iir & 0x0000f000) >> 12);
	ASSIGN_AND_OVERFLOW_CHECK(fw_param.b5, (param->iir & 0x00000f00) >> 8);
	ASSIGN_AND_OVERFLOW_CHECK(fw_param.b6, (param->iir & 0x000000f0) >> 4);
	ASSIGN_AND_OVERFLOW_CHECK(fw_param.b7, (param->iir & 0x0000000f));

	return fapi_pon_generic_set(ctx,
				    PONFW_SYNCE_CONFIG_CMD_ID,
				    &fw_param,
				    sizeof(struct ponfw_synce_config));
}

static enum fapi_pon_errorcode pon_synce_cfg_get_copy(struct pon_ctx *ctx,
						      const void *data,
						      size_t data_size,
						      void *priv)
{
	enum fapi_pon_errorcode ret;
	const struct ponfw_synce_config *src_param = data;
	struct pon_synce_cfg *dst_param = priv;

	UNUSED(ctx);

	ret = integrity_check(dst_param, sizeof(*src_param), data_size);
	if (ret != PON_STATUS_OK)
		return ret;

	dst_param->synce_mode = src_param->synce_mode;
	dst_param->loop_mode = src_param->loop_mode;
	dst_param->src = src_param->synce_src;
	dst_param->frq = src_param->synce_frq;
	dst_param->n0_avg = src_param->n0;
	dst_param->ddt = src_param->ddt;
	dst_param->lim_thr = src_param->lim_thr;
	dst_param->iir = 0;

	dst_param->iir = (dst_param->iir | src_param->b0) << 4;
	dst_param->iir = (dst_param->iir | src_param->b1) << 4;
	dst_param->iir = (dst_param->iir | src_param->b2) << 4;
	dst_param->iir = (dst_param->iir | src_param->b3) << 4;
	dst_param->iir = (dst_param->iir | src_param->b4) << 4;
	dst_param->iir = (dst_param->iir | src_param->b5) << 4;
	dst_param->iir = (dst_param->iir | src_param->b6) << 4;
	dst_param->iir = (dst_param->iir | src_param->b7);

	return ret;
}

enum fapi_pon_errorcode fapi_pon_synce_cfg_get(struct pon_ctx *ctx,
					struct pon_synce_cfg *param)
{
	if (!ctx)
		return PON_STATUS_INPUT_ERR;

	if (!is_plat_type_any_of(ctx, PONFW_SYNCE_CONFIG_PLAT_COMPATIBILITY))
		return PON_STATUS_SUPPORT;

	return fapi_pon_generic_get(ctx,
				    PONFW_SYNCE_CONFIG_CMD_ID,
				    NULL,
				    0,
				    &pon_synce_cfg_get_copy,
				    param);
}

static enum fapi_pon_errorcode pon_synce_status_get_copy(struct pon_ctx *ctx,
							 const void *data,
							 size_t data_size,
							 void *priv)
{
	enum fapi_pon_errorcode ret;
	const struct ponfw_synce_status *src_param = data;
	struct pon_synce_status *dst_param = priv;

	UNUSED(ctx);

	ret = integrity_check(dst_param, sizeof(*src_param), data_size);
	if (ret != PON_STATUS_OK)
		return ret;

	dst_param->stat = src_param->synce_stat;

	return ret;
}

enum fapi_pon_errorcode fapi_pon_synce_status_get(struct pon_ctx *ctx,
					struct pon_synce_status *param)
{
	if (!ctx)
		return PON_STATUS_INPUT_ERR;

	if (!is_plat_type_any_of(ctx, PONFW_SYNCE_STATUS_PLAT_COMPATIBILITY))
		return PON_STATUS_SUPPORT;

	return fapi_pon_generic_get(ctx,
				    PONFW_SYNCE_STATUS_CMD_ID,
				    NULL,
				    0,
				    &pon_synce_status_get_copy,
				    param);
}

static enum fapi_pon_errorcode fapi_pon_synce_en_dis(struct pon_ctx *ctx,
						     uint8_t state)
{
	struct ponfw_synce_control fw_param = {0};

	if (!ctx)
		return PON_STATUS_INPUT_ERR;

	fw_param.synce_en = state;
	fw_param.mask_en = PONFW_SYNCE_CONTROL_MASK_EN_EN;

	return fapi_pon_generic_set(ctx,
				    PONFW_SYNCE_CONTROL_CMD_ID,
				    &fw_param,
				    sizeof(struct ponfw_synce_control));
}

enum fapi_pon_errorcode fapi_pon_synce_enable(struct pon_ctx *ctx)
{
	return fapi_pon_synce_en_dis(ctx, PONFW_SYNCE_CONTROL_SYNCE_EN_EN);
}

enum fapi_pon_errorcode fapi_pon_synce_disable(struct pon_ctx *ctx)
{
	return fapi_pon_synce_en_dis(ctx, PONFW_SYNCE_CONTROL_SYNCE_EN_DIS);
}

static enum fapi_pon_errorcode fapi_pon_synce_hold_en_dis(struct pon_ctx *ctx,
							  uint8_t state)
{
	struct ponfw_synce_control fw_param = {0};

	if (!ctx)
		return PON_STATUS_INPUT_ERR;

	fw_param.force_hold = state;
	fw_param.mask_force = PONFW_SYNCE_CONTROL_MASK_FORCE_EN;

	return fapi_pon_generic_set(ctx,
				    PONFW_SYNCE_CONTROL_CMD_ID,
				    &fw_param,
				    sizeof(struct ponfw_synce_control));
}

enum fapi_pon_errorcode fapi_pon_synce_hold_enable(struct pon_ctx *ctx)
{
	return fapi_pon_synce_hold_en_dis(ctx,
					  PONFW_SYNCE_CONTROL_FORCE_HOLD_EN);
}

enum fapi_pon_errorcode fapi_pon_synce_hold_disable(struct pon_ctx *ctx)
{
	return fapi_pon_synce_hold_en_dis(ctx,
					  PONFW_SYNCE_CONTROL_FORCE_HOLD_DIS);
}

enum fapi_pon_errorcode fapi_pon_xgem_key_cfg_set(struct pon_ctx *ctx,
					const struct pon_xgem_key *param)
{
	enum fapi_pon_errorcode ret;
	struct ponfw_xgem_key_write fw_param = {0};

	if (!pon_mode_check(ctx, MODE_ITU_PON))
		return PON_STATUS_OPERATION_MODE_ERR;

	ret = debug_support_check(ctx);
	if (ret != PON_STATUS_OK)
		return ret;

	if (!param || param->size != PON_MAXIMUM_XGEM_KEY_SIZE_BY_4)
		return PON_STATUS_VALUE_RANGE_ERR;

	ASSIGN_AND_OVERFLOW_CHECK(fw_param.key_idx, param->index);
	ASSIGN_AND_OVERFLOW_CHECK(fw_param.key1, param->key[0]);
	ASSIGN_AND_OVERFLOW_CHECK(fw_param.key2, param->key[1]);
	ASSIGN_AND_OVERFLOW_CHECK(fw_param.key3, param->key[2]);
	ASSIGN_AND_OVERFLOW_CHECK(fw_param.key4, param->key[3]);

	return fapi_pon_generic_set(ctx,
				    PONFW_XGEM_KEY_WRITE_CMD_ID,
				    &fw_param,
				    sizeof(struct ponfw_xgem_key_write));
}

enum fapi_pon_errorcode fapi_pon_optic_cfg_set(struct pon_ctx *ctx,
					      const struct pon_optic_cfg *param)
{
	struct ponfw_onu_optic_config fw_param = {0};
	enum fapi_pon_errorcode ret;
	struct nl_msg *msg;
	int err;
	int clock_cycle;

	if (!ctx)
		return PON_STATUS_INPUT_ERR;

	if (pon_mode_check(ctx, MODE_AON))
		return PON_STATUS_OPERATION_MODE_ERR;

	clock_cycle = get_clock_cycle(ctx);

	if (!clock_cycle) {
		PON_DEBUG_ERR("Can't get clock cycle");
		return PON_STATUS_ERR;
	}

	ASSIGN_AND_OVERFLOW_CHECK(fw_param.laser_lead,
			(param->laser_setup_time / clock_cycle));
	ASSIGN_AND_OVERFLOW_CHECK(fw_param.laser_lag,
			(param->laser_hold_time / clock_cycle));
	ASSIGN_AND_OVERFLOW_CHECK(fw_param.serdes_lead,
			(param->serdes_setup_time / clock_cycle));
	ASSIGN_AND_OVERFLOW_CHECK(fw_param.serdes_lag,
			(param->serdes_hold_time / clock_cycle));
	ASSIGN_AND_OVERFLOW_CHECK(fw_param.bias_rd,
			(param->bias_setup_time / clock_cycle));
	ASSIGN_AND_OVERFLOW_CHECK(fw_param.bias_fd,
			(param->bias_hold_time / clock_cycle));
	ASSIGN_AND_OVERFLOW_CHECK(fw_param.bpp, param->burst_idle_pattern);
	ASSIGN_AND_OVERFLOW_CHECK(fw_param.env_sel, param->burst_en_mode);
	ASSIGN_AND_OVERFLOW_CHECK(fw_param.tx_pup_mode, param->tx_pup_mode);
	ASSIGN_AND_OVERFLOW_CHECK(fw_param.tx_bias_mode, param->tx_en_mode);
	ASSIGN_AND_OVERFLOW_CHECK(fw_param.los_sd, param->sd_polarity);
	ASSIGN_AND_OVERFLOW_CHECK(fw_param.loop_ps_en,
				  param->loop_timing_power_save);
	ASSIGN_AND_OVERFLOW_CHECK(fw_param.pse_en, param->pse_en);
	fw_param.re = param->rogue_auto_en;
	fw_param.rogue_lead = param->rogue_lead_time * 1000 / clock_cycle;
	fw_param.rogue_lag = param->rogue_lag_time * 1000 / clock_cycle;
	fw_param.opt_tx_sd_pol = param->opt_tx_sd_pol;

	/* Always enable LOS output, will be gated by pinctrl */
	fw_param.los_sd_en = 1;

	ret = fapi_pon_msg_prepare(&ctx, &msg, PON_MBOX_C_LT_CONFIG);

	if (ret != PON_STATUS_OK)
		return ret;

	err = nla_put_u8(msg, PON_MBOX_LT_POWER_SAVE,
			 param->loop_timing_power_save);
	if (err) {
		PON_DEBUG_ERR("Can't add netlink attribute");
		nlmsg_free(msg);
		return PON_STATUS_NL_ERR;
	}

	err = nl_send_auto_complete(ctx->nls, msg);
	nlmsg_free(msg);
	if (err < 0) {
		PON_DEBUG_ERR("Can't send netlink message: %i", err);
		return PON_STATUS_NL_ERR;
	}

	return fapi_pon_generic_set(ctx,
				    PONFW_ONU_OPTIC_CONFIG_CMD_ID,
				    &fw_param,
				    sizeof(struct ponfw_onu_optic_config));
}

static enum fapi_pon_errorcode pon_optic_cfg_get_copy(struct pon_ctx *ctx,
						     const void *data,
						     size_t data_size,
						     void *priv)
{
	enum fapi_pon_errorcode ret;
	const struct ponfw_onu_optic_config *src_param = data;
	struct pon_optic_cfg *dst_param = priv;
	int clock_cycle;

	UNUSED(ctx);

	ret = integrity_check(dst_param, sizeof(*src_param), data_size);
	if (ret != PON_STATUS_OK)
		return ret;

	clock_cycle = get_clock_cycle(ctx);

	if (!clock_cycle) {
		PON_DEBUG_ERR("Can't get clock cycle");
		return PON_STATUS_ERR;
	}

	dst_param->laser_setup_time = src_param->laser_lead * clock_cycle;
	dst_param->laser_hold_time = src_param->laser_lag * clock_cycle;
	dst_param->serdes_setup_time = src_param->serdes_lead * clock_cycle;
	dst_param->serdes_hold_time = src_param->serdes_lag * clock_cycle;
	dst_param->bias_setup_time = src_param->bias_rd * clock_cycle;
	dst_param->bias_hold_time = src_param->bias_fd * clock_cycle;
	dst_param->burst_idle_pattern = src_param->bpp;
	dst_param->burst_en_mode = src_param->env_sel;
	dst_param->tx_pup_mode = src_param->tx_pup_mode;
	dst_param->tx_en_mode = src_param->tx_bias_mode;
	dst_param->sd_polarity = src_param->los_sd;
	dst_param->loop_timing_power_save = src_param->loop_ps_en;
	dst_param->pse_en = src_param->pse_en;
	dst_param->rogue_auto_en = src_param->re;
	dst_param->rogue_lead_time = (uint32_t)src_param->rogue_lead *
				     clock_cycle / 1000;
	dst_param->rogue_lag_time = (uint32_t)src_param->rogue_lag *
				    clock_cycle / 1000;
	dst_param->opt_tx_sd_pol = src_param->opt_tx_sd_pol;

	return PON_STATUS_OK;
}

enum fapi_pon_errorcode fapi_pon_optic_cfg_get(struct pon_ctx *ctx,
					      struct pon_optic_cfg *param)
{
	if (!ctx)
		return PON_STATUS_INPUT_ERR;

	if (pon_mode_check(ctx, MODE_AON))
		return PON_STATUS_OPERATION_MODE_ERR;

	return fapi_pon_generic_get(ctx,
				    PONFW_ONU_OPTIC_CONFIG_CMD_ID,
				    NULL,
				    0,
				    &pon_optic_cfg_get_copy,
				    param);
}

static enum fapi_pon_errorcode
pon_serdes_cfg_decode(struct pon_ctx *ctx,
			   struct nlattr **attrs,
			   void *priv)
{
	struct pon_serdes_cfg *dst_param = priv;
	struct nlattr *cfg[PON_MBOX_SRDS_MAX + 1];

	UNUSED(ctx);

	memset(dst_param, 0, sizeof(*dst_param));

	if (!attrs[PON_MBOX_A_SRDS_READ])
		return PON_STATUS_ERR;

	if (nla_parse_nested(cfg, PON_MBOX_SRDS_MAX,
			     attrs[PON_MBOX_A_SRDS_READ],
			     serdes_config_policy) < 0)
		return PON_STATUS_ERR;

	SRDS_DECODE_U8(TX_EQ_MAIN, tx_eq_main);
	SRDS_DECODE_U8(TX_EQ_POST, tx_eq_post);
	SRDS_DECODE_U8(TX_EQ_PRE, tx_eq_pre);
	SRDS_DECODE_U8(VBOOST_EN, vboost_en);
	SRDS_DECODE_U8(VBOOST_LVL, vboost_lvl);
	SRDS_DECODE_U8(IBOOST_LVL, iboost_lvl);
	SRDS_DECODE_U8(RX_ADAPT_AFE_EN, rx_adapt_afe_en);
	SRDS_DECODE_U8(RX_ADAPT_DFE_EN, rx_adapt_dfe_en);
	SRDS_DECODE_U8(RX_ADAPT_CONT, rx_adapt_cont);
	SRDS_DECODE_U8(RX_ADAPT_EN, rx_adapt_en);
	SRDS_DECODE_U8(RX_EQ_ATT_LVL, rx_eq_att_lvl);
	SRDS_DECODE_U8(RX_EQ_ADAPT_MODE, rx_eq_adapt_mode);
	SRDS_DECODE_U8(RX_EQ_ADAPT_SEL, rx_eq_adapt_sel);
	SRDS_DECODE_U8(RX_EQ_CTLE_BOOST, rx_eq_ctle_boost);
	SRDS_DECODE_U8(RX_VCO_TEMP_COMP_EN, rx_vco_temp_comp_en);
	SRDS_DECODE_U8(RX_VCO_STEP_CTRL, rx_vco_step_ctrl);
	SRDS_DECODE_U8(RX_VCO_FRQBAND, rx_vco_frqband);
	SRDS_DECODE_U8(RX_MISC, rx_misc);
	SRDS_DECODE_U8(RX_DELTA_IQ, rx_delta_iq);
	SRDS_DECODE_U8(RX_MARGIN_IQ, rx_margin_iq);
	SRDS_DECODE_U8(EQ_CTLE_POLE, rx_eq_ctle_pole);
	SRDS_DECODE_U8(EQ_DFE_TAP1, rx_eq_dfe_tap1);
	SRDS_DECODE_U8(EQ_DFE_BYPASS, rx_eq_dfe_bypass);
	SRDS_DECODE_U8(EQ_VGA1_GAIN, rx_eq_vga1_gain);
	SRDS_DECODE_U8(EQ_VGA2_GAIN, rx_eq_vga2_gain);

	return PON_STATUS_OK;
}

static enum fapi_pon_errorcode pon_serdes_cfg_get_copy(struct pon_ctx *ctx,
						       const void *data,
						       size_t data_size,
						       void *priv)
{
	enum fapi_pon_errorcode ret;
	const struct ponfw_serdes_config *src_param = data;
	struct pon_serdes_cfg *dst_param = priv;

	UNUSED(ctx);

	ret = integrity_check(dst_param, sizeof(*src_param), data_size);
	if (ret != PON_STATUS_OK)
		return ret;

	dst_param->rx_adapt_en = src_param->rx_adapt_en;

	return ret;
}

enum fapi_pon_errorcode fapi_pon_serdes_cfg_get(struct pon_ctx *ctx,
						struct pon_serdes_cfg *param)
{
	struct read_cmd_cb cb_data;
	struct nl_msg *msg;
	enum fapi_pon_errorcode ret;
	uint32_t seq = NL_AUTO_SEQ;

	if (!ctx || !param)
		return PON_STATUS_INPUT_ERR;

	if (pon_mode_check(ctx, MODE_AON))
		return PON_STATUS_OPERATION_MODE_ERR;

	/*  Read parameters used inside mbox driver */
	ret = fapi_pon_nl_msg_prepare_decode(ctx, &msg, &cb_data, &seq,
					     &pon_serdes_cfg_decode,
					     NULL,
					     param,
					     PON_MBOX_C_SRDS_CONFIG_READ);
	if (ret != PON_STATUS_OK)
		return ret;

	ret = fapi_pon_nl_msg_send(ctx, &msg, &cb_data, &seq);

	if (ret != PON_STATUS_OK)
		return ret;

	/*  Read parameter rx_adapt_en used by FW */
	return fapi_pon_generic_get(ctx,
				    PONFW_SERDES_CONFIG_CMD_ID,
				    NULL,
				    0,
				    &pon_serdes_cfg_get_copy,
				    param);
}

enum fapi_pon_errorcode fapi_pon_serdes_cfg_set(struct pon_ctx *ctx,
					const struct pon_serdes_cfg *param)
{
	struct ponfw_serdes_config fw_param = {0};
	enum fapi_pon_errorcode ret;
	struct nl_msg *msg;
	int err;

	if (!ctx)
		return PON_STATUS_INPUT_ERR;

	if (pon_mode_check(ctx, MODE_AON))
		return PON_STATUS_OPERATION_MODE_ERR;

	fw_param.rx_adapt_en = param->rx_adapt_en;

	ret = fapi_pon_msg_prepare(&ctx, &msg, PON_MBOX_C_SRDS_CONFIG);
	if (ret != PON_STATUS_OK)
		return ret;

	err = nla_put_u8(msg, PON_MBOX_SRDS_TX_EQ_MAIN, param->tx_eq_main);
	if (err) {
		PON_DEBUG_ERR("Can't add netlink attribute");
		nlmsg_free(msg);
		return PON_STATUS_NL_ERR;
	}

	err = nla_put_u8(msg, PON_MBOX_SRDS_TX_EQ_POST, param->tx_eq_post);
	if (err) {
		PON_DEBUG_ERR("Can't add netlink attribute");
		nlmsg_free(msg);
		return PON_STATUS_NL_ERR;
	}

	err = nla_put_u8(msg, PON_MBOX_SRDS_TX_EQ_PRE, param->tx_eq_pre);
	if (err) {
		PON_DEBUG_ERR("Can't add netlink attribute");
		nlmsg_free(msg);
		return PON_STATUS_NL_ERR;
	}

	err = nla_put_u8(msg, PON_MBOX_SRDS_VBOOST_EN, param->vboost_en);
	if (err) {
		PON_DEBUG_ERR("Can't add netlink attribute");
		nlmsg_free(msg);
		return PON_STATUS_NL_ERR;
	}

	err = nla_put_u8(msg, PON_MBOX_SRDS_VBOOST_LVL, param->vboost_lvl);
	if (err) {
		PON_DEBUG_ERR("Can't add netlink attribute");
		nlmsg_free(msg);
		return PON_STATUS_NL_ERR;
	}

	err = nla_put_u8(msg, PON_MBOX_SRDS_IBOOST_LVL, param->iboost_lvl);
	if (err) {
		PON_DEBUG_ERR("Can't add netlink attribute");
		nlmsg_free(msg);
		return PON_STATUS_NL_ERR;
	}

	err = nla_put_u8(msg, PON_MBOX_SRDS_RX_ADAPT_AFE_EN,
			 param->rx_adapt_afe_en);
	if (err) {
		PON_DEBUG_ERR("Can't add netlink attribute");
		nlmsg_free(msg);
		return PON_STATUS_NL_ERR;
	}

	err = nla_put_u8(msg, PON_MBOX_SRDS_RX_ADAPT_DFE_EN,
			 param->rx_adapt_dfe_en);
	if (err) {
		PON_DEBUG_ERR("Can't add netlink attribute");
		nlmsg_free(msg);
		return PON_STATUS_NL_ERR;
	}

	err = nla_put_u8(msg, PON_MBOX_SRDS_RX_ADAPT_CONT,
			 param->rx_adapt_cont);
	if (err) {
		PON_DEBUG_ERR("Can't add netlink attribute");
		nlmsg_free(msg);
		return PON_STATUS_NL_ERR;
	}

	err = nla_put_u8(msg, PON_MBOX_SRDS_RX_ADAPT_EN, param->rx_adapt_en);
	if (err) {
		PON_DEBUG_ERR("Can't add netlink attribute");
		nlmsg_free(msg);
		return PON_STATUS_NL_ERR;
	}

	err = nla_put_u8(msg, PON_MBOX_SRDS_RX_EQ_ATT_LVL,
			 param->rx_eq_att_lvl);
	if (err) {
		PON_DEBUG_ERR("Can't add netlink attribute");
		nlmsg_free(msg);
		return PON_STATUS_NL_ERR;
	}

	err = nla_put_u8(msg, PON_MBOX_SRDS_RX_EQ_ADAPT_MODE,
			 param->rx_eq_adapt_mode);
	if (err) {
		PON_DEBUG_ERR("Can't add netlink attribute");
		nlmsg_free(msg);
		return PON_STATUS_NL_ERR;
	}

	err = nla_put_u8(msg, PON_MBOX_SRDS_RX_EQ_ADAPT_SEL,
			 param->rx_eq_adapt_sel);
	if (err) {
		PON_DEBUG_ERR("Can't add netlink attribute");
		nlmsg_free(msg);
		return PON_STATUS_NL_ERR;
	}

	err = nla_put_u8(msg, PON_MBOX_SRDS_RX_EQ_CTLE_BOOST,
			 param->rx_eq_ctle_boost);
	if (err) {
		PON_DEBUG_ERR("Can't add netlink attribute");
		nlmsg_free(msg);
		return PON_STATUS_NL_ERR;
	}

	err = nla_put_u8(msg, PON_MBOX_SRDS_RX_VCO_TEMP_COMP_EN,
			 param->rx_vco_temp_comp_en);
	if (err) {
		PON_DEBUG_ERR("Can't add netlink attribute");
		nlmsg_free(msg);
		return PON_STATUS_NL_ERR;
	}

	err = nla_put_u8(msg, PON_MBOX_SRDS_RX_VCO_STEP_CTRL,
			 param->rx_vco_step_ctrl);
	if (err) {
		PON_DEBUG_ERR("Can't add netlink attribute");
		nlmsg_free(msg);
		return PON_STATUS_NL_ERR;
	}

	err = nla_put_u8(msg, PON_MBOX_SRDS_RX_VCO_FRQBAND,
			 param->rx_vco_frqband);
	if (err) {
		PON_DEBUG_ERR("Can't add netlink attribute");
		nlmsg_free(msg);
		return PON_STATUS_NL_ERR;
	}

	err = nla_put_u8(msg, PON_MBOX_SRDS_RX_MISC, param->rx_misc);
	if (err) {
		PON_DEBUG_ERR("Can't add netlink attribute");
		nlmsg_free(msg);
		return PON_STATUS_NL_ERR;
	}

	err = nla_put_u8(msg, PON_MBOX_SRDS_RX_DELTA_IQ, param->rx_delta_iq);
	if (err) {
		PON_DEBUG_ERR("Can't add netlink attribute");
		nlmsg_free(msg);
		return PON_STATUS_NL_ERR;
	}

	err = nla_put_u8(msg, PON_MBOX_SRDS_RX_MARGIN_IQ, param->rx_margin_iq);
	if (err) {
		PON_DEBUG_ERR("Can't add netlink attribute");
		nlmsg_free(msg);
		return PON_STATUS_NL_ERR;
	}

	err = nla_put_u8(msg, PON_MBOX_SRDS_EQ_CTLE_POLE,
			 param->rx_eq_ctle_pole);
	if (err) {
		PON_DEBUG_ERR("Can't add netlink attribute");
		nlmsg_free(msg);
		return PON_STATUS_NL_ERR;
	}

	err = nla_put_u8(msg, PON_MBOX_SRDS_EQ_DFE_TAP1,
			 param->rx_eq_dfe_tap1);
	if (err) {
		PON_DEBUG_ERR("Can't add netlink attribute");
		nlmsg_free(msg);
		return PON_STATUS_NL_ERR;
	}

	err = nla_put_u8(msg, PON_MBOX_SRDS_EQ_DFE_BYPASS,
			 param->rx_eq_dfe_bypass);
	if (err) {
		PON_DEBUG_ERR("Can't add netlink attribute");
		nlmsg_free(msg);
		return PON_STATUS_NL_ERR;
	}

	err = nla_put_u8(msg, PON_MBOX_SRDS_EQ_VGA1_GAIN,
			 param->rx_eq_vga1_gain);
	if (err) {
		PON_DEBUG_ERR("Can't add netlink attribute");
		nlmsg_free(msg);
		return PON_STATUS_NL_ERR;
	}

	err = nla_put_u8(msg, PON_MBOX_SRDS_EQ_VGA2_GAIN,
			 param->rx_eq_vga2_gain);
	if (err) {
		PON_DEBUG_ERR("Can't add netlink attribute");
		nlmsg_free(msg);
		return PON_STATUS_NL_ERR;
	}

	err = nl_send_auto_complete(ctx->nls, msg);
	nlmsg_free(msg);
	if (err < 0) {
		PON_DEBUG_ERR("Can't send netlink message: %i", err);
		return PON_STATUS_NL_ERR;
	}

	return fapi_pon_generic_set(ctx,
				    PONFW_SERDES_CONFIG_CMD_ID,
				    &fw_param,
				    sizeof(struct ponfw_serdes_config));
}

enum fapi_pon_errorcode fapi_pon_gpio_cfg_set(struct pon_ctx *ctx,
					      const struct pon_gpio_cfg *param)
{
	struct ponfw_gpio_config fw_param = {0};

	if (!ctx)
		return PON_STATUS_INPUT_ERR;

	if (pon_mode_check(ctx, MODE_AON))
		return PON_STATUS_OPERATION_MODE_ERR;

	fw_param.gpio21_mode = param->gpio21_mode;
	fw_param.gpio24_mode = param->gpio24_mode;
	fw_param.gpio25_mode = param->gpio25_mode;

	return fapi_pon_generic_set(ctx,
				    PONFW_GPIO_CONFIG_CMD_ID,
				    &fw_param,
				    sizeof(struct ponfw_gpio_config));
}

static enum fapi_pon_errorcode pon_gpio_cfg_get_copy(struct pon_ctx *ctx,
						     const void *data,
						     size_t data_size,
						     void *priv)
{
	enum fapi_pon_errorcode ret;
	const struct ponfw_gpio_config *src_param = data;
	struct pon_gpio_cfg *dst_param = priv;

	UNUSED(ctx);

	ret = integrity_check(dst_param, sizeof(*src_param), data_size);
	if (ret != PON_STATUS_OK)
		return ret;

	dst_param->gpio21_mode = src_param->gpio21_mode;
	dst_param->gpio24_mode = src_param->gpio24_mode;
	dst_param->gpio25_mode = src_param->gpio25_mode;

	return ret;
}

enum fapi_pon_errorcode fapi_pon_gpio_cfg_get(struct pon_ctx *ctx,
					      struct pon_gpio_cfg *param)
{
	if (!ctx)
		return PON_STATUS_INPUT_ERR;

	if (pon_mode_check(ctx, MODE_AON))
		return PON_STATUS_OPERATION_MODE_ERR;

	return fapi_pon_generic_get(ctx,
				    PONFW_GPIO_CONFIG_CMD_ID,
				    NULL,
				    0,
				    &pon_gpio_cfg_get_copy,
				    param);
}

enum fapi_pon_errorcode fapi_pon_iop_cfg_set(struct pon_ctx *ctx,
					     const struct pon_iop_cfg *param)
{
	union ponfw_msg fw_param = {0};
	enum fapi_pon_errorcode ret;
	struct nl_msg *msg;
	int err;

	if (!param || !ctx)
		return PON_STATUS_INPUT_ERR;

	if (!pon_mode_check(ctx, MODE_ITU_PON))
		return PON_STATUS_OPERATION_MODE_ERR;

	ret = fapi_pon_msg_prepare(&ctx, &msg, PON_MBOX_C_IOP_CONFIG);
	if (ret != PON_STATUS_OK)
		return ret;

	err = nla_put_u32(msg, PON_MBOX_IOP_MSK, param->iop_mask);
	if (err) {
		PON_DEBUG_ERR("Can't add netlink attribute");
		nlmsg_free(msg);
		return PON_STATUS_NL_ERR;
	}

	err = nl_send_auto_complete(ctx->nls, msg);
	nlmsg_free(msg);
	if (err < 0) {
		PON_DEBUG_ERR("Can't send netlink message: %i", err);
		return PON_STATUS_NL_ERR;
	}

	fw_param.val[0] = param->iop_mask;

	return fapi_pon_generic_set(ctx,
				    PONFW_ONU_INTEROP_CONFIG_CMD_ID,
				    &fw_param,
				    sizeof(struct ponfw_onu_interop_config));
}

static enum fapi_pon_errorcode pon_iop_cfg_get_copy(struct pon_ctx *ctx,
						    const void *data,
						    size_t data_size,
						    void *priv)
{
	const union ponfw_msg *src_param = data;
	struct pon_iop_cfg *dst_param = priv;

	UNUSED(ctx);
	UNUSED(data_size);

	memset(dst_param, 0x0, sizeof(*dst_param));

	dst_param->iop_mask = src_param->val[0];

	return PON_STATUS_OK;
}

enum fapi_pon_errorcode fapi_pon_iop_cfg_get(struct pon_ctx *ctx,
					     struct pon_iop_cfg *param)
{
	if (!pon_mode_check(ctx, MODE_ITU_PON))
		return PON_STATUS_OPERATION_MODE_ERR;

	return fapi_pon_generic_get(ctx,
				    PONFW_ONU_INTEROP_CONFIG_CMD_ID,
				    NULL,
				    0,
				    &pon_iop_cfg_get_copy,
				    param);
}

enum fapi_pon_errorcode fapi_pon_pqsf_cfg_set(struct pon_ctx *ctx,
					      const struct pon_pqsf_cfg *param)
{
	struct ponfw_onu_qos_config fw_param = {0};

	if (!pon_mode_check(ctx, MODE_984_GPON))
		return PON_STATUS_OPERATION_MODE_ERR;

	ASSIGN_AND_OVERFLOW_CHECK(fw_param.psf, param->pqsf);

	return fapi_pon_generic_set(ctx,
				    PONFW_ONU_QOS_CONFIG_CMD_ID,
				    &fw_param,
				    sizeof(struct ponfw_onu_qos_config));
}

static enum fapi_pon_errorcode pon_pqsf_cfg_get_copy(struct pon_ctx *ctx,
						     const void *data,
						     size_t data_size,
						     void *priv)
{
	enum fapi_pon_errorcode ret;
	const struct ponfw_onu_qos_config *src_param = data;
	struct pon_pqsf_cfg *dst_param = priv;

	UNUSED(ctx);

	ret = integrity_check(dst_param, sizeof(*src_param), data_size);
	if (ret != PON_STATUS_OK)
		return ret;

	dst_param->pqsf = src_param->psf;

	return PON_STATUS_OK;
}

enum fapi_pon_errorcode fapi_pon_pqsf_cfg_get(struct pon_ctx *ctx,
					      struct pon_pqsf_cfg *param)
{
	/* GPON operation mode only */
	if (!pon_mode_check(ctx, MODE_984_GPON))
		return PON_STATUS_OPERATION_MODE_ERR;

	return fapi_pon_generic_get(ctx,
				    PONFW_ONU_QOS_CONFIG_CMD_ID,
				    NULL,
				    0,
				    &pon_pqsf_cfg_get_copy,
				    param);
}

static enum fapi_pon_errorcode pon_monitor_cfg_get_fw_copy(struct pon_ctx *ctx,
							   const void *data,
							   size_t data_size,
							   void *priv)
{
	enum fapi_pon_errorcode ret;
	const struct ponfw_monitor_config *src_param = data;
	struct ponfw_monitor_config *dst_param = priv;

	UNUSED(ctx);

	ret = integrity_check(dst_param, sizeof(*src_param), data_size);
	if (ret != PON_STATUS_OK)
		return ret;

	if (memcpy_s(dst_param, sizeof(*dst_param), src_param,
		     sizeof(*src_param))) {
		PON_DEBUG_ERR("memcpy_s failed");
		return PON_STATUS_MEMCPY_ERR;
	}

	return PON_STATUS_OK;
}

static enum fapi_pon_errorcode fapi_pon_ploam_log_state_set(struct pon_ctx *ctx,
							    uint32_t state)
{
	struct ponfw_monitor_config fw_param;
	enum fapi_pon_errorcode ret;

	if (!ctx)
		return PON_STATUS_INPUT_ERR;

	if (pon_mode_check(ctx, MODE_AON))
		return PON_STATUS_OPERATION_MODE_ERR;

	/*
	 * read the values which will not be
	 * modified to allow read/modify/write
	 */
	ret = fapi_pon_generic_get(ctx,
				   PONFW_MONITOR_CONFIG_CMD_ID,
				   NULL,
				   0,
				   &pon_monitor_cfg_get_fw_copy,
				   &fw_param);
	if (ret != PON_STATUS_OK)
		return ret;

	fw_param.ctrl_log = state;

	return fapi_pon_generic_set(ctx,
				    PONFW_MONITOR_CONFIG_CMD_ID,
				    &fw_param,
				    sizeof(struct ponfw_monitor_config));
}

enum fapi_pon_errorcode fapi_pon_ploam_log_enable(struct pon_ctx *ctx)
{
	if (!pon_mode_check(ctx, MODE_ITU_PON))
		return PON_STATUS_OPERATION_MODE_ERR;

	return fapi_pon_ploam_log_state_set(ctx,
		PONFW_MONITOR_CONFIG_CTRL_LOG_EN);
}

enum fapi_pon_errorcode fapi_pon_ploam_log_disable(struct pon_ctx *ctx)
{
	if (!pon_mode_check(ctx, MODE_ITU_PON))
		return PON_STATUS_OPERATION_MODE_ERR;

	return fapi_pon_ploam_log_state_set(ctx,
		PONFW_MONITOR_CONFIG_CTRL_LOG_DIS);
}

static enum fapi_pon_errorcode fapi_pon_1pps_event_state(struct pon_ctx *ctx,
							 uint32_t state)
{
	struct ponfw_monitor_config fw_param;
	enum fapi_pon_errorcode ret;

	if (!ctx)
		return PON_STATUS_INPUT_ERR;

	if (pon_mode_check(ctx, MODE_AON))
		return PON_STATUS_OPERATION_MODE_ERR;

	/*
	 * read the values which will not be
	 * modified to allow read/modify/write
	 */
	ret = fapi_pon_generic_get(ctx,
				   PONFW_MONITOR_CONFIG_CMD_ID,
				   NULL,
				   0,
				   &pon_monitor_cfg_get_fw_copy,
				   &fw_param);
	if (ret != PON_STATUS_OK)
		return ret;

	fw_param.pps_time = state;

	return fapi_pon_generic_set(ctx,
				    PONFW_MONITOR_CONFIG_CMD_ID,
				    &fw_param,
				    sizeof(struct ponfw_monitor_config));
}

enum fapi_pon_errorcode fapi_pon_1pps_event_enable(struct pon_ctx *ctx)
{
	return fapi_pon_1pps_event_state(ctx, PONFW_MONITOR_CONFIG_PPS_TIME_EN);
}

enum fapi_pon_errorcode fapi_pon_1pps_event_disable(struct pon_ctx *ctx)
{
	return fapi_pon_1pps_event_state(ctx,
					 PONFW_MONITOR_CONFIG_PPS_TIME_DIS);
}

enum fapi_pon_errorcode fapi_pon_link_enable(struct pon_ctx *ctx)
{
	return fapi_pon_reset(ctx, PON_MODE_UNKNOWN);
}

enum fapi_pon_errorcode fapi_pon_lwi_test_enable(struct pon_ctx *ctx)
{
	enum fapi_pon_errorcode ret;
	struct ponfw_local_wakeup fw_param = {0};

	if (!pon_mode_check(ctx, MODE_ITU_PON))
		return PON_STATUS_OPERATION_MODE_ERR;

	ret = debug_support_check(ctx);
	if (ret != PON_STATUS_OK)
		return ret;

	fw_param.lwi_sw = LWI_SW_CONTROL_ENABLE;

	return fapi_pon_generic_set(ctx,
				    PONFW_LOCAL_WAKEUP_CMD_ID,
				    &fw_param,
				    sizeof(struct ponfw_local_wakeup));
}

enum fapi_pon_errorcode fapi_pon_lwi_test_disable(struct pon_ctx *ctx)
{
	enum fapi_pon_errorcode ret;
	struct ponfw_local_wakeup fw_param = {0};

	if (!pon_mode_check(ctx, MODE_ITU_PON))
		return PON_STATUS_OPERATION_MODE_ERR;

	ret = debug_support_check(ctx);
	if (ret != PON_STATUS_OK)
		return ret;

	fw_param.lwi_sw = LWI_SW_CONTROL_DISABLE;

	return fapi_pon_generic_set(ctx,
				    PONFW_LOCAL_WAKEUP_CMD_ID,
				    &fw_param,
				    sizeof(struct ponfw_local_wakeup));
}

static enum fapi_pon_errorcode pon_lwi_test_state(struct pon_ctx *ctx,
						  const void *data,
						  size_t data_size,
						  void *priv)
{
	enum fapi_pon_errorcode ret;
	const struct ponfw_local_wakeup *src_param = data;
	struct pon_lwi *dst_param = priv;

	UNUSED(ctx);

	ret = integrity_check(dst_param, sizeof(*src_param), data_size);
	if (ret != PON_STATUS_OK)
		return ret;

	dst_param->lwi_sw = src_param->lwi_sw;
	dst_param->lwi = src_param->lwi;

	return PON_STATUS_OK;
}

enum fapi_pon_errorcode fapi_pon_lwi_set(struct pon_ctx *ctx)
{
	enum fapi_pon_errorcode ret;
	struct pon_lwi param;
	struct ponfw_local_wakeup fw_param = {0};

	if (!pon_mode_check(ctx, MODE_ITU_PON))
		return PON_STATUS_OPERATION_MODE_ERR;

	ret = debug_support_check(ctx);
	if (ret != PON_STATUS_OK)
		return ret;

	ret = fapi_pon_generic_get(ctx,
				   PONFW_LOCAL_WAKEUP_CMD_ID,
				   NULL,
				   0,
				   &pon_lwi_test_state,
				   &param);
	if (ret != PON_STATUS_OK)
		return ret;

	if (param.lwi_sw == LWI_SW_CONTROL_DISABLE)
		return PON_STATUS_LWI_SW_CONTROL_DISABLE;

	fw_param.lwi_sw = param.lwi_sw;
	fw_param.lwi = LWI_ON;

	return fapi_pon_generic_set(ctx,
				    PONFW_LOCAL_WAKEUP_CMD_ID,
				    &fw_param,
				    sizeof(struct ponfw_local_wakeup));
}

enum fapi_pon_errorcode fapi_pon_lwi_clear(struct pon_ctx *ctx)
{
	enum fapi_pon_errorcode ret;
	struct pon_lwi param;
	struct ponfw_local_wakeup fw_param = {0};

	if (!pon_mode_check(ctx, MODE_ITU_PON))
		return PON_STATUS_OPERATION_MODE_ERR;

	ret = debug_support_check(ctx);
	if (ret != PON_STATUS_OK)
		return ret;

	ret = fapi_pon_generic_get(ctx,
				   PONFW_LOCAL_WAKEUP_CMD_ID,
				   NULL,
				   0,
				   &pon_lwi_test_state,
				   &param);
	if (ret != PON_STATUS_OK)
		return ret;

	if (param.lwi_sw == LWI_SW_CONTROL_DISABLE)
		return PON_STATUS_LWI_SW_CONTROL_DISABLE;

	fw_param.lwi_sw = param.lwi_sw;
	fw_param.lwi = LWI_OFF;

	return fapi_pon_generic_set(ctx,
				    PONFW_LOCAL_WAKEUP_CMD_ID,
				    &fw_param,
				    sizeof(struct ponfw_local_wakeup));
}

enum fapi_pon_errorcode fapi_pon_debug_ploam_cfg_set(struct pon_ctx *ctx,
				const struct pon_debug_ploam_cfg *param)
{
	enum fapi_pon_errorcode ret;
	struct ponfw_debug_ploam_break fw_param = {0};

	if (!param)
		return PON_STATUS_INPUT_ERR;

	if (!pon_mode_check(ctx, MODE_ITU_PON))
		return PON_STATUS_OPERATION_MODE_ERR;

	ret = debug_support_check(ctx);
	if (ret != PON_STATUS_OK)
		return ret;

	ASSIGN_AND_OVERFLOW_CHECK(fw_param.break_enable, param->break_enable);
	ASSIGN_AND_OVERFLOW_CHECK(fw_param.state, param->ploam_state);
	ASSIGN_AND_OVERFLOW_CHECK(fw_param.force_state, param->force_enable);

	return fapi_pon_generic_set(ctx,
				    PONFW_DEBUG_PLOAM_BREAK_CMD_ID,
				    &fw_param,
				    sizeof(fw_param));
}

static enum fapi_pon_errorcode pon_debug_ploam_cfg_get_copy(struct pon_ctx *ctx,
							    const void *data,
							    size_t data_size,
							    void *priv)
{
	enum fapi_pon_errorcode ret;
	const struct ponfw_debug_ploam_break_status *src_param = data;
	struct pon_debug_ploam_cfg_status *dst_param = priv;

	UNUSED(ctx);

	ret = integrity_check(dst_param, sizeof(*src_param), data_size);
	if (ret != PON_STATUS_OK)
		return ret;

	dst_param->ploam_breakpoints |= src_param->o_00 << 0;
	dst_param->ploam_breakpoints |= src_param->o_10 << 1;
	dst_param->ploam_breakpoints |= src_param->o_11 << 2;
	dst_param->ploam_breakpoints |= src_param->o_12 << 3;
	dst_param->ploam_breakpoints |= src_param->o_20 << 4;
	dst_param->ploam_breakpoints |= src_param->o_23 << 5;
	dst_param->ploam_breakpoints |= src_param->o_30 << 6;
	dst_param->ploam_breakpoints |= src_param->o_40 << 7;
	dst_param->ploam_breakpoints |= src_param->o_50 << 8;
	dst_param->ploam_breakpoints |= src_param->o_51 << 9;
	dst_param->ploam_breakpoints |= src_param->o_52 << 10;
	dst_param->ploam_breakpoints |= src_param->o_60 << 11;
	dst_param->ploam_breakpoints |= src_param->o_70 << 12;
	dst_param->ploam_breakpoints |= src_param->o_71 << 13;
	dst_param->ploam_breakpoints |= src_param->o_72 << 14;
	dst_param->ploam_breakpoints |= src_param->o_81 << 15;
	dst_param->ploam_breakpoints |= src_param->o_82 << 16;
	dst_param->ploam_breakpoints |= src_param->o_90 << 17;

	return PON_STATUS_OK;
}

enum fapi_pon_errorcode fapi_pon_debug_ploam_cfg_get(struct pon_ctx *ctx,
				struct pon_debug_ploam_cfg_status *param)
{
	enum fapi_pon_errorcode ret;

	if (!pon_mode_check(ctx, MODE_ITU_PON))
		return PON_STATUS_OPERATION_MODE_ERR;

	ret = debug_support_check(ctx);
	if (ret != PON_STATUS_OK)
		return ret;

	return fapi_pon_generic_get(ctx,
				    PONFW_DEBUG_PLOAM_BREAK_STATUS_CMD_ID,
				    NULL,
				    0,
				    &pon_debug_ploam_cfg_get_copy,
				    param);
}

static enum fapi_pon_errorcode
pon_debug_gem_port_id_create(struct pon_ctx *ctx, uint32_t id,
			     uint32_t alloc_id, uint32_t max_gem_size,
			     uint32_t alloc_link_ref)
{
	struct ponfw_gem_port_id gem_port_id = {0};

	gem_port_id.dir = PONFW_GEM_PORT_ID_DIR_BI;

	ASSIGN_AND_OVERFLOW_CHECK(gem_port_id.gem_port_id, id);
	ASSIGN_AND_OVERFLOW_CHECK(gem_port_id.alloc_id, alloc_id);
	ASSIGN_AND_OVERFLOW_CHECK(gem_port_id.max_gem_size, max_gem_size);
	ASSIGN_AND_OVERFLOW_CHECK(gem_port_id.alloc_link_ref, alloc_link_ref);

	return fapi_pon_generic_set(ctx, PONFW_GEM_PORT_ID_CMD_ID, &gem_port_id,
				    sizeof(gem_port_id));
}

#define DEBUG_XPON_OMCI_ALLOC_ID 1090
#define DEBUG_GPON_OMCI_ALLOC_ID 640
#define DEBUG_GEM_PORT_ID 0x430
#define DEBUG_GEM_PORT_MAX_SIZE 0x0708
#define DEBUG_QOS_IDX 1
enum fapi_pon_errorcode fapi_pon_debug_create_omcc(struct pon_ctx *ctx)
{
	enum fapi_pon_errorcode ret;
	struct pon_allocation_index allocation_index = {0};
	uint16_t alloc_id;
	struct pon_gpon_status gpon_status = {0};

	ret = debug_support_check(ctx);
	if (ret != PON_STATUS_OK)
		return ret;

	if (!pon_mode_check(ctx, MODE_ITU_PON))
		return PON_STATUS_OPERATION_MODE_ERR;

	ret = fapi_pon_gpon_status_get(ctx, &gpon_status);
	if (ret != PON_STATUS_OK)
		return ret;

	alloc_id = DEBUG_XPON_OMCI_ALLOC_ID;
	if (pon_mode_check(ctx, MODE_984_GPON))
		alloc_id = DEBUG_GPON_OMCI_ALLOC_ID;

	ret = pon_debug_alloc(ctx, gpon_status.onu_id, alloc_id, true);
	if (ret != PON_STATUS_OK)
		return ret;

	ret = pon_alloc_id_get(ctx, alloc_id, &allocation_index, true,
			       DEBUG_QOS_IDX);
	if (ret != PON_STATUS_OK)
		return ret;

	return pon_debug_gem_port_id_create(ctx, DEBUG_GEM_PORT_ID, alloc_id,
					    DEBUG_GEM_PORT_MAX_SIZE,
					    allocation_index.alloc_link_ref);
}

#define DEBUG_GTC_PLOAM_SEND_ONU_ID 0xFF
#define DEBUG_GTC_PLOAM_SEND_MSG_TYPE_ID 0x03
static enum fapi_pon_errorcode
pon_debug_gpon_serial_number_send(struct pon_ctx *ctx, uint32_t onu_id,
				  const struct pon_serial_number *serial_number)
{
	struct ponfw_debug_gtc_ploam_send gtc_ploam_send = {0};

	gtc_ploam_send.id = DEBUG_GTC_PLOAM_SEND_ONU_ID;
	gtc_ploam_send.mid = DEBUG_GTC_PLOAM_SEND_MSG_TYPE_ID;
	gtc_ploam_send.data1 = GLUE_U16(onu_id,
					serial_number->serial_no[0]);
	gtc_ploam_send.data2 = GLUE_U16(serial_number->serial_no[1],
					serial_number->serial_no[2]);
	gtc_ploam_send.data3 = GLUE_U16(serial_number->serial_no[3],
					serial_number->serial_no[4]);
	gtc_ploam_send.data4 = GLUE_U16(serial_number->serial_no[5],
					serial_number->serial_no[6]);
	gtc_ploam_send.data5 = GLUE_U16(serial_number->serial_no[7],
					0);

	return fapi_pon_generic_set(ctx, PONFW_DEBUG_GTC_PLOAM_SEND_CMD_ID,
				    &gtc_ploam_send, sizeof(gtc_ploam_send));
}

#define DEBUG_XGTC_PLOAM_SEND_ONU_ID 0x03FF
#define DEBUG_XGTC_PLOAM_SEND_MSG_TYPE_ID 0x03
#define DEBUG_XGTC_PLOAM_SEND_SEQNO 0
static enum fapi_pon_errorcode
pon_debug_xpon_serial_number_send(struct pon_ctx *ctx, uint32_t onu_id,
				  const struct pon_serial_number *serial_number)
{
	struct ponfw_debug_xgtc_ploam_send xgtc_ploam_send = {0};

	xgtc_ploam_send.id = DEBUG_XGTC_PLOAM_SEND_ONU_ID;
	xgtc_ploam_send.type = DEBUG_XGTC_PLOAM_SEND_MSG_TYPE_ID;
	xgtc_ploam_send.seqno = DEBUG_XGTC_PLOAM_SEND_SEQNO;

	xgtc_ploam_send.msg[0] = GLUE_U32(
	    (onu_id >> 8) & 0xFF, onu_id & 0xFF,
	    serial_number->serial_no[0], serial_number->serial_no[1]);
	xgtc_ploam_send.msg[1] = GLUE_U32(
	    serial_number->serial_no[2], serial_number->serial_no[3],
	    serial_number->serial_no[4], serial_number->serial_no[5]);
	xgtc_ploam_send.msg[2] = GLUE_U32(
	    serial_number->serial_no[6], serial_number->serial_no[7],
	    0, 0);

	return fapi_pon_generic_set(ctx, PONFW_DEBUG_XGTC_PLOAM_SEND_CMD_ID,
				    &xgtc_ploam_send, sizeof(xgtc_ploam_send));
}

static enum fapi_pon_errorcode
pon_debug_serial_number_send(struct pon_ctx *ctx, uint32_t onu_id,
			     const struct pon_serial_number *serial_number)
{
	if (pon_mode_check(ctx, MODE_984_GPON))
		return pon_debug_gpon_serial_number_send(ctx, onu_id,
							 serial_number);
	return pon_debug_xpon_serial_number_send(ctx, onu_id, serial_number);
}

static enum fapi_pon_errorcode
pon_debug_configured_serial_number_send(struct pon_ctx *ctx, uint32_t onu_id)
{
	enum fapi_pon_errorcode ret;
	struct pon_serial_number serial_number = {0};

	ret = fapi_pon_serial_number_get(ctx, &serial_number);
	if (ret != PON_STATUS_OK)
		return ret;

	return pon_debug_serial_number_send(ctx, onu_id, &serial_number);
}

static enum fapi_pon_errorcode
pon_debug_ploam_state_set(struct pon_ctx *ctx, int32_t ploam_state)
{
	struct pon_debug_ploam_cfg ploam_cfg = {0};

	ploam_cfg.break_enable = 1;
	ploam_cfg.force_enable = true;
	ploam_cfg.ploam_state = ploam_state;

	return fapi_pon_debug_ploam_cfg_set(ctx, &ploam_cfg);
}

static enum fapi_pon_errorcode
pon_debug_alloc_id_all_unlink(struct pon_ctx *ctx)
{
	enum fapi_pon_errorcode ret;
	struct ponfw_alloc_id_unlink alloc_id_unlink = {0};

	alloc_id_unlink.all = 0;
	ret = fapi_pon_generic_set(ctx, PONFW_ALLOC_ID_UNLINK_CMD_ID,
				   &alloc_id_unlink, sizeof(alloc_id_unlink));
	if (ret != PON_STATUS_OK)
		return ret;

	alloc_id_unlink.all = 1;
	return fapi_pon_generic_set(ctx, PONFW_ALLOC_ID_UNLINK_CMD_ID,
				    &alloc_id_unlink, sizeof(alloc_id_unlink));
}

static enum fapi_pon_errorcode
pon_debug_ploam_processing_enable(struct pon_ctx *ctx)
{
	struct ponfw_debug_config debug_config = {0};

	debug_config.log_lvl = pon_mode_check(ctx, MODE_984_GPON) ? 0xA : 0x4;
	debug_config.ploamd = 1;
	debug_config.req_act = 1;

	return fapi_pon_generic_set(ctx, PONFW_DEBUG_CONFIG_CMD_ID,
				    &debug_config, sizeof(debug_config));
}

static enum fapi_pon_errorcode
pon_debug_lods_alarm_disable(struct pon_ctx *ctx)
{
	struct pon_debug_alarm_cfg alarm_cfg = {0};

	alarm_cfg.enable = 0;
	alarm_cfg.logging = 0;

	return fapi_pon_debug_alarm_cfg_set(ctx, PON_ALARM_STATIC_LODS,
					    &alarm_cfg);
}

enum fapi_pon_errorcode fapi_pon_debug_operational_enter(struct pon_ctx *ctx,
							 uint32_t onu_id)
{
	enum fapi_pon_errorcode ret;
	bool is_gpon = pon_mode_check(ctx, MODE_984_GPON);

	ret = debug_support_check(ctx);
	if (ret != PON_STATUS_OK)
		return ret;

	if (!pon_mode_check(ctx, MODE_ITU_PON))
		return PON_STATUS_OPERATION_MODE_ERR;

	ret = pon_debug_lods_alarm_disable(ctx);
	if (ret != PON_STATUS_OK)
		return ret;

	ret = pon_debug_alloc_id_all_unlink(ctx);
	if (ret != PON_STATUS_OK)
		return ret;

	ret = pon_debug_ploam_state_set(ctx, is_gpon ? 20 : 12);
	if (ret != PON_STATUS_OK)
		return ret;

	ret = pon_debug_ploam_state_set(ctx, is_gpon ? 30 : 23);
	if (ret != PON_STATUS_OK)
		return ret;

	ret = pon_debug_ploam_processing_enable(ctx);
	if (ret != PON_STATUS_OK)
		return ret;

	ret = pon_debug_configured_serial_number_send(ctx, onu_id);
	if (ret != PON_STATUS_OK)
		return ret;

	ret = pon_debug_ploam_state_set(ctx, 40);
	if (ret != PON_STATUS_OK)
		return ret;

	ret = pon_debug_ploam_state_set(ctx, is_gpon ? 50 : 51);
	if (ret != PON_STATUS_OK)
		return ret;

	return PON_STATUS_OK;
}

enum fapi_pon_errorcode
fapi_pon_twdm_cfg_set(struct pon_ctx *ctx, const struct pon_twdm_cfg *param)
{
	struct ponfw_twdm_optic_config fw_param = {0};

	if (!param)
		return PON_STATUS_INPUT_ERR;

	/* NG-PON2 mode only */
	if (!pon_mode_check(ctx, MODE_989_NGPON2_10G | MODE_989_NGPON2_2G5))
		return PON_STATUS_OPERATION_MODE_ERR;

	ASSIGN_AND_OVERFLOW_CHECK(fw_param.rx_tune_time,
				  param->rx_wl_switch_time);
	ASSIGN_AND_OVERFLOW_CHECK(fw_param.fsr, param->free_spectral_range);
	ASSIGN_AND_OVERFLOW_CHECK(fw_param.wcs, param->wl_ch_spacing);
	ASSIGN_AND_OVERFLOW_CHECK(fw_param.sp_exc,
				  param->max_spectral_excursion);
	ASSIGN_AND_OVERFLOW_CHECK(fw_param.tune_gran, param->tuning_gran);
	ASSIGN_AND_OVERFLOW_CHECK(fw_param.tune_time, param->fine_tune_time);
	ASSIGN_AND_OVERFLOW_CHECK(fw_param.cpi, param->ch_partition_index);
	ASSIGN_AND_OVERFLOW_CHECK(fw_param.dwlch_id, param->dwlch_id);
	ASSIGN_AND_OVERFLOW_CHECK(fw_param.lt, param->link_type);
	ASSIGN_AND_OVERFLOW_CHECK(fw_param.tx_tune_time,
				  param->tx_wl_switch_time);
	ASSIGN_AND_OVERFLOW_CHECK(fw_param.wl_lock,
				  param->wl_lock);

	return fapi_pon_generic_set(ctx,
				    PONFW_TWDM_OPTIC_CONFIG_CMD_ID,
				    &fw_param,
				    sizeof(struct ponfw_twdm_optic_config));
}

static enum fapi_pon_errorcode pon_twdm_cfg_get_copy(struct pon_ctx *ctx,
						     const void *data,
						     size_t data_size,
						     void *priv)
{
	enum fapi_pon_errorcode ret;
	const struct ponfw_twdm_optic_config *src_param = data;
	struct pon_twdm_cfg *dst_param = priv;

	UNUSED(ctx);
	UNUSED(data_size);

	ret = integrity_check(dst_param, sizeof(*src_param), data_size);
	if (ret != PON_STATUS_OK)
		return ret;

	memset(dst_param, 0x0, sizeof(*dst_param));

	dst_param->link_type = src_param->lt;
	dst_param->dwlch_id = src_param->dwlch_id;
	dst_param->free_spectral_range = src_param->fsr;
	dst_param->wl_ch_spacing = src_param->wcs;
	dst_param->max_spectral_excursion = src_param->sp_exc;
	dst_param->tuning_gran = src_param->tune_gran;
	dst_param->rx_wl_switch_time = src_param->rx_tune_time;
	dst_param->tx_wl_switch_time = src_param->tx_tune_time;
	dst_param->ch_partition_index = src_param->cpi;
	dst_param->fine_tune_time = src_param->tune_time;
	dst_param->wl_lock = src_param->wl_lock;

	return PON_STATUS_OK;
}

enum fapi_pon_errorcode
fapi_pon_twdm_cfg_get(struct pon_ctx *ctx, struct pon_twdm_cfg *param)
{
	/* NG-PON2 mode only */
	if (!pon_mode_check(ctx, MODE_989_NGPON2_10G | MODE_989_NGPON2_2G5))
		return PON_STATUS_OPERATION_MODE_ERR;

	return fapi_pon_generic_get(ctx,
				    PONFW_TWDM_OPTIC_CONFIG_CMD_ID,
				    NULL,
				    0,
				    &pon_twdm_cfg_get_copy,
				    param);
}

static enum fapi_pon_errorcode ponfw_twdm_config_get_copy(struct pon_ctx *ctx,
							 const void *data,
							 size_t data_size,
							 void *priv)
{
	enum fapi_pon_errorcode ret;
	const struct ponfw_twdm_config *src = data;
	struct ponfw_twdm_config *dst = priv;

	UNUSED(ctx);

	ret = integrity_check(dst, sizeof(*src), data_size);
	if (ret != PON_STATUS_OK)
		return ret;

	if (memcpy_s(dst, sizeof(*dst), src, sizeof(*src))) {
		PON_DEBUG_ERR("memcpy_s failed");
		return PON_STATUS_MEMCPY_ERR;
	}

	return PON_STATUS_OK;
}

enum fapi_pon_errorcode
fapi_pon_twdm_wlse_config_set(struct pon_ctx *ctx,
			      const struct pon_twdm_wlse_config *param)
{
	enum fapi_pon_errorcode ret;
	struct ponfw_twdm_config fw_param;

	if (!param)
		return PON_STATUS_INPUT_ERR;

	/* NG-PON2 mode only */
	if (!pon_mode_check(ctx, MODE_989_NGPON2_10G | MODE_989_NGPON2_2G5))
		return PON_STATUS_OPERATION_MODE_ERR;

	/* Check value to be configured against parameter's minimum value */
	if (param->wl_sw_rounds_init < PONIPFW_TWDM_CONFIG_WL_SW_ROUNDS_MIN)
		return PON_STATUS_VALUE_RANGE_ERR;

	/* Populate fw_param struct essentially by channel_partition_index by
	 * a readout from the FW. All other fw_param struct fields will be
	 * anyhow overwritten below.
	 */
	ret = fapi_pon_generic_get(ctx,
				   PONFW_TWDM_CONFIG_CMD_ID,
				   NULL,
				   0,
				   &ponfw_twdm_config_get_copy,
				   &fw_param);
	if (ret != PON_STATUS_OK)
		return ret;

	/* [FW] ms, [UCI/FAPI] 125us */
	ASSIGN_AND_OVERFLOW_CHECK(fw_param.wl_sw_delay,
				  TO_MILLISECONDS(param->wl_switch_delay));
	/* DWLCH_ID set to 0 (as required by the SAS) */
	ASSIGN_AND_OVERFLOW_CHECK(fw_param.dwlch_id, 0);
	/* [FW] ms, [UCI/FAPI] 125us */
	ASSIGN_AND_OVERFLOW_CHECK(fw_param.wl_sw_delay_init,
				  TO_MILLISECONDS(param->wl_sw_delay_init));
	ASSIGN_AND_OVERFLOW_CHECK(fw_param.wl_sw_rounds_init,
				  param->wl_sw_rounds_init);

	return fapi_pon_generic_set(ctx,
				    PONFW_TWDM_CONFIG_CMD_ID,
				    &fw_param,
				    sizeof(struct ponfw_twdm_config));
}

static enum fapi_pon_errorcode
pon_twdm_wlse_config_get_copy(struct pon_ctx *ctx,
			      const void *data,
			      size_t data_size,
			      void *priv)
{
	enum fapi_pon_errorcode ret;
	const struct ponfw_twdm_config *src = data;
	struct pon_twdm_wlse_config *dst = priv;

	UNUSED(ctx);
	UNUSED(data_size);

	ret = integrity_check(dst, sizeof(*src), data_size);
	if (ret != PON_STATUS_OK)
		return ret;

	memset(dst, 0x0, sizeof(*dst));

	ASSIGN_AND_OVERFLOW_CHECK(dst->wl_switch_delay,
				  FROM_MILLISECONDS(src->wl_sw_delay));
	ASSIGN_AND_OVERFLOW_CHECK(dst->wl_sw_delay_init,
				  FROM_MILLISECONDS(src->wl_sw_delay_init));
	ASSIGN_AND_OVERFLOW_CHECK(dst->wl_sw_rounds_init,
				  src->wl_sw_rounds_init);

	return PON_STATUS_OK;
}

enum fapi_pon_errorcode
fapi_pon_twdm_wlse_config_get(struct pon_ctx *ctx,
			      struct pon_twdm_wlse_config *param)
{
	/* NG-PON2 mode only */
	if (!pon_mode_check(ctx, MODE_989_NGPON2_10G | MODE_989_NGPON2_2G5))
		return PON_STATUS_OPERATION_MODE_ERR;

	return fapi_pon_generic_get(ctx,
				    PONFW_TWDM_CONFIG_CMD_ID,
				    NULL,
				    0,
				    &pon_twdm_wlse_config_get_copy,
				    param);
}

static enum fapi_pon_errorcode pon_status_get_copy_twdm(struct pon_ctx *ctx,
							const void *data,
							size_t data_size,
							void *priv)
{
	enum fapi_pon_errorcode ret;
	const struct ponfw_onu_status *src_param = data;
	struct pon_twdm_status *dst_param = priv;

	UNUSED(ctx);

	ret = integrity_check(dst_param, sizeof(*src_param), data_size);
	if (ret != PON_STATUS_OK)
		return ret;

	memset(dst_param, 0x0, sizeof(*dst_param));

	dst_param->us_ch_index = src_param->uwlch_id;
	dst_param->ds_ch_index = src_param->dwlch_id;

	return PON_STATUS_OK;
}

static enum fapi_pon_errorcode
fapi_pon_twdm_status_get_copy(struct pon_ctx *ctx,
			      const void *data,
			      size_t data_size,
			      void *priv)
{
	enum fapi_pon_errorcode ret;
	const struct ponfw_twdm_wl_status *src_param = data;
	struct pon_twdm_status *dst_param = priv;

	UNUSED(ctx);

	ret = integrity_check(dst_param, sizeof(*src_param), data_size);
	if (ret != PON_STATUS_OK)
		return ret;

	/* no memset, as this is the second part of the copy */
	dst_param->sys_id = src_param->sys_id;
	dst_param->us_ch_freq = src_param->us_ch_frq;
	dst_param->ds_ch_freq = src_param->ds_ch_frq;
	dst_param->sys_profile_version = src_param->sys_prof_vers;
	dst_param->ch_count = src_param->wl_ch_cnt;
	dst_param->us_wl_adj_amplitude = src_param->twdm_us_adj;
	dst_param->us_ch_spacing = src_param->us_ch_spacing;
	dst_param->us_wl_band_option = src_param->uwbo;

	return PON_STATUS_OK;
}

enum fapi_pon_errorcode
fapi_pon_twdm_status_get(struct pon_ctx *ctx, struct pon_twdm_status *param)
{
	enum fapi_pon_errorcode ret;

	/* NG-PON2 mode only */
	if (!pon_mode_check(ctx, MODE_989_NGPON2_10G | MODE_989_NGPON2_2G5))
		return PON_STATUS_OPERATION_MODE_ERR;

	ret = fapi_pon_generic_get(ctx,
				   PONFW_ONU_STATUS_CMD_ID,
				   NULL,
				   0,
				   &pon_status_get_copy_twdm,
				   param);
	if (ret != PON_STATUS_OK)
		return ret;

	return fapi_pon_generic_get(ctx,
				    PONFW_TWDM_WL_STATUS_CMD_ID,
				    NULL,
				    0,
				    &fapi_pon_twdm_status_get_copy,
				    param);
}

static enum fapi_pon_errorcode
fapi_pon_twdm_ch_pro_sts_get_copy(struct pon_ctx *ctx,
				  const void *data,
				  size_t data_size,
				  void *priv)
{
	enum fapi_pon_errorcode ret;
	const struct ponfw_twdm_channel_profile *src_param = data;
	struct pon_twdm_channel_profile *dst_param = priv;

	UNUSED(ctx);

	ret = integrity_check(dst_param, sizeof(*src_param), data_size);
	if (ret != PON_STATUS_OK)
		return ret;

	memset(dst_param, 0x0, sizeof(*dst_param));
	dst_param->pon_id = src_param->pon_id;
	dst_param->def_resp_ch = src_param->def_resp_ch;
	dst_param->us_frq = src_param->us_frq;
	dst_param->resp_thr = src_param->resp_thr;
	dst_param->ds_offset = src_param->ds_offset;
	dst_param->cloned = src_param->cloned;
	dst_param->version = src_param->version;
	dst_param->ds_valid = src_param->ds_valid;
	dst_param->ds_fec = src_param->ds_fec;
	dst_param->ds_rate = src_param->ds_rate;
	dst_param->us_valid = src_param->us_valid;
	dst_param->us_type = src_param->us_type;
	dst_param->us_rate = src_param->us_rate;
	dst_param->dswlch_id = src_param->dwlch_id;
	dst_param->uswlch_id = src_param->uwlch_id;
	dst_param->def_att = src_param->def_att;
	dst_param->cpi = src_param->cpi;
	dst_param->this_wl_channel = src_param->this_ch;

	return PON_STATUS_OK;
}

enum fapi_pon_errorcode
fapi_pon_twdm_channel_profile_status_get(struct pon_ctx *ctx, uint32_t cp_id,
					 struct pon_twdm_channel_profile *param)
{
	struct ponfw_twdm_channel_profile fw_param = {0};

	/* NG-PON2 mode only */
	if (!pon_mode_check(ctx, MODE_989_NGPON2_10G | MODE_989_NGPON2_2G5))
		return PON_STATUS_OPERATION_MODE_ERR;

	ASSIGN_AND_OVERFLOW_CHECK(fw_param.cp_id, cp_id);

	return fapi_pon_generic_get(ctx,
				    PONFW_TWDM_CHANNEL_PROFILE_CMD_ID,
				    &fw_param,
				    PONFW_TWDM_CHANNEL_PROFILE_LENR,
				    &fapi_pon_twdm_ch_pro_sts_get_copy,
				    param);
}

static enum fapi_pon_errorcode
pon_eth_counters_get_decode(struct pon_ctx *ctx,
			    struct nlattr **attrs,
			    void *priv)
{
	struct pon_eth_counters *dst_param = priv;
	struct nlattr *cnt[PON_MBOX_A_CNT_ETH_MAX + 1];

	UNUSED(ctx);

	memset(dst_param, 0, sizeof(*dst_param));

	if (!attrs[PON_MBOX_A_CNT])
		return PON_STATUS_ERR;

	if (nla_parse_nested(cnt, PON_MBOX_A_CNT_ETH_MAX,
			     attrs[PON_MBOX_A_CNT],
			     pon_mbox_cnt_eth_policy) < 0)
		return PON_STATUS_ERR;

	CNT_DECODE_U64(ETH_BYTES, bytes);
	CNT_DECODE_U64(ETH_FRAMES_LT_64, frames_lt_64);
	CNT_DECODE_U64(ETH_FRAMES_64, frames_64);
	CNT_DECODE_U64(ETH_FRAMES_65_127, frames_65_127);
	CNT_DECODE_U64(ETH_FRAMES_128_255, frames_128_255);
	CNT_DECODE_U64(ETH_FRAMES_256_511, frames_256_511);
	CNT_DECODE_U64(ETH_FRAMES_512_1023, frames_512_1023);
	CNT_DECODE_U64(ETH_FRAMES_1024_1518, frames_1024_1518);
	CNT_DECODE_U64(ETH_FRAMES_GT_1518, frames_gt_1518);
	CNT_DECODE_U64(ETH_FRAMES_FCS_ERR, frames_fcs_err);
	CNT_DECODE_U64(ETH_BYTES_FCS_ERR, bytes_fcs_err);
	CNT_DECODE_U64(ETH_FRAMES_TOO_LONG, frames_too_long);

	return PON_STATUS_OK;
}

static enum fapi_pon_errorcode
fapi_pon_eth_counters_get(struct pon_ctx *ctx,
			  uint32_t gem_port_id,
			  struct pon_eth_counters *param,
			  int nl_cmd)
{
	struct pon_gem_port gem_port;
	struct pon_range_limits limits = {0};
	struct read_cmd_cb cb_data;
	struct nl_msg *msg;
	uint32_t seq = NL_AUTO_SEQ;
	enum fapi_pon_errorcode ret;

	if (!ctx)
		return PON_STATUS_INPUT_ERR;

	if (!pon_mode_check(ctx, MODE_ITU_PON))
		return PON_STATUS_OPERATION_MODE_ERR;

	ret = fapi_pon_limits_get(ctx, &limits);
	if (ret != PON_STATUS_OK)
		return ret;

	if (gem_port_id > limits.gem_port_id_max)
		return PON_STATUS_VALUE_RANGE_ERR;

	/* This is only done to get the GEM port index for the GEM ID. */
	ret = fapi_pon_gem_port_id_get(ctx, gem_port_id, &gem_port);
	if (ret != PON_STATUS_OK)
		return ret;

	ret = fapi_pon_nl_msg_prepare_decode(ctx, &msg, &cb_data, &seq,
					     &pon_eth_counters_get_decode,
					     NULL,
					     param,
					     nl_cmd);
	if (ret != PON_STATUS_OK)
		return ret;

	ret = nla_put_u8(msg, PON_MBOX_D_GEM_IDX,
			 gem_port.gem_port_index);
	if (ret) {
		PON_DEBUG_ERR("Can't add netlink attribute");
		nlmsg_free(msg);
		return PON_STATUS_NL_ERR;
	}

	return fapi_pon_nl_msg_send(ctx, &msg, &cb_data, &seq);
}

enum fapi_pon_errorcode fapi_pon_eth_rx_counters_get(struct pon_ctx *ctx,
		    uint32_t gem_port_id,
		    struct pon_eth_counters *param)
{
	if (!pon_mode_check(ctx, MODE_ITU_PON))
		return PON_STATUS_OPERATION_MODE_ERR;

	return fapi_pon_eth_counters_get(ctx, gem_port_id, param,
					 PON_MBOX_C_ETH_RX_COUNTERS);
}

enum fapi_pon_errorcode fapi_pon_eth_tx_counters_get(struct pon_ctx *ctx,
		    uint32_t gem_port_id,
		    struct pon_eth_counters *param)
{
	if (!pon_mode_check(ctx, MODE_ITU_PON))
		return PON_STATUS_OPERATION_MODE_ERR;

	return fapi_pon_eth_counters_get(ctx, gem_port_id, param,
					 PON_MBOX_C_ETH_TX_COUNTERS);
}

enum fapi_pon_errorcode
fapi_pon_pin_config_set(struct pon_ctx *ctx, enum pon_gpio_pin_id pin_id,
			enum pon_gpio_pin_status status)
{
	struct read_cmd_cb cb_data;
	struct nl_msg *msg;
	enum fapi_pon_errorcode ret;
	uint32_t seq = NL_AUTO_SEQ;
	uint8_t pin_id_num;
	uint8_t status_num;

	if (!ctx)
		return PON_STATUS_INPUT_ERR;

	switch (pin_id) {
	case PON_GPIO_PIN_ID_RX_LOS:
		pin_id_num = PON_MBOX_GPIO_PIN_ID_RX_LOS;
		break;

	case PON_GPIO_PIN_ID_1PPS:
		pin_id_num = PON_MBOX_GPIO_PIN_ID_1PPS;
		break;

	case PON_GPIO_PIN_ID_NTR:
		pin_id_num = PON_MBOX_GPIO_PIN_ID_NTR;
		break;

	default:
		return PON_STATUS_VALUE_RANGE_ERR;
	}

	switch (status) {
	case PON_GPIO_PIN_STATUS_ENABLE:
		status_num = PON_MBOX_GPIO_PIN_STATUS_ENABLE;
		break;

	case PON_GPIO_PIN_STATUS_DISABLE:
		status_num = PON_MBOX_GPIO_PIN_STATUS_DISABLE;
		break;

	case PON_GPIO_PIN_STATUS_DEFAULT:
		status_num = PON_MBOX_GPIO_PIN_STATUS_DEFAULT;
		break;

	default:
		return PON_STATUS_VALUE_RANGE_ERR;
	}

	ret = fapi_pon_nl_msg_prepare(ctx, &msg, &cb_data, &seq,
				      NULL, NULL, NULL,
				      PON_MBOX_C_PIN_CONFIG);
	if (ret != PON_STATUS_OK)
		return ret;

	ret = nla_put_u8(msg, PON_MBOX_A_PIN_ID, pin_id_num);
	if (ret) {
		PON_DEBUG_ERR("Can't add PON_MBOX_A_PIN_ID netlink attribute");
		nlmsg_free(msg);
		return PON_STATUS_NL_ERR;
	}

	ret = nla_put_u8(msg, PON_MBOX_A_PIN_STATUS, status_num);
	if (ret) {
		PON_DEBUG_ERR(
		    "Can't add PON_MBOX_A_PIN_STATUS netlink attribute");
		nlmsg_free(msg);
		return PON_STATUS_NL_ERR;
	}

	return fapi_pon_nl_msg_send(ctx, &msg, &cb_data, &seq);
}

enum fapi_pon_errorcode
fapi_pon_uart_cfg_set(struct pon_ctx *ctx, const struct pon_uart_cfg *param)
{
	struct read_cmd_cb cb_data;
	struct nl_msg *msg;
	enum fapi_pon_errorcode ret;
	uint32_t seq = NL_AUTO_SEQ;
	uint8_t uart_mode;

	if (!ctx)
		return PON_STATUS_INPUT_ERR;

	switch (param->mode) {
	case PON_UART0_OFF:
		uart_mode = PON_MBOX_UART_OFF;
		break;

	case PON_UART0:
		uart_mode = PON_MBOX_UART_ASC0;
		break;

	case PON_UART1:
		uart_mode = PON_MBOX_UART_ASC1;
		break;

	default:
		return PON_STATUS_VALUE_RANGE_ERR;
	}

	ret = fapi_pon_nl_msg_prepare(ctx, &msg, &cb_data, &seq,
				      NULL, NULL, NULL,
				      PON_MBOX_C_UART_CONFIG);
	if (ret != PON_STATUS_OK)
		return ret;

	ret = nla_put_u8(msg, PON_MBOX_UART_MODE, uart_mode);
	if (ret) {
		PON_DEBUG_ERR("Can't add PON_MBOX_UART_MODE netlink attribute");
		nlmsg_free(msg);
		return PON_STATUS_NL_ERR;
	}

	return fapi_pon_nl_msg_send(ctx, &msg, &cb_data, &seq);
}

static enum fapi_pon_errorcode fapi_pon_uart_cfg_get_copy(struct pon_ctx *ctx,
							  struct nlattr **attrs,
							  void *priv)
{
	struct pon_uart_cfg *dst_param = priv;

	UNUSED(ctx);

	memset(dst_param, 0, sizeof(*dst_param));

	if (!attrs[PON_MBOX_A_MODE])
		return PON_STATUS_ERR;

	dst_param->mode = nla_get_u8(attrs[PON_MBOX_A_MODE]);
	return PON_STATUS_OK;
}

enum fapi_pon_errorcode
fapi_pon_uart_cfg_get(struct pon_ctx *ctx, struct pon_uart_cfg *param)
{
	struct read_cmd_cb cb_data;
	struct nl_msg *msg;
	enum fapi_pon_errorcode ret;
	uint32_t seq = NL_AUTO_SEQ;

	if (!ctx || !param)
		return PON_STATUS_INPUT_ERR;

	ret = fapi_pon_nl_msg_prepare_decode(ctx, &msg, &cb_data, &seq,
					&fapi_pon_uart_cfg_get_copy, NULL,
					param, PON_MBOX_C_UART_CONFIG);
	if (ret != PON_STATUS_OK)
		return ret;

	return fapi_pon_nl_msg_send(ctx, &msg, &cb_data, &seq);
}

enum fapi_pon_errorcode
fapi_pon_user_mngmt(struct pon_ctx *ctx,
		    enum pon_perm_revoke_grant revoke_grant,
		    enum pon_perm_command_group command_group,
		    uint32_t uid_val)
{
	struct read_cmd_cb cb_data;
	struct nl_msg *msg;
	enum fapi_pon_errorcode ret;
	uint8_t rev_grp;
	uint16_t cmd_grp;
	uint32_t seq = NL_AUTO_SEQ;

	if (!ctx)
		return PON_STATUS_INPUT_ERR;

	switch (revoke_grant) {
	case PON_PERM_REVOKE:
		rev_grp = PON_MBOX_PERM_REVOKE;
		break;

	case PON_PERM_GRANT:
		rev_grp = PON_MBOX_PERM_GRANT;
		break;

	default:
		return PON_STATUS_VALUE_RANGE_ERR;
	}

	switch (command_group) {
	case PON_PERM_SYNCE:
		cmd_grp = PON_MBOX_PERM_SYNCE;
		break;
	case PON_PERM_BC_KEY:
		cmd_grp = PON_MBOX_PERM_BC_KEY;
		break;
	default:
		return PON_STATUS_VALUE_RANGE_ERR;
	}

	ret = fapi_pon_nl_msg_prepare(ctx, &msg, &cb_data, &seq,
				      NULL, NULL, NULL,
				      PON_MBOX_C_USER_MNGMT);
	if (ret != PON_STATUS_OK)
		return ret;

	ret = nla_put_u8(msg, PON_MBOX_A_USER_MNGMT_REVOKE_GRANT, rev_grp);
	if (ret) {
		PON_DEBUG_ERR(
			"Can't add PON_MBOX_A_USER_MNGMT_REVOKE_GRANT netlink attribute");
		nlmsg_free(msg);
		return PON_STATUS_NL_ERR;
	}

	ret = nla_put_u16(msg, PON_MBOX_A_USER_MNGMT_CMD_GROUP, cmd_grp);
	if (ret) {
		PON_DEBUG_ERR(
			"Can't add PON_MBOX_A_USER_MNGMT_CMD_GROUP netlink attribute");
		nlmsg_free(msg);
		return PON_STATUS_NL_ERR;
	}

	ret = nla_put_u32(msg, PON_MBOX_A_USER_MNGMT_USER, uid_val);
	if (ret) {
		PON_DEBUG_ERR(
			"Can't add PON_MBOX_A_USER_MNGMT_USER netlink attribute");
		nlmsg_free(msg);
		return PON_STATUS_NL_ERR;
	}

	return fapi_pon_nl_msg_send(ctx, &msg, &cb_data, &seq);
}

static enum fapi_pon_errorcode pon_aon_cfg_copy(struct pon_ctx *ctx,
						const void *data,
						size_t data_size,
						void *priv)
{
	enum fapi_pon_errorcode ret;
	const struct ponfw_aon_config *src_param = data;
	struct ponfw_aon_config *dst_param = priv;

	UNUSED(ctx);

	ret = integrity_check(dst_param, sizeof(*src_param), data_size);
	if (ret != PON_STATUS_OK)
		return ret;

	if (memcpy_s(dst_param, sizeof(*dst_param), src_param,
		     sizeof(*src_param))) {
		PON_DEBUG_ERR("memcpy_s failed");
		return PON_STATUS_MEMCPY_ERR;
	}

	return PON_STATUS_OK;
}

enum fapi_pon_errorcode fapi_pon_aon_cfg_set(struct pon_ctx *ctx,
					     const struct pon_aon_cfg *param)
{
	enum fapi_pon_errorcode ret;
	struct ponfw_aon_config fw_param = {0};

	if (!param)
		return PON_STATUS_INPUT_ERR;

	if (!pon_mode_check(ctx, MODE_AON))
		return PON_STATUS_OPERATION_MODE_ERR;

	if (!is_plat_type_any_of(ctx, PONFW_AON_CONFIG_PLAT_COMPATIBILITY))
		return PON_STATUS_SUPPORT;

	ret = fapi_pon_generic_get(ctx,
				   PONFW_AON_CONFIG_CMD_ID,
				   NULL,
				   0,
				   &pon_aon_cfg_copy,
				   &fw_param);
	if (ret != PON_STATUS_OK)
		return ret;

	fw_param.pol = param->tx_en_pol;

	return fapi_pon_generic_set(ctx,
				    PONFW_AON_CONFIG_CMD_ID,
				    &fw_param,
				    sizeof(struct ponfw_aon_config));
}

static enum fapi_pon_errorcode pon_aon_cfg_get_copy(struct pon_ctx *ctx,
						    const void *data,
						    size_t data_size,
						    void *priv)
{
	enum fapi_pon_errorcode ret;
	const struct ponfw_aon_config *src_param = data;
	struct pon_aon_cfg *dst_param = priv;

	UNUSED(ctx);

	ret = integrity_check(dst_param, sizeof(*src_param), data_size);
	if (ret != PON_STATUS_OK)
		return ret;

	dst_param->tx_en_pol = src_param->pol;

	return PON_STATUS_OK;
}

enum fapi_pon_errorcode fapi_pon_aon_cfg_get(struct pon_ctx *ctx,
					     struct pon_aon_cfg *param)
{
	if (!pon_mode_check(ctx, MODE_AON))
		return PON_STATUS_OPERATION_MODE_ERR;

	if (!is_plat_type_any_of(ctx, PONFW_AON_CONFIG_PLAT_COMPATIBILITY))
		return PON_STATUS_SUPPORT;

	return fapi_pon_generic_get(ctx,
				    PONFW_AON_CONFIG_CMD_ID,
				    NULL,
				    0,
				    &pon_aon_cfg_get_copy,
				    param);
}

static enum fapi_pon_errorcode fapi_pon_aon_tx_en_dis(struct pon_ctx *ctx,
						      uint8_t state)
{
	enum fapi_pon_errorcode ret;
	struct ponfw_aon_config fw_param = {0};

	if (!ctx)
		return PON_STATUS_INPUT_ERR;

	if (!pon_mode_check(ctx, MODE_AON))
		return PON_STATUS_OPERATION_MODE_ERR;

	if (!is_plat_type_any_of(ctx, PONFW_AON_CONFIG_PLAT_COMPATIBILITY))
		return PON_STATUS_SUPPORT;

	ret = fapi_pon_generic_get(ctx,
				   PONFW_AON_CONFIG_CMD_ID,
				   NULL,
				   0,
				   &pon_aon_cfg_copy,
				   &fw_param);
	if (ret != PON_STATUS_OK)
		return ret;

	fw_param.laser_on = state;

	return fapi_pon_generic_set(ctx,
				    PONFW_AON_CONFIG_CMD_ID,
				    &fw_param,
				    sizeof(struct ponfw_aon_config));
}

enum fapi_pon_errorcode fapi_pon_aon_tx_enable(struct pon_ctx *ctx)
{
	if (!pon_mode_check(ctx, MODE_AON))
		return PON_STATUS_OPERATION_MODE_ERR;

	return fapi_pon_aon_tx_en_dis(ctx, PONFW_AON_CONFIG_LASER_ON_ON);
}

enum fapi_pon_errorcode fapi_pon_aon_tx_disable(struct pon_ctx *ctx)
{
	if (!pon_mode_check(ctx, MODE_AON))
		return PON_STATUS_OPERATION_MODE_ERR;

	return fapi_pon_aon_tx_en_dis(ctx, PONFW_AON_CONFIG_LASER_ON_OFF);
}

static enum fapi_pon_errorcode pon_serdes_biterr_error(struct pon_ctx *ctx,
						       uint8_t ack,
						       void *priv)
{
	UNUSED(ctx);
	UNUSED(priv);

	switch (ack) {
	case PONFW_ACK:
		return PON_STATUS_OK;
	default:
		/* map all reported errors to the generic error */
		return PON_STATUS_ERR;
	}
}

static enum fapi_pon_errorcode
pon_test_pattern_control_copy(struct pon_ctx *ctx, const void *data,
			      size_t data_size, void *priv);

enum fapi_pon_errorcode fapi_pon_serdes_biterr_start(struct pon_ctx *ctx)
{
	struct ponfw_debug_test_pattern_control fw_param_ctrl = {0};
	struct read_cmd_cb cb_data;
	struct nl_msg *msg;
	enum fapi_pon_errorcode ret;
	uint32_t seq = NL_AUTO_SEQ;

	if (!ctx)
		return PON_STATUS_INPUT_ERR;

	if (pon_mode_check(ctx, MODE_AON))
		return PON_STATUS_OPERATION_MODE_ERR;

	ret = fapi_pon_generic_get(ctx,
				   PONFW_DEBUG_TEST_PATTERN_CONTROL_CMD_ID,
				   NULL,
				   0,
				   &pon_test_pattern_control_copy,
				   &fw_param_ctrl);
	if (ret != PON_STATUS_OK)
		return ret;

	/* When debug test pattern is disabled, counter can not be started */
	if (fw_param_ctrl.tmo == PONFW_DEBUG_TEST_PATTERN_CONTROL_TMO_OFF) {
		PON_DEBUG_ERR("biterror counter cannot be started when debug test pattern is disabled");
		return PON_STATUS_ERR;
	}

	ret = fapi_pon_nl_msg_prepare(ctx, &msg, &cb_data, &seq,
				      NULL, pon_serdes_biterr_error, NULL,
				      PON_MBOX_C_BITERR_START);
	if (ret != PON_STATUS_OK)
		return ret;

	return fapi_pon_nl_msg_send(ctx, &msg, &cb_data, &seq);
}

static enum fapi_pon_errorcode
pon_serdes_biterror_decode(struct pon_ctx *ctx,
			   struct nlattr **attrs,
			   void *priv)
{
	struct pon_biterr_status *dst_param = priv;

	memset(dst_param, 0, sizeof(*dst_param));

	UNUSED(ctx);

	if (!attrs[PON_MBOX_A_BITERR_CNT] || !attrs[PON_MBOX_A_BITERR_TIME] ||
	    !attrs[PON_MBOX_A_BITERR_STATUS])
		return PON_STATUS_ERR;

	dst_param->counter = nla_get_u64(attrs[PON_MBOX_A_BITERR_CNT]);
	dst_param->period_ms = nla_get_u32(attrs[PON_MBOX_A_BITERR_TIME]);
	dst_param->counter_running =
				nla_get_u8(attrs[PON_MBOX_A_BITERR_STATUS]);

	return PON_STATUS_OK;
}

static enum fapi_pon_errorcode
pon_serdes_biterr_req(struct pon_ctx *ctx,
		      struct pon_biterr_status *param,
		      uint8_t msg_type)
{

	struct read_cmd_cb cb_data;
	struct nl_msg *msg;
	enum fapi_pon_errorcode ret;
	uint32_t seq = NL_AUTO_SEQ;

	if (!ctx || !param)
		return PON_STATUS_INPUT_ERR;

	if (pon_mode_check(ctx, MODE_AON))
		return PON_STATUS_OPERATION_MODE_ERR;

	ret = fapi_pon_nl_msg_prepare_decode(ctx, &msg, &cb_data, &seq,
					     pon_serdes_biterror_decode,
					     pon_serdes_biterr_error,
					     param, msg_type);
	if (ret != PON_STATUS_OK)
		return ret;

	return fapi_pon_nl_msg_send(ctx, &msg, &cb_data, &seq);
}

enum fapi_pon_errorcode
fapi_pon_serdes_biterr_stop(struct pon_ctx *ctx,
			    struct pon_biterr_status *param)
{
	enum fapi_pon_errorcode ret;

	ret = pon_serdes_biterr_req(ctx, param, PON_MBOX_C_BITERR_READ);
	if (ret != PON_STATUS_OK)
		return ret;

	/* If counter is already stopped, return error */
	if (param->counter_running == 0) {
		PON_DEBUG_ERR("%s: counter is already stopped", __func__);
		return PON_STATUS_ERR;
	}

	if (memset_s(param, sizeof(*param), 0, sizeof(*param))) {
		PON_DEBUG_ERR("memset_s failed");
		return PON_STATUS_MEMCPY_ERR;
	}

	return pon_serdes_biterr_req(ctx, param, PON_MBOX_C_BITERR_STOP);
}

enum fapi_pon_errorcode
fapi_pon_serdes_biterr_read(struct pon_ctx *ctx,
			    struct pon_biterr_status *param)
{
	return pon_serdes_biterr_req(ctx, param, PON_MBOX_C_BITERR_READ);
}

static enum fapi_pon_errorcode
pon_debug_test_pattern_control(struct pon_ctx *ctx,
			       enum serdes_test_mode test_mode)
{
	enum fapi_pon_errorcode ret;
	struct ponfw_debug_test_pattern_control fw_param = {0};

	if (!ctx)
		return PON_STATUS_INPUT_ERR;

	if (pon_mode_check(ctx, MODE_AON))
		return PON_STATUS_OPERATION_MODE_ERR;

	ret = debug_support_check(ctx);
	if (ret != PON_STATUS_OK)
		return ret;

	fw_param.tmo = test_mode;

	return fapi_pon_generic_set(ctx,
				PONFW_DEBUG_TEST_PATTERN_CONTROL_CMD_ID,
				&fw_param,
				sizeof(fw_param));
}

static enum fapi_pon_errorcode
fapi_pon_wl_set(struct pon_ctx *ctx,
		unsigned short int cmd,
		void *wl_config,
		size_t wl_config_len)
{
	struct pon_fake_event event_header = {0};
	enum fapi_pon_errorcode ret;
	size_t data_len;
	char *data;

	PON_FEVT_CMD_SET(event_header, cmd);
	PON_FEVT_LEN_SET(event_header, wl_config_len);

	data_len = sizeof(event_header) + wl_config_len;
	data = malloc(data_len);
	if (memcpy_s(data, data_len, &event_header,
		     sizeof(event_header))) {
		PON_DEBUG_ERR("memcpy_s failed");
		free(data);
		return PON_STATUS_MEMCPY_ERR;
	}
	if (memcpy_s(data + sizeof(event_header),
		     data_len - sizeof(event_header),
		     wl_config, wl_config_len)) {
		PON_DEBUG_ERR("memcpy_s failed");
		free(data);
		return PON_STATUS_MEMCPY_ERR;
	}

	/* Command 0x999 will be interpreted as fake event */
	ret = fapi_pon_generic_set(ctx, 0x999, data, data_len);

	free(data);

	return ret;

}

/* Deprecated function, shall not be used for new implementations */
enum fapi_pon_errorcode
fapi_pon_debug_test_pattern_enable(struct pon_ctx *ctx,
				   enum serdes_test_mode test_mode,
				   uint8_t wl_ch_id)
{
	struct pon_debug_test_pattern param = {0};
	enum test_pattern_type default_type = TP_MODE_PRBS31_28;
	enum fapi_pon_errorcode ret;
	struct ponfw_twdm_us_wl_config twdm_us_wl_config = {0};
	struct ponfw_twdm_ds_wl_config twdm_ds_wl_config = {0};

	if (test_mode < SERDES_TEST_MODE_RX ||
	    test_mode >= __SERDES_TEST_MODE_MAX)
		return PON_STATUS_VALUE_RANGE_ERR;

	ret = fapi_pon_debug_test_pattern_cfg_get(ctx, &param);
	if (ret != PON_STATUS_OK)
		return ret;

	/* If test pattern is not defined, set default test pattern */
	if (param.rx_type == 0 || param.tx_type == 0) {
		param.rx_type =
			param.rx_type == 0 ? default_type : param.rx_type;
		param.tx_type =
			param.tx_type == 0 ? default_type : param.tx_type;
		ret = fapi_pon_debug_test_pattern_cfg_set(ctx, &param);
		if (ret != PON_STATUS_OK)
			return ret;
	}

	/* The definition of serdes_test_mode is a 1:1 mapping of the FW
	 * definition. No mapping of values necessary here.
	 */
	ret = pon_debug_test_pattern_control(ctx, test_mode);
	if (ret != PON_STATUS_OK)
		return ret;

	/* Wavelenth change is for NG-PON2 mode only */
	if (!pon_mode_check(ctx, MODE_989_NGPON2_10G | MODE_989_NGPON2_2G5))
		return 0;

	twdm_us_wl_config.uwlch_id = wl_ch_id;
	twdm_ds_wl_config.dwlch_id = wl_ch_id;

	fapi_pon_wl_set(ctx, PONFW_TWDM_US_WL_CONFIG_CMD_ID,
			&twdm_us_wl_config, sizeof(twdm_us_wl_config));
	if (ret != PON_STATUS_OK)
		return ret;

	return fapi_pon_wl_set(ctx, PONFW_TWDM_DS_WL_CONFIG_CMD_ID,
			       &twdm_ds_wl_config, sizeof(twdm_ds_wl_config));
}

enum fapi_pon_errorcode fapi_pon_debug_test_pattern_disable(struct pon_ctx *ctx)
{
	enum fapi_pon_errorcode ret;
	struct pon_biterr_status param = {0};

	ret = pon_serdes_biterr_req(ctx, &param, PON_MBOX_C_BITERR_STOP);
	if (ret != PON_STATUS_OK)
		return ret;

	return pon_debug_test_pattern_control(ctx,
				PONFW_DEBUG_TEST_PATTERN_CONTROL_TMO_OFF);
}

static enum fapi_pon_errorcode
pon_test_pattern_control_copy(struct pon_ctx *ctx, const void *data,
			      size_t data_size, void *priv)
{
	enum fapi_pon_errorcode ret;
	const struct ponfw_debug_config *src_param = data;
	struct ponfw_debug_test_pattern_control *dst_param = priv;

	UNUSED(ctx);

	ret = integrity_check(dst_param, sizeof(*src_param), data_size);
	if (ret != PON_STATUS_OK)
		return ret;

	if (memcpy_s(dst_param, sizeof(*dst_param), src_param,
		     sizeof(*src_param))) {
		PON_DEBUG_ERR("memcpy_s failed");
		return PON_STATUS_MEMCPY_ERR;
	}

	return PON_STATUS_OK;
}

static enum fapi_pon_errorcode
pon_pattern_type_overflow_check(enum test_pattern_type type)
{
	if (type < PRBS_TP_MODE_MIN || type > PRBS_TP_MODE_MAX)
		return PON_STATUS_VALUE_RANGE_ERR;

	return PON_STATUS_OK;
}

static enum fapi_pon_errorcode
pon_pattern_type_value_check(enum test_pattern_type type,
			     uint8_t pattern)
{
	switch (type) {
	case TP_MODE_PAT:
	case TP_MODE_PAT_DCBAL:
	case TP_MODE_PAT_FIX:
		if (pattern < 1 || pattern > 254)
			return PON_STATUS_VALUE_RANGE_ERR;
		break;
	default:
		return PON_STATUS_OK;
	}

	return PON_STATUS_OK;
}

/* Deprecated function, shall not be used for new implementations */
enum fapi_pon_errorcode
fapi_pon_debug_test_pattern_cfg_set(struct pon_ctx *ctx,
				    const struct pon_debug_test_pattern *param)
{
	enum fapi_pon_errorcode ret;
	struct ponfw_debug_test_pattern_config fw_param = {0};

	if (!ctx)
		return PON_STATUS_INPUT_ERR;

	if (pon_mode_check(ctx, MODE_AON))
		return PON_STATUS_OPERATION_MODE_ERR;

	ret = debug_support_check(ctx);
	if (ret != PON_STATUS_OK)
		return ret;

	if (pon_pattern_type_overflow_check(param->rx_type) ||
	    pon_pattern_type_overflow_check(param->tx_type))
		return PON_STATUS_VALUE_RANGE_ERR;

	if (pon_pattern_type_value_check(param->tx_type, param->pattern))
		return PON_STATUS_VALUE_RANGE_ERR;

	ASSIGN_AND_OVERFLOW_CHECK(fw_param.tp, param->pattern);
	ASSIGN_AND_OVERFLOW_CHECK(fw_param.tx_mode, param->tx_type);
	ASSIGN_AND_OVERFLOW_CHECK(fw_param.rx_mode, param->rx_type);
	ASSIGN_AND_OVERFLOW_CHECK(fw_param.tx_inv, param->tx_inv);
	ASSIGN_AND_OVERFLOW_CHECK(fw_param.rx_inv, param->rx_inv);

	return fapi_pon_generic_set(ctx,
				PONFW_DEBUG_TEST_PATTERN_CONFIG_CMD_ID,
				&fw_param,
				sizeof(struct ponfw_debug_test_pattern_config));
}

static enum fapi_pon_errorcode pon_debug_prbs_cfg_get_copy(struct pon_ctx *ctx,
							   const void *data,
							   size_t data_size,
							   void *priv)
{
	enum fapi_pon_errorcode ret;
	const struct ponfw_debug_test_pattern_config *src_param = data;
	struct pon_debug_test_pattern *dst_param = priv;

	UNUSED(ctx);

	ret = integrity_check(dst_param, sizeof(*src_param), data_size);
	if (ret != PON_STATUS_OK)
		return ret;

	dst_param->tx_type = src_param->tx_mode;
	dst_param->rx_type = src_param->rx_mode;
	dst_param->pattern = src_param->tp;
	dst_param->tx_inv = src_param->tx_inv;
	dst_param->rx_inv = src_param->rx_inv;

	return PON_STATUS_OK;
}

enum fapi_pon_errorcode
fapi_pon_debug_test_pattern_cfg_get(struct pon_ctx *ctx,
				    struct pon_debug_test_pattern *param)
{
	enum fapi_pon_errorcode ret;

	if (!ctx)
		return PON_STATUS_INPUT_ERR;

	if (pon_mode_check(ctx, MODE_AON))
		return PON_STATUS_OPERATION_MODE_ERR;

	ret = debug_support_check(ctx);
	if (ret != PON_STATUS_OK)
		return ret;

	return fapi_pon_generic_get(ctx,
				    PONFW_DEBUG_TEST_PATTERN_CONFIG_CMD_ID,
				    NULL,
				    0,
				    &pon_debug_prbs_cfg_get_copy,
				    param);
}

enum fapi_pon_errorcode
fapi_pon_debug_test_pattern_status_get(struct pon_ctx *ctx,
				   struct pon_debug_test_pattern_status *param)
{
	enum fapi_pon_errorcode ret;
	struct pon_biterr_status biterr_status = {0};
	struct ponfw_debug_test_pattern_control fw_param_ctrl = {0};

	if (!ctx)
		return PON_STATUS_INPUT_ERR;

	if (pon_mode_check(ctx, MODE_AON))
		return PON_STATUS_OPERATION_MODE_ERR;

	ret = debug_support_check(ctx);
	if (ret != PON_STATUS_OK)
		return ret;

	ret = pon_serdes_biterr_req(ctx, &biterr_status,
				    PON_MBOX_C_BITERR_READ);
	if (ret != PON_STATUS_OK)
		return ret;

	param->counting = biterr_status.counter_running;

	ret = fapi_pon_generic_get(ctx,
				   PONFW_DEBUG_TEST_PATTERN_CONTROL_CMD_ID,
				   NULL,
				   0,
				   &pon_test_pattern_control_copy,
				   &fw_param_ctrl);
	if (ret != PON_STATUS_OK)
		return ret;

	param->test_mode = fw_param_ctrl.tmo;

	return PON_STATUS_OK;
}

enum fapi_pon_errorcode fapi_pon_debug_bit_error_set(struct pon_ctx *ctx)
{
	enum fapi_pon_errorcode ret;

	if (!ctx)
		return PON_STATUS_INPUT_ERR;

	if (pon_mode_check(ctx, MODE_AON))
		return PON_STATUS_OPERATION_MODE_ERR;

	ret = debug_support_check(ctx);
	if (ret != PON_STATUS_OK)
		return ret;

	return fapi_pon_generic_set(ctx,
				    PONFW_DEBUG_INSERT_BIT_ERROR_CMD_ID,
				    NULL,
				    0);
}

enum fapi_pon_errorcode fapi_pon_gpon_rerange_cfg_set(struct pon_ctx *ctx,
				    const struct pon_gpon_rerange_cfg *param)
{
	struct ponfw_link_control fw_param = {0};

	if (!ctx || !param)
		return PON_STATUS_INPUT_ERR;

	if ((param->lods_time != 0 && param->lods_time < 3) ||
	    param->lods_time > 8000)
		return PON_STATUS_INPUT_ERR;

	if (!pon_mode_check(ctx, MODE_ITU_PON))
		return PON_STATUS_OPERATION_MODE_ERR;

	fw_param.lods_time = param->lods_time;
	fw_param.lods_act = param->active;
	fw_param.psmdis = param->psmdis;
	fw_param.txdis = param->txdis;

	return fapi_pon_generic_set(ctx,
			    PONFW_LINK_CONTROL_CMD_ID,
			    &fw_param,
			    sizeof(struct ponfw_link_control));
}

static enum fapi_pon_errorcode
pon_gpon_rerange_status_get_copy(struct pon_ctx *ctx,
				 const void *data,
				 size_t data_size,
				 void *priv)
{
	enum fapi_pon_errorcode ret;
	const struct ponfw_link_control *src_param = data;
	struct pon_gpon_rerange_cfg *dst_param = priv;

	UNUSED(ctx);

	ret = integrity_check(dst_param, sizeof(*src_param), data_size);
	if (ret != PON_STATUS_OK)
		return ret;

	dst_param->lods_time = src_param->lods_time;
	dst_param->active = src_param->lods_act;
	dst_param->psmdis = src_param->psmdis;
	dst_param->txdis = src_param->txdis;

	return ret;
}

enum fapi_pon_errorcode fapi_pon_gpon_rerange_status_get(struct pon_ctx *ctx,
					struct pon_gpon_rerange_cfg *param)
{
	if (!ctx || !param)
		return PON_STATUS_INPUT_ERR;

	if (!pon_mode_check(ctx, MODE_ITU_PON))
		return PON_STATUS_OPERATION_MODE_ERR;

	return fapi_pon_generic_get(ctx,
				PONFW_LINK_CONTROL_CMD_ID,
				NULL,
				0,
				&pon_gpon_rerange_status_get_copy,
				param);
}

enum fapi_pon_errorcode
fapi_pon_alarm_limit_cfg_set(struct pon_ctx *ctx,
			     const struct pon_alarm_limit_cfg *param)
{
	struct ponfw_monitor_config fw_param = {0};
	struct ponfw_aon_config fw_param_aon = {0};
	enum fapi_pon_errorcode ret;

	if (!param || !ctx)
		return PON_STATUS_INPUT_ERR;

	if (pon_mode_check(ctx, MODE_ITU_PON)) {
		ret = fapi_pon_generic_get(ctx,
					PONFW_MONITOR_CONFIG_CMD_ID,
					NULL,
					0,
					&pon_monitor_cfg_get_fw_copy,
					&fw_param);
		if (ret != PON_STATUS_OK)
			return ret;
		ASSIGN_AND_OVERFLOW_CHECK(fw_param.alarm_ot,
					  param->alarm_ot);
		ASSIGN_AND_OVERFLOW_CHECK(fw_param.alarm_thr,
					  param->alarm_thr);

		return fapi_pon_generic_set(ctx,
					PONFW_MONITOR_CONFIG_CMD_ID,
					&fw_param,
					sizeof(struct ponfw_monitor_config));
	}

	if (pon_mode_check(ctx, MODE_AON)) {
		ret = fapi_pon_generic_get(ctx,
					PONFW_AON_CONFIG_CMD_ID,
					NULL,
					0,
					&pon_aon_cfg_copy,
					&fw_param_aon);
		if (ret != PON_STATUS_OK)
			return ret;
		ASSIGN_AND_OVERFLOW_CHECK(fw_param_aon.alarm_ot,
					  param->alarm_ot);
		ASSIGN_AND_OVERFLOW_CHECK(fw_param_aon.alarm_thr,
					  param->alarm_thr);

		return fapi_pon_generic_set(ctx,
					PONFW_AON_CONFIG_CMD_ID,
					&fw_param_aon,
					sizeof(struct ponfw_aon_config));
	}

	return PON_STATUS_OPERATION_MODE_ERR;
}

static enum fapi_pon_errorcode
fapi_pon_alarm_limit_cfg_get_copy(struct pon_ctx *ctx,
				  const void *data,
				  size_t data_size,
				  void *priv)
{
	enum fapi_pon_errorcode ret;
	const struct ponfw_monitor_config *src_param = data;
	struct pon_alarm_limit_cfg *dst_param = priv;

	UNUSED(ctx);

	ret = integrity_check(dst_param, sizeof(*src_param), data_size);
	if (ret != PON_STATUS_OK)
		return ret;

	dst_param->alarm_ot = src_param->alarm_ot;
	dst_param->alarm_thr = src_param->alarm_thr;

	return ret;
}

static enum fapi_pon_errorcode
fapi_pon_alarm_limit_cfg_get_aon_copy(struct pon_ctx *ctx,
				      const void *data,
				      size_t data_size,
				      void *priv)
{
	enum fapi_pon_errorcode ret;
	const struct ponfw_aon_config *src_param = data;
	struct pon_alarm_limit_cfg *dst_param = priv;

	UNUSED(ctx);

	ret = integrity_check(dst_param, sizeof(*src_param), data_size);
	if (ret != PON_STATUS_OK)
		return ret;

	dst_param->alarm_ot = src_param->alarm_ot;
	dst_param->alarm_thr = src_param->alarm_thr;

	return ret;
}

enum fapi_pon_errorcode
fapi_pon_alarm_limit_cfg_get(struct pon_ctx *ctx,
			     struct pon_alarm_limit_cfg *param)
{
	if (pon_mode_check(ctx, MODE_ITU_PON)) {
		return fapi_pon_generic_get(ctx,
					    PONFW_MONITOR_CONFIG_CMD_ID,
					    NULL,
					    0,
					    &fapi_pon_alarm_limit_cfg_get_copy,
					    param);
	}

	if (pon_mode_check(ctx, MODE_AON)) {
		return fapi_pon_generic_get(ctx,
					PONFW_AON_CONFIG_CMD_ID,
					NULL,
					0,
					&fapi_pon_alarm_limit_cfg_get_aon_copy,
					param);
	}

	return PON_STATUS_OPERATION_MODE_ERR;
}

enum fapi_pon_errorcode
fapi_pon_timeout_cfg_set(struct pon_ctx *ctx,
			 const struct pon_timeout_cfg *param)
{
	struct ponfw_timeout_values fw_param = { 0 };

	if (!param || !ctx)
		return PON_STATUS_INPUT_ERR;

	if (!pon_mode_check(ctx, MODE_ITU_PON))
		return PON_STATUS_OPERATION_MODE_ERR;

	ASSIGN_AND_OVERFLOW_CHECK(fw_param.toz, param->ploam_timeout_0);
	ASSIGN_AND_OVERFLOW_CHECK(fw_param.to1, param->ploam_timeout_1);
	ASSIGN_AND_OVERFLOW_CHECK(fw_param.to2, param->ploam_timeout_2);
	ASSIGN_AND_OVERFLOW_CHECK(fw_param.to3, param->ploam_timeout_3);
	ASSIGN_AND_OVERFLOW_CHECK(fw_param.to4, param->ploam_timeout_4);
	ASSIGN_AND_OVERFLOW_CHECK(fw_param.to5, param->ploam_timeout_5);
	ASSIGN_AND_OVERFLOW_CHECK(fw_param.to6, param->ploam_timeout_6);
	ASSIGN_AND_OVERFLOW_CHECK(fw_param.to_cpl, param->ploam_timeout_cpl);
	ASSIGN_AND_OVERFLOW_CHECK(fw_param.to_tpd, param->ploam_timeout_tpd);
	if (param->ploam_timeout_cpi >= 0xFFFF)
		/* Disable timeout if 0xFFFF or higher is received */
		fw_param.to_cpi = 0;
	else
		fw_param.to_cpi = param->ploam_timeout_cpi;

	return fapi_pon_generic_set(ctx,
				    PONFW_TIMEOUT_VALUES_CMD_ID,
				    &fw_param,
				    sizeof(struct ponfw_timeout_values));
}

static enum fapi_pon_errorcode
pon_timeout_cfg_get_copy(struct pon_ctx *ctx,
			 const void *data,
			 size_t data_size,
			 void *priv)
{
	enum fapi_pon_errorcode ret;
	const struct ponfw_timeout_values *src_param = data;
	struct pon_timeout_cfg *dst_param = priv;

	UNUSED(ctx);

	ret = integrity_check(dst_param, sizeof(*src_param), data_size);
	if (ret != PON_STATUS_OK)
		return ret;

	dst_param->ploam_timeout_0 = src_param->toz;
	dst_param->ploam_timeout_1 = src_param->to1;
	dst_param->ploam_timeout_2 = src_param->to2;
	dst_param->ploam_timeout_3 = src_param->to3;
	dst_param->ploam_timeout_4 = src_param->to4;
	dst_param->ploam_timeout_5 = src_param->to5;
	dst_param->ploam_timeout_6 = src_param->to6;
	dst_param->ploam_timeout_cpl = src_param->to_cpl;
	dst_param->ploam_timeout_cpi = src_param->to_cpi;
	dst_param->ploam_timeout_tpd = src_param->to_tpd;

	return PON_STATUS_OK;
}

enum fapi_pon_errorcode fapi_pon_timeout_cfg_get(struct pon_ctx *ctx,
						 struct pon_timeout_cfg *param)
{
	if (!ctx || !param)
		return PON_STATUS_INPUT_ERR;

	if (!pon_mode_check(ctx, MODE_ITU_PON))
		return PON_STATUS_OPERATION_MODE_ERR;

	return fapi_pon_generic_get(ctx,
				    PONFW_TIMEOUT_VALUES_CMD_ID,
				    NULL,
				    0,
				    &pon_timeout_cfg_get_copy,
				    param);
}

static enum fapi_pon_errorcode pon_dp_config_get_decode(struct pon_ctx *ctx,
							struct nlattr **attrs,
							void *priv)
{
	struct pon_dp_config *dp_config = priv;
	struct nlattr *cfg[PON_MBOX_DPCFG_MAX + 1];

	UNUSED(ctx);

	memset(dp_config, 0, sizeof(*dp_config));

	if (!attrs[PON_MBOX_A_DP_CONFIG])
		return PON_STATUS_ERR;

	if (nla_parse_nested(cfg, PON_MBOX_DPCFG_MAX,
			     attrs[PON_MBOX_A_DP_CONFIG],
			     dp_config_policy) < 0)
		return PON_STATUS_ERR;

	if (cfg[PON_MBOX_A_DP_CONFIG_WITH_RX_FCS])
		dp_config->with_rx_fcs =
			nla_get_u8(cfg[PON_MBOX_A_DP_CONFIG_WITH_RX_FCS]);
	if (cfg[PON_MBOX_A_DP_CONFIG_WITH_TX_FCS])
		dp_config->with_tx_fcs =
			nla_get_u8(cfg[PON_MBOX_A_DP_CONFIG_WITH_TX_FCS]);

	if (cfg[PON_MBOX_A_DP_CONFIG_WITHOUT_TIMESTAMP])
		dp_config->without_timestamp =
			nla_get_u8(cfg[PON_MBOX_A_DP_CONFIG_WITHOUT_TIMESTAMP]);

	return PON_STATUS_OK;
}

enum fapi_pon_errorcode fapi_pon_dp_config_get(struct pon_ctx *ctx,
					       struct pon_dp_config *param)
{
	struct read_cmd_cb cb_data;
	struct nl_msg *msg;
	enum fapi_pon_errorcode ret;
	uint32_t seq = NL_AUTO_SEQ;

	if (!ctx || !param)
		return PON_STATUS_INPUT_ERR;

	ret = fapi_pon_nl_msg_prepare_decode(ctx, &msg, &cb_data, &seq,
					     &pon_dp_config_get_decode,
					     NULL, param,
					     PON_MBOX_C_DP_CONFIG);
	if (ret != PON_STATUS_OK)
		return ret;

	return fapi_pon_nl_msg_send(ctx, &msg, &cb_data, &seq);
}

static enum fapi_pon_errorcode
pon_twdm_cpi_get_copy(struct pon_ctx *ctx,
		      const void *data,
		      size_t data_size,
		      void *priv)
{
	enum fapi_pon_errorcode ret;
	const struct ponfw_twdm_config *src_param = data;
	uint8_t *dst_param = priv;

	UNUSED(ctx);

	ret = integrity_check(dst_param, sizeof(*src_param), data_size);
	if (ret != PON_STATUS_OK)
		return ret;

	*dst_param = src_param->cpi;

	return PON_STATUS_OK;
}

enum fapi_pon_errorcode
fapi_pon_twdm_cpi_get(struct pon_ctx *ctx, uint8_t *channel_partition_index)
{
	if (!ctx || !channel_partition_index)
		return PON_STATUS_INPUT_ERR;

	/* NG-PON2 mode only */
	if (!pon_mode_check(ctx, MODE_989_NGPON2_10G | MODE_989_NGPON2_2G5))
		return PON_STATUS_OPERATION_MODE_ERR;

	return fapi_pon_generic_get(ctx,
				    PONFW_TWDM_CONFIG_CMD_ID,
				    NULL,
				    0,
				    &pon_twdm_cpi_get_copy,
				    channel_partition_index);
}

enum fapi_pon_errorcode
fapi_pon_twdm_cpi_set(struct pon_ctx *ctx, uint8_t channel_partition_index)
{
	enum fapi_pon_errorcode ret;
	struct ponfw_twdm_config fw_param;

	if (!ctx)
		return PON_STATUS_INPUT_ERR;

	/* NG-PON2 mode only */
	if (!pon_mode_check(ctx, MODE_989_NGPON2_10G | MODE_989_NGPON2_2G5))
		return PON_STATUS_OPERATION_MODE_ERR;

	/* Allowed range of the CPI value is 0..15 */
	if (channel_partition_index & ~0xF)
		return PON_STATUS_VALUE_RANGE_ERR;

	/* do a read modify write as more params are in this message now */
	ret = fapi_pon_generic_get(ctx,
				   PONFW_TWDM_CONFIG_CMD_ID,
				   NULL,
				   0,
				   &ponfw_twdm_config_get_copy,
				   &fw_param);
	if (ret != PON_STATUS_OK)
		return ret;

	/* Send the update message to the firmware (TWDM_CONFIG) */
	ASSIGN_AND_OVERFLOW_CHECK(fw_param.cpi, channel_partition_index);
	/* dwlch_id gets reported in the TWDM_CONFIG readout, clear it when
	 * bouncing back the FW message, just to be in compliance with the SAS.
	 */
	ASSIGN_AND_OVERFLOW_CHECK(fw_param.dwlch_id, 0);
	/* leave the rest of the readout as provided by the FW */

	return fapi_pon_generic_set(ctx, PONFW_TWDM_CONFIG_CMD_ID, &fw_param,
				    sizeof(fw_param));
}

enum fapi_pon_errorcode
fapi_pon_olt_type_set(struct pon_ctx *ctx,
		      const struct pon_olt_type *param,
		      const uint32_t iop_mask)
{
	struct ponfw_onu_interop_config fw_param = {0};
	enum fapi_pon_errorcode ret;
	struct ponfw_onu_interop_config *fw_mask = NULL;

	if (!ctx || !param)
		return PON_STATUS_INPUT_ERR;
	if (param->type > PON_OLT_LAST)
		return PON_STATUS_INPUT_ERR;

	ret = fapi_pon_generic_get(ctx,
				   PONFW_ONU_INTEROP_CONFIG_CMD_ID,
				   NULL,
				   0,
				   &pon_iop_cfg_get_copy,
				   &fw_param);
	if (ret != PON_STATUS_OK)
		return ret;

	switch (param->type) {
	case PON_OLT_TIBIT:
	case PON_OLT_CIENA:
		/* Force interoperability config for these OLTs */
		fw_param.iop10 = PONFW_ONU_INTEROP_CONFIG_IOP10_TIB;
		break;
	case PON_OLT_UNKNOWN:
	case PON_OLT_NOKIA:
	case PON_OLT_HUAWEI:
	case PON_OLT_ADTRAN:
	case PON_OLT_DZS:
	case PON_OLT_ZTE:
	case PON_OLT_CALIX:
	default:
		fw_mask = (struct ponfw_onu_interop_config *)&iop_mask;
		/* Keep external bits if given */
		fw_param.iop10 =
			PONFW_ONU_INTEROP_CONFIG_IOP10_STD | fw_mask->iop10;
		break;
	}

	ret = fapi_pon_generic_set(ctx,
				   PONFW_ONU_INTEROP_CONFIG_CMD_ID,
				   &fw_param,
				   sizeof(fw_param));
	if (ret != PON_STATUS_OK)
		return ret;

	return PON_STATUS_OK;
}

enum fapi_pon_errorcode
fapi_pon_twdm_counter_wlchid_set(struct pon_ctx *ctx,
			 const uint8_t dswlch_id)
{
	enum fapi_pon_errorcode ret;
	struct nl_msg *msg;
	int err;

	if (!ctx)
		return PON_STATUS_INPUT_ERR;

	if (dswlch_id >= 8)
		return PON_STATUS_INPUT_ERR;

	if (!pon_mode_check(ctx, MODE_989_NGPON2_10G | MODE_989_NGPON2_2G5))
		return PON_STATUS_OPERATION_MODE_ERR;

	ret = fapi_pon_msg_prepare(&ctx, &msg, PON_MBOX_C_CNT_TWDM_WLCHID);
	if (ret != PON_STATUS_OK)
		return ret;

	err = nla_put_u8(msg, PON_MBOX_CNT_TWDM_WLCHID_DS, dswlch_id);
	if (err) {
		PON_DEBUG_ERR("Can't add netlink attribute");
		nlmsg_free(msg);
		return PON_STATUS_NL_ERR;
	}

	err = nl_send_auto_complete(ctx->nls, msg);
	nlmsg_free(msg);
	if (err < 0) {
		PON_DEBUG_ERR("Can't send netlink message: %i", err);
		return PON_STATUS_NL_ERR;
	}

	return PON_STATUS_OK;
}

static enum fapi_pon_errorcode
pon_twdm_lods_counters_get_decode(struct pon_ctx *ctx,
				  struct nlattr **attrs,
				  void *priv)
{
	struct pon_twdm_xgtc_counters *dst_param = priv;
	struct nlattr *cnt[PON_MBOX_A_CNT_TWDM_LODS_MAX + 1];

	memset(dst_param, 0, sizeof(*dst_param));

	UNUSED(ctx);

	if (!attrs[PON_MBOX_A_CNT])
		return PON_STATUS_ERR;

	if (nla_parse_nested(cnt, PON_MBOX_A_CNT_TWDM_LODS_MAX,
			     attrs[PON_MBOX_A_CNT],
			     pon_mbox_cnt_twdm_lods_policy) < 0)
		return PON_STATUS_ERR;

	CNT_DECODE_U64(TWDM_LODS_EVENTS_ALL, lods_events_all);
	CNT_DECODE_U64(TWDM_LODS_RESTORED_OPER, lods_restored_oper);
	CNT_DECODE_U64(TWDM_LODS_RESTORED_PROT, lods_restored_prot);
	CNT_DECODE_U64(TWDM_LODS_RESTORED_DISK, lods_restored_disc);
	CNT_DECODE_U64(TWDM_LODS_REACTIVATION_OPER, lods_reactivation);
	CNT_DECODE_U64(TWDM_LODS_REACTIVATION_PROT, lods_reactivation_prot);
	CNT_DECODE_U64(TWDM_LODS_REACTIVATION_DISC, lods_reactivation_disc);

	return PON_STATUS_OK;
}

enum fapi_pon_errorcode
fapi_pon_twdm_xgtc_counters_get(struct pon_ctx *ctx,
				const uint8_t dswlch_id,
				struct pon_twdm_xgtc_counters *param)
{
	struct read_cmd_cb cb_data;
	struct nl_msg *msg;
	enum fapi_pon_errorcode ret;
	uint32_t seq = NL_AUTO_SEQ;
	struct pon_xgtc_counters pon_xgtc_cnt = { 0 };
	struct pon_gtc_counters gtc_counters = { 0 };

	if (!ctx || !param)
		return PON_STATUS_INPUT_ERR;

	/* NG-PON2 mode only */
	if (!pon_mode_check(ctx, MODE_989_NGPON2_10G |
				 MODE_989_NGPON2_2G5))
		return PON_STATUS_OPERATION_MODE_ERR;

	/* fill the nl message and get the LODS counters */
	ret = fapi_pon_nl_msg_prepare_decode(ctx, &msg, &cb_data, &seq,
					     &pon_twdm_lods_counters_get_decode,
					     NULL, param,
					     PON_MBOX_C_TWDM_LODS_COUNTERS);
	if (ret != PON_STATUS_OK)
		return ret;

	ret = nla_put_u8(msg, PON_MBOX_D_DSWLCH_ID, dswlch_id);
	if (ret) {
		PON_DEBUG_ERR("Can't add netlink attribute");
		nlmsg_free(msg);
		return PON_STATUS_NL_ERR;
	}

	ret = fapi_pon_nl_msg_send(ctx, &msg, &cb_data, &seq);
	if (ret != PON_STATUS_OK)
		return ret;

	/* get XGTC */
	ret = pon_xgtc_counters_get(ctx, dswlch_id, &pon_xgtc_cnt);
	if (ret != PON_STATUS_OK)
		return ret;

	param->psbd_hec_err_uncorr = pon_xgtc_cnt.psbd_hec_err_uncorr;
	param->psbd_hec_err_corr = pon_xgtc_cnt.psbd_hec_err_corr;
	param->fs_hec_err_uncorr = pon_xgtc_cnt.fs_hec_err_uncorr;
	param->fs_hec_err_corr = pon_xgtc_cnt.fs_hec_err_corr;
	param->lost_words = pon_xgtc_cnt.lost_words;
	param->ploam_mic_err = pon_xgtc_cnt.ploam_mic_err;
	param->burst_profile_err = pon_xgtc_cnt.burst_profile_err;

	/* get GTC */
	ret = pon_gtc_counters_get(ctx, dswlch_id, &gtc_counters);
	if (ret != PON_STATUS_OK)
		return ret;

	param->xgem_hec_err_corr = gtc_counters.gem_hec_errors_corr;
	param->xgem_hec_err_uncorr = gtc_counters.gem_hec_errors_uncorr;
	param->bip_errors = gtc_counters.bip_errors;
	if (pon_mode_check(ctx, MODE_989_NGPON2_2G5)) {
		param->words = gtc_counters.total_frames
				* DS_FRAMES_TO_BIP32_WORDS_MODE_NGPON2_2G5;
	} else {
		/* Only possible option here is MODE_989_NGPON2_10G */
		param->words = gtc_counters.total_frames
				* DS_FRAMES_TO_BIP32_WORDS_MODE_NGPON2_10G;
	}

	return PON_STATUS_OK;
}

static enum fapi_pon_errorcode
pon_twdm_optic_pl_counters_get_decode(struct pon_ctx *ctx,
				      struct nlattr **attrs,
				      void *priv)
{
	struct pon_twdm_optic_pl_counters *dst_param = priv;
	struct nlattr *cnt[PON_MBOX_A_CNT_TWDM_OPTIC_PL_MAX + 1];

	memset(dst_param, 0, sizeof(*dst_param));

	UNUSED(ctx);

	if (!attrs[PON_MBOX_A_CNT])
		return PON_STATUS_ERR;

	if (nla_parse_nested(cnt, PON_MBOX_A_CNT_TWDM_OPTIC_PL_MAX,
			     attrs[PON_MBOX_A_CNT],
			     pon_mbox_cnt_twdm_optic_pl_policy) < 0)
		return PON_STATUS_ERR;

	CNT_DECODE_U64(TWDM_OPTIC_PL_REJECTED, rejected);
	CNT_DECODE_U64(TWDM_OPTIC_PL_INCOMPLETE, incomplete);
	CNT_DECODE_U64(TWDM_OPTIC_PL_COMPLETE, complete);

	return PON_STATUS_OK;
}

enum fapi_pon_errorcode
fapi_pon_twdm_optic_pl_counters_get(struct pon_ctx *ctx,
				    const uint8_t dswlch_id,
				    struct pon_twdm_optic_pl_counters *param)
{
	struct read_cmd_cb cb_data;
	struct nl_msg *msg;
	enum fapi_pon_errorcode ret;
	uint32_t seq = NL_AUTO_SEQ;

	if (!ctx || !param)
		return PON_STATUS_INPUT_ERR;

	/* NG-PON2 mode only */
	if (!pon_mode_check(ctx, MODE_989_NGPON2_10G |
				 MODE_989_NGPON2_2G5))
		return PON_STATUS_OPERATION_MODE_ERR;

	/* fill the nl message and get the LODS counters */
	ret = fapi_pon_nl_msg_prepare_decode(ctx, &msg, &cb_data, &seq,
				&pon_twdm_optic_pl_counters_get_decode,
				NULL, param, PON_MBOX_C_TWDM_LODS_COUNTERS);
	if (ret != PON_STATUS_OK)
		return ret;

	ret = nla_put_u8(msg, PON_MBOX_D_DSWLCH_ID, dswlch_id);
	if (ret) {
		PON_DEBUG_ERR("Can't add netlink attribute");
		nlmsg_free(msg);
		return PON_STATUS_NL_ERR;
	}

	return fapi_pon_nl_msg_send(ctx, &msg, &cb_data, &seq);
}

enum fapi_pon_errorcode
fapi_pon_twdm_ploam_ds_counters_get(struct pon_ctx *ctx,
				    const uint8_t dswlch_id,
				    struct pon_twdm_ploam_ds_counters *param)
{
	enum fapi_pon_errorcode ret;
	struct pon_xgtc_counters pon_xgtc_cnt = { 0 };

	if (!ctx || !param)
		return PON_STATUS_INPUT_ERR;

	/* NG-PON2 mode only */
	if (!pon_mode_check(ctx, MODE_989_NGPON2_10G |
				 MODE_989_NGPON2_2G5))
		return PON_STATUS_OPERATION_MODE_ERR;

	memset(param, 0, sizeof(*param));

	ret = pon_tc_ploam_ds_counters_get(ctx, dswlch_id,
			&pon_twdm_ploam_ds_counters_get_decode, param);
	if (ret != PON_STATUS_OK)
		return ret;

	/* get XGTC counters*/
	ret = pon_xgtc_counters_get(ctx, dswlch_id, &pon_xgtc_cnt);
	if (ret != PON_STATUS_OK)
		return ret;

	param->mic_err = pon_xgtc_cnt.ploam_mic_err;
	param->all += param->mic_err;

	return PON_STATUS_OK;
}

enum fapi_pon_errorcode
fapi_pon_twdm_ploam_us_counters_get(struct pon_ctx *ctx,
				    const uint8_t dswlch_id,
				    struct pon_twdm_ploam_us_counters *param)
{
	enum fapi_pon_errorcode ret;

	if (!ctx || !param)
		return PON_STATUS_INPUT_ERR;

	/* NG-PON2 mode only */
	if (!pon_mode_check(ctx, MODE_989_NGPON2_10G |
				 MODE_989_NGPON2_2G5))
		return PON_STATUS_OPERATION_MODE_ERR;

	memset(param, 0, sizeof(*param));

	ret = pon_tc_ploam_us_counters_get(ctx, dswlch_id,
			&pon_twdm_ploam_us_counters_get_decode, param);
	if (ret != PON_STATUS_OK)
		return ret;

	return PON_STATUS_OK;
}

static enum fapi_pon_errorcode
pon_twdm_tc_counters_get_decode(struct pon_ctx *ctx,
				struct nlattr **attrs,
				void *priv)
{
	struct pon_twdm_tuning_counters *dst_param = priv;
	struct nlattr *cnt[PON_MBOX_A_CNT_TWDM_TC_MAX + 1];
	struct nlattr *attr = NULL, *tc;
	unsigned int i;
	int err, remaining;

	memset(dst_param, 0, sizeof(*dst_param));

	UNUSED(ctx);

	if (!attrs[PON_MBOX_A_CNT])
		return PON_STATUS_ERR;

	if (nla_parse_nested(cnt, PON_MBOX_A_CNT_TWDM_TC_MAX,
			     attrs[PON_MBOX_A_CNT],
			     pon_mbox_cnt_twdm_tc_policy) < 0)
		return PON_STATUS_ERR;

	tc = cnt[PON_MBOX_A_CNT_TWDM_TC];
	if (tc) {
		err = nla_validate(nla_data(tc),
				   nla_len(tc),
				   PON_MBOX_A_CNT_TWDM_TC_ITEM_MAX,
				   pon_mbox_cnt_twdm_tc_item_policy);
		if (err)
			return PON_STATUS_ERR;
		i = 0;
		nla_for_each_nested(attr, tc, remaining) {
			if (i >= ARRAY_SIZE(dst_param->counters))
				break;
			if (nla_type(attr) != PON_MBOX_A_CNT_TWDM_TC_ITEM)
				continue;
			dst_param->counters[i] = nla_get_u64(attr);
			i++;
		}
	}

	return PON_STATUS_OK;
}

enum fapi_pon_errorcode
fapi_pon_twdm_tuning_counters_get(struct pon_ctx *ctx,
				  const uint8_t dswlch_id,
				  struct pon_twdm_tuning_counters *param)
{
	struct read_cmd_cb cb_data;
	struct nl_msg *msg;
	enum fapi_pon_errorcode ret;
	uint32_t seq = NL_AUTO_SEQ;

	if (!ctx || !param)
		return PON_STATUS_INPUT_ERR;

	/* NG-PON2 mode only */
	if (!pon_mode_check(ctx, MODE_989_NGPON2_10G |
				 MODE_989_NGPON2_2G5))
		return PON_STATUS_OPERATION_MODE_ERR;

	/* fill the nl message and get the LODS counters */
	ret = fapi_pon_nl_msg_prepare_decode(ctx, &msg, &cb_data, &seq,
				&pon_twdm_tc_counters_get_decode,
				NULL, param, PON_MBOX_C_TWDM_TC_COUNTERS);
	if (ret != PON_STATUS_OK)
		return ret;

	ret = nla_put_u8(msg, PON_MBOX_D_DSWLCH_ID, dswlch_id);
	if (ret) {
		PON_DEBUG_ERR("Can't add netlink attribute");
		nlmsg_free(msg);
		return PON_STATUS_NL_ERR;
	}

	return fapi_pon_nl_msg_send(ctx, &msg, &cb_data, &seq);
}

static enum fapi_pon_errorcode
pon_xgspon_lods_counters_get_decode(struct pon_ctx *ctx,
				    struct nlattr **attrs,
				    void *priv)
{
	struct pon_xgspon_lods_counters *dst_param = priv;
	struct nlattr *cnt[PON_MBOX_A_CNT_TWDM_LODS_MAX + 1];

	memset(dst_param, 0, sizeof(*dst_param));

	UNUSED(ctx);

	if (!attrs[PON_MBOX_A_CNT])
		return PON_STATUS_ERR;

	if (nla_parse_nested(cnt, PON_MBOX_A_CNT_TWDM_LODS_MAX,
			     attrs[PON_MBOX_A_CNT],
			     pon_mbox_cnt_twdm_lods_policy) < 0)
		return PON_STATUS_ERR;

	CNT_DECODE_U64(TWDM_LODS_EVENTS_ALL, lods_events_all);
	CNT_DECODE_U64(TWDM_LODS_RESTORED_OPER, lods_restored_oper);
	CNT_DECODE_U64(TWDM_LODS_RESTORED_PROT, lods_restored_prot);
	CNT_DECODE_U64(TWDM_LODS_RESTORED_DISK, lods_restored_disc);
	CNT_DECODE_U64(TWDM_LODS_REACTIVATION_OPER, lods_reactivation);
	CNT_DECODE_U64(TWDM_LODS_REACTIVATION_PROT, lods_reactivation_prot);
	CNT_DECODE_U64(TWDM_LODS_REACTIVATION_DISC, lods_reactivation_disc);

	return PON_STATUS_OK;
}

enum fapi_pon_errorcode
fapi_pon_xgspon_lods_counters_get(struct pon_ctx *ctx,
				  struct pon_xgspon_lods_counters *param)
{
	struct read_cmd_cb cb_data;
	struct nl_msg *msg;
	enum fapi_pon_errorcode ret;
	uint32_t seq = NL_AUTO_SEQ;

	if (!ctx || !param)
		return PON_STATUS_INPUT_ERR;

	/* XG-PON and XGS-PON mode only */
	if (!pon_mode_check(ctx, MODE_987_XGPON|MODE_9807_XGSPON))
		return PON_STATUS_OPERATION_MODE_ERR;

	/* fill the nl message and get the LODS counters */
	ret = fapi_pon_nl_msg_prepare_decode(ctx, &msg, &cb_data, &seq,
					     &pon_xgspon_lods_counters_get_decode,
					     NULL, param,
					     PON_MBOX_C_TWDM_LODS_COUNTERS);
	if (ret != PON_STATUS_OK)
		return ret;

	ret = nla_put_u8(msg, PON_MBOX_D_DSWLCH_ID, PON_MBOX_D_DSWLCH_ID_CURR);
	if (ret) {
		PON_DEBUG_ERR("Can't add netlink attribute");
		nlmsg_free(msg);
		return PON_STATUS_NL_ERR;
	}

	ret = fapi_pon_nl_msg_send(ctx, &msg, &cb_data, &seq);
	if (ret != PON_STATUS_OK)
		return ret;

	return PON_STATUS_OK;
}
