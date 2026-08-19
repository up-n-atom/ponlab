/******************************************************************************
**
** FILE NAME  : ppacmd.c
** PROJECT    : PPA Configuration Utility
** MODULES    : Packet Acceleration
**
** DATE     : 10 JUN 2008
** AUTHOR     : Mark Smith
** DESCRIPTION  : PPA (Routing Acceleration) User Configuration Utility
** COPYRIGHT  : Copyright (c) 2020-2025 MaxLinear, Inc.
**              Copyright (c) 2009, Lantiq Deutschland GmbH
**              Am Campeon 3; 85579 Neubiberg, Germany
**
**   For licensing information, see the file 'LICENSE' in the root folder of
**   this software module.
**
** HISTORY
** $Date        $Author                $Comment
** 10 DEC 2012  Manamohan Shetty       Added the support for RTP,MIB mode  
**                                     Features 
**
*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <getopt.h>
#include <uapi/asm-generic/ioctl.h>
#include <errno.h>
#include <ctype.h>

#include "ppacmd.h"
#include "ppacmd_autotest.h"

int enable_debug = 0;
char *debug_enable_file="/var/ppa_gdb_enable";

static int g_all = 0;

#define MAX_STR_IP	64
#define MAX_MEM_OFFSET	32

static void ppa_print_help(void);
static int ppa_parse_simple_cmd(PPA_CMD_OPTS *popts, PPA_CMD_DATA *pdata);

int32_t isValidMacAddress(const uint8_t* mac) {
    uint32_t i = 0;
    uint32_t s = 0;

    while (*mac) {
       if (isxdigit(*mac)) {
          i++;
       }
       else if (*mac == ':' || *mac == '-') {

          if (i == 0 || i / 2 - 1 != s)
            break;

          ++s;
       }
       else {
           s = -1;
       }

       ++mac;
    }

    return (i == 12 && (s == 5 || s == 0));
}
static void print_session_flags( uint32_t flags)
{
    static const char *str_flag[] =
    {
        "IS_REPLY",                 //  0x00000001
        "Reserved",
        "SESSION_IS_TCP",
        "STAMPING",
        "ADDED_IN_HW",              //  0x00000010
        "NOT_ACCEL_FOR_MGM",
        "STATIC",
        "DROP",
        "VALID_NAT_IP",             //  0x00000100
        "VALID_NAT_PORT",
        "VALID_NAT_SNAT",
        "NOT_ACCELABLE",
        "VALID_VLAN_INS",           //  0x00001000
        "VALID_VLAN_RM",
        "SESSION_VALID_OUT_VLAN_INS",
        "SESSION_VALID_OUT_VLAN_RM",
        "VALID_PPPOE",              //  0x00010000
        "VALID_NEW_SRC_MAC",
        "VALID_MTU",
        "VALID_NEW_DSCP",
        "SESSION_VALID_DSLWAN_QID", //  0x00100000
        "SESSION_TX_ITF_IPOA",
        "SESSION_TX_ITF_PPPOA",
        "Reserved",
        "SRC_MAC_DROP_EN",          //  0x01000000
        "SESSION_TUNNEL_6RD",
        "SESSION_TUNNEL_DSLITE",
        "Reserved",
        "LAN_ENTRY",                //  0x10000000
        "WAN_ENTRY",
        "IPV6",
        "Reserved",
    };

    int flag;
    unsigned long bit;
    int i;


    flag = 0;
    for ( bit = 1, i = 0; bit; bit <<= 1, i++ )
    {
        if ( (flags & bit) )
        {
            if ( flag++ )
                IFX_PPACMD_PRINT( "|");
            IFX_PPACMD_PRINT( "%s",str_flag[i]);
            //IFX_PPACMD_PRINT( " ");
        }
    }

}
int stricmp(const char * p1, const char * p2)
{
    int c1, c2;

    while ( *p1 && *p2 )
    {
        c1 = *p1 >= 'A' && *p1 <= 'Z' ? *p1 + 'a' - 'A' : *p1;
        c2 = *p2 >= 'A' && *p2 <= 'Z' ? *p2 + 'a' - 'A' : *p2;
        if ( (c1 -= c2) )
            return c1;
        p1++;
        p2++;
    }

    return *p1 - *p2;
}

int strincmp(const char *p1, const char *p2, int n)
{
    int c1 = 0, c2;

    while ( n && *p1 && *p2 )
    {
        c1 = *p1 >= 'A' && *p1 <= 'Z' ? *p1 + 'a' - 'A' : *p1;
        c2 = *p2 >= 'A' && *p2 <= 'Z' ? *p2 + 'a' - 'A' : *p2;
        if ( (c1 -= c2) )
            return c1;
        p1++;
        p2++;
        n--;
    }

    return n ? *p1 - *p2 : c1;
}


/*Note:
  if type == STRING_TYPE_INTEGER, directly return the value
  if type == STRING_TYPE_IP, return value definition as below:
                       IP_NON_VALID(0): means non-valid IP
                       IP_VALID_V4(1): means valid IPV4 address
                       IP_VALID_V6(2) :means valid IPV6 address
*/

unsigned long int str_convert(int type, const char *nptr, void *buff )
{
    char *endPtr;
    unsigned long int res;

    if( nptr == NULL )
    {
        IFX_PPACMD_PRINT("str_convert: nptr is NULL ??\n");
        return 0;
    }
    if( type == STRING_TYPE_IP )
    {
        if( (res = inet_pton(AF_INET, nptr, buff) ) == 0 )
        {
            if( (res = inet_pton(AF_INET6, nptr, buff)) == 0 )
            {
                return IP_NON_VALID;
            }
            else return IP_VALID_V6;
        }
        else return IP_VALID_V4;

        return 0;
    }
    else if( type == STRING_TYPE_INTEGER )
    {
        if( strlen(nptr) >= 2 && nptr[0] == '0' && ( nptr[1] == 'x' || nptr[1] == 'X') ) /*hex value start with 0x */
        {
            res = strtoul(nptr, &endPtr, 16);
		if (endPtr == NULL)
			res = PPA_CMD_ERR;
		else
			return res;
        }
        else  /*by default it is decimal value */
        {
            res = strtoul(nptr, &endPtr, 10);
		if (endPtr == NULL)
			res = PPA_CMD_ERR;
		else
			return res;
        }
    }
    else
    {
        IFX_PPACMD_PRINT("str_convert: wrong type parameter(%d)\n", type);
        return 0;
    }
	return PPA_CMD_OK;
}


static int is_digital_value(char *s)
{
    int i;
    
    if( !s ) return 0;

     if( (strlen(s) > 2 ) && ( s[0] == '0')  && ( s[1] == 'x') )
    { //hex
        for(i=2; i<strlen(s); i++ )
        { //
            if( ( s[i] >='0' &&  s[i] <='9' )  || ( s[i] >='a' &&  s[i] <='f' ) || ( s[i] >='A' &&  s[i] <='F' ) )
                continue;
            else 
                return 0;
        }
    }
    else
    { //normal value
        for(i=0; i<strlen(s); i++ )
            if( s[i] >='0' &&  s[i] <='9' )  continue;
            else
                return 0; 
    }
    return 1;
}

/*
===========================================================================================
   ppa_do_ioctl_cmd

===========================================================================================
*/
static int ppa_do_ioctl_cmd(int ioctl_cmd, void *data)
{
    int ret = PPA_CMD_OK;
    int fd  = 0;

    if ((fd = open (PPA_DEVICE, O_RDWR)) < 0)
    {
        IFX_PPACMD_PRINT ("\n [%s] : open PPA device (%s) failed. (errno=%d)\n", __FUNCTION__, PPA_DEVICE, errno);
        ret = PPA_CMD_ERR;
    }
    else
    {
        if (ioctl (fd, ioctl_cmd, data) < 0)
        {
            IFX_PPACMD_PRINT ("\n [%s] : ioctl failed for NR %d. (errno=%d(system error:%s))\n", __FUNCTION__, _IOC_NR(ioctl_cmd), errno, strerror(errno));
            ret = PPA_CMD_ERR;
        }
        close (fd);
    }
    
    return ret;
}

/*
====================================================================================
   Input conversion functions
   These sunctions convert input strings to the appropriate data types for
   the ioctl commands.
===================================================================================
*/
static void stomac(char *s,unsigned char mac_addr[])
{
    unsigned int mac[PPA_ETH_ALEN];

    sscanf(s,"%2x:%2x:%2x:%2x:%2x:%2x",&mac[0],&mac[1],&mac[2],&mac[3],&mac[4],&mac[5]);

    mac_addr[5] = mac[5];
    mac_addr[4] = mac[4];
    mac_addr[3] = mac[3];
    mac_addr[2] = mac[2];
    mac_addr[1] = mac[1];
    mac_addr[0] = mac[0];
    return;
}

/*
====================================================================================
   Generic option sets
   These option sets are shared among several commands.
===================================================================================
*/

static const char ppa_no_short_opts[] = "-:h";
static struct option ppa_no_long_opts[] =
{
    { 0,0,0,0 }
};

static const char ppa_if_short_opts[] = "-:i:fl:wh";
static struct option ppa_if_long_opts[] =
{
    {"interface", required_argument,  NULL, 'i'},
    {"force", no_argument,  NULL, 'f'}, //-f is used for addlan and addwan only
    {"lower", required_argument,  NULL, 'l'}, //-l is used for manually configure its lower interface
    {"hardware", no_argument,  NULL, 'w'}, // -w is used to disable HW acceleration for interface (SW acceleration would still work for that interface)
    { 0,0,0,0 }
};

static const char ppa_mac_short_opts[] = "-:m:h";
static const struct option ppa_mac_long_opts[] =
{
    {"macaddr",   required_argument,  NULL, 'm'},
    { 0,0,0,0 }
};

static const char ppa_if_mac_short_opts[] = "-:i:m:h";
static const struct option ppa_if_mac_long_opts[] =
{
    {"interface", required_argument,  NULL, 'i'},
    {"macaddr",   required_argument,  NULL, 'm'},
    { 0,0,0,0 }
};

static const char ppa_output_short_opts[] = "-:h";

static const char ppa_if_output_short_opts[] = "-:i:h";

#if defined(MIB_MODE_ENABLE)
static const char ppa_init_short_opts[] = "-:f:l:w:b:m:n:i:h";
#else
static const char ppa_init_short_opts[] = "f:l:w:b:m:n:h";
#endif
static const struct option ppa_init_long_opts[] =
{
    {"file",   required_argument,  NULL, 'f'},
    {"lan",    required_argument,  NULL, 'l'},
    {"wan",    required_argument,  NULL, 'w'},
    {"bridge",   required_argument,  NULL, 'b'},
    {"multicast",required_argument,  NULL, 'm'},
    {"minimal-hit",    required_argument,  NULL, 'n'},
#if defined(MIB_MODE_ENABLE)
    {"mib-mode", required_argument,  NULL, 'i'},
#endif
    {"help",   no_argument,    NULL, 'h'},
    { 0,0,0,0 }
};

static const char ppa_if_mib_short_opts[] = "-:i:h";

static int ppa_do_cmd(PPA_COMMAND *pcmd, PPA_CMD_DATA *pdata)
{
    return ppa_do_ioctl_cmd(pcmd->ioctl_cmd,pdata);
}

static int ppa_do_add_del_if_cmd(PPA_COMMAND *pcmd, PPA_CMD_DATA *pdata)
{
    int idx = 0;
    for (idx = 0; idx < pdata->all_if_info.num_ifinfos; idx++)
        ppa_do_ioctl_cmd(pcmd->ioctl_cmd, &(pdata->all_if_info.ifinfo[idx]));
    return 0;
}

/*
====================================================================================
   command:   init
   description: Initialize the Packet Processing Acceleration Module
   options:   None
====================================================================================
*/

typedef enum
{
    INIT_CMD_INVALID = -1,
    INIT_CMD_COMMENT,
    INIT_CMD_SECTION,
    INIT_CMD_END,
    INIT_CMD_NUM_ENTRIES,
    INIT_CMD_MULTICAST_ENTRIES,
    INIT_CMD_BRIDGE_ENTRIES,
    INIT_CMD_TCP_HITS_TO_ADD,
    INIT_CMD_UDP_HITS_TO_ADD,
#if defined(CONFIG_PPA_HANDLE_CONNTRACK_SESSIONS)
    INIT_CMD_LAN_COLLISION,
    INIT_CMD_WAN_COLLISION,
#endif
    INIT_CMD_INTERFACE
} INIT_CMD;

const char *cfg_names[] =
{
    "comment",
    "section",
    "end",
    "ip-header-check",
    "tcp-udp-header-check",
    "drop-on-error",
    "unicast-drop-on-miss",
    "multicast-drop-on-miss",
    "max-unicast-sessions",
    "max-multicast-sessions",
    "max-bridging-sessions",
    "tcp-threshold",
    "udp-threshold",
#if defined(CONFIG_PPA_HANDLE_CONNTRACK_SESSIONS)
    "lan-collision",
    "wan-collision",
#endif
    "interface"
};

typedef enum
{
    SECTION_NONE,
    SECTION_WAN,
    SECTION_LAN
} SECTION_NAME;

typedef enum
{
    ERR_MULTIPLE_SECTIONS,
    ERR_INVALID_SECTION_NAME,
    ERR_INVALID_COMMAND,
    ERR_NOT_IN_SECTION,
    ERR_IN_SECTION,
    ERR_INVALID_RANGE
} INIT_FILE_ERR;

static void ppa_print_init_help(int summary)
{
    if( summary )
    {
#if defined(MIB_MODE_ENABLE)
        
        IFX_PPACMD_PRINT("init [-f <filename>] [-l <lan_num>] [-w <wan_num>] [-m <mc_num>] [-b <br_num>] [ -n minimal-hit ] [-i <mib-mode> ]\n");

#else
        IFX_PPACMD_PRINT("init [-f <filename>] [-l <lan_num>] [-w <wan_num>] [-m <mc_num>] [-b <br_num>] [ -n minimal-hit ] \n");
#endif
        IFX_PPACMD_PRINT("    -l/w/m/b: to set maximum LAN/WAN/Multicast/Bridge Acceeration entries\n");
        IFX_PPACMD_PRINT("    -n: to specify minimal hit before doing acceleration\n");
#if defined(MIB_MODE_ENABLE)
        IFX_PPACMD_PRINT("    -i: to set mibmode 0-Session MIB in terms of Byte,1- in terms of Packet\n");
#endif
        IFX_PPACMD_PRINT("    -f: note, if -f option is used, then no need to use other options\n");
        
    }
    else
        IFX_PPACMD_PRINT("init [-f <filename>]\n");
    return;
}

static INIT_CMD parse_init_config_line(char *buf, char **val)
{
    char *p_cmd, *p_end;
    int ndx;
    INIT_CMD ret_cmd = INIT_CMD_INVALID;
    int eol_seen = 0;

    if (buf[0] == '#')
        return INIT_CMD_COMMENT;

    p_cmd = buf;
    while (*p_cmd != '\n' && isspace(*p_cmd))   // skip leading white space while checking for eol
        p_cmd++;
    if (*p_cmd == '\n')
        return INIT_CMD_COMMENT;         // empty line
    p_end = p_cmd;                // null terminate the command
    while (!isspace(*p_end))
        p_end++;
    if (*p_end == '\n')
        eol_seen = 1;
    *p_end = '\0';

    for (ndx = 0; ndx < (sizeof(cfg_names)/sizeof(char *)); ndx++)
    {
        if ( strcasecmp(cfg_names[ndx], p_cmd ) == 0)
        {
            // return the following string if present
            if (!eol_seen)
            {
                p_cmd = p_end + 1;
                while (*p_cmd != '\n' && isspace(*p_cmd))
                    p_cmd++;
                p_end = p_cmd;
                while (!isspace(*p_end))
                    p_end++;
                *p_end = '\0';
                *val = p_cmd;
            }
            else
            {
                *val = NULL;   // no parameter present
            }
            ret_cmd = ndx;
            break;
        }
    }
    return ret_cmd;
}

#define	CMD_LINE_SIZE	128

static int parse_init_config_file(char *filename, PPA_CMD_INIT_INFO *pinfo)
{
    FILE *fd;
    char cmd_line[CMD_LINE_SIZE];
    char *val = NULL;
    int linenum = 0;
    INIT_CMD cmd;
    int num_entries, num_hits;
#if defined(CONFIG_PPA_HANDLE_CONNTRACK_SESSIONS)
    int lan_collisions,wan_collisions;
#endif
    SECTION_NAME curr_section = SECTION_NONE;
    int seen_wan_section = 0, seen_lan_section = 0;
    //int if_index = 0;
    int num_wanifs = 0, num_lanifs = 0;
    INIT_FILE_ERR err;

    fd = fopen(filename,"r");
    if (fd != NULL)
    {
        while ( fgets(cmd_line, 128, fd) != NULL)
        {
            linenum++;
            cmd = parse_init_config_line(cmd_line, &val);
            if(val == NULL)
            {
		err = ERR_INVALID_SECTION_NAME;
                goto parse_error;
            }
            switch(cmd)
            {
            case INIT_CMD_COMMENT:
                break;

            case INIT_CMD_SECTION:
                if (!strcasecmp("wan", val))
                {
                    if(seen_wan_section)
                    {
                        err = ERR_MULTIPLE_SECTIONS;
                        goto parse_error;
                    }
                    curr_section = SECTION_WAN;
                }
                else if (!strcasecmp("lan", val))
                {
                    if (seen_lan_section)
                    {
                        err = ERR_MULTIPLE_SECTIONS;
                        goto parse_error;
                    }
                    curr_section = SECTION_LAN;
                }
                else
                {
                    err = ERR_INVALID_SECTION_NAME;
                    goto parse_error;
                }
                break;

            case INIT_CMD_END:
                if (curr_section == SECTION_NONE)
                {
                    err = ERR_NOT_IN_SECTION;
                    goto parse_error;
                }
                if (curr_section == SECTION_WAN)
                {
                    pinfo->num_wanifs = num_wanifs;
                    seen_wan_section = 1;
                }
                else
                {
                    pinfo->num_lanifs = num_lanifs;
                    seen_lan_section = 1;
                }
                curr_section = SECTION_NONE;
                break;
            case INIT_CMD_NUM_ENTRIES:
            case INIT_CMD_BRIDGE_ENTRIES:
            case INIT_CMD_MULTICAST_ENTRIES:
                num_entries = atoi(val);
                if (num_entries > 1000 || num_entries < 0)
                {
                    err = ERR_INVALID_RANGE;
                    goto parse_error;
                }
                if (cmd == INIT_CMD_NUM_ENTRIES)
                {
                    if (curr_section == SECTION_WAN)
                        pinfo->max_wan_source_entries = num_entries;
                    else if (curr_section == SECTION_LAN)
                        pinfo->max_lan_source_entries = num_entries;
                    else
                    {
                        err = ERR_NOT_IN_SECTION;
                        goto parse_error;
                    }
                }
                else
                {
                    if (curr_section != SECTION_NONE)
                    {
                        err = ERR_IN_SECTION;
                        goto parse_error;
                    }
                    if (cmd == INIT_CMD_BRIDGE_ENTRIES)
                        pinfo->max_bridging_entries = num_entries;
                    else
                        pinfo->max_mc_entries = num_entries;
                }
                break;

            case INIT_CMD_TCP_HITS_TO_ADD:
            case INIT_CMD_UDP_HITS_TO_ADD:
                num_hits = atoi(val);
                if (num_hits < 0)
                {
                    err = ERR_INVALID_COMMAND;
                    goto parse_error;
                }
                if (cmd == INIT_CMD_TCP_HITS_TO_ADD)
                    pinfo->add_requires_min_hits = num_hits;
                else
                    pinfo->add_requires_min_hits = num_hits;
                break;

            case INIT_CMD_INTERFACE:
                if (curr_section == SECTION_NONE)
                {
                    err = ERR_NOT_IN_SECTION;
                    goto parse_error;
                }
                if (curr_section == SECTION_WAN)
                {
                    if ( num_wanifs < sizeof(pinfo->p_wanifs) / sizeof(pinfo->p_wanifs[0]) )
                    {
                        strncpy_s(pinfo->p_wanifs[num_wanifs].ifname,PPA_IF_NAME_SIZE, val,PPA_IF_NAME_SIZE);
                        pinfo->p_wanifs[num_wanifs].if_flags = 0;
                        num_wanifs++;
                    }
                }
                else if (curr_section == SECTION_LAN)
                {
                    if ( num_wanifs < sizeof(pinfo->p_lanifs) / sizeof(pinfo->p_lanifs[0]) )
                    {
                        strncpy_s(pinfo->p_lanifs[num_lanifs].ifname,PPA_IF_NAME_SIZE, val,PPA_IF_NAME_SIZE);
                        pinfo->p_lanifs[num_lanifs].if_flags = PPA_F_LAN_IF;
                        num_lanifs++;
                    }
                }
                break;

            default:
                err = ERR_INVALID_COMMAND;
                goto parse_error;
            }
        }
    }

    if( fd != NULL )
    {
        fclose(fd);
        fd = NULL;
    }
    return PPA_CMD_OK;

    // error messages
parse_error:

    switch(err)
    {
    case ERR_MULTIPLE_SECTIONS:
        IFX_PPACMD_PRINT("error: multiple section definitions - line %d\n", linenum);
        break;
    case ERR_INVALID_SECTION_NAME:
        IFX_PPACMD_PRINT("error: invalid section name - line %d\n", linenum);
        break;
    case ERR_INVALID_COMMAND:
        IFX_PPACMD_PRINT("error: invalid command - line %d\n", linenum);
        break;
    case ERR_NOT_IN_SECTION:
        IFX_PPACMD_PRINT("error: command not within valid section - line %d\n", linenum);
        break;
    case ERR_IN_SECTION:
        IFX_PPACMD_PRINT("error: command within section - line %d\n", linenum);
        break;
    case ERR_INVALID_RANGE:
        IFX_PPACMD_PRINT("error: parameter outside allowed range - line %d\n", linenum);
        break;
    }
    if( fd != NULL )
    {
        fclose(fd);
        fd = NULL;
    }
    return PPA_CMD_ERR;
}

static int ppa_parse_init_cmd(PPA_CMD_OPTS *popts,PPA_CMD_DATA *pdata)
{
    unsigned int i;
    PPA_CMD_INIT_INFO *pinfo = &pdata->init_info;
    PPA_CMD_MAX_ENTRY_INFO max_entries = {0};
	
#if defined(MIB_MODE_ENABLE)
    PPA_CMD_MIB_MODE_INFO    var_mib_mode;   /*!< MIB mode configuration parameter */  
#endif
    // Default PPA Settings
    pinfo->num_lanifs = 0;
    ppa_memset(pinfo->p_lanifs,sizeof(pinfo->p_lanifs),0); 
    pinfo->num_wanifs = 0;
    ppa_memset(pinfo->p_wanifs,sizeof(pinfo->p_wanifs),0);

    pinfo->max_lan_source_entries  = 0;
    pinfo->max_wan_source_entries  = 0;
    pinfo->max_mc_entries      = 0;
    pinfo->max_bridging_entries  = 0;
    pinfo->add_requires_min_hits   = 0;

    if( ppa_do_ioctl_cmd(PPA_CMD_GET_MAX_ENTRY, &max_entries) != PPA_CMD_OK )
    {
        return -EIO;
    }

    pinfo->max_lan_source_entries = max_entries.entries.max_lan_entries;
    pinfo->max_wan_source_entries = max_entries.entries.max_wan_entries;
    pinfo->max_mc_entries     = max_entries.entries.max_mc_entries;
    pinfo->max_bridging_entries   = max_entries.entries.max_bridging_entries;

    // Override any default setting from configuration file (if specified)
    while(popts->opt)
    {
        switch(popts->opt)
        {
        case 'f':
            if ( parse_init_config_file( popts->optarg, &pdata->init_info) )
            {
                IFX_PPACMD_PRINT("%s: error reading PPA configuration file: %s\n", PPA_CMD_NAME, popts->optarg);
                return PPA_CMD_ERR;
            }
            break;

        case 'l':
            if(  str_convert(STRING_TYPE_INTEGER,  popts->optarg, NULL ) < pinfo->max_lan_source_entries )
                pinfo->max_lan_source_entries = str_convert(STRING_TYPE_INTEGER,  popts->optarg, NULL );
            break;

        case 'w':
            if( str_convert(STRING_TYPE_INTEGER,  popts->optarg, NULL ) < pinfo->max_wan_source_entries )
                pinfo->max_wan_source_entries = str_convert(STRING_TYPE_INTEGER,  popts->optarg, NULL );
            break;

        case 'm':
            if( str_convert(STRING_TYPE_INTEGER,  popts->optarg, NULL ) < pinfo->max_mc_entries )
                pinfo->max_mc_entries = str_convert(STRING_TYPE_INTEGER,  popts->optarg, NULL );
            break;

        case 'b':
            if( str_convert(STRING_TYPE_INTEGER,  popts->optarg, NULL ) < pinfo->max_bridging_entries)
                pinfo->max_bridging_entries = str_convert(STRING_TYPE_INTEGER,  popts->optarg, NULL );
            break;

        case 'n':
            pinfo->add_requires_min_hits = str_convert(STRING_TYPE_INTEGER,  popts->optarg, NULL );
            break;

#if defined(MIB_MODE_ENABLE)
        case 'i':
            //var_mib_mode.mib_mode = str_convert(STRING_TYPE_INTEGER,  popts->optarg, NULL );
            pinfo->mib_mode = str_convert(STRING_TYPE_INTEGER,  popts->optarg, NULL );

           /* if( ppa_do_ioctl_cmd(PPA_CMD_SET_MIB_MODE, &var_mib_mode) != PPA_CMD_OK )
            {
                return -EIO;
            }*/
            break;
#endif
        default:
            return PPA_CMD_ERR;
        }
        popts++;
    }

    IFX_PPACMD_DBG("INTERFACES\n");
    IFX_PPACMD_DBG("Number of LAN IF: %d\n", pinfo->num_lanifs);
    for ( i = 0; i < pinfo->num_lanifs; i++ )
        IFX_PPACMD_DBG("  %s (%08d)\n", pinfo->p_lanifs[i].ifname, pinfo->p_lanifs[i].if_flags);
    IFX_PPACMD_DBG("Number of WAN IF: %d\n", pinfo->num_wanifs);
    for ( i = 0; i < pinfo->num_wanifs; i++ )
        IFX_PPACMD_DBG("  %s %08X)\n", pinfo->p_wanifs[i].ifname, pinfo->p_wanifs[i].if_flags);

    IFX_PPACMD_DBG("OTHER\n");
    IFX_PPACMD_DBG("   Max. LAN Entries: %d\n", pinfo->max_lan_source_entries);
    IFX_PPACMD_DBG("   Max. WAN Entries: %d\n", pinfo->max_wan_source_entries);
    IFX_PPACMD_DBG("   Max. MC Entries: %d\n", pinfo->max_mc_entries);
    IFX_PPACMD_DBG("   Max. Bridge Entries: %d\n", pinfo->max_bridging_entries);
    IFX_PPACMD_DBG("   Min. Hits: %d\n", pinfo->add_requires_min_hits);

#ifdef CONFIG_PPA_PUMA7
    system("echo enable > /proc/net/ti_pp");
    system("echo 1 > /sys/devices/platform/toe/enable");
#endif

    return PPA_CMD_OK;
}

static void ppa_print_init_fake_cmd(PPA_CMD_DATA *pdata)
{
    /* By default, we will enable ppa LAN/WAN acceleratation */
    PPA_CMD_ENABLE_INFO  enable_info;

    ppa_memset( &enable_info, sizeof(enable_info), 0) ;

    enable_info.lan_rx_ppa_enable = 1;
    enable_info.wan_rx_ppa_enable = 1;

    if( ppa_do_ioctl_cmd(PPA_CMD_ENABLE, &enable_info ) != PPA_CMD_OK )
    {
        IFX_PPACMD_PRINT("ppacmd control to enable lan/wan failed\n");
        return ;
    }
}



/*
====================================================================================
   command:   exit
   description: Remove the Packet Processing Acceleration Module
   options:   None
====================================================================================
*/
static void ppa_print_exit_help(int summary)
{
    IFX_PPACMD_PRINT("exit\n");
    return;
}

static int ppa_parse_exit_cmd(PPA_CMD_OPTS *popts,PPA_CMD_DATA *pdata)
{
    if (popts->opt != 0)
        return PPA_CMD_ERR;

    IFX_PPACMD_DBG("EXIT COMMAND\n");

#ifdef CONFIG_PPA_PUMA7
    system("echo disable > /proc/net/ti_pp");
    system("echo 0 > /sys/devices/platform/toe/enable");
#endif

    return PPA_CMD_OK;
}

/*
====================================================================================
   command:   control
   description: Enable and Disable Packet Processing Acceleration for WAN and/or LAN
        interfaces.
   options:   Enable and/or Disable parameters
====================================================================================
*/

static const struct option ppa_control_long_opts[] =
{
    {"enable-lan",  no_argument, NULL, OPT_ENABLE_LAN},
    {"disable-lan", no_argument, NULL, OPT_DISABLE_LAN},
    {"enable-wan",  no_argument, NULL, OPT_ENABLE_WAN},
    {"disable-wan", no_argument, NULL, OPT_DISABLE_WAN},
    { 0,0,0,0 }
};

static void ppa_print_control_help(int summary)
{
    IFX_PPACMD_PRINT("control {--enable-lan|--disable-lan} {--enable-wan|--disable-wan} \n");
    return;
}

static int ppa_parse_control_cmd(PPA_CMD_OPTS *popts,PPA_CMD_DATA *pdata)
{
    unsigned int lan_opt = 0, wan_opt = 0;

    while(popts->opt)
    {
        switch(popts->opt)
        {
        case OPT_ENABLE_LAN:
            pdata->ena_info.lan_rx_ppa_enable = 1;
            pdata->ena_info.flags |= PPA_LAN_VALID;
            lan_opt++;
            break;

        case OPT_DISABLE_LAN:
            pdata->ena_info.lan_rx_ppa_enable = 0;
            pdata->ena_info.flags |= PPA_LAN_VALID;
            lan_opt++;
            break;

        case OPT_ENABLE_WAN:
            pdata->ena_info.wan_rx_ppa_enable = 1;
            pdata->ena_info.flags |= PPA_WAN_VALID;
            wan_opt++;
            break;

        case OPT_DISABLE_WAN:
            pdata->ena_info.wan_rx_ppa_enable = 0;
            pdata->ena_info.flags |= PPA_WAN_VALID;
            wan_opt++;
            break;
        }
        popts++;
    }

    /* Allow only one of the parameters for LAN or WAN to be specified */
    if (wan_opt > 1 || lan_opt > 1)
        return PPA_CMD_ERR;

    if (wan_opt ==0 &&  lan_opt == 0) /*sgh add: without this checking, all lan/wan acceleration will be disabled if user run command "ppacmd control" without any parameter */
        return PPA_CMD_ERR;

    IFX_PPACMD_DBG("PPA CONTROL: LAN = %s   WAN = %s\n", pdata->ena_info.lan_rx_ppa_enable ? "enable" : "disable",
            pdata->ena_info.wan_rx_ppa_enable ? "enable" : "disable");

    return PPA_CMD_OK;
}

