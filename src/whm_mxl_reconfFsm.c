/******************************************************************************

         Copyright (c) 2023 - 2025, MaxLinear, Inc.

  This software may be distributed under the terms of the BSD license.
  See README for more details.

*******************************************************************************/

/*  *****************************************************************************
*         File Name    : whm_mxl_reconfFsm.c                                   *
*         Description  : MXL Reconf FSM Implementation                         *
*                                                                              *
*  *****************************************************************************/

#include "wld/wld_util.h"
#include "wld/wld_radio.h"
#include "swl/swl_common.h"

#include "whm_mxl_reconfFsm.h"
#include "whm_mxl_reconfMngr.h"
#include "whm_mxl_rad.h"
#include "whm_mxl_vap.h"
#include "whm_mxl_utils.h"
#include "whm_mxl_fsmLocker.h"

#define ME "mxlFsm"

#define MXL_FSM_WAIT_FOR_INIT_CYCLE             (1000)
#define MXL_FSM_TRY_LOCK_INTERVAL_MS            (250)
/* 30 Second to attempt to lock the radio - (30 Sec = 100ms * 4 * 30) */
#define NUM_OF_RETRIES_IN_LOCK_INTERVAL_UNITS   (120)
#define MXL_RECONF_FSM_MAX_LOCK_RETRIES         (NUM_OF_RETRIES_IN_LOCK_INTERVAL_UNITS)

