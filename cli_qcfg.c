/******************************************************************************

                         Copyright (c) 2012, 2014, 2015
                        Lantiq Beteiligungs-GmbH & Co. KG

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/

#include <stdio.h>
#include <unistd.h> // getopt
#include <getopt.h>
#include <string.h>
#include <stdlib.h>

#include "commondefs.h"
#include "qosal_debug.h"
#include "qosal_utils.h"
#include "qosal_queue_api.h"
#include "fapi_high.h"
#include "cli_qcfg.h"

#include "safe_str_lib.h"
#include "safe_mem_lib.h"

#define CHECK_FOR_NULL_ARG(arg, opt) { \
		if(arg == NULL) { \
			printf("Could not find value for argument [%c]\n", opt); \
			return LTQ_FAIL; \
		} \
	}

#define QCLI_MAX_STR 1024

/* Max chars in queue name, we append interface 
   name before the queue name while creating iptable 
   chain for the queue, iptable max chain lenght is 30 chars*/
#define QCLI_MAX_QUEUE_NAME_LEN 19

extern uint16_t LOGLEVEL, LOGTYPE;

char actmp[QCLI_MAX_STR];

name_value_t axschednv[]={
	{"wfq",QOS_SCHED_WFQ},
  {"sp",QOS_SCHED_SP},
	{"\0",0}
};

name_value_t axtypenv[]={
	{"ingress",QOS_Q_F_INGRESS},
	{"egress",0},
	{"\0",0}
};

name_value_t axshapernv[]={
	{"cb",QOS_SHAPER_COLOR_BLIND},
  {"trtcm",QOS_SHAPER_TR_TCM},
  {"trtcm_4115",QOS_SHAPER_TR_TCM_RFC4115},
  {"lcb",QOS_SHAPER_LOOSE_COLOR_BLIND},
	{"\0",0}
};

name_value_t axmeternv[]={
	{"srTCM",QOS_METER_SR_TCM},
	{"trtcm",QOS_METER_TR_TCM},
	{"\0",0}
};

name_value_t axdropnv[]={
	{"tail",QOS_DROP_TAIL},
  {"red",QOS_DROP_RED},
  {"wred",QOS_DROP_WRED},
  {"codel",QOS_DROP_CODEL},
	{"\0",0}
};

static struct option long_options[] = {
  {"priority",	required_argument, 0,  0 },
  {"weight",  required_argument, 0,  0 },
  {"map",  required_argument, 0,  0 },
  {"cir",  required_argument, 0,  0 },
  {"cbs",  required_argument, 0,  0 },
  {"pir",  required_argument, 0,  0 },
  {"pbs",  required_argument, 0,  0 },
  {"minth0",  required_argument, 0,  0 },
  {"maxth0",  required_argument, 0,  0 },
  {"maxp0",  required_argument, 0,  0 },
  {"minth1",  required_argument, 0,  0 },
  {"maxth1",  required_argument, 0,  0 },
  {"maxp1",  required_argument, 0,  0 },
  {"wredwt",  required_argument, 0,  0 },
  {"gthx",  required_argument, 0,  0 },
  {"ythx",  required_argument, 0,  0 },
  {"rthx",  required_argument, 0,  0 },
  {"ds-accel", required_argument, 0,  0 },
	/*Not subcmds */
  {"queue", no_argument,       0,  'q' },
  {"port", no_argument,       0,  'P' },
  {0,         0,                 0,  0 }
};

char*
qcli_getstring(name_value_t *pxnv);
void
qcli_print_usage(void);
int32_t
qcli_getval_fromname(char *pcname,name_value_t *pxnv);
int32_t
qcli_print_shaper(qos_shaper_t *pxshpr);
int32_t
qcli_print_drop(qos_drop_cfg_t *pxdrop);
int32_t
qcli_print_qstats(int32_t inumqs, qos_queue_stats_t *pxqstats);
int32_t
qcli_print_qcfg(int32_t inumqs, qos_queue_cfg_t *pxqcfg);
int32_t
qcli_print_pcfg(qos_shaper_t *shaper, int32_t weight, int32_t priority);
int32_t
qcli_invoke_fapi(struct x_qapi_t *pxqapi);
int32_t
qcli_handle_subcmd(int32_t index, const char* optarg, struct x_qapi_t *pxqapi);
extern int find_error_message(IN int respCode, OUT char *sErrMsg);

