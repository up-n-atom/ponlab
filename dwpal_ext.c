/******************************************************************************

         Copyright (c) 2020, MaxLinear, Inc.
         Copyright 2016 - 2020 Intel Corporation

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

*******************************************************************************/

/*  *****************************************************************************
 *         File Name    : dwpal_ext.c                             	        *
 *         Description  : D-WPAL Extender control interface 		        *
 *                                                                              *
 *  *****************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <time.h>

#if defined YOCTO
#include <slibc/string.h>
#include <slibc/stdio.h>
#else
#include <stddef.h>
#include "libsafec/safe_str_lib.h"
#include "libsafec/safe_mem_lib.h"
#endif

#include "dwpal_ext.h"
#include "dwpal_os.h"

#include "strcmp.h"
/* list of banned SDL methods */
#define strlen(...)  	SDL_BANNED_FUNCTION ERROR_TOKEN
#define strtok(...)  	SDL_BANNED_FUNCTION ERROR_TOKEN
#define strcat(...)  	SDL_BANNED_FUNCTION ERROR_TOKEN
#define strncat(...) 	SDL_BANNED_FUNCTION ERROR_TOKEN
#define strcpy(...)  	SDL_BANNED_FUNCTION ERROR_TOKEN
#define strncpy(...) 	SDL_BANNED_FUNCTION ERROR_TOKEN
#define snprintf(...)	SDL_BANNED_FUNCTION ERROR_TOKEN

/* Dedicated thread for hostapd event callbacks */
//#define EVENT_CALLBACK_THREAD

#define PING_CHECK_TIME 3
#define RECOVERY_RETRY_TIME 1
#define EVENT_HANDLER_SOCKET "/tmp/dwpal_event_handler_socket"


typedef enum
{
	THREAD_CANCEL = 0,
	THREAD_CREATE
} DwpalThreadOperation;

typedef struct
{
	char                        VAPName[DWPAL_VAP_NAME_STRING_LENGTH];
	int                         fd, fdCmdGet;
	bool                        isConnectionEstablishNeeded;
	bool                        isReconnectEventNeeded;
	DwpalExtHostapEventCallback hostapEventCallback;
	DwpalExtNlEventCallback     nlEventCallback, nlCmdGetCallback;
	DwpalExtNlNonVendorEventCallback nlNonVendorEventCallback;
	DwpalConnectionType         connectionType;
} DwpalService;

typedef struct
{
	int    serviceIdx;
	char   VAPName[DWPAL_VAP_NAME_STRING_LENGTH];
	char   opCode[64];
	char   msg[HOSTAPD_TO_DWPAL_MSG_LENGTH];
	size_t msgStringLen;
} EventData;

typedef struct
{
	pthread_t    threadID;
	int          pipeFDs[2];
	volatile int threadShouldStop;
} threadData_t;

typedef void *(*ThreadEntryFunc)(void *data);  /* thread entry function */


bool isCliPrintf = false;

static DwpalService *dwpalService[NUM_OF_SUPPORTED_VAPS + 1] = { [0 ... NUM_OF_SUPPORTED_VAPS ] = NULL };  /* add 1 place for NL */
static void *context[ARRAY_SIZE(dwpalService)]= { [0 ... (ARRAY_SIZE(dwpalService) - 1) ] = NULL };
static threadData_t g_listenerThreadInfo;
#ifndef DISABLE_DWPAL_HOSTAP_SUPPORT
static threadData_t g_monitorThreadInfo;
#endif /* DISABLE_DWPAL_HOSTAP_SUPPORT */
static pthread_mutex_t context_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t attach_mutex = PTHREAD_MUTEX_INITIALIZER;

static pthread_mutex_t nl_cmd_mutex = PTHREAD_MUTEX_INITIALIZER;
static unsigned char *nl_response_data = NULL;
static size_t *nl_response_len = NULL;
static bool nl_response_received = false;
static bool nl_response_save_data = false;

#if defined EVENT_CALLBACK_THREAD
static int dwpal_event_handler = (-1);
static pthread_t eventHandlerThreadId = (pthread_t)0;
#endif

static const size_t numOfServices = ARRAY_SIZE(dwpalService);

static const char* connectionTypeToStr(DwpalConnectionType connectionType)
{
	switch (connectionType) {
		case DWPAL_CONN_TYPE_HOSTAP:
			return "hostap";
		case DWPAL_CONN_TYPE_DRIVER:
			return "driver";
		default:
			return "unknown";
	}
}

static DWPAL_Ret interfaceIndexGet(DwpalConnectionType connectionType, const char *VAPName, int *idx)
{
	unsigned i;
	*idx = 0;

	for (i = 0; i < numOfServices; i++)
	{
		if (dwpalService[i] == NULL)
			continue;

		if ((connectionType == dwpalService[i]->connectionType) &&
		    (!strncmp(VAPName, dwpalService[i]->VAPName, DWPAL_VAP_NAME_STRING_LENGTH)) )
		{
			*idx = i;
			return DWPAL_SUCCESS;
		}
	}

	return DWPAL_INTERFACE_IS_DOWN;
}


static DWPAL_Ret interfaceIndexCreate(DwpalConnectionType connectionType, const char *VAPName, int *idx)
{
	unsigned i;

	*idx = 0;

	if (interfaceIndexGet(connectionType, VAPName, idx) == DWPAL_SUCCESS)
	{
		void* l_context;
		console_printf("%s; the interface's database (connectionType= '%s', VAPName= '%s') is already exist ==> check if the interface is already up\n",
		            __FUNCTION__, connectionTypeToStr(connectionType), VAPName);

		/* Even if 'idx' already exist, the attach may have failed ==> check if the attach succeeded */
		MUTEX_LOCK(&context_mutex);
		l_context = context[*idx];
		MUTEX_UNLOCK(&context_mutex);

		if (l_context == NULL)
		{  /* the attach failed, meaning the interface is down but the rest of 'dwpalService' is ready */
			return DWPAL_SUCCESS;
		}
		else
		{  /* the attach succeeded, meaning the interface is already up */
			return DWPAL_INTERFACE_ALREADY_UP;
		}
	}

	/* Getting here means that the interface does NOT exist ==> create it! */
	for (i = 0; i < numOfServices; i++)
	{
		if (dwpalService[i] == NULL)
		{  /* First empty entry ==> use it */
			dwpalService[i] = (DwpalService *)calloc(1, sizeof(DwpalService));
			if (dwpalService[i] == NULL)
			{
				console_printf("%s; malloc failed ==> Abort!\n", __FUNCTION__);
				return DWPAL_FAILURE;
			}

			dwpalService[i]->connectionType = connectionType;
			strcpy_s(dwpalService[i]->VAPName, sizeof(dwpalService[i]->VAPName), VAPName);

			*idx = i;
			return DWPAL_SUCCESS;
		}
	}

	console_printf("%s; number of interfaces (%d) reached its limit ==> Abort!\n", __FUNCTION__, i);

	return DWPAL_FAILURE;
}


static bool isAnyInterfaceActive(void)
{
	unsigned i;

	/* check if there are active interfaces */
	for (i = 0; i < numOfServices; i++)
	{
		/* In case that there is a valid context, break! */
		if (dwpalService[i] != NULL)
		{
			return true;
		}
	}

	return false;
}


#if defined EVENT_CALLBACK_THREAD
static DWPAL_Ret socket_data_send(char *socketPrefixName, char *data, size_t size)
{
    int                	fd = -1, byte;
	struct sockaddr_un 	un;
	size_t             	len;
	char               	socketName[SOCKET_NAME_LENGTH] = "\0";
	pid_t              	pid = getpid();
	int					status;

	//console_printf("%s Entry; socketPrefixName= '%s', size= %d\n", __FUNCTION__, socketPrefixName, size);

	/* create a UNIX domain stream socket */
	if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
	{
		console_printf("%s; create socket fail; pid= %d; errno= %d ('%s')\n", __FUNCTION__, pid, errno, strerror(errno));
		return DWPAL_FAILURE;
    }

	/* fill socket address structure with server's address */
	memset(&un, 0, sizeof(un));
	un.sun_family = AF_UNIX;

	status = sprintf_s(socketName, sizeof(socketName), "%s_%d", socketPrefixName, pid);
	if (status <= 0)
		console_printf("%s; sprintf_s failed!\n", __FUNCTION__);

	strcpy_s(un.sun_path, sizeof(un.sun_path), socketName);
	len = offsetof(struct sockaddr_un, sun_path) + strnlen_s(socketName, sizeof(socketName));

	if (connect(fd, (struct sockaddr *)&un, len) < 0)
	{
		console_printf("%s; connect() fail; pid= %d; errno= %d ('%s')\n",
		       __FUNCTION__, pid, errno, strerror(errno));

		if (close(fd) == (-1))
		{
			console_printf("%s; close() fail; pid= %d; errno= %d ('%s')\n",
				   __FUNCTION__, pid, errno, strerror(errno));
		}

		return DWPAL_FAILURE;
	}

	if ((byte = write(fd, data, size)) == -1)
	{
		console_printf("%s; write() fail; pid= %d; errno= %d ('%s')\n",
		       __FUNCTION__, pid, errno, strerror(errno));

		if (close(fd) == (-1))
		{
			console_printf("%s; close() fail; pid= %d; errno= %d ('%s')\n",
				   __FUNCTION__, pid, errno, strerror(errno));
		}

		return DWPAL_FAILURE;
	}

	if (close(fd) == (-1))
	{
		console_printf("%s; close() fail; pid= %d; errno= %d ('%s')\n",
		       __FUNCTION__, pid, errno, strerror(errno));
	}

	return DWPAL_SUCCESS;
}
#endif

