/******************************************************************************
**
** FILE NAME  : qoscmd.c
** PROJECT    : QoS Configuration Utility
** MODULES    : Packet Acceleration
**
** DESCRIPTION  : QoS User Configuration Utility
** COPYRIGHT  : Copyright © 2021 MaxLinear, Inc.
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
#include <stdbool.h>
#include <uapi/asm-generic/ioctl.h>
#include <uapi/net/qos_mgr_common.h>
#include <errno.h>
#include <ctype.h>

#include "qoscmd.h"

int enable_debug = 0;
char *debug_enable_file="/var/qos_gdb_enable";

static int g_output_to_file = 0;
static char g_output_filename[QOSCMD_MAX_FILENAME]= {0};
#define DEFAULT_OUTPUT_FILENAME   "/var/tmp.dat"

static void qos_print_help(void);
static void qos_print_get_qstatus_cmd(QOS_MGR_CMD_DATA *pdata);
static int qos_get_qstatus_do_cmd(QOS_COMMAND *pcmd, QOS_MGR_CMD_DATA *pdata);

int SaveDataToFile(char *FileName, char *data, int len )
{
    FILE *pFile;

    if( data == NULL ) return  -1;
    
    if( FileName == NULL || strnlen_s(FileName,QOSCMD_MAX_FILENAME) == 0 )
        FileName = DEFAULT_OUTPUT_FILENAME;

    if( strcmp(FileName, "null") == 0 ) return 0; //don't save, but remove console print also no file saving
    
    pFile = fopen(FileName, "wb");

    if( pFile == NULL )
    {
        IFX_QOSCMD_PRINT("SaveDataToFile: fail to open file %s\n", FileName );
        return -1;
    }

    fwrite (data , 1 , len , pFile );
    fflush(pFile);
    fclose (pFile);

    return 0;
}

uint32_t str_convert(int type, const char *nptr, void *buff )
{
    char *endPtr;
    uint32_t res;

    if( nptr == NULL )
    {
        IFX_QOSCMD_PRINT("str_convert: nptr is NULL ??\n");
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
			res = QOS_CMD_ERR;
		else
			return res;
        }
        else  /*by default it is decimal value */
        {
            res = strtoul(nptr, &endPtr, 10);
		if (endPtr == NULL)
			res = QOS_CMD_ERR;
		else
			return res;
        }
    }
    else
    {
        IFX_QOSCMD_PRINT("str_convert: wrong type parameter(%d)\n", type);
        return 0;
    }
	return QOS_CMD_OK;
}

/*
===========================================================================================
   qos_do_ioctl_cmd

===========================================================================================
*/
static int qos_do_ioctl_cmd(int ioctl_cmd, void *data)
{
    int ret = QOS_CMD_OK;
    int fd  = 0;

    if ((fd = open (QOS_DEVICE, O_RDWR)) < 0)
    {
        IFX_QOSCMD_PRINT ("\n [%s] : open QoS device (%s) failed. (errno=%d)\n", __FUNCTION__, QOS_DEVICE, errno);
        ret = QOS_CMD_ERR;
    }
    else
    {
        if (ioctl (fd, ioctl_cmd, data) < 0)
        {
            IFX_QOSCMD_PRINT ("\n [%s] : ioctl failed for NR %d. (errno=%d(system error:%s))\n", __FUNCTION__, _IOC_NR(ioctl_cmd), errno, strerror(errno));
            ret = QOS_CMD_ERR;
        }
        close (fd);
    }
    
    return ret;
}

int get_portid(char *ifname)
{
    QOS_MGR_CMD_PORTID_INFO portid;

    qos_memset( &portid, sizeof(portid), 0);
    strncpy_s( portid.ifname,QOS_MGR_IF_NAME_SIZE, ifname,QOS_MGR_IF_NAME_SIZE);

    if( qos_do_ioctl_cmd(QOS_MGR_CMD_GET_PORTID, &portid ) != QOS_CMD_OK )
    {
        IFX_QOSCMD_PRINT("qoscmd get portid failed\n");
        return -1;
    }

    return portid.portid;
}

/*
====================================================================================
   Generic option sets
   These option sets are shared among several commands.
===================================================================================
*/

static int qos_do_cmd(QOS_COMMAND *pcmd, QOS_MGR_CMD_DATA *pdata)
{
    return qos_do_ioctl_cmd(pcmd->ioctl_cmd,pdata);
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
} INIT_CMD;

const char *cfg_names[] =
{
    "comment",
};

#ifdef CONFIG_PPA_QOS

static void qos_get_qstatus_help( int summary)
{
    IFX_QOSCMD_PRINT("getqstatus [-o outfile]\n");
    return;
}
static const char qos_get_qstatus_short_opts[] = "o:h";
static int qos_parse_get_qstatus_cmd(QOS_CMD_OPTS *popts, QOS_MGR_CMD_DATA* pdata)
{
    int  out_opt=0;

    qos_memset( pdata, sizeof(QOS_MGR_CMD_STATUS_INFO), 0 );

    while(popts->opt)
    {
        if (popts->opt == 'o')
        {
            g_output_to_file = 1;
            strncpy_s(g_output_filename,QOSCMD_MAX_FILENAME,popts->optarg,QOSCMD_MAX_FILENAME);
            out_opt++;
        }
        else
        {
            return QOS_CMD_ERR;
        }
        popts++;
    }

    if (out_opt > 1)
        return QOS_CMD_ERR;

    IFX_QOSCMD_DBG("QOS getqstatus\n");

    return QOS_CMD_OK;
}

static int qos_get_qstatus_do_cmd(QOS_COMMAND *pcmd, QOS_MGR_CMD_DATA *pdata)
{
    int i;
    int res=QOS_CMD_ERR;

    for(i=0; i<QOS_MGR_MAX_PORT_NUM; i++ )
    {
        pdata->qos_status_info.qstat.qos_queue_portid = i;
        if( qos_do_ioctl_cmd(QOS_MGR_CMD_GET_QOS_STATUS, &pdata->qos_status_info ) == QOS_CMD_OK )
        {          
            if( pdata->qos_status_info.qstat.res == QOS_MGR_SUCCESS )
            {
                qos_print_get_qstatus_cmd(pdata);
                res = QOS_CMD_OK;
            }            
        }
    }
    return res;
}
static void qos_print_get_qstatus_cmd(QOS_MGR_CMD_DATA *pdata)
{
    unsigned long long rate[QOS_MGR_MAX_QOS_QUEUE_NUM];
    unsigned long long rate_L1[QOS_MGR_MAX_QOS_QUEUE_NUM];
    
    if( g_output_to_file)
    {
        SaveDataToFile(g_output_filename, (char *)(&pdata->qos_status_info), sizeof(pdata->qos_status_info) );
        return ;
    }

    if( pdata->qos_status_info.qstat.qos_queue_portid == -1 )
    {
        /*qos is not enabled */
        IFX_QOSCMD_PRINT("Note: QOS is not supported for wan_itf=%x  mixed_itf=%x\n", (unsigned int)pdata->qos_status_info.qstat.wan_port_map, (unsigned int)pdata->qos_status_info.qstat.wan_mix_map);
        return ;
    }

    IFX_QOSCMD_PRINT("\nPort[%2d]\n  qos     : %s(For VR9 E5 VDSL WAN mode, this flag is not used)\n  wfq     : %s\n  Rate shaping: %s\n\n",
                     pdata->qos_status_info.qstat.qos_queue_portid,
                     pdata->qos_status_info.qstat.eth1_qss ?"enabled":"disabled",
                     pdata->qos_status_info.qstat.wfq_en?"enabled":"disabled",
                     pdata->qos_status_info.qstat.shape_en ?"enabled":"disabled");

    IFX_QOSCMD_PRINT("  Ticks  =%u,  overhd  =%u,     qnum=%u  @0x%x\n",
                     (unsigned int)pdata->qos_status_info.qstat.time_tick,
                     (unsigned int)pdata->qos_status_info.qstat.overhd_bytes,
                     (unsigned int)pdata->qos_status_info.qstat.eth1_eg_qnum,
                     (unsigned int)pdata->qos_status_info.qstat.tx_qos_cfg_addr);

    IFX_QOSCMD_PRINT("  PPE clk=%u MHz, basic tick=%u\n",(unsigned int)pdata->qos_status_info.qstat.pp32_clk/1000000, (unsigned int)pdata->qos_status_info.qstat.basic_time_tick );

    IFX_QOSCMD_PRINT("\n  wfq_multiple : %08u @0x%x", (unsigned int)pdata->qos_status_info.qstat.wfq_multiple, (unsigned int)pdata->qos_status_info.qstat.wfq_multiple_addr);
    IFX_QOSCMD_PRINT("\n  strict_weight: %08u @0x%x\n", (unsigned int)pdata->qos_status_info.qstat.wfq_strict_pri_weight, (unsigned int)pdata->qos_status_info.qstat.wfq_strict_pri_weight_addr);

    if ( pdata->qos_status_info.qstat.eth1_eg_qnum && pdata->qos_status_info.qstat.max_buffer_size  )
    {
        uint32_t i;
        uint32_t times = (pdata->qos_status_info.qstat.eth1_eg_qnum > pdata->qos_status_info.qstat.max_buffer_size) ? pdata->qos_status_info.qstat.max_buffer_size:pdata->qos_status_info.qstat.eth1_eg_qnum;

        IFX_QOSCMD_PRINT("\n  Cfg :  T   R   S -->  Bit-rate(kbps)    Weight --> Level     Address     d/w    tick_cnt   b/S\n");

        for ( i = 0; i < times; i++ )
        {
            IFX_QOSCMD_PRINT("\n    %2u:  %03u  %05u  %05u   %07u      %08u   %03u    @0x%x   %08u  %03u   %05u\n", (unsigned int)i,
                             (unsigned int)pdata->qos_status_info.qstat.queue_internal[i].t,
                             (unsigned int)pdata->qos_status_info.qstat.queue_internal[i].r,
                             (unsigned int)pdata->qos_status_info.qstat.queue_internal[i].s,
                             (unsigned int)pdata->qos_status_info.qstat.queue_internal[i].bit_rate_kbps,
                             (unsigned int)pdata->qos_status_info.qstat.queue_internal[i].w,
                             (unsigned int)pdata->qos_status_info.qstat.queue_internal[i].weight_level,
                             (unsigned int)pdata->qos_status_info.qstat.queue_internal[i].reg_addr,
                             (unsigned int)pdata->qos_status_info.qstat.queue_internal[i].d,
                             (unsigned int)pdata->qos_status_info.qstat.queue_internal[i].tick_cnt,
                             (unsigned int)pdata->qos_status_info.qstat.queue_internal[i].b);
        }

        //QOS Note: For ethernat wan mode only one port rateshaping.  For E5 ptm mode, we have 4 gamma interface port rateshaping configuration
        if( pdata->qos_status_info.qstat.qos_queue_portid == 7 ){//PTM wan mode
            for( i = 0; i < 4; i ++){
                IFX_QOSCMD_PRINT("\n  p[%d]:  %03u  %05u  %05u   %07u                        @0x%x   %08u  %03u   %05u\n",
                             i, (unsigned int)pdata->qos_status_info.qstat.ptm_qos_port_rate_shaping[i].t,
                             (unsigned int)pdata->qos_status_info.qstat.ptm_qos_port_rate_shaping[i].r,
                             (unsigned int)pdata->qos_status_info.qstat.ptm_qos_port_rate_shaping[i].s,
                             (unsigned int)pdata->qos_status_info.qstat.ptm_qos_port_rate_shaping[i].bit_rate_kbps,
                             (unsigned int)pdata->qos_status_info.qstat.ptm_qos_port_rate_shaping[i].reg_addr,
                             (unsigned int)pdata->qos_status_info.qstat.ptm_qos_port_rate_shaping[i].d,
                             (unsigned int)pdata->qos_status_info.qstat.ptm_qos_port_rate_shaping[i].tick_cnt,
                             (unsigned int)pdata->qos_status_info.qstat.ptm_qos_port_rate_shaping[i].b);
            }
        }
        else if( pdata->qos_status_info.qstat.qos_queue_portid & 3){
            IFX_QOSCMD_PRINT("\n  port:  %03u  %05u  %05u   %07u                        @0x%x   %08u  %03u   %05u\n",
                             (unsigned int)pdata->qos_status_info.qstat.qos_port_rate_internal.t,
                             (unsigned int)pdata->qos_status_info.qstat.qos_port_rate_internal.r,
                             (unsigned int)pdata->qos_status_info.qstat.qos_port_rate_internal.s,
                             (unsigned int)pdata->qos_status_info.qstat.qos_port_rate_internal.bit_rate_kbps,
                             (unsigned int)pdata->qos_status_info.qstat.qos_port_rate_internal.reg_addr,
                             (unsigned int)pdata->qos_status_info.qstat.qos_port_rate_internal.d,
                             (unsigned int)pdata->qos_status_info.qstat.qos_port_rate_internal.tick_cnt,
                             (unsigned int)pdata->qos_status_info.qstat.qos_port_rate_internal.b);

        }
        
        //print debug if necessary
        for ( i = 0; i <times ; i++ )
        {
            if( pdata->qos_status_info.qstat.mib[i].mib.tx_diff_jiffy == 0 ) 
            {
                IFX_QOSCMD_PRINT("Error, why tx_diff_jiffy[%2u] is zero\n", i);                
                pdata->qos_status_info.qstat.mib[i].mib.tx_diff_jiffy = 1;
            }
            rate[i]=pdata->qos_status_info.qstat.mib[i].mib.tx_diff * 8 * pdata->qos_status_info.qstat.mib[i].mib.sys_hz/pdata->qos_status_info.qstat.mib[i].mib.tx_diff_jiffy;
            rate_L1[i]=pdata->qos_status_info.qstat.mib[i].mib.tx_diff_L1 * 8 * pdata->qos_status_info.qstat.mib[i].mib.sys_hz/pdata->qos_status_info.qstat.mib[i].mib.tx_diff_jiffy;
        }
        
        IFX_QOSCMD_DBG("\n  Info : Rate  tx_diff_bytes tx_diff_jiffy   HZ\n");
        for ( i = 0; i < times ; i++ )
        {            
            IFX_QOSCMD_DBG("  %2u:    %010llu %010llu %010llu %010u\n",(unsigned int)i,rate[i],
                             (unsigned long long)pdata->qos_status_info.qstat.mib[i].mib.tx_diff,
                             (unsigned long long)pdata->qos_status_info.qstat.mib[i].mib.tx_diff_jiffy,
                             pdata->qos_status_info.qstat.mib[i].mib.sys_hz);            
        }

        IFX_QOSCMD_PRINT("\n  MIB : rx_pkt/rx_bytes      tx_pkt/tx_bytes         cpu_small/total_drop  fast_small/total_drop  tx rate/L1(bps/sec)   address\n");
        for ( i = 0; i < times ; i++ )
        {            
            IFX_QOSCMD_PRINT("  %2u: %010u/%010u  %010u/%010u  %010u/%010u  %010u/%010u  %010u/%010u @0x%x\n",
                             (unsigned int)i,
                             (unsigned int)pdata->qos_status_info.qstat.mib[i].mib.total_rx_pkt,
                             (unsigned int)pdata->qos_status_info.qstat.mib[i].mib.total_rx_bytes,
                             (unsigned int)pdata->qos_status_info.qstat.mib[i].mib.total_tx_pkt,
                             (unsigned int)pdata->qos_status_info.qstat.mib[i].mib.total_tx_bytes,
                             (unsigned int)pdata->qos_status_info.qstat.mib[i].mib.cpu_path_small_pkt_drop_cnt,
                             (unsigned int)pdata->qos_status_info.qstat.mib[i].mib.cpu_path_total_pkt_drop_cnt,
                             (unsigned int)pdata->qos_status_info.qstat.mib[i].mib.fast_path_small_pkt_drop_cnt,
                             (unsigned int)pdata->qos_status_info.qstat.mib[i].mib.fast_path_total_pkt_drop_cnt,
                             (unsigned int)rate[i],
                             (unsigned int)rate_L1[i],
                             (unsigned int)pdata->qos_status_info.qstat.mib[i].reg_addr                            
                            ) ;

        }

        //QOS queue descriptor
        IFX_QOSCMD_PRINT("\n  Desc: threshold  num  base_addr  rd_ptr   wr_ptr\n");
        for(i=0; i<times; i++)
        {
            IFX_QOSCMD_PRINT("  %2u: 0x%02x     0x%02x   0x%04x   0x%04x   0x%04x  @0x%x\n",
                             (unsigned int)i,
                             (unsigned int)pdata->qos_status_info.qstat.desc_cfg_interanl[i].threshold,
                             (unsigned int)pdata->qos_status_info.qstat.desc_cfg_interanl[i].length,
                             (unsigned int)pdata->qos_status_info.qstat.desc_cfg_interanl[i].addr,
                             (unsigned int)pdata->qos_status_info.qstat.desc_cfg_interanl[i].rd_ptr,
                             (unsigned int)pdata->qos_status_info.qstat.desc_cfg_interanl[i].wr_ptr,
                             (unsigned int)pdata->qos_status_info.qstat.desc_cfg_interanl[i].reg_addr );
        }

    }
    else
    {
        IFX_QOSCMD_PRINT("Note: QOS is disabled for wan_itf=%x  mixed_itf=%x\n",
                         (unsigned int)pdata->qos_status_info.qstat.wan_port_map,
                         (unsigned int)pdata->qos_status_info.qstat.wan_mix_map);
    }


}