char*
qcli_getstring(name_value_t *pxnv)
{
	name_value_t *pxtmp=pxnv;
	int32_t ilen=0;
	while(pxtmp&&pxtmp->name[0]){
		ilen += snprintf(actmp+ilen,QCLI_MAX_STR,"%s/",pxtmp->name);
		pxtmp++;
	}
	return actmp;
}

void
qcli_print_usage(void)
{
	printf("Usage: qcfg --port/--queue \n");
	printf("-A/-D/-M/ [qname] => Add/delete/modify specific queue name\n");
	printf("-L/-S [all/qname] => Get all queues or a specific queue configuration/Statistics \n");
	printf("-l [qlen] => Length of the queue in bytes, applicable for Software TC Qdisc\n");
#ifdef PLATFORM_XRX500
        printf("\tFor queue in hardware use along the below sub options\n");
	printf("\t--gthx [Green color Threshold value in 0-2304 packets (default value 288 packets)] => Applicable for queue in hardware \n");
	printf("\t--ythx [Yellow color Threshold value in 0-2304 packets (default value 288 packets)] => Applicable for queue in hardware \n");
	printf("\t--rthx [Red color Threshold value in 0-2304 packets (default value 288 packets)] => Applicable for queue in hardware \n");
#endif
	printf("-i [interface name]\n");
	printf("--map [comma seperated list of traffic class numbers between 1 and 16]\n");
	printf("-m [%s] => Scheduler mode(sub options mentioned below)\n",qcli_getstring(axschednv));
	printf("\t--priority [priority] => Applicable when scheduler mode is SP\n");
	printf("\t--weight [weight] => Applicable when scheduler mode is WFQ\n");
	printf("-t [%s] => queue type, if not specified egress is taken as default\n",qcli_getstring(axtypenv));
	printf("-d [%s] => Drop mode(sub options mentioned below) \n",qcli_getstring(axdropnv));
	printf("\t--wredwt [weight] => WRED weight applicable when drop mode is WRED\n");
	printf("\t--minth0 [Min Threshold for curve 0 in percent qlen]\n");
	printf("\t--maxth0 [Max Threshold for curve 0 in percent qlen]\n");
	printf("\t--maxp0 [Max Drop Probability in percent at max threshold 0 for WRED curve 0]\n");
	printf("\t--minth1 [Min Threshold for curve 1 in percent qlen]\n");
	printf("\t--maxth1 [Max Threshold for curve 1 in percent qlen]\n");
	printf("\t--maxp1 [Max Drop Probability in percent at max threshold 1 for WRED curve 1]\n");
	printf("-s [%s] => Shaper Rate Limit(sub options mentioned below)\n",qcli_getstring(axshapernv));
	printf("\t--pir [Peak Info rate in kilo bits/sec]\n");
	printf("\t--pbs [Peak burst size rate in kilo bits]\n");
	printf("\t--cir [committed Info rate in kilo bits/sec]\n");
	printf("\t--cbs [committed burst size rate in kilo bits]\n");
	printf("-r [%s] => DownStream Rate Metering(sub options mentioned below)\n",qcli_getstring(axmeternv));
	printf("\t--pir [Peak Info rate in kilo bits/sec]\n");
	printf("\t--pbs [Peak burst size rate in kilo bits]\n");
	printf("\t--cir [committed Info rate in kilo bits/sec]\n");
	printf("\t--cbs [committed burst size rate in kilo bits]\n");
#if PLATFORM_XRX500
	printf("--ds-accel [value] => Downstream QoS acceleration Engine selection. Set to '1' to select MPE based QoS Acceleration in Downstream.\n");
#endif
}

int32_t
qcli_getval_fromname(char *pcname,name_value_t *pxnv)
{

	name_value_t *pxtmp=pxnv;
	while (pxtmp && pxtmp->name && strnlen_s(pxtmp->name, MAX_NAME_LEN) > 0) {
		if (strcasecmp(pxtmp->name,pcname)==0) {
			LOGF_LOG_DEBUG("Returning Val: %d\n",pxtmp->value);
			return pxtmp->value;
		}
		pxtmp++;
	}
	qcli_print_usage();
	return LTQ_FAIL;
}