#ifndef DISABLE_DWPAL_HOSTAP_SUPPORT
static DWPAL_Ret activeVapsGet(const char *VAPName, char *vaps, size_t sizeOfVaps)
{
	int j;
	size_t replyLen = HOSTAPD_TO_DWPAL_MSG_LENGTH * sizeof(char) - 1;
	char *reply;
	char stringToSearch[DWPAL_VAP_NAME_STRING_LENGTH] = "\0";
	int status;

	console_printf("%s; Entry\n", __FUNCTION__);

	if ( (VAPName == NULL) || (vaps == NULL) )
	{
		console_printf("%s; VAPName and/or vaps is NULL ==> Abort!\n", __FUNCTION__);
		return DWPAL_FAILURE;
	}

	reply = (char *)calloc(HOSTAPD_TO_DWPAL_MSG_LENGTH, sizeof(char));
	if (reply == NULL)
	{
		console_printf("%s; reply malloc failed\n", __FUNCTION__);
		return DWPAL_FAILURE;
	}

	if (dwpal_ext_hostap_cmd_send(VAPName, "STATUS", NULL, reply, &replyLen) == DWPAL_SUCCESS)
	{
		char          bss[DWPAL_VAP_NAME_STRING_LENGTH + 1];
		size_t        numOfValidArgs;
		FieldsToParse fieldsToParse[2] = {
			{ &bss, &numOfValidArgs, DWPAL_STR_PARAM, stringToSearch, sizeof(bss) },
			{ NULL, NULL, DWPAL_NUM_OF_PARSING_TYPES, NULL, 0 }
		};

		console_printf("%s; replyLen= %zu, reply= '%s'\n", __FUNCTION__, replyLen, reply);

		for (j=0; j < NUM_OF_SUPPORTED_VAPS; j++)
		{
			status = sprintf_s(stringToSearch, sizeof(stringToSearch), "bss[%d]=", j);
			if (status <= 0)
				console_printf("%s; sprintf_s failed!\n", __FUNCTION__);

			if (dwpal_string_to_struct_parse(reply, replyLen, fieldsToParse, sizeof(bss)) != DWPAL_SUCCESS)
			{
				console_printf("%s; dwpal_string_to_struct_parse ERROR ==> Break!\n", __FUNCTION__);
				break;
			}

			if (numOfValidArgs == 0)
			{
				// console_printf("%s; (numOfValidArgs == 0) ==> Break!\n", __FUNCTION__);
				break;
			}

			if (vaps[0] == '\0')
			{
				strncpy_s(vaps, sizeOfVaps, "vaps=", sizeof("vaps=") - 1);
			}

			strcat_s(vaps, sizeOfVaps, " ");
			strcat_s(vaps, sizeOfVaps, bss);
		}
	}
	else
	{
		console_printf("%s; dwpal_ext_hostap_cmd_send ERROR\n", __FUNCTION__);
		free(reply);
		return DWPAL_FAILURE;
	}
	free(reply);

	return DWPAL_SUCCESS;
}

static DWPAL_Ret interfaceEventSend(uint serviceIdx, char* opCode, char* msg, size_t msgStringLen)
{
#if defined EVENT_CALLBACK_THREAD
	{
		EventData eventData;
		memset(&eventData, 0, sizeof(eventData));
		eventData.serviceIdx = serviceIdx;
		strcpy_s(eventData.VAPName, sizeof(eventData.VAPName), dwpalService[serviceIdx]->VAPName);
		strcpy_s(eventData.opCode, sizeof(eventData.opCode), opCode);
		if (msg) {
			if (strncpy_s(eventData.msg, sizeof(eventData.msg), msg, msgStringLen)) {
				console_printf("%s; copying message to buffer failed ==> abort...\n", __FUNCTION__);
				return DWPAL_FAILURE;
			}
		}
		eventData.msgStringLen = msgStringLen;

		/* Send the event via the callback */
		if (socket_data_send(EVENT_HANDLER_SOCKET, (char *)&eventData, sizeof(EventData)) == DWPAL_FAILURE)
		{
			console_printf("%s; socket_data_send failed, opCode '%s' ==> cont...\n", __FUNCTION__, opCode);
		}
	}
#else
	if (NULL != dwpalService[serviceIdx]->hostapEventCallback) {
		dwpalService[serviceIdx]->hostapEventCallback(dwpalService[serviceIdx]->VAPName,
			opCode, msg, msgStringLen);
	}
#endif

	return DWPAL_SUCCESS;
}

static DWPAL_Ret interfaceDisconnectedSend(uint serviceIdx)
{
	return interfaceEventSend(serviceIdx, "INTERFACE_DISCONNECTED", NULL, 0);
}

static DWPAL_Ret interfaceConnectedOkSend(uint serviceIdx)
{
	char vaps[NUM_OF_SUPPORTED_VAPS * (DWPAL_VAP_NAME_STRING_LENGTH + 1) + sizeof("vaps=")] = "\0";

	if (activeVapsGet(dwpalService[serviceIdx]->VAPName, vaps, sizeof(vaps)) == DWPAL_FAILURE)
	{
		console_printf("%s; activeVapsGet failed ==> cont...\n", __FUNCTION__);
		return DWPAL_FAILURE;
	}

	return interfaceEventSend(serviceIdx, "INTERFACE_CONNECTED_OK", vaps, strnlen_s(vaps, sizeof(vaps) - 1));
}

static DWPAL_Ret interfaceReconnectedOkSent(uint serviceIdx)
{
	char vaps[NUM_OF_SUPPORTED_VAPS * (DWPAL_VAP_NAME_STRING_LENGTH + 1) + sizeof("vaps=")] = "\0";

	if (activeVapsGet(dwpalService[serviceIdx]->VAPName, vaps, sizeof(vaps)) == DWPAL_FAILURE)
	{
		console_printf("%s; activeVapsGet failed ==> cont...\n", __FUNCTION__);
		return DWPAL_FAILURE;
	}

	return interfaceEventSend(serviceIdx, "INTERFACE_RECONNECTED_OK", vaps, strnlen_s(vaps, sizeof(vaps) - 1));
}

static void interfacesRecoverIfNeeded(void)
{
	unsigned i;
	DWPAL_Ret ret;

	//console_printf("%s Entry\n", __FUNCTION__);

	for (i = 0; i < numOfServices; i++)
	{
		/* In case that there is no valid context, continue... */
		if (dwpalService[i] == NULL)
		{
			continue;
		}

		/* In case of recovery needed, try recover; in case of interface init, try to establish the connection */
		if (DWPAL_CONN_TYPE_HOSTAP == dwpalService[i]->connectionType)
		{
			MUTEX_LOCK(&context_mutex);
			if (dwpalService[i]->isConnectionEstablishNeeded)
			{
				/* If needed, close 'wpaCtrlPtr' and free 'context' (probably it was performed already by interfacesPingCheck) */
				if ( (context[i] != NULL) && (dwpal_hostap_interface_detach(&context[i] /*OUT*/) != DWPAL_SUCCESS) )
				{
					console_printf("%s; dwpal_hostap_interface_detach (VAPName= '%s') returned ERROR ==> cont...\n", __FUNCTION__, dwpalService[i]->VAPName);
				}

				ret = dwpal_hostap_interface_attach(&context[i] /*OUT*/, dwpalService[i]->VAPName, NULL /*use one-way interface*/);

				if (ret == DWPAL_SUCCESS) {
					dwpalService[i]->isConnectionEstablishNeeded = false;
					dwpalService[i]->isReconnectEventNeeded = true;
					console_printf("%s; VAPName= '%s' interface recovered successfully!\n", __FUNCTION__, dwpalService[i]->VAPName);
				}
			}
			MUTEX_UNLOCK(&context_mutex);

			if (dwpalService[i]->isReconnectEventNeeded)
			{
				if (interfaceReconnectedOkSent(i) != DWPAL_SUCCESS) {
					console_printf("%s: interfaceReconnectedOkSent failed ==> cont...\n", __FUNCTION__);
				} else {
					dwpalService[i]->isReconnectEventNeeded = false;
				}
			}
		}
	}
}

static void interfacesPingCheck(void)
{
	unsigned i;
	char   reply[HOSTAPD_TO_DWPAL_SHORT_REPLY_LENGTH];
	size_t replyLen = sizeof(reply) - 1;

	//console_printf("%s Entry\n", __FUNCTION__);

	for (i = 0; i < numOfServices; i++)
	{
		/* In case that there is no valid context, continue... */
		if (context[i] == NULL)
		{
			continue;
		}

		if (DWPAL_CONN_TYPE_HOSTAP == dwpalService[i]->connectionType)
		{
			if (dwpalService[i]->fd > 0)
			{
				/* check if interface that should exist, still exists */
				replyLen = sizeof(reply) - 1;
				if (dwpal_ext_hostap_cmd_send(dwpalService[i]->VAPName, "PING", NULL, reply, &replyLen) == DWPAL_FAILURE)
				{
					dwpalService[i]->fd = -1;

					MUTEX_LOCK(&context_mutex);
					if (dwpalService[i]->isConnectionEstablishNeeded == false)
					{
						/* dwpal_ext_hostap_cmd_send() failed for a reason other than sending error,
						   so need to "detach" here */
						console_printf("%s; VAPName= '%s' interface needs to be recovered\n", __FUNCTION__, dwpalService[i]->VAPName);
						dwpalService[i]->isConnectionEstablishNeeded = true;

						/* Close 'wpaCtrlPtr', and free 'context' */
						if ( (context[i] != NULL) && (dwpal_hostap_interface_detach(&context[i]) == DWPAL_FAILURE) )
						{
							console_printf("%s; dwpal_hostap_interface_detach (VAPName= '%s') returned ERROR ==> cont...\n", __FUNCTION__, dwpalService[i]->VAPName);
						}
						MUTEX_UNLOCK(&context_mutex);
						interfaceDisconnectedSend(i);
					}
					else {
						MUTEX_UNLOCK(&context_mutex);
					}
				}
			}
		}
	}
}
#endif /* DISABLE_DWPAL_HOSTAP_SUPPORT */

#if defined EVENT_CALLBACK_THREAD
static void *eventHandlerThreadStart(void *temp)
{
	int                clientfd = -1, byte;
	size_t             len;
	char               rcv_buf[sizeof(EventData)];
	EventData          *eventData = NULL;
	struct sockaddr_un un;

	(void)temp;

	console_printf("%s Entry\n", __FUNCTION__);

	if (dwpal_event_handler == -1)
	{
		console_printf("%s; dwpal_event_handler not valid ==> Abort!\n", __FUNCTION__);
		return NULL;
	}

	/* Receive the msg */
	while (true)
	{
		/* Handle the COMMAND_SOCKET */
		len = sizeof(un);
		if ((clientfd = accept(dwpal_event_handler, (struct sockaddr *)&un, &len)) < 0)
		{ // wait for client to connect to me (server)
			console_printf_err("%s; ERROR: accept() received; clientfd= %d; errno= %d ('%s') ==> cont...\n", __FUNCTION__, clientfd, errno, strerror(errno));
			continue;
		}

		memset(rcv_buf, '\0', sizeof(rcv_buf));

		if ((byte = read(clientfd, rcv_buf, sizeof(rcv_buf))) == -1)
		{   // wait for client to write/send data to me (server)
			console_printf("%s; read() fail; errno= %d ('%s')\n", __FUNCTION__, errno, strerror(errno));

			if (close(clientfd) == (-1))
			{
				console_printf("%s; close() fail; clientfd= %d; errno= %d ('%s')\n", __FUNCTION__, clientfd, errno, strerror(errno));
			}
			continue;
		}

		if (close(clientfd) == (-1))
		{
			console_printf("%s; close() fail; clientfd= %d; errno= %d ('%s')\n", __FUNCTION__, clientfd, errno, strerror(errno));
		}

		if ( (byte > (int)sizeof(EventData)) || (byte < 1) )
		{
			console_printf_err("%s; ERROR: read() returned buffer size (%d) out of range (1 <-> %d) ==> cont...\n", __FUNCTION__, byte, sizeof(EventData));
			continue;
		}

		eventData = (EventData *)rcv_buf;

		dwpalService[eventData->serviceIdx]->hostapEventCallback(eventData->VAPName, eventData->opCode, eventData->msg, eventData->msgStringLen);
	}

	return NULL;
}
#endif