/*qoscmd getqnum: get eth1 queue number  ---begin*/
static void qos_get_qnum_help( int summary)
{
    IFX_QOSCMD_PRINT("getqnum [-p portid] [-i ifname] [-o outfile]\n");
    if (summary )
        IFX_QOSCMD_PRINT("note: one or only one of portid or ifname must be specified\n");
    return;
}
static const char qos_get_qnum_short_opts[] = "p:o:i:h";
static const struct option qos_get_qnum_long_opts[] =
{
    {"portid",   required_argument,  NULL, 'p'},
    {"interface",   required_argument,  NULL, 'i'},
    {"save-to-file",   required_argument,  NULL, 'o'},
    { 0,0,0,0 }
};
static int qos_parse_get_qnum_cmd(QOS_CMD_OPTS *popts, QOS_MGR_CMD_DATA* pdata)
{
    int p_opt=0, i_opt=0;
    qos_memset( pdata, sizeof(QOS_MGR_CMD_QUEUE_NUM_INFO),0 );

    while(popts->opt)
    {
        switch(popts->opt)
        {
        case 'p':
            pdata->qnum_info.portid = str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL);
            \
            p_opt ++;
            break;

        case 'i':
            if( get_portid(popts->optarg) >= 0 )
            {
                pdata->qnum_info.portid = get_portid(popts->optarg);
                i_opt ++;
            }
            else
            {
                IFX_QOSCMD_PRINT("The portid of %s is not exist.\n", popts->optarg);
                return QOS_CMD_ERR;
            }
            break;

        case  'o':
            g_output_to_file = 1;
            strncpy_s(g_output_filename,QOSCMD_MAX_FILENAME,popts->optarg,QOSCMD_MAX_FILENAME);
            break;

        default:
            return QOS_CMD_ERR;
        }
        popts++;
    }

    if( p_opt + i_opt != 1)
    {
        IFX_QOSCMD_PRINT("note: one or only one of portid and ifname must be specified\n");
        return QOS_CMD_ERR;
    }
    IFX_QOSCMD_DBG("qos_parse_get_qnum_cmd: portid=%d, queue_num=%d\n", (unsigned int)pdata->qnum_info.portid, (unsigned int)pdata->qnum_info.queue_num);

    return QOS_CMD_OK;
}
static void qos_print_get_qnum_cmd(QOS_MGR_CMD_DATA *pdata)
{
    if( !g_output_to_file )
    {
        IFX_QOSCMD_PRINT("The queue number( of port id %d )  is %d\n", (unsigned int)pdata->qnum_info.portid, (unsigned int)pdata->qnum_info.queue_num);
    }
    else
    {
        SaveDataToFile(g_output_filename, (char *)(&pdata->qnum_info), sizeof(pdata->qnum_info) );
    }
}

/*qoscmd getmib: get qos mib counter  ---begin*/
static void qos_get_qmib_help( int summary)
{
    IFX_QOSCMD_PRINT("getqmib [-p portid] [-i ifname] <-q queuid> [-o outfile]\n");
    if( summary )
    {
        IFX_QOSCMD_PRINT("note: one or only one of portid or ifname must be specified\n");
        IFX_QOSCMD_PRINT("    if queueid is not provided, then it will get all queue's mib coutners\n");
    }
    return;
}
static const char qos_get_qmib_short_opts[] = "p:i:q:o:h";
static const struct option qos_get_qmib_long_opts[] =
{
    {"portid",   required_argument,  NULL, 'p'},
    {"ifname",   required_argument,  NULL, 'i'},
    {"queueid",   required_argument,  NULL, 'q'},
    {"save-to-file",   required_argument,  NULL, 'o'},
    { 0,0,0,0 }
};
static int qos_parse_get_qmib_cmd(QOS_CMD_OPTS *popts, QOS_MGR_CMD_DATA* pdata)
{
    int p_opt=0, i_opt=0;

    qos_memset( pdata, sizeof(QOS_MGR_CMD_MIB_INFO), 0 );
    pdata->qos_mib_info.queue_num = -1;

    while(popts->opt)
    {
        switch(popts->opt)
        {
        case 'p':
            pdata->qos_mib_info.portid = str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL);
            p_opt ++;
            break;

        case 'i':
            strcpy_s( pdata->qos_mib_info.ifname,QOS_MGR_IF_NAME_SIZE, popts->optarg );
            if( get_portid(popts->optarg) >= 0 )
            {
                pdata->qos_mib_info.portid = get_portid(popts->optarg);
                i_opt ++;
            }
            else
            {
                IFX_QOSCMD_PRINT("The portid of %s is not exist.\n", popts->optarg);
                return QOS_CMD_ERR;
            }
            break;

        case 'q':
            pdata->qos_mib_info.queue_num= str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL);
            break;

        case  'o':
            g_output_to_file = 1;
            strncpy_s(g_output_filename,QOSCMD_MAX_FILENAME, popts->optarg,QOSCMD_MAX_FILENAME);
            break;

        default:
            return QOS_CMD_ERR;
        }
        popts++;
    }

    if( p_opt + i_opt != 1)
    {
        IFX_QOSCMD_PRINT("note: one or only one of portid and ifname must be specified\n");
        return QOS_CMD_ERR;
    }

    IFX_QOSCMD_DBG("qos_parse_get_qmib_cmd: portid=%d, queueid=%d\n", (unsigned int)pdata->qos_mib_info.portid, (unsigned int)pdata->qos_mib_info.queue_num);

    return QOS_CMD_OK;
}

static int qos_get_qmib_do_cmd(QOS_COMMAND *pcmd, QOS_MGR_CMD_DATA *pdata)
{
    QOS_MGR_CMD_QUEUE_NUM_INFO qnum_info;
    QOS_MGR_CMD_MIB_INFO mib_info;
    int i, start_i, end_i=0;
    int res;

    qos_memset( &qnum_info, sizeof(qnum_info), 0 );
    qnum_info.portid = pdata->qos_mib_info.portid;
    if( (res = qos_do_ioctl_cmd(QOS_MGR_CMD_GET_QOS_QUEUE_MAX_NUM, &qnum_info ) != QOS_CMD_OK ) )
    {
        IFX_QOSCMD_PRINT("ioctl QOS_MGR_CMD_GET_QOS_QUEUE_MAX_NUM fail\n");
        return QOS_MGR_FAILURE;
    }
    if( pdata->qos_mib_info.queue_num != -1 ) // set queueid already, then use it
    {
        start_i = pdata->qos_mib_info.queue_num;
        if( start_i >= qnum_info.queue_num -1 )
            start_i = qnum_info.queue_num -1;
        end_i = start_i + 1;
        IFX_QOSCMD_PRINT("Need to read queue %d's mib counter\n", start_i);
    }
    else
    {
        start_i = 0;
        end_i = qnum_info.queue_num;
        IFX_QOSCMD_PRINT("Need to read mib counter from queue %d to %d\n", start_i, end_i-1 );
    }

    for(i=start_i; i<end_i; i++)
    {
        qos_memset( &mib_info, sizeof(mib_info), 0 );
        mib_info.portid = pdata->qos_mib_info.portid;
        strcpy_s( mib_info.ifname,QOS_MGR_IF_NAME_SIZE, pdata->qos_mib_info.ifname);
        mib_info.queue_num = i;

        if( (res = qos_do_ioctl_cmd(QOS_MGR_CMD_GET_QOS_MIB, &mib_info) == QOS_CMD_OK ) )
        {
            if( !g_output_to_file  )
            {
                if( i== start_i )
                    IFX_QOSCMD_PRINT("MIB  rx_pkt/rx_bytes     tx_pkt/tx_bytes    cpu_small_drop/cpu_drop  fast_small_drop/fast_drop_cnt  q_occ(pkt)\n");

                IFX_QOSCMD_PRINT("  %2d: 0x%08x/0x%08x  0x%08x/0x%08x  0x%08x/0x%08x  0x%08x/0x%08x  0x%08x\n", i,
                                 (unsigned int)mib_info.mib.total_rx_pkt, (unsigned int)mib_info.mib.total_rx_bytes,
                                 (unsigned int)mib_info.mib.total_tx_pkt, (unsigned int)mib_info.mib.total_tx_bytes,
                                 (unsigned int)mib_info.mib.cpu_path_small_pkt_drop_cnt, (unsigned int)mib_info.mib.cpu_path_total_pkt_drop_cnt,
                                 (unsigned int)mib_info.mib.fast_path_small_pkt_drop_cnt, (unsigned int)mib_info.mib.fast_path_total_pkt_drop_cnt,
                                 (unsigned int)mib_info.mib.q_occ );
            }
            else
            {
                /*note, currently only last valid flow info is saved to file and all other flow informations are all overwritten */
                SaveDataToFile(g_output_filename, (char *)(&mib_info), sizeof(mib_info) );
            }
        }
    }
    return QOS_MGR_SUCCESS;
}

