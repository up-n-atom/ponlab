/******************************************************************************

         Copyright (c) 2020, MaxLinear, Inc.
         Copyright 2016 - 2020 Intel Corporation

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

*******************************************************************************/

#include "mtlkinc.h"
#include "rcvry_monitor.h"
#include "argv_parser.h"
#include <stdio.h>
#include <string.h>
#include <signal.h>

#ifndef CONFIG_USE_DWPAL_DAEMON
#define CONFIG_USE_DWPAL_DAEMON 0
#endif /* CONFIG_USE_DWPAL_DAEMON */

#if CONFIG_USE_DWPAL_DAEMON
#if defined YOCTO
#include <wav-dpal/dwpal.h>
#include <wav-dpal/dwpald_client.h>
#else /* YOCTO */
#include "dwpal.h"
#include "dwpald_client.h"
#endif /* YOCTO */
#endif /* CONFIG_USE_DWPAL_DAEMON */

#ifdef CONFIG_WAVE_RTLOG_REMOTE
#include "mtlk_rtlog_app.h"
#endif /* CONFIG_WAVE_RTLOG_REMOTE */

#define LOG_LOCAL_GID GID_RCVRY_MONITOR
#define LOG_LOCAL_FID 1
#define RCVRY_MONITOR_VERSION "1.1"

#define IFACE_NAME_LEN 6 /* wlanX */
#define MAX_FILE_NAME_SIZE 256
#define MAX_CMD_SIZE 1024

#define EVENT_LISTENER "/usr/sbin/dwpal_cli"
#define EVENT_LISTENER_PUMA "/usr/bin/dwpal_cli"

#if CONFIG_USE_DWPAL_DAEMON
struct rcvry_monitor_cfg {
  BOOL terminate;
};

struct rcvry_monitor_cfg g_rm_config;
#endif /* CONFIG_USE_DWPAL_DAEMON */

#ifdef CONFIG_WAVE_RTLOG_REMOTE
static rtlog_app_info_t rtlog_info_data;
#endif /* CONFIG_WAVE_RTLOG_REMOTE */

static void _print_help(const char *app_name)
{
  const struct mtlk_argv_param_info_ex *all_params[] = { };
  const char *app_fname = strrchr(app_name, '/');
  char version[MAX_FILE_NAME_SIZE];
  int sprintf_res = 0;

  if (!app_fname) {
    app_fname = app_name;
  }
  else {
    ++app_fname; /* skip '/' */
  }

  sprintf_res = sprintf_s(version, sizeof(version), "%s: %s\n%s %u %s",
                          "Rcvry Monitor application version",
                          RCVRY_MONITOR_VERSION,
                          "Debug: echo",
                          LOG_LOCAL_OID,
                          "cdebug=1 > /proc/net/mtlk_log/debug");

  if (sprintf_res <= 0 || sprintf_res >= sizeof(version)) {
    ELOG_V("sprintf_s() ERROR");
  }

  ILOG0_S("%s", version);

  mtlk_argv_print_help(stdout,
                       app_fname,
                       version,
                       all_params,
                       (uint32) ARRAY_SIZE(all_params));
}

#if defined YOCTO

#if CONFIG_USE_DWPAL_DAEMON
/* If the wifi subsystem uses dwpal daemon, add the option to restart dwpald. */
#define RCVRY_CMD "/etc/wave/scripts/wave_complete_recovery_rdkb.sh -dwpald"
#else /* CONFIG_USE_DWPAL_DAEMON */
#define RCVRY_CMD "/etc/wave/scripts/wave_complete_recovery_rdkb.sh"
#endif /* CONFIG_USE_DWPAL_DAEMON */

#else /* YOCTO */

/* This is a softlink to wave_complete_recovery_updk.sh or wave_complete_recovery_ugw.sh depending on the platform */
#define RCVRY_CMD "/opt/intel/wave/scripts/wave_complete_recovery.sh"
#endif /* YOCTO */