/*
====================================================================================
   command:   status
   description: Display Packet Processing Acceleration status for WAN and/or LAN
        interfaces.

   options:   None
====================================================================================
*/

static void ppa_print_status_help(int summary)
{
	IFX_PPACMD_PRINT("status\n");
	return;
}

static int ppa_parse_status_cmd(PPA_CMD_OPTS *popts, PPA_CMD_DATA *pdata)
{
	if (popts->opt != 0)
		return PPA_CMD_ERR;

	IFX_PPACMD_DBG("PPA STATUS\n");

	return PPA_CMD_OK;
}

static void ppa_print_status(PPA_CMD_DATA *pdata)
{
    if( pdata->ena_info.flags == 0 )
        IFX_PPACMD_PRINT("PPA not initialized yet\n");
    else
    {
        PPA_CMD_MAX_ENTRY_INFO max_entries;
#if defined(MIB_MODE_ENABLE)
        PPA_CMD_MIB_MODE_INFO mode_info;
#endif
#if defined(CAP_WAP_CONFIG) && CAP_WAP_CONFIG
        PPA_CMD_DATA cmd_info;
#endif
        
        ppa_memset( &max_entries, sizeof(max_entries), 0 );
        IFX_PPACMD_PRINT("  LAN Acceleration: %s.\n", pdata->ena_info.lan_rx_ppa_enable ? "enabled": "disabled");
        IFX_PPACMD_PRINT("  WAN Acceleration: %s.\n", pdata->ena_info.wan_rx_ppa_enable ? "enabled": "disabled");

        if( ppa_do_ioctl_cmd(PPA_CMD_GET_MAX_ENTRY, &max_entries) == PPA_CMD_OK )
        {
            IFX_PPACMD_PRINT("  LAN max entries:%d(Collision:%d)\n", (unsigned int)max_entries.entries.max_lan_entries, (unsigned int)max_entries.entries.max_lan_collision_entries);
            IFX_PPACMD_PRINT("  WAN max entries:%d(Collision:%d)\n", (unsigned int)max_entries.entries.max_wan_entries, (unsigned int)max_entries.entries.max_wan_collision_entries);
            IFX_PPACMD_PRINT("  LAN hash index number:%d, bucket number per index:%d)\n", (unsigned int)max_entries.entries.max_lan_hash_index_num, (unsigned int)max_entries.entries.max_lan_hash_bucket_num);
            IFX_PPACMD_PRINT("  WAN hash index number:%d, bucket number per index:%d)\n", (unsigned int)max_entries.entries.max_wan_hash_index_num, (unsigned int)max_entries.entries.max_wan_hash_bucket_num);
            IFX_PPACMD_PRINT("  MC max entries:%d\n", (unsigned int)max_entries.entries.max_mc_entries);
            IFX_PPACMD_PRINT("  Bridge max entries:%d\n", (unsigned int)max_entries.entries.max_bridging_entries);
            IFX_PPACMD_PRINT("  IPv6 address max entries:%d\n", (unsigned int)max_entries.entries.max_ipv6_addr_entries);
            IFX_PPACMD_PRINT("  PPE FW max queue:%d\n", (unsigned int)max_entries.entries.max_fw_queue);
            IFX_PPACMD_PRINT("  6RD max entries:%d\n", (unsigned int)max_entries.entries.max_6rd_entries);
            IFX_PPACMD_PRINT("  MF Flow max entries:%d\n", (unsigned int)max_entries.entries.max_mf_flow);
        }

#if defined(MIB_MODE_ENABLE)
        if( ppa_do_ioctl_cmd(PPA_CMD_GET_MIB_MODE, &mode_info) == PPA_CMD_OK )
        {
            if(mode_info.mib_mode == 1)
                  IFX_PPACMD_PRINT("  Unicast/Multicast Session Mib in Packet\n");
            else
                  IFX_PPACMD_PRINT("  Unicast/Multicast Session Mib in Byte\n");
        }
#endif

    }
}

/*
====================================================================================
  Generic add/delete/get interface functions.
  The add/delete WAN interface commands share the same data structures and command
  options so they are combined into one set of functions and shared by each.
====================================================================================
*/

static int ppa_parse_add_del_if_cmd(PPA_CMD_OPTS *popts, PPA_CMD_DATA *pdata)
{
    int  opt = 0;
    int check_f = 0, check_l = 0, check_w = 0;

    PPA_CMD_IFINFOS  *all_if_info;

    all_if_info = &(pdata->all_if_info);

    ppa_memset(all_if_info, sizeof(PPA_CMD_IFINFOS), 0);
    while (popts->opt)
    {
        if (popts->opt == 'i')
        {
            opt++;
            strncpy_s(all_if_info->ifinfo[opt-1].ifname,PPA_IF_NAME_SIZE,popts->optarg,PPA_IF_NAME_SIZE);
            all_if_info->ifinfo[opt-1].ifname[PPA_IF_NAME_SIZE-1] = 0;
            check_f = check_l = check_w = 0;
        }
        else if (popts->opt == 'f')
        {
            if (opt == 0)
                return PPA_CMD_ERR;
            all_if_info->ifinfo[opt-1].force_wanitf_flag=1;
            check_f++;
        }
        else if (popts->opt == 'l')
        {
            if (opt == 0)
                return PPA_CMD_ERR;
            strncpy_s(all_if_info->ifinfo[opt-1].ifname_lower,PPA_IF_NAME_SIZE-1,popts->optarg,PPA_IF_NAME_SIZE-1);
            all_if_info->ifinfo[opt-1].ifname_lower[PPA_IF_NAME_SIZE-1] = 0;
            check_l++;
        }
        else if (popts->opt == 'w')
        {
            if (opt == 0)
                return PPA_CMD_ERR;
            all_if_info->ifinfo[opt-1].hw_disable = 1;
            check_w++;
        }

        if (check_f > 1 || check_l > 1 || check_w > 1)
            return PPA_CMD_ERR;

        popts++;
    }

    if( opt < 1 )
        return PPA_CMD_ERR;

    all_if_info->num_ifinfos = opt;

    IFX_PPACMD_DBG("PPA ADD/DEL IF: %s with flag=%x\n", pdata->if_info.ifname, pdata->if_info.force_wanitf_flag);

    return PPA_CMD_OK;
}

static int ppa_get_netif_cmd(PPA_CMD_DATA *pdata, unsigned int ioctl_cmd)
{
  unsigned int i = 0, size = 0, if_count = 0;
  PPA_CMD_DATA cmd_info = {0};
  PPA_CMD_IFINFOS *if_list = NULL;

  cmd_info.count_info.flag = (ioctl_cmd == PPA_CMD_GET_LAN_IF)
                             ? PPA_F_LAN_IF : 0;
  /*!< Get the interface count */
  if (ppa_do_ioctl_cmd(PPA_CMD_GET_IF_COUNT, &cmd_info.count_info) !=
      PPA_CMD_OK) {
    IFX_PPACMD_PRINT("ioctl failed\n");
    return -EIO;
  }

  if_count = cmd_info.count_info.count;
  if (if_count > 0) {
    size = sizeof(if_count) + (sizeof(PPA_CMD_IFINFO) * if_count);
    if_list = (PPA_CMD_IFINFOS *)malloc(size);
    if (!if_list) {
      IFX_PPACMD_PRINT("malloc failed\n");
      return PPA_CMD_NOT_AVAIL;
    }
    ppa_memset(if_list, size, 0);

  /*!< Get the interface list from ioctl */
    if_list->num_ifinfos = if_count;
    if (ppa_do_ioctl_cmd(ioctl_cmd, if_list) != PPA_CMD_OK) {
      IFX_PPACMD_PRINT("ioctl failed\n");
      free(if_list);
      return -EIO;
    }

    /*!< Print the interface list */
    for (i = 0; i < if_list->num_ifinfos; i++) {
        IFX_PPACMD_PRINT("[%2d] %15s with acc_rx/acc_tx %llu:%llu\n",
            i, if_list->ifinfo[i].ifname,
            (unsigned long long)if_list->ifinfo[i].acc_rx,
            (unsigned long long)if_list->ifinfo[i].acc_tx);
    }
    free(if_list);
  }
  return PPA_CMD_OK;
}

static int ppa_get_lan_netif_cmd(PPA_COMMAND *pcmd, PPA_CMD_DATA *pdata)
{
	IFX_PPACMD_PRINT("LAN IF: ---\n");
    return ppa_get_netif_cmd(pdata, PPA_CMD_GET_LAN_IF);
}

static int ppa_get_wan_netif_cmd(PPA_COMMAND *pcmd, PPA_CMD_DATA *pdata)
{
	IFX_PPACMD_PRINT("WAN IF: ---\n");
    return ppa_get_netif_cmd(pdata, PPA_CMD_GET_WAN_IF);
}

/*
====================================================================================
   command:   addwan
        delwan
        getwan
   description: Add WAN interface to PPA
   options:
====================================================================================
*/

static void ppa_print_add_wan_help(int summary)
{
    IFX_PPACMD_PRINT("addwan -i <ifname>\n");
    if( summary )
    {
       IFX_PPACMD_PRINT("addwan -i <ifname> -f\n");
       IFX_PPACMD_PRINT("    Note:  -f is used to force change WAN interface in PPE FW level. Be careful to use it !!\n");
       IFX_PPACMD_PRINT("           -l is used to manually configure its lower interface in case auto-searching failed !!\n");
       IFX_PPACMD_PRINT("           multiple interfaces can passed with -i <ifname>...\n");
    }
    return;
}

static void ppa_print_del_wan_help(int summary)
{
    IFX_PPACMD_PRINT("delwan -i <ifname>\n");
    IFX_PPACMD_PRINT("multiple interfaces can passed with -i <ifname>...\n");
    return;
}

static void ppa_print_get_wan_help(int summary)
{
	IFX_PPACMD_PRINT("getwan\n");
	return;
}

/*
====================================================================================
   command:    addlan
         dellan
         getlan
   description:
   options:
====================================================================================
*/

static void ppa_print_add_lan_help(int summary)
{
    IFX_PPACMD_PRINT("addlan -i <ifname>\n");
    if( summary )
    {
        IFX_PPACMD_PRINT("addlan -i <ifname> -f\n");
        IFX_PPACMD_PRINT("    Note:  -f is used to force change LAN interface in PPE FW level. Be careful to use it !!\n");
        IFX_PPACMD_PRINT("           -l is used to manually configure its lower interface in case auto-searching failed !!\n");
    }
    return;
}

static void ppa_print_del_lan_help(int summary)
{
    IFX_PPACMD_PRINT("dellan -i <ifname>\n");
    return;
}

static void ppa_print_get_lan_help(int summary)
{
	IFX_PPACMD_PRINT("getlan\n");
	return;
}

/*
====================================================================================
   command:   addbr
   description:
   options:
====================================================================================
*/

static void ppa_add_mac_entry_help(int summary)
{
    IFX_PPACMD_PRINT("addbr -m <macaddr> -i <ifname> \n");
    return;
}

static int ppa_parse_add_mac_entry_cmd(PPA_CMD_OPTS *popts, PPA_CMD_DATA *pdata)
{
    unsigned int if_opt = 0, mac_opt = 0;

#ifdef CONFIG_SOC_LGM
	IFX_PPACMD_PRINT("Not supported in URX\n");
	return PPA_CMD_ERR;
#endif

    while (popts->opt)
    {
        if (popts->opt == 'i')
        {
            strncpy_s(pdata->mac_entry.ifname,PPA_IF_NAME_SIZE, popts->optarg,PPA_IF_NAME_SIZE);
            pdata->mac_entry.ifname[PPA_IF_NAME_SIZE-1] = 0;
            if_opt++;
        }
        else if (popts->opt == 'm')
        {
		if (! isValidMacAddress(popts->optarg)){
			IFX_PPACMD_PRINT("Wrong mac address:%s\n", popts->optarg);
			return PPA_CMD_ERR;
		}
		stomac(popts->optarg,pdata->mac_entry.mac_addr);
		mac_opt++;
        }
        else
        {
            return PPA_CMD_ERR;
        }
        popts++;
    }

    // Each parameter must be specified just once.
    if (mac_opt != 1 || if_opt != 1)
        return PPA_CMD_ERR;

    IFX_PPACMD_DBG("PPA ADD MAC: %s  =  %02x:%02x:%02x:%02x:%02x:%02x\n", pdata->mac_entry.ifname,
            pdata->mac_entry.mac_addr[0],
            pdata->mac_entry.mac_addr[1],
            pdata->mac_entry.mac_addr[2],
            pdata->mac_entry.mac_addr[3],
            pdata->mac_entry.mac_addr[4],
            pdata->mac_entry.mac_addr[5]);


    return PPA_CMD_OK;
}

/*
====================================================================================
   command:   delbr
   description:
   options:
====================================================================================
*/

static void ppa_del_mac_entry_help(int summary)
{
    IFX_PPACMD_PRINT("delbr -m <macaddr> \n");
    return;
}

static int ppa_parse_del_mac_entry_cmd(PPA_CMD_OPTS *popts, PPA_CMD_DATA *pdata)
{
    unsigned int mac_opt = 0;

#ifdef CONFIG_SOC_LGM
	IFX_PPACMD_PRINT("Not supported in URX\n");
	return PPA_CMD_ERR;
#endif

    while (popts->opt)
    {
        if (popts->opt == 'm')
        {
		if (! isValidMacAddress(popts->optarg)){
			IFX_PPACMD_PRINT("Wrong mac address:%s\n", popts->optarg);
			return PPA_CMD_ERR;
		}
		stomac(popts->optarg,pdata->mac_entry.mac_addr);
		mac_opt++;
        }
        else
        {
            return PPA_CMD_ERR;
        }
        popts++;
    }

    // MAC parameter must be specified just once.
    if (mac_opt != 1)
        return PPA_CMD_ERR;


    IFX_PPACMD_DBG("PPA DEL MAC:  %02x:%02x:%02x:%02x:%02x:%02x\n", pdata->mac_entry.mac_addr[0],
            pdata->mac_entry.mac_addr[1],
            pdata->mac_entry.mac_addr[2],
            pdata->mac_entry.mac_addr[3],
            pdata->mac_entry.mac_addr[4],
            pdata->mac_entry.mac_addr[5]);
    return PPA_CMD_OK;
}

/*
====================================================================================
   command:   setvif
   description: Set interface VLAN configuration.
   options:
====================================================================================
*/

static const char ppa_set_vlan_if_cfg_short_opts[] = "i:V:c:O:h";

static const struct option ppa_set_vlan_if_cfg_long_opts[] =
{
    {"interface",     required_argument,  NULL,  'i'},
    {"vlan-type",     required_argument,  NULL,  'V'},
    {"tag-control",     required_argument,  NULL,  'c'},
    {"outer-tag-control", required_argument,  NULL,  'O'},
    {"vlan-aware",    no_argument,    NULL,  OPT_VLAN_AWARE},
    {"outer-tag-control", no_argument,    NULL,  OPT_OUTER_VLAN_AWARE},
    { 0,0,0,0 }
};

static void ppa_set_vlan_if_cfg_help(int summary)
{
    IFX_PPACMD_PRINT("setvif -i <ifname> -V <vlan-type> -c <inner-tag-control> -O <outer-tag-control>\n");
    if( summary )
    {
        IFX_PPACMD_PRINT("    <vlan-type>   := {src-ip-addr|eth-type|ingress-vid|port} \n");
        IFX_PPACMD_PRINT("    <tag-control> := {insert|remove|replace|none} \n");

        IFX_PPACMD_PRINT("    This command is for A4/D4 only at present\n");
    }
    return;
}

static int ppa_parse_set_vlan_if_cfg_cmd(PPA_CMD_OPTS *popts, PPA_CMD_DATA *pdata)
{
    unsigned int vlan_type_opt = 0, in_tag_opt = 0, out_tag_opt = 0;
    unsigned int in_aware_opt = 0, out_aware_opt = 0, if_opt = 0;

    while (popts->opt)
    {
        switch(popts->opt)
        {
        case 'V':
            if (strcmp("src-ip-addr",popts->optarg) == 0)
                pdata->br_vlan.vlan_cfg.src_ip_based_vlan = 1;
            else if (strcmp("eth-type",popts->optarg) == 0)
                pdata->br_vlan.vlan_cfg.eth_type_based_vlan = 1;
            else if (strcmp("ingress-vid",popts->optarg) == 0)
                pdata->br_vlan.vlan_cfg.vlanid_based_vlan = 1;
            else if (strcmp("port",popts->optarg) == 0)
                pdata->br_vlan.vlan_cfg.port_based_vlan = 1;
            else
                return PPA_CMD_ERR;
            vlan_type_opt++;
            break;

        case 'c':
            if (strcmp("insert",popts->optarg) == 0)
                pdata->br_vlan.vlan_tag_ctrl.insertion = 1;
            else if (strcmp("remove",popts->optarg) == 0)
                pdata->br_vlan.vlan_tag_ctrl.remove = 1;
            else if (strcmp("replace",popts->optarg) == 0)
                pdata->br_vlan.vlan_tag_ctrl.replace = 1;
            else if (strcmp("none",popts->optarg) == 0)
                pdata->br_vlan.vlan_tag_ctrl.unmodified = 1;
            else
                return PPA_CMD_ERR;
            in_tag_opt++;
            break;

        case 'O':
            if (strcmp("insert",popts->optarg) == 0)
                pdata->br_vlan.vlan_tag_ctrl.out_insertion = 1;
            else if (strcmp("remove",popts->optarg) == 0)
                pdata->br_vlan.vlan_tag_ctrl.out_remove = 1;
            else if (strcmp("replace",popts->optarg) == 0)
                pdata->br_vlan.vlan_tag_ctrl.out_replace = 1;
            else if (strcmp("none",popts->optarg) == 0)
                pdata->br_vlan.vlan_tag_ctrl.out_unmodified = 1;
            else
                return PPA_CMD_ERR;
            out_tag_opt++;
            break;

        case 'i':
            strncpy_s(pdata->br_vlan.if_name,PPA_IF_NAME_SIZE,popts->optarg,PPA_IF_NAME_SIZE);
            pdata->br_vlan.if_name[PPA_IF_NAME_SIZE-1] = 0;
            if_opt++;
            break;

        case OPT_VLAN_AWARE:
            pdata->br_vlan.vlan_cfg.vlan_aware = 1;
            in_aware_opt++;
            break;

        case OPT_OUTER_VLAN_AWARE:
            pdata->br_vlan.vlan_cfg.out_vlan_aware = 1;
            out_aware_opt++;
            break;

        default:
            return PPA_CMD_ERR;
        }
        popts++;
    }

    if (   /*vlan_type_opt > 1 ||*/ if_opt     != 1
                                    || in_tag_opt  > 1 || out_tag_opt   > 1
                                    || in_aware_opt  > 1 || out_aware_opt > 1)
        return PPA_CMD_ERR;


    // Set default values is not specified in command line
    if (vlan_type_opt == 0)
        pdata->br_vlan.vlan_cfg.port_based_vlan = 1;

    if (in_tag_opt == 0)
        pdata->br_vlan.vlan_tag_ctrl.unmodified = 1;

    if (out_tag_opt == 0)
        pdata->br_vlan.vlan_tag_ctrl.out_unmodified = 1;

    IFX_PPACMD_DBG("VLAN TYPE:%s\n", pdata->br_vlan.if_name);
    IFX_PPACMD_DBG("  SRC IP VLAN: %s\n", pdata->br_vlan.vlan_cfg.src_ip_based_vlan ? "enable" : "disable");
    IFX_PPACMD_DBG("  ETH TYPE VLAN: %s\n", pdata->br_vlan.vlan_cfg.eth_type_based_vlan ? "enable" : "disable");
    IFX_PPACMD_DBG("     VID VLAN: %s\n", pdata->br_vlan.vlan_cfg.vlanid_based_vlan ? "enable" : "disable");
    IFX_PPACMD_DBG("PORT BASED VLAN: %s\n", pdata->br_vlan.vlan_cfg.port_based_vlan ? "enable" : "disable");

    IFX_PPACMD_DBG("TAG CONTROL\n");
    IFX_PPACMD_DBG("    INSERT: %s\n", pdata->br_vlan.vlan_tag_ctrl.insertion ? "enable" : "disable");
    IFX_PPACMD_DBG("    REMOVE: %s\n", pdata->br_vlan.vlan_tag_ctrl.remove ? "enable" : "disable");
    IFX_PPACMD_DBG("     REPLACE: %s\n", pdata->br_vlan.vlan_tag_ctrl.replace ? "enable" : "disable");
    IFX_PPACMD_DBG("  OUT INSERT: %s\n",pdata->br_vlan.vlan_tag_ctrl.out_insertion ? "enable" : "disable");
    IFX_PPACMD_DBG("  OUT REMOVE: %s\n", pdata->br_vlan.vlan_tag_ctrl.out_remove ? "enable" : "disable");
    IFX_PPACMD_DBG("   OUT REPLACE: %s\n", pdata->br_vlan.vlan_tag_ctrl.out_replace ? "enable" : "disable");
    IFX_PPACMD_DBG("  VLAN AWARE: %s\n", pdata->br_vlan.vlan_cfg.vlan_aware ? "enable" : "disable");
    IFX_PPACMD_DBG("OUT VLAN AWARE: %s\n", pdata->br_vlan.vlan_cfg.out_vlan_aware ? "enable" : "disable");

    return PPA_CMD_OK;
}

/*
====================================================================================
   command:
   description:
   options:
====================================================================================
*/

static void ppa_get_vlan_if_cfg_help(int summary)
{
	IFX_PPACMD_PRINT("getvif -i <ifname>\n");
    if( summary ) IFX_PPACMD_PRINT("    This command is for A4/D4 only at present\n");
    return;
}

static int ppa_parse_get_vlan_if_cfg_cmd(PPA_CMD_OPTS *popts, PPA_CMD_DATA *pdata)
{
    unsigned int out_opts = 0, if_opts = 0;

    while (popts->opt)
    {
        if (popts->opt == 'i')
        {
            strcpy_s(pdata->br_vlan.if_name,PPA_IF_NAME_SIZE, popts->optarg);
            if_opts++;
        }
        else
        {
            return PPA_CMD_ERR;
        }
        popts++;
    }

	if (if_opts != 1)
		return PPA_CMD_ERR;

    IFX_PPACMD_DBG("PPA GET VLAN CFG: %s\n", pdata->br_vlan.if_name);

    return PPA_CMD_OK;
}


static void ppa_print_get_vif(PPA_CMD_DATA *pdata)
{
	//vlan_tag_ctrl, &cmd_info.br_vlan.vlan_cfg, c
	IFX_PPACMD_PRINT("%s: ", pdata->br_vlan.if_name);
	if (pdata->br_vlan.vlan_cfg.eth_type_based_vlan)
		IFX_PPACMD_PRINT("ether-type based");
	if (pdata->br_vlan.vlan_cfg.src_ip_based_vlan)
		IFX_PPACMD_PRINT("src-ip based");
	if (pdata->br_vlan.vlan_cfg.vlanid_based_vlan)
		IFX_PPACMD_PRINT("vlan id based");
	if (pdata->br_vlan.vlan_cfg.port_based_vlan)
		IFX_PPACMD_PRINT("port based");

	IFX_PPACMD_PRINT("%s", pdata->br_vlan.vlan_cfg.vlan_aware ? "inner vlan aware":"inner vlan no");
	IFX_PPACMD_PRINT("%s", pdata->br_vlan.vlan_cfg.out_vlan_aware ? "out vlan aware":"outlvan vlan no");

	if (pdata->br_vlan.vlan_tag_ctrl.unmodified)
		IFX_PPACMD_PRINT("inner-vlan unmodified, ");
	else  if (pdata->br_vlan.vlan_tag_ctrl.insertion)
		IFX_PPACMD_PRINT("inner-vlan insert");
	else  if (pdata->br_vlan.vlan_tag_ctrl.remove)
		IFX_PPACMD_PRINT("inner-vlan remove");
	else  if (pdata->br_vlan.vlan_tag_ctrl.replace)
		IFX_PPACMD_PRINT("inner-vlan replace");


	if (pdata->br_vlan.vlan_tag_ctrl.out_unmodified)
		IFX_PPACMD_PRINT("out-vlan unmodified");
	else  if (pdata->br_vlan.vlan_tag_ctrl.out_insertion)
		IFX_PPACMD_PRINT("out-vlan insert");
	else  if (pdata->br_vlan.vlan_tag_ctrl.out_remove)
		IFX_PPACMD_PRINT("out-vlan remove");
	else  if (pdata->br_vlan.vlan_tag_ctrl.out_replace)
		IFX_PPACMD_PRINT("out-vlan replace");

	IFX_PPACMD_PRINT("\n");

}

/*
====================================================================================
   command:   addvfilter
   description:
   options:
====================================================================================
*/
typedef struct vlan_ctrl
{
    char* cmd_str;  //command
    char op; //qid
} vlan_ctrl;
vlan_ctrl vlan_ctrl_list[]= {{"none", 0},{"remove", 1},{"insert", 2},{"replace", 3} };

static const char ppa_add_vlan_filter_short_opts[] = "t:V:i:a:e:o:q:d:c:r:h";
static const struct option ppa_add_vlan_filter_long_opts[] =
{
    {"vlan-tag",  required_argument,  NULL, 't'},
    {"ingress-vid", required_argument,  NULL, 'V'},
    {"interface",  required_argument,  NULL, 'i'},
    {"src-ipaddr",  required_argument,  NULL, 'a'},
    {"eth-type",  required_argument,  NULL, 'e'},
    {"out-vlan-id", required_argument,  NULL, 'o'},
    {"dest_qos",  required_argument,  NULL, 'q'},
    {"dst-member",  required_argument,  NULL, 'd'},
    {"inner-vctrl",  required_argument,  NULL, 'r'},
    {"outer-vctrl",  required_argument,  NULL, 'c'},
    { 0,0,0,0 }
};

static void ppa_add_vlan_filter_help(int summary)
{
    if( !summary )
    {
        //only display part of parameter since there are too many parameters
        IFX_PPACMD_PRINT("addvfilter {-i <ifname>|-a <src-ip-addr>|-e <eth-type>|-V <vlan-id>} ...\n");
    }
    else
    {
        IFX_PPACMD_PRINT("addvfilter {-i <ifname>|-a <src-ip-addr>|-e <eth-type>|-V <vlan-id>} -t <vlan-tag>\n");
        IFX_PPACMD_PRINT("    -o <out_vlan_id> -q <queue_id> -d <member-list> -c <in-tag-control> -r <out-tag-control>\n");
        IFX_PPACMD_PRINT("    parameter c/r: for tag based vlan filter only\n");
        IFX_PPACMD_PRINT("    <tag-control: none | remove | insert | replace\n");
        IFX_PPACMD_PRINT("    This command is for A4/D4 only at present\n");
    }
    return;
}