static void *listenerThreadStart(void *temp)
{
	unsigned i;
	int     highestValFD, ret;
	fd_set  rfds;
	struct  timeval tv;

#ifndef DISABLE_DWPAL_HOSTAP_SUPPORT
	char    *msg;
	size_t  msgLen, msgStringLen;
	char    opCode[64];
#endif /* DISABLE_DWPAL_HOSTAP_SUPPORT */

	(void)temp;

	console_printf("%s Entry\n", __FUNCTION__);

	/* Receive the msg */
	while (!g_listenerThreadInfo.threadShouldStop)
	{
		FD_ZERO(&rfds);

		FD_SET(g_listenerThreadInfo.pipeFDs[0], &rfds);
		highestValFD = g_listenerThreadInfo.pipeFDs[0];

		for (i = 0; i < numOfServices; i++)
		{
			/* In case that there is no valid context, or no valid service, continue... */
			if ( (context[i] == NULL) || (dwpalService[i] == NULL) )
			{
				continue;
			}
#ifndef DISABLE_DWPAL_HOSTAP_SUPPORT
			if (DWPAL_CONN_TYPE_HOSTAP == dwpalService[i]->connectionType)
			{
				if (dwpal_hostap_event_fd_get(context[i], &dwpalService[i]->fd) == DWPAL_FAILURE)
				{
					/*console_printf("%s; dwpal_hostap_event_fd_get returned error ==> cont. (VAPName= '%s')\n",
					       __FUNCTION__, dwpalService[i]->VAPName);*/
					continue;
				}

				if (dwpalService[i]->fd > 0)
				{
					FD_SET(dwpalService[i]->fd, &rfds);
					highestValFD = (dwpalService[i]->fd > highestValFD)? dwpalService[i]->fd : highestValFD;  /* find the highest value fd */
				}
			}
			else
#endif /* DISABLE_DWPAL_HOSTAP_SUPPORT */
			if (DWPAL_CONN_TYPE_DRIVER == dwpalService[i]->connectionType)
			{
				if (dwpal_driver_nl_fd_get(context[i], &dwpalService[i]->fd, &dwpalService[i]->fdCmdGet) == DWPAL_FAILURE)
				{
					/*console_printf("%s; dwpal_driver_nl_fd_get returned error ==> cont. (VAPName= '%s')\n",
					       __FUNCTION__, dwpalService[i].VAPName);*/
					continue;
				}

				if (dwpalService[i]->fd > 0)
				{
					FD_SET(dwpalService[i]->fd, &rfds);
					highestValFD = (dwpalService[i]->fd > highestValFD)? dwpalService[i]->fd : highestValFD;  /* find the highest value fd */
				}
			}
		}

		//console_printf("%s; highestValFD= %d\n", __FUNCTION__, highestValFD);

		/* Interval of time in which the select() will be released */
		tv.tv_sec = 1;
		tv.tv_usec = 0;

		/* In case that no active hostap is available, highestValFD is '0' and we'll loop out according to tv values */
		ret = select(highestValFD + 1, &rfds, NULL, NULL, &tv);
		if (ret < 0)
		{
			console_printf("%s; select() return value= %d ==> cont...; errno= %d ('%s')\n", __FUNCTION__, ret, errno, strerror(errno));
			continue;
		}

		if (FD_ISSET(g_listenerThreadInfo.pipeFDs[0], &rfds))
		{
			char pipedMsg[16];

			console_printf("%s; received message from main thread => shutting down\n", __FUNCTION__);

			if ((ret = read(g_listenerThreadInfo.pipeFDs[0], pipedMsg, sizeof(pipedMsg))) < 0)
			{
				console_printf("%s; read() return value= %d ==> cont...; errno= %d ('%s')\n", __FUNCTION__, ret, errno, strerror(errno));
			}
			break;
		}

		for (i = 0; i < numOfServices; i++)
		{
			/* In case that there is no valid context, or no valid service, continue... */
			if ( (context[i] == NULL) || (dwpalService[i] == NULL) )
			{
				continue;
			}
#ifndef DISABLE_DWPAL_HOSTAP_SUPPORT
			if ((DWPAL_CONN_TYPE_HOSTAP == dwpalService[i]->connectionType) &&
			    (dwpalService[i]->fd > 0) &&
			    (FD_ISSET(dwpalService[i]->fd, &rfds))) {
				/*console_printf("%s; event received; connectionType= '%s', VAPName= '%s'\n",
				       __FUNCTION__, connectionTypeToStr(dwpalService[i]->connectionType), dwpalService[i]->VAPName);*/
				msg = (char *)malloc((size_t)(HOSTAPD_TO_DWPAL_MSG_LENGTH * sizeof(char)));
				if (msg == NULL)
				{
					console_printf("%s; invalid input ('msg') parameter ==> cont...\n", __FUNCTION__);
					continue;
				}
				memset(msg, 0, HOSTAPD_TO_DWPAL_MSG_LENGTH * sizeof(char));  /* Clear the output buffer */
				memset(opCode, 0, sizeof(opCode));
				msgLen = HOSTAPD_TO_DWPAL_MSG_LENGTH - 1;  //was "msgLen = HOSTAPD_TO_DWPAL_MSG_LENGTH;"
				if (dwpal_hostap_event_get(context[i], msg /*OUT*/, &msgLen /*IN/OUT*/, opCode /*OUT*/) == DWPAL_FAILURE)
				{
					console_printf("%s; dwpal_hostap_event_get ERROR; VAPName= '%s', msgLen= %zu\n",
					       __FUNCTION__, dwpalService[i]->VAPName, msgLen);

					/* Trigger the recovery of iface immediately if needed */
					if (write(g_monitorThreadInfo.pipeFDs[1], "R", 1) < 0)
						console_printf("%s; write to g_monitorThreadInfo->pipeFDs[1] FAILED (errno= %d)\n", __FUNCTION__, errno);
				}
				else
				// ThreadShouldStop may be changed during select() call. Don't call event handler if thread is cancelling
				if (!g_listenerThreadInfo.threadShouldStop)
				{
					//console_printf("%s; msgLen= %d, msg= '%s'\n", __FUNCTION__, msgLen, msg);
					msgStringLen = strnlen_s(msg, HOSTAPD_TO_DWPAL_MSG_LENGTH);
					//console_printf("%s; opCode= '%s', msg= '%s'\n", __FUNCTION__, opCode, msg);
					
					if (opCode[0])
						interfaceEventSend(i, opCode, msg, msgStringLen);
				}
				free(msg);
			}
			else 
#endif /* DISABLE_DWPAL_HOSTAP_SUPPORT */
			if ((DWPAL_CONN_TYPE_DRIVER == dwpalService[i]->connectionType) &&
				 (dwpalService[i]->fd > 0) &&
				 (FD_ISSET(dwpalService[i]->fd, &rfds)))
			{
				console_printf("%s; event received; connectionType= '%s', VAPName= '%s'\n",
						__FUNCTION__, connectionTypeToStr(dwpalService[i]->connectionType), dwpalService[i]->VAPName);

				if (dwpal_driver_nl_msg_get(context[i], DWPAL_NL_UNSOLICITED_EVENT, dwpalService[i]->nlEventCallback, dwpalService[i]->nlNonVendorEventCallback) == DWPAL_FAILURE)
				{
					console_printf("%s; dwpal_driver_nl_msg_get ERROR\n", __FUNCTION__);
				}
			}
		}
	}

	console_printf("%s; exit\n", __FUNCTION__);
	return NULL;
}

#ifndef DISABLE_DWPAL_HOSTAP_SUPPORT

static void *monitorThreadStart(void *temp)
{
	int    highestValFD, ret;
	bool   isInterfacesPingCheck = false;
	fd_set rfds;
	struct timeval tv;
	long last_ping_check, last_recovery_time, current_time;

	(void)temp;

	console_printf("%s Entry\n", __FUNCTION__);

	last_ping_check = dwpal_get_uptime();
	last_recovery_time = last_ping_check;

	while (!g_monitorThreadInfo.threadShouldStop)
	{
		FD_ZERO(&rfds);

		FD_SET(g_monitorThreadInfo.pipeFDs[0], &rfds);
		highestValFD = g_monitorThreadInfo.pipeFDs[0];

		tv.tv_sec = 1;
		tv.tv_usec = 0;

		ret = select(highestValFD + 1, &rfds, NULL, NULL, &tv);
		if (ret < 0) {
			console_printf("%s; select() return value= %d ==> cont...; errno= %d ('%s')\n", __FUNCTION__, ret, errno, strerror(errno));
			continue;
		}

		if (FD_ISSET(g_monitorThreadInfo.pipeFDs[0], &rfds)) {
			char pipedMsg[16] = {0};

			if ((ret = read(g_monitorThreadInfo.pipeFDs[0], pipedMsg, sizeof(pipedMsg))) < 0)
			{
				console_printf("%s; read() return value= %d ==> cont...; errno= %d ('%s')\n", __FUNCTION__, ret, errno, strerror(errno));
				break;
			}

			if ('R' == pipedMsg[0])
				isInterfacesPingCheck = true;
			else
				break; /* Stop the thread */
		}

		current_time = dwpal_get_uptime();
		if (isInterfacesPingCheck || ((current_time - last_ping_check) >= PING_CHECK_TIME)) {
			last_ping_check = current_time;
			interfacesPingCheck();
		}

		if ((current_time - last_recovery_time) >= RECOVERY_RETRY_TIME) {
			last_recovery_time = current_time;
			interfacesRecoverIfNeeded();
		}
	}

	console_printf("%s; exit\n", __FUNCTION__);
	return NULL;
}
#endif /* DISABLE_DWPAL_HOSTAP_SUPPORT */