static int _call_complete_recovery(void)
{
  char sys_cmd[MAX_CMD_SIZE] = RCVRY_CMD;
  int res = MTLK_ERR_UNKNOWN;
  FILE *pf = NULL;
  int status, exit_status;

  /* call the complete recovery script. */
  pf = popen(sys_cmd, "r");
  if (pf == NULL) {
    ELOG_V("Cannot open pipe for output");
    res = MTLK_ERR_FILEOP;
  } else {
    status = pclose(pf);
    pf = NULL;
    if (WIFEXITED(status)) {
      exit_status = WEXITSTATUS(status);
      if (!exit_status) {
        res = MTLK_ERR_OK;
      } else {
        ELOG_SD("popen: cmd: %s exit_status: %i", sys_cmd, exit_status);
        res = MTLK_ERR_NOT_HANDLED;
      }
    }
  }
  return res;
}

#if CONFIG_USE_DWPAL_DAEMON
static int unrecoverable_error_event_handler(/* const */ char *ifname, int drv_event_id, void *data, size_t len)
{
  struct intel_vendor_unrecoverable_error_info unrecoverable_error_nl_event_info;
  int res = MTLK_ERR_UNKNOWN;

  ILOG1_DSD("Received event '%d' from iface '%s' (len=%zu)", drv_event_id, ifname, len);

  if (LTQ_NL80211_VENDOR_EVENT_UNRECOVERABLE_ERROR != drv_event_id) {
    WLOG_D("Unexpected event %d received", drv_event_id);
    res = MTLK_ERR_SYSTEM;
    goto end;
  }

  if (len != sizeof(unrecoverable_error_nl_event_info)) {
    WLOG_DD("Expected event data size %zu but received %zu", sizeof(unrecoverable_error_nl_event_info), len);
    res = MTLK_ERR_VALUE;
    goto end;
  }
  wave_memcpy(&unrecoverable_error_nl_event_info, sizeof(unrecoverable_error_nl_event_info), data, len);

  ILOG1_D("Event received for card_idx (%u)", unrecoverable_error_nl_event_info.card_idx);

  res = _call_complete_recovery();
  if (res != MTLK_ERR_OK) {
    ELOG_D("_call_complete_recovery failed: res: %i", res);
  }

end:
  return res;
}

int rcvry_monitor_dwpald_term_cond_get(void)
{
  return g_rm_config.terminate;
}

static void rcvry_monitor_dwpald_term_cond_set(BOOL val)
{
  g_rm_config.terminate = val;
}

static dwpald_driver_nl_event rcvry_monitor_drv_events[] = {
  { LTQ_NL80211_VENDOR_EVENT_UNRECOVERABLE_ERROR, unrecoverable_error_event_handler },
};

static void _sig_handler(int sig)
{
  rcvry_monitor_dwpald_term_cond_set(TRUE);
}

static void _listener()
{
  dwpald_ret ret;
  int num_attach_attempts = 10;
  char app_name[MAX_FILE_NAME_SIZE];
  size_t num_drv_events = ARRAY_SIZE(rcvry_monitor_drv_events);

  g_rm_config.terminate = FALSE;

  if (sprintf_s(app_name, sizeof(app_name), "rcvry_monitor"/* , card_idx */) <= 0) {
    ELOG_V("sprintf_s() ERROR");
    return;
  }

  ret = dwpald_connect(app_name);
  if (DWPALD_ERROR == ret) {
    ELOG_D("dwpald_connect ERROR %d", ret);
    return;
  }

again:
  ret = dwpald_nl_drv_attach(num_drv_events, rcvry_monitor_drv_events, NULL);
  if (ret != DWPALD_SUCCESS) {
    ELOG_DD("dwpald_nl_drv_attach ERROR %d, num attempts left %d", ret, num_attach_attempts);
    if (num_attach_attempts-- > 0) {
      sleep(1);
      goto again;
    }

    dwpald_disconnect();
    return;
  }

  ret = dwpald_start_blocked_listen(rcvry_monitor_dwpald_term_cond_get);
  if (ret != DWPALD_SUCCESS) {
    ELOG_D("dwpald_start_blocked_listen ERROR %d", ret);
  }

  dwpald_disconnect();
}

