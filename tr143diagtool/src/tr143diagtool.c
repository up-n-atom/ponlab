/******************************************************************************

  Copyright © 2022 - 2024 MaxLinear, Inc.

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

 ******************************************************************************/
/***************************************************************************** *
 *     File Name  : tr143diagtool.c                                            *
 *     Description: TR-143 speed test application.							   *
 *                                                                             *
 ******************************************************************************/

#include <stdlib.h> 
#include <stdio.h>
#include <unistd.h>
#include <inttypes.h> 
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <stdbool.h>
#include <getopt.h>
#include <fcntl.h>
#include <netdb.h>
#include <net/if.h>
#include <linux/in6.h>
#include "safe_str_lib.h"
#include "tr143diagtool.h"

#ifndef STR
	#define STR(macro)  #macro
#endif

#define SUCCESS			(0)
#define FAILURE			(-1)
#define DEF_TYPE		0
#define DEF_CON			1
#define DEF_DSCP		0
#define DEF_MODE		1
#define DEF_PORT		80
#define MAX_IP_LEN		46
#define PATH_LEN		64
#define DOMAIN_LEN		32
#define INC_INTERVAL	1
#define DEF_MODE_VAL	"30sec"
#define CONFIG_OPT		"t:c:d:m:v:w:i:s:p:n:u:l::"
#define TR143DIAG_OPT	"CSARINh"
#define TR143DIAG_CHAR_FILE	"/dev/tr143diagtool"
#define KB				(1024)
#define MB				(1024*1024)
#define GB				(1024*1024*1024)