int32_t
qcli_print_shaper(qos_shaper_t *pxshpr)
{
	printf("\tShaper Config:\n");
	char *strVar = util_get_name_from_value(pxshpr->mode,axshapernv);
	if(strVar != NULL && strnlen_s(strVar, MAX_NAME_LEN) > 0) {
		printf("\t\tMode: %s\n",strVar);
		printf("------> flags [%d]\n", pxshpr->flags);
		if(pxshpr->pir > 0) {
			printf("\t\tPeak rate: %d kilo bits/sec\n",pxshpr->pir);
		}
		if(pxshpr->pbs > 0) {
			printf("\t\tPeak burst size rate: %d kilo bits\n",pxshpr->pbs);
		}
		if(pxshpr->cir > 0) {
			printf("\t\tCommitted rate: %d kilo bits/sec\n",pxshpr->cir);
		}
		if(pxshpr->cbs > 0) {
			printf("\t\tCommitted burst size rate: %d kilo bits\n",pxshpr->cbs);
		}
	}
	else {
		printf("\t\tMode: %s\n","NONE");
	}
	return LTQ_SUCCESS;
}

int32_t
qcli_print_drop(qos_drop_cfg_t *pxdrop)
{
	if(pxdrop->enable != 0){
		printf("\tDrop Config:\n");
		printf("\t\tState: %s\n",pxdrop->enable?"enable":"disable");
		char *strVar = util_get_name_from_value(pxdrop->mode,axdropnv);
		printf("\t\tMode: %s\n",(strVar != NULL)?strVar:"");
		printf("\t\tWRED Config:\n");
		printf("\t\t\tWeight: %u\n",pxdrop->wred.weight);
		printf("\t\t\tMin Threshold for curve 0 in percent qlen: %u\n",pxdrop->wred.min_th0);
		printf("\t\t\tMax Threshold for curve 0 in percent qlen: %u\n",pxdrop->wred.max_th0);
		printf("\t\t\tMax Drop Probability in percent at max threshold 0 for WRED curve 0: %u\n",pxdrop->wred.max_p0);
		printf("\t\t\tMin Threshold for curve 1 in percent qlen: %u\n",pxdrop->wred.min_th1);
		printf("\t\t\tMax Threshold for curve 1 in percent qlen: %u\n",pxdrop->wred.max_th1);
		printf("\t\t\tMax Drop Probability in percent at max threshold 1 for WRED curve 1: %u\n",pxdrop->wred.max_p1);
	}
	return LTQ_SUCCESS;
}

int32_t
qcli_print_qstats(int32_t inumqs, qos_queue_stats_t *pxqstats)
{
	int32_t i;

	printf("Number of Queues requested: %d\n", inumqs);
	for(i=0;i<inumqs;i++){
		printf("Queue Stats Name: %s\n",pxqstats->name);
		if (pxqstats->flags & QOS_Q_STAT_RX_PKT)
			printf("\tReceived Packets: %llu\n",pxqstats->rx_pkt);
		if (pxqstats->flags & QOS_Q_STAT_RX_BYTE)
			printf("\tReceived Bytes: %llu\n",pxqstats->rx_bytes);
		if (pxqstats->flags & QOS_Q_STAT_TX_PKT)
			printf("\tTransmitted Packets: %llu\n",pxqstats->tx_pkt);
		if (pxqstats->flags & QOS_Q_STAT_TX_BYTE)
			printf("\tTransmitted Bytes: %llu\n",pxqstats->tx_bytes);
		if (pxqstats->flags & QOS_Q_STAT_DROP_PKT)
			printf("\tDropped Packets: %llu\n",pxqstats->drop_pkt);
		if (pxqstats->flags & QOS_Q_STAT_DROP_BYTE)
			printf("\tDropped Bytes: %llu\n",pxqstats->drop_bytes);
		pxqstats++;
	}
	return LTQ_SUCCESS;
}

