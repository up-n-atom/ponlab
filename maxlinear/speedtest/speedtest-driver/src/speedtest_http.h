// SPDX-License-Identifier: GPL-2.0
/*****************************************************************************
 * Copyright © 2022-2024 MaxLinear, Inc.
 *
 *****************************************************************************/
/*!
 * This file contains header declarations for http protocol
 */

#ifndef __SPEEDTEST_HTTP_H_
#define __SPEEDTEST_HTTP_H_

#define HTTP_REASON_PHRASE_LEN_MAX		24
#define HTTP_VERSION_LEN				10
#define HTTP_HEADER_LEN_MAX				128
#define HTTP_REQUEST_HEADER_SIZE_MAX	400
#define HTTP_RESPONSE_HEADER_SIZE_MAX	2048

/*! HTTP header types
 */
enum HTTP_HEADER_TYPE {
    /*! The MIME type of content */
    HTTP_CONTENT_TYPE,
    /*! The length of the response body expressed in 8-bit bytes */
    HTTP_CONTENT_LEN,
    /*! An identifier for a specific version of a resource,
		often a message digest */
    HTTP_ETAG,
    /*! server uses this Alternative Services to indicate that
		its resources can also be accessed at a different network location */
    HTTP_ALT_SVC,
	/*! Used when a new resource has been created */
	HTTP_LOCATION,
	/*! Control options for the current connection and list of
		hop-by-hop response fields */
	HTTP_CONNECTION,
	/*! End of headers */
	HTTP_HEADER_TYPE_MAX
};

/*! Structure to hold HTTP header with it's value
 */ 
struct http_header {
	/*! Flag to check and ensure particular header is present in response */
    bool valid;
    /*! To hold the value of http parameter */
    union {
        uint64_t num;
        char str[HTTP_HEADER_LEN_MAX];
    } value;
};

/*! Struct to hold the http response message
 */
struct http_response {
	/*! server supporting HTTP version */
	char version[HTTP_VERSION_LEN];
	/*! standard 3-digit status code */
    int status_code;
	/*! short textual description of the status-code*/
    char text_phrase[HTTP_REASON_PHRASE_LEN_MAX];
    /*! http response headeres */
	struct http_header hdr[HTTP_HEADER_TYPE_MAX + 1];
};


int sptest_http_client_kthread(void *data);
int tr143_stats_collect_kthread(void *data);

#endif //__SPEEDTEST_HTTP_H_