const char* s_debug_fsm_state(FSM_STATE state) {
    switch (state) {
        case FSM_IDLE: return "IDLE";
        case FSM_WAIT: return "WAIT";
        case FSM_DEPENDENCY: return "DEPENDENCY";
        case FSM_RUN: return "RUN";
        case FSM_COMPEND: return "COMPEND";
        case FSM_FINISH: return "FINISH";
        case FSM_ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

static bool s_anyCommitsPending(T_Radio* pRad, mxl_VendorData_t* pRadVendor) {

    if (pRadVendor->reconfFsm.FSM_ComPend && areBitsSetLongArray(pRadVendor->reconfFsm.FSM_BitActionArray, FSM_BW)) {
        /* We have pending commits to work on */
        SAH_TRACEZ_NOTICE(ME, "%s: Commits pending (%d) (Request bits pending 0x%08lx // 0x%08lx)", pRad->Name, pRadVendor->reconfFsm.FSM_ComPend,
                           pRadVendor->reconfFsm.FSM_BitActionArray[0], pRadVendor->reconfFsm.FSM_BitActionArray[1]);
        return true;
    } else if (pRadVendor->reconfFsm.FSM_ComPend) {
        SAH_TRACEZ_NOTICE(ME, "%s: Ignore commit pending (%d) without request bits", pRad->Name, pRadVendor->reconfFsm.FSM_ComPend);
        pRadVendor->reconfFsm.FSM_ComPend = 0;
    } else if (areBitsSetLongArray(pRadVendor->reconfFsm.FSM_BitActionArray, FSM_BW)) {
        SAH_TRACEZ_NOTICE(ME, "%s: Ignore request bits 0x%08lx // 0x%08lx - no commit",
                           pRad->Name,
                           pRadVendor->reconfFsm.FSM_BitActionArray[0], pRadVendor->reconfFsm.FSM_BitActionArray[1]);
    } else {
        SAH_TRACEZ_NOTICE(ME, "%s: No commits pending", pRad->Name);
    }
    return false;
}

static bool s_checkAllVapsDone(T_Radio* pRad) {
    T_AccessPoint* pAP;
    wld_rad_forEachAp(pAP, pRad) {
        mxl_VapVendorData_t* pVapVendor = mxl_vap_getVapVendorData(pAP);
        if (pAP && pVapVendor && !whm_mxl_utils_isDummyVap(pAP)) {
            if (areBitsSetLongArray(pVapVendor->reconfFsm.FSM_AC_BitActionArray, FSM_BW)) {
                SAH_TRACEZ_INFO(ME, "%s: Not done 0x%08lx // 0x%08lx", pAP->alias,
                                        pVapVendor->reconfFsm.FSM_AC_BitActionArray[0],
                                        pVapVendor->reconfFsm.FSM_AC_BitActionArray[1]);
                return false;
            }
        }
    }
    SAH_TRACEZ_INFO(ME, "All VAPs are done");
    return true;
}

static void s_preDependency(T_Radio* pRad, wld_fsmMngr_t* reconfMngr, mxl_VendorData_t* pRadVendor) {
    T_AccessPoint* pAP;

    /* Copy requested actions to execution action array */
    longArrayCopy(pRadVendor->reconfFsm.FSM_AC_BitActionArray, pRadVendor->reconfFsm.FSM_BitActionArray, FSM_BW);

    /* Copy requeseted actions to radio action array */
    wld_rad_forEachAp(pAP, pRad) {
        mxl_VapVendorData_t* pVapVendor = mxl_vap_getVapVendorData(pAP);
        if (pAP && pVapVendor && !whm_mxl_utils_isDummyVap(pAP)) {
            longArrayCopy(pVapVendor->reconfFsm.FSM_AC_BitActionArray, pVapVendor->reconfFsm.FSM_BitActionArray, FSM_BW);
            SWL_CALL(reconfMngr->checkVapDependency, pAP, pRad);
        }
    }
}

static void s_printBits(T_Radio* pRad, mxl_VendorData_t* pRadVendor) {
    T_AccessPoint* pAP;
    wld_rad_forEachAp(pAP, pRad) {
        mxl_VapVendorData_t* pVapVendor = mxl_vap_getVapVendorData(pAP);
        if (pAP && pVapVendor && !whm_mxl_utils_isDummyVap(pAP)) {
            SAH_TRACEZ_INFO(ME, "%s: Reconf FSM: VAP Enable(%u) ReqArray: 0x%08lx 0x%08lx // ExecArray: 0x%08lx 0x%08lx",
                               pAP->alias,
                               pAP->enable,
                               pVapVendor->reconfFsm.FSM_BitActionArray[0], pVapVendor->reconfFsm.FSM_BitActionArray[1],
                               pVapVendor->reconfFsm.FSM_AC_BitActionArray[0], pVapVendor->reconfFsm.FSM_AC_BitActionArray[1]);
        }
    }

    SAH_TRACEZ_INFO(ME, "%s: Reconf FSM: RAD Enable(%u) ReqArray: 0x%08lx 0x%08lx // ExecArray: 0x%08lx 0x%08lx",
                       pRad->Name,
                       pRad->enable,
                       pRadVendor->reconfFsm.FSM_BitActionArray[0], pRadVendor->reconfFsm.FSM_BitActionArray[1],
                       pRadVendor->reconfFsm.FSM_AC_BitActionArray[0], pRadVendor->reconfFsm.FSM_AC_BitActionArray[1]);
}

static void s_cleanRequestedBits(T_Radio* pRad, mxl_VendorData_t* pRadVendor) {
    /* Clear the requested action array */
    T_AccessPoint* pAP;
    /* Clean VAPs request action array */
    wld_rad_forEachAp(pAP, pRad) {
        mxl_VapVendorData_t* pVapVendor = mxl_vap_getVapVendorData(pAP);
        if (pAP && pVapVendor && !whm_mxl_utils_isDummyVap(pAP)) {
            longArrayClean(pVapVendor->reconfFsm.FSM_BitActionArray, FSM_BW);
        }
    }
    /* Clean radio request action array */
    longArrayClean(pRadVendor->reconfFsm.FSM_BitActionArray, FSM_BW);
}

static void s_cleanExecBits(T_Radio* pRad, mxl_VendorData_t* pRadVendor) {
    /* Clear the execution action array */
    T_AccessPoint* pAP;
    /* Clean VAPs execution action array */
    wld_rad_forEachAp(pAP, pRad) {
        mxl_VapVendorData_t* pVapVendor = mxl_vap_getVapVendorData(pAP);
        if (pAP && pVapVendor && !whm_mxl_utils_isDummyVap(pAP)) {
            longArrayClean(pVapVendor->reconfFsm.FSM_AC_BitActionArray, FSM_BW);
        }
    }
    /* Clean radio request execution action array */
    longArrayClean(pRadVendor->reconfFsm.FSM_AC_BitActionArray, FSM_BW);
}

static bool s_isValidExecBit(int execBit) {
    SAH_TRACEZ_INFO(ME, "validate exec bit %d", execBit);
    return ((execBit >= 0) && (execBit < RECONF_FSM_MAX));
}

static int s_getMaxReconfRunLoops(T_Radio* pRad) {
    ASSERT_NOT_NULL(pRad, -1, ME, "NULL");
    int numOfInstance = 1 + whm_mxl_utils_getNumOfVaps(pRad); // Radio + Current number of VAPs
    int maxLoops = (numOfInstance * RECONF_FSM_MAX);
    SAH_TRACEZ_INFO(ME, "%s: Max num of loops %d", pRad->Name, maxLoops);
    return maxLoops;
}

static void s_resetReconfFsm(T_Radio* pRad, mxl_VendorData_t* pRadVendor) {
    s_cleanExecBits(pRad, pRadVendor);
    s_cleanRequestedBits(pRad, pRadVendor);
    pRadVendor->reconfFsm.FSM_Loop  = 0;
    pRadVendor->reconfFsm.FSM_Retry = 0;
    pRadVendor->reconfFsm.FSM_Error = 0;
    pRadVendor->reconfFsm.FSM_State = FSM_IDLE;
    pRadVendor->reconfFsmBriefState = MXL_RECONF_FSM_IDLE;
    if (pRadVendor->reconfFsm.timer) {
        amxp_timer_delete(&pRadVendor->reconfFsm.timer);
    }
    pRadVendor->reconfFsm.timeout_msec = 0;
    pRadVendor->reconfFsm.timer = 0;
}

static wld_fsmMngr_t* s_getReconfMngr(mxl_VendorData_t* pRadVendor) {
    ASSERT_NOT_NULL(pRadVendor, NULL, ME, "pRadVendor is NULL");
    ASSERT_NOT_NULL(pRadVendor->reconfFsmMngr, NULL, ME, "reconfFsmMngr is NULL");
    return (wld_fsmMngr_t*)pRadVendor->reconfFsmMngr;
}

static void s_reconf_fsm_th(amxp_timer_t* timer _UNUSED, void* userdata) {
    SAH_TRACEZ_IN(ME);
    T_Radio* pRad = (T_Radio*) userdata;
    ASSERT_NOT_NULL(pRad, , ME, "NULL");
    mxl_VendorData_t* pRadVendor = mxl_rad_getVendorData(pRad);
    ASSERT_NOT_NULL(pRadVendor, , ME, "pRadVendor is NULL");

    whm_mxl_reconf_fsm(pRad);

    if(pRadVendor->reconfFsm.timer && (pRadVendor->reconfFsm.timeout_msec > 0)) {
        SAH_TRACEZ_INFO(ME, "%s: reconf fsm retrigger timer - timeout %d",
                        pRad->Name,
                        pRadVendor->reconfFsm.timeout_msec);
        amxp_timer_start(pRadVendor->reconfFsm.timer, pRadVendor->reconfFsm.timeout_msec);
    }
    SAH_TRACEZ_OUT(ME);
}

FSM_STATE whm_mxl_reconf_fsm(T_Radio* pRad) {
    ASSERT_NOT_NULL(pRad, FSM_FATAL, ME, "NULL");
    mxl_VendorData_t* pRadVendor = mxl_rad_getVendorData(pRad);
    ASSERT_NOT_NULL(pRadVendor, FSM_FATAL, ME, "pRadVendor is NULL");
    wld_fsmMngr_t* reconfMngr = s_getReconfMngr(pRadVendor);
    ASSERT_NOT_NULL(reconfMngr, FSM_FATAL, ME, "reconfMngr is NULL");
    T_AccessPoint* pAP;

    SAH_TRACEZ_INFO(ME, "%s: run reconf fsm in state %u [%s]", pRad->Name,
                                                               pRadVendor->reconfFsm.FSM_State, 
                                                               s_debug_fsm_state(pRadVendor->reconfFsm.FSM_State));

    switch (pRadVendor->reconfFsm.FSM_State) {
        case FSM_IDLE: {
            if (!pRadVendor->reconfFsm.timer) {
                if (amxp_timer_new(&pRadVendor->reconfFsm.timer, s_reconf_fsm_th, pRad)) {
                    SAH_TRACEZ_INFO(ME, "%s: Create new reconf fsm timer", pRad->Name);
                }
            }
            if (pRadVendor->reconfFsm.timer) {
                /* FSM timer is created - start it and move to WAIT state */
                pRadVendor->reconfFsm.timeout_msec = MXL_FSM_TRY_LOCK_INTERVAL_MS;
                amxp_timer_start(pRadVendor->reconfFsm.timer, pRadVendor->reconfFsm.timeout_msec);
                pRadVendor->reconfFsm.FSM_State = FSM_WAIT;
                pRadVendor->reconfFsmBriefState = MXL_RECONF_FSM_RUNNING;
                pRadVendor->reconfFsm.FSM_Error = MXL_RECONF_FSM_REQ_OK; // Used as an indication for DEPENDENCY state
                pRadVendor->reconfFsm.FSM_Retry = MXL_RECONF_FSM_MAX_LOCK_RETRIES; // Used as an indication of try lock attempts
            }
            break;
        }
        case FSM_WAIT: {
            pRadVendor->reconfFsm.FSM_Retry--;
            if (pRadVendor->reconfFsm.FSM_Retry <= 0) {
                /* Try lock timed out - go to ERROR state */
                SAH_TRACEZ_WARNING(ME, "%s: try lock timed out - resetting reconf FSM", pRad->Name);
                pRadVendor->reconfFsm.timeout_msec = 100; // speed up the finish
                pRadVendor->reconfFsm.FSM_State = FSM_ERROR;
                break;
            }

            bool allVapsDone = wld_rad_areAllVapsDone(pRad);
            if (!allVapsDone) {
                    SAH_TRACEZ_WARNING(ME, "%s Cant try lock vaps not done (retry %d)", pRad->Name,
                                                                  pRadVendor->reconfFsm.FSM_Retry);
                    pRadVendor->reconfFsm.timeout_msec = MXL_FSM_WAIT_FOR_INIT_CYCLE; // Increase try lock interval for init phase
                    break;
            }

            /* Speed up try lock interval */
            pRadVendor->reconfFsm.timeout_msec = MXL_FSM_TRY_LOCK_INTERVAL_MS; // try lock interval

            bool lock = reconfMngr->doLock(pRad);
            if (lock) {
                pRadVendor->reconfFsm.FSM_State = FSM_DEPENDENCY;
            }
            /* If cant get lock - stay at WAIT state 
             * Untill success or time out
             */
            break;
        }
        case FSM_DEPENDENCY: {
            reconfMngr->ensureLock(pRad);
            SWL_CALL(reconfMngr->checkPreDependency, pRad);

            /* We work on all commits - zero pending indication */
            pRadVendor->reconfFsm.FSM_ComPend = 0;
            
            /* Check if no error is set by preDependency */
            if (pRadVendor->reconfFsm.FSM_Error == MXL_RECONF_FSM_REQ_OK) {
                s_preDependency(pRad, reconfMngr, pRadVendor);
                SWL_CALL(reconfMngr->checkRadDependency, pRad);
                pRadVendor->reconfFsm.FSM_Loop  = s_getMaxReconfRunLoops(pRad); // Used as an indication of loop limit of RUN state
                pRadVendor->reconfFsm.FSM_State = FSM_RUN;
            } else {
                /* Cant execute requested actions - requirements not met - go to FINISH */
                SAH_TRACEZ_NOTICE(ME, "%s: pre requirements not met - going to FINISH", pRad->Name);
                reconfMngr->doUnlock(pRad); // unlock the FSM
                pRadVendor->reconfFsm.FSM_State = FSM_FINISH;
            }
            s_printBits(pRad, pRadVendor);
            s_cleanRequestedBits(pRad, pRadVendor);
            break;
        }
        case FSM_RUN: {
            bool radExecStatus = true;
            bool allVapsDone = s_checkAllVapsDone(pRad);
            int radExecBit = getLowestBitLongArray(pRadVendor->reconfFsm.FSM_AC_BitActionArray, FSM_BW);

            /* Check if radio and vaps are done executing all requested actions */
            if ((radExecBit < 0) && allVapsDone) {
                pRadVendor->reconfFsm.timeout_msec = 100; // speed up the finish when all done
                pRadVendor->reconfFsm.FSM_State = FSM_COMPEND;
                break;
            }

            if (s_isValidExecBit(radExecBit)) {
                if (reconfMngr->actionList && reconfMngr->actionList[radExecBit].doRadFsmAction) {
                    SAH_TRACEZ_INFO(ME, "%s: Reconf RAD action RUN bit %u : %s", pRad->Name, radExecBit, reconfMngr->actionList[radExecBit].name);
                    radExecStatus = reconfMngr->actionList[radExecBit].doRadFsmAction(pRad);
                }

                if (!radExecStatus) {
                    /* Something went wrong - go to ERROR state */
                    pRadVendor->reconfFsm.timeout_msec = 100; // speed up the finish
                    pRadVendor->reconfFsm.FSM_State = FSM_ERROR;
                }
                clearBitLongArray(pRadVendor->reconfFsm.FSM_AC_BitActionArray, FSM_BW, radExecBit);
            }

            wld_rad_forEachAp(pAP, pRad) {
                bool vapExecStatus = true;
                mxl_VapVendorData_t* pVapVendor = mxl_vap_getVapVendorData(pAP);
                if (pAP && pVapVendor && !whm_mxl_utils_isDummyVap(pAP)) {
                    int vapExecBit = getLowestBitLongArray(pVapVendor->reconfFsm.FSM_AC_BitActionArray, FSM_BW);
                    if (vapExecBit < 0) {
                        continue;
                    }
                    if (s_isValidExecBit(vapExecBit)) {
                        if (reconfMngr->actionList && reconfMngr->actionList[vapExecBit].doVapFsmAction) {
                            SAH_TRACEZ_INFO(ME, "%s: Reconf VAP action RUN bit %u : %s", pAP->alias, vapExecBit, reconfMngr->actionList[vapExecBit].name);
                            vapExecStatus = reconfMngr->actionList[vapExecBit].doVapFsmAction(pAP, pRad);
                        }
                        if (!vapExecStatus) {
                            SAH_TRACEZ_NOTICE(ME, "%s: Failed bit %u in RUN state", pAP->alias, vapExecBit);
                        }
                        clearBitLongArray(pVapVendor->reconfFsm.FSM_AC_BitActionArray, FSM_BW, vapExecBit);
                    }
                }
            }

            /* We cannot stay here forever - check if limit is reached */
            pRadVendor->reconfFsm.FSM_Loop--;
            if ((pRadVendor->reconfFsm.FSM_Loop < 0)) {
                SAH_TRACEZ_WARNING(ME, "%s: Stayed too long in RUN state - resetting reconf FSM", pRad->Name);
                pRadVendor->reconfFsm.timeout_msec = 100; // speed up the finish
                pRadVendor->reconfFsm.FSM_State = FSM_ERROR;
            }
            break;
        }
        case FSM_COMPEND: {
            if (s_anyCommitsPending(pRad, pRadVendor)) {
                /* Need to work on more commits - go to DEPENDENCY - we still have lock */
                pRadVendor->reconfFsm.FSM_State = FSM_DEPENDENCY;
                break;
            }
            /* No pending commits - unlock FSM and go to finish */
            pRadVendor->reconfFsm.timeout_msec = 100; // speed up the finish
            pRadVendor->reconfFsm.FSM_State = FSM_FINISH;
            reconfMngr->doUnlock(pRad);
            break;
        }
        case FSM_FINISH: {
            if (s_anyCommitsPending(pRad, pRadVendor)) {
                /* We have pending commits - lock the FSM and work on commits */
                pRadVendor->reconfFsm.FSM_State = FSM_WAIT;
                pRadVendor->reconfFsm.FSM_Retry = MXL_RECONF_FSM_MAX_LOCK_RETRIES;
                break;
            }
            // make sure we unlock?
            pRadVendor->reconfFsm.FSM_State = FSM_IDLE;
            if (pRadVendor->reconfFsm.timer) {
                amxp_timer_delete(&pRadVendor->reconfFsm.timer);
            }
            pRadVendor->reconfFsm.timeout_msec = 0;
            pRadVendor->reconfFsm.timer = 0;

            pRadVendor->reconfFsmBriefState = MXL_RECONF_FSM_IDLE;
            break;
        }
        case FSM_ERROR: {
            SAH_TRACEZ_WARNING(ME, "%s: Error occured in reconf FSM - reset FSM", pRad->Name);
            reconfMngr->doUnlock(pRad); // unlock FSM
            pRadVendor->reconfFsm.FSM_ComPend = 0;
            s_resetReconfFsm(pRad, pRadVendor);
            s_printBits(pRad, pRadVendor);
            break;
        }
        default:
            break;
    }

    return (pRadVendor->reconfFsm.FSM_State);
}

static void s_fullReconfFsmReset(T_Radio* pRad, mxl_VendorData_t* pRadVendor) {
    SAH_TRACEZ_ERROR(ME, "Reconf FSM in radio %s full reset", pRad->Name);
    s_resetReconfFsm(pRad, pRadVendor);
}

void whm_mxl_reconFsm_allRadioReset(void) {
    T_Radio* pRad = NULL;
    wld_for_eachRad(pRad) {
        mxl_VendorData_t* pRadVendor = mxl_rad_getVendorData(pRad);
        if(pRad && pRadVendor && (pRadVendor->reconfFsmBriefState != MXL_RECONF_FSM_IDLE)) {
            s_fullReconfFsmReset(pRad, pRadVendor);
        }
    }
}


void whm_mxl_reconfFsm_init(T_Radio* pRad) {
    mxl_VendorData_t* pRadVendor = mxl_rad_getVendorData(pRad);
    ASSERT_NOT_NULL(pRadVendor, , ME, "pRadVendor is NULL");
    s_resetReconfFsm(pRad, pRadVendor);
}