static int ppa_parse_add_vlan_filter_cmd(PPA_CMD_OPTS *popts, PPA_CMD_DATA *pdata)
{
    unsigned int i, j, tag_opts = 0, match_opts = 0, out_vlan_id_opts=0, vlan_if_member_opts=0, qid_opts=0,inner_vlan_ctrl_opts=0, out_vlan_ctrl_opts=0;
    uint32_t vlan_ctrl;

    ppa_memset( &pdata->vlan_filter, sizeof(pdata->vlan_filter), 0 );
    while(popts->opt)
    {
        switch(popts->opt)
        {
        case 't':  /*inner vlan: for all kinds of vlan filters */
            pdata->vlan_filter.vlan_filter_cfg.vlan_info.vlan_vci = str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL);
            tag_opts++;
            break;

        case 'i': /*port based vlan filter: for comparing  */
            strncpy_s(pdata->vlan_filter.vlan_filter_cfg.match_field.match_field.ifname,PPA_IF_NAME_SIZE-1, popts->optarg, PPA_IF_NAME_SIZE-1);
            pdata->vlan_filter.vlan_filter_cfg.match_field.match_flags = PPA_F_VLAN_FILTER_IFNAME;
            match_opts++;
            break;

        case 'a': /*ip based vlan filter: for comparing*/
            inet_aton(popts->optarg,&pdata->vlan_filter.vlan_filter_cfg.match_field.match_field.ip_src);
            pdata->vlan_filter.vlan_filter_cfg.match_field.match_flags = PPA_F_VLAN_FILTER_IP_SRC;
            match_opts++;
            break;

        case 'e': /*protocol based vlan filter: for comparing */
            pdata->vlan_filter.vlan_filter_cfg.match_field.match_field.eth_protocol = str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL);
            pdata->vlan_filter.vlan_filter_cfg.match_field.match_flags = PPA_F_VLAN_FILTER_ETH_PROTO;
            match_opts++;
            break;

        case 'V': /*vlan tag based vlan filter: for comparing */
            pdata->vlan_filter.vlan_filter_cfg.match_field.match_field.ingress_vlan_tag = str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL);
            pdata->vlan_filter.vlan_filter_cfg.match_field.match_flags = PPA_F_VLAN_FILTER_VLAN_TAG;
            match_opts++;
            break;

        case 'o': /*outer vlan: for all kinds of vlan filters */
            pdata->vlan_filter.vlan_filter_cfg.vlan_info.out_vlan_id= str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL);
            out_vlan_id_opts++;
            break;

        case 'd': /*member list: for all kinds of vlan filters */
            if( vlan_if_member_opts < PPA_MAX_IFS_NUM )
            {
                strncpy_s(pdata->vlan_filter.vlan_filter_cfg.vlan_info.vlan_if_membership[vlan_if_member_opts].ifname,sizeof(pdata->vlan_filter.vlan_filter_cfg.vlan_info.vlan_if_membership[vlan_if_member_opts].ifname), popts->optarg, sizeof(pdata->vlan_filter.vlan_filter_cfg.vlan_info.vlan_if_membership[vlan_if_member_opts].ifname) );

                vlan_if_member_opts++;
                pdata->vlan_filter.vlan_filter_cfg.vlan_info.num_ifs = vlan_if_member_opts;

            }
            break;

        case 'q': /*qid: for all kinds of vlan filters */
            pdata->vlan_filter.vlan_filter_cfg.vlan_info.qid = str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL);
            qid_opts++;
            break;

        case 'c': //inner vlan ctrl: only for vlan tag based vlan filter
        case 'r': //out vlan ctrl:: only for vlan tag based vlan filter
            for(i=0; i<sizeof(vlan_ctrl_list)/sizeof(vlan_ctrl_list[0]); i++ )
            {
                if( strcmp( vlan_ctrl_list[i].cmd_str, popts->optarg ) == 0 )
                {
                    if( popts->opt == 'c' )
                    {
                        vlan_ctrl = vlan_ctrl_list[i].op << 2;
                        pdata->vlan_filter.vlan_filter_cfg.vlan_info.inner_vlan_tag_ctrl = 0;
                        for(j=0; j<8; j++)
                        {
                            pdata->vlan_filter.vlan_filter_cfg.vlan_info.inner_vlan_tag_ctrl |= vlan_ctrl << ( 4 * j );
                        }
                        inner_vlan_ctrl_opts++;
                        break;
                    }
                    else
                    {
                        vlan_ctrl = (vlan_ctrl_list[i].op);
                        pdata->vlan_filter.vlan_filter_cfg.vlan_info.out_vlan_tag_ctrl = 0;
                        for(j=0; j<8; j++)
                        {
                            pdata->vlan_filter.vlan_filter_cfg.vlan_info.out_vlan_tag_ctrl |= vlan_ctrl << ( 4 * j);
                        }
                        out_vlan_ctrl_opts++;
                        break;
                    }
                }
            }
            break;

        default:
            IFX_PPACMD_PRINT("not known parameter: %c\n", popts->opt);
            return PPA_CMD_ERR;
        }
        popts++;
    }

    /* Check that match field is not defined more than once and VLAN tag is specified */
    if ( ( match_opts != 1) || (tag_opts  != 1) || (out_vlan_id_opts !=1)  || (vlan_if_member_opts == 0)  )
    {
        if( match_opts != 1)
            IFX_PPACMD_PRINT( "match_opts wrong:%d\n", match_opts);
        else     if( tag_opts != 1)
            IFX_PPACMD_PRINT( "tag_opts wrong:%d\n", tag_opts);
        else     if( out_vlan_id_opts != 1)
            IFX_PPACMD_PRINT( "out_vlan_id_opts wrong:%d\n", out_vlan_id_opts);
        else     if( vlan_if_member_opts != 1)
            IFX_PPACMD_PRINT( "vlan_if_member_opts wrong:%d\n", vlan_if_member_opts);


        return PPA_CMD_ERR;
    }

    if( qid_opts == 0 )
    {
        pdata->vlan_filter.vlan_filter_cfg.vlan_info.qid = PPA_INVALID_QID;
    }


    if( pdata->vlan_filter.vlan_filter_cfg.match_field.match_flags == PPA_F_VLAN_FILTER_VLAN_TAG )
    {
        if( (inner_vlan_ctrl_opts != 1) || (out_vlan_ctrl_opts != 1) )
        {
            IFX_PPACMD_PRINT("vlan control wrong: inner_vlan_ctrl_opts=%d, out_vlan_ctrl_opts=%d\n", inner_vlan_ctrl_opts , out_vlan_ctrl_opts);
            return PPA_CMD_ERR;
        }
    }
    else
    {
        if(( inner_vlan_ctrl_opts != 0) ||( out_vlan_ctrl_opts != 0 ) )
        {
            IFX_PPACMD_PRINT("vlan control wrong 2: inner_vlan_ctrl_opts=%d, out_vlan_ctrl_opts=%d\n", inner_vlan_ctrl_opts , out_vlan_ctrl_opts);
            return PPA_CMD_ERR;
        }
    }


    IFX_PPACMD_DBG("INNER VLAN TAG: 0x%d\n", pdata->vlan_filter.vlan_filter_cfg.vlan_info.vlan_vci);
    IFX_PPACMD_DBG("OUT VLAN TAG: 0x%d\n", pdata->vlan_filter.vlan_filter_cfg.vlan_info.out_vlan_id);

    IFX_PPACMD_DBG("MATCH FIELD\n");
    switch(pdata->vlan_filter.vlan_filter_cfg.match_field.match_flags)
    {
    case PPA_F_VLAN_FILTER_VLAN_TAG:
        IFX_PPACMD_DBG("VLAN TAG: %04d\n", pdata->vlan_filter.vlan_filter_cfg.match_field.match_field.ingress_vlan_tag);
        IFX_PPACMD_DBG("INNER VLAN CTRL: %s\n", vlan_ctrl_list[ (pdata->vlan_filter.vlan_filter_cfg.vlan_info.inner_vlan_tag_ctrl >> 2 ) & 0x3].cmd_str);
        IFX_PPACMD_DBG("OUT   VLAN CTRL: %s\n", vlan_ctrl_list[ (pdata->vlan_filter.vlan_filter_cfg.vlan_info.out_vlan_tag_ctrl ) & 0x3].cmd_str);
        break;
    case PPA_F_VLAN_FILTER_IFNAME:
        IFX_PPACMD_DBG( "IF NAME: %s\n", pdata->vlan_filter.vlan_filter_cfg.match_field.match_field.ifname);
        break;
    case PPA_F_VLAN_FILTER_IP_SRC:
        IFX_PPACMD_DBG("IP SRC: %08d\n", pdata->vlan_filter.vlan_filter_cfg.match_field.match_field.ip_src);
        break;
    case PPA_F_VLAN_FILTER_ETH_PROTO:
        IFX_PPACMD_DBG("ETH TYPE: %04d\n",pdata->vlan_filter.vlan_filter_cfg.match_field.match_field.eth_protocol);
        break;
    }

    for(i=0; i< pdata->vlan_filter.vlan_filter_cfg.vlan_info.num_ifs; i++ )
    {
        IFX_PPACMD_DBG("Dest member[%d]=%s\n", i, pdata->vlan_filter.vlan_filter_cfg.vlan_info.vlan_if_membership[i].ifname);
    }
    return PPA_CMD_OK;
}

/*
====================================================================================
   command:   delvfilter
   description:
   options:
====================================================================================
*/

static const char ppa_del_vlan_filter_short_opts[] = "V:i:a:e:h";

static const struct option ppa_del_vlan_filter_long_opts[] =
{
    {"ingress-vid", required_argument,  NULL, 'V'},
    {"interface",   required_argument,  NULL, 'i'},
    {"src-ipaddr",  required_argument,  NULL, 'a'},
    {"eth-type",  required_argument,  NULL, 'e'},
    { 0,0,0,0 }
};

static void ppa_del_vlan_filter_help(int summary)
{
    IFX_PPACMD_PRINT("delvfilter {-i <ifname>|-a <src-ip-addr>|-e <eth-type>|-V <vlan-id>} \n");
    if( summary )   IFX_PPACMD_PRINT("    This command is for A4/D4 only at present\n");
    return;
}

static int ppa_parse_del_vlan_filter_cmd(PPA_CMD_OPTS *popts, PPA_CMD_DATA *pdata)
{
    unsigned int match_opts = 0;

    while(popts->opt)
    {
        switch(popts->opt)
        {
        case 'i': /*port based vlan filter: for comparing  */
            strncpy_s(pdata->vlan_filter.vlan_filter_cfg.match_field.match_field.ifname,PPA_IF_NAME_SIZE, popts->optarg, PPA_IF_NAME_SIZE);
            pdata->vlan_filter.vlan_filter_cfg.match_field.match_flags = PPA_F_VLAN_FILTER_IFNAME;
            match_opts++;
            break;

        case 'a': /*ip based vlan filter: for comparing*/
            inet_aton(popts->optarg, &pdata->vlan_filter.vlan_filter_cfg.match_field.match_field.ip_src);
            pdata->vlan_filter.vlan_filter_cfg.match_field.match_flags = PPA_F_VLAN_FILTER_IP_SRC;
            match_opts++;
            break;

        case 'e': /*protocol based vlan filter: for comparing */
            pdata->vlan_filter.vlan_filter_cfg.match_field.match_field.eth_protocol = str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL);
            pdata->vlan_filter.vlan_filter_cfg.match_field.match_flags = PPA_F_VLAN_FILTER_ETH_PROTO;
            match_opts++;
            break;

        case 'V': /*vlan tag based vlan filter: for comparing */
            pdata->vlan_filter.vlan_filter_cfg.match_field.match_field.ingress_vlan_tag = str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL);
            pdata->vlan_filter.vlan_filter_cfg.match_field.match_flags = PPA_F_VLAN_FILTER_VLAN_TAG;
            match_opts++;
            break;

        default:
            return PPA_CMD_ERR;
        }
        popts++;
    }

    /* Check that match field is not defined more than once and VLAN tag is specified */
    if ( match_opts != 1)
        return PPA_CMD_ERR;


    switch(pdata->vlan_filter.vlan_filter_cfg.match_field.match_flags)
    {
    case PPA_F_VLAN_FILTER_VLAN_TAG:
        IFX_PPACMD_DBG("VLAN TAG: %04d\n", pdata->vlan_filter.vlan_filter_cfg.match_field.match_field.ingress_vlan_tag);
        break;
    case PPA_F_VLAN_FILTER_IFNAME:
        IFX_PPACMD_DBG(" IF NAME: %s\n", pdata->vlan_filter.vlan_filter_cfg.match_field.match_field.ifname);
        break;
    case PPA_F_VLAN_FILTER_IP_SRC:
        IFX_PPACMD_DBG("  IP SRC: %08d\n", pdata->vlan_filter.vlan_filter_cfg.match_field.match_field.ip_src);
        break;
    case PPA_F_VLAN_FILTER_ETH_PROTO:
        IFX_PPACMD_DBG("ETH TYPE: %04d\n",pdata->vlan_filter.vlan_filter_cfg.match_field.match_field.eth_protocol);
        break;
    }
    return PPA_CMD_OK;
}

/*
====================================================================================
   command:   getvfiltercount
   description: get vlan fitlers counter
   options:
====================================================================================
*/
static void ppa_get_vfilter_count_help(int summary)
{
	IFX_PPACMD_PRINT("getvfilternum\n");
    if( summary ) IFX_PPACMD_PRINT("    This command is for A4/D4 only at present\n");
    return;
}

static int ppa_parse_get_vfilter_count(PPA_CMD_OPTS *popts, PPA_CMD_DATA *pdata)
{
    int res;

    res =   ppa_parse_simple_cmd( popts, pdata );

    if( res != PPA_CMD_OK ) return res;


    pdata->count_info.flag = 0;

    return PPA_CMD_OK;
}

/*
====================================================================================
   command:   getfilters
   description: get all vlan fitlers information
   options:
====================================================================================
*/

static void ppa_get_all_vlan_filter_help(int summary)
{
	IFX_PPACMD_PRINT("getvfilters\n");
    if( summary ) IFX_PPACMD_PRINT("    This command is for A4/D4 only at present\n");
    return;
}

static int ppa_get_all_vlan_filter_cmd (PPA_COMMAND *pcmd, PPA_CMD_DATA *pdata)
{
    PPA_CMD_VLAN_ALL_FILTER_CONFIG *psession_buffer;
    PPA_CMD_DATA cmd_info;
    int res = PPA_CMD_OK, i, j, size;
    uint32_t flag = PPA_CMD_GET_ALL_VLAN_FILTER_CFG;
    unsigned char bfCorrectType = 0;

    //get session count first before malloc memroy
    cmd_info.count_info.flag = 0;
    if( ppa_do_ioctl_cmd(PPA_CMD_GET_COUNT_VLAN_FILTER, &cmd_info ) != PPA_CMD_OK )
        return -EIO;

    if( cmd_info.count_info.count == 0 )
    {
        IFX_PPACMD_DBG("vfilter count=0\n");
        return PPA_CMD_OK;
    }

    //malloc memory and set value correctly
    size = sizeof(PPA_CMD_COUNT_INFO) + sizeof(PPA_CMD_VLAN_FILTER_CONFIG) * ( 1 + cmd_info.count_info.count ) ;
    psession_buffer = (PPA_CMD_VLAN_ALL_FILTER_CONFIG *) malloc( size );
    if( psession_buffer == NULL )
    {
        IFX_PPACMD_PRINT("Malloc %d bytes failed\n", size );
        return PPA_CMD_NOT_AVAIL;
    }

    ppa_memset( psession_buffer, sizeof(size), 0 );

    psession_buffer->count_info.count = cmd_info.count_info.count;
    psession_buffer->count_info.flag = 0;

    //get session information
    if( (res = ppa_do_ioctl_cmd(flag, psession_buffer ) != PPA_CMD_OK ) )
    {
        free( psession_buffer );
        return res;
    }

    IFX_PPACMD_DBG("Vfilter count=%u. \n", (unsigned int)psession_buffer->count_info.count);


	for (i=0; i<psession_buffer->count_info.count; i++) {
		IFX_PPACMD_PRINT("[%02d]", i);
		if (psession_buffer->filters[i].vlan_filter_cfg.match_field.match_flags == PPA_F_VLAN_FILTER_VLAN_TAG) {
			bfCorrectType = 1;
			IFX_PPACMD_PRINT("Vlan tag based:vlan %u. Qos:%u.",  (unsigned int)psession_buffer->filters[i].vlan_filter_cfg.match_field.match_field.ingress_vlan_tag, (unsigned int)psession_buffer->filters[i].vlan_filter_cfg.vlan_info.qid);
		}
		else  if (psession_buffer->filters[i].vlan_filter_cfg.match_field.match_flags == PPA_F_VLAN_FILTER_IFNAME) {
                bfCorrectType = 1;
			IFX_PPACMD_PRINT("Port based: %s. Qos:%d.",  psession_buffer->filters[i].vlan_filter_cfg.match_field.match_field.ifname, psession_buffer->filters[i].vlan_filter_cfg.vlan_info.qid);
		}
		else if (psession_buffer->filters[i].vlan_filter_cfg.match_field.match_flags == PPA_F_VLAN_FILTER_IP_SRC) {
			bfCorrectType = 1;
			IFX_PPACMD_PRINT("Src ip based: %u.%u.%u.%u. Qos:%d.", NIPQUAD(psession_buffer->filters[i].vlan_filter_cfg.match_field.match_field.ip_src),psession_buffer->filters[i].vlan_filter_cfg.vlan_info.qid);
		}
		else if (psession_buffer->filters[i].vlan_filter_cfg.match_field.match_flags == PPA_F_VLAN_FILTER_ETH_PROTO) {
			bfCorrectType = 1;
			IFX_PPACMD_PRINT("Ether type based: %04x. Qos:%u.",  (unsigned int)psession_buffer->filters[i].vlan_filter_cfg.match_field.match_field.eth_protocol, (unsigned int)psession_buffer->filters[i].vlan_filter_cfg.vlan_info.qid);
		}

		if (bfCorrectType) {
			IFX_PPACMD_PRINT("Inner/Out VLAN:%03X/%03x", (unsigned int)psession_buffer->filters[i].vlan_filter_cfg.vlan_info.vlan_vci, (unsigned int)psession_buffer->filters[i].vlan_filter_cfg.vlan_info.out_vlan_id);

			IFX_PPACMD_PRINT("Dst members:");
			for (j=0; j<psession_buffer->filters[i].vlan_filter_cfg.vlan_info.num_ifs ; j++) {
				if (i == 0)
					IFX_PPACMD_PRINT("%s", psession_buffer->filters[i].vlan_filter_cfg.vlan_info.vlan_if_membership[j].ifname);
				else
					IFX_PPACMD_PRINT("%s", psession_buffer->filters[i].vlan_filter_cfg.vlan_info.vlan_if_membership[j].ifname);
			}
		IFX_PPACMD_PRINT(".");

			if (psession_buffer->filters[i].vlan_filter_cfg.match_field.match_flags == PPA_F_VLAN_FILTER_VLAN_TAG) {
				IFX_PPACMD_PRINT("Inner/Out vlan control:%s/%s ",
                                     vlan_ctrl_list[ (psession_buffer->filters[i].vlan_filter_cfg.vlan_info.inner_vlan_tag_ctrl >> 2) & 0x3].cmd_str,
                                     vlan_ctrl_list[ (psession_buffer->filters[i].vlan_filter_cfg.vlan_info.out_vlan_tag_ctrl >> 0) & 0x3].cmd_str);
			}
		}
		IFX_PPACMD_PRINT("\n");
	}

    free(psession_buffer);
    return PPA_CMD_OK;
}

/*multicast bridging/routing */
static const char ppa_mc_sess_short_opts[] = "-:b:g:l:w:s:i:r:h";
//need to further implement add/remove/modify vlan and enable new dscp and its value
static const struct option ppa_mc_sess_long_opts[] = {
	{"bridging_flag",  required_argument,  NULL, 'b'},  /*0-means routing, 1 means bridging */
	/*{"multicat mac address", required_argument,  NULL, 'm'}, */  /*for bridging only as ritech suggest. I may not help at present */
	{"multicast group",   required_argument,  NULL, 'g'},
	{"down interface",  required_argument,  NULL, 'l'},
	{"up interface",  required_argument,  NULL, 'w'},
	{"source_ip",  required_argument,  NULL, 's'},
	{"group_id", required_argument, NULL, 'i'},
	{"drop", optional_argument, NULL, 'r'},
	{ 0, 0, 0, 0 }
};

static unsigned int is_ip_zero(IP_ADDR_C *ip)
{
    if(ip->f_ipv6){
		return ((ip->ip.ip6[0] | ip->ip.ip6[1] | ip->ip.ip6[2] | ip->ip.ip6[3]) == 0);
	}else{
		return (ip->ip.ip == 0);
	}
}

static unsigned int ip_equal(IP_ADDR_C *dst_ip, IP_ADDR_C *src_ip)
{
	if(dst_ip->f_ipv6){
		return (((dst_ip->ip.ip6[0] ^ src_ip->ip.ip6[0] ) |
			     (dst_ip->ip.ip6[1] ^ src_ip->ip.ip6[1] ) |
			     (dst_ip->ip.ip6[2] ^ src_ip->ip.ip6[2] ) |
			     (dst_ip->ip.ip6[3] ^ src_ip->ip.ip6[3] )) == 0);
	}else{
		return ( (dst_ip->ip.ip ^ src_ip->ip.ip) == 0);
	}
}


static void ppa_add_mc_help(int summary)
{
	if (!summary)
		IFX_PPACMD_PRINT("addmc -g <ip-group> -l <down interface> -w<up-interface> -s< src-ip> -i<group_id> \n");
	else {
		IFX_PPACMD_PRINT("addmc -g <ip-group> -l <down interface> -w<up-interface> -s< src-ip> -i<group_id> [-b <bridging_flag>]\n");
		IFX_PPACMD_PRINT("option -g: specify the group-ip for the group \n");
		IFX_PPACMD_PRINT("option -l: specifies the client interface for the group \n");
		IFX_PPACMD_PRINT("option -w: specifies the sender interface \n");
		IFX_PPACMD_PRINT("option -s: specifies the source ip for the group  \n");
		IFX_PPACMD_PRINT("option -i: specifies the group id for the mcast group \n");
	}
	return;
}

static int ppa_parse_add_mc_cmd(PPA_CMD_OPTS *popts, PPA_CMD_DATA *pdata)
{
	unsigned int lan_if_opts = 0, g_opts = 0;
	int ret;

	ppa_memset(pdata, sizeof(*pdata), 0);

	while (popts->opt) {

		switch (popts->opt) {
		case 'b':
			pdata->mc_sess_info.bridging_flag = str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL);
			if ((pdata->mc_sess_info.bridging_flag != 0) &&
				(pdata->mc_sess_info.bridging_flag != 1))
				return PPA_CMD_ERR;
			IFX_PPACMD_DBG("addmc  mode: %s\n", pdata->mc_sess_info.bridging_flag ? "bridging":"routing");
			break;

#ifdef PPA_MC_FUTURE_USE
		case 'm':
			if (!isValidMacAddress(popts->optarg)) {
				IFX_PPACMD_PRINT("Wrong mac address:%s\n", popts->optarg);
				return PPA_CMD_ERR;
			}
			stomac(popts->optarg, pdata->mc_sess_info.mac);
			IFX_PPACMD_DBG("addmc mac address: %02x:%02x:%02x:%02x:%02x:%02x\n",
			pdata->mc_sess_info.mac[0], pdata->mc_sess_info.mac[1], pdata->mc_sess_info.mac[2],
			pdata->mc_sess_info.mac[3], pdata->mc_sess_info.mac[4], pdata->mc_sess_info.mac[5]);
			break;
#endif

		case 'g':   /*Support IPv4 and IPv6 */
			ret = str_convert(STRING_TYPE_IP, popts->optarg, pdata->mc_sess_info.mc.mcast_addr.ip.ip6);
			if (ret == IP_NON_VALID) {
				IFX_PPACMD_DBG("Multicast group ip is not a valid IPv4 or IPv6 IP address: %s\n", popts->optarg);
				break;
			} else if (ret == IP_VALID_V6) {
				IFX_PPACMD_DBG("MLD GROUP IP: "NIP6_FMT"\n", NIP6(pdata->mc_sess_info.mc.mcast_addr.ip.ip6));
				pdata->mc_sess_info.mc.mcast_addr.f_ipv6 = 1;
			}
            
			g_opts++;
			break;

		case 'l':
			if (lan_if_opts > PPA_MAX_MC_IFS_NUM)
				return PPA_CMD_ERR;
			/* not to accelerate since too many lan interface join the same group */

			if (lan_if_opts < PPA_MAX_MC_IFS_NUM) {
				strncpy_s(pdata->mc_sess_info.lan_ifname[lan_if_opts], PPA_IF_NAME_SIZE, popts->optarg,
					PPA_IF_NAME_SIZE);
				pdata->mc_sess_info.lan_ifname[lan_if_opts][PPA_IF_NAME_SIZE-1] = 0;
				IFX_PPACMD_DBG("addmc lan if:%s,lan_if_opts:%d\n", pdata->mc_sess_info.lan_ifname[lan_if_opts],
					lan_if_opts+1);
				lan_if_opts++;
			}
			break;

		case 'w':
			strncpy_s(pdata->mc_sess_info.src_ifname, sizeof(pdata->mc_sess_info.src_ifname)-1,
				popts->optarg, sizeof(pdata->mc_sess_info.src_ifname)-1);
			pdata->mc_sess_info.src_ifname[sizeof(pdata->mc_sess_info.src_ifname)-1] = 0;
			IFX_PPACMD_DBG("addmc wan if:%s\n", pdata->mc_sess_info.src_ifname);
			break;

		case 's': // Src IP, Support IPv4 & IPv6
			ret = str_convert(STRING_TYPE_IP, popts->optarg, pdata->mc_sess_info.mc.source_ip.ip.ip6);
			if (ret == IP_NON_VALID) {
				IFX_PPACMD_DBG("Multicast group ip is not a valid IPv4 or IPv6 IP address: %s\n", popts->optarg);
				break;
			} else if (ret == IP_VALID_V6) {
				IFX_PPACMD_DBG("Source IP: "NIP6_FMT"\n", NIP6(pdata->mc_sess_info.mc.mcast_addr.ip.ip6));
				pdata->mc_sess_info.mc.source_ip.f_ipv6 = 1;
			}
			break;

		case 'i':
			pdata->mc_sess_info.mc.group_id  = str_convert(STRING_TYPE_INTEGER,  popts->optarg, NULL);
			IFX_PPACMD_DBG("addmc group_id:%d\n", pdata->mc_sess_info.mc.group_id);
			break;

		case 'r':
			pdata->mc_sess_info.drop  = str_convert(STRING_TYPE_INTEGER,  popts->optarg, NULL);
			IFX_PPACMD_DBG("addmc drop:%d\n", pdata->mc_sess_info.drop);
			break;

		default:
			IFX_PPACMD_PRINT("mc_add not support parameter -%c \n", popts->opt);
			return PPA_CMD_ERR;
		}
		popts++;
	}


	/* Check that match field is not defined more than once and VLAN tag is specified */
	if (g_opts != 1)
		return  PPA_CMD_ERR;

	pdata->mc_sess_info.num_ifs = lan_if_opts;

	return PPA_CMD_OK;
}

static void ppa_del_mc_help(int summary)
{
	if (!summary)
		IFX_PPACMD_PRINT("delmc -g <ip-group> [-l <down interface>] -w<up-interface> -s< src-ip> -i<group_id> [-r 0 | 1] \n");
	else {
		IFX_PPACMD_PRINT("delmc -g <ip-group> -l <down interface> -w<up-interface> -s< src-ip> -i<group_id> [-b <bridging_flag>] [-r 0 | 1} \n");
		IFX_PPACMD_PRINT("option -g: specify the group-ip for the group \n");
		IFX_PPACMD_PRINT("option -l: specifies the client interface for the group,if not set,means to delete the multicast group \n");
		IFX_PPACMD_PRINT("option -w: specifies the sender interface \n");
		IFX_PPACMD_PRINT("option -s: specifies the source ip for the group  \n");
		IFX_PPACMD_PRINT("option -i: specifies the group id for the mcast group \n");
		IFX_PPACMD_PRINT("option -r: specifies the drop action for the client \n");
	}
	return;
}

/* rtp sampling */
#if defined(RTP_SAMPLING_ENABLE) && RTP_SAMPLING_ENABLE

static const char ppa_rtp_set_short_opts[] = "g:i:r:"; 
static const struct option ppa_rtp_set_long_opts[] =
{
    {"multicast group",   required_argument,  NULL, 'g'},
    {"source_ip",  required_argument,  NULL, 'i'},
    {"rtp_flag",  required_argument,  NULL, 'r'},
    { 0,0,0,0 }
};


static void ppa_set_rtp_help( int summary)
{
    if( !summary )
    {
        IFX_PPACMD_PRINT("setrtp -g <mc-group> [-i<src-ip>] -r<rtp_flag> ...\n");
    }
    else
    {
        IFX_PPACMD_PRINT("setrtp -g <mc-group> [-i<src-ip>] -r<rtp_flag>\n");
        IFX_PPACMD_PRINT("      option -g: multicast group ip\n");
        IFX_PPACMD_PRINT("      option -i: source ip, this parameter is optional\n");
        IFX_PPACMD_PRINT("      option -r: RTP sequence number update by PPE FW, 1-enable 0-disable\n");
    }
    return;
}

static int ppa_parse_set_rtp_cmd(PPA_CMD_OPTS *popts, PPA_CMD_DATA *pdata)
{
    unsigned int lan_if_opts=0, g_opts=0;
	int ret;

    ppa_memset(pdata, sizeof(*pdata), 0 );

    while(popts->opt)
    {
        switch(popts->opt)
        {
        case 'g':   /*Support IPv4 and IPv6 */
            ret = str_convert(STRING_TYPE_IP, popts->optarg, pdata->mc_entry.mcast_addr.ip.ip6);
            if(ret == IP_NON_VALID){
                IFX_PPACMD_DBG("Multicast group ip is not a valid IPv4 or IPv6 IP address: %s\n", popts->optarg);
				break;
            }else if(ret == IP_VALID_V6){
                IFX_PPACMD_DBG("MLD GROUP IP: "NIP6_FMT"\n", NIP6(pdata->mc_entry.mcast_addr.ip.ip6));
                pdata->mc_entry.mcast_addr.f_ipv6 = 1;
            }
            
            g_opts ++;
            break;

        case 'i': // Src IP, Support IPv4 & IPv6

            ret = str_convert(STRING_TYPE_IP, popts->optarg, pdata->mc_entry.source_ip.ip.ip6);
            if(ret == IP_NON_VALID){
                IFX_PPACMD_DBG("Multicast group ip is not a valid IPv4 or IPv6 IP address: %s\n", popts->optarg);
				break;
            }else if(ret == IP_VALID_V6){
                IFX_PPACMD_DBG("Source IP: "NIP6_FMT"\n",NIP6(pdata->mc_entry.source_ip.ip.ip6));
                pdata->mc_entry.source_ip.f_ipv6 = 1;
            }
            break;

        case 'r':
		ret = str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL);
		if (ret == PPA_CMD_ERR){
			IFX_PPACMD_PRINT("ERROR: invalid input\n");
			return ret;
		}
		pdata->mc_entry.RTP_flag = ret;
		IFX_PPACMD_DBG("RTP_flag:%d \n",pdata->mc_entry.RTP_flag);
		break;

        default:
            IFX_PPACMD_PRINT("setrtp does not support parameter -%c \n", popts->opt);
            return PPA_CMD_ERR;
        }
        popts++;
    }

    if( g_opts != 1 )  return  PPA_CMD_ERR;

    return PPA_CMD_OK;
}


#endif

static const char ppa_hal_config_short_opts[] = "t:f:d";
static const struct option ppa_hal_config_long_opts[] = {
	{"name",   required_argument,  NULL, 't'},
	{"enable",   required_argument,  NULL, 'f'},
	{"show",   no_argument,  NULL, 'd'},
	{0, 0, 0, 0}
};

void ppa_hal_config_help(int summary)
{
	IFX_PPACMD_PRINT("ppacmd hal --show\n");
	IFX_PPACMD_PRINT("ppacmd hal -t <hal_name> -f [0/1]\n");
}

static int ppa_parse_hal_config_cmd(PPA_CMD_OPTS *popts, PPA_CMD_DATA *pdata)
{
	uint32_t flag = 0, i;

	ppa_memset(pdata, sizeof(*pdata), 0);

	while (popts->opt) {

		switch (popts->opt)
		{
		case 'd':
				pdata->hal_info.status = 1;
				flag = 1;
				break;
		case 't':
				strncpy_s(pdata->hal_info.hal, MAX_HAL_NAME_LEN,
				popts->optarg, MAX_HAL_NAME_LEN);
				break;
		case 'f':
				if (pdata->hal_info.hal[0]) {
						pdata->hal_info.enable =
							str_convert(STRING_TYPE_INTEGER,
							popts->optarg, NULL);
						flag = 1;
					} else {
							return PPA_CMD_ERR;
					}
				break;
		default:
				IFX_PPACMD_PRINT("Invalid Parameter passed\n");
				return PPA_CMD_ERR;
		}
		popts++;
	}
	if (flag == 0)
		return  PPA_CMD_ERR;

	return PPA_CMD_OK;
}

static void ppa_print_hal_config(PPA_CMD_DATA *cmd_info)
{
	int i;
	PPA_CMD_HAL_CONFIG_INFO *hal_info = &cmd_info->hal_info;

	if (hal_info->status == 0)
		return;

	IFX_PPACMD_PRINT("=====================\n");
	IFX_PPACMD_PRINT("All Registered HALs:\n");
	IFX_PPACMD_PRINT("=====================\n");
	for (i = 0; hal_info->axhals[i].name[0]; i++)
		IFX_PPACMD_PRINT("  %s : %s\n", hal_info->axhals[i].name,
						 hal_info->axhals[i].status == 1 ? "enabled" : "disabled");
}

static const char ppa_hal_stats_short_opts[] = "t:h";
static const struct option ppa_hal_stats_long_opts[] = {
	{"name", required_argument, NULL, 't'},
	{0, 0, 0, 0}
};

void ppa_hal_stats_help(int summary)
{
	IFX_PPACMD_PRINT("ppacmd halstats -t <hal_name>\n");
}

static int ppa_parse_hal_stats_cmd(PPA_CMD_OPTS *popts, PPA_CMD_DATA *pdata)
{
	ppa_memset(pdata, sizeof(*pdata), 0);
	if (popts->opt == 't') {
		strncpy_s(pdata->halstats_info.hal, MAX_HAL_NAME_LEN, popts->optarg, MAX_HAL_NAME_LEN);
	} else {
		IFX_PPACMD_PRINT("Invalid Parameter passed\n");
		return PPA_CMD_ERR;
	}
	return PPA_CMD_OK;
}