/*qoscmd getqmib: get eth1 queue number  ---end*/

/*qoscmd setctrlwfq ---begin*/
static void qos_set_ctrl_wfq_help( int summary)
{
    IFX_QOSCMD_PRINT("setctrlwfq [-p portid] [-i ifname] <-c enable | disable> [-m manual_wfq]\n");
    if( summary )
    {
        IFX_QOSCMD_PRINT("note: one or only one of portid and ifname must be specified\n");
        IFX_QOSCMD_PRINT("note: manual_wfq 0      -- default weight\n");
        IFX_QOSCMD_PRINT("note: manual_wfq 1      -- use user specified weight directly\n");
        IFX_QOSCMD_PRINT("note: manual_wfq other_value-- use user specified mapping \n");
    }
    return;
}
static const char qos_set_ctrl_wfq_short_opts[] = "p:c:i:m:h";
static const struct option qos_set_ctrl_wfq_long_opts[] =
{
    {"portid",  required_argument,  NULL, 'p'},
    {"ifname",   required_argument,  NULL, 'i'},
    {"control",   required_argument,  NULL, 'c'},
    {"manual",   required_argument,  NULL, 'm'},
    { 0,0,0,0 }
};
static int qos_parse_set_ctrl_wfq_cmd(QOS_CMD_OPTS *popts, QOS_MGR_CMD_DATA *pdata)
{
    unsigned int c_opt=0;
    int p_opt=0, i_opt=0;
    uint32_t res = 0;

    qos_memset( pdata, sizeof(QOS_MGR_CMD_CTRL_INFO), 0 );

    while(popts->opt)
    {
        switch(popts->opt)
        {
        case 'p':
		res = str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL);
		if (res == QOS_CMD_ERR){
			IFX_QOSCMD_PRINT("ERROR: invalid input\n");
			return res;
		}
		pdata->qos_ctrl_info.portid = res;
		p_opt ++;
		break;

        case 'i':
            if( get_portid(popts->optarg) >= 0 )
            {
                pdata->qos_ctrl_info.portid = get_portid(popts->optarg);
                i_opt ++;
            }
            else
            {
                IFX_QOSCMD_PRINT("The portid of %s is not exist.\n", popts->optarg);
                return QOS_CMD_ERR;
            }
            break;

        case 'c':
            if( strcmp("enable" , popts->optarg) == 0 )
                pdata->qos_ctrl_info.enable = 1;
            else if ( strcmp("disable" , popts->optarg) == 0 )
                pdata->qos_ctrl_info.enable = 0;
	    else {
		IFX_QOSCMD_PRINT("ERROR: invalid input\n");
		return QOS_CMD_ERR;
	    }
            c_opt = 1;
            break;

        case 'm':
		res = str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL);
		if (res == QOS_CMD_ERR){
			IFX_QOSCMD_PRINT("ERROR: invalid input\n");
			return res;
		}
		pdata->qos_ctrl_info.flags= res; //use manual set WFQ weight
		break;

        default:
            return QOS_CMD_ERR;
        }
        popts++;
    }

    if( c_opt == 0 )
    {
        IFX_QOSCMD_PRINT("Need enter control option\n");
        return QOS_CMD_ERR;
    }

    if( p_opt + i_opt != 1)
    {
        IFX_QOSCMD_PRINT("note: one or only one of portid and ifname must be specified\n");
        return QOS_CMD_ERR;
    }

    IFX_QOSCMD_DBG("qos_parse_set_ctrl_wfq_cmd: portid=%d, ctrl=%s\n", (unsigned int)pdata->qos_ctrl_info.portid, pdata->qos_ctrl_info.enable ? "enabled":"disable");

    return QOS_CMD_OK;
}
/*qoscmd setctrlwfq ---end*/

/*qoscmd getctrlwfq ---begin*/
static void qos_get_ctrl_wfq_help( int summary)
{
    IFX_QOSCMD_PRINT("getctrlwfq [-p portid] [-i ifname] [-o outfile]\n");
    if( summary )
        IFX_QOSCMD_PRINT("note: one or only one of portid and ifname must be specified\n");
    return;
}
static const char qos_get_ctrl_wfq_short_opts[] = "p:i:o:h";
static const struct option qos_get_ctrl_wfq_long_opts[] =
{
    {"portid",   required_argument,  NULL, 'p'},
    {"ifname",   required_argument,  NULL, 'i'},
    {"save-to-file",   required_argument,  NULL, 'o'},
    { 0,0,0,0 }
};
static int qos_parse_get_ctrl_wfq_cmd(QOS_CMD_OPTS *popts, QOS_MGR_CMD_DATA *pdata)
{
    int p_opt=0, i_opt=0;
    qos_memset( pdata, sizeof(QOS_MGR_CMD_CTRL_INFO), 0 );

    while(popts->opt)
    {
        switch(popts->opt)
        {
        case 'p':
            pdata->qos_ctrl_info.portid = str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL);
            p_opt ++;
            break;

        case 'i':
            if( get_portid(popts->optarg) >= 0 )
            {
                pdata->qos_ctrl_info.portid = get_portid(popts->optarg);
                i_opt ++;
            }
            else
            {
                IFX_QOSCMD_PRINT("The portid of %s is not exist.\n", popts->optarg);
                return QOS_CMD_ERR;
            }
            break;

        case  'o':
            g_output_to_file = 1;
            strncpy_s(g_output_filename,QOSCMD_MAX_FILENAME,popts->optarg,QOSCMD_MAX_FILENAME);
            break;

        default:
            return QOS_CMD_ERR;
        }
        popts++;
    }

    if( p_opt + i_opt != 1)
    {
        IFX_QOSCMD_PRINT("note: one or only one of portid and ifname must be specified\n");
        return QOS_CMD_ERR;
    }
    IFX_QOSCMD_DBG("qos_parse_get_ctrl_wfq_cmd: portid=%d, ctrl=%s\n", (unsigned int)pdata->qos_ctrl_info.portid, pdata->qos_ctrl_info.enable? "enabled":"disabled");

    return QOS_CMD_OK;
}
static void qos_print_get_ctrl_wfq_cmd(QOS_MGR_CMD_DATA *pdata)
{
    if( !g_output_to_file )
    {
        IFX_QOSCMD_PRINT("The wfq of port id %d is %s\n", (unsigned int)pdata->qos_ctrl_info.portid, pdata->qos_ctrl_info.enable ? "enabled":"disabled");
    }
    else
    {
        SaveDataToFile(g_output_filename, (char *)(&pdata->qos_ctrl_info), sizeof(pdata->qos_ctrl_info) );
    }
}
/*qoscmd getctrlwfq ---end*/



/*qoscmd setwfq ---begin*/
static void qos_set_wfq_help( int summary)
{
    IFX_QOSCMD_PRINT("setwfq [-p portid] [-i ifname] <-q queuid> <-w weight-level>\n");
    if( summary )
    {
        IFX_QOSCMD_PRINT("weight-level is from 0 ~ 100. 0/100 means lowest/highest strict priority queue\n");
        IFX_QOSCMD_PRINT("note: one or only one of portid and ifname must be specified\n");
    }
    return;
}
static const char qos_set_wfq_short_opts[] = "p:q:w:i:h";
static const struct option qos_set_wfq_long_opts[] =
{
    {"portid",   required_argument,  NULL, 'p'},
    {"ifname",   required_argument,  NULL, 'i'},
    {"queuid",   required_argument,  NULL, 'q'},
    {"weight-level",   required_argument,  NULL, 'w'},
    { 0,0,0,0 }
};

static int qos_parse_set_wfq_cmd(QOS_CMD_OPTS *popts, QOS_MGR_CMD_DATA*pdata)
{
    unsigned int w_opt=0, q_opt=0;
    int p_opt=0, i_opt=0;
    uint32_t res;
    qos_memset( pdata, sizeof(QOS_MGR_CMD_WFQ_INFO), 0 );

    while(popts->opt)
    {
        switch(popts->opt)
        {
        case 'p':
		res = str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL);
		if (res == QOS_CMD_ERR){
			IFX_QOSCMD_PRINT("ERROR: invalid input\n");
			return res;
		}
		pdata->qos_wfq_info.portid = res;
		p_opt ++;
		break;
        case 'i':
            if( get_portid(popts->optarg) >= 0 )
            {
                pdata->qos_wfq_info.portid = get_portid(popts->optarg);
                i_opt ++;
            }
            else
            {
                IFX_QOSCMD_PRINT("The portid of %s is not exist.\n", popts->optarg);
                return QOS_CMD_ERR;
            }
            break;
        case 'q':
		res = str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL);
		if (res == QOS_CMD_ERR){
			IFX_QOSCMD_PRINT("ERROR: invalid input\n");
			return res;
		}
		pdata->qos_wfq_info.queueid = res;
		q_opt = 1;
		break;
        case 'w':
		res = str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL);
		if (res == QOS_CMD_ERR){
			IFX_QOSCMD_PRINT("ERROR: invalid input\n");
			return res;
		}
		pdata->qos_wfq_info.weight = res;
		w_opt = 1;
		break;
        default:
            IFX_QOSCMD_PRINT("qos_parse_set_wfq_cmd not support parameter -%c \n", popts->opt);
            return QOS_CMD_ERR;
        }
        popts++;
    }

    if( q_opt == 0 )
    {
        IFX_QOSCMD_PRINT("Need enter queueid\n");
        return QOS_CMD_ERR;
    }
    if( w_opt == 0 )
    {
        IFX_QOSCMD_PRINT("Need enter weight\n");
        return QOS_CMD_ERR;
    }
    if( p_opt + i_opt != 1)
    {
        IFX_QOSCMD_PRINT("note: one or only one of portid and ifname must be specified\n");
        return QOS_CMD_ERR;
    }
    IFX_QOSCMD_DBG("qos_parse_set_wfq_cmd: portid=%d, queueid=%d, weight=%d\n", (unsigned int)pdata->qos_wfq_info.portid, (unsigned int)pdata->qos_wfq_info.queueid, (unsigned int)pdata->qos_wfq_info.weight);

    return QOS_CMD_OK;
}
/*qoscmd setwfq ---end*/

