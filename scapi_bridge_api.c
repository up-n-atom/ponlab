/******************************************************************************

  Copyright (C) 2022 MaxLinear, Inc.
  Copyright (C) 2017-2018 Intel Corporation
  Lantiq Beteiligungs-GmbH & Co. KG

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/
#define __GLIBC__ 0
#define __USE_MISC 0

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>
#include <net/if.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#ifdef CONFIG_USE_MUSL
#include <errno.h>
#include <fcntl.h>
#else
#include <sys/errno.h>
#include <sys/fcntl.h>
#endif
#include <linux/sockios.h>
#include <ulogging.h>
#include <ltq_api_include.h>
#include <linux/if_bridge.h>

#define SYSFS_CLASS_NET "/sys/class/net/" 


/* 
** =============================================================================
**   Function Name    :strtotimeval
**
**   Description      :
**
**   Parameters       :time(IN) timeval(OUT)
**
**   Return Value     :Success -> EXIT_SUCCESS
**                     Failure -> Different -ve values
** 
**   Notes            :structure pointed by tv should be created by caller
**
** ============================================================================
*/

static int strtotimeval(struct timeval *tv, const char *time)
{
    double secs = 0;
    if (sscanf_s(time, "%lf", &secs) != 1)
	return -1;

    tv->tv_sec = secs;
    tv->tv_usec = 1000000 * (secs - tv->tv_sec);

    return 0;
}


/* 
** =============================================================================
**   Function Name    :if_nametoindex
**
**   Description      :map a network interface name to its corresponding index
**
**   Parameters       :ifname(IN)
**
**   Return Value     :Success -> index ifname
**                      Failure -> zero.
** 
**   Notes            :
**
** ============================================================================
*/

unsigned int if_nametoindex(const char *ifname)
{
    int nIndex;
    int ctl_sock;
    struct ifreq ifr;

    memset(&ifr, 0, sizeof(struct ifreq));
    if (sprintf_s(ifr.ifr_name, IFNAMSIZ, "%s", ifname) <= 0 ) {
	LOGF_LOG_ERROR( "Error : sprintf_s failed %s\n", strerror(errno)); 
	return 0;
    }
    ifr.ifr_name[IFNAMSIZ - 1] = 0;

    nIndex = 0;
    if ((ctl_sock = socket(AF_INET, SOCK_DGRAM, 0)) >= 0) {
        if (ioctl(ctl_sock, SIOCGIFINDEX, &ifr) >= 0) {
            nIndex = ifr.ifr_ifindex;
        }
        close(ctl_sock);
    }
    return nIndex;
}


/* 
** =============================================================================
**   Function Name    :set_sysfs
**
**   Description      :Writes the value of bridge to sys path
**
**   Parameters       :path(IN), value(IN)
**
**   Return Value     :Success -> EXIT_SUCCESS
**                     Failure -> Different -ve values
** 
**   Notes            :
**
** ============================================================================
*/

static int set_sysfs(const char *path, unsigned long value)
{
    int fd, ret = 0, cc;
    char buf[32]={0};

    fd = open(path, O_WRONLY);
    if (fd < 0){
	ret = -errno;
	LOGF_LOG_ERROR("ERROR = %d --> %s\n", ret, strerror(-ret));
	goto returnHandler;
    }
    if ( (cc = sprintf_s(buf, sizeof(buf), "%lu", value)) <= 0) {
	LOGF_LOG_ERROR( "Error : sprintf_s failed %s\n", strerror(errno)); 
	ret=-1;
	goto returnHandler;
    }
    if(cc>0){
    	if (write(fd, buf, cc) < 0)
		ret = -1;
    }
returnHandler:
    if(fd != -1)
    	close(fd);
    return ret;
}



/* 
** =============================================================================
**   Function Name    :br_set
**
**   Description      :Execute the previously set command 
**
**   Parameters       :bridge(IN), name(IN), value(IN), oldcode(IN)
**
**   Return Value     :Success -> EXIT_SUCCESS
**                     Failure -> Different -ve values
** 
**   Notes            :
**
** ============================================================================
*/

