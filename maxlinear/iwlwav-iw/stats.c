#include <errno.h>
#include <string.h>
#include <stdio.h>

#include <netlink/genl/genl.h>
#include <netlink/genl/family.h>
#include <netlink/genl/ctrl.h>
#include <netlink/msg.h>
#include <netlink/attr.h>

#include "nl80211.h"
#include "iw.h"

#if defined YOCTO
#include <puma_safe_libc.h>
#else
#include "libsafec/safe_str_lib.h"
#include "libsafec/safe_mem_lib.h"
#endif

typedef unsigned long long int u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;
typedef int64_t s64;
typedef int32_t s32;
typedef int16_t s16;
typedef int8_t s8;
#include "vendor_cmds_copy.h"

#define MIN(a,b) (((a)<(b))?(a):(b))

DECLARE_SECTION(iwlwav);

/* Data Types and Structures */
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
/*****************************/

/**** Headers ****/
static int print_link_adapt_mu_groups_counters(struct nl_msg *msg, void *arg);
static void dump_la_mu_groups_counters_stats(char *outData, unsigned int outLen);
/*****************/

/***************************** GET FUNCTIONS *****************************/
static int get_stat(struct nl_msg *msg,
			int argc, char **argv,
			enum ltq_nl80211_vendor_subcmds subcmd,
			int (*stat_handler)(struct nl_msg *, void *))
{
	if (!msg) return -EFAULT;

	register_handler(stat_handler, NULL);

	NLA_PUT_U32(msg, NL80211_ATTR_VENDOR_ID, OUI_MXL);
	NLA_PUT_U32(msg, NL80211_ATTR_VENDOR_SUBCMD, subcmd);

	return 0;
nla_put_failure:
	return -ENOBUFS;
}
/*************************************************************************/

/***************************** GET HANDLERS *****************************/
static int handle_stats_get_link_adapt_mu_groups_counters(struct nl80211_state *state,
					struct nl_msg *msg, int argc,
					char **argv, enum id_input id)
{
	return get_stat(msg, argc, argv, LTQ_NL80211_VENDOR_SUBCMD_GET_LINK_ADAPT_MU_GROUPS_COUNTERS_STATS, print_link_adapt_mu_groups_counters);
}
COMMAND(iwlwav, gLinkAdaptationMuGroupsCounters, "", NL80211_CMD_VENDOR, 0, CIB_NETDEV, handle_stats_get_link_adapt_mu_groups_counters, "");
/************************************************************************/

/***************************** PRINT FUNCTIONS ****************************/
static int print_link_adapt_mu_groups_counters(struct nl_msg *msg, void *arg)
{
	struct nlattr *attr;
	struct genlmsghdr *gnlh;
	char *data = NULL;

	gnlh = nlmsg_data(nlmsg_hdr(msg));
	attr = nla_find(genlmsg_attrdata(gnlh, 0),
			genlmsg_attrlen(gnlh, 0),
			NL80211_ATTR_VENDOR_DATA);

	if (!attr) {
		fprintf(stderr, "ERROR: Vendor data attribute missing!\n");
		return NL_SKIP;
	}

	data = (char *)nla_data(attr);
	if (data != NULL) {
		dump_la_mu_groups_counters_stats(data, nla_len(attr));
	}

	return NL_OK;
}
/**************************************************************************/

