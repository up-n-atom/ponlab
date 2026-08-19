/******************************************************************************

         Copyright (c) 2023 - 2025, MaxLinear, Inc.

  This software may be distributed under the terms of the BSD license.
  See README for more details.

*******************************************************************************/

/*  *****************************************************************************
*         File Name    : whm_mxl_fsmLocker.c                                   *
*         Description  : MXL Ext Locker API                                    *
*                                                                              *
*  *****************************************************************************/

#include "whm_mxl_fsmLocker.h"
#include "whm_mxl_reconfMngr.h"
#include "whm_mxl_rad.h"

#define ME "mxlLck"

#define RADIO_LOCK                          (0x0100)
#define RADIO_INDEX_MASK                    (0x00FF)
#define GENERIC_FSM_OFFSET_IN_LOCK_BITMAP   (0x0)
#define RECONF_FSM_OFFSET_IN_LOCK_BITMAP    (0x4)

/*
*   Locking bitmap is as follows:
*
*   |----------------------------------------------------------------------------------------------------------|
*   | BIT               | b'8  |   b'7   |   b'6   |   b'5   |  b'4    |   b'3   |   b'2   |   b'1   |   b'0   |
*   |----------------------------------------------------------------------------------------------------------|
*   |   Per FMS         |      | Reconf  | Reconf  | Reconf  | Reconf  | Generic | Generic | Generic | Generic |
*   |   Radio           | LOCK | FSM     | FSM     | FSM     | FSM     | FSM     | FSM     | FSM     | FSM     |
*   |   Bit Index       |      | radio 3 | radio 2 | radio 1 | radio 0 | radio 3 | radio 2 | radio 1 | radio 0 |
*   |----------------------------------------------------------------------------------------------------------|
*	
*/

static int s_radFSMLockBitMap = 0;
static int s_radFSMWaiting = 0;

static bool s_lock(T_Radio* pRad, int bitmask) {
    if ((s_radFSMLockBitMap & RADIO_INDEX_MASK) == bitmask) {
        SAH_TRACEZ_ERROR(ME, "%s: requesting lock while has lock 0x%x", pRad->Name, s_radFSMLockBitMap);
        return true;
    }

    if (s_radFSMLockBitMap) {
        s_radFSMWaiting |= bitmask;
        return false;
    } else {
        s_radFSMLockBitMap = RADIO_LOCK | bitmask;
        s_radFSMWaiting &= ~(bitmask);
        return true;
    }
}

static void s_unLock(T_Radio* pRad, int bitmask) {
    if ((s_radFSMLockBitMap & RADIO_INDEX_MASK) != bitmask) {
        SAH_TRACEZ_ERROR(ME, "%s: freeing lock while not has lock 0x%0x", pRad->Name, s_radFSMLockBitMap);
        return;
    }
    s_radFSMLockBitMap = 0;
}

static void s_ensureLock(T_Radio* pRad, int bitmask) {
    if ((s_radFSMLockBitMap & RADIO_INDEX_MASK) != bitmask) {
        SAH_TRACEZ_ERROR(ME, "%s: Checking lock while not has lock 0x%0x", pRad->Name, s_radFSMLockBitMap);
    }
}

static bool s_genericFsmLock_ext(T_Radio* pRad) {
    ASSERT_NOT_NULL(pRad, false, ME, "NULL");
    int radFsmIdx = ((1 << pRad->ref_index) << GENERIC_FSM_OFFSET_IN_LOCK_BITMAP);
    bool lockStatus = s_lock(pRad, radFsmIdx);
    SAH_TRACEZ_INFO(ME, "%s: Generic FSM Lock Req for idx:0x%x lockStatus %d lockBitmap:0x%x WaitingBitmap:0x%x",
                                                                            pRad->Name, radFsmIdx, lockStatus,
                                                                            s_radFSMLockBitMap, s_radFSMWaiting);
    return lockStatus;
}

static void s_genericFsmUnLock_ext(T_Radio* pRad) {
    ASSERT_NOT_NULL(pRad, , ME, "NULL");
    int radFsmIdx = ((1 << pRad->ref_index) << GENERIC_FSM_OFFSET_IN_LOCK_BITMAP);
    s_unLock(pRad, radFsmIdx);
    SAH_TRACEZ_INFO(ME, "%s: Generic FSM UnLock Req for idx:0x%x lockBitmap:0x%x WaitingBitmap:0x%x",
                                                                pRad->Name, radFsmIdx,
                                                                s_radFSMLockBitMap, s_radFSMWaiting);
}

static void s_genericFsmEnsureLock_ext(T_Radio* pRad) {
    ASSERT_NOT_NULL(pRad, , ME, "NULL");
    int radFsmIdx = ((1 << pRad->ref_index) << GENERIC_FSM_OFFSET_IN_LOCK_BITMAP);
    s_ensureLock(pRad, radFsmIdx);
}

bool whm_mxl_reconfFsmLock(T_Radio* pRad) {
    ASSERT_NOT_NULL(pRad, false, ME, "NULL");
    int radFsmIdx = ((1 << pRad->ref_index) << RECONF_FSM_OFFSET_IN_LOCK_BITMAP);
    bool lockStatus = s_lock(pRad, radFsmIdx);
    SAH_TRACEZ_INFO(ME, "%s: Reconf FSM Lock Req for idx:0x%x lockStatus %d lockBitmap:0x%x WaitingBitmap:0x%x",
                                                                            pRad->Name, radFsmIdx, lockStatus,
                                                                            s_radFSMLockBitMap, s_radFSMWaiting);
    return lockStatus;
}

void whm_mxl_reconfFsmUnLock(T_Radio* pRad) {
    ASSERT_NOT_NULL(pRad, , ME, "NULL");
    int radFsmIdx = ((1 << pRad->ref_index) << RECONF_FSM_OFFSET_IN_LOCK_BITMAP);
    s_unLock(pRad, radFsmIdx);
    SAH_TRACEZ_INFO(ME, "%s: Reconf FSM UnLock Req for idx:0x%x lockBitmap:0x%x WaitingBitmap:0x%x",
                                                                pRad->Name, radFsmIdx,
                                                                s_radFSMLockBitMap, s_radFSMWaiting);
}

void whm_mxl_reconfFsmEnsureLock(T_Radio* pRad) {
    ASSERT_NOT_NULL(pRad, , ME, "NULL");
    int radFsmIdx = ((1 << pRad->ref_index) << RECONF_FSM_OFFSET_IN_LOCK_BITMAP);
    s_ensureLock(pRad, radFsmIdx);
}

/* Should be use only on vendor module init */
static void s_resetLocker(void) {
    s_radFSMLockBitMap = 0;
    s_radFSMWaiting = 0;
}

/**
 * @brief Attach generic FSM ext locking callbacks to MxL shared locker
 * 
 * @param fsmMngr Pointer to generic fsmManager
 * @return None
 */
void whm_mxl_extLocker_init(wld_fsmMngr_t* fsmMngr) {
    ASSERT_NOT_NULL(fsmMngr, , ME, "fsmMngr is NULL");
    s_resetLocker();
    fsmMngr->doLock = s_genericFsmLock_ext;
    fsmMngr->doUnlock = s_genericFsmUnLock_ext;
    fsmMngr->ensureLock = s_genericFsmEnsureLock_ext;
}