/*qoscmd resetwfq ---begin*/
static void qos_reset_wfq_help( int summary)
{
    IFX_QOSCMD_PRINT("resetwfq [-p portid] [-i ifname] <-q queuid> \n");
    if( summary )
        IFX_QOSCMD_PRINT("note: one or only one of portid and ifname must be specified\n");
    return;
}
static const char qos_reset_wfq_short_opts[] = "p:q:i:h";
static const struct option qos_reset_wfq_long_opts[] =
{
    {"portid",   required_argument,  NULL, 'p'},
    {"ifname",   required_argument,  NULL, 'i'},
    {"queuid",   required_argument,  NULL, 'q'},
    { 0,0,0,0 }
};
static int qos_parse_reset_wfq_cmd(QOS_CMD_OPTS *popts, QOS_MGR_CMD_DATA *pdata)
{
    unsigned int q_opt=0;
    int p_opt=0, i_opt=0;
    uint32_t res;
    qos_memset( pdata, sizeof(QOS_MGR_CMD_WFQ_INFO), 0 );

    while(popts->opt)
    {
        switch(popts->opt)
        {
        case 'p':
		res = str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL);
		if (res == QOS_CMD_ERR){
			IFX_QOSCMD_PRINT("ERROR: invalid input\n");
			return res;
		}
		pdata->qos_wfq_info.portid = res;
		p_opt ++;
		break;
        case 'i':
            if( get_portid(popts->optarg) >= 0 )
            {
                pdata->qos_wfq_info.portid = get_portid(popts->optarg);
                i_opt ++;
            }
            else
            {
                IFX_QOSCMD_PRINT("The portid of %s is not exist.\n", popts->optarg);
                return QOS_CMD_ERR;
            }
            break;
        case 'q':
		res = str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL);
		if (res == QOS_CMD_ERR){
			IFX_QOSCMD_PRINT("ERROR: invalid input\n");
			return res;
		}
		pdata->qos_wfq_info.queueid = res;
		q_opt = 1;
		break;
        default:
            IFX_QOSCMD_PRINT("qos_parse_reset_wfq_cmd not support parameter -%c \n", popts->opt);
            return QOS_CMD_ERR;
        }
        popts++;
    }

    if( q_opt == 0 )
    {
        IFX_QOSCMD_PRINT("Need enter queueid\n");
        return QOS_CMD_ERR;
    }
    if( p_opt + i_opt != 1)
    {
        IFX_QOSCMD_PRINT("note: one or only one of portid and ifname must be specified\n");
        return QOS_CMD_ERR;
    }

    IFX_QOSCMD_DBG("qos_parse_reset_wfq_cmd: portid=%d, queueid=%d\n", (unsigned int)pdata->qos_wfq_info.portid, (unsigned int)pdata->qos_wfq_info.queueid);

    return QOS_CMD_OK;
}
/*qoscmd resetwfq ---end*/

/*qoscmd getwfq ---begin*/
static void qos_get_wfq_help( int summary)
{
    IFX_QOSCMD_PRINT("getwfq [-p portid] [-i ifname] <-q queuid> [-o outfile]\n");
    if( summary )
    {
        IFX_QOSCMD_PRINT("note: one or only one of portid and ifname must be specified\n");
        IFX_QOSCMD_PRINT("    if queueid is not provided, then it will get all queue's wfq\n");
    }
    return;
}
static const char qos_get_wfq_short_opts[] = "p:q:o:i:h";
static const struct option qos_get_wfq_long_opts[] =
{
    {"portid",   required_argument,  NULL, 'p'},
    {"ifname",   required_argument,  NULL, 'i'},
    {"queuid",   required_argument,  NULL, 'q'},
    {"save-to-file",   required_argument,  NULL, 'o'},
    { 0,0,0,0 }
};
static int qos_parse_get_wfq_cmd(QOS_CMD_OPTS *popts, QOS_MGR_CMD_DATA *pdata)
{
    unsigned int q_opt=0;
    int p_opt=0, i_opt=0;

    qos_memset( pdata, sizeof(QOS_MGR_CMD_WFQ_INFO), 0 );
    pdata->qos_wfq_info.queueid = -1;

    while(popts->opt)
    {
        switch(popts->opt)
        {
        case 'p':
            pdata->qos_wfq_info.portid = str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL);
            p_opt ++;
            break;
        case 'i':
            if( get_portid(popts->optarg) >= 0 )
            {
                pdata->qos_wfq_info.portid = get_portid(popts->optarg);
                i_opt ++;
            }
            else
            {
                IFX_QOSCMD_PRINT("The portid of %s is not exist.\n", popts->optarg);
                return QOS_CMD_ERR;
            }
            break;
        case 'q':
            pdata->qos_wfq_info.queueid = str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL);
            q_opt = 1;
            break;
        case  'o':
            g_output_to_file = 1;
            strncpy_s(g_output_filename,QOSCMD_MAX_FILENAME,popts->optarg,QOSCMD_MAX_FILENAME);
            break;

        default:
            return QOS_CMD_ERR;
        }
        popts++;
    }

    if( p_opt + i_opt != 1)
    {
        IFX_QOSCMD_PRINT("note: one or only one of portid and ifname must be specified\n");
        return QOS_CMD_ERR;
    }

    IFX_QOSCMD_DBG("qos_parse_get_wfq_cmd: portid=%d, queueid=%d\n", (unsigned int)pdata->qos_wfq_info.portid, (unsigned int)pdata->qos_wfq_info.queueid);

    return QOS_CMD_OK;
}

static int qos_get_wfq_do_cmd(QOS_COMMAND *pcmd, QOS_MGR_CMD_DATA *pdata)
{
    QOS_MGR_CMD_QUEUE_NUM_INFO qnum_info;
    QOS_MGR_CMD_WFQ_INFO  info;
    int i, start_i, end_i=0;
    int res;

    qos_memset( &qnum_info, sizeof(qnum_info), 0 );
    qnum_info.portid = pdata->qos_wfq_info.portid;
    if( (res = qos_do_ioctl_cmd(QOS_MGR_CMD_GET_QOS_QUEUE_MAX_NUM, &qnum_info ) != QOS_CMD_OK ) )
    {
        IFX_QOSCMD_PRINT("ioctl QOS_CMD_GET_QOS_QUEUE_MAX_NUM fail\n");
        return QOS_MGR_FAILURE;
    }

    if( pdata->qos_wfq_info.queueid != -1 ) // set queuid already, then use it
    {
        start_i = pdata->qos_wfq_info.queueid;
        if( start_i >= qnum_info.queue_num -1 )
            start_i = qnum_info.queue_num -1;
        end_i = start_i + 1;
        IFX_QOSCMD_DBG("Need to read wfq from queue %d \n", start_i);
    }
    else
    {
        start_i = 0;
        end_i = qnum_info.queue_num;
        IFX_QOSCMD_DBG("Need to read wfq from queue %d to %d\n", start_i, end_i-1 );
    }

    for(i=start_i; i<end_i; i++)
    {
        qos_memset( &info, sizeof(info), 0 );
        info.portid = pdata->qos_wfq_info.portid;
        info.queueid = i;

        if( (res = qos_do_ioctl_cmd(QOS_MGR_CMD_GET_QOS_WFQ, &info) == QOS_CMD_OK ) )
        {
            if( !g_output_to_file  )
            {
                IFX_QOSCMD_PRINT("  queue %2d wfq rate: %d\n", i, (unsigned int)info.weight );
            }
            else
            {
                /*note, currently only last valid flow info is saved to file and all other flow informations are all overwritten */
                SaveDataToFile(g_output_filename, (char *)(&info), sizeof(info) );
            }
        }
    }
    return QOS_MGR_SUCCESS;
}

/*qoscmd getwfq ---end*/

/*qoscmd setctrlrate ---begin*/
static void qos_set_ctrl_rate_help( int summary)
{
    IFX_QOSCMD_PRINT("setctrlrate [-p portid] [-i ifname] <-c enable | disable>\n");
    if( summary )
        IFX_QOSCMD_PRINT("note: one or only one of portid and ifname must be specified\n");
    return;
}
static const char qos_set_ctrl_rate_short_opts[] = "p:c:i:h";
static const struct option qos_set_ctrl_rate_long_opts[] =
{
    {"portid",  required_argument,  NULL, 'p'},
    {"ifname",   required_argument,  NULL, 'i'},
    {"control",   required_argument,  NULL, 'c'},
    { 0,0,0,0 }
};
static int qos_parse_set_ctrl_rate_cmd(QOS_CMD_OPTS *popts, QOS_MGR_CMD_DATA *pdata)
{
    unsigned int c_opt=0;
    int p_opt=0, i_opt=0;
    uint32_t res;
    qos_memset( pdata, sizeof(QOS_MGR_CMD_CTRL_INFO), 0 );

    while(popts->opt)
    {
        switch(popts->opt)
        {
        case 'p':
		res = str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL);
		if (res == QOS_CMD_ERR){
			IFX_QOSCMD_PRINT("ERROR: invalid input\n");
			return res;
		}
		pdata->qos_ctrl_info.portid = res;
		p_opt ++;
		break;
        case 'i':
            if( get_portid(popts->optarg) >= 0 )
            {
                pdata->qos_ctrl_info.portid = get_portid(popts->optarg);
                i_opt ++;
            }
            else
            {
                IFX_QOSCMD_PRINT("The portid of %s is not exist.\n", popts->optarg);
                return QOS_CMD_ERR;
            }
            break;
        case 'c':
		if( strcmp("enable" , popts->optarg) == 0 )
			pdata->qos_ctrl_info.enable = 1;
		else if( strcmp("enable" , popts->optarg) == 0 )
			pdata->qos_ctrl_info.enable = 0;
		else {
			IFX_QOSCMD_PRINT("ERROR: invalid input\n");
			return QOS_CMD_ERR;
		}
		c_opt = 1;
		break;
        default:
            return QOS_CMD_ERR;
        }
        popts++;
    }

    if( c_opt == 0 )
    {
        IFX_QOSCMD_PRINT("Need enter control option\n");
        return QOS_CMD_ERR;
    }
    if( p_opt + i_opt != 1)
    {
        IFX_QOSCMD_PRINT("note: one or only one of portid and ifname must be specified\n");
        return QOS_CMD_ERR;
    }

    IFX_QOSCMD_DBG("qos_parse_set_ctrl_rate_cmd: portid=%d, ctrl=%s\n", (unsigned int)pdata->qos_ctrl_info.portid, pdata->qos_ctrl_info.enable ? "enabled":"disable");

    return QOS_CMD_OK;
}
/*qoscmd setctrlrate ---end*/

/*qoscmd getctrlrate ---begin*/
static void qos_get_ctrl_rate_help( int summary)
{
    IFX_QOSCMD_PRINT("getctrlrate [-p portid] [-i ifname] [-o outfile]\n");
    if( summary )
        IFX_QOSCMD_PRINT("note: one or only one of portid and ifname must be specified\n");
    return;
}
static const char qos_get_ctrl_rate_short_opts[] = "p:i:h";
static const struct option qos_get_ctrl_rate_long_opts[] =
{
    {"portid",   required_argument,  NULL, 'p'},
    {"ifname",   required_argument,  NULL, 'i'},
    {"save-to-file",   required_argument,  NULL, 'o'},
    { 0,0,0,0 }
};
static int qos_parse_get_ctrl_rate_cmd(QOS_CMD_OPTS *popts, QOS_MGR_CMD_DATA *pdata)
{
    int p_opt=0, i_opt=0;

    qos_memset( pdata, sizeof(QOS_MGR_CMD_CTRL_INFO), 0 );

    while(popts->opt)
    {
        switch(popts->opt)
        {
        case 'p':
            pdata->qos_ctrl_info.portid = str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL);
            p_opt ++;
            break;
        case 'i':
            if( get_portid(popts->optarg) >= 0 )
            {
                pdata->qos_ctrl_info.portid = get_portid(popts->optarg);
                i_opt ++;
            }
            else
            {
                IFX_QOSCMD_PRINT("The portid of %s is not exist.\n", popts->optarg);
                return QOS_CMD_ERR;
            }
            break;
        case  'o':
            g_output_to_file = 1;
            strncpy_s(g_output_filename,QOSCMD_MAX_FILENAME,popts->optarg,QOSCMD_MAX_FILENAME);
            break;

        default:
            return QOS_CMD_ERR;
        }
        popts++;
    }
    if( p_opt + i_opt != 1)
    {
        IFX_QOSCMD_PRINT("note: one or only one of portid and ifname must be specified\n");
        return QOS_CMD_ERR;
    }
    IFX_QOSCMD_DBG("qos_parse_get_ctrl_rate_cmd: portid=%d\n", (unsigned int)pdata->qos_ctrl_info.portid);

    return QOS_CMD_OK;
}
static void qos_print_get_ctrl_rate_cmd(QOS_MGR_CMD_DATA *pdata)
{
    if( !g_output_to_file )
    {
        IFX_QOSCMD_PRINT("The rate of port id %d is %s\n", (unsigned int)pdata->qos_ctrl_info.portid, pdata->qos_ctrl_info.enable ? "enabled":"disabled");
    }
    else
    {
        SaveDataToFile(g_output_filename, (char *)(&pdata->qos_ctrl_info), sizeof(pdata->qos_ctrl_info) );
    }
}
/*qoscmd getctrlrate ---end*/

/*qoscmd setrate ---begin*/
static void qos_set_rate_help( int summary)
{
    IFX_QOSCMD_PRINT("setrate [-p portid] [-i ifname] [-q queuid | -g gamma_itf_id ] [-s shaperid | -r rate ] <-b burst>\n");
    IFX_QOSCMD_PRINT("set '-q = -1' for setting Port Rate Shaper \n");
    if( summary )
        IFX_QOSCMD_PRINT("note: one or only one of portid and ifname must be specified\n");
    return;
}
static const char qos_set_rate_short_opts[] = "p:i:q:g:s:r:b:h";