static void ppa_print_hal_stats(PPA_CMD_DATA *cmd_info)
{
	PPA_CMD_HAL_STATS_INFO *halstats_info = &cmd_info->halstats_info;

	if (halstats_info->get_stats == 1) {
		IFX_PPACMD_PRINT("================================================\n");
		IFX_PPACMD_PRINT("Showing STATS of %s HAL\n", halstats_info->hal);
		IFX_PPACMD_PRINT("maximum no of sessions:                     %d\n", halstats_info->all_hal_stats.max_uc);
		IFX_PPACMD_PRINT(" max no of UC sessions:                     -1\n");
		IFX_PPACMD_PRINT("    - IPV4 UC sessions:                     %d\n", halstats_info->all_hal_stats.max_uc_ipv4);
		IFX_PPACMD_PRINT("    - IPV6 UC sessions:                     %d\n", halstats_info->all_hal_stats.max_uc_ipv6);
		IFX_PPACMD_PRINT(" max no of MC sessions:                     %d\n", halstats_info->all_hal_stats.max_mc);
		IFX_PPACMD_PRINT("------------------------------------------------\n");
		IFX_PPACMD_PRINT("current no of UC sessions :                 %d\n", halstats_info->all_hal_stats.curr_uc_ipv4 + halstats_info->all_hal_stats.curr_uc_ipv6);
		IFX_PPACMD_PRINT("      -  Ipv4 UC sessions :                 %d\n", halstats_info->all_hal_stats.curr_uc_ipv4);
		IFX_PPACMD_PRINT("      -  Ipv6 UC sessions :                 %d\n", halstats_info->all_hal_stats.curr_uc_ipv6);
		IFX_PPACMD_PRINT("current no of MC sessions :                 %d\n", halstats_info->all_hal_stats.curr_mc_ipv4 + halstats_info->all_hal_stats.curr_mc_ipv6);
		IFX_PPACMD_PRINT("      -  Ipv4 MC sessions :                 %d\n", halstats_info->all_hal_stats.curr_mc_ipv4);
		IFX_PPACMD_PRINT("      -  Ipv6 MC sessions :                 %d\n", halstats_info->all_hal_stats.curr_mc_ipv6);
		IFX_PPACMD_PRINT("Total no of unicast dropped sessions :      %d\n", halstats_info->all_hal_stats.tot_uc_drop);
		IFX_PPACMD_PRINT("Total no of multicast dropped sessions :    %d\n", halstats_info->all_hal_stats.tot_mc_drop);
		IFX_PPACMD_PRINT("(Note: -1 indicates no limit in this category, it can go up to limits of parent category)\n");
		IFX_PPACMD_PRINT("================================================\n");
	} else
		return;
}

/* mib mode */
#if defined(MIB_MODE_ENABLE)

static const char ppa_mib_mode_short_opts[] = "i:"; 
static void ppa_set_mib_mode_help( int summary)
{
    if( !summary )
    {
        IFX_PPACMD_PRINT("setmibmode -i <mib-mode> ...\n");
    }
    else
    {
        IFX_PPACMD_PRINT("setmibmode -i <mib-mode>\n");
        IFX_PPACMD_PRINT("      option -i: Mib mode, 0-Byte 1-Packet\n");
    }
    return;
}

static int ppa_parse_set_mib_mode(PPA_CMD_OPTS *popts, PPA_CMD_DATA *pdata)
{
	int ret;

    ppa_memset(pdata, sizeof(*pdata), 0 );

    switch(popts->opt)
    {
   
        case 'i':
            pdata->mib_mode_info.mib_mode = str_convert(STRING_TYPE_INTEGER,  popts->optarg, NULL);;
            IFX_PPACMD_DBG("mib_mode:%d \n",pdata->mib_mode_info.mib_mode);
            break;

        default:
            IFX_PPACMD_PRINT("setmibmode does not support parameter -%c \n", popts->opt);
            return PPA_CMD_ERR;
    }


    return PPA_CMD_OK;
}
#endif


/*
====================================================================================
   command:   getmcextra
   description: get multicast extra information, like vlan/dscp
   options:
====================================================================================
*/

static const char    ppa_get_mc_extra_short_opts[] = "-:g:h";
//need to further implement add/remove/modify vlan and enable new dscp and its value
static const struct option ppa_get_mc_extra_long_opts[] =
{
    {"multicast group",   required_argument,  NULL, 'g'},
    { 0,0,0,0 }
};
static void ppa_get_mc_extra_help( int summary)
{
	IFX_PPACMD_PRINT("getmcextra -g <multicast group >\n"); // [ -m <multicast-mac-address (for bridging only)>]
	return;
}

static int ppa_parse_get_mc_extra_cmd(PPA_CMD_OPTS *popts, PPA_CMD_DATA *pdata)
{
    unsigned int g_opt=0;

    ppa_memset(pdata, sizeof(*pdata), 0 );

    while(popts->opt)
    {
        switch(popts->opt)
        {
        case 'g':
            inet_aton(popts->optarg, &pdata->mc_entry.mcast_addr);
            IFX_PPACMD_DBG("getmcextra  group ip: %d.%d.%d.%d\n", NIPQUAD( (pdata->mc_entry.mcast_addr.ip.ip)) );
            g_opt ++;
            break;

        default:
            IFX_PPACMD_PRINT("getmcextra not support parameter -%c \n", popts->opt);
            return PPA_CMD_ERR;
        }
        popts++;
    }

    if( g_opt != 1)  return PPA_CMD_ERR;

    return PPA_CMD_OK;
}

static void ppa_print_get_mc_extra_cmd(PPA_CMD_DATA *pdata)
{
	IFX_PPACMD_PRINT("multicast group %d.%d.%d.%d extra properties:\n", NIPQUAD(pdata->mc_entry.mcast_addr));
	if (pdata->mc_entry. mc_extra.dscp_remark)
		IFX_PPACMD_PRINT("new_dscp=%04x.", (unsigned int)pdata->mc_entry.mc_extra.new_dscp);
	else
		IFX_PPACMD_PRINT("Not new dscp editing");

	if (pdata->mc_entry.mc_extra.vlan_insert)
			IFX_PPACMD_PRINT("New  inner vlan =%04x", (unsigned int)(pdata->mc_entry.mc_extra.vlan_prio<<13) + (pdata->mc_entry.mc_extra.vlan_cfi<<12) +  pdata->mc_entry.mc_extra.vlan_id);
	else if (pdata->mc_entry.mc_extra.vlan_remove)
		IFX_PPACMD_PRINT("Remove inner vlan");
	else
		IFX_PPACMD_PRINT("No inner vlan editing");

	if (pdata->mc_entry.mc_extra.out_vlan_insert)
		IFX_PPACMD_PRINT("New outvlan=%04x", (unsigned int)pdata->mc_entry.mc_extra.out_vlan_tag);
	else if (pdata->mc_entry.mc_extra.out_vlan_remove)
		IFX_PPACMD_PRINT("Remove out vlan");
	else
		IFX_PPACMD_PRINT("No out vlan editing");

	if (pdata->mc_entry.mc_extra.dslwan_qid_remark)
		IFX_PPACMD_PRINT("qid: %d\n", pdata->mc_entry.mc_extra.dslwan_qid);

	IFX_PPACMD_PRINT("\n");


}


/*
====================================================================================
   command:   getmcnum
   description: get multicast groups count
   options:
====================================================================================
*/

static void ppa_get_mc_count_help(int summary)
{
	IFX_PPACMD_PRINT("getmcnum\n");
	return;
}

static int ppa_parse_get_mc_count_cmd(PPA_CMD_OPTS *popts, PPA_CMD_DATA *pdata)
{
    int res;

    res =   ppa_parse_simple_cmd( popts, pdata );

    if( res != PPA_CMD_OK ) return res;


    pdata->count_info.flag = 0;

    return PPA_CMD_OK;
}
/*
====================================================================================
   command:   getmcgroups
   description: get all multicast groups information
   options:
====================================================================================
*/

static void ppa_get_mc_groups_help( int summary)
{
	IFX_PPACMD_PRINT("getmcgroups -g <multicast group > -s <source ip> -f <ssm flag>\n"); // [ -m <multicast-mac-address (for bridging only)>]
    return;
}

static const char ppa_get_mc_group_short_opts[] = "-:g:s:f";
//need to further implement add/remove/modify vlan and enable new dscp and its value
static const struct option ppa_get_mc_group_long_opts[] =
{
    {"multicast group",   required_argument,  NULL, 'g'},
    {"source_ip",         required_argument,  NULL, 's'},
    {"ssm_flag",          required_argument,  NULL, 'f'},
    { 0,0,0,0 }
};

static int ppa_parse_get_mc_group_cmd(PPA_CMD_OPTS *popts, PPA_CMD_DATA *pdata)
{
    int ret;
    ppa_memset(pdata, sizeof(*pdata), 0 );

    while(popts->opt)
    {
        switch(popts->opt)
        {
        
        case 'g':   /*Support IPv4 and IPv6 */
            ret = str_convert(STRING_TYPE_IP, popts->optarg, pdata->mc_entry.mcast_addr.ip.ip6);
            if(ret == IP_NON_VALID){
                IFX_PPACMD_DBG("Multicast group ip is not a valid IPv4 or IPv6 IP address: %s\n", popts->optarg);
				break;
            }else if(ret == IP_VALID_V6){
                IFX_PPACMD_DBG("MLD GROUP IP: "NIP6_FMT"\n", NIP6(pdata->mc_entry.mcast_addr.ip.ip6));
                pdata->mc_entry.mcast_addr.f_ipv6= 1;
            }
            
            break;

        case 's': // Src IP, Support IPv4 & IPv6

            ret = str_convert(STRING_TYPE_IP, popts->optarg, pdata->mc_entry.source_ip.ip.ip6);
            if(ret == IP_NON_VALID){
                IFX_PPACMD_DBG("Multicast group ip is not a valid IPv4 or IPv6 IP address: %s\n", popts->optarg);
				break;
            }else if(ret == IP_VALID_V6){
                IFX_PPACMD_DBG("Source IP: "NIP6_FMT"\n", NIP6(pdata->mc_entry.mcast_addr.ip.ip6));
                pdata->mc_entry.source_ip.f_ipv6 = 1;
            }
            break;

        case 'f':
            pdata->mc_entry.SSM_flag = str_convert(STRING_TYPE_INTEGER,  popts->optarg, NULL);;
            IFX_PPACMD_DBG("addmc SSM_flag:%d \n",pdata->mc_entry.SSM_flag);
            break;

        default:
            IFX_PPACMD_PRINT("mc_get_mc_group not support parameter -%c \n", popts->opt);
            return PPA_CMD_ERR;
        }
        popts++;
    }

    return PPA_CMD_OK;
}

