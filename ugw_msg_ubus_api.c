/********************************************************************************
 
  Copyright (c) 2016
  Lantiq Beteiligungs-GmbH & Co. KG
  Lilienthalstrasse 15, 85579 Neubiberg, Germany 

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.
 
********************************************************************************/

/*  ***************************************************************************** 
 *         File Name    : ugw_msg_ubus_api.c                                    *
 *         Description  : UGW Msg API layer which exposes APIs that can be used *
 *			  to communicate with UGW framework from non UGW 	*
 *			  component 						*
 *  *****************************************************************************/


#include "ugw_msg_api.h"


cb_func gnMyCallFunc;

static int ubus_notifyMsg(struct ubus_context *pxCtx,
			    __attribute__ ((unused))struct ubus_object *pxObj,
			    struct ubus_request_data *pxReq,
			    __attribute__ ((unused))const char *pcMethod,
			    struct blob_attr *pxMsg);

/*  =============================================================================
 *   Function name 	: ubus_notifyMsg					*
 *   Description 	: Function ubus_notifyMsg is call back function which	*
 *   			  will be called on notify.msg Method.			*
 *  ============================================================================*/
static int ubus_notifyMsg(struct ubus_context *pxCtx,
			    __attribute__ ((unused))struct ubus_object *pxObj,
			    struct ubus_request_data *pxReq,
			    __attribute__ ((unused))const char *pcMethod,
			    struct blob_attr *pxMsg)
{

	Msg_t xMsg;
	int nRet=UGW_SUCCESS;

	memset(&xMsg, 0, sizeof xMsg);

	//parse blob msg and fill the standrad struct.
	nRet = msg_recv(pxMsg, &xMsg);

	if(nRet == UGW_FAILURE)
		return UGW_FAILURE;

	// After successfully parsing msg call callback function to use the msg.
	nRet = gnMyCallFunc(&xMsg);

	if(nRet == UGW_FAILURE)
		return UGW_FAILURE;

	/* After callback return send data back to client */
	if(xMsg.nRMsgSize == 0){
		//printf("No Reply msg to send back\n");
	}else
		nRet = msg_sendReply(&xMsg,pxCtx,pxReq);

	return nRet;
}


/*  =============================================================================
 *   Function name 	: msg_fd_set_cloexec					*
 *   Description 	: function flush out file stream on exec  which is 	*
 * 			  associated fd 					*
 *  ============================================================================*/
static void msg_fd_set_cloexec(int fd)
{
        fcntl(fd, F_SETFD, fcntl(fd, F_GETFD) | FD_CLOEXEC);
}

/*  =============================================================================
 *   Function name 	: msg_ubus_add_fd					*
 *   Description 	: function to add fd to ubus 				* 
 *  ============================================================================*/
static void
msg_ubus_add_fd(struct ubus_context *pxServer)
{
	ubus_add_uloop(pxServer);
	msg_fd_set_cloexec(pxServer->sock.fd);
}

/*  =============================================================================
 *   Function name 	: msg_ubus_reconnect_timer				*
 *   Description 	: function reconnect server to ubusd on ubusd restart	* 	
 *  ============================================================================*/
static void
msg_ubus_reconnect_timer(struct uloop_timeout *timeout)
{

	struct ubus_auto_conn *conn = container_of(timeout, struct ubus_auto_conn, timer);

	static struct uloop_timeout retry = {
		.cb = msg_ubus_reconnect_timer,
	};
	int t = 2;

	if (ubus_reconnect(&conn->ctx, NULL) != 0) {
		printf(" failed to reconnect, trying again in %d seconds\n", t);
		uloop_timeout_set(&retry, t * 1000);
		return;
	}

	msg_ubus_add_fd(&conn->ctx);
}

/*  =============================================================================
 *   Function name 	: msg_ubus_connection_lost				*
 *   Description 	: Call back function connection lost with ubusd		*
 *  ============================================================================*/
static void
msg_ubus_connection_lost(__attribute__ ((unused)) struct ubus_context *pxCtx)
{
	msg_ubus_reconnect_timer(NULL);
}

/*  =============================================================================
 *   Function name 	: msg_add_object					*
 *   Description 	: Add methods to ubusd 					*
 *  ============================================================================*/
static int msg_add_object(struct ubus_object *pxObj, struct ubus_context *pxServer)
{
        int nRet = ubus_add_object(pxServer, pxObj);
        if (nRet != 0)
	{
                printf(" Failed to publish object '%s': %s\n", pxObj->name, ubus_strerror(nRet));
	}
	return nRet;

}

/*  =============================================================================
 *   Function Name 	: ubusConnect						*
 *   Description 	: Function to create ubus connection			*
 *  ============================================================================*/
OUT struct ubus_context * ubusConnect(void);
OUT struct ubus_context * ubusConnect(void){
	const char *ubus_socket = NULL;
	struct ubus_context *ctx;
	ctx =  ubus_connect(ubus_socket);
	if(ctx)
		return ctx;
	else 
		return NULL;
}

/*  =============================================================================
 *   Function Name 	: ubusDisconnect					*
 *   Description 	: Function to diconnect the ubus connection		*
 *  ============================================================================*/
void ubusDisconnect(IN struct ubus_context * ctx);
void ubusDisconnect(IN struct ubus_context * ctx)
{
	if(ctx != NULL)
	{
		ubus_free(ctx);
		ctx = NULL;
	}
}

/*  =============================================================================
 *   Function Name 	: msg_ubusConnect					*
 *   Description 	: Function to create the ubus connection and wait for 	*
 *			  ubus msges.						*
 *  ============================================================================*/
int32_t msg_ubusConnect(cb_func myFunc, char *sSerName)
{

	struct ubus_context *pxServer=NULL;

	//ubus method
	struct ubus_method api_methods[] = {
		UBUS_METHOD_NOARG("notify.msg",ubus_notifyMsg)
	};

	// ubus object type
	struct ubus_object_type api_object_type =
	UBUS_OBJECT_TYPE(sSerName, api_methods);

	struct ubus_object api_object = {
		.name = sSerName,
		.type = &api_object_type,
		.methods = api_methods,
		.n_methods = ARRAY_SIZE(api_methods)
	};

	//assigning callback functions to call on ubus messages arrival.
	gnMyCallFunc = myFunc;

	uloop_init();
	pxServer = ubusConnect();
	if(!pxServer)
	{
		printf("\n MSG API>> Failed to connect to ubus\n");
		return UGW_FAILURE;
	}

	pxServer->connection_lost = msg_ubus_connection_lost;
	msg_ubus_add_fd(pxServer);

	if(msg_add_object(&api_object, pxServer))
	{
		printf("\n MSG API>> Failed host ubus server[%s]\n",api_object.name);
		return UGW_FAILURE;
	}

	//looping and waiting for ubus msges
	printf("\nMSG API>> Looping and waiting ubus messages: server[%s]\n",api_object.name);
	uloop_run();
	printf("\nMSG API>> Looping End : server[%s]\n",api_object.name);
	
	ubusDisconnect(pxServer);

	uloop_done();
	return UGW_SUCCESS;
}