#else /* CONFIG_USE_DWPAL_DAEMON */

static BOOL _is_terminated = FALSE;

static void _sig_handler(int sig)
{
  _is_terminated = TRUE;
}

static void _listener()
{
  char sys_cmd[MAX_CMD_SIZE];
  int res = MTLK_ERR_OK;
  int sprintf_res = 0;
  FILE *pf = NULL;
  int status, exit_status;

  /* listen to Driver's UNRECOVERABLE_ERROR event */
  while (res != MTLK_ERR_FILEOP) {
    sprintf_res = sprintf_s(sys_cmd, sizeof(sys_cmd), "EVENT_LISTENER=%s;[ -e $EVENT_LISTENER ] || EVENT_LISTENER=%s;"
                            "$EVENT_LISTENER -iDriver -l\"UNRECOVERABLE_ERROR\"", EVENT_LISTENER, EVENT_LISTENER_PUMA);
    if (sprintf_res <= 0 || sprintf_res >= sizeof(sys_cmd)) {
      ELOG_V("sprintf_s() ERROR");
      return;
    }

    pf = popen(sys_cmd, "r");
    if (pf == NULL) {
      ELOG_V("Cannot open pipe for output");
      res = MTLK_ERR_FILEOP;
      goto end;
    } else {
      status = pclose(pf);
      pf = NULL;
      if (WIFEXITED(status)) {
        exit_status = WEXITSTATUS(status);
        if (!exit_status) {
          /* call the complete recovery script. */
          res = _call_complete_recovery();
          if (res != MTLK_ERR_OK) {
            ELOG_D("_call_complete_recovery failed: res: %i", res);
            goto end;
          }
        } else {
          ELOG_SD("popen: cmd: %s exit status: %i", sys_cmd, exit_status);
          res = MTLK_ERR_NOT_HANDLED;
        }
      }

      /* Exit on received signal */
      if (_is_terminated) {
        goto end;
      }
    } // if - else
  } // while

end:
  return;
}
#endif /* CONFIG_USE_DWPAL_DAEMON */

static int _set_sigaction(void)
{
  struct sigaction act;

  memset(&act, '\0', sizeof(act));
  act.sa_handler = &_sig_handler;
  if (sigaction(SIGTERM, &act, NULL) < 0) {
    perror("sigaction");
    return 1;
  }

  return 0;
}

int main(int argc, char *argv[])
{
  int res = MTLK_ERR_UNKNOWN;
  BOOL print_help = FALSE;

  ILOG0_SD("Rcvry Monitor application v.%s [pid=%d]",
           RCVRY_MONITOR_VERSION, (int)getpid());

  if ((argc > 1) &&
      ((!strcmp(argv[1], "-h")) ||
       (!strcmp(argv[1], "--help")) ||
       (!strcmp(argv[1], "help")))) {
    print_help = TRUE;
    goto end;
  }

  if (MTLK_ERR_OK != _mtlk_osdep_log_init(IWLWAV_RTLOG_APP_NAME_RCVRYMONITOR)) {
    ELOG_V("Rcvry Monitor _mtlk_osdep_log_init ERROR");
    print_help = TRUE;
    goto end;
  }

#ifdef CONFIG_WAVE_RTLOG_REMOTE
  if (MTLK_ERR_OK != mtlk_rtlog_app_init(&rtlog_info_data, IWLWAV_RTLOG_APP_NAME_RCVRYMONITOR)) {
    ELOG_V("Rcvry Monitor application mtlk_rtlog_app_init ERROR");
    print_help = TRUE;
    goto end;
  }
#endif /* CONFIG_WAVE_RTLOG_REMOTE */

  if (_set_sigaction() != 0) {
    ELOG_V("sigaction() ERROR");
    res = MTLK_ERR_UNKNOWN;
    goto end;
  }

  _listener();
  res = MTLK_ERR_OK;

end:
  if (print_help)
    _print_help(argv[0]);

  ILOG0_V("Rcvry Monitor application exit");

  return res;
}