static int ppa_get_mc_groups_cmd (PPA_COMMAND *pcmd, PPA_CMD_DATA *pdata)
{
    PPA_CMD_MC_GROUPS_INFO *psession_buffer;
    PPA_CMD_DATA cmd_info;
    int res = PPA_CMD_OK, i, j;
	uint32_t size;
    uint32_t flag = PPA_CMD_GET_MC_GROUPS;
    char str_srcip[MAX_STR_IP], str_dstip[MAX_STR_IP];

    //get session count first before malloc memroy
    cmd_info.count_info.flag = 0;
    if( ppa_do_ioctl_cmd(PPA_CMD_GET_COUNT_MC_GROUP, &cmd_info ) != PPA_CMD_OK )
        return -EIO;

    if( cmd_info.count_info.count == 0 )
    {
        IFX_PPACMD_PRINT("MC groups count=0\n");
        return PPA_CMD_OK;
    }

    //malloc memory and set value correctly
    size = sizeof(PPA_CMD_COUNT_INFO) + sizeof(PPA_CMD_MC_GROUP_INFO) * ( 1 + cmd_info.count_info.count ) ;
    psession_buffer = (PPA_CMD_MC_GROUPS_INFO *) malloc( size );
    if( psession_buffer == NULL )
    {
        IFX_PPACMD_PRINT("Malloc %d bytes failed\n", size );
        return PPA_CMD_NOT_AVAIL;
    }
    IFX_PPACMD_DBG("Get buffer size=%ul\n", size);
    ppa_memset( psession_buffer, size, 0);

    psession_buffer->count_info.count = cmd_info.count_info.count;
    psession_buffer->count_info.flag = 0;

    IFX_PPACMD_DBG("specified dst ip: %d.%d.%d.%d, src ip: %d.%d.%d.%d \n", 
        NIPQUAD(pdata->mc_entry.mcast_addr.ip.ip),
        NIPQUAD(pdata->mc_entry.source_ip.ip.ip));

    //get session information
    if( (res = ppa_do_ioctl_cmd(flag, psession_buffer ) != PPA_CMD_OK ) )
    {
        free( psession_buffer );
        return res;
    }

    IFX_PPACMD_DBG("MC groups total count=%u. \n", (unsigned int)psession_buffer->count_info.count);

#if defined(MIB_MODE_ENABLE)
            PPA_CMD_MIB_MODE_INFO mode_info;
            char str_mib[12];
            if( ppa_do_ioctl_cmd(PPA_CMD_GET_MIB_MODE, &mode_info) == PPA_CMD_OK )
            {
                if(mode_info.mib_mode == 1)
                        strcpy_s(str_mib,12," packet");
                else
                        strcpy_s(str_mib,12," byte");
            }
            else
            {
                free( psession_buffer );
                return PPA_CMD_ERR;
            }
#endif

	for (i=0; i<psession_buffer->count_info.count; i++) {
		if (!is_ip_zero(&pdata->mc_entry.mcast_addr)) {
			if(!ip_equal(&pdata->mc_entry.mcast_addr,&psession_buffer->mc_group_list[i].mc.mcast_addr)
                    || (pdata->mc_entry.SSM_flag == 1 
                          && !ip_equal(&pdata->mc_entry.source_ip,&psession_buffer->mc_group_list[i].mc.source_ip)))
				continue;
            }
		if (!psession_buffer->mc_group_list[i].mc.mcast_addr.f_ipv6) {
			if (psession_buffer->mc_group_list[i].mc.source_ip.ip.ip == 0) {
				snprintf(str_srcip,MAX_STR_IP, "%s", "ANY");
			} else {
				snprintf(str_srcip,MAX_STR_IP, NIPQUAD_FMT, NIPQUAD(psession_buffer->mc_group_list[i].mc.source_ip.ip.ip));
                    }
				//format like: [002] Dst: 239.  2.  2.  3  Src: 1.1.1.1 \n\t (route) qid  0 vlan 0000/04x From  nas0 to  eth0 ADDED_IN_HW|VALID_PPPOE|VALID_NEW_SRC_MAC
                    
#if defined(MIB_MODE_ENABLE)
			IFX_PPACMD_PRINT("[%03u] MC GROUP:%3u.%3u.%3u.%3u Src IP: %s \n\t (%s) qid(%2u) dscp(%2u) vlan (%04x/%04x) mib (%llu:%llu(cpu:hw mib in%s)) ", i,
#else
			IFX_PPACMD_PRINT("[%03u] MC GROUP:%3u.%3u.%3u.%3u Src IP: %s \n\t (%s) qid(%2u) dscp(%2u) vlan (%04x/%04x) mib (%llu:%llu) ", i,
#endif
					NIPQUAD((psession_buffer->mc_group_list[i].mc.mcast_addr.ip.ip)), str_srcip,
					psession_buffer->mc_group_list[i].bridging_flag ? "bridge":"route ",
					(unsigned int)psession_buffer->mc_group_list[i].mc.mc_extra.dslwan_qid,
					(unsigned int)psession_buffer->mc_group_list[i].mc.mc_extra.new_dscp,
					(unsigned int)( psession_buffer->mc_group_list[i].mc.mc_extra.vlan_prio << 13) |( psession_buffer->mc_group_list[i].mc.mc_extra.vlan_cfi << 12) | psession_buffer->mc_group_list[i].mc.mc_extra.vlan_id,
					(unsigned int)psession_buffer->mc_group_list[i].mc.mc_extra.out_vlan_tag,
					(unsigned long long)psession_buffer->mc_group_list[i].mc.host_bytes,
					(unsigned long long)psession_buffer->mc_group_list[i].mc.hw_bytes
#if defined(MIB_MODE_ENABLE)
					, str_mib
#endif
					);

#if defined(RTP_SAMPLING_ENABLE) && RTP_SAMPLING_ENABLE
				if (psession_buffer->mc_group_list[i].mc.RTP_flag == 1)
				{
					IFX_PPACMD_PRINT("rtp pkt cnt(%u) rtp seq num(%u) ",
						psession_buffer->mc_group_list[i].mc.rtp_pkt_cnt,
						psession_buffer->mc_group_list[i].mc.rtp_seq_num
						);
				}
#endif
		} else {
			if (is_ip_zero(&psession_buffer->mc_group_list[i].mc.source_ip)) {
				snprintf(str_srcip, MAX_STR_IP,"%s", "ANY");
			} else {
				snprintf(str_srcip,MAX_STR_IP, NIP6_FMT, NIP6(psession_buffer->mc_group_list[i].mc.source_ip.ip.ip6));
			}
			snprintf(str_dstip,MAX_STR_IP, NIP6_FMT, NIP6(psession_buffer->mc_group_list[i].mc.mcast_addr.ip.ip6));

			IFX_PPACMD_PRINT("[%03u] MC GROUP:%s Src IP: %s \n\t (%s) qid(%2u) dscp(%2u) vlan (%04x/%04x) mib (%llu:%llu(host:hw)) ", i,
						str_dstip, str_srcip,
						psession_buffer->mc_group_list[i].bridging_flag ? "bridge":"route ",
						(unsigned int)psession_buffer->mc_group_list[i].mc.mc_extra.dslwan_qid,
						(unsigned int)psession_buffer->mc_group_list[i].mc.mc_extra.new_dscp,
						(unsigned int)( psession_buffer->mc_group_list[i].mc.mc_extra.vlan_prio << 13) |( psession_buffer->mc_group_list[i].mc.mc_extra.vlan_cfi << 12) | psession_buffer->mc_group_list[i].mc.mc_extra.vlan_id,
						(unsigned int)psession_buffer->mc_group_list[i].mc.mc_extra.out_vlan_tag,
						(unsigned long long)psession_buffer->mc_group_list[i].mc.host_bytes,
						(unsigned long long)psession_buffer->mc_group_list[i].mc.hw_bytes
						);
#if defined(RTP_SAMPLING_ENABLE) && RTP_SAMPLING_ENABLE
				if (psession_buffer->mc_group_list[i].mc.RTP_flag == 1) {
					IFX_PPACMD_PRINT("rtp pkt cnt(%u) rtp seq num(%u)",
							psession_buffer->mc_group_list[i].mc.rtp_pkt_cnt,
							psession_buffer->mc_group_list[i].mc.rtp_seq_nu
							);
				}
#endif

                
		}

		if (strnlen_s(psession_buffer->mc_group_list[i].src_ifname,PPA_IF_NAME_SIZE ) == 0)
			IFX_PPACMD_PRINT("From N/A ");
		else
		{
			IFX_PPACMD_PRINT("From %s ", psession_buffer->mc_group_list[i].src_ifname);
		}

		if( psession_buffer->mc_group_list[i].num_ifs ==0 ||psession_buffer->mc_group_list[i].lan_ifname[0] == 0 )
			IFX_PPACMD_PRINT("to N/A");
		else
		{
			IFX_PPACMD_PRINT("to ");
			for(j=0; j<psession_buffer->mc_group_list[i].num_ifs; j++)
			{
				if (j == 0)
					IFX_PPACMD_PRINT("%s", psession_buffer->mc_group_list[i].lan_ifname[j]);
				else
					IFX_PPACMD_PRINT("/%s", psession_buffer->mc_group_list[i].lan_ifname[j]);
			}
		}
		IFX_PPACMD_PRINT("\n\t ");
		print_session_flags(psession_buffer->mc_group_list[i].mc.flags);
		IFX_PPACMD_PRINT("\n");
	}

    free( psession_buffer );
    return PPA_CMD_OK;
}


/*
====================================================================================
   command:   getlansessionum/getwansessionnum
   description: get LAN/WAN session count
   options:
====================================================================================
*/
static void ppa_get_lan_session_count_help( int summary)
{
	IFX_PPACMD_PRINT("getlansessionnum [-w | -s | n | a ]\n");
    
    if( summary )
    {
        IFX_PPACMD_PRINT("  -w: all ppe accelerated sessions in PPA\n");
        IFX_PPACMD_PRINT("  -s: all software accelerated sessions in PPA\n");
        IFX_PPACMD_PRINT("  -n: all non-accelerated sessions in PPA\n");
        IFX_PPACMD_PRINT("  -a: all sessions in PPA\n");
    }
    return;
}

static void ppa_get_wan_session_count_help( int summary)
{
	IFX_PPACMD_PRINT("getwansessionnum [-w | -s | n | a ]\n");
    if( summary )
    {
        IFX_PPACMD_PRINT("  -w: all ppe accelerated sessions in PPA\n");
        IFX_PPACMD_PRINT("  -s: all software accelerated sessions in PPA\n");
        IFX_PPACMD_PRINT("  -n: all non-accelerated sessions in PPA\n");
        IFX_PPACMD_PRINT("  -a: all sessions in PPA\n");
    }
    return;
}

static const char ppa_get_simple_short_opts[] = "-:ah";
static const struct option ppa_get_simple_long_opts[] =
{
    {"all",   no_argument,  NULL, 'a'},
    { 0,0,0,0 }
};

/*this is a simple template parse command. At most there is only one parameter for saving result to file */
static int ppa_parse_simple_cmd(PPA_CMD_OPTS *popts, PPA_CMD_DATA *pdata)
{
    while(popts->opt)
    {
        switch(popts->opt)
        {
        case 'a':
            g_all = 1;
            break;

        case 'w': //only for getlan/wassessionnum
            pdata->session_info.count_info.flag = SESSION_ADDED_IN_HW;
            break;
        
        case 'n': //only for getlan/wassessionnum
            pdata->session_info.count_info.flag = SESSION_NON_ACCE_MASK;
            break;
        
        default:
            return PPA_CMD_ERR;
        }
        popts++;
    }

    return PPA_CMD_OK;
}

static int ppa_parse_get_lan_session_count(PPA_CMD_OPTS *popts, PPA_CMD_DATA *pdata)
{

    pdata->count_info.flag =0;
    while(popts->opt)
    {
        switch(popts->opt)
        {
        case 'w':
            pdata->count_info.flag = SESSION_ADDED_IN_HW;
            break;

        case 's':
            pdata->count_info.flag = SESSION_ADDED_IN_SW;
            break;

        case 'n':
            pdata->count_info.flag = SESSION_NON_ACCE_MASK;
            break;       

        case 'a':
            pdata->count_info.flag = 0;
            break;

        default:
            return PPA_CMD_ERR;
        }
        popts++;
    }

    return PPA_CMD_OK;
}


static int ppa_parse_get_wan_session_count(PPA_CMD_OPTS *popts, PPA_CMD_DATA *pdata)
{

    pdata->count_info.flag =0;
    while(popts->opt)
    {
        switch(popts->opt)
        {
        case 'w':
            pdata->count_info.flag = SESSION_ADDED_IN_HW;
            break;
        
	case 's':
            pdata->count_info.flag = SESSION_ADDED_IN_SW;
            break;

        case 'n':
            pdata->count_info.flag = SESSION_NON_ACCE_MASK;
            break;       

        case 'a':
            pdata->count_info.flag = 0;
            break;

        default:
            return PPA_CMD_ERR;
        }
        popts++;
    }

    return PPA_CMD_OK;
}


/*
====================================================================================
   command:   getlansessions/getwansessions
   description: get LAN/WAN all session detail information
   options:
====================================================================================
*/

static const char ppa_get_session_short_opts[] = "-:wsnafd:h";
static void ppa_get_lan_sessions_help( int summary)
{
	IFX_PPACMD_PRINT("getlansessions [-w] [-s] [-a ] [-n ] [ -f ] [-d delay]\n");
    if( summary )
    {
        IFX_PPACMD_PRINT("  -w: all ppe-accelerated sessions in PPA\n");
        IFX_PPACMD_PRINT("  -s: all software-accelerated sessions in PPA\n");
        IFX_PPACMD_PRINT("  -n: all non-accelerated sessions in PPA\n");
        IFX_PPACMD_PRINT("  -a: all sessions in PPA\n");
        IFX_PPACMD_PRINT("  -f: Set SESSION_BYTE_STAMPING flag for testing purpose\n");
        IFX_PPACMD_PRINT("  -d: sleep time in seconds. For testing purpose\n");
    }
    return;
}

#if defined(CONFIG_PPA_HANDLE_CONNTRACK_SESSIONS)
static const char ppa_get_session_criteria_short_opts[] = "a";
static const char ppa_swap_sessions_short_opts[] = "a";
#endif
static const char ppa_get_session_count_short_opts[] = "-:wsna:h";
static int delay_in_second =0;

static void ppa_get_wan_sessions_help( int summary)
{
	IFX_PPACMD_PRINT("getwansessions [-w] [-s] [-a ] [-n ] [ -f ] [-d delay]\n");
    if( summary )
    {
        IFX_PPACMD_PRINT("  -w: all ppe-accelerated sessions in PPA\n");
        IFX_PPACMD_PRINT("  -s: all software-accelerated sessions in PPA\n");
        IFX_PPACMD_PRINT("  -n: all non-accelerated sessions in PPA\n");
        IFX_PPACMD_PRINT("  -a: all sessions in PPA\n");
        IFX_PPACMD_PRINT("  -f: Set SESSION_BYTE_STAMPING flag for testing purpose\n");
        IFX_PPACMD_PRINT(" -d: sleep time in seconds. For testing purpose\n");
    }
    return;
}

static int ppa_parse_get_session_cmd(PPA_CMD_OPTS *popts,PPA_CMD_DATA *pdata)
{
    pdata->session_info.count_info.flag = 0;  //default get all lan or wan sessions

    while(popts->opt)
    {
        switch(popts->opt)
        {
        case 'w':
            pdata->session_info.count_info.flag = SESSION_ADDED_IN_HW;
            break;
        
	case 's':
            pdata->session_info.count_info.flag = SESSION_ADDED_IN_SW;
            break;

        case 'n':
            pdata->session_info.count_info.flag = SESSION_NON_ACCE_MASK;
            break;

        case 'a':
            pdata->session_info.count_info.flag = 0;
            break;

        case 'f':
            pdata->session_info.count_info.stamp_flag |= SESSION_BYTE_STAMPING;
            break;

       case 'd':
            delay_in_second= str_convert(STRING_TYPE_INTEGER,  popts->optarg, NULL);
            break;     

        default:
            return PPA_CMD_ERR;
        }
        popts++;
    }

    return PPA_CMD_OK;
}

static PPA_CMD_SESSIONS_INFO *ppa_get_sessions_malloc (uint32_t session_flag, uint32_t flag, uint32_t stamp_flag, uint32_t hash_index)
{
    PPA_CMD_SESSIONS_INFO *psession_buffer;
    PPA_CMD_DATA cmd_info;
    int res = PPA_CMD_OK, size;

    ppa_memset( &cmd_info, sizeof(cmd_info),0 );
    cmd_info.count_info.flag = session_flag;
    cmd_info.count_info.stamp_flag = 0;
    cmd_info.count_info.hash_index = hash_index;

    if( flag == PPA_CMD_GET_LAN_SESSIONS )
    {
        if( ppa_do_ioctl_cmd(PPA_CMD_GET_COUNT_LAN_SESSION, &cmd_info ) != PPA_CMD_OK )
            return NULL;
    }
    else if( flag == PPA_CMD_GET_WAN_SESSIONS )
    {
        if( ppa_do_ioctl_cmd(PPA_CMD_GET_COUNT_WAN_SESSION, &cmd_info ) != PPA_CMD_OK )
            return NULL;
    }    
    else if( flag == PPA_CMD_GET_LAN_WAN_SESSIONS )
    {
        IFX_PPACMD_DBG("PPA_CMD_GET_LAN_WAN_SESSIONS\n");
        if( ppa_do_ioctl_cmd(PPA_CMD_GET_COUNT_LAN_WAN_SESSION, &cmd_info ) != PPA_CMD_OK )
            return NULL;
    }
    
    if( cmd_info.count_info.count == 0 )
    {
        IFX_PPACMD_DBG("session count=0. \n");
        return NULL;
    }

    //malloc memory and set value correctly
    size = sizeof(PPA_CMD_SESSIONS_INFO) + sizeof(PPA_CMD_SESSION_ENTRY) * ( cmd_info.count_info.count + 1 );
    psession_buffer = (PPA_CMD_SESSIONS_INFO *) malloc( size );
    if( psession_buffer == NULL )
    {
        IFX_PPACMD_PRINT("ppa_get_sessions_malloc : Memory allocation Fail for psession_buffer\n" );
        return NULL;
    }
    
    ppa_memset( psession_buffer, sizeof(size), 0 );

    psession_buffer->count_info.count = cmd_info.count_info.count;
    psession_buffer->count_info.flag = session_flag;
    psession_buffer->count_info.stamp_flag = stamp_flag;
    psession_buffer->count_info.hash_index = hash_index;
    //get session information
    if( delay_in_second ) sleep(delay_in_second);
    if( (res = ppa_do_ioctl_cmd(flag, psession_buffer ) != PPA_CMD_OK ) )
    {
        free( psession_buffer );
        IFX_PPACMD_PRINT("ppa_get_sessions_malloc failed for ioctl not succeed.\n");
        return NULL;
    }

    
    return psession_buffer;
}


static int ppa_get_sessions (PPA_COMMAND *pcmd, PPA_CMD_DATA *pdata, uint32_t flag)
{
    PPA_CMD_SESSIONS_INFO *psession_buffer;
    int i=0, size=0;
    unsigned int hash_index = 0;
    unsigned int session_no=0;

    for(hash_index=0; hash_index<SESSION_LIST_HASH_TABLE_SIZE; hash_index++)
    {
        psession_buffer = ppa_get_sessions_malloc(pdata->session_info.count_info.flag, flag, pdata->session_info.count_info.stamp_flag, hash_index+1);
        if( !psession_buffer )
        {
            continue;
        }
        
		IFX_PPACMD_PRINT("\nSession Information: %5u in PPA Hash[%02u] -------\n", (unsigned int)psession_buffer->count_info.count, hash_index );
#if defined(MIB_MODE_ENABLE)
		PPA_CMD_MIB_MODE_INFO mode_info;
		char str_mib[12];
		if (ppa_do_ioctl_cmd(PPA_CMD_GET_MIB_MODE, &mode_info) == PPA_CMD_OK)
		{
			if(mode_info.mib_mode == 1)
				strcpy_s(str_mib,12," packet");
			else
				strcpy_s(str_mib,12," byte");
		}
		else
		{
			free( psession_buffer );
			return PPA_CMD_ERR;
		}
#endif /* MIB_MODE_ENABLE */

		for (i=0; i<psession_buffer->count_info.count; i++)
		{
			//print format: <packet index> <packet-type>  <rx interface name> (source ip : port) -> <tx interface name> ( dst_ip : dst_port ) nat ( nat_ip: nat_port )
			IFX_PPACMD_PRINT("[%03d]", i + session_no);

			if( psession_buffer->session_list[i].ip_proto == 0x11 ) //UDP
				IFX_PPACMD_PRINT("udp");
			else if( psession_buffer->session_list[i].ip_proto == 6)
				IFX_PPACMD_PRINT("tcp");
			else
				IFX_PPACMD_PRINT("---");
			if( psession_buffer->session_list[i].flags & SESSION_IS_IPV6 )

#if defined(MIB_MODE_ENABLE)
			IFX_PPACMD_PRINT(": %8s (%04x:%04x:%04x:%04x:%04x:%04x:%04x:%04x/%5d) -> %8s (%04x:%04x:%04x:%04x:%04x:%04x:%04x:%04x/%5d) NAT (%04x:%04x:%04x:%04x:%04x:%04x:%04x:%04x/%5d) (%llu:%llu(hw mib in %s )) @session0x%p with coll %d pri %d\n",
					psession_buffer->session_list[i].rx_if_name,
					NIP6(psession_buffer->session_list[i].src_ip.ip6),
					psession_buffer->session_list[i].src_port,
					psession_buffer->session_list[i].tx_if_name,
					NIP6(psession_buffer->session_list[i].dst_ip.ip6),
					psession_buffer->session_list[i].dst_port,
					NIP6(psession_buffer->session_list[i].natip.ip6),
					psession_buffer->session_list[i].nat_port,
					psession_buffer->session_list[i].host_bytes,
					psession_buffer->session_list[i].hw_bytes,str_mib,
					psession_buffer->session_list[i].session,
					psession_buffer->session_list[i].collision_flag,
					psession_buffer->session_list[i].priority);


#else
			IFX_PPACMD_PRINT(": %8s (%04x:%04x:%04x:%04x:%04x:%04x:%04x:%04x/%5d) -> %8s (%04x:%04x:%04x:%04x:%04x:%04x:%04x:%04x/%5d) NAT (%04x:%04x:%04x:%04x:%04x:%04x:%04x:%04x/%5d) (%llu:%llu(hw)) @session0x%p with coll %d pri %d\n",
					psession_buffer->session_list[i].rx_if_name,
					NIP6(psession_buffer->session_list[i].src_ip.ip6),
					psession_buffer->session_list[i].src_port,
					psession_buffer->session_list[i].tx_if_name,
					NIP6(psession_buffer->session_list[i].dst_ip.ip6),
					psession_buffer->session_list[i].dst_port,
					NIP6(psession_buffer->session_list[i].natip.ip6), psession_buffer->session_list[i].nat_port,
					(unsigned long long)psession_buffer->session_list[i].host_bytes,
					(unsigned long long)psession_buffer->session_list[i].hw_bytes,
					psession_buffer->session_list[i].session,
					psession_buffer->session_list[i].collision_flag,
					psession_buffer->session_list[i].priority);
		else
#endif

#if defined(MIB_MODE_ENABLE)
			IFX_PPACMD_PRINT(": %8s (%3d.%3d.%3d.%3d/%5d) -> %8s (%3d.%3d.%3d.%3d/%5d) NAT (%3d.%3d.%3d.%3d/%5d) (%llu:%llu(hw mib in %s))  @session0x%p with coll %d pri %d\n",
					psession_buffer->session_list[i].rx_if_name,
					NIPQUAD(psession_buffer->session_list[i].src_ip),
					psession_buffer->session_list[i].src_port,
					psession_buffer->session_list[i].tx_if_name,
					NIPQUAD(psession_buffer->session_list[i].dst_ip),
					psession_buffer->session_list[i].dst_port,
					NIPQUAD(psession_buffer->session_list[i].natip),
					psession_buffer->session_list[i].nat_port,
					psession_buffer->session_list[i].host_bytes,
					psession_buffer->session_list[i].hw_bytes,str_mib,
					psession_buffer->session_list[i].session,
					psession_buffer->session_list[i].collision_flag,
					psession_buffer->session_list[i].priority);

#else
			IFX_PPACMD_PRINT(": %8s (%3d.%3d.%3d.%3d/%5d) -> %8s (%3d.%3d.%3d.%3d/%5d) NAT (%3d.%3d.%3d.%3d/%5d) (%llu:%llu(hw))  @session0x%p with coll %d pri %d\n",
					psession_buffer->session_list[i].rx_if_name,
					NIPQUAD(psession_buffer->session_list[i].src_ip),
					psession_buffer->session_list[i].src_port,
					psession_buffer->session_list[i].tx_if_name,
					NIPQUAD(psession_buffer->session_list[i].dst_ip),
					psession_buffer->session_list[i].dst_port,
					NIPQUAD(psession_buffer->session_list[i].natip),
					psession_buffer->session_list[i].nat_port,
					(unsigned long long)psession_buffer->session_list[i].host_bytes,
					(unsigned long long)psession_buffer->session_list[i].hw_bytes,
					psession_buffer->session_list[i].session,
					psession_buffer->session_list[i].collision_flag,
					psession_buffer->session_list[i].priority);

#endif
		}

        free( psession_buffer );
    }
    return PPA_CMD_OK;
}



static int ppa_get_lan_sessions_cmd (PPA_COMMAND *pcmd, PPA_CMD_DATA *pdata)
{
    return ppa_get_sessions(pcmd, pdata, PPA_CMD_GET_LAN_SESSIONS);
}

static int ppa_get_wan_sessions_cmd (PPA_COMMAND *pcmd, PPA_CMD_DATA *pdata)
{
    return ppa_get_sessions(pcmd, pdata, PPA_CMD_GET_WAN_SESSIONS);
}


#if defined(CONFIG_PPA_HANDLE_CONNTRACK_SESSIONS)
static int ppa_swap_sessions(PPA_COMMAND *pcmd, PPA_CMD_DATA *pdata)
{
}
static int ppa_get_session_criteria(PPA_COMMAND *pcmd, PPA_CMD_DATA *pdata)
{
	PPA_CMD_SESSIONS_CRITERIA_INFO *session_criteria_info=NULL;
    	int res = PPA_CMD_OK;
    	PPA_CMD_DATA cmd_info;
	if( ppa_do_ioctl_cmd(PPA_CMD_GET_SESSIONS_CRITERIA, &cmd_info) != PPA_CMD_OK ) {
        	IFX_PPACMD_PRINT("PPA_CMD_GET_SESSIONS_CRITERIA failed\n");
        	return PPA_CMD_ERR;
    	}
    IFX_PPACMD_PRINT("low_prio_rate=%d def_prio_rate=%d low_prio_thresh=%d def_prio_thresh=%d\n",cmd_info.session_criteria_info.ppa_low_prio_data_rate,cmd_info.session_criteria_info.ppa_def_prio_data_rate,cmd_info.session_criteria_info.ppa_low_prio_thresh,cmd_info.session_criteria_info.ppa_def_prio_thresh);

    free(session_criteria_info);
    return PPA_CMD_OK;
}
#endif

#if defined(CONFIG_PPA_HANDLE_CONNTRACK_SESSIONS)
static void ppa_get_session_criteria_help()
{
    IFX_PPACMD_PRINT("getsessioncriteria\n");
}
static int ppa_parse_get_session_criteria(PPA_CMD_OPTS *popts,PPA_CMD_DATA *pdata)
{
    return PPA_CMD_OK;
}
#endif

/*
====================================================================================
   command:   delsession
   description: delete a routing session based on IP address (IPv4/v6) or MAC address
   options:
====================================================================================
*/
static const char ppa_del_session_short_opts[] = "-:i:m:";

static void ppa_del_session_help(void)
{
    IFX_PPACMD_PRINT("delsession [-i IPv4/v6_address(source/destination)] [-m MAC_address(source/destination)]\n");
    return;
}

static int ppa_parse_del_session_cmd(PPA_CMD_OPTS *popts, PPA_CMD_DATA *pdata)
{
    unsigned int i_opt=0, m_opt=0, t_opt=0;
    unsigned int ip_type=0;

    while(popts->opt)
    {
        switch(popts->opt)
        {
        case 'i':
            if( (ip_type = str_convert(STRING_TYPE_IP, popts->optarg, (void *)&pdata->del_session.u.ip)) == IP_NON_VALID )
            {
                IFX_PPACMD_PRINT("Wrong source ip:%s\n", popts->optarg);
                return PPA_CMD_ERR;
            };
            if( ip_type == IP_VALID_V6 )
                pdata->del_session.type |= PPA_SESS_DEL_USING_IPv6;
            else
                pdata->del_session.type |= PPA_SESS_DEL_USING_IPv4;

            i_opt ++;
            break;

        case 'm':
		if (! isValidMacAddress(popts->optarg)){
			IFX_PPACMD_PRINT("Wrong mac address:%s\n", popts->optarg);
			return PPA_CMD_ERR;

		}
		stomac(popts->optarg, pdata->del_session.u.mac_addr);
		pdata->del_session.type |= PPA_SESS_DEL_USING_MAC;
		m_opt ++;
		break;


        default:
            return PPA_CMD_ERR;
        }
        popts++;
    }
    if( i_opt == 0 && m_opt==0 && t_opt==0 )  /*no parameter is provided */
    {
        ppa_del_session_help();
        return PPA_CMD_ERR;
    }
    if( i_opt > 1 || m_opt > 1 || t_opt > 1 )
    {
        IFX_PPACMD_PRINT("wrong input: -i/m.\n");
        ppa_del_session_help();
        return PPA_CMD_ERR;
    }

    if( m_opt == 1)
    {
        IFX_PPACMD_DBG("delete session via mac address: %02x:%02x:%02x:%02x:%02x:%02x\n",
                        pdata->del_session.u.mac_addr[0],
                        pdata->del_session.u.mac_addr[1],
                        pdata->del_session.u.mac_addr[2],
                        pdata->del_session.u.mac_addr[3],
                        pdata->del_session.u.mac_addr[4],
                        pdata->del_session.u.mac_addr[5]);
    }
    else
    {
        if( pdata->del_session.type & PPA_SESS_DEL_USING_IPv6)
        {
            IFX_PPACMD_DBG("delete session via ppa session with source/destination IPv6 address %04x:%04x:%04x:%04x:%04x:%04x:%04x:%04x",
                    NIP6(pdata->del_session.u.ip.ip6));
        }
        else
        {
            IFX_PPACMD_DBG("delete session via ppa session with source/destination IPv4 address %u.%u.%u.%u",
                    NIPQUAD(pdata->del_session.u.ip.ip));
        }
    }
    return PPA_CMD_OK;

}

/*
====================================================================================
   command:   addsession
   description: delete a routing session
   options:
====================================================================================
*/
static const char ppa_add_session_short_opts[] = "-:f:t:s:d:p:i:w:x:y:n:m:o:r:c:q:z:h";
static const struct option ppa_add_session_long_opts[] =
{
    {"src-ip",   required_argument,  NULL, 'f'},
    {"dst-ip",   required_argument,  NULL, 't'},
    {"src-port",   required_argument,  NULL, 's'},
    {"dst-port",   required_argument,  NULL, 'd'},
    {"proto",   required_argument,  NULL, 'p'},
    {"dest_ifid",   required_argument,  NULL, 'i'},
    {"wan-flag",   required_argument,  NULL, 'w'},
    {"src-mac",   required_argument,  NULL, 'x'},
    {"dst-mac",   required_argument,  NULL, 'y'},


    {"nat-ip",   required_argument,  NULL, 'n'},
    {"nat-port",   required_argument,  NULL, 'm'},
    {"new_dscp",   required_argument,  NULL, 'o'},

    {"in_vlan_id",   required_argument,  NULL, 'r'},
    {"out_vlan_tag",   required_argument,  NULL, 'c'},
    {"qid",   required_argument,  NULL, 'q'},
    {"pppoe-id",   required_argument,  NULL, 'z'},

    { 0,0,0,0 }
};

#define DEFAULT_SRC_IP "192.168.168.100"
#define DEFAULT_DST_IP "192.168.0.100"
#define DEFAULT_SRC_PORT "1024"
#define DEFAULT_DST_PORT "1024"
#define DEFAULT_SRC_MAC "00:11:22:33:44:11"
#define DEFAULT_DST_MAC "00:11:22:33:44:22"
static void ppa_add_session_help( int summary)
{
    IFX_PPACMD_PRINT("addsession\n");
    if( summary )
    {
        IFX_PPACMD_PRINT("addsession [-f src-ip] [-t dst-ip ] [-s src-port ] [-d dst-port] [-p proto] [-i dest_ifid] [-w wan-flag]\n");
        IFX_PPACMD_PRINT("         [-x src-mac] [-y dst-mac]\n");
        IFX_PPACMD_PRINT("         [-n nat-ip] [-m nat-port ] [-o new_dscp ] \n");
        IFX_PPACMD_PRINT("         [-r in_vlan_id] [-c out_vlan_tag ] [-q queue-id ] [-z pppoe-id] \n");
        IFX_PPACMD_PRINT("  Note:  This commands is only for test purpose !!!\n");
        IFX_PPACMD_PRINT("        a) [-f src-ip]: default is %s\n", DEFAULT_SRC_IP);
        IFX_PPACMD_PRINT("        b) [-t dst-ip]: default is %s\n", DEFAULT_DST_IP);
        IFX_PPACMD_PRINT("        c) [-s src-port ]: default is %s\n", DEFAULT_SRC_PORT);
        IFX_PPACMD_PRINT("        d) [-d dst-port ]: default is %s\n", DEFAULT_DST_PORT);
        IFX_PPACMD_PRINT("        e) [-p proto]: default is udp.  Supported value are  udp and tcp\n");
        IFX_PPACMD_PRINT("        f) [-i dest_ifid]: default is 1, ie, eth1\n");
        IFX_PPACMD_PRINT("        g) [-w wan-flag]: default is lan, supported value are lan and wan\n");
        IFX_PPACMD_PRINT("        h) [-i dest_ifid]: default is 1, ie, eth1\n");
        IFX_PPACMD_PRINT("        i) [-x src-mac]: default is %s\n", DEFAULT_SRC_MAC );
        IFX_PPACMD_PRINT("        j) [-y dst_mac]: default is %s\n", DEFAULT_DST_MAC);
        IFX_PPACMD_PRINT("        l) [-n nat-ip]: default is 0, ie no NAT\n");
        IFX_PPACMD_PRINT("        l) [-m nat-port]: default is 0, ie, no NAT\n");
        IFX_PPACMD_PRINT("        m) [-o new_dscp]: default is 0, ie, no dscp change\n");
        IFX_PPACMD_PRINT("        n) [-r in_vlan_id]: default is 0, ie, no inner vlan change\n");
        IFX_PPACMD_PRINT("        o) [-c out_vlan_id]: default is 0, ie, no out vlan change\n");
        IFX_PPACMD_PRINT("        p) [-q queue-id]: default is 0\n");
        IFX_PPACMD_PRINT("        q) [-q pppoe-id]: default is 0, ie, no pppoe header insert\n");
    }
    return;
}

static int ppa_parse_add_session_cmd(PPA_CMD_OPTS *popts, PPA_CMD_DATA *pdata)
{
    unsigned int i_opt=0, f_opt=0, t_opt=0, s_opt=0,d_opt=0, p_opt=0, w_opt=0, x_opt=0, y_opt=0, n_opt=0,m_opt=0, o_opt=0, r_opt=0, c_opt=0,q_opt=0,z_opt=0;
    unsigned int ip1_type=0, ip2_type=0, ip3_type=0;

    ppa_memset( &pdata->detail_session_info, sizeof(pdata->detail_session_info), 0 );
    pdata->detail_session_info.ip_proto = PPA_IPPROTO_UDP;
    pdata->detail_session_info.flags = SESSION_LAN_ENTRY | SESSION_VALID_OUT_VLAN_RM | SESSION_VALID_OUT_VLAN_RM | SESSION_VALID_MTU | SESSION_VALID_NEW_SRC_MAC;

    ip1_type = str_convert(STRING_TYPE_IP, DEFAULT_SRC_IP, (void *)&pdata->detail_session_info.src_ip);
    if( ip1_type == IP_VALID_V6)
        pdata->detail_session_info.flags |= SESSION_IS_IPV6;
    ip2_type = str_convert(STRING_TYPE_IP, DEFAULT_DST_IP, (void *)&pdata->detail_session_info.dst_ip);
    if( ip2_type == IP_VALID_V6 )
        pdata->detail_session_info.flags |= SESSION_IS_IPV6;

    pdata->detail_session_info.src_port= str_convert(STRING_TYPE_INTEGER, DEFAULT_SRC_PORT, NULL);
    pdata->detail_session_info.dst_port = str_convert(STRING_TYPE_INTEGER, DEFAULT_DST_PORT, NULL);
    stomac(DEFAULT_SRC_MAC, pdata->detail_session_info.src_mac );
    stomac(DEFAULT_DST_MAC, pdata->detail_session_info.dst_mac );


    while(popts->opt)
    {
        switch(popts->opt)
        {
        case 'f':
            if( (ip1_type = str_convert(STRING_TYPE_IP, popts->optarg, (void *)&pdata->detail_session_info.src_ip)) == IP_NON_VALID )
            {
                IFX_PPACMD_PRINT("Wrong source ip:%s\n", popts->optarg);
                return PPA_CMD_ERR;
            };
            if( ip1_type == IP_VALID_V6 )
                pdata->detail_session_info.flags |= SESSION_IS_IPV6;

            f_opt ++;
            break;

        case 't':
            if( (ip2_type = str_convert(STRING_TYPE_IP, popts->optarg, (void *)&pdata->detail_session_info.dst_ip)) == IP_NON_VALID )
            {
                IFX_PPACMD_PRINT("Wrong Dst ip:%s\n", popts->optarg);
                return PPA_CMD_ERR;
            };
            if( ip2_type == IP_VALID_V6 )
                pdata->detail_session_info.flags |= SESSION_IS_IPV6;
            t_opt ++;
            break;

        case 's':
            pdata->detail_session_info.src_port=  str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL);
            s_opt ++;
            break;

        case 'd':
            pdata->detail_session_info.dst_port=  str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL);
            d_opt ++;
            break;

        case 'p':
            if( stricmp(popts->optarg, "tcp") == 0 )
            {
                pdata->detail_session_info.ip_proto= PPA_IPPROTO_TCP;
            }
            else
            {
                pdata->detail_session_info.ip_proto= PPA_IPPROTO_UDP;
            }
            p_opt ++;
            break;
        case 'i':
            pdata->detail_session_info.dest_ifid=  str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL);
            i_opt ++;
            break;
        case 'w':
            if( stricmp(popts->optarg, "wan") == 0 )
                pdata->detail_session_info.flags |= SESSION_WAN_ENTRY;
            else
                pdata->detail_session_info.flags |= SESSION_LAN_ENTRY;
            w_opt ++;
            break;
        case 'x':  //src-mac
		if (! isValidMacAddress(popts->optarg)){
			IFX_PPACMD_PRINT("Wrong mac address:%s\n", popts->optarg);
			return PPA_CMD_ERR;
		}
		stomac(popts->optarg, pdata->detail_session_info.src_mac );
		pdata->detail_session_info.flags |= SESSION_VALID_NEW_SRC_MAC;
		x_opt ++;
		break;
        case 'y': //dst_mac
		if (! isValidMacAddress(popts->optarg)){
			IFX_PPACMD_PRINT("Wrong mac address:%s\n", popts->optarg);
			return PPA_CMD_ERR;
		}
		stomac(popts->optarg, pdata->detail_session_info.dst_mac );
		y_opt ++;
		break;
        case 'n': //nat-ip
            if( (ip3_type = str_convert(STRING_TYPE_IP, popts->optarg, (void *)&pdata->detail_session_info.natip)) == IP_NON_VALID )
            {
                IFX_PPACMD_PRINT("Wrong nat ip:%s\n", popts->optarg);
                return PPA_CMD_ERR;
            };
            if( ip3_type == IP_VALID_V6 )
                pdata->detail_session_info.flags |= SESSION_IS_IPV6;
            pdata->detail_session_info.flags |= SESSION_VALID_NAT_IP;
            n_opt ++;
            break;
        case 'm': //nat-port
            pdata->detail_session_info.nat_port=  str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL);
            pdata->detail_session_info.flags |= SESSION_VALID_NAT_PORT;
            m_opt ++;
            break;
        case 'o': //new_dscp
            pdata->detail_session_info.new_dscp =  str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL);
            pdata->detail_session_info.new_dscp = SESSION_VALID_NEW_DSCP;
            o_opt ++;
            break;
        case 'r':  //in_vlan_id
            pdata->detail_session_info.in_vci_vlanid =  str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL);
            pdata->detail_session_info.flags |= SESSION_VALID_VLAN_INS;
            r_opt ++;
            break;
        case 'c':  //out_vlan_tag
            pdata->detail_session_info.out_vlan_tag =  str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL);
            pdata->detail_session_info.flags |= SESSION_VALID_OUT_VLAN_INS;
            c_opt ++;
            break;
        case 'q': //qid
            pdata->detail_session_info.qid =  str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL);
            q_opt ++;
            break;
        case 'z': //pppoe-id
            pdata->detail_session_info.pppoe_session_id =  str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL);
            pdata->detail_session_info.flags |= SESSION_VALID_PPPOE;
            z_opt ++;
            break;

        default:
            return PPA_CMD_ERR;
        }
        popts++;
    }

    if( f_opt>1|| t_opt>1|| s_opt>1||d_opt>1|| p_opt>1|| w_opt>1|| x_opt>1||
            y_opt>1|| n_opt>1||m_opt>1|| o_opt>1|| r_opt>1|| c_opt>1||q_opt>1||z_opt>1)  /*too many parameters are provided */
    {
        IFX_PPACMD_PRINT("too many same paramemter are provided\n");
        return PPA_CMD_ERR;
    }
    if( f_opt==0&& t_opt==0&& s_opt==0&&d_opt==0&& p_opt==0&& w_opt==0&& x_opt==0&&
            y_opt==0&& n_opt==0&&m_opt==0&& o_opt==0&& r_opt==0&& c_opt==0&&q_opt==0&&z_opt==0)  /*too many parameters are provided */
    {
        IFX_PPACMD_PRINT("At least provide one parameter\n");
        return PPA_CMD_ERR;
    }
    if( ip1_type != ip2_type || ( ip3_type && ip3_type != ip1_type )  )
    {
        IFX_PPACMD_PRINT("src-ip, dst-ip, nap-ip should match, ie, both are IPV4 or IPV6 address\n");
        return PPA_CMD_ERR;
    }

    if( ip1_type == IP_VALID_V6 )
    {
        IFX_PPACMD_PRINT("ppacmd not support IPV6. Pls recompile ppacmd\n");
        return PPA_CMD_ERR;
    }

    if( pdata->detail_session_info.flags & SESSION_IS_IPV6 )
    {
        IFX_PPACMD_DBG("add session via ppa session tuple:%s from %04x:%04x:%04x:%04x:%04x:%04x:%04x:%04x/%5d to %04x:%04x:%04x:%04x:%04x:%04x:%04x:%04x/%5d\n",
                (pdata->detail_session_info.ip_proto == PPA_IPPROTO_TCP)?"tcp":"udp",
                NIP6(pdata->detail_session_info.src_ip.ip6), pdata->detail_session_info.src_port,
                NIP6(pdata->detail_session_info.dst_ip.ip6), pdata->detail_session_info.dst_port);

    }
    else
    {
        IFX_PPACMD_DBG("add session via ppa session tuple:%s from %u.%u.%u.%u/%5d to %u.%u.%u.%u/%5d\n",
                (pdata->detail_session_info.ip_proto == PPA_IPPROTO_TCP)?"tcp":"udp",
                NIPQUAD(pdata->detail_session_info.src_ip), pdata->detail_session_info.src_port,
                NIPQUAD(pdata->detail_session_info.dst_ip), pdata->detail_session_info.dst_port);
    }

    return PPA_CMD_OK;

}

/**** addsession --- end */

/*
====================================================================================
   command:   modifysession
   description: modify a routing session
   options:
====================================================================================
*/
static const char ppa_modify_session_short_opts[] = "-:a:f:m:p:h";
static const struct option ppa_modify_session_long_opts[] =
{
    {"session address",   required_argument,  NULL, 'a'},
    {"session address",   required_argument,  NULL, 'f'},
    {"acceleration mode",   required_argument,  NULL, 'm'},
    {"pppoe",  required_argument,  NULL, 'p'},
    {"help",   no_argument,  NULL, 'h'},
    { 0,0,0,0 }
};

static void ppa_modify_session_help( int summary)
{
    IFX_PPACMD_PRINT("modifysession [-a ppa-session-address] [ -f 0 | 1 | 2 ] [-m 0 | 1] \n");
    if( summary )
    {
        IFX_PPACMD_PRINT("  Note 1: -f: 1 to match LAN session only, 2 to match WAN session only, and 0 match LAN/WAN both\n");
        IFX_PPACMD_PRINT("  Note 2: -m: 0 to disable acceleration for this specified session and 1 to enable acceleration\n");
        IFX_PPACMD_PRINT("  Note 3: -p: none zero id is to add or replace pppoe session id ( for test purpose only)\n");
        IFX_PPACMD_PRINT("            : otherwise no pppoe action ( for test purpose only)\n");

    }
    return;
}

static int ppa_parse_modify_session_cmd(PPA_CMD_OPTS *popts, PPA_CMD_DATA *pdata)
{
    unsigned int a_opt=0, m_opt=0, p_opt=0;
    uint32_t tmp_f = 0;

    ppa_memset( &pdata->session_extra_info, sizeof(pdata->session_extra_info), 0 );
    pdata->session_extra_info.lan_wan_flags = SESSION_WAN_ENTRY | SESSION_LAN_ENTRY;

    while(popts->opt)
    {
        switch(popts->opt)
        {
        case 'a':
            pdata->session_extra_info.session = (void *)(uintptr_t)str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL);
            a_opt ++;
            break;

        case 'm':
            pdata->session_extra_info.flags |= PPA_F_ACCEL_MODE;
            if( str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL ) ) {
                pdata->session_extra_info.session_extra.accel_enable = 1;
            } else {
                pdata->session_extra_info.session_extra.accel_enable = 0;
		pdata->session_extra_info.flags |=SESSION_NOT_ACCEL_FOR_MGM;
	    }
            m_opt ++;
            break;

        case 'f':
            tmp_f= str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL );
            if( tmp_f == 1 ) pdata->session_extra_info.lan_wan_flags = SESSION_LAN_ENTRY;
            else if( tmp_f == 2 ) pdata->session_extra_info.lan_wan_flags = SESSION_WAN_ENTRY;
            else if( tmp_f == 0 ) pdata->session_extra_info.lan_wan_flags = SESSION_WAN_ENTRY | SESSION_LAN_ENTRY;
            else
            {
                IFX_PPACMD_PRINT("Wrong flag:%d\n", tmp_f);
                return PPA_CMD_ERR;
            }
            break;
            
        case 'p':
            pdata->session_extra_info.flags |= PPA_F_PPPOE;
            pdata->session_extra_info.session_extra.pppoe_id = str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL );
            p_opt ++;
            IFX_PPACMD_DBG("pppoe_id:%d\n", pdata->session_extra_info.session_extra.pppoe_id);
            break;

        default:
            return PPA_CMD_ERR;
        }
        popts++;
    }

    if( ( a_opt != 1 ) || ( m_opt != 1 && p_opt != 1 ) )
    {
        ppa_modify_session_help(1);
        return PPA_CMD_ERR;
    }

    IFX_PPACMD_DBG("session address=0x%p\n", pdata->session_extra_info.session);
    IFX_PPACMD_DBG("Flag=0x%x\n", (unsigned int)pdata->session_extra_info.flags );
    if( pdata->session_extra_info.flags & PPA_F_ACCEL_MODE)
    {
        IFX_PPACMD_DBG("  %s\n", pdata->session_extra_info.session_extra.accel_enable ?"/s acceleration":"disable acceleration" );
    }
    if( pdata->session_extra_info.flags & PPA_F_PPPOE)
    {
        IFX_PPACMD_DBG("  %s with id=0x%d\n", pdata->session_extra_info.session_extra.pppoe_id? "new pppoe session id":"remove pppoe", pdata->session_extra_info.session_extra.pppoe_id );
    }

    return PPA_CMD_OK;
}

/*
====================================================================================
   command:   setsessiontimer
   description: set routing session polling timer
   options:
====================================================================================
*/
static const char ppa_set_session_timer_short_opts[] = "-:a:t:h";
static const struct option ppa_set_session_timer_long_opts[] =
{
    {"session address",   required_argument,  NULL, 'a'},
    {"timter",   required_argument,  NULL, 't'},
    {"help",   no_argument,  NULL, 'h'},
    { 0,0,0,0 }
};

static void ppa_set_session_timer_help( int summary)
{
    IFX_PPACMD_PRINT("setsessiontimer [-a ppa-session-address] [-t polling_timer_in_seconds] \n");
    if( summary )
    {
        IFX_PPACMD_PRINT("Note 1: by default, session address is 0, ie, to set ppa routing session polling timer only\n");
    }
    return;
}

static int ppa_parse_set_session_timer_cmd(PPA_CMD_OPTS *popts, PPA_CMD_DATA *pdata)
{
    unsigned int a_opt=0, t_opt=0;

    ppa_memset( &pdata->session_timer_info, sizeof(pdata->session_timer_info), 0 );

    while(popts->opt)
    {
        switch(popts->opt)
        {
        case 'a':
            pdata->session_timer_info.session = (void *)(uintptr_t)str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL);
            a_opt ++;
            break;

        case 't':
		if (! is_digital_value(popts->optarg)){
			IFX_PPACMD_PRINT("ppa_parse_set_session_timer_cmd : Invalid input\n");
			return PPA_CMD_ERR;
		}
		pdata->session_timer_info.timer_in_sec = str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL );
		t_opt ++;
		break;

        default:
            return PPA_CMD_ERR;
        }
        popts++;
    }

    if( t_opt !=1 )
    {
        ppa_set_session_timer_help(1);
        return PPA_CMD_ERR;
    }

    IFX_PPACMD_DBG("session address=%p\n", pdata->session_timer_info.session);
    IFX_PPACMD_DBG("timer=%d\n", (unsigned int)pdata->session_timer_info.timer_in_sec);

    return PPA_CMD_OK;
}


/*
====================================================================================
   command:   getsessiontimer
   description: get routing session polling timer
   options:
====================================================================================
*/
static const char ppa_get_session_timer_short_opts[] = "-:h";
static const struct option ppa_get_session_timer_long_opts[] =
{
    {"help",   no_argument,  NULL, 'h'},
    { 0,0,0,0 }
};

static void ppa_get_session_timer_help( int summary)
{
	IFX_PPACMD_PRINT("getsessiontimer\n");
	return;
}

static int ppa_parse_get_session_timer_cmd(PPA_CMD_OPTS *popts, PPA_CMD_DATA *pdata)
{
    ppa_memset( &pdata->session_timer_info, sizeof(pdata->session_timer_info), 0 );

	if (popts->opt)
		return PPA_CMD_ERR;

    return PPA_CMD_OK;
}


static void ppa_print_get_session_timer(PPA_CMD_DATA *pdata)
{
	IFX_PPACMD_PRINT("PPA Routing poll timer:%d\n", (unsigned int)pdata->session_timer_info.timer_in_sec);
}


/*
====================================================================================
   command:   delvfilter
   description: delete one vlan filter
   options:
====================================================================================
*/

/*delete all vfilter */
static void ppa_del_all_vfilter_help( int summary)
{
    IFX_PPACMD_PRINT("delallvfilter\n"); // [ -m <multicast-mac-address (for bridging only)>]
    if( summary )
        IFX_PPACMD_PRINT("    This command is for A4/D4 only at present\n");
    return;
}

/*
====================================================================================
   command:   getversion
   description: get ppa/ppe driver/ppe fw version
   options:
====================================================================================
*/
static void ppa_get_version_help( int summary)
{
	IFX_PPACMD_PRINT("getversion \n");
	return;
}

static void ppa_print_get_version_cmd(PPA_CMD_DATA *pdata)
{
	IFX_PPACMD_PRINT("PPA-DRV version info:v%u.%u.%u.%s\n", (unsigned int)pdata->ver.ppa_ver.major, (unsigned int)pdata->ver.ppa_ver.mid, (unsigned int)pdata->ver.ppa_ver.minor, pdata->ver.ppa_ver.tag );
}

