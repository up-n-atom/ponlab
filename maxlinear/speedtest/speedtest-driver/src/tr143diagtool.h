// SPDX-License-Identifier: GPL-2.0
/*****************************************************************************
 * Copyright © 2022-2024 MaxLinear, Inc.
 *
 *****************************************************************************/
/*!
 * Common interface header file for user-space and speed test kernel module
 * Has declarations for TR-143 speed test data model
 */

#ifndef __TR143DIAGTOOL__
#define __TR143DIAGTOOL__

/*! structure to hold ip address
 */
enum IP_type {
	IPV4,
	IPV6
};
struct ipaddr {
	enum IP_type ip_type;
	union {
		__be32	ip4;
		struct in6_addr ip6; /*! For IPv6 - now, not in scope */
	} ip;
};

/*! Default Maximum duration (in second) for the Time-Based Transfer mode.
 * Tool must provide the proc for configuring the max test duration. */
#define TR143_MAX_DEFAULT_DURATION  30

/*! Default Maximum size (in bytes) for the File Transfer mode.
 * Tool must provide the proc for configuring this variable */
#define TR143_MAX_DEFAULT_SIZE 107374182400ULL /* Defaut is 100Gb, Overwritten (1ULL << 32 )*/
#define TR143_DIAG_URL_LEN  128
#define TR143_HOSTNAME_LEN  128
#define TR143_MAX_INC_RESULTS	20
#define TR143_MAX_CONN 10

/*! TR143 test type */
enum test_type {
    TR143_DIAG_TEST_UPLOAD,
    TR143_DIAG_TEST_DOWNLOAD,
};

/*! Test mode 0 - File Transfer , 1 -  Time-Based */
union mode {
	uint64_t duration;	/*! In seconds */
	uint64_t filesize;	/*! in bytes */
};

/*! This struct holds the TR143 configuration data */
struct tr143diag_config {
    enum test_type ttype;	/*! Test type */
    uint16_t nconn;			/*! Number of connections */
    uint8_t dscp;			/*! DSCP value for TCP connection */
    /*! Test mode 0 - File Transfer , 1 -  Time-Based */
    uint8_t tmode;
	union mode mode;		/*! transfer mode values */
	char wanif[IFNAMSIZ];	/* WAMN interface name */
    struct ipaddr wanip;	/*! WAN IP address. Set to zero to use default WAN */
    struct ipaddr serverip;	/*! server ip address */
    uint16_t serverport;	/*! Server port */
    /*! Server name. If not pssed serverip is used in HTTP request */
    char servername[TR143_HOSTNAME_LEN];
    /*! URL - Upload/download URL */
    char url[TR143_DIAG_URL_LEN];
	/*! Interval time in seconds to get TR-143 incremental results */
	int inc_interval;
};

/*! structure for TR143 Incremental results
 */
struct IncrementalResult {
	/*! The start time of the interval */
	uint64_t StartTime;
	/*! The end time of the interval */
	uint64_t EndTime;
	/*! Change in the value of TestBytesSent between StartTime and EndTime. */
	uint64_t TestBytesSent;
	/*! Change in the value of TestBytesReceived between StartTime and EndTime. */
	uint64_t TestBytesReceived;
	/*! The total number of bytes (at the IP layer) received on the Interface
	 *	between StartTime and EndTime */
	uint64_t TotalBytesReceived;
	/*! The total number of bytes (at the IP layer) sent on the Interface
	 *	between StartTime and EndTime. */
	uint64_t TotalBytesSent;
};

/*! The struct used to collect incremental results
 */
struct tr143inc_result {
	/*! Maximum entries of IncrementalResult */
	int max_entries;
	/*! Number of IncrementalResult entries */
	uint16_t num_entries;
	/*! Variable length array to store incremental results */
	struct IncrementalResult inc_result[];
};

/*! Default Maximum number of connections supported by tool.
 * Tool must provide the proc for configuring the MAX connection */
#define TR143_DEFAULT_MAX_CONN  4

struct  PerConnectionResult {
    /*! ROMTime in microsecond.
     *  For HTTP this is the time at which the client sends the GET command.*/
    uint64_t ROMTime;
    /*! BOMTime in microsecond.
     * For HTTP this is the time at which the first data packet is received.*/
    uint64_t BOMTime;