int32_t
qcli_print_qcfg(int32_t inumqs, qos_queue_cfg_t *pxqcfg)
{
	int32_t i,j;
	printf("Number of Queues requested: %d\n", inumqs);
	for(i=0;i<inumqs;i++){
		printf("Queue Name: %s\n",pxqcfg->name);
		printf("\tState: %s\n",pxqcfg->enable?"enable":"disable");
		printf("\tStatus: %u\n",pxqcfg->status);
		printf("\tOwner: %u\n",pxqcfg->owner);
		printf("\tFlags: %u\n",pxqcfg->flags);
		printf("\tqid: %u\n",pxqcfg->queue_id);
		printf("\tTC Map:\t");
		for(j=0;j<MAX_TC_NUM;j++){
			printf("%d,",pxqcfg->tc_map[j]);
		}
		printf("\n");
		printf("\tqlen: %d\n",pxqcfg->qlen);
#ifdef PLATFORM_XRX500
		printf("\t\tqlen thresholds: green :%d, yellow :%d, red:%d\n",pxqcfg->drop.wred.max_th0,pxqcfg->drop.wred.max_th1,pxqcfg->drop.wred.drop_th1);
#endif
		char *strVar =util_get_name_from_value(pxqcfg->sched,axschednv);
		printf("\tScheduler: %s\n",(strVar != NULL)?strVar:"");
		strVar = util_get_name_from_value(pxqcfg->flags&QOS_Q_F_INGRESS,axtypenv);
		printf("\tType: %s\n",(strVar != NULL)?strVar:"");
		printf("\tweight: %d\n",pxqcfg->weight);
		printf("\tpriority: %d\n",pxqcfg->priority);
		qcli_print_drop(&pxqcfg->drop);
		qcli_print_shaper(&pxqcfg->shaper);
		pxqcfg++;
	}
	return LTQ_SUCCESS;
}

int32_t
qcli_print_pcfg(qos_shaper_t *shaper, int32_t weight, int32_t priority)
{
	printf("Port priority: %d\n",priority);
	printf("Port weight: %d\n",weight);
	qcli_print_shaper(shaper);
	return LTQ_SUCCESS;
}

int32_t
qcli_invoke_fapi(struct x_qapi_t *pxqapi)
{
	int32_t iret=-1, nRet = LTQ_FAIL;
	char sErrMsg[128] = {0};
	if(pxqapi->uiflags&QOS_Q_F_QUEUE){
		/* Queue Level FAPI */
		if(pxqapi->uiflags&QOS_Q_F_STATS_GET){
			int32_t inumqs;
			qos_queue_stats_t *pxqstats;
			iret=fapi_qos_queue_stats_get(pxqapi->acifname, pxqapi->xqcfg.name,&inumqs,&pxqstats,pxqapi->uiflags);
			LOGF_LOG_DEBUG("\nQueue Stats for interface %s:\n", pxqapi->acifname);
			qcli_print_qstats(inumqs, pxqstats);
			os_free(pxqstats);
		}
		else if(pxqapi->uiflags&QOS_Q_F_GET){
			int32_t inumqs;
			qos_queue_cfg_t *pxqcfg;
			iret=fapi_qos_queue_get(pxqapi->acifname, pxqapi->xqcfg.name,&inumqs,&pxqcfg,pxqapi->uiflags);
			LOGF_LOG_DEBUG("\nQueue Configuration for interface %s:\n", pxqapi->acifname);
			qcli_print_qcfg(inumqs, pxqcfg);
			os_free(pxqcfg);
		}
		else{
			LOGF_LOG_DEBUG("Calling set FAPI with following Config\n");
			//qcli_print_qcfg(1,&pxqapi->xqcfg);
			iret=fapi_qos_queue_set(pxqapi->acifname,&pxqapi->xqcfg,pxqapi->uiflags);
		}
	}
	else if(pxqapi->uiflags&QOS_Q_F_PORT){
		/* Port Level FAPI */
		if(pxqapi->uiflags&QOS_Q_F_GET){
			LOGF_LOG_DEBUG("Invoking port cfg get FAPI\n");
			iret=fapi_qos_port_config_get(pxqapi->acifname,&pxqapi->xqcfg.shaper,
										&pxqapi->xqcfg.weight,&pxqapi->xqcfg.priority,pxqapi->uiflags); 
			qcli_print_pcfg(&pxqapi->xqcfg.shaper,pxqapi->xqcfg.weight,pxqapi->xqcfg.priority);
		}
		else{
			if(pxqapi->uiflags & QOS_OP_F_MODIFY){
				LOGF_LOG_DEBUG("Invoking port cfg update FAPI\n");
				if(pxqapi->xqcfg.shaper.pir !=0) {
					iret=fapi_qos_port_config_set(pxqapi->acifname,&pxqapi->xqcfg.shaper,
						pxqapi->xqcfg.weight,pxqapi->xqcfg.priority,pxqapi->uiflags);
				}
				else
				iret=fapi_qos_port_update(pxqapi->acifname,pxqapi->uiflags);
			}
			else{
				LOGF_LOG_DEBUG("Invoking port cfg set FAPI\n");
				if(pxqapi->uiflags & QOS_OP_F_PORT_METER) {
					LOGF_LOG_DEBUG("Invoking port cfg set METER FAPI\n");
					iret=fapi_qos_port_config_set(pxqapi->acifname,&pxqapi->xqcfg.meter,
						pxqapi->xqcfg.weight,pxqapi->xqcfg.priority,pxqapi->uiflags);
				}
				if(pxqapi->uiflags & QOS_OP_F_QUEUE_SHAPER) {
					LOGF_LOG_DEBUG("Invoking port cfg set SHAPER FAPI\n");
					iret=fapi_qos_port_config_set(pxqapi->acifname,&pxqapi->xqcfg.shaper,
						pxqapi->xqcfg.weight,pxqapi->xqcfg.priority,pxqapi->uiflags);
				}
			}
		}
	}
	else{
		qcli_print_usage();
	}

	if(iret<0) {
		if(pxqapi->uiflags&QOS_Q_F_PORT) {
			printf("Requested port configuration failed.\n");
		}
		else if(pxqapi->uiflags&QOS_Q_F_QUEUE) {
			printf("Requested queue configuration failed.\n");
		}
		printf("Reason : ");
		nRet = find_error_message(iret, sErrMsg);
		printf("%s\n", (nRet == LTQ_SUCCESS)?sErrMsg:"Unknown.");
	}
	else {
		if(pxqapi->uiflags&QOS_Q_F_PORT) {
			printf("Requested port configuration succeeded.\n");
		}
		else if(pxqapi->uiflags&QOS_Q_F_QUEUE) {
			printf("Requested queue configuration succeeded.\n");
		}
	}
	return iret;
}