/*get bridge mac count */
static void ppa_get_br_count_help( int summary)
{
	IFX_PPACMD_PRINT("getbrnum\n");
	return;
}

static int ppa_parse_get_br_count(PPA_CMD_OPTS *popts, PPA_CMD_DATA *pdata)
{
    int res =   ppa_parse_simple_cmd( popts, pdata );

    if( res != PPA_CMD_OK ) return res;


    pdata->count_info.flag = 0;

    return PPA_CMD_OK;
}

static void ppa_print_get_count_cmd(PPA_CMD_DATA *pdata)
{
	IFX_PPACMD_PRINT("The count is %u\n", (unsigned int)pdata->count_info.count);
}


/*get all bridge mac information */
static void ppa_get_all_br_help( int summary)
{
	IFX_PPACMD_PRINT("getbrs\n"); // [ -m <multicast-mac-address (for bridging only)>]
	return;
}

static int ppa_get_all_br_cmd (PPA_COMMAND *pcmd, PPA_CMD_DATA *pdata)
{
    PPA_CMD_ALL_MAC_INFO *psession_buffer;
    PPA_CMD_DATA cmd_info;
    int res = PPA_CMD_OK, i, size;
    uint32_t flag = PPA_CMD_GET_ALL_MAC;

    //get session count first before malloc memroy
    cmd_info.count_info.flag = 0;
    if( ppa_do_ioctl_cmd(PPA_CMD_GET_COUNT_MAC, &cmd_info ) != PPA_CMD_OK )
        return -EIO;

    if( cmd_info.count_info.count == 0 )
    {
        IFX_PPACMD_DBG("bridge mac count=0\n");
        return PPA_CMD_OK;
    }

    //malloc memory and set value correctly
    size = sizeof(PPA_CMD_COUNT_INFO) + sizeof(PPA_CMD_MAC_ENTRY) * ( 1 + cmd_info.count_info.count ) ;
    psession_buffer = (PPA_CMD_ALL_MAC_INFO *) malloc( size );
    if( psession_buffer == NULL )
    {
        IFX_PPACMD_PRINT("Malloc %d bytes failed\n", size );
        return PPA_CMD_NOT_AVAIL;
    }
    ppa_memset( psession_buffer, sizeof(size), 0 );

    psession_buffer->count_info.count = cmd_info.count_info.count;
    psession_buffer->count_info.flag = 0;

    //get session information
    if( (res = ppa_do_ioctl_cmd(flag, psession_buffer ) != PPA_CMD_OK ) )
    {
        free( psession_buffer );
        return res;
    }

    IFX_PPACMD_DBG("bridge mac count=%u. \n", (unsigned int)psession_buffer->count_info.count);


	for(i=0; i<psession_buffer->count_info.count; i++ )
	{
		//format like: [002] 239.  2.  2.  3 (route) qid  0 vlan 0000/04x From  nas0 to  eth0 ADDED_IN_HW|VALID_PPPOE|VALID_NEW_SRC_MAC
		IFX_PPACMD_PRINT("[%03d] %02x:%02x:%02x:%02x:%02x:%02x %s\n", i, psession_buffer->session_list[i].mac_addr[0],
                             psession_buffer->session_list[i].mac_addr[1],
                             psession_buffer->session_list[i].mac_addr[2],
                             psession_buffer->session_list[i].mac_addr[3],
                             psession_buffer->session_list[i].mac_addr[4],
                             psession_buffer->session_list[i].mac_addr[5],
                             psession_buffer->session_list[i].ifname );
	}
    free( psession_buffer );
    return PPA_CMD_OK;
}

/*ppacmd setbr: set bridge mac address learning hook enable/disable---begin*/
static void ppa_set_br_help( int summary)
{
    IFX_PPACMD_PRINT("setbr [-f 0/1]\n");
    return;
}
static const char ppa_set_br_short_opts[] = "-:f:h";
static const struct option ppa_set_br_long_opts[] =
{
    {"flag",   required_argument,  NULL, 'f'},
    { 0,0,0,0 }
};

static int ppa_set_br_cmd(PPA_CMD_OPTS *popts, PPA_CMD_DATA *pdata)
{
    ppa_memset( pdata, sizeof(PPA_CMD_BRIDGE_ENABLE_INFO), 0 );

#ifdef CONFIG_SOC_LGM
	IFX_PPACMD_PRINT("Not supported in URX\n");
	return PPA_CMD_ERR;
#endif

    while(popts->opt)
    {
        switch(popts->opt)
        {
        case  'f':
            pdata->br_enable_info.bridge_enable= str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL);;
            break;

        default:
            IFX_PPACMD_PRINT("ppa_set_br_cmd not support parameter -%c \n", popts->opt);
            return PPA_CMD_ERR;
        }
        popts++;
    }

    return PPA_CMD_OK;
}

/*ppacmd setbr: set bridge mac address learning hook enable/disable ---end*/

/*ppacmd getbrstatus: get bridge mac address learning hook status: enabled/disabled---begin*/
static void ppa_get_br_status_help( int summary)
{
	IFX_PPACMD_PRINT("getbrstatus\n");
    return;
}
static const char ppa_get_br_status_short_opts[] = "-:h";
static const struct option ppa_get_br_status_long_opts[] =
{
    { 0,0,0,0 }
};
static int ppa_get_br_status_cmd(PPA_CMD_OPTS *popts, PPA_CMD_DATA *pdata)
{
    ppa_memset( pdata, sizeof(PPA_CMD_BRIDGE_ENABLE_INFO), 0 );

#ifdef CONFIG_SOC_LGM
	IFX_PPACMD_PRINT("Not supported in URX\n");
	return PPA_CMD_ERR;
#endif

	if (popts->opt)
    {
		IFX_PPACMD_PRINT("ppa_get_br_status_cmd not support parameter -%c\n", popts->opt);
		return PPA_CMD_ERR;
	}

    return PPA_CMD_OK;
}
static void ppa_print_get_br_status_cmd(PPA_CMD_DATA *pdata)
{
	IFX_PPACMD_PRINT("The bridge mac learning hook is %s\n", pdata->br_enable_info.bridge_enable ? "enabled":"disabled");
}
int get_portid(char *ifname)
{
    PPA_CMD_PORTID_INFO portid;

    ppa_memset( &portid, sizeof(portid), 0);
    strncpy_s( portid.ifname,PPA_IF_NAME_SIZE, ifname,PPA_IF_NAME_SIZE);

    if( ppa_do_ioctl_cmd(PPA_CMD_GET_PORTID, &portid ) != PPA_CMD_OK )
    {
        IFX_PPACMD_PRINT("ppacmd get portid failed\n");
        return -1;
    }

    return portid.portid;
}

/*ppacmd ppa_get_br_status_cmd:   ---end*/

#ifdef CONFIG_SOC_GRX500
/*ppacmd getbrfid: get bridge Fid */
static void ppa_get_br_fid_help(int summary)
{
    IFX_PPACMD_PRINT("getbrfid <-i ifname> (NOTE: 65535 represents invalid bridge name)\n");
    return;
}
static const char ppa_get_br_fid_short_opts[] = "-:i:h";
static const struct option ppa_get_br_fid_long_opts[] =
{
    {"ifname",   required_argument,  NULL, 'i'},
    { 0,0,0,0 }
};
static int ppa_get_br_fid_cmd(PPA_CMD_OPTS *popts, PPA_CMD_DATA *pdata)
{
    unsigned int i_opt = 0;

    ppa_memset(pdata, sizeof(pdata->br_fid_info ), 0);

    while(popts->opt)
    {
        switch(popts->opt)
        {
        	case 'i':
            	strncpy_s(pdata->br_fid_info.ifname,sizeof(pdata->br_fid_info.ifname), popts->optarg, sizeof(pdata->br_fid_info.ifname) );
            	i_opt ++;
            	break;

        	default:
            	return PPA_CMD_ERR;
        }
        popts++;
    }

    if(i_opt != 1)
    {
        IFX_PPACMD_PRINT("Wrong parameter.\n");
        return PPA_CMD_ERR;
    }
    return PPA_CMD_OK;
}

static void ppa_print_get_br_fid_cmd(PPA_CMD_DATA *pdata)
{
	IFX_PPACMD_PRINT("The %s's fid is %d\n", pdata->br_fid_info.ifname, (unsigned int)pdata->br_fid_info.fid);
}

/*ppacmd getbrfid:   ---end*/
#endif /* CONFIG_SOC_GRX500 */

/*ppacmd ppa_get_iface_mib ---begin */
#if defined(PPA_IP_COUNTER) && PPA_IP_COUNTER
static void ppa_get_iface_mib_help( int summary)
{
	IFX_PPACMD_PRINT("getifacemib -i <interface name>\n");
	return;
}

static int ppa_parse_get_iface_mib_cmd(PPA_CMD_OPTS *popts, PPA_CMD_DATA *pdata)
{
	int  opt = 0;
	PPA_CMD_IFINFO *if_info;

	if_info = &(pdata->if_info);

	ppa_memset(if_info, sizeof(PPA_CMD_IFINFO), 0);
	while (popts->opt) {
		if (popts->opt == 'i') {
			opt++;
			if (strncpy_s(if_info->ifname, PPA_IF_NAME_SIZE, popts->optarg, PPA_IF_NAME_SIZE) != EOK) {
				IFX_PPACMD_DBG("Invalid interfcae name!\n");
				return PPA_CMD_ERR;
			}
		}
		popts++;
	}

	if (opt != 1)
		return PPA_CMD_ERR;

	IFX_PPACMD_DBG("PPA IFACE: [%s]\n", pdata->if_info.ifname);

	return PPA_CMD_OK;
}

static void ppa_print_get_iface_mib_cmd(PPA_CMD_DATA *pdata)
{
	int strlen = 0;
	IFX_PPACMD_PRINT("Interface:[%s]\n\n", pdata->if_info.ifname);
	IFX_PPACMD_PRINT("IPv4 RX pkts:[%llu]\tTX pkts:[%llu]\n", pdata->if_info.acc_rx_ipv4, pdata->if_info.acc_tx_ipv4);
	IFX_PPACMD_PRINT("IPv6 RX pkts:[%llu]\tTX pkts:[%llu]\n", pdata->if_info.acc_rx_ipv6, pdata->if_info.acc_tx_ipv6);

	return;
}

static int ppa_do_cmd_get_iface_mib(PPA_COMMAND *pcmd, PPA_CMD_DATA *pdata)
{
	int ret = 0;

	ret = ppa_do_ioctl_cmd(pcmd->ioctl_cmd, pdata);

	return ret;
}
#endif /* PPA_IP_COUNTER */
/****ppa_get_iface_mib ----end */

#if 0 //tc set/get for queue map
/*
===============================================================================
  Command : setQosTc
  discription:
===============================================================================
*/

static const char ppa_setQosTc_queue_map_short_opts[] = "i:p:q:t:f:h";
static const struct option ppa_setQosTc_queue_map_long_opts[] =
{
    {"ifname",  required_argument,  NULL, 'i'},
    {"portid",  required_argument,  NULL, 'p'},
    {"queue_id",   required_argument,  NULL, 'q'},
    {"tcmap",  required_argument,  NULL, 't'},
    {"flag",  required_argument,  NULL, 'f'},
    {"help",   no_argument,  NULL, 'h'},
    { 0,0,0,0 }
};

static int ppa_parse_setQosTc_queue_map_cmd(PPA_CMD_OPTS *popts,PPA_CMD_DATA *pdata)
{
	
    int p_flg=0, q_flg=0, i_flg=0;
    memset( pdata, 0, sizeof(PPA_CMD_QOS_QUEUE_INFO) );
    while(popts->opt)
    {
        switch(popts->opt)
        {
        case 'i':
    	    strcpy( pdata->qos_tc_queuemap_info.ifname, popts->optarg );
	    i_flg=1;
	    break;
        case 'p':
            pdata->qos_tc_queuemap_info.portid = str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL);
            p_flg=1;
            break;
        case 'q':
            pdata->qos_tc_queuemap_info.queue_id = str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL);
            break;
        case 't':
            pdata->qos_tc_queuemap_info.tcmap = str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL);
            break;

        case 'f':
            pdata->qos_tc_queuemap_info.flags = str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL);
            break;
	case 'h':
	    return PPA_CMD_HELP;
        default:
            IFX_PPACMD_PRINT("%s not support parameter -%c \n", __FUNCTION__, popts->opt);
            return PPA_CMD_ERR;
        }
        popts++;
    }

    if ((i_flg | p_flg) & q_flg & t_flg) { 
    	IFX_PPACMD_DBG("%s: queueid=%d tc=%d\n",  (unsigned int)pdata->qos_tc_queuemap_info.qid, (unsigned int)pdata->qos_tc_queuemap_info.tcmap);
    	return PPA_CMD_OK;
    }

    return PPA_CMD_ERR;
}


static void ppa_setQosTc_queue_map_help(int summary)
{
    IFX_PPACMD_PRINT("setQosTc {-p <port-id> -q <que-id> -t <tc-map> | -i <ifname> -q <que-id> -t <tc-map> } \
	{ --portid <port-id> --queue_id <que-id> --tcmap <tc-map> | --ifname <ifname> --queue_id <que-id> --tcmap <tc-map>} \n");
    IFX_PPACMD_PRINT("SetQosTc -h | --help");
    return;
}

/*
===============================================================================
  Command : getQosTc
  discription:
===============================================================================
*/

static const char ppa_getQosTc_queue_map_short_opts[] = "i:p:q:t:f:h";
static const struct option ppa_getQosTc_queue_map_long_opts[] =
{
    {"ifname",  required_argument,  NULL, 'i'},
    {"portid",  required_argument,  NULL, 'p'},
    {"queue_id",   required_argument,  NULL, 'q'},
    {"tcmap",  required_argument,  NULL, 't'},
    {"flag",  required_argument,  NULL, 'f'},
    {"help",   no_argument,  NULL, 'h'},
    { 0,0,0,0 }
};

static int ppa_parse_getQosTc_queue_map_cmd(PPA_CMD_OPTS *popts,PPA_CMD_DATA *pdata)
{
	
    int p_flg=0, q_flg=0, i_flg=0;
    memset( pdata, 0, sizeof(PPA_CMD_QOS_QUEUE_INFO) );
    while(popts->opt)
    {
        switch(popts->opt)
        {
        case 'i':
    	    strcpy_s(pdata->qos_tc_queuemap_info.ifname, PPA_IF_NAME_SIZE, popts->optarg );
	    i_flg=1;
	    break;
        case 'p':
            pdata->qos_tc_queuemap_info.portid = str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL);
            p_flg=1;
            break;
        case 'q':
            pdata->qos_tc_queuemap_info.queue_id = str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL);
            break;
        case 't':
            pdata->qos_tc_queuemap_info.tcmap = str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL);
            break;

        case 'f':
            pdata->qos_tc_queuemap_info.flags = str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL);
            break;
	case 'h':
	    return PPA_CMD_HELP;
        default:
            IFX_PPACMD_PRINT("%s not support parameter -%c \n", __FUNCTION__, popts->opt);
            return PPA_CMD_ERR;
        }
        popts++;
    }

    if ((i_flg | p_flg) & q_flg & t_flg) { 
    	IFX_PPACMD_DBG("%s: queueid=%d tc=%d\n",  (unsigned int)pdata->qos_tc_queuemap_info.qid, (unsigned int)pdata->qos_tc_queuemap_info.tcmap);
    	return PPA_CMD_OK;
    }

    return PPA_CMD_ERR;
}


static void ppa_setQosTc_queue_map_help(int summary)
{
    IFX_PPACMD_PRINT("setQosTc {-p <port-id> -q <que-id> -t <tc-map> | -i <ifname> -q <que-id> -t <tc-map> } \
	{ --portid <port-id> --queue_id <que-id> --tcmap <tc-map> | --ifname <ifname> --queue_id <que-id> --tcmap <tc-map>} \n");
    IFX_PPACMD_PRINT("SetQosTc -h | --help");
    return;
}

#endif

static void ppa_swap_sessions_help( int summary)
{
    IFX_PPACMD_PRINT("swapsessions\n");
    return;
}
static int ppa_swap_sessions_cmd(PPA_CMD_OPTS *popts, PPA_CMD_DATA *pdata)
{
    return PPA_CMD_OK;
}

#ifdef CONFIG_SOC_GRX500
/*
===============================================================================
  Command : addclass
===============================================================================
*/
typedef enum {
pid=256,
pid_val,
pid_ex,
subif,
subif_id,
subif_ex,
dscp,
dscp_in,
dscp_val,
dscp_ex,
vlanpcp,
cvlan,
svlan,
vlanpcp_val,
vlanpcp_ex,
pktlen,
pktlen_val,
pktlen_range,
pktlen_ex,
mac,
mac_dst,
mac_src,
mac_addr,
mac_mask,
mac_ex,
appdata,
appdata_msb,
appdata_lsb,
appdata_val,
appdata_range,
appdata_ex,
ip,
ip_dst,
ip_src,
ip_dst_in,
ip_src_in,
ip_type,
ip_addr,
ip_mask,
ip_ex,
ethertype,
type_val,
type_mask,
type_ex,
ipproto,
proto_in,
proto_val,
proto_mask,
proto_ex,
pppoe,
sess_id,
sess_ex,
ppp,
ppp_val,
ppp_mask,
ppp_ex,
vlan,
vid,
vid_range,
vid_mask,
vid_ex,
svid,
svid_val,
svid_ex,
payload,
payld1,
payld2,
payld_val,
payld_mask_range_sel,
payld_mask,
payld_ex,
parserflg,
msbflg,
lsbflg,
flg_val,
flg_mask,
flg_ex,
interface,
interface_name,
} pattern_t;

typedef enum {
filter=1,
portfilter,
crossstate,
act_vlan,
act_vid,
fid,
act_svlan,
act_svid,
crossvlan,
crossvlan_ignore,
fwd,
fwd_crossvlan,
learning,
port_trunk,
portmap,
fwd_portmap,
fwd_subifid,
rtextid_enable,
rtextid,
rtdestportmaskcmp,
rtsrcportmaskcmp,
rtdstipmaskcmp,
rtsrcipmaskcmp,
rtinneripaskey,
rtaccelenable,
rtctrlenable,
processpath,
qos,
tc,
alt_tc,
meter,
meter_id,
criticalframe,
remark,
remark_pcp,
remark_stagpcp,
remark_stagdei,
remark_dscp,
remark_class,
flowid_action,
flowid,
mgmt,
irq,
timestamp,
rmon,
rmon_id
} action_t;

static const char ppa_addclass_short_opts[] = "d:c:s:o:p:a:h";
static const struct option ppa_addclass_long_opts[] =
{
   {"dev", required_argument, NULL, 'd'},
   {"category", required_argument, NULL, 'c'},
   {"subcategory", required_argument, NULL, 's'},
   {"order", required_argument, NULL, 'o'},
   {"pattern", no_argument,NULL, 'p'},
   {"action", no_argument,NULL, 'a'},
   {"help", required_argument, NULL, 'h'},
   {"port", no_argument, NULL, pid},
   {"pid",required_argument,NULL, pid_val},
   {"pid-excl",no_argument, NULL, pid_ex},
   {"subif",no_argument,NULL, subif},
   {"subifid",required_argument,NULL, subif_id},
   {"subif-excl",no_argument,NULL,subif_ex},
   {"dscp",no_argument,NULL, dscp},
   {"dscp-inner",no_argument,NULL, dscp_in},
   {"dscp-val",required_argument,NULL, dscp_val},
   {"dscp-excl",no_argument,NULL, dscp_ex},
   {"vlanpcp",no_argument,NULL, vlanpcp},
   {"cvlan",optional_argument,NULL,cvlan},
   {"svlan",optional_argument,NULL,svlan},
   {"vlanpcp-val",required_argument,NULL,vlanpcp_val},
   {"vlanpcp-excl",no_argument,NULL,vlanpcp_ex},
   {"pktlen",no_argument,NULL,pktlen},
   {"length",required_argument,NULL, pktlen_val},
   {"pktlen-range",required_argument,NULL,pktlen_range},
   {"pktlen-excl",no_argument ,NULL,pktlen_ex},
   {"mac",no_argument,NULL,mac},
   {"mac-dst",optional_argument,NULL,mac_dst},
   {"mac-src",optional_argument,NULL,mac_src},
   {"mac-addr",required_argument,NULL,mac_addr},
   {"mac-mask",required_argument,NULL,mac_mask},
   {"mac-excl",no_argument,NULL,mac_ex},
   {"appdata",no_argument, NULL, appdata},
   {"data-msb",required_argument,NULL,appdata_msb},
   {"data-lsb",required_argument,NULL,appdata_lsb},
   {"data-val",required_argument,NULL,appdata_val},
   {"data-range",required_argument,NULL,appdata_range},
   {"data-excl",no_argument, NULL ,appdata_ex},
   {"ip",no_argument,NULL,ip},
   {"ip-dst",required_argument,NULL,ip_dst},
   {"ip-src",required_argument,NULL,ip_src},
   {"ip-inner-dst",required_argument,NULL,ip_dst_in},
   {"ip-inner-src",required_argument,NULL,ip_src_in},
   {"ip-type",required_argument,NULL,ip_type},
   {"ip-addr", required_argument, NULL, ip_addr},
   {"ip-mask",required_argument,NULL,ip_mask},
   {"ip-excl",no_argument,NULL,ip_ex},
   {"ethertype",no_argument,NULL,ethertype},
   {"type-val",required_argument,NULL,type_val},
   {"type-mask",required_argument,NULL,type_mask},
   {"type-excl",no_argument, NULL,type_ex},
   {"protocol",no_argument,NULL,ipproto},
   {"proto-inner",required_argument, NULL, proto_in},
   {"proto-val",required_argument, NULL,proto_val},
   {"proto-mask",required_argument, NULL,proto_mask},
   {"proto-excl",no_argument, NULL, proto_ex},
   {"pppoe",no_argument,NULL,pppoe},
   {"sessionid",required_argument, NULL, sess_id},
   {"pppoe-excl", no_argument, NULL, sess_ex},
   {"ppp",no_argument,NULL,ppp},
   {"ppp-val", required_argument, NULL, ppp_val},
   {"ppp-mask", required_argument, NULL, ppp_mask},
   {"ppp-excl",no_argument, NULL, ppp_ex},
   {"vlan",no_argument,NULL,vlan},
   {"vlanid",required_argument, NULL, vid},
   {"vlan-range" , required_argument, NULL ,vid_range},
   {"vlan-mask", required_argument, NULL, vid_mask},
   {"vlan-excl", no_argument, NULL,vid_ex},
   {"svlan", no_argument,NULL,svid},
   {"svlanid", required_argument, NULL, svid_val},
   {"svlan-excl", optional_argument, NULL, svid_ex},
   {"payload", no_argument,NULL, payload},
   {"payld1", required_argument, NULL, payld1},
   {"payld2", required_argument, NULL, payld2},
   {"payld-val", required_argument,NULL, payld_val},
   {"mask-range", required_argument,NULL, payld_mask_range_sel},
   {"payld-mask", required_argument, NULL, payld_mask},
   {"payld-excl", no_argument, NULL, payld_ex},
   {"parserflg", no_argument, NULL, parserflg},
   {"parser-msb", no_argument, NULL, msbflg},
   {"parser-lsb", no_argument, NULL, lsbflg},
   {"parser-val", required_argument, NULL, flg_val},
   {"parser-mask", required_argument, NULL, flg_mask},
   {"parser-excl", no_argument, NULL, flg_ex},
   {"interface", no_argument, NULL, interface},
   {"interface_name", required_argument, NULL, interface_name},
   {"filter", no_argument, NULL, filter},
   {"port_filter",required_argument, NULL, portfilter},
   {"cross_state",required_argument, NULL,crossstate},
   {"vlan-action", no_argument,NULL, act_vlan},
   {"vid", required_argument , NULL, act_vid},
   {"fid", required_argument, NULL, fid},
   {"svlan-action", required_argument, NULL, act_svlan},
   {"svid", required_argument, NULL, act_svid},
   {"crossvlan",  required_argument, NULL,crossvlan},
   {"vlan-ignore", required_argument, NULL,crossvlan_ignore},
   {"forward", no_argument, NULL,fwd},
   {"fwd-crossvlan", no_argument, NULL,fwd_crossvlan},
   {"learning", required_argument, NULL, learning},
   {"port-trunk", required_argument, NULL, port_trunk},
   {"portmap-enable",required_argument, NULL , portmap},
   {"portmap", required_argument, NULL, fwd_portmap},
   {"subifid", required_argument, NULL, fwd_subifid},
   {"routextid-enable",required_argument, NULL, rtextid_enable},
   {"routextid",required_argument, NULL, rtextid},
   {"rtdestportmaskcmp",required_argument, NULL, rtdestportmaskcmp},
   {"rtsrcportmaskcmp",required_argument, NULL,rtsrcportmaskcmp},
   {"rtdstipmaskcmp",required_argument, NULL,rtdstipmaskcmp},
   {"rtsrcipmaskcmp",required_argument, NULL,rtsrcipmaskcmp},
   {"rtinneripaskey",required_argument, NULL,rtinneripaskey},
   {"rtaccelenable", required_argument, NULL,rtaccelenable},
   {"rtctrlenable", required_argument, NULL,rtctrlenable},
   {"processpath", required_argument, NULL,processpath},
   {"qos-action", no_argument, NULL, qos},
   {"tc-action", required_argument, NULL,tc},
   {"alt-tc", required_argument, NULL,alt_tc},
   {"meter",required_argument, NULL, meter},
   {"meter-id",required_argument, NULL, meter_id},
   {"criticalframe-action",required_argument, NULL,criticalframe},
   {"remark",required_argument, NULL,remark},
   {"pcp",required_argument, NULL,remark_pcp},
   {"stagpcp",required_argument, NULL,remark_stagpcp},
   {"stagdei",required_argument, NULL,remark_stagdei},
   {"dscpremark", required_argument, NULL,remark_dscp},
   {"class",required_argument, NULL,remark_class},
   {"flowid_action",required_argument, NULL,flowid_action},
   {"flowid",required_argument, NULL,flowid},
   {"mgmt",no_argument, NULL, mgmt},
   {"irq",required_argument, NULL,irq},
   {"timestamp",required_argument, NULL,timestamp},
   {"rmon",no_argument, NULL,rmon},
   {"rmon_id",required_argument, NULL,rmon_id},
   { 0,0,0,0}
};