/***************************** DUMP FUNCTIONS ****************************/
static const char* _getLaMuRemovalReasonName(group_removal_reason_t reason)
{
	switch (reason)
	{
		case REMOVAL_REASON_ULOFDMA_MINXPUT:
		case REMOVAL_REASON_DLOFDMA_MINXPUT:
		case REMOVAL_REASON_DLMIMO_MINXPUT:
		case REMOVAL_REASON_ULMIMO_MINXPUT:
			return "Minxput";
		case REMOVAL_REASON_ULOFDMA_CONGESTION:
		case REMOVAL_REASON_DLOFDMA_CONGESTION:
			return "Congestion";
		case REMOVAL_REASON_DLOFDMA_SU_VALIDATION:
			return "SU Validation";
		case REMOVAL_REASON_DLOFDMA_LA_CONVERGENCE:
			return "LA Convergence";
		case REMOVAL_REASON_DLOFDMA_SHORT_TX:
			return "Short TX";
		case REMOVAL_REASON_DLOFDMA_MOVED_STA:
			return "Moved STA";
		case REMOVAL_REASON_DLOFDMA_ATR:
			return "ATR";
		case REMOVAL_REASON_ULOFDMA_PAYLOAD_EFF:
		case REMOVAL_REASON_DLOFDMA_PAYLOAD_EFF:
		case REMOVAL_REASON_DLMIMO_PAYLOAD_EFF:
		case REMOVAL_REASON_ULMIMO_PAYLOAD_EFF:
			return "Payload EFF";
		case REMOVAL_REASON_DLOFDMA_LATENCY:
			return "Latency";
		case REMOVAL_REASON_DLMIMO_MCS_DEGRADATION:
		case REMOVAL_REASON_ULMIMO_MCS_DEGRADATION:
			return "MCS Degradation";
		case REMOVAL_REASON_DLMIMO_TXOP_USAGE:
		case REMOVAL_REASON_ULMIMO_TXOP_USAGE:
			return "TXOP Usage";
		case REMOVAL_REASON_DLMIMO_SUMRATE:
			return "SumRate";
		case REMOVAL_REASON_DLOFDMA_ULPCALGO:
		case REMOVAL_REASON_ULOFDMA_ULPCALGO:
		case REMOVAL_REASON_DLMIMO_ULPCALGO:
		case REMOVAL_REASON_ULMIMO_ULPCALGO:
			return "ULPC Algorithm";
		case REMOVAL_REASON_ULOFDMA_UPHALGO:
		case REMOVAL_REASON_ULMIMO_UPHALGO:
			return "UPH Algorithm";
		case REMOVAL_REASON_REQUESTOR_DISABLE_PROCESS:
			return "Disable process";
		case REMOVAL_REASON_REQUESTOR_DISABLE_DYNAMIC_MU_PROCESS:
			return "Disable dynamic mu process";
		case REMOVAL_REASON_REQUESTOR_REMOVE_STATIC:
			return "Remove static";
		case REMOVAL_REASON_REQUESTOR_STATION_REMOVAL:
			return "STA Removal";
		case REMOVAL_REASON_REQUESTOR_CREATE_STATIC:
			return "Create static";
		case REMOVAL_REASON_REQUESTOR_SET_CHANNEL:
			return "Set Channel";
		case REMOVAL_REASON_REQUESTOR_SET_ANTENNA:
			return "Set Antenna";
		case REMOVAL_REASON_REQUESTOR_REMOVE_GROUPS_FOR_STATION_TID:
			return "Remove groups for station TID";
		case REMOVAL_REASON_REQUESTOR_CHANGE_FROM_STATIC_TO_DYNAMIC:
			return "Change from static to dynamic";
		case REMOVAL_REASON_REQUESTOR_STA_CAPABILITY_CHANGED:
			return "STA capability changed";
		case REMOVAL_REASON_REQUESTOR_STA_UPH_OUT_OF_BOUNDS:
			return "STA UPH out of bounds";
		case REMOVAL_REASON_REQUESTOR_ML_SWITCH_LINK:
			return "ML switch link";
		case REMOVAL_REASON_REQUESTOR_REBUILD_ALL_GROUPS:
			return "Rebuild all groups";
		case REMOVAL_REASON_REQUESTOR_POWER_CHANGE:
			return "Power change";
		case REMOVAL_REASON_REQUESTOR_CREATE_GROUP_FAILED:
			return "Create group failed";
		case REMOVAL_REASON_REQUESTOR_ULPC_ALGO_ERROR:
			return "ULPC Algo error";
		case REMOVAL_REASON_REQUESTOR_DISABLE_DYNAMIC_MU_BY_TYPE_PROCESS:
			return "Disable dynamic MU by type process";
		case REMOVAL_REASON_REQUESTOR_GROUP_SIMILAR_CHARACTERISTICS:
			return "Group similar characteristics";
		case REMOVAL_REASON_REQUESTOR_STA_IN_PS_MODE:
			return "STA in ps mode";
		case REMOVAL_REASON_REQUESTOR_TWT_AGREEMENT:
			return "TWT agreement";
		case REMOVAL_REASON_REQUESTOR_MBFM_ERRORS:
			return "MBFM errors";
		case REMOVAL_REASON_REQUESTOR_INVALID_UL_PSDU_LENGTH:
			return "Invalid UL PSDU length";
		case REMOVAL_REASON_OTHER:
			return "Other";
		default:
			return "Unknown";
	}
}