    /*! EOMTime in microsecond
     * For HTTP this is the time at which the last data packet is received.*/
    uint64_t EOMTime;
     /*! TCP Open Request time in microsecond.
      * For HTTP this is the time at which the TCP socket open (SYN) was sent
      * for the HTTP connection.
      * Tool should record the time of connection init.*/
    uint64_t TCPOpenRequestTime;
    /*! TCP Open Response time in microsecond.
     * For HTTP this is the time at which the TCP ACK to the socket opening the
     * HTTP connection was received.
     * Tool should record this time once TCP connection established. */
    uint64_t TCPOpenResponseTime;

    /*! The number of bytes of the test file sent during the FTP/HTTP
     * transaction including FTP/HTTP headers, between BOMTime and EOMTime.
     * Set to zero for download test */
    uint64_t TestBytesSent;

    /*! The number of bytes of the test file received during the FTP/HTTP
     * transaction including FTP/HTTP headers, between BOMTime and EOMTime.
     * Set to zero for upload test */
    uint64_t TestBytesReceived;

    /*! The total number of bytes (at the IP layer) received on the Interface
     * between BOMTime and EOMTime. This MAY be calculated by sampling
     * Stats.bytesReceived on the Interface object at BOMTime and at EOMTime and
     * subtracting.*/
    uint64_t TotalBytesReceived;

    /*! The total number of bytes (at the IP layer) sent on the Interface
     * between BOMTime and EOMTime. This MAY be calculated by sampling
     * Stats.bytesSent on the Interface object at BOMTime and at EOMTime and
     * subtracting.*/
    uint64_t TotalBytesSent;
};

enum tr143diag_error {
    TR143_NONE,      /* None - need be used in this tool */
    TR143_COMPLETED, /*! Test completed */
    TR143_Error_CannotResolveHostName, /*! Cannot resolve host name.
                                         Not reported by tr143diagtool */
    TR143_Error_NoRouteToHost,  /*! No route to host */
    TR143_Error_InitConnectionFailed,   /*! Connection init failed */
    TR143_Error_NoResponse, /*! No response after HTTP request */
    TR143_Error_PasswordRequestFailed, /*! Invalid Password. FTP tests */
    TR143_Error_LoginFailed,    /*! LOgin failed - For FTP tests only */
    TR143_Error_NoTransferMode, /*! No transfer mode */
    TR143_Error_NoPASV, /*! For FTP tests */
    TR143_Error_NoCWD,  /*! For FTP tests */
    TR143_Error_NoSTOR, /*! For FTP tests */
    TR143_Error_NoTransferComplete, /*! Transfer did not complete */
    TR143_Error_Timeout,    /*! Timeout */
    TR143_Error_Internal,   /*! INternal error */
    TR143_Error_Other,       /*! Unknown error */
};

/*! The struct to hold the diagnostics test results. The individial connections
 *  results are also stored in this struct.
 */	
struct tr143diag_result {
	enum test_type ttype;  /*! Test type - UPLOAD/DOWNLOAD */
    enum tr143diag_error err; /*! Error. On success set to TR143_COMPLETED */
	int http_status_code;	/* HTTP response status code */
    uint64_t ROMTime;    /*! ROMTime in microsecond - for the test */
    uint64_t BOMTime;    /*! BOMTime in microsecond - for the test */
    uint64_t EOMTime;    /*! EOMTime in microsecond - for the test */

    /*!The number of bytes of the test file sent during the FTP/HTTP
     * transaction including FTP/HTTP headers, between BOMTime and EOMTime
     * acrosss all connections.
     * Set to zero for download test */
    uint64_t TestBytesSent;

    /*!The number of bytes of the test file received during the FTP/HTTP
     * transaction including FTP/HTTP headers, between BOMTime and EOMTime
     * acrosss all connections.
     * Set to zero for upload test */
    uint64_t TestBytesReceived;

    /*! The total number of bytes (at the IP layer) received on the Interface
     * between BOMTime and EOMTime. This MAY be calculated by sampling
     * Stats.bytesReceived on the Interface object at BOMTime and at EOMTime and
     * subtracting.*/
    uint64_t TotalBytesReceived;