static int ppa_parse_pattern_cmd(PPA_CMD_OPTS **popts,PPA_CMD_DATA **pdata)
{
    printf("In function Parse_pattern\n");
    int msb_field=0,dst_ip=0,src_ip=0,dst_inner_ip=0,src_inner_ip=0,inner_dscp=0,cvlan_pcp=0,svlan_pcp=0;    
    int dst_mac=0,src_mac=0,lsb_field=0,inner_ip_prot=0,payload_1=0,payload_2=0,MSW_Flag=0,LSW_Flag=0;
    int ret=0;
	(*popts)++;
	switch((*popts)->opt)
	{
	case pid:
	    printf("In PortId\n");
	    while((*popts)->opt) {
		printf( "Inside while loop");
	    	(*popts)++;
	    	switch ((*popts)->opt)
            	{
		case pid_val://port_id
		    printf("In case pid\n");
		    (*pdata)->class_info.pattern.bPortIdEnable=1;
		    (*pdata)->class_info.pattern.nPortId = str_convert(STRING_TYPE_INTEGER,(*popts)->optarg, NULL);
		    printf("PortId value is %d\n",(*pdata)->class_info.pattern.nPortId);
		    break;
		case pid_ex://port_exclude
		    (*pdata)->class_info.pattern.bPortId_Exclude=str_convert(STRING_TYPE_INTEGER, (*popts)->optarg, NULL);
		    break;
		}
		if (((*popts)->opt =='p')|| ((*popts)->opt == 'a')) {
		   break;
		}
	    }
	break;
	case subif:
	    while((*popts)->opt) {
	    (*popts)++;
	    switch((*popts)->opt)
	    {
		case subif_id:
		    (*pdata)->class_info.pattern.bSubIfIdEnable=1;
		    (*pdata)->class_info.pattern.nSubIfId=str_convert(STRING_TYPE_INTEGER, (*popts)->optarg, NULL);
		    break;
		case subif_ex:
		    (*pdata)->class_info.pattern.bSubIfId_Exclude=str_convert(STRING_TYPE_INTEGER, (*popts)->optarg, NULL);
		    break;
	    }
	    if (((*popts)->opt =='p')|| ((*popts)->opt == 'a')) {
                break;
	    }
	    }
	    break;
	case dscp:
	    printf("In case DSCP\n");
	    while((*popts)->opt) {
	    (*popts)++;
	    switch((*popts)->opt)
	    {
		case dscp_in:
		    inner_dscp=1;
		    break;
		case dscp_val:
		    if(inner_dscp==1) {
			(*pdata)->class_info.pattern.bInner_DSCP_Enable=1;
			(*pdata)->class_info.pattern.nInnerDSCP=str_convert(STRING_TYPE_INTEGER, (*popts)->optarg, NULL);
		    }
		    else {
			printf( "In DSCP Value else part\n");
		    	(*pdata)->class_info.pattern.bDSCP_Enable=1;
		    	(*pdata)->class_info.pattern.nDSCP=str_convert(STRING_TYPE_INTEGER, (*popts)->optarg, NULL);
		    }
		    break;
		case dscp_ex:
		    if(inner_dscp==1) 
			(*pdata)->class_info.pattern.bInnerDSCP_Exclude=str_convert(STRING_TYPE_INTEGER, (*popts)->optarg, NULL);
		    else
		    	(*pdata)->class_info.pattern.bDSCP_Exclude=str_convert(STRING_TYPE_INTEGER, (*popts)->optarg,NULL);
		    break;
	    }
	    if (((*popts)->opt =='p')|| ((*popts)->opt == 'a')) {
                break;
	    }
	    }
	    break;

	case vlanpcp:
	    while((*popts)->opt) {
	    (*popts)++;
	    switch((*popts)->opt)
	    {
		case cvlan :
		    cvlan_pcp=1;
		    break;
		case svlan:
		    svlan_pcp=1;
		    break;
		case vlanpcp_val:
		    if(cvlan_pcp==1) {
		    	(*pdata)->class_info.pattern.bPCP_Enable=1;
		    	(*pdata)->class_info.pattern.nPCP=str_convert(STRING_TYPE_INTEGER, (*popts)->optarg, NULL);
		    }
		    else if (svlan_pcp==1) {
			(*pdata)->class_info.pattern.bSTAG_PCP_DEI_Enable=1;
			(*pdata)->class_info.pattern.nSTAG_PCP_DEI=str_convert(STRING_TYPE_INTEGER, (*popts)->optarg, NULL);
		    }
		    break;
		case vlanpcp_ex:
		    if(cvlan_pcp==1) 
		    	(*pdata)->class_info.pattern.bCTAG_PCP_DEI_Exclude=str_convert(STRING_TYPE_INTEGER, (*popts)->optarg, NULL);
		    else if (svlan_pcp==1) 
			(*pdata)->class_info.pattern.bSTAG_PCP_DEI_Exclude=str_convert(STRING_TYPE_INTEGER, (*popts)->optarg, NULL);
		    break;
	    }
	    if (((*popts)->opt =='p')|| ((*popts)->opt == 'a')) {
                break;
            }
	    }
	break;

	case pktlen:
	    while((*popts)->opt) {
	    (*popts)++;
	    switch((*popts)->opt)
	    {
		case pktlen_val:
		    (*pdata)->class_info.pattern.bPktLngEnable=1;
		    (*pdata)->class_info.pattern.nPktLng=str_convert(STRING_TYPE_INTEGER, (*popts)->optarg, NULL);
		    break;
		case pktlen_range:
		    (*pdata)->class_info.pattern.nPktLngRange=str_convert(STRING_TYPE_INTEGER, (*popts)->optarg, NULL);
		    break;
		case pktlen_ex:
		    (*pdata)->class_info.pattern.bPktLng_Exclude=str_convert(STRING_TYPE_INTEGER, (*popts)->optarg, NULL);
		    break;
	    }
	    if (((*popts)->opt =='p')|| ((*popts)->opt == 'a')) {
                break;
            }
	    }
	break;

	case mac:
	    while((*popts)->opt) {
	    (*popts)++;
	    switch((*popts)->opt)
	    {
		case mac_dst:
		    dst_mac=1;
		    break;
		case mac_src:
		    src_mac=1;
		    break;
		case mac_addr:
		    if(dst_mac==1) {
		    	(*pdata)->class_info.pattern.bMAC_DstEnable=1;
			stomac((*popts)->optarg,(*pdata)->class_info.pattern.nMAC_Dst);
		    }
		    else {
		    	(*pdata)->class_info.pattern.bMAC_SrcEnable=1;
		    	stomac((*popts)->optarg,(*pdata)->class_info.pattern.nMAC_Src);
		    }
		    break;
		case mac_mask:
		    if(dst_mac==1) 
			(*pdata)->class_info.pattern.nMAC_DstMask=str_convert(STRING_TYPE_INTEGER, (*popts)->optarg, NULL);
		    else 
		    	(*pdata)->class_info.pattern.nMAC_SrcMask=str_convert(STRING_TYPE_INTEGER, (*popts)->optarg, NULL);
		    break;
		case mac_ex:
		    if(dst_mac==1)
			(*pdata)->class_info.pattern.bDstMAC_Exclude=str_convert(STRING_TYPE_INTEGER, (*popts)->optarg, NULL);
		    else
		    	(*pdata)->class_info.pattern.bSrcMAC_Exclude=str_convert(STRING_TYPE_INTEGER, (*popts)->optarg, NULL);
		    break;
		default:
		    printf("Pattern invalid or not related to specific pattern\n");
		    break;
	    }
	    if (((*popts)->opt =='p')|| ((*popts)->opt == 'a')) {
                break;
            }
	    }
	break;

	case appdata:
	    while((*popts)->opt) {
	    (*popts)++;
	    switch((*popts)->opt)
	    {
		case appdata_msb:
		    msb_field=1;
		    break;
		case appdata_lsb:
		    lsb_field=1;
		    break;
		case appdata_val:
		    if(msb_field==1) {
			(*pdata)->class_info.pattern.bAppDataMSB_Enable=1;
			(*pdata)->class_info.pattern.nAppDataMSB=str_convert(STRING_TYPE_INTEGER, (*popts)->optarg, NULL);
		    }
		    else if(lsb_field==1) {
		    	(*pdata)->class_info.pattern.bAppDataLSB_Enable=1;
		    	(*pdata)->class_info.pattern.nAppDataLSB=str_convert(STRING_TYPE_INTEGER, (*popts)->optarg, NULL);
		    }
		    break;
		case appdata_range://range select
//		    msb_range_select=1;lsb_range_select=1;
		    if(msb_field==1) { 
			(*pdata)->class_info.pattern.bAppMaskRangeMSB_Select=1;
			(*pdata)->class_info.pattern.nAppMaskRangeMSB=str_convert(STRING_TYPE_INTEGER, (*popts)->optarg, NULL);	
		    }
		    else if(lsb_field==1) {
		    	(*pdata)->class_info.pattern.bAppMaskRangeLSB_Select=1;
		    	(*pdata)->class_info.pattern.nAppMaskRangeLSB=str_convert(STRING_TYPE_INTEGER, (*popts)->optarg, NULL);
		    }
		    break;
		case appdata_ex:
		    if(msb_field==1)
		    	(*pdata)->class_info.pattern.bAppMSB_Exclude=1;
		    else if (lsb_field==1)
		    	(*pdata)->class_info.pattern.bAppLSB_Exclude=1;
		    break;
	    }
	    if (((*popts)->opt =='p')|| ((*popts)->opt == 'a')) {
                break;
            }
	    }
	break;

	case ip:
	    while((*popts)->opt) {
	    (*popts)++;
	    switch((*popts)->opt)
	    {
		case ip_dst:
		    dst_ip=1;
		    break;
		case ip_src:
		    src_ip=1;
		    break;
		case ip_dst_in:
		    dst_inner_ip=1;
		    break;
		case ip_src_in:
		    src_inner_ip=1;
		    break;
		case ip_type:
		    if(dst_ip==1) 
			(*pdata)->class_info.pattern.eDstIP_Select=str_convert(STRING_TYPE_INTEGER, (*popts)->optarg, NULL);
		    
		    else if(dst_inner_ip==1) 
			(*pdata)->class_info.pattern.eInnerDstIP_Select=str_convert(STRING_TYPE_INTEGER, (*popts)->optarg, NULL);
		    
		    else if(src_inner_ip==1)
			(*pdata)->class_info.pattern.eInnerSrcIP_Select=str_convert(STRING_TYPE_INTEGER, (*popts)->optarg, NULL);
		       
		    else 
			(*pdata)->class_info.pattern.eSrcIP_Select=str_convert(STRING_TYPE_INTEGER, (*popts)->optarg, NULL);
		    break;

		case ip_addr:
		    if(dst_ip==1) {
		    	ret=str_convert(STRING_TYPE_IP, (*popts)->optarg,(void *)&(*pdata)->class_info.pattern.nDstIP);
		    	if(ret==IP_NON_VALID) {        
    			    printf("Not Valid IP\n");  
    			    break;                     
 		    	}
		    }                           
		    else if(dst_inner_ip==1) {
			ret=str_convert(STRING_TYPE_IP, (*popts)->optarg,(void *)&(*pdata)->class_info.pattern.nInnerDstIP);
			if(ret==IP_NON_VALID) {       
			    printf("Not Valid IP\n"); 
    			    break;                    
			}
		    }
		    else if(src_inner_ip==1) {
			ret=str_convert(STRING_TYPE_IP, (*popts)->optarg, (void *)&(*pdata)->class_info.pattern.nInnerSrcIP);
			if(ret==IP_NON_VALID) {
			    printf("Not Valid IP\n");
			    break;
		 	}
		    }
		    else if(src_ip==1) {
		    	ret=str_convert(STRING_TYPE_IP, (*popts)->optarg, (void *)&(*pdata)->class_info.pattern.nSrcIP);   
		    	if(ret==IP_NON_VALID) {     
			    printf("Not Valid IP\n");
			    break;
		        }
		    }
		    else
		    	break;
		
		
		case ip_mask:
		    if(dst_ip==1) 
			(*pdata)->class_info.pattern.nDstIP_Mask=str_convert(STRING_TYPE_INTEGER, (*popts)->optarg, NULL);
		    else if(dst_inner_ip==1)
			(*pdata)->class_info.pattern.nInnerDstIP_Mask=str_convert(STRING_TYPE_INTEGER, (*popts)->optarg, NULL);
		    else if(src_inner_ip==1)
			(*pdata)->class_info.pattern.nInnerSrcIP_Mask=str_convert(STRING_TYPE_INTEGER, (*popts)->optarg, NULL);
		    else
		    	(*pdata)->class_info.pattern.nSrcIP_Mask=str_convert(STRING_TYPE_INTEGER, (*popts)->optarg, NULL);
		    break;

		case ip_ex:
		    if(dst_ip==1)
			(*pdata)->class_info.pattern.bDstIP_Exclude=1;
		    else if(dst_inner_ip==1)
			(*pdata)->class_info.pattern.bInnerDstIP_Exclude=1;		
		    else if(src_inner_ip==1)
			(*pdata)->class_info.pattern.bInnerSrcIP_Exclude=1;
		    else
		        (*pdata)->class_info.pattern.bSrcIP_Exclude=1;
		    break;
		}
		if (((*popts)->opt =='p')|| ((*popts)->opt == 'a')) {
                break;
            	}
		}
	break;
		
	case ethertype:
	    while((*popts)->opt) {
  	    (*popts)++;                                
	    switch((*popts)->opt)
	    {
		case type_val:
		    (*pdata)->class_info.pattern.bEtherTypeEnable=1;
		    (*pdata)->class_info.pattern.nEtherType=str_convert(STRING_TYPE_INTEGER, (*popts)->optarg, NULL);
		    break;
		case type_mask:
		    (*pdata)->class_info.pattern.nEtherTypeMask=str_convert(STRING_TYPE_INTEGER, (*popts)->optarg, NULL);
		    break;
		case type_ex:
		    (*pdata)->class_info.pattern.bEtherType_Exclude=1;
		    break;
	    }
	    if (((*popts)->opt =='p')|| ((*popts)->opt == 'a')) {
                break;
            }
	    }
	    break;
	
	case ipproto:
	    while ((*popts)->opt) {
	    (*popts)++;
	    switch((*popts)->opt)
	    {
		case proto_in:
		    inner_ip_prot=1;
		    break;
		case proto_val:
		    if(inner_ip_prot==1) {
			(*pdata)->class_info.pattern.bInnerProtocolEnable=1;
			(*pdata)->class_info.pattern.nInnerProtocol=str_convert(STRING_TYPE_INTEGER, (*popts)->optarg, NULL);
		    }
		    else {
		    	(*pdata)->class_info.pattern.bProtocolEnable=1;
		    	(*pdata)->class_info.pattern.nProtocol=str_convert(STRING_TYPE_INTEGER, (*popts)->optarg, NULL);
		    }
		    break;
		case proto_mask:
		    if(inner_ip_prot==1) 
			(*pdata)->class_info.pattern.nInnerProtocolMask=str_convert(STRING_TYPE_INTEGER, (*popts)->optarg, NULL);
		    else
		    	(*pdata)->class_info.pattern.nProtocolMask=str_convert(STRING_TYPE_INTEGER, (*popts)->optarg, NULL);
		    break;
		case proto_ex:
		    if(inner_ip_prot==1)
			(*pdata)->class_info.pattern.bInnerProtocol_Exclude=1;
		    else
		    	(*pdata)->class_info.pattern.bProtocol_Exclude=1;
		    break;
	    }
	    if (((*popts)->opt =='p')|| ((*popts)->opt == 'a')) {
                break;
            }
	    }
	    break;
	
	
	case pppoe:
	    while((*popts)->opt) {
	    (*popts)++;
	    switch((*popts)->opt)
	    {
		case sess_id:
		    (*pdata)->class_info.pattern.bSessionIdEnable=1;
		    (*pdata)->class_info.pattern.nSessionId=str_convert(STRING_TYPE_INTEGER, (*popts)->optarg, NULL);
		    break;
		case sess_ex:
		    (*pdata)->class_info.pattern.bSessionId_Exclude=1;
		    break;
	    }
	    if (((*popts)->opt =='p')|| ((*popts)->opt == 'a')) {
                break;
            }
	    }
	    break;

	case ppp:
	    while((*popts)->opt) { 
	    (*popts)++;                               
	    switch((*popts)->opt)
	    {
		case ppp_val:
		    (*pdata)->class_info.pattern.bPPP_ProtocolEnable=1;
		    (*pdata)->class_info.pattern.nPPP_Protocol=str_convert(STRING_TYPE_INTEGER, (*popts)->optarg, NULL);
		    break;
		case ppp_mask:
		    (*pdata)->class_info.pattern.nPPP_ProtocolMask=str_convert(STRING_TYPE_INTEGER, (*popts)->optarg, NULL);
		    break;
		case ppp_ex:
		    (*pdata)->class_info.pattern.bPPP_Protocol_Exclude=1;
		    break;
	    }
	    if (((*popts)->opt =='p')|| ((*popts)->opt == 'a')) {
                break;
            }
	    }
	    break;
	case vlan:
	    while((*popts)->opt) { 
	    (*popts)++; 
	    switch((*popts)->opt)
	    {
		case vid:
		    (*pdata)->class_info.pattern.bVid=1;
		    (*pdata)->class_info.pattern.nVid=str_convert(STRING_TYPE_INTEGER, (*popts)->optarg, NULL);
		    break;
		case vid_range://range select
		    (*pdata)->class_info.pattern.bVidRange_Select=str_convert(STRING_TYPE_INTEGER, (*popts)->optarg, NULL);
		    break;
		case vid_mask://range or mask
		    (*pdata)->class_info.pattern.nVidRange=str_convert(STRING_TYPE_INTEGER, (*popts)->optarg, NULL);
		    break;
		case vid_ex:
		    (*pdata)->class_info.pattern.bVid_Exclude=1;
		    break;
	    }
	    if (((*popts)->opt =='p')|| ((*popts)->opt == 'a')) {
                break;
            }
	    }
	    break;

		    
	case svid:
	    while((*popts)->opt) {    
	    (*popts)++;                                 
	    switch((*popts)->opt)
	    {
		case svid_val:
		    (*pdata)->class_info.pattern.bSLAN_Vid=1;
		    (*pdata)->class_info.pattern.nSLAN_Vid=str_convert(STRING_TYPE_INTEGER, (*popts)->optarg, NULL);
		    break;
		case svid_ex:
		    (*pdata)->class_info.pattern.bSLANVid_Exclude=1;
		    break;
	    }
	    if (((*popts)->opt =='p')|| ((*popts)->opt == 'a')) {
                break;
            }
	    }
	    break;
	
	case payload:
	    while((*popts)->opt) {
	    (*popts)++;
	    switch((*popts)->opt)
	    {
	        case payld1:
		    payload_1=1;
		    break;
		case payld2:
		    payload_2=1;
		    break;
		case payld_val:
		    if(payload_1==1) {
			(*pdata)->class_info.pattern.bPayload1_SrcEnable=1;
			(*pdata)->class_info.pattern.nPayload1=str_convert(STRING_TYPE_INTEGER, (*popts)->optarg, NULL);
			
		    }
		    else {
		    	(*pdata)->class_info.pattern.bPayload2_SrcEnable=1;
		   	(*pdata)->class_info.pattern.nPayload2=str_convert(STRING_TYPE_INTEGER, (*popts)->optarg, NULL);
		    }
		    break;
		case payld_mask_range_sel:
		    if(payload_1==1) {
			(*pdata)->class_info.pattern.bPayload1MaskRange_Select=str_convert(STRING_TYPE_INTEGER, (*popts)->optarg, NULL);
		    }
		    else
		        (*pdata)->class_info.pattern.bPayload2MaskRange_Select=str_convert(STRING_TYPE_INTEGER, (*popts)->optarg, NULL);
		    break;
		case payld_mask:
		    if(payload_1==1) {
			(*pdata)->class_info.pattern.nPayload1_Mask=str_convert(STRING_TYPE_INTEGER, (*popts)->optarg, NULL);
		    }
		    else
		        (*pdata)->class_info.pattern.nPayload2_Mask=str_convert(STRING_TYPE_INTEGER, (*popts)->optarg, NULL);
		    break;
		case payld_ex:
		    if(payload_1==1) 
			(*pdata)->class_info.pattern.bPayload1_Exclude=1;
		    else
		    	(*pdata)->class_info.pattern.bPayload2_Exclude=1;
		    break;
	    }
	    if (((*popts)->opt =='p')|| ((*popts)->opt == 'a')) {
                break;
            }
	    }
	    break;

	case parserflg:
	    while((*popts)->opt) {
	    (*popts)++;
	    switch((*popts)->opt)
	    {
	    	case msbflg:
		    MSW_Flag=1;
		    break;
		case lsbflg:
		    LSW_Flag=1;
		    break;
		case flg_val:
		    if(MSW_Flag==1) {
		    	(*pdata)->class_info.pattern.bParserFlagMSB_Enable=1;
		    	(*pdata)->class_info.pattern.nParserFlagMSB=str_convert(STRING_TYPE_INTEGER, (*popts)->optarg, NULL);
		    }
		    else {
			(*pdata)->class_info.pattern.bParserFlagLSB_Enable=1;
			(*pdata)->class_info.pattern.nParserFlagLSB=str_convert(STRING_TYPE_INTEGER, (*popts)->optarg, NULL);  
		    }
		    break;
		case flg_mask:
		    if(MSW_Flag==1) 
		    	(*pdata)->class_info.pattern.nParserFlagMSB_Mask=str_convert(STRING_TYPE_INTEGER, (*popts)->optarg, NULL);
		    else
			(*pdata)->class_info.pattern.nParserFlagLSB_Mask=str_convert(STRING_TYPE_INTEGER, (*popts)->optarg, NULL);
		    break;
		case flg_ex:
		    if(MSW_Flag==1)
		    	(*pdata)->class_info.pattern.bParserFlagMSB_Exclude=1;
		    else
			(*pdata)->class_info.pattern.bParserFlagLSB_Exclude=1;
		    break;
	    }
	    if (((*popts)->opt =='p')||( (*popts)->opt == 'a')) {
                break;
            }
	    }
		break;
	case interface:
		while((*popts)->opt) {
		(*popts)++;
		switch((*popts)->opt)
			{
				case interface_name:
					ret = strncpy_s((*pdata)->class_info.rx_if, PPA_IF_NAME_SIZE, (*popts)->optarg, PPA_IF_NAME_SIZE);
					if (ret != PPA_CMD_OK) {
						printf("ERROR: Invalid interface name\n");
						return ret;
					}
					break;
			}
			if (((*popts)->opt =='p')|| ((*popts)->opt == 'a')) {
				break;
			}
		}
		break;
	default:
		printf("Deafult Argument\n");
	
			
    } 
return 0;
}

static int ppa_parse_action_cmd(PPA_CMD_OPTS **popts,PPA_CMD_DATA **pdata)
{
	printf("In action \n");
	(*popts)++;
	switch((*popts)->opt)
	{
	case filter:
	    printf("In case filter\n");
	    while ((*popts)->opt) {
	    (*popts)++;
	    switch((*popts)->opt)
	    {
	    case portfilter:
		printf("in case port_filter\n");
		(*pdata)->class_info.action.filter_action.portfilter=str_convert(STRING_TYPE_INTEGER, (*popts)->optarg, NULL);
	 	break;
	    case crossstate:
		printf("In case cross_state\n");
		(*pdata)->class_info.action.filter_action.crossstate=str_convert(STRING_TYPE_INTEGER, (*popts)->optarg, NULL);
		break;
	
	    }
	    if (((*popts)->opt =='p')|| ((*popts)->opt == 'a')) {
                break;
            }
	    }
	break;
	case act_vlan:
	    while ((*popts)->opt) {
	    (*popts)++; 
	    switch((*popts)->opt)
	    {
	    case act_vid:
		printf("In case act_vid\n");
		(*pdata)->class_info.action.vlan_action.vlan_id=str_convert(STRING_TYPE_INTEGER, (*popts)->optarg, NULL);
		break;
	    case fid:
		(*pdata)->class_info.action.vlan_action.fid=str_convert(STRING_TYPE_INTEGER, (*popts)->optarg, NULL);
		break;
	    case act_svlan:
		(*pdata)->class_info.action.vlan_action.cvlan=str_convert(STRING_TYPE_INTEGER, (*popts)->optarg, NULL);
		break;
	    case act_svid:
		(*pdata)->class_info.action.vlan_action.svlan=str_convert(STRING_TYPE_INTEGER, (*popts)->optarg, NULL);
		break;
	    case crossvlan:
		(*pdata)->class_info.action.vlan_action.cross_vlan=str_convert(STRING_TYPE_INTEGER, (*popts)->optarg, NULL);
		break;
	    case crossvlan_ignore:
		(*pdata)->class_info.action.vlan_action.cvlan_ignore=1;
		break;
	    }
	    if (((*popts)->opt =='p' )|| ((*popts)->opt == 'a')) {
                break;
            }
	    }
	    break;
	case fwd:
	    while ((*popts)->opt) { 
	    (*popts)++;
	    switch((*popts)->opt)
	    {
	    case learning:
		(*pdata)->class_info.action.fwd_action.learning=str_convert(STRING_TYPE_INTEGER, (*popts)->optarg, NULL);
		break;
	    case port_trunk:
		(*pdata)->class_info.action.fwd_action.port_trunk=1;
		break;
	    case portmap:
		(*pdata)->class_info.action.fwd_action.portmap=str_convert(STRING_TYPE_INTEGER, (*popts)->optarg, NULL);
		break;
	    case fwd_portmap:
		(*pdata)->class_info.action.fwd_action.forward_portmap=str_convert(STRING_TYPE_INTEGER, (*popts)->optarg, NULL);
		break;
	    case fwd_subifid:
		 (*pdata)->class_info.action.fwd_action.forward_subifid=str_convert(STRING_TYPE_INTEGER, (*popts)->optarg, NULL);
		break;
	    case rtextid_enable:
		(*pdata)->class_info.action.fwd_action.routextid_enable=1;
		(*pdata)->class_info.action.fwd_action.routextid=str_convert(STRING_TYPE_INTEGER, (*popts)->optarg, NULL);
		break;
	    case rtdestportmaskcmp:
		(*pdata)->class_info.action.fwd_action.rtdestportmaskcmp=str_convert(STRING_TYPE_INTEGER, (*popts)->optarg, NULL);
		break;
	    case rtsrcportmaskcmp:
		(*pdata)->class_info.action.fwd_action.rtsrcportmaskcmp=str_convert(STRING_TYPE_INTEGER, (*popts)->optarg, NULL);
		break;
	    case rtdstipmaskcmp:
		(*pdata)->class_info.action.fwd_action.rtdstipmaskcmp=str_convert(STRING_TYPE_INTEGER, (*popts)->optarg, NULL);
		break;
	    case rtsrcipmaskcmp:
		(*pdata)->class_info.action.fwd_action.rtsrcipmaskcmp=str_convert(STRING_TYPE_INTEGER, (*popts)->optarg, NULL);
		break;
	    case rtinneripaskey:
		(*pdata)->class_info.action.fwd_action.rtinneripaskey=str_convert(STRING_TYPE_INTEGER, (*popts)->optarg, NULL);
		break;
	    case rtaccelenable:
		(*pdata)->class_info.action.fwd_action.rtaccelenable=str_convert(STRING_TYPE_INTEGER, (*popts)->optarg, NULL);
		break;
	    case rtctrlenable:
		(*pdata)->class_info.action.fwd_action.rtctrlenable=str_convert(STRING_TYPE_INTEGER, (*popts)->optarg, NULL);
		break;
	    case processpath:
		(*pdata)->class_info.action.fwd_action.processpath=str_convert(STRING_TYPE_INTEGER, (*popts)->optarg, NULL);
		break;
	    }
	    if (((*popts)->opt =='p')|| ((*popts)->opt == 'a')) {
                break;
            }
	    }
	break;
	case qos:
	    while ((*popts)->opt) { 
	    (*popts)++;
	    switch((*popts)->opt)
	    {
	    case tc:	
		(*pdata)->class_info.action.qos_action.trafficclass=str_convert(STRING_TYPE_INTEGER, (*popts)->optarg, NULL);
		break;
	    case alt_tc:
		(*pdata)->class_info.action.qos_action.alt_trafficclass=str_convert(STRING_TYPE_INTEGER, (*popts)->optarg, NULL);
		break;
	    case meter:
		(*pdata)->class_info.action.qos_action.meter=str_convert(STRING_TYPE_INTEGER, (*popts)->optarg, NULL);
		break;
	    case meter_id:
		(*pdata)->class_info.action.qos_action.meterid=str_convert(STRING_TYPE_INTEGER, (*popts)->optarg, NULL);
		break;
	    case criticalframe:
		(*pdata)->class_info.action.qos_action.criticalframe=str_convert(STRING_TYPE_INTEGER, (*popts)->optarg, NULL);
		break;
	    case remark:
		(*pdata)->class_info.action.qos_action.remark=str_convert(STRING_TYPE_INTEGER, (*popts)->optarg, NULL);
		break;
	    case remark_pcp:
		(*pdata)->class_info.action.qos_action.remarkpcp=str_convert(STRING_TYPE_INTEGER, (*popts)->optarg, NULL);
		break;
	    case remark_stagpcp:
		 (*pdata)->class_info.action.qos_action.remark_stagpcp=str_convert(STRING_TYPE_INTEGER, (*popts)->optarg, NULL);
		break;
	    case remark_stagdei:
		(*pdata)->class_info.action.qos_action.remark_stagdei=str_convert(STRING_TYPE_INTEGER, (*popts)->optarg, NULL);
		break;
	    case remark_dscp:
		(*pdata)->class_info.action.qos_action.remark_dscp=str_convert(STRING_TYPE_INTEGER, (*popts)->optarg, NULL);
		break;
	    case remark_class:
		(*pdata)->class_info.action.qos_action.remark_class=str_convert(STRING_TYPE_INTEGER, (*popts)->optarg, NULL);
		break;
	    case flowid_action:
		(*pdata)->class_info.action.qos_action.flowid_enabled=1;
		(*pdata)->class_info.action.qos_action.flowid=str_convert(STRING_TYPE_INTEGER, (*popts)->optarg, NULL);
		break;
	    }
	    if (((*popts)->opt =='p')|| ((*popts)->opt == 'a')) {
                break;
            }
	    }
	break;

	case mgmt:
	    while ((*popts)->opt) {
            (*popts)++;
	    switch((*popts)->opt)
	    {
	    case irq:
		(*pdata)->class_info.action.mgmt_action.irq=str_convert(STRING_TYPE_INTEGER, (*popts)->optarg, NULL);
		break;
	    case timestamp:
		(*pdata)->class_info.action.mgmt_action.timestamp=str_convert(STRING_TYPE_INTEGER, (*popts)->optarg, NULL);
		break;
	    }
	    if (((*popts)->opt =='p')|| ((*popts)->opt == 'a')) {
                break;
            }
	    }
	break;
	case rmon:
	    while ((*popts)->opt) {
            (*popts)++;
            switch((*popts)->opt)
            {
	    case rmon_id:
		(*pdata)->class_info.action.rmon_action=1;
	 	(*pdata)->class_info.action.rmon_id=str_convert(STRING_TYPE_INTEGER, (*popts)->optarg, NULL);
		break;
	    }
	   if ((*popts)->opt =='p'|| (*popts)->opt == 'a') {
                break;
           }
	   }
	break;
	default:
	    IFX_PPACMD_PRINT("ppa_parse_add_class_cmd not support parameter -%c \n",(*popts)->opt);
            return PPA_CMD_ERR;	    
	}
	
return 0;
}
static int ppa_parse_addclass_cmd(PPA_CMD_OPTS *popts,PPA_CMD_DATA *pdata)
{
    uint32_t ret;
    ppa_memset( pdata, sizeof(PPA_CMD_CLASSIFIER_INFO), 0 );
    while(popts->opt)
    {
	switch(popts->opt)
	{
	case 'd':
		/* (default) 0 - GSWIP-R (dev=1)
		             1 - GSWIP-L (dev=0)  */
		ret = str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL);
		if (ret == PPA_CMD_ERR){
			IFX_PPACMD_PRINT("ERROR: invalid input\n");
			return ret;
		}
		pdata->class_info.in_dev=ret;
		break;
	case 'c':
		printf("in case 'c'\n");
		ret = str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL);
		if (ret == PPA_CMD_ERR){
			IFX_PPACMD_PRINT("ERROR: invalid input\n");
			return ret;
		}
		pdata->class_info.category=ret;
		printf("Category _id is %d\n",pdata->class_info.category);
		break;
	case 's':
		printf("in case 's'\n");
		ret = str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL);
		if (ret == PPA_CMD_ERR){
			IFX_PPACMD_PRINT("ERROR: invalid input\n");
			return ret;
		}
		pdata->class_info.subcategory=ret;
		break;
	case 'o' :
		printf("in case 'o'\n");
		ret = str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL);
		if (ret == PPA_CMD_ERR){
			IFX_PPACMD_PRINT("ERROR: invalid input\n");
			return ret;
		}
		pdata->class_info.order=ret;
		break;
	case 'p' :
	    printf( "In case 'p'\n");
	    ret =  ppa_parse_pattern_cmd(&popts,&pdata);
	    if (ret==0) {
			/* Enable pattern if parsing is successfull */
	    	pdata->class_info.pattern.bEnable = 1;
			popts--;
	    	break;
	    }
	case 'a':
	    ret= ppa_parse_action_cmd(&popts,&pdata);
	    if (ret==0) {
                popts--;
                break;
            }
	    else {
                printf( "Error\n");
                return PPA_CMD_ERR;
            }
	case 'h':
	    return PPA_CMD_HELP;
	    break;
	default:
	    IFX_PPACMD_PRINT("ppa_parse_add_class_cmd does not support parameter -%c \n",popts->opt);
            return PPA_CMD_ERR;
	}
	popts++;
    }
return PPA_CMD_OK;
	  
}
static void ppa_addclass_help(int summary)
{
    IFX_PPACMD_PRINT("addclass {-d <--device> -c <--category> -s <--subcategory> -o <--order> -p [--pattern] -a [--action>]\n");
    IFX_PPACMD_PRINT("pattern : 		parameters \n");
    IFX_PPACMD_PRINT("<--port> : <--pid  |  --pid-excl>\n<--subif> : <--subifid  |  --sifid-excl>\n<--dscp>  : <--dscp-inner  |  --dscp-val  |  --dscp-excl>\n<--vlanpcp> : <--cvlan>  |  --svlan  |  --vlanpcp-val  |  --vlanpcp-excl>\n<--pktlen> : <--length  |  --pktlen-range  |  --pktlen-excl>\n<--mac>: <--mac-dst  |  --mac-src  |  --mac-addr  |  --mac-mask  |  --mac_excl>\n<--appdata> : <--data-msb  | --data-lsb  |  --data-val  |  --data-range  |  --data-excl>\n<--ip>   :<--ip-dst  |  --ip-src  |  --ip-inner-dst  |  --ip-inner-src  |  --ip-type  |  --ip-addr  |  --ip-mask  |  --ip-excl>\n<--ethertype> : <--type-val  |  --type-mask  |  --type-excl>\n<--protocol> : <--proto-inner  |  --proto-val  |  --proto-mask  |  --proto-excl>\n<--pppoe>: <--sessionid  |  --pppoe-excl>\n<--ppp>: <--ppp-val  |  --ppp-mask  |  --ppp-excl>\n<--vlan>: <--vlanid  |  --vlan-range  |  --vlan-mask  |  --vlan-excl>\n<--svlan>:<--svlanid  |  --svlan-excl>\n<--payload>:<--payld1  |  --payld2  |  --payld-val  |  --mask-range  |  --payld-mask  |  --payld-excl>\n<--parserflg>: <--parser-msb  |  --parser-lsb  |  --parser-val  |  --parser-mask  |  --parser-excl>\n<--interface>: <--interface_name>\n");
   IFX_PPACMD_PRINT("action: \t parameters \n");
   IFX_PPACMD_PRINT("<--filter> : <--port_filter  |  --cross_state>\n<--vlan-action> : <--vid  | --fid  |  --svlan-action  |  --svid  |  --crossvlan  |  --vlan-ignore>\n<--forward> : <--fwd-crossvlan   |  --learning  |  --port-trunk  |  --portmap-enable  |  --portmap  |  --subifid  |  --routextid-enable  |  --routextid  |  --rtdestportmaskcmp  |  --rtsrcportmaskcmp  |  --rtdstipmaskcmp  |  --rtsrcipmaskcmp  |  -- rtinneripaskey  -- rtaccelenable  --rtctrlenable  --processpath>\n<--qos-action> : <--tc-action  |   --alt-tc  |  --meter  |  --meter-id  |  --criticalframe-action  |  --remark  |  --pcp  |  --stagpcp  |  --stagdei  |  --dscpremark  |  --class  |  --flowid_action  |  --flowid>\n<--mgmt> : <--irq  |  --timestamp>\n<--rmon> : <--rmon_id>");
    return;
}