static const struct option qos_set_rate_long_opts[] =
{
    {"portid",   required_argument,  NULL,   'p'},
    {"ifname",   required_argument,  NULL,   'i'},
    {"queuid",   required_argument,  NULL,   'q'},
	{"gammaitf", required_argument,  NULL,   'g'},
    {"shaperid", required_argument,  NULL,	 's'},
    {"rate",     required_argument,  NULL,   'r'},
    {"burst",    required_argument,  NULL,   'b'},
    { 0,0,0,0 }
};

static int qos_set_rate_cmd(QOS_CMD_OPTS *popts, QOS_MGR_CMD_DATA*pdata)
{
    unsigned int r_opt=0, q_opt=0;
    unsigned int s_opt=0;
    int p_opt=0, i_opt=0, res = 0;
    qos_memset( pdata, sizeof(QOS_MGR_CMD_RATE_INFO), 0 );

    q_opt = 1;  //by default if no queue id is specified, it will be regarded as port based rate shaping.
    pdata->qos_rate_info.queueid = -1;
    pdata->qos_rate_info.shaperid = -1;
    while(popts->opt)
    {
        switch(popts->opt)
        {
        case 'p':
		res = str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL);
		if (res == QOS_CMD_ERR){
			IFX_QOSCMD_PRINT("ERROR: invalid input\n");
			return res;
		}
		pdata->qos_rate_info.portid = res;
		p_opt ++;
		break;
        case 'i':
	    strcpy_s(pdata->qos_rate_info.ifname,QOS_MGR_IF_NAME_SIZE,(char *)popts->optarg);
            if( get_portid(popts->optarg) >= 0 )
            {
                pdata->qos_rate_info.portid = get_portid(popts->optarg);
                i_opt ++;
            }
            else
            {
                IFX_QOSCMD_PRINT("The portid of %s is not exist.\n", popts->optarg);
                return QOS_CMD_ERR;
            }
            break;
        case 'q':
		res = str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL);
		if (res == QOS_CMD_ERR){
			IFX_QOSCMD_PRINT("ERROR: invalid input\n");
			return res;
		}
		pdata->qos_rate_info.queueid = res;
		q_opt ++;
		break;
	case 'g':
		res = str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL);
		if (res == QOS_CMD_ERR){
			IFX_QOSCMD_PRINT("ERROR: invalid input\n");
			return res;
		}
		pdata->qos_rate_info.queueid = res;
		pdata->qos_rate_info.queueid = ~ pdata->qos_rate_info.queueid; //if queueid is bigger than max allowed queueid, it is regarded as port id
		q_opt ++;
		break;
        case 's':
		res = str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL);
		if (res == QOS_CMD_ERR){
			IFX_QOSCMD_PRINT("ERROR: invalid input\n");
			return res;
		}
		pdata->qos_rate_info.shaperid = res;
		s_opt ++;
		break;
        case 'r':
		res = str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL);
		if (res == QOS_CMD_ERR){
			IFX_QOSCMD_PRINT("ERROR: invalid input\n");
			return res;
		}
		pdata->qos_rate_info.rate    = res;
		r_opt = 1;
		break;
        case 'b':
		res = str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL);
		if (res == QOS_CMD_ERR){
			IFX_QOSCMD_PRINT("ERROR: invalid input\n");
			return res;
		}
		pdata->qos_rate_info.burst   = res;
		r_opt = 1;
		break;
        default:
            return QOS_CMD_ERR;
        }
        popts++;
    }

    if( q_opt > 2 )
    {
        IFX_QOSCMD_PRINT("Queue id and gamma interface id cannot both be set id\n");
        return QOS_CMD_ERR;
    }
	if( p_opt + i_opt != 1)
    {
        IFX_QOSCMD_PRINT("note: one or only one of portid and ifname must be specified\n");
        return QOS_CMD_ERR;
    }   

    if( (s_opt == 1) && (r_opt == 1) )
    {
        IFX_QOSCMD_PRINT(" Both Shaper id and Rate cannot be set\n");
        return QOS_CMD_ERR;
    }
    
    if( (s_opt == 0) && (r_opt == 0) )
    {
        IFX_QOSCMD_PRINT("Either Shaper id or Rate should be set\n");
        return QOS_CMD_ERR;
    }

    IFX_QOSCMD_DBG("qos_set_rate_cmd: portid=%d, queueid=%d,rate=%d burst=%d\n", 
	         (unsigned int)pdata->qos_rate_info.portid, 
			 (unsigned int)pdata->qos_rate_info.queueid, 
			 (unsigned int)pdata->qos_rate_info.rate, 
			 (unsigned int)pdata->qos_rate_info.burst);

    return QOS_CMD_OK;
}
/*qoscmd setrate ---end*/

/*qoscmd resetrate ---begin*/
static void qos_reset_rate_help( int summary)
{
    IFX_QOSCMD_PRINT("resetrate [-p portid] [-i ifname] [ <-q queuid> | -g <gamma interface id> ] \n");
    IFX_QOSCMD_PRINT("Use '-q = -1' to reset port rate shaper \n");
    if( summary )
        IFX_QOSCMD_PRINT("note: one or only one of portid and ifname must be specified\n");
    return;
}
static const char qos_reset_rate_short_opts[] = "p:q:i:h";
static const struct option qos_reset_rate_long_opts[] =
{
    {"portid",   required_argument,  NULL, 'p'},
    {"ifname",   required_argument,  NULL, 'i'},
    {"queuid",   required_argument,  NULL, 'q'},
    { 0,0,0,0 }
};
static int qos_parse_reset_rate_cmd(QOS_CMD_OPTS *popts, QOS_MGR_CMD_DATA *pdata)
{
    unsigned int q_opt=0;
    int p_opt=0, i_opt=0, res = 0;
    qos_memset( pdata, sizeof(QOS_MGR_CMD_RATE_INFO), 0 );
    pdata->qos_rate_info.burst = -1;

    while(popts->opt)
    {
        switch(popts->opt)
        {
        case 'p':
		res = str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL);
		if (res == QOS_CMD_ERR){
			IFX_QOSCMD_PRINT("ERROR: invalid input\n");
			return res;
		}
		pdata->qos_rate_info.portid = res;
		p_opt ++;
		break;
        case 'i':
	    strcpy_s(pdata->qos_rate_info.ifname,QOS_MGR_IF_NAME_SIZE,(char *)popts->optarg);
            if( get_portid(popts->optarg) >= 0 )
            {
                pdata->qos_rate_info.portid = get_portid(popts->optarg);
                i_opt ++;
            }
            else
            {
                IFX_QOSCMD_PRINT("The portid of %s is not exist.\n", popts->optarg);
                return QOS_CMD_ERR;
            }
            break;
        case 'q':
		res = str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL);
		if (res == QOS_CMD_ERR){
			IFX_QOSCMD_PRINT("ERROR: invalid input\n");
			return res;
		}
		pdata->qos_rate_info.queueid = res;
		res = str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL);
		if (res == QOS_CMD_ERR){
			IFX_QOSCMD_PRINT("ERROR: invalid input\n");
			return res;
		}
		pdata->qos_rate_info.shaperid = res;
		pdata->qos_rate_info.shaper.enable = 1;
		q_opt ++ ;
		break;
            
        case 'g':
		res = str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL);
		if (res == QOS_CMD_ERR){
			IFX_QOSCMD_PRINT("ERROR: invalid input\n");
			return res;
		}
		pdata->qos_rate_info.queueid = res;
		pdata->qos_rate_info.queueid = ~ pdata->qos_rate_info.queueid; //if queueid is bigger than max allowed queueid, it is regarded as port id
		q_opt ++;
		break;
        default:
            IFX_QOSCMD_PRINT("qos_parse_reset_rate_cmd not support parameter -%c \n", popts->opt);
            return QOS_CMD_ERR;
        }
        popts++;
    }

    if( q_opt > 2 )
    {
        IFX_QOSCMD_PRINT("Queue id and gamma interface id cannot both be set id\n");
        return QOS_CMD_ERR;
    }

    if( q_opt == 0 )
    {
        IFX_QOSCMD_PRINT("Need enter queueid\n");
        return QOS_CMD_ERR;
    }
    if( p_opt + i_opt != 1)
    {
        IFX_QOSCMD_PRINT("note: one or only one of portid and ifname must be specified\n");
        return QOS_CMD_ERR;
    }

    IFX_QOSCMD_DBG("qos_parse_reset_rate_cmd: portid=%d, queueid=%d\n", (unsigned int)pdata->qos_rate_info.portid, (unsigned int)pdata->qos_rate_info.queueid);

    return QOS_CMD_OK;
}
/*qoscmd resetrate ---end*/

/*qoscmd getrate ---begin*/
static void qos_get_rate_help( int summary)
{
    IFX_QOSCMD_PRINT("getrate [-p portid] [-i ifname] <-q queuid> [-o outfile]\n");
    if( summary )
    {
        IFX_QOSCMD_PRINT("note: one or only one of portid and ifname must be specified\n");
        IFX_QOSCMD_PRINT("    if queueid is not provided, then get all queue's rate information\n");
    }
    return;
}
static const char qos_get_rate_short_opts[] = "p:q:o:i:h";
static const struct option qos_get_rate_long_opts[] =
{
    {"portid",   required_argument,  NULL, 'p'},
    {"ifname",   required_argument,  NULL, 'i'},
    {"queuid",   required_argument,  NULL, 'q'},
    {"save-to-file",   required_argument,  NULL, 'o'},
    { 0,0,0,0 }
};
static int qos_parse_get_rate_cmd(QOS_CMD_OPTS *popts, QOS_MGR_CMD_DATA *pdata)
{
    unsigned int q_opt=0;
    int p_opt=0, i_opt=0;
    qos_memset( pdata, sizeof(QOS_MGR_CMD_RATE_INFO), 0 );
    pdata->qos_rate_info.queueid = -1;

    while(popts->opt)
    {
        switch(popts->opt)
        {
        case 'p':
            pdata->qos_rate_info.portid = str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL);
            p_opt ++;
            break;
        case 'i':
            if( get_portid(popts->optarg) >= 0 )
            {
                pdata->qos_rate_info.portid = get_portid(popts->optarg);
                i_opt ++;
            }
            else
            {
                IFX_QOSCMD_PRINT("The portid of %s is not exist.\n", popts->optarg);
                return QOS_CMD_ERR;
            }
            break;
        case 'q':
            pdata->qos_rate_info.queueid = str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL);
            q_opt = 1;
            break;
        case 'o':
            g_output_to_file = 1;
            strncpy_s(g_output_filename,QOSCMD_MAX_FILENAME,popts->optarg,QOSCMD_MAX_FILENAME);
            break;

        default:
            IFX_QOSCMD_PRINT("qos_parse_get_rate_cmd not support parameter -%c \n", popts->opt);
            return QOS_CMD_ERR;
        }
        popts++;
    }

    if( p_opt + i_opt != 1)
    {
        IFX_QOSCMD_PRINT("note: one or only one of portid and ifname must be specified\n");
        return QOS_CMD_ERR;
    }

    IFX_QOSCMD_DBG("qos_parse_get_rate_cmd: portid=%d, queueid=%d\n", (unsigned int)pdata->qos_rate_info.portid, (unsigned int)pdata->qos_rate_info.queueid);

    return QOS_CMD_OK;
}