static DWPAL_Ret threadCreate(threadData_t *threadInfo, ThreadEntryFunc threadEntryFunc)
{
	int            ret;
	DWPAL_Ret      dwpalRet = DWPAL_SUCCESS;
	//void           *res;

	console_printf("%s Entry\n", __FUNCTION__);

	if (pipe(threadInfo->pipeFDs) == -1)
	{
		console_printf("%s; pipe ERROR (errno= %d) ==> Abort!\n", __FUNCTION__, errno);
		return DWPAL_FAILURE;
	}

	/* Switch the output pipe to non-blocking mode to avoid pipe read operation stall */
	if (fcntl(threadInfo->pipeFDs[0], F_SETFL, O_NONBLOCK) == -1)
	{
		console_printf("%s; error set pipe mode, (errno= %d) ==> Abort!\n", __FUNCTION__, errno);
		dwpalRet = DWPAL_FAILURE;
	}

	if (dwpalRet == DWPAL_SUCCESS)
	{
		threadInfo->threadShouldStop = 0; /* must be 0 before creating the listener Thread */

		console_printf("%s; call pthread_create\n", __FUNCTION__);
		ret = pthread_create(&threadInfo->threadID, NULL, threadEntryFunc, NULL /*can be used to send params*/);
		if (ret != 0)
		{
			console_printf("%s; pthread_create ERROR (ret= %d) ==> Abort!\n", __FUNCTION__, ret);
			dwpalRet = DWPAL_FAILURE;
		}
		console_printf("%s; return from call pthread_create, ret= %d\n", __FUNCTION__, ret);

		if (dwpalRet == DWPAL_SUCCESS)
		{
#if 0
			/* Causing the thread to be joined with the main process;
			   meaning, the process will suspend due to the thread suspend.
			   Otherwise, when process ends, the thread ends as well (although it is suspended and waiting ) */
			ret = pthread_join(*thread_id, &res);
			if (ret != 0)
			{
				console_printf("%s; pthread_join ERROR (ret= %d) ==> Abort!\n", __FUNCTION__, ret);
				dwpalRet = DWPAL_FAILURE;
			}

			free(res);  /* Free memory allocated by thread */
#endif
		}
	}

	if (dwpalRet != DWPAL_SUCCESS)
	{
		if (close(threadInfo->pipeFDs[0]) != 0)
		{
			console_printf("%s; close() to threadInfo->pipeFDs[0] FAILED (errno= %d)\n", __FUNCTION__, errno);
		}
		if (close(threadInfo->pipeFDs[1]) != 0)
		{
			console_printf("%s; close() to threadInfo->pipeFDs[1] FAILED (errno= %d)\n", __FUNCTION__, errno);
		}
	}

	/* sleep for 100 ms - it is needed in case of a loop of thread create/cancel */
	usleep(100000 /*micro-sec*/);

	return dwpalRet;
}


static DWPAL_Ret threadSet(threadData_t *threadInfo, DwpalThreadOperation threadOperation, ThreadEntryFunc threadEntryFunc)
{
	int ret;

	switch (threadOperation)
	{
		case THREAD_CREATE:
			if (threadInfo->threadID == 0)
			{
				if (threadEntryFunc == NULL)
				{
					return DWPAL_FAILURE;
				}

				return threadCreate(threadInfo, threadEntryFunc);
			}
			break;

		case THREAD_CANCEL:
			if (threadInfo->threadID != 0)
			{
				if (pthread_self() == threadInfo->threadID)
				{
					console_printf("%s; trying cancelling the thread from the same thread ==> Abort!\n", __FUNCTION__);
					return DWPAL_SUCCESS;
				}

				/* tell the Thread to stop, both with flag, and message.
				   While threadShouldStop is set to 1, no new "hostap_send" operations are available
				 */
				threadInfo->threadShouldStop = 1;
				if (write(threadInfo->pipeFDs[1], "C", 1) < 0)
				{
					console_printf("%s; write to threadInfo->pipeFDs[1] FAILED (errno= %d)\n", __FUNCTION__, errno);
				}

				console_printf("%s; before pthread_join\n", __FUNCTION__);
				ret = pthread_join(threadInfo->threadID, NULL);
				console_printf("%s; after pthread_join\n", __FUNCTION__);

				/* Thread has been stopped, so flag can be set back to 0 */
				threadInfo->threadShouldStop = 0;
				threadInfo->threadID = 0;

				/* Close the pipe and reset thread_id REGARDLESS to the return code of pthread_join() */

				if (close(threadInfo->pipeFDs[0]) != 0)
				{
					console_printf("%s; close() to threadInfo->pipeFDs[0] FAILED (errno= %d)\n", __FUNCTION__, errno);
				}
				if (close(threadInfo->pipeFDs[1]) != 0)
				{
					console_printf("%s; close() to threadInfo->pipeFDs[1] FAILED (errno= %d)\n", __FUNCTION__, errno);
				}

				if ( (ret != 0) && (ret != ESRCH) )
				{
					console_printf("%s;pthread_join ERROR (ret= %d) ==> Abort!\n", __FUNCTION__, ret);
					return DWPAL_FAILURE;
				}
			}
			break;

		default:
			console_printf("%s; threadOperation (%d) not supported ==> Abort!\n", __FUNCTION__, threadOperation);
			return DWPAL_FAILURE;
			break;
	}

	return DWPAL_SUCCESS;
}