static int br_set(const char *bridge, const char *name,
		  unsigned long value, unsigned long oldcode)
{
    int iRet, fd = -1;
    char path[128] = { 0 };

    if (sprintf_s(path, 128, SYSFS_CLASS_NET "%s/bridge/%s", bridge, name) <= 0) {
	LOGF_LOG_ERROR( "Error : sprintf_s failed %s\n", strerror(errno)); 
	iRet=-1;
	goto errorHandler;
    }

    if ((iRet = set_sysfs(path, value)) < 0) 
    {
	struct ifreq ifr;
	unsigned long args[4] = { oldcode, value, 0, 0 };

	if (sprintf_s(ifr.ifr_name, IFNAMSIZ, "%s", bridge) <= 0) {
		LOGF_LOG_ERROR( "Error : sprintf_s failed %s\n", strerror(errno)); 
		iRet=-1;
		goto errorHandler;
    	}
	ifr.ifr_data = (char *) &args;
        fd = socket(AF_INET, SOCK_STREAM, 0);
        if ( fd < 0 )
        {
       	    iRet = errno;
	    LOGF_LOG_ERROR( "socket error: %s \n", strerror(errno));
            goto errorHandler;
        }

	iRet = ioctl(fd, SIOCDEVPRIVATE, &ifr);

        if ( iRet < 0 )
        {
       	    iRet = errno;
	    LOGF_LOG_ERROR( "IOCTL call failed: %s\n",strerror(errno));
            goto errorHandler;
        }
    }

errorHandler:
    if(fd != -1)
	close(fd);
    return iRet;
}

static inline unsigned long tv_to_jiffies(struct timeval *tv)
{
    unsigned long long jif;

    jif = 1000000ULL * tv->tv_sec + tv->tv_usec;
    return jif/10000;
}


/* 
** =============================================================================
**   Function Name    :scapi_brAdd
**
**   Description      :Add bridge
**
**   Parameters       :br(IN)
**
**   Return Value     :Success -> EXIT_SUCCESS
**                      Failure -> Different -ve values
** 
**   Notes            :
**
** ============================================================================
*/

int scapi_brAdd(char *br)
{
    int fd, iRet = 0;

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if ( fd < 0 )
    {
        iRet = errno;
	LOGF_LOG_ERROR( "socket error: %s \n", strerror(errno));
        goto errorHandler;
    }

    iRet = ioctl(fd, SIOCBRADDBR, br);

    if ( iRet < 0 )
    {
        iRet = errno;
	LOGF_LOG_ERROR( "IOCTL call failed: %s\n",strerror(errno));
        goto errorHandler;
    }

errorHandler:
     close(fd);
     return iRet;
}


/* 
** =============================================================================
**   Function Name    :scapi_brDel
**
**   Description      :Delete bridge
**
**   Parameters       :br(IN)
**
**   Return Value     :Success -> EXIT_SUCCESS
**                      Failure -> Different -ve values
** 
**   Notes            :
**
** ============================================================================
*/

int scapi_brDel(char *br)
{
    int fd, iRet = 0;

    fd = socket(AF_INET, SOCK_STREAM, 0); 
    if ( fd < 0 ) 
    {   
        iRet = errno;
	LOGF_LOG_ERROR( "socket error: %s \n", strerror(errno));
        goto errorHandler;
    }

    iRet = ioctl(fd, SIOCBRDELBR, br);

    if ( iRet < 0 ) 
    {   
        iRet = errno;
	LOGF_LOG_ERROR( "IOCTL call failed: %s\n",strerror(errno));
        goto errorHandler;
    }

errorHandler:
     close(fd);
     return iRet;
}


/* 
** =============================================================================
**   Function Name    :scapi_brAddIf
**
**   Description      :Add interface to bridge
**
**   Parameters       :br(IN), ifname(IN)
**
**   Return Value     :Success -> EXIT_SUCCESS
**                      Failure -> Different -ve values
** 
**   Notes            :
**
** ============================================================================
*/