static int qos_get_rate_do_cmd(QOS_COMMAND *pcmd, QOS_MGR_CMD_DATA *pdata)
{
    QOS_MGR_CMD_QUEUE_NUM_INFO qnum_info;
    QOS_MGR_CMD_RATE_INFO  info;
    int i=0, j=0, start_i, end_i=0;
    int res;

    qos_memset( &qnum_info, sizeof(qnum_info), 0 );
    qnum_info.portid = pdata->qos_rate_info.portid;
    if( (res = qos_do_ioctl_cmd(QOS_MGR_CMD_GET_QOS_QUEUE_MAX_NUM, &qnum_info ) != QOS_CMD_OK ) )
    {
        IFX_QOSCMD_PRINT("ioctl QOS_MGR_CMD_GET_QOS_QUEUE_MAX_NUM fail\n");
        return QOS_MGR_FAILURE;
    }

    if( pdata->qos_rate_info.queueid!= -1 ) // set index already, then use it
    {
        qos_memset( &info, 0, sizeof(info) );
        info.portid = pdata->qos_rate_info.portid;
        info.queueid = pdata->qos_rate_info.queueid;

        if( (res = qos_do_ioctl_cmd(QOS_MGR_CMD_GET_QOS_RATE, &info) == QOS_CMD_OK ) )
        {
            if( !g_output_to_file  )
            {
                IFX_QOSCMD_PRINT("              Rate           Burst      ShaperId\n");

                if( pdata->qos_rate_info.queueid < qnum_info.queue_num )
                    IFX_QOSCMD_PRINT("   queue %2d:  %08d(kbps) 0x%04d       %d\n", (unsigned int)info.queueid, (unsigned int)info.rate, (unsigned int)info.burst, (unsigned int)info.shaperid );
                else
                    IFX_QOSCMD_PRINT("   port    :  %08d(kbps) 0x%04d\n", (unsigned int)info.rate, (unsigned int)info.burst );
            }
            else
            {
                /*note, currently only last valid flow info is saved to file and all other flow informations are all overwritten */
                SaveDataToFile(g_output_filename, (char *)(&info), sizeof(info) );
            }
            j++;
        }
    }
    else
    {
        start_i = 0;
        end_i = qnum_info.queue_num;
        IFX_QOSCMD_DBG("Need to read rate shaping from queue %d to %d\n", start_i, end_i-1 );

        for(i=start_i; i<end_i; i++)
        {
            qos_memset( &info, sizeof(info), 0 );
            info.portid = pdata->qos_rate_info.portid;
            info.queueid = i;

            if( (res = qos_do_ioctl_cmd(QOS_MGR_CMD_GET_QOS_RATE, &info) == QOS_CMD_OK ) )
            {
                if( !g_output_to_file  )
                {
                    if( i == start_i )
                        IFX_QOSCMD_PRINT("              Rate          Burst      ShaperId\n");
                        IFX_QOSCMD_PRINT("   queue %2d:  %07d(kbps) 0x%04d      %d\n", i, (unsigned int)info.rate, (unsigned int)info.burst, (unsigned int)info.shaperid );
                }
                else
                {
                    /*note, currently only last valid flow info is saved to file and all other flow informations are all overwritten */
                    SaveDataToFile(g_output_filename, (char *)(&info), sizeof(info) );
                }
                j++;
            }
        }

        if( pdata->qos_rate_info.queueid == -1 )
        {
            qos_memset( &info, sizeof(info),0 );
            info.portid = pdata->qos_rate_info.portid;
            info.queueid = qnum_info.queue_num;

            if( (res = qos_do_ioctl_cmd(QOS_MGR_CMD_GET_QOS_RATE, &info) == QOS_CMD_OK ) )
            {
                if( !g_output_to_file  )
                {
                    IFX_QOSCMD_PRINT("   port   :   %07d(kbps) 0x%04d\n", (unsigned int)info.rate, (unsigned int)info.burst );
                }
                else
                {
                    /*note, currently only last valid flow info is saved to file and all other flow informations are all overwritten */
                    SaveDataToFile(g_output_filename, (char *)(&info), sizeof(info) );
                }
                j++;
            }
        }
    }




    return QOS_MGR_SUCCESS;
}

/*qoscmd setshaper ---begin*/
static void qos_set_shaper_help( int summary)
{
    IFX_QOSCMD_PRINT("setshaper <-i ifname> <-s shaperid> <-r rate/PIR> <-c rate/CIR> <-b burst/PBS/CBS> <-m shapermode>\n");
    IFX_QOSCMD_PRINT("      Use '-s = -1' to create Port rate shaper instance\n");
    if( summary )
    {
        IFX_QOSCMD_PRINT("note: shaperid and rate must be specified\n");
        IFX_QOSCMD_PRINT("      option -s: Shaper id, valid range -1 to 7\n");
    }
    return;
}
static const char qos_set_shaper_short_opts[] = "i:s:r:c:b:m";
static const struct option qos_set_shaper_long_opts[] =
{
    {"ifname", required_argument,  NULL,   'i'},
    {"shaperid", required_argument,  NULL,   's'},
    {"rate",     required_argument,  NULL,   'r'},
    {"commitrate",     required_argument,  NULL,   'c'},
    {"burst",    required_argument,  NULL,   'b'},
    {"shapermode",     required_argument,  NULL,   'm'},
    { 0,0,0,0 }
};

static int qos_set_shaper_cmd(QOS_CMD_OPTS *popts, QOS_MGR_CMD_DATA*pdata)
{
    unsigned int r_opt=0, s_opt=0,b_opt=0;
    int res = 0;

    qos_memset( pdata, sizeof(QOS_MGR_CMD_RATE_INFO), 0 );
    while(popts->opt)
    {
        switch(popts->opt)
        {
        case 'i':
	    strcpy_s(pdata->qos_rate_info.ifname,QOS_MGR_IF_NAME_SIZE,(char *)popts->optarg);
            pdata->qos_rate_info.portid = 15;
	    break;
        case 's':
		res = str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL);
		if (res == QOS_CMD_ERR){
			IFX_QOSCMD_PRINT("ERROR: invalid input\n");
			return res;
		}
		pdata->qos_rate_info.shaperid = res;
		pdata->qos_rate_info.shaper.enable = 1;
		s_opt ++;
		break;
        case 'r':
		res = str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL);
		if (res == QOS_CMD_ERR){
			IFX_QOSCMD_PRINT("ERROR: invalid input\n");
			return res;
		}
		pdata->qos_rate_info.rate = pdata->qos_rate_info.shaper.pir   = res;
		r_opt = 1;
		break;
        case 'c':
		res = str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL);
		if (res == QOS_CMD_ERR){
			IFX_QOSCMD_PRINT("ERROR: invalid input\n");
			return res;
		}
		pdata->qos_rate_info.shaper.cir   = res;
		r_opt = 1;
		break;
        case 'b':
		res = str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL);
		if (res == QOS_CMD_ERR){
			IFX_QOSCMD_PRINT("ERROR: invalid input\n");
			return res;
		}
		pdata->qos_rate_info.burst = pdata->qos_rate_info.shaper.pbs = pdata->qos_rate_info.shaper.cbs = res;
		b_opt = 1;
		break;
        case 'm':
		res = str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL);
		if (res == QOS_CMD_ERR){
			IFX_QOSCMD_PRINT("ERROR: invalid input\n");
			return res;
		}
		pdata->qos_rate_info.shaper.mode  = res;
		break;
        default:
            return QOS_CMD_ERR;
        }
        popts++;
    }

    if( r_opt == 0 )
    {
        IFX_QOSCMD_PRINT("Need to enter rate\n");
        return QOS_CMD_ERR;
    }
        
    if( s_opt == 0 ) 
    {
        IFX_QOSCMD_PRINT("Shaper id is required\n");
        return QOS_CMD_ERR;
    }
    IFX_QOSCMD_DBG("qos_set_shaper_cmd: shaperid=%d,rate=%d burst=%d\n", 
	       (unsigned int)pdata->qos_rate_info.shaperid, 
			 (unsigned int)pdata->qos_rate_info.rate, 
			 (unsigned int)pdata->qos_rate_info.burst);

    return QOS_CMD_OK;
}
/*qoscmd setshaper ---end*/

/*qoscmd getshaper ---begin*/
static void qos_get_shaper_help( int summary)
{
    IFX_QOSCMD_PRINT("getshaper <-s shaperid> [-o outfile]\n");
    if( summary )
    {
        IFX_QOSCMD_PRINT("note: Shaper id must be specified\n");
        IFX_QOSCMD_PRINT("      option -s: Shaper id, valid range 0 to 7\n");
    }
    return;
}
static const char qos_get_shaper_short_opts[] = "s:o";
static const struct option qos_get_shaper_long_opts[] =
{
    {"shaperid",   required_argument,  NULL, 's'},
    {"save-to-file",   required_argument,  NULL, 'o'},
    { 0,0,0,0 }
};
static int qos_parse_get_shaper_cmd(QOS_CMD_OPTS *popts, QOS_MGR_CMD_DATA *pdata)
{
    unsigned int s_opt=0;
    qos_memset( pdata, sizeof(QOS_MGR_CMD_RATE_INFO), 0 );
    pdata->qos_rate_info.queueid = -1;

    while(popts->opt)
    {
        switch(popts->opt)
        {
        case 's':
            pdata->qos_rate_info.shaperid = str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL);
            s_opt = 1;
            break;
        case 'o':
            g_output_to_file = 1;
            strncpy_s(g_output_filename,QOSCMD_MAX_FILENAME,popts->optarg,QOSCMD_MAX_FILENAME);
            break;

        default:
            IFX_QOSCMD_PRINT("qos_parse_get_shaper_cmd not support parameter -%c \n", popts->opt);
            return QOS_CMD_ERR;
        }
        popts++;
    }

    if( s_opt != 1)
    {
        IFX_QOSCMD_PRINT("note: Shaper id must be specified\n");
        return QOS_CMD_ERR;
    }

    if( (pdata->qos_rate_info.shaperid < 0) || (pdata->qos_rate_info.shaperid > 7) ) 
    {
        IFX_QOSCMD_PRINT("Shaper id valid range is 0 to 7\n");
        return QOS_CMD_ERR;
    }

    IFX_QOSCMD_DBG("qos_parse_get_shaper_cmd: shaperid=%d\n", (unsigned int)pdata->qos_rate_info.shaperid );

    return QOS_CMD_OK;
}

static int qos_get_shaper_do_cmd(QOS_COMMAND *pcmd, QOS_MGR_CMD_DATA *pdata)
{
    QOS_MGR_CMD_RATE_INFO  info;
    int res;

    {
        qos_memset( &info, sizeof(info), 0 );
        info.shaperid = pdata->qos_rate_info.shaperid;

        if( (res = qos_do_ioctl_cmd(QOS_MGR_CMD_GET_QOS_SHAPER, &info) == QOS_CMD_OK ) )
        {
            if( !g_output_to_file  )
            {
                IFX_QOSCMD_PRINT("    ShaperId  Rate              Burst\n");

                IFX_QOSCMD_PRINT("     %d        %08d(kbps)    0x%04d\n", (unsigned int)info.shaperid, (unsigned int)info.rate, (unsigned int)info.burst );
            }
            else
            {
                /*note, currently only last valid flow info is saved to file and all other flow informations are all overwritten */
                SaveDataToFile(g_output_filename, (char *)(&info), sizeof(info) );
            }
        }
    }
    
    return QOS_MGR_SUCCESS;
}
/*qoscmd getshaper ---end*/

/*qoscmd getrate ---end*/


#endif

#ifdef CONFIG_SOC_GRX500
/*
===============================================================================
  Command : setgroup
  discription: Command for setting ingress group
===============================================================================
*/

static const char qos_setgroup_short_opts[] = "g:i:h";
static const struct option qos_setgroup_long_opts[] =
{
    {"ifname",  required_argument,  NULL, 'i'},
    {"group",  required_argument,  NULL, 'g'},
    {"help",   no_argument,  NULL, 'h'},
    { 0,0,0,0 }
};


static int qos_parse_setgroup_cmd(QOS_CMD_OPTS *popts,QOS_MGR_CMD_DATA *pdata)
{
    int i_flg = 0, g_flg = 0;
    uint32_t res;

    qos_memset(pdata, sizeof(QOS_MGR_CMD_INGGRP_INFO), 0);

    while(popts->opt)
    {
        switch(popts->opt)
        {
        case 'i':
    	    strcpy_s(pdata->qos_inggrp_info.ifname,QOS_MGR_IF_NAME_SIZE, popts->optarg);
            i_flg=1;
            break;
        case 'g':
		res = str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL);
		if (res == QOS_CMD_ERR){
			IFX_QOSCMD_PRINT("ERROR: invalid input\n");
			return res;
		}
		pdata->qos_inggrp_info.ingress_group = res;
		if (pdata->qos_inggrp_info.ingress_group < QOS_MGR_INGGRP0 || pdata->qos_inggrp_info.ingress_group >= QOS_MGR_INGGRP_MAX)
		{
			IFX_QOSCMD_PRINT("Invalid Group %d (allowed [%d])\n", QOS_MGR_INGGRP0, QOS_MGR_INGGRP_MAX - 1);
			return QOS_CMD_ERR;
		}
		g_flg=1;
		break;
        case 'h':
            return QOS_CMD_HELP;
        default:
            IFX_QOSCMD_PRINT("qos_parse_setgroup_cmd not support parameter -%c \n", popts->opt);
            return QOS_CMD_ERR;
        }
        popts++;
    }

    if(!(i_flg && g_flg))
    {
        IFX_QOSCMD_PRINT("Manditory option missing:\n qoscmd setgroup [-i ifname] | [-g Ingress group]\n");
        return QOS_CMD_ERR;
    }

    IFX_QOSCMD_DBG("qos_parse_setgroup_cmd: ifname=%s, group=%d\n", pdata->qos_inggrp_info.ifname, (unsigned int)pdata->qos_inggrp_info.ingress_group); 
    return QOS_CMD_OK;
}