static void _dump_la_mu_groups_counters(mu_groups_counters_t *counters, int removal_reason_start, int removal_reason_end)
{
	const char *_fmt = "\t\t%-40s - %3u\n";
	int loop_start;
	int loop_end;
	int i;

	fprintf(stdout, "\tGroups Created: %3u\n", counters->groupsCreated);
	fprintf(stdout, "\tGroups Removed: %3u\n", counters->groupsRemoved);
	fprintf(stdout, "\tRemoval Reason List:\n");

	/* Print removal reasons that can only be applied to this Formation Type */
	loop_start = removal_reason_start;
	loop_end   = MIN((REMOVAL_REASON_MAX -1), removal_reason_end);
	for (i = loop_start; i <= loop_end; i++) {
		fprintf(stdout, _fmt, _getLaMuRemovalReasonName(i), counters->removalReason[i]);
	}

	/* Print Removal Requestors */
	loop_start  = REMOVAL_REASON_REQUESTOR_DISABLE_PROCESS;
	loop_end    = REMOVAL_REASON_REQUESTOR_INVALID_UL_PSDU_LENGTH;
	for (i = loop_start; i <= loop_end; i++) {
		fprintf(stdout, _fmt, _getLaMuRemovalReasonName(i), counters->removalReason[i]);
	}

	/* Print Other Reasons */
	fprintf(stdout, _fmt, _getLaMuRemovalReasonName(REMOVAL_REASON_OTHER), counters->removalReason[REMOVAL_REASON_OTHER]);
}

static void dump_la_mu_groups_counters_stats(char *outData, unsigned int outLen)
{
	la_mu_groups_counters_stats_t *stats = (la_mu_groups_counters_stats_t *)outData;

	if (outLen != sizeof(*stats)) {
		fprintf(stdout, "\nERROR: Unaligned iw/driver shared structure\n");
		return;
	}

	fprintf(stdout, "\n####################################################################\n");

	fprintf(stdout, "\nDownlink OFDMA Groups Counters\n");
	_dump_la_mu_groups_counters(&stats->dlOfdmaGroupsCounters, REMOVAL_REASON_DLOFDMA_MINXPUT, REMOVAL_REASON_DLOFDMA_CONGESTION);

	fprintf(stdout, "\nUplink OFDMA Groups Counters\n");
	_dump_la_mu_groups_counters(&stats->ulOfdmaGroupsCounters, REMOVAL_REASON_ULOFDMA_CONGESTION, REMOVAL_REASON_ULOFDMA_UPHALGO);

	fprintf(stdout, "\nDownlink MIMO Groups Counters\n");
	_dump_la_mu_groups_counters(&stats->dlMimoGroupsCounters, REMOVAL_REASON_DLMIMO_MINXPUT, REMOVAL_REASON_DLMIMO_ULPCALGO);

	if (!stats->is_gen6) {
		fprintf(stdout, "\nUplink MIMO Groups Counters\n");
		_dump_la_mu_groups_counters(&stats->ulMimoGroupsCounters, REMOVAL_REASON_ULMIMO_MINXPUT, REMOVAL_REASON_ULMIMO_UPHALGO);
	}

	fprintf(stdout, "\n####################################################################\n\n");
}
/*************************************************************************/