int32_t qcli_handle_subcmd(int32_t index, const char* optarg, struct x_qapi_t *pxqapi)
{
	int32_t i=0;
	const char *pcptr=NULL;
	char *pctmp;
	LOGF_LOG_DEBUG("Index [%d] option [%s]\n", index,long_options[index].name);
	switch(index)
	{
		case 0:
			/*Priority*/
			if(((pxqapi->uiflags & QOS_OP_F_QUEUE_SCHED)!=QOS_OP_F_QUEUE_SCHED)&&
					(pxqapi->xqcfg.sched != 1)){
				printf("Appropriate Scheduler not set");
				return LTQ_FAIL;
			}
			pxqapi->xqcfg.priority=atoi(optarg);
		break;
		case 1:
			/*weight*/
			if(((pxqapi->uiflags & QOS_OP_F_QUEUE_SCHED)!=QOS_OP_F_QUEUE_SCHED)&&
					(pxqapi->xqcfg.sched != 0)){
				printf("Appropriate Scheduler not set");
				return LTQ_FAIL;
			}
			pxqapi->xqcfg.weight=atoi(optarg);
		break;
		case 2:
			/*map*/
			i=0;
			pcptr=optarg;
			LOGF_LOG_DEBUG("map %s\n",optarg);
			while((pctmp=strchr(pcptr,','))!=NULL){
			LOGF_LOG_DEBUG("map %s\n",pcptr);
				pxqapi->xqcfg.tc_map[i]=atoi(pcptr);
				i++;
				pcptr=pctmp+1;
			}
			pxqapi->xqcfg.tc_map[i]=atoi(pcptr);
		break;
		case 3:
			/*cir*/
			if(pxqapi->uiflags & QOS_OP_F_PORT_METER)
				pxqapi->xqcfg.meter.cir=atoi(optarg);
			else {
				if((pxqapi->uiflags & QOS_OP_F_QUEUE_SHAPER)!=QOS_OP_F_QUEUE_SHAPER){
					printf("Appropriate Shaper not set");
					return LTQ_FAIL;
				}
				pxqapi->xqcfg.shaper.cir=atoi(optarg);
			}
		break;
		case 4:
			/*cbs*/
			if(pxqapi->uiflags & QOS_OP_F_PORT_METER)
				pxqapi->xqcfg.meter.cbs = atoi(optarg);
			else {
				if ((pxqapi->uiflags & QOS_OP_F_QUEUE_SHAPER)!=QOS_OP_F_QUEUE_SHAPER){
					printf("Appropriate Shaper not set");
					return LTQ_FAIL;
				}
				pxqapi->xqcfg.shaper.cbs=atoi(optarg);
			}
		break;
		case 5:
			/*pir*/
			if(pxqapi->uiflags & QOS_OP_F_PORT_METER)
				pxqapi->xqcfg.meter.pir=atoi(optarg);
			else {
				if((pxqapi->uiflags & QOS_OP_F_QUEUE_SHAPER)!=QOS_OP_F_QUEUE_SHAPER) {
					printf("Appropriate Shaper not set");
					return LTQ_FAIL;
				}
				pxqapi->xqcfg.shaper.pir=atoi(optarg);
			}
			
		break;
		case 6:
			/*pbs*/
			if(pxqapi->uiflags & QOS_OP_F_PORT_METER)
				pxqapi->xqcfg.meter.pbs=atoi(optarg);
			else {
				if((pxqapi->uiflags & QOS_OP_F_QUEUE_SHAPER)!=QOS_OP_F_QUEUE_SHAPER){
					printf("Appropriate Shaper not set");
					return LTQ_FAIL;
				}
				pxqapi->xqcfg.shaper.pbs=atoi(optarg);
			}
		break;
		case 7:
			/*minth0*/
			if((pxqapi->uiflags & QOS_OP_F_QUEUE_DROP)!=QOS_OP_F_QUEUE_DROP){
				printf("Appropriate drop not set");
				return LTQ_FAIL;
			}
			pxqapi->xqcfg.drop.wred.min_th0 =atoi(optarg); 
		break;
		case 8:
			/*maxth0*/
			if((pxqapi->uiflags & QOS_OP_F_QUEUE_DROP)!=QOS_OP_F_QUEUE_DROP){
				printf("Appropriate drop not set");
				return LTQ_FAIL;
			}
			pxqapi->xqcfg.drop.wred.max_th0 =atoi(optarg); 
		break;
		case 9:
			/*maxp0*/
			if((pxqapi->uiflags & QOS_OP_F_QUEUE_DROP)!=QOS_OP_F_QUEUE_DROP){
				printf("Appropriate drop not set");
				return LTQ_FAIL;
			}
			pxqapi->xqcfg.drop.wred.max_p0 =atoi(optarg); 
		break;
		case 10:
			/*minth1*/
			if((pxqapi->uiflags & QOS_OP_F_QUEUE_DROP)!=QOS_OP_F_QUEUE_DROP){
				printf("Appropriate drop not set");
				return LTQ_FAIL;
			}
			pxqapi->xqcfg.drop.wred.min_th1 =atoi(optarg); 
		break;
		case 11:
			/*maxth1*/
			if((pxqapi->uiflags & QOS_OP_F_QUEUE_DROP)!=QOS_OP_F_QUEUE_DROP){
				printf("Appropriate drop not set");
				return LTQ_FAIL;
			}
			pxqapi->xqcfg.drop.wred.max_th1 =atoi(optarg); 
		break;
		case 12:
			/*maxp1*/
			if((pxqapi->uiflags & QOS_OP_F_QUEUE_DROP)!=QOS_OP_F_QUEUE_DROP){
				printf("Appropriate drop not set");
				return LTQ_FAIL;
			}
			pxqapi->xqcfg.drop.wred.max_p1 =atoi(optarg); 
		break;
		case 13:
			/*wredwt*/
			if((pxqapi->uiflags & QOS_OP_F_QUEUE_DROP)!=QOS_OP_F_QUEUE_DROP){
				printf("Appropriate drop not set");
				return LTQ_FAIL;
			}
			pxqapi->xqcfg.drop.wred.weight =atoi(optarg);
		break;
		case 14:
			/*qlen green threshold*/
			if((pxqapi->uiflags & QOS_OP_F_QUEUE_LENGTH)!=QOS_OP_F_QUEUE_LENGTH){
				printf("Appropriate QUEUE LENGTH threshold not set");
				return LTQ_FAIL;
			}
			pxqapi->xqcfg.drop.flags |= QOS_DT_F_GREEN_THRESHOLD;
			pxqapi->xqcfg.drop.wred.max_th0 =atoi(optarg);
		break;
		case 15:
			/*qlen yellow threshold*/
			if((pxqapi->uiflags & QOS_OP_F_QUEUE_LENGTH)!=QOS_OP_F_QUEUE_LENGTH){
				printf("Appropriate QUEUE LENGTH threshold not set");
				return LTQ_FAIL;
			}
			pxqapi->xqcfg.drop.flags |= QOS_DT_F_YELLOW_THRESHOLD;
			pxqapi->xqcfg.drop.wred.max_th1 =atoi(optarg);
		break;
		case 16:
			/*qlen red threshold*/
			if((pxqapi->uiflags & QOS_OP_F_QUEUE_LENGTH)!=QOS_OP_F_QUEUE_LENGTH){
				printf("Appropriate QUEUE LENGTH threshold not set");
				return LTQ_FAIL;
			}
			pxqapi->xqcfg.drop.flags |= QOS_DT_F_RED_THRESHOLD;
			pxqapi->xqcfg.drop.wred.drop_th1 =atoi(optarg);
		break;
#ifdef PLATFORM_XRX500
		case 17:
			/*ds-accel*/
			if (atoi(optarg) == 1) {
				pxqapi->xqcfg.flags |= QOS_Q_F_DS_QOS;
			} else {
				printf("Down stream QOS should be set with value 1\n");
				return LTQ_FAIL;
			}
		break;
#endif
		default:
			return LTQ_FAIL;
	}
	return LTQ_SUCCESS;
}

