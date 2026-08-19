/******************************************************************************
**
** FILE NAME  : qoscmd.h
** PROJECT    : QoS Configuration Utility
** MODULES    : Packet Acceleration
**
** DESCRIPTION  : QoS User Configuration Utility
** COPYRIGHT  : Copyright © 2021-2024 MaxLinear, Inc.
**
**   For licensing information, see the file 'LICENSE' in the root folder of
**   this software module.
**
*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <ctype.h>

/*safec v3 standard compatibility*/
#include <safe_str_lib.h>
#include <safe_mem_lib.h>

/* Note: The below macro is used for the new safec v3 standard update.
** This will help in future updates in standard to be incorporated.
*/
#define qos_memset(buff, sz, val) \
  memset_s(buff, sz, val, sz);

/* note, we canot include this header file, otherwise uint32_t will conflicts with system defined.
** So we have to define some macro here: */
#define AF_INET     2
#define AF_INET6    10
extern int inet_pton(int af, const char *src, void *dst);

typedef signed char	     int8_t;
typedef short		     int16_t;
typedef int		     int32_t;

#include <uapi/net/qos_mgr_common.h>


#define QOS_DEVICE   "/dev/qos_mgr"
#define QOS_MGR_CMD_NAME "qoscmd"

#define QOSCMD_DEBUG             1

#if QOSCMD_DEBUG
extern int enable_debug ;
#define IFX_QOSCMD_DBG(args...) if( enable_debug) printf(args)
#else
#define IFX_QOSCMD_DBG(args...)
#endif

#define IFX_QOSCMD_PRINT(args...)   printf(args)

#define QOS_CMD_OK          0
#define QOS_CMD_ERR         UINT_MAX
#define QOS_CMD_HELP        2
#define QOS_CMD_NOT_AVAIL       4
#define QOS_MAX_CMD_OPTS      20

typedef struct qos_cmd_opts_t
{
    int   opt;
    char *optarg;
} QOS_CMD_OPTS;

typedef struct qos_cmd_t
{
    const char   *name;                       // command string
    int       ioctl_cmd;                    // ioctl command value
    void     (*print_help)(int);                // function to print help for the command: 0 means first level, 1 mean 2nd level
    int      (*parse_options)(QOS_CMD_OPTS *,QOS_MGR_CMD_DATA *);   //
    int      (*do_command)(struct qos_cmd_t *,QOS_MGR_CMD_DATA *);  // function to do error checking on options and fill out QOS_CMD_DATA
    void     (*print_data)(QOS_MGR_CMD_DATA *);           // print any returned values from QoS Mgr
    const struct option *long_opts;                 // long options for command
    const char  *short_opts;                    // short options for command
} QOS_COMMAND;

#define QOSCMD_MAX_FILENAME     32
#define STRING_TYPE_INTEGER 0
#define STRING_TYPE_IP 1
#define IP_NON_VALID 0
#define IP_VALID_V4 1
#define IP_VALID_V6 2