static void qos_setgroup_help(int summary)
{
    IFX_QOSCMD_PRINT("setgroup -i <ifname> -g <Ingress Group>\n");
    IFX_QOSCMD_PRINT("  Ingress Group: INGGRP%d:INGGRP%d\n", QOS_MGR_INGGRP0, QOS_MGR_INGGRP_MAX - 1);
}
#endif //QOS ingress grouping

/*
===============================================================================
  Command : addque
  discription: Command for Queue Creation
===============================================================================
*/

static const char qos_addque_short_opts[] = "p:q:i:l:c:w:f:t:v:T:d:h";
static const struct option qos_addque_long_opts[] =
{
    {"portid",  required_argument,  NULL, 'p'},
    {"que_num",   required_argument,  NULL, 'q'},
    {"ifname",  required_argument,  NULL, 'i'},
    {"priority",  required_argument,  NULL, 'l'},
    {"drop_type",  required_argument,  NULL, 'c'},
    {"weight",  required_argument,  NULL, 'w'},
    {"tc",  required_argument,  NULL, 't'},
    {"intfid",  required_argument,  NULL, 'v'},
    {"flag",  required_argument,  NULL, 'f'},
    {"Type",  required_argument,  NULL, 'T'},
    {"defque",  required_argument,  NULL, 'd'},
    {"help",   no_argument,  NULL, 'h'},
    { 0,0,0,0 }
};

static int qos_parse_addque_cmd(QOS_CMD_OPTS *popts,QOS_MGR_CMD_DATA *pdata)
{
    int p_flg=0, q_flg=0, i_flg=0, i ;
    uint32_t res;
    char *str;
    qos_memset( pdata, sizeof(QOS_MGR_CMD_QUEUE_INFO), 0 );
    while(popts->opt)
    {
        switch(popts->opt)
        {
        case 'p':
		res = str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL);
		if (res == QOS_CMD_ERR){
			IFX_QOSCMD_PRINT("ERROR: invalid input\n");
			return res;
		}

		pdata->qos_queue_info.portid = res;
		p_flg=1;
		break;
        case 'q':
		res = str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL);
		if (res == QOS_CMD_ERR){
			IFX_QOSCMD_PRINT("ERROR: invalid input\n");
			return res;
		}
		pdata->qos_queue_info.queue_num = res;
		pdata->qos_queue_info.shaper.enable = 1;
		q_flg=1;
		break;
        case 'i':
    	    strcpy_s( pdata->qos_queue_info.ifname,QOS_MGR_IF_NAME_SIZE, popts->optarg );
	    i_flg=1;
	    break;
	case 'l':
		res = str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL);
		if (res == QOS_CMD_ERR){
			IFX_QOSCMD_PRINT("ERROR: invalid input\n");
			return res;
		}
		pdata->qos_queue_info.priority = res;
		if (pdata->qos_queue_info.priority < 0 || pdata->qos_queue_info.priority > 15 ) {
			IFX_QOSCMD_PRINT("ERROR: invalid level should be 0-15\n");
			return QOS_CMD_ERR;
		}
		break;
	case 'w':
		res = str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL);
		if (res == QOS_CMD_ERR){
			IFX_QOSCMD_PRINT("ERROR: invalid input\n");
			return res;
		}
		pdata->qos_queue_info.weight = res;
		pdata->qos_queue_info.sched = QOS_MGR_SCHED_WFQ; 
		if (pdata->qos_queue_info.weight < 1 || pdata->qos_queue_info.weight > 100 ) {
			IFX_QOSCMD_PRINT("ERROR: Invalid weight Should be 1-100\n");
			return QOS_CMD_ERR;
		}
	    break;

        case 'c':
		if (str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL) == 1) {
			pdata->qos_queue_info.drop.mode = QOS_MGR_DROP_CODEL;
		}
		break;

	case 't':
	    i = 0;
	    pdata->qos_queue_info.tc_map[i++] = str_convert(STRING_TYPE_INTEGER, strtok(popts->optarg, ","), NULL);
	    str = strtok(NULL, ",");
	    while( str ){
		pdata->qos_queue_info.tc_map[i++] = str_convert(STRING_TYPE_INTEGER, str, NULL);
		if(i >= MAX_TC_NUM)
		{
			IFX_QOSCMD_PRINT("Error: Tc count= %d <= Max Tc count=%d\n ", i, MAX_TC_NUM);
			return QOS_CMD_ERR;
		}
	    str = strtok(NULL, ",");
	    }
	    pdata->qos_queue_info.tc_no = i ;
           break;
        case 'v':
		res = str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL);
		if (res == QOS_CMD_ERR){
			IFX_QOSCMD_PRINT("ERROR: invalid input\n");
			return res;
		}
		pdata->qos_queue_info.flowId = res;
		break;
	case 'T':
	    if (QOS_MGR_Q_F_INGRESS == str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL))
	            pdata->qos_queue_info.flags |= QOS_MGR_Q_F_INGRESS;
	    else {
		IFX_QOSCMD_PRINT("ERROR: invalid input\n");
		return QOS_CMD_ERR;
	    }

            break;
        case 'd':
	    if (str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL))
	            pdata->qos_queue_info.flags |= QOS_MGR_Q_F_DEFAULT;
	    else{
		IFX_QOSCMD_PRINT("ERROR: invalid input\n");
		return QOS_CMD_ERR;
	    }
            break;
        case 'f':
            pdata->qos_queue_info.flags |= str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL);
	    break;
	case 'h':
	    return QOS_CMD_HELP;
        default:
            IFX_QOSCMD_PRINT("qos_parse_addque_cmd not support parameter -%c \n", popts->opt);
            return QOS_CMD_ERR;
        }
        popts++;
    }

    if( !(p_flg | i_flg ) & !q_flg)
    {
        IFX_QOSCMD_PRINT("Manditory option missing:\n qoscmd addque [-p portid] | [-i ifname] & [-q Qid]\n");
        return QOS_CMD_ERR;
    }
    IFX_QOSCMD_DBG("qos_parse_addque_cmd: portid=%d, queueid=%d\n", (unsigned int)pdata->qos_queue_info.portid, (unsigned int)pdata->qos_queue_info.queue_num);
    return QOS_CMD_OK;
}


static void qos_addque_help(int summary)
{
    IFX_QOSCMD_PRINT("addque {-p <port-id> -q <que-id> -i <ifname> -c <drop-type> -l <priority> -w <weight> -t <Tc> -v <FlowId> -T <Type>\n");
    IFX_QOSCMD_PRINT("  Type: 1 - INGRESS, 0 - EGRESS\n");
    IFX_QOSCMD_PRINT("  flags: 1 - Default Queue \n");
    IFX_QOSCMD_PRINT("  Drop Type: 1 - CODEL, 0 - WRED\n");
}

/*
===============================================================================
  Command : delque
  discription: Delete queue 
===============================================================================
*/

static const char qos_delque_short_opts[] = "i:q:p:T:h";
static const struct option qos_delque_long_opts[] =
{
    {"queid",   required_argument,  NULL, 'q'},
    {"ifname",   required_argument,  NULL, 'i'},
    {"portid",   required_argument,  NULL, 'p'},
    {"type",   required_argument,  NULL, 'T'},
    {"help",   no_argument,  NULL, 'h'},
    { 0,0,0,0 }
};

static int qos_parse_delque_cmd(QOS_CMD_OPTS *popts,QOS_MGR_CMD_DATA *pdata)
{
    qos_memset( pdata, sizeof(QOS_MGR_CMD_QUEUE_INFO),0 );

    while(popts->opt)
    {
        switch(popts->opt)
        {
        case 'q':
            pdata->qos_queue_info.queue_num = str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL);
            pdata->qos_queue_info.shaper_num = str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL);
            break;
        case 'p':
            pdata->qos_queue_info.portid = str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL);
            break;
        case 'i':
    	    strcpy_s( pdata->qos_queue_info.ifname,QOS_MGR_IF_NAME_SIZE,popts->optarg);
            break;
        case 'T':
            if (QOS_MGR_Q_F_INGRESS == str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL))
	            pdata->qos_queue_info.portid |= QOS_MGR_Q_F_INGRESS;
            break;
        case 'h':
            return QOS_CMD_HELP;
        default:
            IFX_QOSCMD_PRINT("qos_parse_delque_cmd not support parameter -%c \n", popts->opt);
            return QOS_CMD_ERR;
        }
        popts++;
    }

    IFX_QOSCMD_DBG("qos_parse_delque_cmd: queueid=%d\n", (unsigned int)pdata->qos_queue_info.queue_num);

    return QOS_CMD_OK;
}

static void qos_delque_help(int summary)
{
    IFX_QOSCMD_PRINT("delque -p <port-id> -q <que-id> -i <ifname> \n");
    return;
}

/*
===============================================================================
  Command : modsubif2port
  discription:
===============================================================================
*/

static const char qos_modsubif2port_short_opts[] = "i:p:l:w:f:h";
static const struct option qos_modsubif2port_long_opts[] =
{
    {"portid",  required_argument,  NULL, 'p'},
    {"ifname",  required_argument,  NULL, 'i'},
    {"priority",  required_argument,  NULL, 'l'},
    {"weight",  required_argument,  NULL, 'w'},
    {"flag",  required_argument,  NULL, 'f'},
    {"help",   no_argument,  NULL, 'h'},
    { 0,0,0,0 }
};

static int qos_parse_modsubif2port_cmd(QOS_CMD_OPTS *popts,QOS_MGR_CMD_DATA *pdata)
{
    int p_flg=0, l_flg=0, i_flg=0, w_flg=0;
    uint32_t res;
    qos_memset( pdata, sizeof(QOS_MGR_CMD_SUBIF_PORT_INFO), 0 );
    while(popts->opt)
    {
        switch(popts->opt)
        {
        case 'p':
		res = str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL);
		if (res == QOS_CMD_ERR){
			IFX_QOSCMD_PRINT("ERROR: invalid input\n");
			return res;
		}
		pdata->subif_port_info.port_id = res;
		p_flg=1;
		break;
        case 'i':
    	    strcpy_s( pdata->subif_port_info.ifname,QOS_MGR_IF_NAME_SIZE, popts->optarg );
	    i_flg=1;
	    break;
	case 'l':
		l_flg=1;
		res = str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL);
		if (res == QOS_CMD_ERR){
			IFX_QOSCMD_PRINT("ERROR: invalid input\n");
			return res;
		}
		pdata->subif_port_info.priority_level = res;
		if (pdata->subif_port_info.priority_level > 15 ) {
			IFX_QOSCMD_PRINT("ERROR: invalid level should be 0-15\n");
		return QOS_CMD_ERR;
		}
		break;
	case 'w':
		w_flg=1;
		res = str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL);
		if (res == QOS_CMD_ERR){
			IFX_QOSCMD_PRINT("ERROR: invalid input\n");
			return res;
		}
		pdata->subif_port_info.weight = res;
		if (pdata->subif_port_info.weight < 1 || pdata->subif_port_info.weight > 100 ) {
			IFX_QOSCMD_PRINT("ERROR: Invalid weight Should be 1-100\n");
			return QOS_CMD_ERR;
		}
	    break;
        case 'f':
		res = str_convert(STRING_TYPE_INTEGER, popts->optarg, NULL);
		if (res == QOS_CMD_ERR){
			IFX_QOSCMD_PRINT("ERROR: invalid input\n");
			return res;
		}
		pdata->qos_queue_info.flags = res;
		break;
	case 'h':
	    return QOS_CMD_HELP;
        default:
            IFX_QOSCMD_PRINT("qos_parse_subif_port_info_cmd not support parameter -%c \n", popts->opt);
            return QOS_CMD_ERR;
        }
        popts++;
    }

    if( !(p_flg | i_flg)  & (l_flg | w_flg))
    {
        IFX_QOSCMD_PRINT("Manditory option missing:\n qoscmd modsubif2port {[-p portid] | [-i ifname]} & {[-l priority] | [-w weight]}\n");
        return QOS_CMD_ERR;
    }
    return QOS_CMD_OK;
}


static void qos_modsubif2port_help(int summary)
{
    IFX_QOSCMD_PRINT("modsubif2port {-p <port-id> | -i <ifname>} & {-l <priority> | -w <weight>}\n");
    return;
}

/*
===============================================================================
  Command definitions
===============================================================================
*/