int32_t
qcfg_main(int32_t argc, char**argv)
{
  int32_t c, nRet = 0;
	int32_t option_index = 0;
  struct x_qapi_t xqcfg;
  
	memset_s(&xqcfg, sizeof(xqcfg), 0);

  while(1){
    c = getopt_long(argc, argv, "A:D:M:S:P:L:s:r:d:m:t:i:l:q",
                        long_options, &option_index);
    if(c==-1) break;
    switch(c){
      case 0:
        if(optarg){
        	LOGF_LOG_DEBUG("\n");
		nRet = qcli_handle_subcmd(option_index,optarg, &xqcfg);
		if(nRet == LTQ_FAIL) {
			return nRet;
		}
	}
      break;
      case 'A':
		CHECK_FOR_NULL_ARG(optarg, c)
	if ((strnlen_s(optarg, QCLI_MAX_STR)) > QCLI_MAX_QUEUE_NAME_LEN) {
		printf("Error Queue Name is too Long, should be less than %d chars\n", QCLI_MAX_QUEUE_NAME_LEN);
		return LTQ_FAIL;
	}
	if(strncpy_s(xqcfg.xqcfg.name, MAX_Q_NAME_LEN, optarg, strnlen_s(optarg, QCLI_MAX_STR)) != EOK) {
		printf("Error copying Queue name !!\n");
		return LTQ_FAIL;
	}
	xqcfg.uiflags |= QOS_OP_F_ADD;
        LOGF_LOG_DEBUG("Add option: [%s]\n",xqcfg.xqcfg.name);
        break;
      case 'D':
		CHECK_FOR_NULL_ARG(optarg, c)
	if(strncpy_s(xqcfg.xqcfg.name, MAX_Q_NAME_LEN, optarg, strnlen_s(optarg, QCLI_MAX_STR)) != EOK) {
                printf("Error copying Queue name !!\n");
                return LTQ_FAIL;
        }
	xqcfg.uiflags |= QOS_OP_F_DELETE;
        LOGF_LOG_DEBUG("Delete option: [%s]\n",xqcfg.xqcfg.name);
        break;
      case 'M':
		CHECK_FOR_NULL_ARG(optarg, c)
	if(strncpy_s(xqcfg.xqcfg.name, MAX_Q_NAME_LEN, optarg, strnlen_s(optarg, QCLI_MAX_STR)) != EOK) {
                printf("Error copying Queue name !!\n");
                return LTQ_FAIL;
        }
	xqcfg.uiflags |= QOS_OP_F_MODIFY;
        LOGF_LOG_DEBUG("Modify option: [%s]\n",xqcfg.xqcfg.name);
        break;
      case 'L':
		CHECK_FOR_NULL_ARG(optarg, c)
	if(strncpy_s(xqcfg.xqcfg.name, MAX_Q_NAME_LEN, optarg, strnlen_s(optarg, QCLI_MAX_STR)) != EOK) {
                printf("Error copying Queue name !!\n");
                return LTQ_FAIL;
        }
	xqcfg.uiflags |= QOS_Q_F_GET;
        LOGF_LOG_DEBUG("Get option: [%s]\n",xqcfg.xqcfg.name);
        break;
      case 'S':
		CHECK_FOR_NULL_ARG(optarg, c)
	if(strncpy_s(xqcfg.xqcfg.name, MAX_Q_NAME_LEN, optarg, strnlen_s(optarg, QCLI_MAX_STR)) != EOK) {
                printf("Error copying Queue name !!\n");
                return LTQ_FAIL;
        }
	xqcfg.uiflags |= QOS_Q_F_STATS_GET;
        LOGF_LOG_DEBUG("Get option: [%s]\n",xqcfg.xqcfg.name);
        break;

	case 'P':
		/* port operation */
		xqcfg.uiflags |= QOS_Q_F_PORT;
		break;
	case 's':
		/*Shaper*/
		CHECK_FOR_NULL_ARG(optarg, c)
		xqcfg.uiflags |= QOS_OP_F_QUEUE_SHAPER;
		xqcfg.xqcfg.shaper.enable = QOSAL_ENABLE;
		nRet = qcli_getval_fromname(optarg,axshapernv);
		if(nRet == LTQ_FAIL) {
			return nRet;
		}
		xqcfg.xqcfg.shaper.mode = nRet;
	break;
	case 'r':
                /*Meter*/
                CHECK_FOR_NULL_ARG(optarg, c)
                xqcfg.uiflags |= QOS_OP_F_PORT_METER;
                xqcfg.xqcfg.meter.enable = QOSAL_ENABLE;
		strncpy_s(xqcfg.xqcfg.meter.name, MAX_IF_NAME_LEN, xqcfg.xqcfg.name, strnlen_s(xqcfg.xqcfg.name, MAX_IF_NAME_LEN));
                nRet = qcli_getval_fromname(optarg,axmeternv);
                if(nRet == LTQ_FAIL) {
                        return nRet;
                }
                xqcfg.xqcfg.meter.mode = nRet;
	break;
	case 'm':
		/*Scheduling mode*/
		CHECK_FOR_NULL_ARG(optarg, c)
		xqcfg.uiflags |= QOS_OP_F_QUEUE_SCHED;
		nRet = qcli_getval_fromname(optarg,axschednv);
		if(nRet == LTQ_FAIL) {
			return nRet;
		}
		xqcfg.xqcfg.sched = nRet;
	break;
	case 't':
		/*Queue type */
		CHECK_FOR_NULL_ARG(optarg, c)
		nRet = qcli_getval_fromname(optarg,axtypenv);
		if(nRet == LTQ_FAIL) {
			return nRet;
		}
		xqcfg.xqcfg.flags |= nRet;
		if (xqcfg.xqcfg.flags & QOS_Q_F_INGRESS)
			xqcfg.uiflags |= QOS_OP_F_INGRESS;
		nRet = qcli_getval_fromname(optarg,axtypenv);
		if(nRet == LTQ_FAIL) {
			return nRet;
		}
		xqcfg.xqcfg.shaper.flags |= nRet;
		if (xqcfg.xqcfg.shaper.flags & QOS_Q_F_INGRESS)
			xqcfg.uiflags |= QOS_OP_F_INGRESS;
	break;

	case 'd':
		/*Drop mechanism*/
		CHECK_FOR_NULL_ARG(optarg, c)
		xqcfg.uiflags |= QOS_OP_F_QUEUE_DROP;
		nRet = qcli_getval_fromname(optarg,axdropnv);
		if(nRet == LTQ_FAIL) {
			return nRet;
		}
		xqcfg.xqcfg.drop.mode = nRet;
		xqcfg.xqcfg.drop.enable=1;
	break;
	case 'i':
		/*Interface name*/
		CHECK_FOR_NULL_ARG(optarg, c)
		if(strncpy_s(xqcfg.acifname, MAX_IF_NAME_LEN, optarg, strnlen_s(optarg, QCLI_MAX_STR)) != EOK)
        	{
                	printf("Error copying Interface name !!\n");
                	return LTQ_FAIL;
        	}
		xqcfg.acifname[sizeof(xqcfg.acifname) - 1] = '\0';
	break;
	case 'l':
		/* Q len */
		CHECK_FOR_NULL_ARG(optarg, c)
		xqcfg.uiflags |= QOS_OP_F_QUEUE_LENGTH;
		xqcfg.xqcfg.qlen=atoi(optarg);
	break;
	case 'q':
		/* Q operation */
		xqcfg.uiflags |= QOS_Q_F_QUEUE;
	break;
      default:
		qcli_print_usage();
		return LTQ_SUCCESS;
    }
  }
	xqcfg.xqcfg.enable = QOSAL_ENABLE;

	if(strnlen_s(xqcfg.acifname, QCLI_MAX_STR) == 0)
	{
		printf("please specify the interface name \n\n");
		qcli_print_usage();
		return LTQ_FAIL;
	}

	return qcli_invoke_fapi(&xqcfg);
}