static void ppa_print_addclass_cmd(PPA_CMD_DATA *pdata)
{
#define MAX_DUMP_STRLEN 256
	int category = pdata->class_info.category;
	char dumpStr[MAX_DUMP_STRLEN] = {'\0'};

	/* Dumping data based on category */
	switch(category) {
		case CAT_FWD:
			snprintf(dumpStr, MAX_DUMP_STRLEN, "Rule Parameters: device: %s catgeory: %d sub-catgeory: %d"
				" order: %d PCE rule idx: %d", pdata->class_info.in_dev ? "GSWIP-L" : "GSWIP-R",
				pdata->class_info.category, pdata->class_info.subcategory, pdata->class_info.order,
				pdata->class_info.pattern.nIndex);
			break;
		/* Future categories */
 		defualt:
			break;
	}
	IFX_PPACMD_PRINT("%s\n", dumpStr);
#undef MAX_DUMP_STRLEN
	return;
}

/*
===============================================================================
  Command : modclass
===============================================================================
*/
static const char ppa_modclass_short_opts[]= "c:s:o:p:a";
static const struct option ppa_modclass_long_opts[] =
{
   {"categoryid", required_argument, NULL, 'c'},
   {"subcategoryid", optional_argument, NULL, 's'},
   {"orderno", required_argument, NULL, 'o'},
   {"pattern",required_argument,NULL, 'p'},
   {"action",required_argument,NULL, 'a'},
   {"help",no_argument, NULL, 'h'},
   { 0,0,0,0}
};
static int ppa_parse_modclass_cmd(PPA_CMD_OPTS *popts,PPA_CMD_DATA *pdata)
{
    ppa_memset( pdata, sizeof(PPA_CMD_CLASSIFIER_INFO),0 );
    while(popts->opt)
    {
        switch(popts->opt)
        {
	case 'd':
	    pdata->class_info.in_dev=str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL);
	    break;
        case 'c':
            pdata->class_info.category=str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL);
            break;
        case 's':
            pdata->class_info.subcategory=str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL);
            break;
        case 'o' :
            pdata->class_info.order=str_convert(STRING_TYPE_INTEGER, popts->optarg,NULL);
            break;
	case 'p' :
            if ((ppa_parse_pattern_cmd(&popts,&pdata))== PPA_CMD_OK)
            	break;
	    else
		return PPA_CMD_ERR;
        case 'a':
            if ((ppa_parse_action_cmd(&popts,&pdata))==PPA_CMD_OK)
            	break;
	    else
		return PPA_CMD_ERR;
        case 'h':
            return PPA_CMD_HELP;
            break;
        default:
            IFX_PPACMD_PRINT("ppa_parse_add_class_cmd not support parameter -%c \n",popts->opt);
            return PPA_CMD_ERR;
        }
        popts++;
    }

return PPA_CMD_OK;
}

static void ppa_modclass_help(int summary)
{
    IFX_PPACMD_PRINT("modclass {-c <category-id> -s <subcategory-id> -o <order-no> -p <pattern> -a <action>\n");
    IFX_PPACMD_PRINT("modclass -h | --help");
    return;
}

/*
===============================================================================
  Command : delclass
===============================================================================
*/
static const char ppa_delclass_short_opts[] = "d:c:s:o:h";
static const struct option ppa_delclass_long_opts[] =
{
   {"dev", required_argument, NULL, 'd'},
   {"categoryid", required_argument, NULL, 'c'},
   {"subcategoryid", required_argument, NULL, 's'},
   {"orderno", required_argument, NULL, 'o'},
   {"help",required_argument, NULL, 'h'},
   { 0,0,0,0}
};
static int ppa_parse_delclass_cmd(PPA_CMD_OPTS *popts,PPA_CMD_DATA *pdata)
{
    ppa_memset( pdata, sizeof(PPA_CMD_CLASSIFIER_INFO),0 );
    while(popts->opt)
    {
        switch(popts->opt)
        {
        case 'd':
            pdata->class_info.in_dev=str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL);
            break;
        case 'c':
            pdata->class_info.category=str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL);
            break;
        case 's':
            pdata->class_info.subcategory=str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL);
            break;
        case 'o' :
            pdata->class_info.order=str_convert(STRING_TYPE_INTEGER, popts->optarg,NULL);
            break;
	case 'h':
	    return PPA_CMD_HELP;
	default:
	    break;
	    
        }
    popts++;
    }

	return PPA_CMD_OK;
}
static void ppa_delclass_help(int summary)
{
    IFX_PPACMD_PRINT("delclass {-c <--category> -s <--subcategory> -o <--order> \n");
    IFX_PPACMD_PRINT("delclass -h | --help");
    return;
}
#endif //PPA classification

/*
===============================================================================
  Command : modque
  discription: Modify QOS Sub interface to Port Configuration 
===============================================================================
*/

static const char ppa_mod_queue_short_opts[] = "q:i:w:p:l:h";
static const struct option ppa_mod_queue_long_opts[] =
{
    {"queueid",   required_argument,  NULL, 'q'},
    {"ifname",   required_argument,  NULL, 'i'},
    {"weight",   required_argument,  NULL, 'w'},
    {"priority",   required_argument,  NULL, 'p'},
    {"qlen",   required_argument,  NULL, 'l'},
    {"help",   no_argument,  NULL, 'h'},
    { 0,0,0,0 }
};

static int ppa_parse_mod_queue_cmd(PPA_CMD_OPTS *popts,PPA_CMD_DATA *pdata)
{
	
    ppa_memset( pdata, sizeof(PPA_CMD_QOS_QUEUE_INFO), 0 );

    while(popts->opt)
    {
        switch(popts->opt)
        {
        case 'q':
            pdata->qos_queue_info.queue_num = str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL);
            break;
        case 'p':
            pdata->qos_queue_info.priority = str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL);
            break;
        case 'i':
    	    strcpy_s( pdata->qos_queue_info.ifname,PPA_IF_NAME_SIZE,popts->optarg);
            break;
        case 'w':
            pdata->qos_queue_info.weight = str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL);
            break;
        case 'l':
            pdata->qos_queue_info.qlen = str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL);
            break;
	case 'h':
	    return PPA_CMD_HELP;
        default:
            IFX_PPACMD_PRINT("ppa_parse_mod_queue_cmd not support parameter -%c \n", popts->opt);
            return PPA_CMD_ERR;
        }
        popts++;
    }

    IFX_PPACMD_DBG("ppa_parse_mod_queue_cmd: interface=%s\n", pdata->qos_queue_info.ifname);

    return PPA_CMD_OK;
}

static void ppa_mod_queue_help(int summary)
{ // need to write once conformed
    IFX_PPACMD_PRINT("modque -q <que-id> -i <ifname> -w <weight> -l <priority>\n");
    IFX_PPACMD_PRINT("modque -h | --help\n");
    return;
}


/*
===============================================================================
  Command definitions
===============================================================================
*/

static PPA_COMMAND ppa_cmd[] =
{
    {
        "---PPA Initialization/Status commands",
        PPA_CMD_INIT,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },

    {
        "init",
        PPA_CMD_INIT,
        ppa_print_init_help,
        ppa_parse_init_cmd,
        ppa_do_cmd,
        ppa_print_init_fake_cmd,
        ppa_init_long_opts,
        ppa_init_short_opts
    },
    {
        "exit",
        PPA_CMD_EXIT,
        ppa_print_exit_help,
        ppa_parse_exit_cmd,
        ppa_do_cmd,
        NULL,
        ppa_no_long_opts,
        ppa_no_short_opts
    },
    {
        "control",
        PPA_CMD_ENABLE,
        ppa_print_control_help,
        ppa_parse_control_cmd,
        ppa_do_cmd,
        NULL,
        ppa_control_long_opts,
        ppa_no_short_opts
    },
    {
        "status",
        PPA_CMD_GET_STATUS,
        ppa_print_status_help,
        ppa_parse_status_cmd,
        ppa_do_cmd,
        ppa_print_status,
        ppa_no_long_opts,
        ppa_output_short_opts
    },
    {
        "getversion",
        PPA_CMD_GET_VERSION,
        ppa_get_version_help,
        ppa_parse_simple_cmd,
        ppa_do_cmd,
        ppa_print_get_version_cmd,
        ppa_get_simple_long_opts,
        ppa_get_simple_short_opts
    },

    {
        "---PPA LAN/WAN Interface control commands",
        PPA_CMD_INIT,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    {
        "addwan",
        PPA_CMD_ADD_WAN_IF,
        ppa_print_add_wan_help,
        ppa_parse_add_del_if_cmd,
        ppa_do_add_del_if_cmd,
        NULL,
        ppa_if_long_opts,
        ppa_if_short_opts
    },
    {
        "addlan",
        PPA_CMD_ADD_LAN_IF,
        ppa_print_add_lan_help,
        ppa_parse_add_del_if_cmd,
        ppa_do_add_del_if_cmd,
        NULL,
        ppa_if_long_opts,
        ppa_if_short_opts
    },
    {
        "delwan",
        PPA_CMD_DEL_WAN_IF,
        ppa_print_del_wan_help,
        ppa_parse_add_del_if_cmd,
        ppa_do_add_del_if_cmd,
        NULL,
        ppa_if_long_opts,
        ppa_if_short_opts
    },
    {
        "dellan",
        PPA_CMD_DEL_LAN_IF,
        ppa_print_del_lan_help,
        ppa_parse_add_del_if_cmd,
        ppa_do_add_del_if_cmd,
        NULL,
        ppa_if_long_opts,
        ppa_if_short_opts
    },
    {
        "getwan",
        PPA_CMD_GET_WAN_IF,
        ppa_print_get_wan_help,
        ppa_parse_simple_cmd,
        ppa_get_wan_netif_cmd,
        NULL,
        ppa_get_simple_long_opts,
        ppa_get_simple_short_opts
    },
    {
        "getlan",
        PPA_CMD_GET_LAN_IF,
        ppa_print_get_lan_help,
        ppa_parse_simple_cmd,
        ppa_get_lan_netif_cmd,
        NULL,
        ppa_get_simple_long_opts,
        ppa_get_simple_short_opts
    },

    {
        "---PPA bridging related commands(For A4/D4/E4 Firmware and A5 Firmware in DSL WAN mode)",
        PPA_CMD_INIT,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    {
        "addbr",
        PPA_CMD_ADD_MAC_ENTRY,
        ppa_add_mac_entry_help,
        ppa_parse_add_mac_entry_cmd,
        ppa_do_cmd,
        NULL,
        ppa_if_mac_long_opts,
        ppa_if_mac_short_opts
    },
    {
        "delbr",
        PPA_CMD_DEL_MAC_ENTRY,
        ppa_del_mac_entry_help,
        ppa_parse_del_mac_entry_cmd,
        ppa_do_cmd,
        NULL,
        ppa_mac_long_opts,
        ppa_mac_short_opts
    },
    {
        "getbrnum",
        PPA_CMD_GET_COUNT_MAC,
        ppa_get_br_count_help,
        ppa_parse_get_br_count,
        ppa_do_cmd,
        ppa_print_get_count_cmd,
        ppa_get_simple_long_opts,
        ppa_get_simple_short_opts
    },
    {
        "getbrs",
        PPA_CMD_GET_ALL_MAC,
        ppa_get_all_br_help,
        ppa_parse_simple_cmd,
        ppa_get_all_br_cmd,
        NULL,
        ppa_get_simple_long_opts,
        ppa_get_simple_short_opts
    },
    {
        "setbr",  // to enable/disable bridge mac learning hook
        PPA_CMD_BRIDGE_ENABLE,
        ppa_set_br_help,
        ppa_set_br_cmd,
        ppa_do_cmd,
        NULL,
        ppa_set_br_long_opts,
        ppa_set_br_short_opts,
    },
    {
        "getbrstatus",  //get bridge mac learning hook status: enabled or disabled
        PPA_CMD_GET_BRIDGE_STATUS,
        ppa_get_br_status_help,
        ppa_get_br_status_cmd,
        ppa_do_cmd,
        ppa_print_get_br_status_cmd,
        ppa_get_br_status_long_opts,
        ppa_get_br_status_short_opts,
    },
#ifdef CONFIG_SOC_GRX500
    {
        "getbrfid",  //get bridge fid
        PPA_CMD_GET_BRIDGE_FID,
        ppa_get_br_fid_help,
        ppa_get_br_fid_cmd,
        ppa_do_cmd,
        ppa_print_get_br_fid_cmd,
        ppa_get_br_fid_long_opts,
        ppa_get_br_fid_short_opts,
    },
#endif

    {
        "---PPA unicast routing acceleration related commands",
        PPA_CMD_INIT,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    {
        "getlansessionnum",
        PPA_CMD_GET_COUNT_LAN_SESSION,
        ppa_get_lan_session_count_help,
        ppa_parse_get_lan_session_count,
        ppa_do_cmd,
        ppa_print_get_count_cmd,
        NULL,
        ppa_get_session_count_short_opts
    },
    {
        "getlansessions",
        PPA_CMD_GET_LAN_SESSIONS,
        ppa_get_lan_sessions_help,
        ppa_parse_get_session_cmd,
        ppa_get_lan_sessions_cmd,
        NULL,
        ppa_no_long_opts,
        ppa_get_session_short_opts
    },
    {
        "getwansessionnum",
        PPA_CMD_GET_COUNT_WAN_SESSION,
        ppa_get_wan_session_count_help,
        ppa_parse_get_wan_session_count,
        ppa_do_cmd,
        ppa_print_get_count_cmd,
        NULL,
        ppa_get_session_count_short_opts
    },
    {
        "getwansessions",
        PPA_CMD_GET_WAN_SESSIONS,
        ppa_get_wan_sessions_help,
        ppa_parse_get_session_cmd,
        ppa_get_wan_sessions_cmd,
        NULL,
        ppa_no_long_opts,
        ppa_get_session_short_opts
    },
    {
        "addsession",  // get the dsl mib
        PPA_CMD_ADD_SESSION,
        ppa_add_session_help,
        ppa_parse_add_session_cmd,
        ppa_do_cmd,
        NULL,
        ppa_add_session_long_opts,
        ppa_add_session_short_opts
    },
    {
        "delsession",  // delet the session
        PPA_CMD_DEL_SESSION,
        (void (*)(int))ppa_del_session_help,
        ppa_parse_del_session_cmd,
        ppa_do_cmd,
        NULL,
        NULL,
        ppa_del_session_short_opts
    },
    {
        "modifysession",  // get the dsl mib
        PPA_CMD_MODIFY_SESSION,
        ppa_modify_session_help,
        ppa_parse_modify_session_cmd,
        ppa_do_cmd,
        NULL,
        ppa_modify_session_long_opts,
        ppa_modify_session_short_opts
    },
    {
        "getsessiontimer",  // get routing session polling timer
        PPA_CMD_GET_SESSION_TIMER,
        ppa_get_session_timer_help,
        ppa_parse_get_session_timer_cmd,
        ppa_do_cmd,
        ppa_print_get_session_timer,
        ppa_get_session_timer_long_opts,
        ppa_get_session_timer_short_opts
    },
    {
        "setsessiontimer",  // set routing session polling timer
        PPA_CMD_SET_SESSION_TIMER,
        ppa_set_session_timer_help,
        ppa_parse_set_session_timer_cmd,
        ppa_do_cmd,
        NULL,
        ppa_set_session_timer_long_opts,
        ppa_set_session_timer_short_opts
    },
    {
        "---PPA multicast acceleration related commands",
        PPA_CMD_INIT,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
	{
		"addmc",
		PPA_CMD_ADD_MC,
		ppa_add_mc_help,
		ppa_parse_add_mc_cmd,
		ppa_do_cmd,
		NULL,
		ppa_mc_sess_long_opts,
		ppa_mc_sess_short_opts
	},
	{
		"delmc",
		PPA_CMD_DEL_MC,
		ppa_del_mc_help,
		ppa_parse_add_mc_cmd,
		ppa_do_cmd,
		NULL,
		ppa_mc_sess_long_opts,
		ppa_mc_sess_short_opts
	},
    {
        "getmcnum",
        PPA_CMD_GET_COUNT_MC_GROUP,
        ppa_get_mc_count_help,
        ppa_parse_get_mc_count_cmd,
        ppa_do_cmd,
        ppa_print_get_count_cmd,
        ppa_get_simple_long_opts,
        ppa_get_simple_short_opts
    },
    {
        "getmcgroups",
        PPA_CMD_GET_MC_GROUPS,
        ppa_get_mc_groups_help,
        ppa_parse_get_mc_group_cmd,
        ppa_get_mc_groups_cmd,
        NULL,
        ppa_get_mc_group_long_opts,
        ppa_get_mc_group_short_opts
    },
    {
        "getmcextra",
        PPA_CMD_GET_MC_ENTRY,
        ppa_get_mc_extra_help,
        ppa_parse_get_mc_extra_cmd,
        ppa_do_cmd,
        ppa_print_get_mc_extra_cmd,
        ppa_get_mc_extra_long_opts,
        ppa_get_mc_extra_short_opts
    },

#if defined(CONFIG_PPA_HANDLE_CONNTRACK_SESSIONS)
    {
        "---PPA session handling commands",
        PPA_CMD_INIT,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    {
        "getsessioncriteria",
        PPA_CMD_GET_SESSIONS_CRITERIA,
        ppa_get_session_criteria_help,
        ppa_parse_get_session_criteria, 
        ppa_do_cmd,
        ppa_get_session_criteria,
        NULL,
        ppa_get_session_criteria_short_opts
    },
    {
        "swapsessions", 
        PPA_CMD_SWAP_SESSIONS,
        ppa_swap_sessions_help,
        ppa_swap_sessions_cmd,
        ppa_do_cmd,
        NULL,
        ppa_no_long_opts,
        ppa_swap_sessions_short_opts
    },    
#endif
#if defined( CONFIG_IFX_VLAN_BR )  /*ONly supported in A4/D4 */
    {
        "---PPA VLAN bridging related commands(For PPE A4/D4/E4 only)",
        PPA_CMD_INIT,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    {
        "setvif",
        PPA_CMD_SET_VLAN_IF_CFG,
        ppa_set_vlan_if_cfg_help,
        ppa_parse_set_vlan_if_cfg_cmd,
        ppa_do_cmd,
        NULL,
        ppa_set_vlan_if_cfg_long_opts,
        ppa_set_vlan_if_cfg_short_opts
    },
    {
        "getvif",
        PPA_CMD_GET_VLAN_IF_CFG,
        ppa_get_vlan_if_cfg_help,
        ppa_parse_get_vlan_if_cfg_cmd,
        ppa_do_cmd,
        ppa_print_get_vif,
        ppa_if_long_opts,
        ppa_if_output_short_opts
    },
    {
        "addvfilter",
        PPA_CMD_ADD_VLAN_FILTER_CFG,
        ppa_add_vlan_filter_help,
        ppa_parse_add_vlan_filter_cmd,
        ppa_do_cmd,
        NULL,
        ppa_add_vlan_filter_long_opts,
        ppa_add_vlan_filter_short_opts
    },
    {
        "delvfilter",
        PPA_CMD_DEL_VLAN_FILTER_CFG,
        ppa_del_vlan_filter_help,
        ppa_parse_del_vlan_filter_cmd,
        ppa_do_cmd,
        NULL,
        ppa_del_vlan_filter_long_opts,
        ppa_del_vlan_filter_short_opts
    },
    {
        "getvfilternum",
        PPA_CMD_GET_COUNT_VLAN_FILTER,
        ppa_get_vfilter_count_help,
        ppa_parse_get_vfilter_count,
        ppa_do_cmd,
        ppa_print_get_count_cmd,
        ppa_get_simple_long_opts,
        ppa_get_simple_short_opts
    },
    {
        "getvfilters",
        PPA_CMD_GET_ALL_VLAN_FILTER_CFG,
        ppa_get_all_vlan_filter_help,
        ppa_parse_simple_cmd,
        ppa_get_all_vlan_filter_cmd,
        NULL,
        ppa_get_simple_long_opts,
        ppa_get_simple_short_opts
    },
    {
        "delallvfilter",
        PPA_CMD_DEL_ALL_VLAN_FILTER_CFG,
        ppa_del_all_vfilter_help,
        ppa_parse_simple_cmd,
        ppa_do_cmd,
        NULL,
        ppa_no_long_opts,
        ppa_no_short_opts
    },
#endif
#ifdef CONFIG_SOC_GRX500
    {
	"--PPA Classification Related Commands",
	PPA_CMD_INIT,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL
    },
    {
        "addclass",
        PPA_CMD_ADD_CLASSIFIER,
        ppa_addclass_help,
        ppa_parse_addclass_cmd,
        ppa_do_cmd,
        ppa_print_addclass_cmd,
        ppa_addclass_long_opts,
        ppa_addclass_short_opts
    },
    {
        "modclass",
        PPA_CMD_MOD_CLASSIFIER,
        ppa_modclass_help,
        ppa_parse_modclass_cmd,
        ppa_do_cmd,
        NULL,
        ppa_modclass_long_opts,
        ppa_modclass_short_opts
    },
    {
        "delclass",
        PPA_CMD_DEL_CLASSIFIER,
        ppa_delclass_help,
        ppa_parse_delclass_cmd,
        ppa_do_cmd,
        NULL,
        ppa_delclass_long_opts,
        ppa_delclass_short_opts
    },
#endif

    {
        "---PPA hook manipulation related commands",
        PPA_CMD_INIT,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },

#if defined(PPA_IP_COUNTER) && PPA_IP_COUNTER
    {
		"getifacemib",  // get the interface mib
		PPA_CMD_GET_IFACE_MIB,
		ppa_get_iface_mib_help,
		ppa_parse_get_iface_mib_cmd,
		ppa_do_cmd_get_iface_mib,
		ppa_print_get_iface_mib_cmd,
		ppa_no_long_opts,
		ppa_if_mib_short_opts
    },
#endif /* PPA_IP_COUNTER */

#if defined(RTP_SAMPLING_ENABLE) && RTP_SAMPLING_ENABLE
    {
        "setrtp",
        PPA_CMD_SET_RTP, 
        ppa_set_rtp_help,
        ppa_parse_set_rtp_cmd,
        ppa_do_cmd,
        NULL,
        ppa_rtp_set_long_opts,
        ppa_rtp_set_short_opts
    },
#endif

#if defined(MIB_MODE_ENABLE)
    {
        "setmibmode",
        PPA_CMD_SET_MIB_MODE, 
        ppa_set_mib_mode_help,
        ppa_parse_set_mib_mode,
        ppa_do_cmd,
        NULL,
        NULL,
        ppa_mib_mode_short_opts
    },
#endif
#if defined(PPA_TEST_AUTOMATION_ENABLE) && PPA_TEST_AUTOMATION_ENABLE
    /*Note, put all not discolsed command at the end of the array */
    {
        "automation",  // //set memory value  --hide the command
        PPA_CMD_INIT,
        ppa_test_automation_help,
        ppa_parse_test_automation_cmd,
        ppa_test_automation_cmd,
        NULL,
        ppa_test_automation_long_opts,
        ppa_test_automation_short_opts,
    },
#endif
	{
		"---PPA HAL commands",
		PPA_CMD_INIT,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL
	},

    {
		"hal",
		PPA_CMD_HAL_CONFIG,
		ppa_hal_config_help,
		ppa_parse_hal_config_cmd,
		ppa_do_cmd,
		ppa_print_hal_config,
		ppa_hal_config_long_opts,
		ppa_hal_config_short_opts,
	},

	{
		"halstats",
		PPA_CMD_HAL_STATS,
		ppa_hal_stats_help,
		ppa_parse_hal_stats_cmd,
		ppa_do_cmd,
		ppa_print_hal_stats,
		ppa_hal_stats_long_opts,
		ppa_hal_stats_short_opts,
	},
    { NULL, 0, NULL, NULL, NULL, NULL, NULL }
};

/*
====================================================================================
  command:   ppa_cmd_help Function
  description: prints help text
  options:   argv
====================================================================================
*/
static void ppa_print_help(void)
{
    PPA_COMMAND *pcmd;
    int i;

    IFX_PPACMD_PRINT("Usage: %s <command> {options} \n", PPA_CMD_NAME);

    IFX_PPACMD_PRINT("Commands: \n");
    for(pcmd = ppa_cmd; pcmd->name != NULL; pcmd++)
    {
        if(pcmd->print_help)
        {
            //IFX_PPACMD_PRINT(" "); //it will cause wrong alignment for hidden internal commands
            (*pcmd->print_help)(0);
        }
        else  if( pcmd->name[0] == '-' || pcmd->name[0] == ' ')
        {
#define MAX_CONSOLE_LINE_LEN 80
            int filling=strlen(pcmd->name)>=MAX_CONSOLE_LINE_LEN ? 0 : MAX_CONSOLE_LINE_LEN-strlen(pcmd->name);
            IFX_PPACMD_PRINT("\n%s", pcmd->name);
            for(i=0; i<filling; i++ ) IFX_PPACMD_PRINT("-");
            IFX_PPACMD_PRINT("\n");
        }
    }

#if PPACMD_DEBUG
    IFX_PPACMD_PRINT("\n");
    IFX_PPACMD_PRINT("* Note: Create a file %s will enable ppacmd debug mode\n", debug_enable_file );
    IFX_PPACMD_PRINT("* Note: Any number inputs will be regarded as decial value without prefix 0x\n");
    IFX_PPACMD_PRINT("* Note: Please run \"ppacmd <command name> -h\" to get its detail usage\n");
    IFX_PPACMD_PRINT("\n");
#endif

    return;
}

/*
====================================================================================
  command:   ppa_cmd_help Function
  description: prints help text
  options:   argv
====================================================================================
*/
static void ppa_print_cmd_help(PPA_COMMAND *pcmd)
{
    if(pcmd->print_help)
    {
        IFX_PPACMD_PRINT("Usage: %s ", PPA_CMD_NAME);
        (*pcmd->print_help)(1);
    }
    return;
}

/*
===============================================================================
  Command processing functions
===============================================================================
*/

/*
===========================================================================================


===========================================================================================
*/
static int get_ppa_cmd(char *cmd_str, PPA_COMMAND **pcmd)
{
    int i;

    // Locate the command where the name matches the cmd_str and return
    // the index in the command array.
    for (i = 0; ppa_cmd[i].name; i++)
    {
        if (strcmp(cmd_str, ppa_cmd[i].name) == 0)
        {
            *pcmd = &ppa_cmd[i];
            return PPA_CMD_OK;
        }
    }
    return PPA_CMD_ERR;
}

/*
===========================================================================================


===========================================================================================
*/
static int ppa_parse_cmd(int ac, char **av, PPA_COMMAND *pcmd, PPA_CMD_OPTS *popts)
{
    int opt, opt_idx, ret = PPA_CMD_OK;
    int num_opts;


    // Fill out the PPA_CMD_OPTS array with the option value and argument for
    // each option that is found. If option is help, display command help and
    // do not process command.
    for (num_opts = 0; num_opts < PPA_MAX_CMD_OPTS; num_opts++)
    {
        opt = getopt_long(ac - 1, av + 1, pcmd->short_opts, pcmd->long_opts, &opt_idx);
		if (opt == -1)  {
			break;
		} else if (opt == 1) {
			ret = PPA_CMD_ERR;
			break;
		} else if (opt == 'h') {
			ret = PPA_CMD_HELP;
			break;
		} else if (opt == '?') {
			ret = PPA_CMD_ERR;
			break;
        }
		popts->opt  = opt;
		popts->optarg = optarg;
		popts++;
	}

    return ret;
}

/*
===========================================================================================


===========================================================================================
*/
static int ppa_parse_cmd_line(int ac, char **av, PPA_COMMAND **pcmd, PPA_CMD_DATA **data)
{
    int ret = PPA_CMD_ERR;
    PPA_CMD_DATA *pdata = NULL;
    PPA_CMD_OPTS *popts = NULL;

    if ((ac <= 1) || (av == NULL))
    {
        return PPA_CMD_HELP;
    }

    pdata = malloc(sizeof(PPA_CMD_DATA));
    if (pdata == NULL)
        return PPA_CMD_NOT_AVAIL;
    ppa_memset(pdata, sizeof(PPA_CMD_DATA), 0 );

    popts = malloc(sizeof(PPA_CMD_OPTS)*PPA_MAX_CMD_OPTS);
    if (popts == NULL)
    {
        free(pdata);
        return PPA_CMD_NOT_AVAIL;
    }
    ppa_memset(popts, sizeof(PPA_CMD_OPTS)*PPA_MAX_CMD_OPTS , 0 );

    ret = get_ppa_cmd(av[1], pcmd);
    if (ret == PPA_CMD_OK)
    {
        ret = ppa_parse_cmd(ac, av, *pcmd, popts);
        if ( ret == PPA_CMD_OK )
        {
            ret = (*pcmd)->parse_options(popts,pdata);
            if ( ret == PPA_CMD_OK )
                *data = pdata;
        }
        else
        {
            IFX_PPACMD_PRINT("Wrong parameter\n");
            ret = PPA_CMD_HELP;
        }
    }
    else
    {
        IFX_PPACMD_PRINT("Unknown commands:  %s\n", av[1]);
    }
    free(popts);
    if (ret != PPA_CMD_OK)
        free(pdata);
    return ret;
}


/*
===========================================================================================


===========================================================================================
*/
int main(int argc, char** argv)
{
    int ret;
    PPA_CMD_DATA *pdata = NULL;
    PPA_COMMAND *pcmd=NULL;

    opterr = 0; //suppress option error messages

#if PPACMD_DEBUG
    {
        FILE *fp;

        fp = fopen(debug_enable_file, "r");
        if( fp != NULL )
        {
            enable_debug = 1;
            fclose(fp);
        }
    }
#endif

    if( argc == 1)
    {
        ppa_print_help();
        return 0;
    }
    ret = ppa_parse_cmd_line (argc, argv, &pcmd, &pdata);
    if (ret == PPA_CMD_OK)
    {
        if (pcmd->do_command)
        {
			ret = pcmd->do_command(pcmd, pdata);
            if (ret == PPA_CMD_OK && pcmd->print_data)
                pcmd->print_data(pdata);
        }
    }
    else if (ret == PPA_CMD_HELP)
    {
        ppa_print_cmd_help(pcmd);
    }
    if( pdata)
    {
        free(pdata);
        pdata=NULL;
    }
    return ret;
}