static QOS_COMMAND qos_cmd[] =
{

#ifdef CONFIG_PPA_QOS
    {
        "getqstatus",  // get maximum eth1 queue number
        QOS_MGR_CMD_GET_QOS_STATUS,
        qos_get_qstatus_help,
        qos_parse_get_qstatus_cmd,
        qos_get_qstatus_do_cmd,
        NULL,
        NULL,
        qos_get_qstatus_short_opts,
    },

    {
        "getqnum",  // get maximum eth1 queue number
        QOS_MGR_CMD_GET_QOS_QUEUE_MAX_NUM,
        qos_get_qnum_help,
        qos_parse_get_qnum_cmd,
        qos_do_cmd,
        qos_print_get_qnum_cmd,
        qos_get_qnum_long_opts,
        qos_get_qnum_short_opts,
    },
    {
        "getqmib",  // get maximum eth1 queue number
        QOS_MGR_CMD_GET_QOS_MIB,
        qos_get_qmib_help,
        qos_parse_get_qmib_cmd,
        qos_get_qmib_do_cmd,
        NULL,
        qos_get_qmib_long_opts,
        qos_get_qmib_short_opts,
    },
    {
        "addque",  // //addque qos
        QOS_MGR_CMD_ADD_QOS_QUEUE,
        qos_addque_help,
        qos_parse_addque_cmd,
        qos_do_cmd,
        NULL,
        qos_addque_long_opts,
        qos_addque_short_opts,
    },
    {
        "delque",  //delque qos
        QOS_MGR_CMD_DEL_QOS_QUEUE,
        qos_delque_help,
        qos_parse_delque_cmd,
        qos_do_cmd,
        NULL,
        qos_delque_long_opts,
        qos_delque_short_opts,
    },
    {
        "modsubif2port",  // modsubif2port qos
        QOS_MGR_CMD_MOD_SUBIF_PORT,
        qos_modsubif2port_help,
        qos_parse_modsubif2port_cmd,
        qos_do_cmd,
        NULL,
        qos_modsubif2port_long_opts,
        qos_modsubif2port_short_opts,
    },
#ifdef CONFIG_SOC_GRX500
    {
        "setgroup",  // //setgroup qos
        QOS_MGR_CMD_SET_QOS_INGGRP,
        qos_setgroup_help,
        qos_parse_setgroup_cmd,
        qos_do_cmd,
        NULL,
        qos_setgroup_long_opts,
        qos_setgroup_short_opts,
    },
#endif
    {
        "setctrlwfq",  //set wfq to enable/disable
        QOS_MGR_CMD_SET_CTRL_QOS_WFQ,
        qos_set_ctrl_wfq_help,
        qos_parse_set_ctrl_wfq_cmd,
        qos_do_cmd,
        NULL,
        qos_set_ctrl_wfq_long_opts,
        qos_set_ctrl_wfq_short_opts,
    },
    {
        "getctrlwfq",   //get  wfq control status---
        QOS_MGR_CMD_GET_CTRL_QOS_WFQ,
        qos_get_ctrl_wfq_help,
        qos_parse_get_ctrl_wfq_cmd,
        qos_do_cmd,
        qos_print_get_ctrl_wfq_cmd,
        qos_get_ctrl_wfq_long_opts,
        qos_get_ctrl_wfq_short_opts,
    },
    {
        "setwfq",  //set wfq weight
        QOS_MGR_CMD_SET_QOS_WFQ,
        qos_set_wfq_help,
        qos_parse_set_wfq_cmd,
        qos_do_cmd,
        NULL,
        qos_set_wfq_long_opts,
        qos_set_wfq_short_opts,
    },
    {
        "resetwfq",  //reset WFQ weight
        QOS_MGR_CMD_RESET_QOS_WFQ,
        qos_reset_wfq_help,
        qos_parse_reset_wfq_cmd,
        qos_do_cmd,
        NULL,
        qos_reset_wfq_long_opts,
        qos_reset_wfq_short_opts,
    },
    {
        "getwfq",   //get  WFQ weight
        QOS_MGR_CMD_GET_QOS_WFQ,
        qos_get_wfq_help,
        qos_parse_get_wfq_cmd,
        qos_get_wfq_do_cmd,
        NULL,
        qos_get_wfq_long_opts,
        qos_get_wfq_short_opts,
    },
    {
        "setctrlrate",  //set rate shaping to enable/disable
        QOS_MGR_CMD_SET_CTRL_QOS_RATE,
        qos_set_ctrl_rate_help,
        qos_parse_set_ctrl_rate_cmd,
        qos_do_cmd,
        NULL,
        qos_set_ctrl_rate_long_opts,
        qos_set_ctrl_rate_short_opts,
    },
    {
        "getctrlrate",   //get  rate shaping control status---
        QOS_MGR_CMD_GET_CTRL_QOS_RATE,
        qos_get_ctrl_rate_help,
        qos_parse_get_ctrl_rate_cmd,
        qos_do_cmd,
        qos_print_get_ctrl_rate_cmd,
        qos_get_ctrl_rate_long_opts,
        qos_get_ctrl_rate_short_opts,
    },
    {
        "setrate",  //set rate shaping
        QOS_MGR_CMD_SET_QOS_RATE,
        qos_set_rate_help,
        qos_set_rate_cmd,
        qos_do_cmd,
        NULL,
        qos_set_rate_long_opts,
        qos_set_rate_short_opts,
    },
    {
        "resetrate",  //reset rate shaping
        QOS_MGR_CMD_RESET_QOS_RATE,
        qos_reset_rate_help,
        qos_parse_reset_rate_cmd,
        qos_do_cmd,
        NULL,
        qos_reset_rate_long_opts,
        qos_reset_rate_short_opts,
    },
    {
        "getrate",   //get ate shaping
        QOS_MGR_CMD_GET_QOS_RATE,
        qos_get_rate_help,
        qos_parse_get_rate_cmd,
        qos_get_rate_do_cmd,
        NULL,
        qos_get_rate_long_opts,
        qos_get_rate_short_opts,
    },
    {
        "setshaper",  //set shaper
        QOS_MGR_CMD_SET_QOS_SHAPER,
        qos_set_shaper_help,
        qos_set_shaper_cmd,
        qos_do_cmd,
        NULL,
        qos_set_shaper_long_opts,
        qos_set_shaper_short_opts,
    },
    {
        "getshaper",   //get ate shaping
        QOS_MGR_CMD_GET_QOS_SHAPER,
        qos_get_shaper_help,
        qos_parse_get_shaper_cmd,
        qos_get_shaper_do_cmd,
        NULL,
        qos_get_shaper_long_opts,
        qos_get_shaper_short_opts,
    },
#endif //end if CONFIG_PPA_QOS

    { NULL, 0, NULL, NULL, NULL, NULL, NULL }
};

/*
====================================================================================
  command:   qos_cmd_help Function
  description: prints help text
  options:   argv
====================================================================================
*/
static void qos_print_help(void)
{
    QOS_COMMAND *pcmd;
    int i;

    IFX_QOSCMD_PRINT("Usage: %s <command> {options} \n", QOS_MGR_CMD_NAME);

    IFX_QOSCMD_PRINT("Commands: \n");
    for(pcmd = qos_cmd; pcmd->name != NULL; pcmd++)
    {
        if(pcmd->print_help)
        {
            //IFX_QOSCMD_PRINT(" "); //it will cause wrong alignment for hidden internal commands
            (*pcmd->print_help)(0);
        }
        else  if( pcmd->name[0] == '-' || pcmd->name[0] == ' ')
        {
#define MAX_CONSOLE_LINE_LEN 80
            int filling=strlen(pcmd->name)>=MAX_CONSOLE_LINE_LEN ? 0 : MAX_CONSOLE_LINE_LEN-strlen(pcmd->name);
            IFX_QOSCMD_PRINT("\n%s", pcmd->name);
            for(i=0; i<filling; i++ ) IFX_QOSCMD_PRINT("-");
            IFX_QOSCMD_PRINT("\n");
        }
    }

#if QOSCMD_DEBUG
    IFX_QOSCMD_PRINT("\n");
    IFX_QOSCMD_PRINT("* Note: Create a file %s will enable qoscmd debug mode\n", debug_enable_file );
    IFX_QOSCMD_PRINT("* Note: Any number inputs will be regarded as decial value without prefix 0x\n");
    IFX_QOSCMD_PRINT("* Note: Please run \"qoscmd <command name> -h\" to get its detail usage\n");
    IFX_QOSCMD_PRINT("\n");
#endif

    return;
}

/*
====================================================================================
  command:   qos_cmd_help Function
  description: prints help text
  options:   argv
====================================================================================
*/
static void qos_print_cmd_help(QOS_COMMAND *pcmd)
{
    if(pcmd->print_help)
    {
        IFX_QOSCMD_PRINT("Usage: %s ", QOS_MGR_CMD_NAME);
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
static int get_qos_cmd(char *cmd_str, QOS_COMMAND **pcmd)
{
    int i;

    // Locate the command where the name matches the cmd_str and return
    // the index in the command array.
    for (i = 0; qos_cmd[i].name; i++)
    {
        if (strcmp(cmd_str, qos_cmd[i].name) == 0)
        {
            *pcmd = &qos_cmd[i];
            return QOS_CMD_OK;
        }
    }
    return QOS_CMD_ERR;
}

/*
===========================================================================================


===========================================================================================
*/
static int qos_parse_cmd(int ac, char **av, QOS_COMMAND *pcmd, QOS_CMD_OPTS *popts)
{
    int opt, opt_idx, ret = QOS_CMD_OK;
    int num_opts;


    // Fill out the QOS_CMD_OPTS array with the option value and argument for
    // each option that is found. If option is help, display command help and
    // do not process command.
    for (num_opts = 0; num_opts < QOS_MAX_CMD_OPTS; num_opts++)
    {
        opt = getopt_long(ac - 1, av + 1, pcmd->short_opts, pcmd->long_opts, &opt_idx);
        if (opt != -1)
        {
            if (opt == 'h')        // help
            {
                ret = QOS_CMD_HELP;
                return ret;
            }
            else if (opt == '?')      // missing argument or invalid option
            {
                ret = QOS_CMD_ERR;
                break;
            }
            popts->opt  = opt;
            popts->optarg = optarg;
            popts++;
        }
        else
            break;
    }
    return ret;
}

/*
===========================================================================================


===========================================================================================
*/
static int qos_parse_cmd_line(int ac, char **av, QOS_COMMAND **pcmd, QOS_MGR_CMD_DATA **data)
{
    int ret = QOS_CMD_ERR;
    QOS_MGR_CMD_DATA *pdata = NULL;
    QOS_CMD_OPTS *popts = NULL;

    if ((ac <= 1) || (av == NULL))
    {
        return QOS_CMD_HELP;
    }

    pdata = malloc(sizeof(QOS_MGR_CMD_DATA));
    if (pdata == NULL)
        return QOS_CMD_NOT_AVAIL;
    qos_memset(pdata, sizeof(QOS_MGR_CMD_DATA), 0 );

    popts = malloc(sizeof(QOS_CMD_OPTS)*QOS_MAX_CMD_OPTS);
    if (popts == NULL)
    {
        free(pdata);
        return QOS_CMD_NOT_AVAIL;
    }
    qos_memset(popts, sizeof(QOS_CMD_OPTS)*QOS_MAX_CMD_OPTS , 0 );

    ret = get_qos_cmd(av[1], pcmd);
    if (ret == QOS_CMD_OK)
    {
        ret = qos_parse_cmd(ac, av, *pcmd, popts);
        if ( ret == QOS_CMD_OK )
        {
            ret = (*pcmd)->parse_options(popts,pdata);
            if ( ret == QOS_CMD_OK )
                *data = pdata;
        }
        else
        {
            IFX_QOSCMD_PRINT("Wrong parameter\n");
            ret = QOS_CMD_HELP;
        }
    }
    else
    {
        IFX_QOSCMD_PRINT("Unknown commands:  %s\n", av[1]);
    }
    free(popts);
    return ret;
}

/*
===========================================================================================


===========================================================================================
*/
int main(int argc, char** argv)
{
    int ret;
    QOS_MGR_CMD_DATA *pdata = NULL;
    QOS_COMMAND *pcmd=NULL;

    opterr = 0; //suppress option error messages

#if QOSCMD_DEBUG
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
        qos_print_help();
        return 0;
    }
    ret = qos_parse_cmd_line (argc, argv, &pcmd, &pdata);
    if (ret == QOS_CMD_OK)
    {
        if (pcmd->do_command)
        {
            ret = pcmd->do_command(pcmd,pdata);
            if (ret == QOS_CMD_OK && pcmd->print_data)
                pcmd->print_data(pdata);
        }
    }
    else if (ret == QOS_CMD_HELP)
    {
        qos_print_cmd_help(pcmd);
    }
    if( pdata)
    {
        free(pdata);
        pdata=NULL;
    }
    return ret;
}