#define TR143_INFO(format, args...)							\
	do {														\
		fprintf(stdout, "[tr143diagtool] "format"\n", ## args);	\
	} while(0)

#define TR143_DEBUG(format, args...)							\
	do {														\
		fprintf(stdout, "[tr143diagtool %s():%d] "format"\n",	\
				__func__, __LINE__, ## args);					\
	} while(0)

#define TR143_ERROR(format, args...)							\
	do {														\
		fprintf(stderr, "[tr143diagtool] "format"\n", ## args);					\
	} while(0)


struct in_addr ip;

const char* tr143_error_name[] = {"TR143_NONE", "TR143_COMPLETED",
	"TR143_Error_CannotResolveHostName", "TR143_Error_NoRouteToHost",
	"TR143_Error_InitConnectionFailed", "TR143_Error_NoResponse",
	"TR143_Error_PasswordRequestFailed", "TR143_Error_Login Failed",
	"TR143_Error_NoTransferMode", "TR143_Error_NoPASV", "TR143_Error_NoCWD",
	"TR143_Error_NoSTOR", "TR143_Error_NoTransferComplete",
	"TR143_Error_Timeout", "TR143_Error_Internal", "TR143_Error_Other"};

struct error_flag {
	unsigned short test_type : 1;
	unsigned short connection : 1;
	unsigned short dscp : 1;
	unsigned short test_mode : 1;
	unsigned short mode_value : 1;
	unsigned short wanif : 1;
	unsigned short wanip : 1;
	unsigned short serverip : 1;
	unsigned short serverport : 1;
	unsigned short servername : 1;
	unsigned short url : 1;
	unsigned short inc_interval : 1;
};

struct error_flag err_flag;

static inline const char* str_test_type(enum test_type t_type)
{
	switch (t_type) {
		case TR143_DIAG_TEST_UPLOAD:
			return STR(TR143_DIAG_TEST_UPLOAD);
		case TR143_DIAG_TEST_DOWNLOAD:
			return STR(TR143_DIAG_TEST_DOWNLOAD);
		default:
			return STR(INVALID);
	}
	return STR(INVALID);
}

static bool isValidIpAddress(char *ipAddress, struct tr143diag_config *config)
{
    struct sockaddr_in sa;
	struct sockaddr_in6 sa6;
	if (inet_pton(AF_INET, ipAddress, &(sa.sin_addr))) {
		config->serverip.ip_type = IPV4;
		return true;
	} else if (inet_pton(AF_INET6, ipAddress, &(sa6.sin6_addr))) {
		config->serverip.ip_type = IPV6;
		return true;
	} else
		return false;
}

static int resolve_hostname(char *hostname, char *ip_addr, struct tr143diag_config *config)
{
	int ret = 0;
	struct addrinfo ai_hints = {0};
	struct addrinfo *ai_result = NULL;

	ai_hints.ai_socktype = SOCK_STREAM;
	ai_hints.ai_family = AF_UNSPEC;
	ai_hints.ai_protocol = IPPROTO_TCP;

	ret = getaddrinfo(hostname, "80", &ai_hints, &ai_result);

	if (ret) {
		fprintf(stderr, "DNS resolution failed : %s\n", gai_strerror(ret));
		exit(EXIT_FAILURE);
	}

	if (ai_result->ai_family == AF_INET) {
		struct sockaddr_in *addr = (struct sockaddr_in *)ai_result->ai_addr;
		if (inet_ntop(ai_result->ai_family, &(addr->sin_addr), ip_addr, NI_MAXHOST))
			config->serverip.ip_type = IPV4;
	} else if (ai_result->ai_family == AF_INET6) {
		struct sockaddr_in6 *addr = (struct sockaddr_in6 *)ai_result->ai_addr;
		if (inet_ntop(ai_result->ai_family, &(addr->sin6_addr), ip_addr, NI_MAXHOST))
			config->serverip.ip_type = IPV6;
	}

	freeaddrinfo(ai_result);
	return SUCCESS;
}

static void get_serverip_from_url(struct tr143diag_config *config)
{
	char domain[TR143_DIAG_URL_LEN] = {0};
	char ip_addr[MAX_IP_LEN] = {0};
	int ret = 0;

	if (strstr(config->url, "https:")) {
		fprintf(stderr, "https is not supported\n");
		exit(0);
	}

	ret = sscanf_s(config->url, "http://%128[^/]/", domain, sizeof(config->url));
	if (ret < 0) {
		fprintf(stderr, "URL is not supported URL:%s\n", config->url);
		exit(0);
	}
	resolve_hostname(domain, &ip_addr[0], config);
	if (config->serverip.ip_type == IPV6) {
		if(!inet_pton(AF_INET6, ip_addr, &(config->serverip.ip.ip6))) {
			fprintf(stderr, "Invalid IPv6 address: %s\n", ip_addr);
			exit(EXIT_FAILURE);
			}
	} else if (config->serverip.ip_type == IPV4) {
		if (!inet_pton(AF_INET, ip_addr, &(config->serverip.ip.ip4))) {
			fprintf(stderr, "Invalid IPv4 address: %s\n", ip_addr);
			exit(EXIT_FAILURE);
		}
	}
}

static void result_throughput(struct tr143diag_result *result)
{
	struct PerConnectionResult *perconn = NULL;
	uint64_t throughput = 0;
	uint64_t nseconds = 0;
	int index = 0;

	if (result->EOMTime != result->BOMTime) {
		nseconds = (result->EOMTime - result->BOMTime);
		if (result->ttype == TR143_DIAG_TEST_UPLOAD) {
			throughput = ((result->TestBytesSent * 8) / nseconds);
			printf("UPLOAD THROUGHPUT : %llu Mbps\n", (unsigned long long int)throughput);
			printf("FULL LOADING UPLOAD THROUGHPUT : %llu Mbps\n",
					(unsigned long long int)((result->TestBytesSentUnderFullLoading * 8)/result->periodOfFullLoading));
		} else if (result->ttype == TR143_DIAG_TEST_DOWNLOAD) {
			throughput = ((result->TestBytesReceived * 8) / nseconds);
			printf("DOWNLOAD THROUGHPUT : %llu Mbps\n", (unsigned long long int)throughput);
			printf("FULL LOADING DOWNLOAD THROUGHPUT : %llu Mbps\n",
				(unsigned long long int)((result->TestBytesReceivedUnderFullLoading * 8)/result->periodOfFullLoading));
		}
	} else {
		printf("Test is not started\n");
		return;
	}

	for (; index < result->nconn; ++index) {
		perconn = &result->PerConnectionResult[index];
		nseconds = (perconn->EOMTime - perconn->BOMTime);
		if (result->ttype == TR143_DIAG_TEST_UPLOAD) {
			throughput = ((perconn->TestBytesSent * 8) / nseconds);
			printf("PerConnection[%d] : %llu Mbps\n", index, (unsigned long long int)throughput);
		} else if (result->ttype == TR143_DIAG_TEST_DOWNLOAD) {
			throughput = ((perconn->TestBytesReceived * 8) / nseconds);
			printf("PerConnection[%d] : %llu Mbps\n", index, (unsigned long long int)throughput);
		}
	}
} 

static int invoke_ioctl(unsigned int cmd, void *data)
{
	int fd = -1;
	int ret = 0;

	fd = open(TR143DIAG_CHAR_FILE, O_RDWR);
	if (fd < 0) {
		TR143_ERROR("unable to open %s file", TR143DIAG_CHAR_FILE);
		exit(0);
	}

	ret = ioctl(fd, cmd, data);
	if (ret != 0) {
		TR143_ERROR("ioctl:%d command failed, %s", cmd, strerror(errno));
		ret = FAILURE;
	}

	close(fd);
	return ret;
}

void print_help(void)
{
	printf("\nUsage: tr143diagtool <command> [parameters]\n\n");
	printf("<command>\n\n");
	printf("-C [config parameters] - Configure the test parameters\n");
	printf("-S [notify parameters] - Start the test\n");
	printf("-N [notify parameters] - Wait for requested event\n");
	printf("-A                     - Abort the test\n");
	printf("-R                     - Get the test result\n\n");
	printf("-I                     - Get the incremental result\n\n");
	printf("[config parameters]\n");
	printf("-t --ttype      Test type up/down. 0 for upload 1 for download\n");
	printf("-c --nconn      Number of simultaeous connections. Default 1\n");
	printf("-d --dscp       DSCP value for TCP connection - Currently not supported\n");
	printf("-m --tmode      Test mode 0 - File Transfer , 1 -  Time-Based\n");
	printf("-v --mvalue     Transfer mode duration in sec, filesize in bytes\n");
	printf("-w --wanif      WAN interface name\n");
	printf("-i --wanip      WAN IP address\n");
	printf("-s --serverip   server ip address \n");
	printf("-p --serverport Server port \n");
	printf("-u --url        URL - Upload/download URL\n\n");
	printf("-l --interval	Incremental Result interval\n\n");
	printf("[notify parameters]\n");
	printf("-b --bomtime	Requested BOMTime to notify\n");
	printf("-e --eomtime    Requested EOMTime to notify\n\n");
	printf("sample command -->>\n");
	printf("tr143diagtool -C -t 0 -c 2 -d 0 -m 0 -v 500MB -s 10.10.200.2 -p 80\n");
	printf("tr143diagtool -C -t 0 -c 1 -m 0 -v 2GB -u http://10.10.200.2/a.txt\n\n");
	printf("Note: use either serverip and port or url\n");
	exit(SUCCESS);
}

static int param_error(void)
{
	if (!err_flag.test_type) {
		TR143_ERROR("Error test type is missing\n");
		return FAILURE;
	}
	if (!err_flag.connection) {
		TR143_ERROR("Error number of connection is missing\n");
		return FAILURE;
	}
	if (!err_flag.test_mode) {
		TR143_ERROR("Error transfer mode is missing\n");
		return FAILURE;
	}
	if (!err_flag.mode_value) {
		TR143_ERROR("Error transfer mode value is missing\n");
		return FAILURE;
	}
	if (!err_flag.serverip && !err_flag.serverport && !err_flag.url) {
		TR143_ERROR("URL or serverip and serverport combination is required\n");
		return FAILURE;
	}
	return SUCCESS;
}

static void print_result(struct tr143diag_result *result)
{
	struct PerConnectionResult *perconn = NULL;
	int index = 0;

	if (!result->nconn) {
		printf("Results are not availabe, already fetched\n");
		return;
	}
	TR143_INFO("HTTP status code : %d", result->http_status_code);
	TR143_INFO("ROMTime : %llu", result->ROMTime);
	TR143_INFO("BOMTime : %llu", result->BOMTime);
	TR143_INFO("EOMTime : %llu", (unsigned long long int)(result->EOMTime));
	TR143_INFO("TestBytesSent       : %llu", (unsigned long long int) (result->TestBytesSent));
	TR143_INFO("TestBytesReceived   : %llu", (unsigned long long int)(result->TestBytesReceived));
	TR143_INFO("TotalBytesReceived(IP Layer)   : %llu", (unsigned long long int)(result->TotalBytesReceived));
	TR143_INFO("TotalBytesSent(IP Layer)       : %llu", (unsigned long long int)(result->TotalBytesSent));
	TR143_INFO("TestBytesSentUnderFullLoading  : %llu", (unsigned long long int)(result->TestBytesSentUnderFullLoading));
	TR143_INFO("TestBytesReceivedUnderFullLoading           : %llu", (unsigned long long int)(result->TestBytesReceivedUnderFullLoading));
	TR143_INFO("TotalBytesReceivedUnderFullLoading(IP Layer): %llu", (unsigned long long int)(result->TotalBytesReceivedUnderFullLoading));
	TR143_INFO("TotalBytesSentUnderFullLoading(IP Layer)    : %llu", (unsigned long long int)(result->TotalBytesSentUnderFullLoading));
	TR143_INFO("PeriodOfFullLoading     : %llu", (unsigned long long int)(result->periodOfFullLoading));
	TR143_INFO("Number of connections   : %u", result->nconn);
	for (; index < result->nconn; ++index) {
		perconn = &result->PerConnectionResult[index];
		TR143_INFO("PerConnectionResult[%d]------", index);
		TR143_INFO("ROMTime : %llu", (unsigned long long int)(perconn->ROMTime));
		TR143_INFO("BOMTime : %llu", (unsigned long long int)(perconn->BOMTime));
		TR143_INFO("EOMTime : %llu", (unsigned long long int)(perconn->EOMTime));
		TR143_INFO("TCPOpenRequestTime   : %llu", (unsigned long long int)(perconn->TCPOpenRequestTime));
		TR143_INFO("TCPOpenResponseTime  : %llu", (unsigned long long int)(perconn->TCPOpenResponseTime));
		TR143_INFO("TestBytesSent        : %llu", (unsigned long long int)(perconn->TestBytesSent));
		TR143_INFO("TestBytesReceived    : %llu", (unsigned long long int)(perconn->TestBytesReceived));
		TR143_INFO("TotalBytesReceived(IP Layer): %llu", (unsigned long long int)(perconn->TotalBytesReceived));
		TR143_INFO("TotalBytesSent(IP Layer)    : %llu", (unsigned long long int)(perconn->TotalBytesSent));
	}
}

static void print_inc_result(struct tr143inc_result *result)
{
	struct IncrementalResult *inc_result = result->inc_result;
	int index = 0;

	printf("Max Entries     : %d\n", result->max_entries);
	printf("Current Entries : %d\n", result->num_entries);
	printf("Sr.No\tStartTime\t\tEndTime\t\tTestBytesReceived\tTestBytesSent\tTotalBytesReceived\tTotalBytesSent\n");
	for (index = 0; index < result->num_entries; index ++, inc_result ++) {
		printf("%d\t%llu\t%llu\t%llu\t\t%llu\t%llu\t\t\t%llu\n", index+1,
				inc_result->StartTime, inc_result->EndTime, inc_result->TestBytesReceived,
				inc_result->TestBytesSent, inc_result->TotalBytesReceived,
				inc_result->TotalBytesSent);
	}
}

void print_config(struct tr143diag_config *config)
{
	char *wanip;
	char *serverip;

	if (err_flag.test_type)
		TR143_INFO("Test Type\t: %s", str_test_type(config->ttype));
	if (err_flag.connection)
		TR143_INFO("Connection\t: %d", config->nconn);
	if (err_flag.dscp)
		TR143_INFO("DSCP\t\t: %d", config->dscp);
	if (err_flag.test_mode)
		TR143_INFO("Test Mode\t: %d", config->tmode);
	if (err_flag.mode_value == 1) {
		if (config->tmode == 0)
			TR143_INFO("File Transfer\t: %lld bytes", config->mode.filesize);
		else if (config->tmode == 1)
			TR143_INFO("Time Transfer\t: %lld seconds", config->mode.duration);
	}
	if (err_flag.wanif)
		TR143_INFO("Wanif\t: %s", config->wanif);
	if (err_flag.wanip) {
		ip.s_addr = config->wanip.ip.ip4;
		wanip = inet_ntoa(ip);
		TR143_INFO("Wanip\t: %s", wanip);
	}
	if (err_flag.serverip) {
		if (config->serverip.ip_type == IPV6) {
			struct in6_addr ipv6_addr;
			char serverip[MAX_IP_LEN];
			memcpy(&ipv6_addr, &config->serverip.ip.ip6, sizeof(struct in6_addr));
			if (inet_ntop(AF_INET6, &ipv6_addr, serverip, INET6_ADDRSTRLEN))
				TR143_INFO("Server IPv6 address: %s", serverip);
			else
				TR143_ERROR("Failed to convert IPv6 address to string: %s", strerror(errno));
		} else if (config->serverip.ip_type == IPV4){
			struct in_addr ipv4_addr;
			char serverip[MAX_IP_LEN];
			ipv4_addr.s_addr = config->serverip.ip.ip4;
			if (inet_ntop(AF_INET, &ipv4_addr, serverip, INET_ADDRSTRLEN))
				TR143_INFO("Server IPv4 address: %s", serverip);
			else
				TR143_ERROR("Failed to convert IPv4 address to string: %s", strerror(errno));
		}
	}
	if (err_flag.serverport)
		TR143_INFO("Server Port	: %d", config->serverport);
	if (err_flag.servername)
		TR143_INFO("server name	: %s", config->servername);
	if (err_flag.url)
		TR143_INFO("url\t\t: %s", config->url);
	if (err_flag.inc_interval)
		TR143_INFO("Inc interval\t: %d", config->inc_interval);
}

int get_mode(char *optarg, union mode *mode, uint8_t tmode)
{
	char suffix[6][6] = {"s", "sec", "B", "KB", "MB", "GB"};
	char *b = strstr(optarg, suffix[2]);
	char *Kb = strstr(optarg, suffix[3]);
	char *Mb = strstr(optarg, suffix[4]);
	char *Gb = strstr(optarg, suffix[5]);
	if ((tmode == 1) && (strstr(optarg, suffix[0]) || strstr(optarg, suffix[1]))) 
		mode->duration  = atol(optarg);
	else if ((tmode == 0)&&(Gb||Kb||Mb||b)) {
		if (Gb)
			mode->filesize = atoll(optarg)*GB;
		else if(Mb)
			mode->filesize = atoll(optarg)*MB;
		else if(Kb)
			mode->filesize = atoll(optarg)*KB;
		else if(b)
			mode->filesize = atoll(optarg);
		else
			return FAILURE;
	} else
		return FAILURE;

	return SUCCESS;	
}

static int parse_configuration_params(int argc, char **argv,
		struct tr143diag_config *config)
{
	int opt = 0;
	int ret = 0;
	int type = 0;
	int t_mode = 0;
	int mode_value = 0;
	int option_index = 0;

	static struct option config_options[] = {
		{"ttype", required_argument, NULL, 't'},
		{"nconn", required_argument, NULL, 'c'},
		{"dscp", required_argument, NULL, 'd'},
		{"tmode", required_argument, NULL, 'm'},
		{"mvalue", required_argument, NULL, 'v'},
		{"wanif", required_argument, NULL, 'w'},
		{"wanip", required_argument, NULL, 'i'},
		{"serverip", required_argument, NULL, 's'},
		{"serverport", required_argument, NULL, 'p'},
		{"servername", required_argument, NULL, 'n'},
		{"url", required_argument, NULL, 'u'},
		{"interval", optional_argument, NULL, 'l'},
		{NULL, 0, NULL, 0}
	};

	while ((opt = getopt_long(argc, argv, CONFIG_OPT, config_options, &option_index)) != -1) {
		switch (opt)
		{
			case 't':
				if (err_flag.test_type == 1) {
					TR143_ERROR("duplicate value of test type");
					return FAILURE;
				}
				type = atoi(optarg);
				if (type != 0 && type != 1) {
					TR143_ERROR("invalid value of ttype it should be 0 or 1");
					return FAILURE;
				}
				config->ttype = type;
				err_flag.test_type = 1;
				break;
			case 'c':
				if (err_flag.connection == 1) {
					TR143_ERROR("duplicate value of connection");
					return FAILURE;
				}
				config->nconn = atoi(optarg);
				err_flag.connection = 1;
				break;
			case 'd':
				if (err_flag.dscp == 1) {
					TR143_ERROR("duplicate value of dscp");
					return FAILURE;
				}
				config->dscp = atoi(optarg);
				err_flag.dscp = 1;
				break;
			case 'm':
				if (err_flag.test_mode == 1) {
					TR143_ERROR("duplicate value of test mode");
					return FAILURE;
				}
				t_mode = atoi(optarg);
				if (t_mode != 0 && t_mode != 1 ){
					TR143_ERROR("Error : Invalid value of mode it should be 0 or 1");
					return FAILURE;
				}
				config->tmode = t_mode;
				err_flag.test_mode = 1;
				break;
			case 'v':
				if (err_flag.mode_value == 1) {
					TR143_ERROR("duplicate value of mode value");
					return FAILURE;
				}
				mode_value = get_mode(optarg, &config->mode, config->tmode);
				if (mode_value == -1) {
					if (t_mode == 0) {
						TR143_ERROR("Error : file mode value should be <NB>, <NKB>, <NMB>, <NGB>");
						return FAILURE;
					} else if (t_mode == 1) {
						TR143_ERROR("Error : time mode value should be <Ns>, <Nsec>");
						return FAILURE;
					}
				}
				err_flag.mode_value = 1;
				break;
			case 'w':
				if (err_flag.wanif == 1) {
					TR143_ERROR("duplicate value of WAN");
					return FAILURE;
				}
				if (strnlen_s(optarg,IFNAMSIZ) >= IFNAMSIZ) {
					TR143_ERROR("Error : WAMN interface name is not valid");
					return FAILURE;
				} else {
					strncpy_s(config->wanif,sizeof(config->wanif), optarg, IFNAMSIZ);
					err_flag.wanif = 1;
				}
				break;
			case 'i':
				if (err_flag.wanip == 1) {
					TR143_ERROR("duplicate value of WAN ip");
					return FAILURE;
				} else {
					if (isValidIpAddress(optarg, config)) {
						config->wanip.ip.ip4 = inet_addr(optarg);
						err_flag.wanip = 1;
					} else {
						TR143_ERROR("Invalid WAN ip");
						exit(0);
					}
				}
				break;
			case 's':
				if (err_flag.serverip == 1) {
					TR143_ERROR("duplicate value of server ip");
					return FAILURE;
				} else {
					if (isValidIpAddress(optarg, config)) {
						if (config->serverip.ip_type == IPV4) {
							config->serverip.ip.ip4 = inet_addr(optarg);
							err_flag.serverip = 1;
						} else if (config->serverip.ip_type == IPV6) {
							if (inet_pton(AF_INET6, optarg, &(config->serverip.ip.ip6)))
								err_flag.serverip = 1;
						} else {
							TR143_ERROR("Invalid server ip");
							exit(0);
						}
					}
				}
				break;
			case 'p':
				if (err_flag.serverport == 1) {
					TR143_ERROR("duplicate value of serverport");
					return FAILURE;
				}
				config->serverport = atoi(optarg);
				err_flag.serverport = 1;
				break;
			case 'n':
				if (err_flag.servername == 1) {
					TR143_ERROR("duplicate value of servername");
					return FAILURE;
				} else {
					if(strnlen_s(optarg,TR143_HOSTNAME_LEN)>=TR143_HOSTNAME_LEN){
					TR143_ERROR("Error : servername is not valid");
					return FAILURE;
					}
					else{
					strncpy_s(config->servername, sizeof(config->servername), optarg, TR143_HOSTNAME_LEN);
					err_flag.servername = 1;
					}
				}
				break;
			case 'u':
				if (err_flag.url == 1) {
					TR143_ERROR("duplicate value of url");
					return FAILURE;
				} else {
					if(strnlen_s(optarg,TR143_DIAG_URL_LEN)>=TR143_DIAG_URL_LEN){
					TR143_ERROR("Error : url is not valid");
					return FAILURE;
					}
					else{
					strncpy_s(config->url, sizeof(config->url), optarg, TR143_DIAG_URL_LEN);
					err_flag.url = 1;
					}
				}
				break;
			case 'l':
				if (optarg)
					config->inc_interval = atoi(optarg);
				else
					config->inc_interval = INC_INTERVAL;
				err_flag.inc_interval = 1;
				break;
			case ':':   /* missing option argument */
				fprintf(stderr, "%s: option `-%c' requires an argument\n", argv[0], optopt);
				break;
			default:
				TR143_ERROR("Invalid Input\n");
				return FAILURE;
		}
	}
#ifndef CONFIG_DNS_RESOLVER
	if (err_flag.url) {
		get_serverip_from_url(config);
	}
#endif
	ret = param_error();
	return ret;
}

void parse_notify_options(struct tr143diag_notify *notify, int argc,
		char **argv)
{
	int opt = 0;
	int option_index = 0;
	struct option long_options[] = {
		   {"bomtime", no_argument, NULL, 'b'},
		   {"eomtime", no_argument, NULL, 'e'},
		   {NULL, 0, NULL, 0}
		};

	while ((opt = getopt_long(argc, argv, "be", long_options,
					&option_index)) > 0) {
		switch (opt) {
			case 'b':
				notify->ntype = TR143_DIAG_NTFY_BOM;
				break;
			case 'e':
				notify->ntype = TR143_DIAG_NTFY_EOM;
				break;
			case '?':
				fprintf(stderr, "%s: option '-%c' is invalid\n", argv[0], optopt);
				exit(0);
			default:
				notify->ntype = TR143_DIAG_NTFY_NONE;
		}
	}
}

int main(int argc, char **argv)
{
	int ret = 0;
	int opt = 0;
	int flag = 0;
	int option_index = 0;
	struct tr143diag_notify notify = {0};
	struct tr143diag_result result = {0};
	struct tr143diag_config config = {0};
	struct tr143inc_result *inc_result = NULL;
	config.ttype = DEF_TYPE;
	config.nconn = DEF_CON;
	config.dscp = DEF_DSCP;
	config.tmode = DEF_MODE;
	config.serverport = DEF_PORT;

	static struct option long_options[] =
	{
		{"config",	no_argument,	NULL,	'C'},
		{"start",	no_argument,	NULL,	'S'},
		{"abort",	no_argument,	NULL,	'A'},
		{"result",	no_argument,	NULL,	'R'},
		{"inc_result",	no_argument,	NULL,	'I'},
		{"notify",	no_argument,	NULL,	'N'},
		{"help",	no_argument,	NULL,	'h'},
		{NULL,		0,				NULL,	0}
	};
	while ((opt = getopt_long(argc, argv, TR143DIAG_OPT, long_options, &option_index)) != -1) {
		switch (opt)
		{
			case 'C':
				if (flag > 0)
					break;
				flag++;
				if (!strcmp(argv[1], "-C") || !strcmp(argv[1], "--config")) {
					ret = parse_configuration_params(argc, argv, &config);
					if (ret < 0)
						break;
					print_config(&config);
					if (invoke_ioctl(TR143_DIAG_IOCTL_CONFIG, &config))
						fprintf(stderr, "config ioctl call failed\n");
				} else {
					fprintf(stderr, "%s: invalid option found\n", argv[0]);
					print_help();
					return FAILURE;
				}
				break;
			case 'S':
				if (flag > 0)
					break;
				flag++;
				if (argc == 3 && (!strcmp(argv[2], "-b") || !strcmp(argv[2], "-e"))) {
					parse_notify_options(&notify, argc, argv);
					if (invoke_ioctl(TR143_DIAG_IOCTL_START, &notify)) {
						fprintf(stderr, "invoke_ioctl failed\n");
						break;
					}
					if (notify.ntype == TR143_DIAG_NTFY_BOM)
						printf("BOMTime : %llu\n", notify.time);
					else if (notify.ntype == TR143_DIAG_NTFY_EOM)
						printf("EOMTime : %llu\n", notify.time);
					else if (notify.ntype == TR143_DIAG_NTFY_ERR)
						printf("operation failed, error_code:%s http_status_code:%d\n",
								tr143_error_name[notify.err_code], notify.http_code);
					else
						printf("speed test started\n");
				} else if (argc == 2 && (!strcmp(argv[1], "-S") || !strcmp(argv[1], "--start"))) {
					if (invoke_ioctl(TR143_DIAG_IOCTL_START, NULL)) {
						fprintf(stderr, "start ioctl command failed\n");
						break;
					} else
						printf("speed test started\n");
				} else {
					fprintf(stderr, "%s: invalid option found\n", argv[0]);
					print_help();
					return FAILURE;
				}
				break;
			case 'A':
				if (flag > 0)
					break;
				flag++;
				if (argc == 2 && (!strcmp(argv[1], "-A") || !strcmp(argv[1], "--abort"))) {
					if (invoke_ioctl(TR143_DIAG_IOCTL_ABORT, NULL))
						fprintf(stderr, "abort ioctl failed\n");
					break;
				} else {
					fprintf(stderr, "%s: invalid option found\n", argv[0]);
					print_help();
					return FAILURE;
				}
				break;
			case 'R':
				if (flag > 0)
					break;
				flag++;
				if (argc == 2 && (!strcmp(argv[1], "-R") || !strcmp(argv[1], "--result"))) {
					if (invoke_ioctl(TR143_DIAG_IOCTL_GETRESULT, &result)) {
						fprintf(stderr, "getresult ioctl call failed\n");
						break;
					}
				} else {
					fprintf(stderr, "%s: invalid option found\n", argv[0]);
					print_help();
					return FAILURE;
				}
				print_result(&result);
				result_throughput(&result);
				break;
			case 'I':
				if (flag > 0)
					break;
				flag++;
				if (argc == 2 && (!strcmp(argv[1], "-I") || !strcmp(argv[1], "--inc_result"))) {
					inc_result = malloc(sizeof(struct tr143inc_result) + 
							(sizeof(struct IncrementalResult) * TR143_MAX_INC_RESULTS));
					if (invoke_ioctl(TR143_DIAG_IOCTL_GET_INCRESULT, inc_result)) {
						fprintf(stderr, "get incremental results ioctl call failed\n");
						break;
					}
				} else {
					fprintf(stderr, "%s: invalid option found\n", argv[0]);
					print_help();
					return FAILURE;
				}
				if (inc_result) {
					print_inc_result(inc_result);
					free(inc_result);
				}
				break;
			case 'N':
				if (flag > 0)
					break;
				flag++;
				if (argc == 3 && (!strcmp(argv[2], "-b") || !strcmp(argv[2], "-e"))) {
					parse_notify_options(&notify, argc, argv);
					if (invoke_ioctl(TR143_DIAG_IOCTL_NOTIFY, &notify)) {
						fprintf(stderr, "notify ioctl call failed\n");
						break;
					}
					if (notify.ntype == TR143_DIAG_NTFY_BOM){
						printf("HTTP status code : %d\n", notify.http_code);
						printf("BOMTime : %llu\n", notify.time);
					} else if (notify.ntype == TR143_DIAG_NTFY_EOM) {
						printf("HTTP status code : %d\n", notify.http_code);
						printf("EOMTime :%llu\n", notify.time);
					} else if (notify.ntype == TR143_DIAG_NTFY_ERR) {
						printf("operation failed, error_code:%s http_status_code:%d\n",
								tr143_error_name[notify.err_code], notify.http_code);
					}
				} else {
					fprintf(stderr, "%s: invalid option found\n", argv[0]);
					print_help();
					return FAILURE;
				}
				break;
			case 'h':
				print_help();
				break;
			default:
				print_help();
				return FAILURE;
		}
	}
	if (flag == 0) {
		fprintf(stderr, "%s: required an argument\n", argv[0]);
		print_help();
	}
	return SUCCESS;
}
