/******************************************************************************
**
** FILE NAME  : ppacmd.h
** PROJECT    : PPA Configuration Utility
** MODULES    : Packet Acceleration
**
** DATE     : 28 Nov 2011
** AUTHOR     : Shao Guohua
** DESCRIPTION  : PPA (Routing Acceleration) User Configuration Utility
** COPYRIGHT  : Copyright © 2020-2025 MaxLinear, Inc.
**              Copyright (c) 2009, Lantiq Deutschland GmbH
**              Am Campeon 3; 85579 Neubiberg, Germany
**
**   For licensing information, see the file 'LICENSE' in the root folder of
**   this software module.
**
**
**
*******************************************************************************/
#ifndef PPACMD_H_
#define PPACMD_H_

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
#define ppa_memset(buff, sz, val) \
  memset_s(buff, sz, val, sz);

#define PPA_TEST_AUTOMATION_ENABLE 0

/* note, we canot include this header file, otherwise uint32_t will conflicts with system defined.
** So we have to define some macro here: */
#define AF_INET     2
#define AF_INET6    10
extern int inet_aton(const char *cp, void *inp);
extern int inet_pton(int af, const char *src, void *dst);

typedef signed char	     int8_t;
typedef short		     int16_t;
typedef int		     int32_t;

#undef CONFIG_IFX_PMCU  //for some macro is not defined in PPA, but PMCU, so here disabled it
#include <net/ppa_api.h>


#define PPA_DEVICE   "/dev/ifx_ppa"
#define PPA_CMD_NAME "ppacmd"

#define PPACMD_DEBUG             1

#if PPACMD_DEBUG
extern int enable_debug ;
#define IFX_PPACMD_DBG(args...) if( enable_debug) printf(args)
#else
#define IFX_PPACMD_DBG(args...)
#endif

#define IFX_PPACMD_PRINT(args...)   printf(args)

#define PPA_CMD_OK          0
#define PPA_CMD_ERR         UINT_MAX
#define PPA_CMD_HELP        2
#define PPA_CMD_DONE        3
#define PPA_CMD_NOT_AVAIL       4


#define OPT_ENABLE_LAN        2
#define OPT_DISABLE_LAN       3
#define OPT_ENABLE_WAN        4
#define OPT_DISABLE_WAN       5

#define OPT_VLAN_AWARE        1
#define OPT_OUTER_VLAN_AWARE    2

#define PPA_MAX_CMD_OPTS      20

#define NIPQUAD(addr) \
  ((unsigned char *)&addr)[0], \
  ((unsigned char *)&addr)[1], \
  ((unsigned char *)&addr)[2], \
  ((unsigned char *)&addr)[3]

//#define ntohs
#undef NIP6
extern uint16_t ntohs(uint16_t netshort);
#define NIP6(addr) \
     ntohs(((unsigned short *)addr)[0]), \
     ntohs(((unsigned short *)addr)[1]), \
     ntohs(((unsigned short *)addr)[2]), \
     ntohs(((unsigned short *)addr)[3]), \
     ntohs(((unsigned short *)addr)[4]), \
     ntohs(((unsigned short *)addr)[5]), \
     ntohs(((unsigned short *)addr)[6]), \
     ntohs(((unsigned short *)addr)[7])
#define NUM_ENTITY(x)               (sizeof(x) / sizeof(*(x)))
//#define ENABLE_IPV6       1


typedef struct ppa_cmd_opts_t
{
    int   opt;
    char *optarg;
} PPA_CMD_OPTS;

typedef struct ppa_cmd_t
{
    const char   *name;                       // command string
    int       ioctl_cmd;                    // ioctl command value
    void     (*print_help)(int);                // function to print help for the command: 0 means first level, 1 mean 2nd level
    int      (*parse_options)(PPA_CMD_OPTS *,PPA_CMD_DATA *);   //
    int      (*do_command)(struct ppa_cmd_t *,PPA_CMD_DATA *);  // function to do error checking on options and fill out PPA_CMD_DATA
    void     (*print_data)(PPA_CMD_DATA *);           // print any returned values from PPA driver
    const struct option *long_opts;                 // long options for command
    const char  *short_opts;                    // short options for command
} PPA_COMMAND;

#define MAX_CMD_OPTS        8

#define STRING_TYPE_INTEGER 0
#define STRING_TYPE_IP 1
#define IP_NON_VALID 0
#define IP_VALID_V4 1
#define IP_VALID_V6 2

#endif