#if defined EVENT_CALLBACK_THREAD
static int fdDaemonSet(char *socketName, int *fd /* output param */)
{
	struct sockaddr_un un;
	size_t len;

	if ((*fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
	{
		console_printf("%s; create socket fail; socketName= '%s'; errno= %d ('%s')\n", __FUNCTION__, socketName, errno, strerror(errno));
		return DWPAL_FAILURE;
    }

	console_printf("%s; fd_daemon (socketName='%s') = %d\n", __FUNCTION__, socketName, *fd);

	unlink(socketName);   /* in case it already exists */

    /* fill in socket address structure */
	memset(&un, 0, sizeof(un));
	un.sun_family = AF_UNIX;
	strcpy_s(un.sun_path, sizeof(un.sun_path), socketName);
	len = offsetof(struct sockaddr_un, sun_path) + strnlen_s(socketName, SOCKET_NAME_LENGTH);

    /* bind the name to the descriptor */
	if (bind(*fd, (struct sockaddr *)&un, len) < 0)  // check if can use connect() instead...
	{
		console_printf("%s; bind() fail; errno= %d ('%s')\n", __FUNCTION__, errno, strerror(errno));

		if (close(*fd) == (-1))
		{
			console_printf("%s; close() fail; errno= %d ('%s')\n", __FUNCTION__, errno, strerror(errno));
		}

		return DWPAL_FAILURE;
    }

	if (chmod(socketName, 0600) < 0)
	{
		console_printf("%s; FAIL to chmod '%s' to 0666\n", __FUNCTION__, socketName);

		if (close(*fd) == (-1))
		{
			console_printf("%s; close() fail; errno= %d ('%s')\n", __FUNCTION__, errno, strerror(errno));
		}

		return DWPAL_FAILURE;
    }

	if (listen(*fd, 10 /*Q Length*/) < 0)
	{ /* tell kernel we're a server */
		console_printf("%s; listen fail\n", __FUNCTION__);

		if (close(*fd) == (-1))
		{
			console_printf("%s; close() fail; errno= %d ('%s')\n", __FUNCTION__, errno, strerror(errno));
		}

		return DWPAL_FAILURE;
	}

	return DWPAL_SUCCESS;
}


static DWPAL_Ret dwpal_socket_create(int *fd, char *socketPrefixName)
{
	pid_t pid = getpid();
	char  socketName[SOCKET_NAME_LENGTH] = "\0";
	int status;

	status = sprintf_s(socketName, sizeof(socketName), "%s_%d", socketPrefixName, pid);
	if (status <= 0)
		console_printf("%s; sprintf_s failed!\n", __FUNCTION__);

	if (*fd > 0)
	{
		console_printf("%s; *fd (%d) ==> cont...\n", __FUNCTION__, *fd);
		return DWPAL_SUCCESS;
	}

	if (fdDaemonSet(socketName, fd /*output*/) == DWPAL_FAILURE)
	{
		console_printf("%s; ERROR; *fd= %d\n", __FUNCTION__, *fd);
		return DWPAL_FAILURE;
	}

	return DWPAL_SUCCESS;
}
#endif


static DWPAL_Ret nlCmdGetCallback(char *ifname, int event, int subevent, size_t len, unsigned char *data)
{
	console_printf("%s Entry; ifname= '%s', event= %d, subevent= %d (len= %zu)\n", __FUNCTION__, ifname, event, subevent, len);

	(void)ifname;
	(void)event;
	(void)subevent;

	if ( (len == 0) || (data == NULL) )
	{
		console_printf("%s; len=0 and/or data is NULL ==> Abort!\n", __FUNCTION__);
		return DWPAL_FAILURE;
	}

	{
		int i;
		size_t lenToPrint = (len <= 10)? len : 10;

		console_printf("%s; Output data from the 'get' function:\n", __FUNCTION__);
		for (i=0; i < (int)lenToPrint; i++)
		{
			console_printf(" 0x%x", data[i]);
		}
		console_printf("\n");
	}

	if (nl_response_save_data == true)
	{
		if (nl_response_data == NULL || nl_response_len == NULL)
		{
			console_printf("%s; nl_response_data or nl_response_len is NULL ==> Abort!\n", __FUNCTION__);
			return DWPAL_FAILURE;
		}

		if (len > DRIVER_NL_TO_DWPAL_MSG_LENGTH)
		{
			console_printf("%s; len (%zu) > (%zu) ==> Abort!\n", __FUNCTION__, len, (size_t)DRIVER_NL_TO_DWPAL_MSG_LENGTH);
			return DWPAL_FAILURE;
		}

		memcpy_s(nl_response_data, DRIVER_NL_TO_DWPAL_MSG_LENGTH, data, len);
		*nl_response_len = len;
		nl_response_received = true;
	}

	return DWPAL_SUCCESS;
}


static DWPAL_Ret nl_cmd_socket_clean_if_needed(int idx)
{
	struct  timeval tv;
	fd_set  rfds;
	int     ret;
	int select_failure_count = 0;

	if (dwpal_driver_nl_fd_get(context[idx], &dwpalService[idx]->fd, &dwpalService[idx]->fdCmdGet) == DWPAL_FAILURE)
	{
		console_printf("%s; dwpal_driver_nl_fd_get returned error ==> Abort.\n", __FUNCTION__);
		return DWPAL_FAILURE;
	}

	if (dwpalService[idx]->fdCmdGet <= 0)
	{
		console_printf("%s; fdCmdGet value is invalid (%d) ==> Abort!\n", __FUNCTION__, dwpalService[idx]->fdCmdGet);
		return DWPAL_FAILURE;
	}

	do {
		FD_ZERO(&rfds);
		FD_SET(dwpalService[idx]->fdCmdGet, &rfds);

		tv.tv_sec = 0;
		tv.tv_usec = 0;

		ret = select(dwpalService[idx]->fdCmdGet + 1, &rfds, NULL, NULL, &tv);
		if (ret < 0)
		{
			console_printf("%s; select() return value= %d ==> cont...; errno= %d ('%s')\n", __FUNCTION__, ret, errno, strerror(errno));
			select_failure_count++;

			if (select_failure_count >= 3)
			{
				console_printf("%s; select() returned error %d times in a row ==> Abort.\n", __FUNCTION__, select_failure_count);
				return DWPAL_FAILURE;
			}

			usleep(1000);
			continue;
		}

		if (FD_ISSET(dwpalService[idx]->fdCmdGet, &rfds))
		{
			console_printf("%s; nl command socket not empty ==> receive one message\n", __FUNCTION__);

			nl_response_save_data = false;
			if (dwpal_driver_nl_msg_get(context[idx], DWPAL_NL_SOLICITED_EVENT, dwpalService[idx]->nlCmdGetCallback, NULL) == DWPAL_FAILURE)
			{
				console_printf("%s; dwpal_driver_nl_msg_get ERROR\n", __FUNCTION__);
				return DWPAL_FAILURE;
			}
		}

	} while(ret);

	return DWPAL_SUCCESS;
}


static DWPAL_Ret nl_cmd_socket_receive_response(int idx, size_t *outLen, unsigned char *outData)
{
	struct  timeval tv;
	fd_set  rfds;
	int     ret;

	if (dwpal_driver_nl_fd_get(context[idx], &dwpalService[idx]->fd, &dwpalService[idx]->fdCmdGet) == DWPAL_FAILURE)
	{
		console_printf("%s; dwpal_driver_nl_fd_get returned error ==> Abort.\n", __FUNCTION__);
		return DWPAL_FAILURE;
	}

	if (dwpalService[idx]->fdCmdGet <= 0)
	{
		console_printf("%s; fdCmdGet value is invalid (%d) ==> Abort!\n", __FUNCTION__, dwpalService[idx]->fdCmdGet);
		return DWPAL_FAILURE;
	}

recv:
	FD_ZERO(&rfds);
	FD_SET(dwpalService[idx]->fdCmdGet, &rfds);

	tv.tv_sec = 2;
	tv.tv_usec = 0;

	ret = select(dwpalService[idx]->fdCmdGet + 1, &rfds, NULL, NULL, &tv);
	if (ret < 0)
	{
		console_printf("%s; select() return value= %d ==> cont...; errno= %d ('%s') ==> expected behavior when 'Interrupted system call'\n", __FUNCTION__, ret, errno, strerror(errno));
		usleep(1000);
		goto recv;
	}

	if (FD_ISSET(dwpalService[idx]->fdCmdGet, &rfds))
	{
		console_printf("%s; nl command socket not empty ==> receive one message\n", __FUNCTION__);

		nl_response_save_data = true;
		nl_response_data = outData;
		nl_response_len = outLen;
		nl_response_received = false;
		if (dwpal_driver_nl_msg_get(context[idx], DWPAL_NL_SOLICITED_EVENT, dwpalService[idx]->nlCmdGetCallback, NULL) == DWPAL_FAILURE)
		{
			console_printf("%s; dwpal_driver_nl_msg_get ERROR\n", __FUNCTION__);
			return DWPAL_FAILURE;
		}

		if (nl_response_received == false)
		{
			console_printf("%s; nlCmdGetCallback did not fill outData with a response\n", __FUNCTION__);
			return DWPAL_FAILURE;
		}
	}

	return DWPAL_SUCCESS;
}


static DWPAL_Ret nl_cmd_handle(char *ifname,
                               unsigned int nl80211Command,
							   CmdIdType cmdIdType,
							   unsigned int subCommand,
							   unsigned char *vendorData,
							   size_t vendorDataSize,
							   size_t *outLen,
							   unsigned char *outData)
{
	int    i, idx;

	console_printf("%s; ifname= '%s', nl80211Command= 0x%x, cmdIdType= %d, subCommand= %u, vendorDataSize= %zu, outLen= %p, outData= %p\n",
	            __FUNCTION__, ifname, nl80211Command, cmdIdType, subCommand, vendorDataSize, (void *)outLen, (void *)outData);

	for (i=0; i < (int)vendorDataSize; i++)
	{
		console_printf("%s; vendorData[%d]= 0x%02x\n", __FUNCTION__, i, ((const char*)vendorData)[i]);
	}

	if (dwpal_ext_interfaceIndexGet(DWPAL_CONN_TYPE_DRIVER, "ALL", &idx) == DWPAL_INTERFACE_IS_DOWN)
	{
		console_printf("%s; dwpal_ext_interfaceIndexGet returned ERROR ==> Abort!\n", __FUNCTION__);
		return DWPAL_INTERFACE_IS_DOWN;
	}

	console_printf("%s; dwpal_ext_interfaceIndexGet returned idx= %d\n", __FUNCTION__, idx);

	if ( (outLen != NULL) && (outData != NULL) )
	{
		MUTEX_LOCK(&nl_cmd_mutex);

		if (nl_cmd_socket_clean_if_needed(idx) == DWPAL_FAILURE)
		{
			console_printf("%s; nl_cmd_socket_clean_if_needed returned ERROR ==> Abort!\n", __FUNCTION__);
			goto cmd_err;
		}

		/* Handle a command which invokes an event with the output data */
		if (dwpal_driver_nl_cmd_send(context[idx],
									 DWPAL_NL_SOLICITED_EVENT,
									 ifname,
									 nl80211Command,
									 cmdIdType,
									 subCommand,
									 vendorData,
									 vendorDataSize) == DWPAL_FAILURE)
		{
			console_printf("%s; dwpal_driver_nl_cmd_send returned ERROR ==> Abort!\n", __FUNCTION__);
			goto cmd_err;
		}

		if (nl_cmd_socket_receive_response(idx, outLen, outData) == DWPAL_FAILURE)
		{
			console_printf("%s; nl_cmd_socket_receive_response ERROR (subCommand= 0x%x) ==> Abort!\n", __FUNCTION__, subCommand);
			goto cmd_err;
		}

		console_printf("%s; 'get command' (subCommand= 0x%x, outLen= %zu) was received\n", __FUNCTION__, subCommand, *outLen);
		MUTEX_UNLOCK(&nl_cmd_mutex);

		return DWPAL_SUCCESS;

cmd_err:
		*outLen = 0;
		MUTEX_UNLOCK(&nl_cmd_mutex);
		return DWPAL_FAILURE;

	}
	else
	{
		return dwpal_driver_nl_cmd_send(context[idx],
										DWPAL_NL_UNSOLICITED_EVENT,
										ifname,
										nl80211Command,
										cmdIdType,
										subCommand,
										vendorData,
										vendorDataSize);
	}
}


/* APIs */

/*! \file dwpal_ext.c
    \brief DWPAL library

	The purpose of this document is to describe the new dynamic configuration infrastructure,
	* AKA D-WPAL (Dynamic Wireless Platform Abstraction Layer).
	In general, the main purpose of the new infrastructure is to have all dynamic configuration APIs (commands, reports, statistics, etc.),
	in one shared library. Dynamic configuration of hostapd (via wpa_ctrl) and the Driver (via nl).
	The library can be used by and linked with any other software clients, and will run under the caller's context,
	* i.e. the caller's own process/thread.
	The role of D-WPAL is to provide:
		Interface to / from hostapd / Driver.
		Parsing services.
*/

DWPAL_Ret dwpal_ext_interfaceIndexGet(DwpalConnectionType connectionType, const char *VAPName, int *idx)
{
	DWPAL_Ret ret;

	// pthread_mutex_lock(&attach_mutex);
	ret = interfaceIndexGet(connectionType, VAPName, idx);
	// pthread_mutex_unlock(&attach_mutex);

	return ret;
}

DWPAL_Ret dwpal_ext_driver_nl_scan_dump(char *ifname, DWPAL_nlNonVendorEventCallback nlEventCallback)
{
	int idx;

	console_printf("%s; ifname= '%s'\n", __FUNCTION__, ifname);

	if (dwpal_ext_interfaceIndexGet(DWPAL_CONN_TYPE_DRIVER, "ALL", &idx) == DWPAL_INTERFACE_IS_DOWN)
	{
		console_printf("%s; dwpal_ext_interfaceIndexGet returned ERROR ==> Abort!\n", __FUNCTION__);
		return DWPAL_INTERFACE_IS_DOWN;
	}

	console_printf("%s; dwpal_ext_interfaceIndexGet returned idx= %d\n", __FUNCTION__, idx);

	return dwpal_driver_nl_scan_dump(context[idx], ifname, nlEventCallback);
}


DWPAL_Ret dwpal_ext_driver_nl_scan_trigger(char *ifname, ScanParams *scanParams)
{
	int idx;

	console_printf("%s; ifname= '%s'\n", __FUNCTION__, ifname);

	if (dwpal_ext_interfaceIndexGet(DWPAL_CONN_TYPE_DRIVER, "ALL", &idx) == DWPAL_INTERFACE_IS_DOWN)
	{
		console_printf("%s; dwpal_ext_interfaceIndexGet returned ERROR ==> Abort!\n", __FUNCTION__);
		return DWPAL_INTERFACE_IS_DOWN;
	}

	console_printf("%s; dwpal_ext_interfaceIndexGet returned idx= %d\n", __FUNCTION__, idx);

	return dwpal_driver_nl_scan_trigger(context[idx], ifname, scanParams);
}

/**************************************************************************/
/*! \fn DWPAL_Ret dwpal_ext_driver_nl_get(char *ifname, unsigned int nl80211Command, CmdIdType cmdIdType, unsigned int subCommand, unsigned char *vendorData, size_t vendorDataSize, size_t *outLen, unsigned char *outData)
 **************************************************************************
 *  \brief driver-NL get command
 *  \param[in] char *ifname - the radio interface
 *  \param[in] unsigned int nl80211Command - NL 80211 command. Note: currently we support ONLY NL80211_CMD_VENDOR (0x67)
 *  \param[in] CmdIdType cmdIdType - The command ID type: NETDEV, PHY or WDEV
 *  \param[in] unsigned int subCommand - the vendor's sub-command
 *  \param[in] unsigned char *vendorData - the vendor's data (can be NULL)
 *  \param[in] size_t vendorDataSize - the vendor's data length (if the vendor data is NULL, it should be '0')
 *  \param[out] size_t *outLen - The length of the returned data
 *  \param[out] unsigned char *outData - Pointer the returned data itself
 *  \return DWPAL_Ret (DWPAL_SUCCESS for success, other for failure)
 ***************************************************************************/
DWPAL_Ret dwpal_ext_driver_nl_get(char *ifname, unsigned int nl80211Command, CmdIdType cmdIdType, unsigned int subCommand, unsigned char *vendorData, size_t vendorDataSize, size_t *outLen, unsigned char *outData)
{
	console_printf("%s; ifname= '%s', nl80211Command= 0x%x, cmdIdType= %d, subCommand= 0x%x\n", __FUNCTION__, ifname, nl80211Command, cmdIdType, subCommand);

	return nl_cmd_handle(ifname, nl80211Command, cmdIdType, subCommand, vendorData, vendorDataSize, outLen, outData);
}


/**************************************************************************/
/*! \fn DWPAL_Ret dwpal_ext_driver_nl_cmd_send(char *ifname, unsigned int nl80211Command, CmdIdType cmdIdType, unsigned int subCommand, unsigned char *vendorData, size_t vendorDataSize)
 **************************************************************************
 *  \brief driver-NL send command
 *  \param[in] char *ifname - the radio interface
 *  \param[in] unsigned int nl80211Command - NL 80211 command. Note: currently we support ONLY NL80211_CMD_VENDOR (0x67)
 *  \param[in] CmdIdType cmdIdType - The command ID type: NETDEV, PHY or WDEV
 *  \param[in] unsigned int subCommand - the vendor's sub-command
 *  \param[in] unsigned char *vendorData - the vendor's data (can be NULL)
 *  \param[in] size_t vendorDataSize - the vendor's data length (if the vendor data is NULL, it should be '0')
 *  \return DWPAL_Ret (DWPAL_SUCCESS for success, other for failure)
 ***************************************************************************/
DWPAL_Ret dwpal_ext_driver_nl_cmd_send(char *ifname, unsigned int nl80211Command, CmdIdType cmdIdType, unsigned int subCommand, unsigned char *vendorData, size_t vendorDataSize)
{
	if ( (ifname == NULL) || ((vendorDataSize > 0) && (vendorData == NULL)) )
	{
		console_printf("%s; ifname or vendorData is NULL ==> Abort!\n", __FUNCTION__);
		return DWPAL_FAILURE;
	}

	console_printf("%s; ifname= '%s', nl80211Command= 0x%x, cmdIdType= %d, subCommand= 0x%x, vendorDataSize= %zu\n", __FUNCTION__, ifname, nl80211Command, cmdIdType, subCommand, vendorDataSize);

	return nl_cmd_handle(ifname, nl80211Command, cmdIdType, subCommand, vendorData, vendorDataSize, NULL, NULL);
}

/* New API */

static int nlRecoveryNeeded(uint serviceIdx, DWPAL_Ret ret, int cmd_res, int *tries)
{
	if (ret != DWPAL_SUCCESS)
		return 0; // error, but normal processing

	if (cmd_res != -ENOENT)
		return 0; // normal processing

	if (--(*tries) == 0)
		return 0; // no more tries

	/* re-attach */
	if ((context[serviceIdx] != NULL) && (dwpal_driver_nl_detach(&context[serviceIdx]) != DWPAL_SUCCESS)) {
		console_printf("%s; dwpal_driver_nl_detach returned ERROR ==> cont...\n", __FUNCTION__);
	}

	if (DWPAL_SUCCESS != dwpal_driver_nl_attach(&context[serviceIdx])) {
		console_printf("%s; dwpal_driver_nl_attach: failure\n", __FUNCTION__);
		return 0; // re-attach failed
	}

	console_printf("%s; NL connection recovered successfully!\n", __FUNCTION__);
	return 1; // try again
}

DWPAL_Ret dwpal_ext_nl80211_cmd_send(struct nl_msg *msg, int *cmd_res /*OUT*/, DWPAL_nl80211Callback nlCallback, void *cb_arg, bool lock_cmd)
{
	int idx;
	DWPAL_Ret ret;
	int tries = 2;

	if (msg == NULL || cmd_res == NULL)
	{
		console_printf("%s; msg or cmd_res is NULL ==> Abort!\n", __FUNCTION__);
		return DWPAL_FAILURE;
	}

	if (dwpal_ext_interfaceIndexGet(DWPAL_CONN_TYPE_DRIVER, "ALL", &idx) == DWPAL_INTERFACE_IS_DOWN)
	{
		console_printf("%s; dwpal_ext_interfaceIndexGet returned ERROR ==> Abort!\n", __FUNCTION__);
		nlmsg_free(msg);
		return DWPAL_INTERFACE_IS_DOWN;
	}

	if (lock_cmd) MUTEX_LOCK(&nl_cmd_mutex);

	do {
		ret = dwpal_nl80211_cmd_send(context[idx], msg, cmd_res, nlCallback, cb_arg);
	} while (nlRecoveryNeeded(idx, ret, *cmd_res, &tries));

	if (lock_cmd) MUTEX_UNLOCK(&nl_cmd_mutex);
	return ret;
}


DWPAL_Ret dwpal_ext_driver_nl_scan_dump_sync(char *ifname, int *cmd_res /*OUT*/, DWPAL_nl80211Callback nlCallback, void *cb_arg, bool lock_cmd)
{
	int idx;
	DWPAL_Ret ret;
	int tries = 2;

	if (dwpal_ext_interfaceIndexGet(DWPAL_CONN_TYPE_DRIVER, "ALL", &idx) == DWPAL_INTERFACE_IS_DOWN)
	{
		console_printf("%s; dwpal_ext_interfaceIndexGet returned ERROR ==> Abort!\n", __FUNCTION__);
		return DWPAL_INTERFACE_IS_DOWN;
	}

	if (lock_cmd) MUTEX_LOCK(&nl_cmd_mutex);

	do {
		ret = dwpal_driver_nl_scan_dump_sync(context[idx], ifname, cmd_res, nlCallback, cb_arg);
	} while (nlRecoveryNeeded(idx, ret, *cmd_res, &tries));

	if (lock_cmd) MUTEX_UNLOCK(&nl_cmd_mutex);
	return ret;
}


DWPAL_Ret dwpal_ext_driver_nl_scan_trigger_sync(char *ifname, int *cmd_res /*OUT*/, ScanParams *scanParams, bool lock_cmd)
{
	int idx;
	DWPAL_Ret ret;
	int tries = 2;

	if (dwpal_ext_interfaceIndexGet(DWPAL_CONN_TYPE_DRIVER, "ALL", &idx) == DWPAL_INTERFACE_IS_DOWN)
	{
		console_printf("%s; dwpal_ext_interfaceIndexGet returned ERROR ==> Abort!\n", __FUNCTION__);
		return DWPAL_INTERFACE_IS_DOWN;
	}

	if (lock_cmd) MUTEX_LOCK(&nl_cmd_mutex);

	do {
		ret = dwpal_driver_nl_scan_trigger_sync(context[idx], ifname, cmd_res, scanParams);
	} while (nlRecoveryNeeded(idx, ret, *cmd_res, &tries));

	if (lock_cmd) MUTEX_UNLOCK(&nl_cmd_mutex);
	return ret;
}


DWPAL_Ret dwpal_ext_nl80211_id_get(int *nl80211_id /*OUT*/)
{
	int idx;

	if (nl80211_id == NULL)
	{
		console_printf("%s; nl80211_id is NULL ==> Abort!\n", __FUNCTION__);
		return DWPAL_FAILURE;
	}

	if (dwpal_ext_interfaceIndexGet(DWPAL_CONN_TYPE_DRIVER, "ALL", &idx) == DWPAL_INTERFACE_IS_DOWN)
	{
		console_printf("%s; dwpal_ext_interfaceIndexGet returned ERROR ==> Abort!\n", __FUNCTION__);
		return DWPAL_INTERFACE_IS_DOWN;
	}

	return dwpal_nl80211_id_get(context[idx], nl80211_id);
}


/**************************************************************************/
/*! \fn DWPAL_Ret dwpal_ext_driver_nl_detach(void)
 **************************************************************************
 *  \brief Resets/clear the Driver-NL interface
 *  \return DWPAL_Ret (DWPAL_SUCCESS for success, other for failure)
 ***************************************************************************/
DWPAL_Ret dwpal_ext_driver_nl_detach(void)
{
	int idx;
	DWPAL_Ret ret;
#if defined EVENT_CALLBACK_THREAD
	int status;
#endif

	console_printf("%s Entry\n", __FUNCTION__);

	/* Cannot to detach from listener thread */
	if (pthread_self() == g_listenerThreadInfo.threadID) {
		console_printf("%s is called from listener thread ==> Abort!\n", __FUNCTION__);
		return DWPAL_FAILURE;
	}

	MUTEX_LOCK(&attach_mutex);

	if ((ret = interfaceIndexGet(DWPAL_CONN_TYPE_DRIVER, "ALL", &idx)) == DWPAL_INTERFACE_IS_DOWN)
	{
		console_printf("%s; interfaceIndexGet returned ERROR ==> Abort!\n", __FUNCTION__);
		MUTEX_UNLOCK(&attach_mutex);
		return ret;
	}

	console_printf("%s; interfaceIndexGet returned idx= %d\n", __FUNCTION__, idx);

	/* Cancel the listener thread, if it does exist */
#if defined EVENT_CALLBACK_THREAD
	threadSet(&eventHandlerThreadId, THREAD_CANCEL, NULL);
#endif
	threadSet(&g_listenerThreadInfo, THREAD_CANCEL, NULL);

	/* dealocate the interface (after canceling the listener thread) */
	if (dwpalService[idx] != NULL)
	{
		free(dwpalService[idx]);
		dwpalService[idx] = NULL;
	}

	if (dwpal_driver_nl_detach(&context[idx]) == DWPAL_FAILURE)
	{
		console_printf("%s; dwpal_driver_nl_detach returned ERROR ==> Abort!\n", __FUNCTION__);
		ret = DWPAL_FAILURE;
		goto end;
	}

	ret = DWPAL_SUCCESS;
end:
	if (isAnyInterfaceActive())
	{ /* There are still active interfaces */
		/* Create the listener thread, if it does NOT exist yet */
#if defined EVENT_CALLBACK_THREAD
		threadSet(&eventHandlerThreadId, THREAD_CREATE, eventHandlerThreadStart);
#endif
		threadSet(&g_listenerThreadInfo, THREAD_CREATE, listenerThreadStart);
	}
#if defined EVENT_CALLBACK_THREAD
	else if (dwpal_event_handler != (-1))
	{
		pid_t pid = getpid();
		char  socketName[SOCKET_NAME_LENGTH] = "\0";

		status = sprintf_s(socketName, sizeof(socketName), "%s_%d", EVENT_HANDLER_SOCKET, pid);
		if (status <= 0)
			console_printf("%s; sprintf_s failed!\n", __FUNCTION__);

		if (close(dwpal_event_handler) == (-1))
		{
			console_printf("%s; close() fail; dwpal_event_handler= %d; errno= %d ('%s')\n", __FUNCTION__, dwpal_event_handler, errno, strerror(errno));
		}

		unlink(socketName);
		dwpal_event_handler = (-1);
	}
#endif

	MUTEX_UNLOCK(&attach_mutex);
	return ret;
}


/**************************************************************************/
/*! \fn DWPAL_Ret dwpal_ext_driver_nl_attach(DwpalExtNlEventCallback nlEventCallback, DwpalExtNlNonVendorEventCallback nlNonVendorEventCallback)
 **************************************************************************
 *  \brief Driver-NL interface attach and event callback register
 *  \param[in] DwpalExtNlEventCallback nlEventCallback - The callback function to be called while event will be received via this interface
 *  \param[in] DwpalExtNlNonVendorEventCallback nlNonVendorEventCallback - The callback function to be called when non-vendor event will be received
 *  \return DWPAL_Ret (DWPAL_SUCCESS for success, other for failure)
 ***************************************************************************/
DWPAL_Ret dwpal_ext_driver_nl_attach(DwpalExtNlEventCallback nlEventCallback, DwpalExtNlNonVendorEventCallback nlNonVendorEventCallback)
{
	int       idx;
	DWPAL_Ret ret;

	console_printf("%s Entry\n", __FUNCTION__);

	/* Cannot attach from listener thread */
	if (pthread_self() == g_listenerThreadInfo.threadID) {
		console_printf("%s is called from listener thread ==> Abort!\n", __FUNCTION__);
		return DWPAL_FAILURE;
	}

	MUTEX_LOCK(&attach_mutex);

	ret = interfaceIndexGet(DWPAL_CONN_TYPE_DRIVER, "ALL", &idx);
	if (ret == DWPAL_SUCCESS && context[idx] != NULL) {
		console_printf("%s; Driver Interface (idx= %d) is already up ==> cont...\n", __FUNCTION__, idx);
		MUTEX_UNLOCK(&attach_mutex);
		return DWPAL_SUCCESS;
	}

	/* Cancel the listener thread, if it does exist */
#if defined EVENT_CALLBACK_THREAD
	threadSet(&eventHandlerThreadId, THREAD_CANCEL, NULL);
#endif
	threadSet(&g_listenerThreadInfo, THREAD_CANCEL, NULL);

	ret = interfaceIndexCreate(DWPAL_CONN_TYPE_DRIVER, "ALL", &idx);
	if (ret == DWPAL_FAILURE)
	{
		console_printf("%s; interfaceIndexCreate returned ERROR ==> Abort!\n", __FUNCTION__);
		goto end;
	}
	else if (ret == DWPAL_INTERFACE_ALREADY_UP)
	{
		console_printf("%s; Interface (idx= %d) is already up ==> cont...\n", __FUNCTION__, idx);
		ret = DWPAL_SUCCESS;
		goto end;
	}

	console_printf("%s; interfaceIndexCreate successfully; returned idx= %d\n", __FUNCTION__, idx);

	if (dwpal_driver_nl_attach(&context[idx] /*OUT*/) == DWPAL_FAILURE)
	{
		console_printf("%s; dwpal_driver_nl_attach returned ERROR ==> Abort!\n", __FUNCTION__);
		ret = DWPAL_FAILURE;
		goto end;
	}

	/* nlEventCallback can be NULL */
	dwpalService[idx]->nlEventCallback = nlEventCallback;

	/* Register here the internal static callback function of the 'get command' event */
	dwpalService[idx]->nlCmdGetCallback = nlCmdGetCallback;

	/* Register here the internal static callback function of the 'non-Vendor' event */
	dwpalService[idx]->nlNonVendorEventCallback = nlNonVendorEventCallback;

	ret = DWPAL_SUCCESS;

end:
	if (ret == DWPAL_FAILURE && dwpalService[idx] != NULL)
	{
		free(dwpalService[idx]);
		dwpalService[idx] = NULL;
	}

	/* Create the listener thread, if it does NOT exist yet */
	if (isAnyInterfaceActive())
	{
#if defined EVENT_CALLBACK_THREAD
		threadSet(&eventHandlerThreadId, THREAD_CREATE, eventHandlerThreadStart);
#endif
		threadSet(&g_listenerThreadInfo, THREAD_CREATE, listenerThreadStart);
	}

	MUTEX_UNLOCK(&attach_mutex);
	return ret;
}

#ifndef DISABLE_DWPAL_HOSTAP_SUPPORT
/**************************************************************************/
/*! \fn DWPAL_Ret dwpal_ext_hostap_cmd_send(const char *VAPName, const char *cmdHeader, FieldsToCmdParse *fieldsToCmdParse, char *reply , size_t *replyLen)
 **************************************************************************
 *  \brief Build and send hostap command
 *  \param[in] char *VAPName - The interface's radio/VAP name to send the command to
 *  \param[in] char *cmdHeader - The beginning of the hostap command string
 *  \param[in] FieldsToCmdParse *fieldsToCmdParse - The command parsing information, in which accordingly, the command string (after the header) will be created
 *  \param[out] char *reply - The output string returning from the hostap command
 *  \param[in,out] size_t *replyLen - Provide the max output string length, and get back the actual string length
 *  \return DWPAL_Ret (DWPAL_SUCCESS for success, other for failure)
 ***************************************************************************/
DWPAL_Ret dwpal_ext_hostap_cmd_send(const char *VAPName, const char *cmdHeader, FieldsToCmdParse *fieldsToCmdParse, char *reply /*OUT*/, size_t *replyLen /*IN/OUT*/)
{
	int idx;
	DWPAL_Ret dwpal_ret;

	if ( (VAPName == NULL) || (cmdHeader == NULL) || (reply == NULL) || (replyLen == NULL) )
	{
		console_printf("%s; input params error ==> Abort!\n", __FUNCTION__);
		return DWPAL_FAILURE;
	}

	if (((pthread_self() == g_listenerThreadInfo.threadID) && g_listenerThreadInfo.threadShouldStop) ||
	    ((pthread_self() == g_monitorThreadInfo.threadID) && g_monitorThreadInfo.threadShouldStop))
	{
		/* function is called from listener thread in cancelling state. Don't send any new commands
		   to hostapd*/
		console_printf("%s; listener thread is in cancelling state ==> Abort!\n", __FUNCTION__);
		return DWPAL_FAILURE;
	}

	console_printf("%s; VAPName= '%s', cmdHeader= '%s'\n", __FUNCTION__, VAPName, cmdHeader);

	if (dwpal_ext_interfaceIndexGet(DWPAL_CONN_TYPE_HOSTAP, VAPName, &idx) == DWPAL_INTERFACE_IS_DOWN)
	{
		console_printf("%s; dwpal_ext_interfaceIndexGet (VAPName= '%s') returned ERROR ==> Abort!\n", __FUNCTION__, VAPName);
		*replyLen = 0;
		return DWPAL_INTERFACE_IS_DOWN;
	}

	console_printf("%s; interfaceIndexGet returned idx= %d\n", __FUNCTION__, idx);

	MUTEX_LOCK(&context_mutex);
	if (dwpalService[idx]->isConnectionEstablishNeeded == true)
	{
		MUTEX_UNLOCK(&context_mutex);
		console_printf("%s; interface is being reconnected, but still NOT ready ==> Abort!\n", __FUNCTION__);
		*replyLen = 0;
		return DWPAL_INTERFACE_IS_DOWN;
	}

	if (context[idx] == NULL)
	{
		MUTEX_UNLOCK(&context_mutex);
		console_printf("%s; context[%d] is NULL ==> Abort!\n", __FUNCTION__, idx);
		*replyLen = 0;
		return DWPAL_FAILURE;
	}

	dwpal_ret = dwpal_hostap_cmd_send(context[idx], cmdHeader, fieldsToCmdParse, reply, replyLen);
	if ((DWPAL_SOCKET_FAILURE == dwpal_ret) || (DWPAL_FAILURE == dwpal_ret))
	{
		console_printf("%s; '%s' command send error\n", __FUNCTION__, cmdHeader);
		*replyLen = 0;

		console_printf("%s; VAPName= '%s' interface needs to be recovered\n", __FUNCTION__, dwpalService[idx]->VAPName);
		dwpalService[idx]->isConnectionEstablishNeeded = true;

		if (DWPAL_SOCKET_FAILURE == dwpal_ret) {
			if (dwpal_hostap_socket_close(&context[idx] /*OUT*/) != DWPAL_SUCCESS)
			{
				console_printf("%s; dwpal_hostap_socket_close (VAPName= '%s') returned ERROR ==> cont...\n", __FUNCTION__, dwpalService[idx]->VAPName);
			}
		}
		else {
			if (dwpal_hostap_interface_detach(&context[idx] /*OUT*/) != DWPAL_SUCCESS)
			{
				console_printf("%s; dwpal_hostap_interface_detach (VAPName= '%s') returned ERROR ==> cont...\n", __FUNCTION__, dwpalService[idx]->VAPName);
			}
		}

		MUTEX_UNLOCK(&context_mutex);
		interfaceDisconnectedSend(idx);
		return DWPAL_FAILURE;
	}

	MUTEX_UNLOCK(&context_mutex);

	if (strncmp(cmdHeader, "PING", sizeof("PING")))
	{
		cli_printf("%s; replyLen= %zu\nreply=\n%s\n", __FUNCTION__, *replyLen, reply);
	}
	else
	{
		console_printf("%s; replyLen= %zu\nreply=\n%s\n", __FUNCTION__, *replyLen, reply);
	}

	return DWPAL_SUCCESS;
}
#endif /* DISABLE_DWPAL_HOSTAP_SUPPORT */

/**************************************************************************/
/*! \fn DWPAL_Ret dwpal_ext_hostap_interface_detach(char *VAPName)
 **************************************************************************
 *  \brief Check if caller code is executed in event's handler thread contex
 *  \return 'true' if function was called from event's handler thread contex
 ***************************************************************************/
bool dwpal_ext_is_events_thread_context(void)
{
#ifdef EVENT_CALLBACK_THREAD
	return (pthread_self() == eventHandlerThreadId);
#else
	return (pthread_self() == g_listenerThreadInfo.threadID);
#endif
}

#ifndef DISABLE_DWPAL_HOSTAP_SUPPORT
/**************************************************************************/
/*! \fn DWPAL_Ret dwpal_ext_hostap_interface_detach(char *VAPName)
 **************************************************************************
 *  \brief Will reset and detach the interface towards/from the hostapd/supplicant of the requested radio/VAP interface
 *  \param[in] char *VAPName - The interface's radio/VAP name to detach from
 *  \return DWPAL_Ret (DWPAL_SUCCESS for success, other for failure)
 ***************************************************************************/
DWPAL_Ret dwpal_ext_hostap_interface_detach(const char *VAPName)
{
	int  idx;
	DWPAL_Ret ret;

	if (VAPName == NULL)
	{
		console_printf("%s; VAPName is NULL ==> Abort!\n", __FUNCTION__);
		return DWPAL_FAILURE;
	}

	console_printf("%s Entry; VAPName= '%s'\n", __FUNCTION__, VAPName);

	/* Cannot detach from listener thread */
	if (pthread_self() == g_listenerThreadInfo.threadID) {
		console_printf("%s is called from listener thread ==> Abort!\n", __FUNCTION__);
		return DWPAL_FAILURE;
	}

	MUTEX_LOCK(&attach_mutex);

	if ((ret = interfaceIndexGet(DWPAL_CONN_TYPE_HOSTAP, VAPName, &idx)) == DWPAL_INTERFACE_IS_DOWN)
	{
		console_printf("%s; interfaceIndexGet (VAPName= '%s') returned ERROR ==> Abort!\n", __FUNCTION__, VAPName);
		MUTEX_UNLOCK(&attach_mutex);
		return ret;
	}

	console_printf("%s; interfaceIndexGet returned idx= %d\n", __FUNCTION__, idx);

#if defined EVENT_CALLBACK_THREAD
	threadSet(&eventHandlerThreadId, THREAD_CANCEL, NULL);
#endif
	/* Cancel the listener thread, if it does exist */
	threadSet(&g_listenerThreadInfo, THREAD_CANCEL, NULL);
	threadSet(&g_monitorThreadInfo, THREAD_CANCEL, NULL);

	/* dealocate the interface (after canceling the listener thread) */
	if (dwpalService[idx] != NULL)
	{
		free((void *)dwpalService[idx]);
		dwpalService[idx] = NULL;
	}

	MUTEX_LOCK(&context_mutex);
	if (context[idx] != NULL && dwpal_hostap_interface_detach(&context[idx]) == DWPAL_FAILURE)
	{
		MUTEX_UNLOCK(&context_mutex);
		console_printf("%s; dwpal_hostap_interface_detach (VAPName= '%s') returned ERROR ==> Abort!\n", __FUNCTION__, VAPName);
		ret = DWPAL_FAILURE;
		goto end;
	}
	MUTEX_UNLOCK(&context_mutex);
	ret = DWPAL_SUCCESS;

end:
	if (isAnyInterfaceActive())
	{ /* There are still active interfaces */
#if defined EVENT_CALLBACK_THREAD
		threadSet(&eventHandlerThreadId, THREAD_CREATE, eventHandlerThreadStart);
#endif
		/* Create the listener thread, if it does exist */
		threadSet(&g_listenerThreadInfo, THREAD_CREATE, listenerThreadStart);
		threadSet(&g_monitorThreadInfo, THREAD_CREATE, monitorThreadStart);
	}
#if defined EVENT_CALLBACK_THREAD
	else if (dwpal_event_handler != (-1))
	{
		pid_t pid = getpid();
		char  socketName[SOCKET_NAME_LENGTH] = "\0";
		int status;

		status = sprintf_s(socketName, sizeof(socketName), "%s_%d", EVENT_HANDLER_SOCKET, pid);
		if (status <= 0)
			console_printf("%s; sprintf_s failed!\n", __FUNCTION__);

		if (close(dwpal_event_handler) == (-1))
		{
			console_printf("%s; close() fail; dwpal_event_handler= %d; errno= %d ('%s')\n", __FUNCTION__, dwpal_event_handler, errno, strerror(errno));
		}

		unlink(socketName);
		dwpal_event_handler = (-1);
	}
#endif

	MUTEX_UNLOCK(&attach_mutex);
	return ret;
}


/**************************************************************************/
/*! \fn DWPAL_Ret dwpal_ext_hostap_interface_attach(char *VAPName, DwpalExtHostapEventCallback hostapEventCallback)
 **************************************************************************
 *  \brief Hostapd/supplicant interface attach and event callback register
 *  \param[in] char *VAPName - The interface's radio/vap name to set attachment to
 *  \param[in] DwpalExtHostapEventCallback hostapEventCallback - The callback function to be called when an event will be received via this interface
 *  \return DWPAL_Ret (DWPAL_SUCCESS for success, other for failure)
 ***************************************************************************/
DWPAL_Ret dwpal_ext_hostap_interface_attach(const char *VAPName, DwpalExtHostapEventCallback hostapEventCallback)
{
	int       idx;
	DWPAL_Ret ret;

	if (VAPName == NULL)
	{
		console_printf("%s; VAPName is NULL ==> Abort!\n", __FUNCTION__);
		return DWPAL_FAILURE;
	}

	if (hostapEventCallback == NULL)
	{
		console_printf("%s; hostapEventCallback is NULL ==> Abort!\n", __FUNCTION__);
		return DWPAL_FAILURE;
	}

	console_printf("%s Entry; VAPName= '%s'\n", __FUNCTION__, VAPName);

	/* Cannot to attach from listener thread */
	if (pthread_self() == g_listenerThreadInfo.threadID) {
		console_printf("%s is called from listener thread ==> Abort!\n", __FUNCTION__);
		return DWPAL_FAILURE;
	}

	MUTEX_LOCK(&attach_mutex);

	ret = interfaceIndexGet(DWPAL_CONN_TYPE_HOSTAP, VAPName, &idx);
	if (ret == DWPAL_SUCCESS && context[idx] != NULL) {
		console_printf("%s; Interface %s (idx= %d) is already up ==> cont...\n", __FUNCTION__, VAPName, idx);
		MUTEX_UNLOCK(&attach_mutex);
		return DWPAL_SUCCESS;
	}

#if defined EVENT_CALLBACK_THREAD
	if (dwpal_event_handler == -1)
	{
		if (dwpal_socket_create(&dwpal_event_handler /*output*/, EVENT_HANDLER_SOCKET) == DWPAL_FAILURE)
		{
			console_printf("%s; dwpal_socket_create returned ERROR ==> Abort!\n", __FUNCTION__);
			MUTEX_UNLOCK(&attach_mutex);
			return DWPAL_FAILURE;
		}
	}
#endif

	/* Cancel the listener thread, if it does exist */
#if defined EVENT_CALLBACK_THREAD
	threadSet(&eventHandlerThreadId, THREAD_CANCEL, NULL);
#endif
	threadSet(&g_listenerThreadInfo, THREAD_CANCEL, NULL);
	threadSet(&g_monitorThreadInfo, THREAD_CANCEL, NULL);

	ret = interfaceIndexCreate(DWPAL_CONN_TYPE_HOSTAP, VAPName, &idx);
	if (ret == DWPAL_FAILURE)
	{
		console_printf("%s; interfaceIndexCreate (VAPName= '%s') returned ERROR ==> Abort!\n", __FUNCTION__, VAPName);
		goto end;
	}
	else if (ret == DWPAL_INTERFACE_ALREADY_UP)
	{
		console_printf("%s; Interface (idx= %d) is already up ==> cont...\n", __FUNCTION__, idx);
		ret = DWPAL_SUCCESS;
		goto end;
	}

	console_printf("%s; interfaceIndexCreate successfully; returned idx= %d\n", __FUNCTION__, idx);

	ret = DWPAL_SUCCESS;

	MUTEX_LOCK(&context_mutex);
	dwpalService[idx]->isConnectionEstablishNeeded = false;

	if ((context[idx] == NULL) && (dwpal_hostap_interface_attach(&context[idx] /*OUT*/, VAPName, NULL /*use one-way interface*/) != DWPAL_SUCCESS))
	{
		console_printf("%s; dwpal_hostap_interface_attach (VAPName= '%s') returned ERROR ==> try later on...\n", __FUNCTION__, VAPName);

		/* in this case, continue and try to establish the connection later on */
		dwpalService[idx]->isConnectionEstablishNeeded = true;
		ret = DWPAL_INTERFACE_IS_DOWN;
	}
	MUTEX_UNLOCK(&context_mutex);

	/* Set the callback whether attach succeeded or not */
	dwpalService[idx]->hostapEventCallback = hostapEventCallback;

	if (ret == DWPAL_SUCCESS)
	{
		/* Must be done AFTER setting hostapEventCallback */
		interfaceConnectedOkSend(idx);
	}


end:
	/* Create the listener thread, if it does NOT exist yet */
	if (isAnyInterfaceActive())
	{
#if defined EVENT_CALLBACK_THREAD
		threadSet(&eventHandlerThreadId, THREAD_CREATE, eventHandlerThreadStart);
#endif
		threadSet(&g_listenerThreadInfo, THREAD_CREATE, listenerThreadStart);
		threadSet(&g_monitorThreadInfo, THREAD_CREATE, monitorThreadStart);
	}

	MUTEX_UNLOCK(&attach_mutex);
	return ret;
}
#endif /* DISABLE_DWPAL_HOSTAP_SUPPORT */