int scapi_brAddIf(char *br, char *ifname )
{
    int fd, iRet = 0;
    struct ifreq ifr;
    int if_index;

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if ( fd < 0 )
    {
        iRet = errno;
	LOGF_LOG_ERROR( "socket error: %s \n", strerror(errno));
        goto errorHandler;
    }

    if (sprintf_s(ifr.ifr_name, IFNAMSIZ, "%s", br) <= 0) {
	LOGF_LOG_ERROR( "Error : sprintf_s failed %s\n", strerror(errno)); 
	iRet=-1;
	goto errorHandler;
    }
    if_index = if_nametoindex(ifname);
    if (!if_index) 
    {
        iRet = errno;
        LOGF_LOG_ERROR( "if_nametoindex() func call failed: %s \n", strerror(errno));
	goto errorHandler;
    }

    ifr.ifr_ifindex = if_index;
    iRet = ioctl(fd, SIOCBRADDIF, &ifr);

    if ( iRet < 0 )
    {
        iRet = errno;
	LOGF_LOG_ERROR( "IOCTL call failed: %s\n",strerror(errno));
        goto errorHandler;
    }

errorHandler:
     close(fd);
     return iRet;
}


/* 
** =============================================================================
**   Function Name    :scapi_brDelIf
**
**   Description      :Delete interface to bridge
**
**   Parameters       :br(IN), ifname(IN)
**
**   Return Value     :Success -> EXIT_SUCCESS
**                      Failure -> Different -ve values
** 
**   Notes            :
**
** ============================================================================
*/

int scapi_brDelIf(char *br, char *ifname )
{
    int fd, iRet = 0;
    struct ifreq ifr;
    int if_index;

   fd = socket(AF_INET, SOCK_STREAM, 0);
    if ( fd < 0 )
    {
        iRet = errno;
	LOGF_LOG_ERROR( "scoket call failed: %s\n",strerror(errno));
        goto errorHandler;
    }

    if (sprintf_s(ifr.ifr_name, IFNAMSIZ, "%s", br) <=0 ) {
	LOGF_LOG_ERROR( "Error : sprintf_s failed %s\n", strerror(errno)); 
	iRet=-1;
	goto errorHandler;
    }
    if_index = if_nametoindex(ifname);
    if (!if_index)
    {
        iRet = errno;
	LOGF_LOG_ERROR( "if_nametoindex() func call failed: %s \n", strerror(errno));
        goto errorHandler;
    }

    ifr.ifr_ifindex = if_index;
    iRet = ioctl(fd, SIOCBRDELIF, &ifr);

    if ( iRet < 0 )
    {
        iRet = errno;
	LOGF_LOG_ERROR( "IOCTL call failed: %s\n",strerror(errno));
        goto errorHandler;
    }

errorHandler:
     close(fd);
     return iRet;
}

/* 
** =============================================================================
**   Function Name    :scapi_brSetStp
**
**   Description      :Set STP to bridge
**
**   Parameters       :br(IN), br_option(IN)
**
**   Return Value     :Success -> EXIT_SUCCESS
**                      Failure -> Different -ve values
** 
**   Notes            :
**
** ============================================================================
*/

int scapi_brSetStp(char *br, bool br_option )
{
    int iRet = 0, stp = -1;

    if (br_option == ENABLE)
        stp = 1;
    else
        stp = 0;

    iRet = br_set(br, "stp_state", stp, BRCTL_SET_BRIDGE_STP_STATE);
    if ( iRet < 0 )
    {
        iRet = errno;
	LOGF_LOG_ERROR( "br_set func call failed : %s\n",strerror(errno));
        goto errorHandler;
    }

errorHandler:
     return iRet;
}

/* 
** =============================================================================
**   Function Name    :scapi_brSetPrio
**
**   Description      :Set bridge priority
**
**   Parameters       :br(IN), br_option(IN)
**
**   Return Value     :Success -> EXIT_SUCCESS
**                      Failure -> Different -ve values
** 
**   Notes            :
**
** ============================================================================
*/