    /*! The total number of bytes (at the IP layer) sent on the Interface
     * between BOMTime and EOMTime. This MAY be calculated by sampling
     * Stats.bytesSent on the Interface object at BOMTime and at EOMTime and
     * subtracting. */
    uint64_t TotalBytesSent;

    /*! The number of bytes of the test file sent between the latest
     * PerConnectionResult.{i}.BOMTime and the earliest
     * PerConnectionResult.{i}.EOMTime across all connections.
     * Set to zero for download test*/
    uint64_t TestBytesSentUnderFullLoading;

    /*! The number of bytes of the test file received between the latest
     * PerConnectionResult.{i}.BOMTime and the earliest
     * PerConnectionResult.{i}.EOMTime across all connections.
     * Set to zero for download test*/
    uint64_t TestBytesReceivedUnderFullLoading;

    /*! The total number of bytes (at the IP layer) received between the latest
     * PerConnectionResult.{i}.BOMTime and the earliest
     * PerConnectionResult.{i}.EOMTime across all connections in the test. This
     * MAY be calculated by sampling Stats.bytesReceived on the Interface object
     * at the latest PerConnectionResult.{i}.BOMTime and at the earliest
     * PerConnectionResult.{i}.EOMTime and subtracting.*/
    uint64_t TotalBytesReceivedUnderFullLoading;

    /*! The total number of bytes (at the IP layer) sent between the latest
     * PerConnectionResult.{i}.BOMTime and the earliest
     * PerConnectionResult.{i}.EOMTime across all connections in the test. This
     * MAY be calculated by sampling Stats.bytesSent on the Interface object at
     * the latest PerConnectionResult.{i}.BOMTime and at the earliest
     * PerConnectionResult.{i}.EOMTime and subtracting.*/
    uint64_t TotalBytesSentUnderFullLoading;

    /*! The period of time in microseconds between the latest
     * PerConnectionResult.{i}.BOMTime and the earliest
     * PerConnectionResult.{i}.EOMTime of the test.*/
    uint64_t periodOfFullLoading;

    uint16_t nconn; /*! Number of connections */

    /*! Per connection result */
    struct PerConnectionResult PerConnectionResult[TR143_MAX_CONN];
};

/*! tr143diagtool notification types.
 * OS may have delay in waking up the applications. Applications shall factor
 * in such delay.
 */
enum tr143_notifytype {
    TR143_DIAG_NTFY_NONE,   /*! Do not wait for any event */
    TR143_DIAG_NTFY_ERR,	/*! To report error to application */
    TR143_DIAG_NTFY_BOM,    /*! Wait until BOM for the test recorded *or an error */
    TR143_DIAG_NTFY_EOM,    /*! Wait tunit EOM for the test recorded or an error */
    TR143_DIAG_NTFY_HTTP,	/*! To report http status code */
};

struct tr143diag_notify {
    enum tr143_notifytype ntype; /*! Notify type */
	/*! Value associated with notify type */
	union {
		uint64_t time;					/*! Time in microsecond. Interpretation is based on ntype */
		enum tr143diag_error err_code;	/*! Error code to be notified to application */
	};
	int http_code;						/*! Notify http status code */
};

/*! This ioctl is used to configure the test parameters for tr143diagtool */
#define TR143_DIAG_IOCTL_CONFIG     _IOW('T',   1, struct tr143diag_config)
/*! This ioctl is used to start the performance dianostic test(TR-143 test) */
#define TR143_DIAG_IOCTL_START      _IOWR('T',   2, struct tr143diag_notify)
/*! This ioctl is used to abort the performance dianostic test(TR-143 test) */
#define TR143_DIAG_IOCTL_ABORT      _IO('T',   3)
/*! This ioctl is used to get resulf of performance dignostic test */
#define TR143_DIAG_IOCTL_GETRESULT  _IOR('T',   4, struct tr143diag_result)
/*! This ioctl is used to get incremental results of performance dignostic test */
#define TR143_DIAG_IOCTL_GET_INCRESULT  _IOR('T',   5, struct tr143inc_result)
/*! This ioctl is used to notify the dignostic test event. To use the eventing
 * fd should be blocking */
#define TR143_DIAG_IOCTL_NOTIFY     _IOWR('T',   6, struct tr143diag_notify)

#endif //__TR143DIAGTOOL__
