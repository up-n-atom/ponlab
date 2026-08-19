/******************************************************************************

         Copyright (c) 2023 - 2025 MaxLinear, Inc.

  This software may be distributed under the terms of the BSD license.
  See README for more details.

*******************************************************************************/

/* *****************************************************************************
*         File Name    : whm_mxl_wmm.c                                         *
*         Description  : WMM statistics related API                            *
*                                                                              *
*  *****************************************************************************/

#include <swl/swl_common.h>

#include "wld/wld.h"

#define ME "mxlWmm"


#define MAX_NUM_WMM_QUEUES         5 /* Default (needed for verification), Background, Best Effort, Video, Voice */
#define WMM_QUEUE_FILE             "/sys/kernel/debug/ppa/qos_helper/mark-to-queue"
/*
 * Example mark-to-queue format:
 * ...
 * wlan0.1 qos data:
 * Mark:   0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   15
 * DP(q): 55   56   57   58   59   55   55   55   55   55   55   55   55   55   55   55
 * ...
 *
 * Mark 0: Default queue     (55) : will be stored locally at index 4
 * Mark 1: Best Effort queue (56) : will be stored locally at index 0 to match WLD_AC_BE
 * Mark 2: Background queue  (57) : will be stored locally at index 1 to match WLD_AC_BK
 * Mark 3: Video queue       (58) : will be stored locally at index 2 to match WLD_AC_VI
 * Mark 4: Voice queue       (59) : will be stored locally at index 3 to match WLD_AC_VO
 */

#define WMM_STATS_FILE "/sys/kernel/debug/pp/qos_queues_stats"
/*
 * Example qos_queues_stats format:
 * ...
 * +------------------+-------------+-------------+-------------+-------------+
 * |       Qnum       | Q Occupancy |   Forward   |  WRED Drop  | CoDel Drop  |
 * +------------------+-------------+-------------+-------------+-------------+
 * | 213(1008)-rlm-55 | 0           | 1715        | 0           | 0           |
 * | 218(1040)-rlm-56 | 0           | 164         | 0           | 0           |
 * | 219(1041)-rlm-57 | 0           | 82          | 0           | 0           |
 * | 220(1042)-rlm-58 | 0           | 339         | 0           | 0           |
 * | 221(1043)-rlm-59 | 0           | 596         | 0           | 0           |
 * +------------------+-------------+-------------+-------------+-------------+
 * ...
 *
 * Qnum:      Ends with number which maps to the Marks (read from mark-to-queue)
 * Forward:   Number of successful TX packets
 * WRED Drop: Number of failed TX packets
 *
 * NOTE: no more other stats (like RX or Bytes) available right now
 */

swl_rc_ne mxl_getWmmStats(T_AccessPoint* pAP, T_Stats *stats, bool add)
{
    ASSERT_TRUE(stats, SWL_RC_INVALID_PARAM, ME, "Pointer to stats is NULL");
    FILE *fp = fopen(WMM_QUEUE_FILE, "r");
    ASSERT_NOT_NULL(fp, SWL_RC_ERROR, ME, "%s: Error opening file", pAP->alias);
    char line[128] = {0}; /* lines are up to 85 chars long, so size 128 should be fine */
    bool found_interface = false;
    int i, num_entries;
    swl_rc_ne rc = SWL_RC_ERROR;
    char iface_marker[64] = {0};

    snprintf(iface_marker, sizeof(iface_marker), "%s qos data:", pAP->alias);
    while (fgets(line, sizeof(line), fp)) {
        if (strstr(line, iface_marker)) {
            found_interface = true;
            break;
        }
    }

    if (!found_interface) {
        SAH_TRACEZ_INFO(ME, "%s: Interface not found", pAP->alias);
        rc = SWL_RC_OK; /* ToDo: num ifaces may be limited by platform, hence return OK? */
        goto end;
    }

    /* Read 'Mark:' line */
    if (!fgets(line, sizeof(line), fp)) {
        SAH_TRACEZ_ERROR(ME, "%s: Error reading Mark line", pAP->alias);
        goto end;
    }

    /* Read 'DP(q):' line */
    if (!fgets(line, sizeof(line), fp)) {
        SAH_TRACEZ_ERROR(ME, "%s: Error reading DP(q) line", pAP->alias);
        goto end;
    }

    /* Check 'DP(q):' line format and get values */
    int queu_num[MAX_NUM_WMM_QUEUES] = {0};
    num_entries = sscanf(line, "DP(q): %d %d %d %d %d", &queu_num[4], &queu_num[0],
                         &queu_num[1], &queu_num[2], &queu_num[3]);
    if (num_entries != MAX_NUM_WMM_QUEUES) {
        SAH_TRACEZ_ERROR(ME, "%s: Error: Invalid DP(q) line format (%d)(%s)", pAP->alias, num_entries, line);
        goto end;
    }

    char wmm_marker[WLD_AC_MAX][16];
    /* If default queue number mathes any WMM queue number, DP is not configured properly */
    for (i = 0; i < SWL_MIN(WLD_AC_MAX, MAX_NUM_WMM_QUEUES); i++) {
        if (queu_num[4] == queu_num[i]) {
            SAH_TRACEZ_INFO(ME, "%s: Queues not configured?", pAP->alias);
            rc = SWL_RC_OK; /* ToDo: return error after full platform integration? */
            goto end;
        }
        snprintf(wmm_marker[i], sizeof(wmm_marker[i]), ")-rlm-%d", queu_num[i]);
    }
    fclose(fp);

    /* Get the actual statistics */
    fp = fopen(WMM_STATS_FILE, "r");
    ASSERT_NOT_NULL(fp, SWL_RC_ERROR, ME, "%s: Error opening file", pAP->alias);

    uint32_t tx_packets;
    uint32_t tx_packets_fail;
    while (fgets(line, sizeof(line), fp)) {
        for (i = 0; i < WLD_AC_MAX; i++) {
            if (!strstr(line, wmm_marker[i]))
                continue;

            num_entries = sscanf(line, "|%*[^|]|%*[^|]|%u |%u", &tx_packets, &tx_packets_fail);
            if (num_entries != 2) {
                SAH_TRACEZ_ERROR(ME, "%s: Cannot extract %d statistics", pAP->alias, i);
                goto end;
            }

            if (add) {
                stats->WmmPacketsSent[i] += tx_packets;
                stats->WmmFailedSent[i] += tx_packets_fail;
            }
            else {
                stats->WmmPacketsSent[i] = tx_packets;
                stats->WmmFailedSent[i] = tx_packets_fail;
            }
        }
    }

    fclose(fp);
    return SWL_RC_OK;

end:
    fclose(fp);
    return rc;
}