int scapi_brSetPrio(char *br, unsigned int br_option)
{

    int iRet = 0;

    iRet = br_set(br, "priority", br_option, BRCTL_SET_BRIDGE_PRIORITY);
    if ( iRet < 0 )
    {
	LOGF_LOG_ERROR( "br_set func call failed : %s\n",strerror(errno));
        goto errorHandler;
    }
errorHandler:
    return iRet;
}
    
/* 
** =============================================================================
**   Function Name    :scapi_brSetHello
**
**   Description      :Set bridge hello time
**
**   Parameters       :br(IN), time(IN)
**
**   Return Value     :Success -> EXIT_SUCCESS
**                      Failure -> Different -ve values
** 
**   Notes            :
**
** ============================================================================
*/

int scapi_brSetHello(char *br, char *time)
{

    int iRet = 0;
    struct timeval tv;

    if (strtotimeval(&tv, time)) {
	LOGF_LOG_ERROR( "bad ageing time value \n");
        iRet = -1;
        goto errorHandler;
    }

    iRet = br_set(br, "hello_time", tv_to_jiffies(&tv), BRCTL_SET_BRIDGE_HELLO_TIME);
    if(iRet < 0)
    {
	LOGF_LOG_ERROR( "br_set func call failed : %s\n",strerror(errno));
	goto errorHandler;
    }

errorHandler:
    return iRet;
}


/* 
** =============================================================================
**   Function Name    :scapi_brSetAge
**
**   Description      :Set bridge ageing time
**
**   Parameters       :br(IN), time(IN)
**
**   Return Value     :Success -> EXIT_SUCCESS
**                      Failure -> Different -ve values
** 
**   Notes            :
**
** ============================================================================
*/

int scapi_brSetAge(char *br, char *time)
{

    int iRet = 0;
    struct timeval tv;

    if (strtotimeval(&tv, time)) {
	LOGF_LOG_ERROR( "bad ageing time value\n");
	iRet = -1;
	goto errorHandler;
    }

    iRet = br_set(br, "ageing_time", tv_to_jiffies(&tv), BRCTL_SET_AGEING_TIME);
    if(iRet == -1)
    {
	LOGF_LOG_ERROR( "br_set fun call failed \n");
        goto errorHandler;
    }

errorHandler:
    return iRet;
}


/* 
** =============================================================================
**   Function Name    :scapi_brFwdDelay
**
**   Description      :Set bridge forward delay time
**
**   Parameters       :br(IN), time(IN)
**
**   Return Value     :Success -> EXIT_SUCCESS
**                      Failure -> Different -ve values
** 
**   Notes            :
**
** ============================================================================
*/

int scapi_brFwdDelay(char *br, char *time)
{

    int iRet = 0;
    struct timeval tv;

    if (strtotimeval(&tv, time)) {
	LOGF_LOG_ERROR( "bad ageing time value\n");
        iRet = -1;
        goto errorHandler;
    }

    iRet = br_set(br, "forward_delay", tv_to_jiffies(&tv), BRCTL_SET_BRIDGE_FORWARD_DELAY);
    if(iRet < 0)
    {
	LOGF_LOG_ERROR( "br_set fun call failed \n");
        goto errorHandler;
    }

errorHandler:
    return iRet;
}


/* 
** =============================================================================
**   Function Name    :scapi_brSetMaxAge
**
**   Description      :Set bridge max message age time
**
**   Parameters       :br(IN), time(IN)
**
**   Return Value     :Success -> EXIT_SUCCESS
**                      Failure -> Different -ve values
** 
**   Notes            :
**
** ============================================================================
*/

int scapi_brSetMaxAge(char *br, char *time)
{

    int iRet = 0;
    struct timeval tv;

    if (strtotimeval(&tv, time)) {
	LOGF_LOG_ERROR( "bad ageing time value\n");
        iRet = -1;
        goto errorHandler;
    }

    iRet = br_set(br, "max_age", tv_to_jiffies(&tv), BRCTL_SET_BRIDGE_MAX_AGE);
    if(iRet < 0)
    {
	LOGF_LOG_ERROR( "br_set fun call failed \n");
        goto errorHandler;
    }

errorHandler:
    return iRet;
}






