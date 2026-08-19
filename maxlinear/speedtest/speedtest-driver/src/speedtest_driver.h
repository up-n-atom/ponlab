// SPDX-License-Identifier: GPL-2.0
/*****************************************************************************
 * Copyright © 2021 - 2024 MaxLinear, Inc.
 *
 *****************************************************************************/
/*!
 * Main header file for speed test driver
 */

#ifndef __SPEEDTEST_DRIVER_H_
#define __SPEEDTEST_DRIVER_H_

#include <linux/printk.h>
#include <linux/socket.h>
#include <linux/kthread.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <linux/list.h>
#include <linux/cdev.h>
#include <linux/proc_fs.h>
#include <linux/device.h>
#include <uapi/linux/if.h>
#include <linux/in6.h>
#include <linux/completion.h>

#include "tr143diagtool.h"

#ifndef STR
	#define STR(macro)	#macro
#endif

/* basic macro definitions */
#define SUCCESS		(0)
#define FAILURE		(-1)
#define SPTEST_TASKS_MAX			TR143_MAX_CONN
#define MODULE_NAME		"tr143diagtool"
#define MAX_IP_LEN	46
#define MAX_CPUS		NR_CPUS

/*! sptest_proc_limits holds information of allowed configuration
 */
struct sptest_proc_limits {
	/*! Maximum data length allowed for File based test in bytes */
	uint64_t max_size;
	/*! Maximum simultaneous connections allowed */
	uint16_t max_conn;
	/*! Maximum duration allowed for Time based test in seconds */
	uint64_t max_duration;
	/*! Buffer size to allocate number of pages for upload test */
	uint64_t buffer_size;
	/*! Page order to be used for allocating pages for upload test */
	uint16_t page_order;
};

/*! speedtest manager states
 *	This state will be changed by ioctl calls
 */
enum spmanager_state {
	/*! This state indicates init state of driver or
	 * TR143_DIAG_IOCTL_CONFIG can be set */
	SPMANAGER_STATE_NONE,
	/*! This state indicates TR143_DIAG_IOCTL_CONFIG is completed */
	SPMANAGER_STATE_CONFIGURED,
	/*! This state to indicates TR143_DIAG_IOCTL_START is completed
	   And test is under progress */
	SPMANAGER_STATE_RUNNING,
	/*! This state will be set once TR143_DIAG_IOCTL_ABORT is completed */
	SPMANAGER_STATE_ABORTED,
	/*! This state states that test is completed and ready for
	   TR143_DIAG_IOCTL_GETRESULTS command */
	SPMANAGER_STATE_COMPLETED,
};

/*! sptest_driver holds the driver related information
 */
struct sptest_driver {
	struct cdev cdev;				/*! Charector device */
	struct sptest_proc_limits proc_limits;
	char name[sizeof(MODULE_NAME)];	/*! Device file name */
	dev_t dev_num;					/*! major and minor number */
	atomic_t open_count;			/*! Allowing one process at a time */
	wait_queue_head_t waitq;		/*! poll wait queue,currently not supported */
	struct class *dev_class;		/*! Device class */
	struct device *device;			/*! Device pointer */
	struct proc_dir_entry *parent;	/*! proc file parent entry */
	void *priv_buf;					/*! private buffer to store results */
	void *user_data;				/*! Holds the user configuration */
	/*! Holds reference to struct sptest_task_common_data */
	struct sptest_task_common_data *common_data;
	int nconns;						/*! number of connnections */
	enum spmanager_state mgr_state;	/*! IOCTL state */
};

/*! states sptest_task instance
 *	state change will be updated speedtest manager and http client thread
 */
enum sptest_task_state {
	/*! defualt state of sptest_task */
	SPTEST_TASK_STATE_NONE,
	/*! Newly created sptest_task instances will be configured with this state */
    SPTEST_TASK_STATE_CREATED,
	/*! This state is set once thread routine starts running */
    SPTEST_TASK_STATE_RUNNING,
	/*! This state is set once thread routine run failed in-between */
    SPTEST_TASK_STATE_RUN_FAILED,
	/*! This state is set once test is started i.e BOMTime */
    SPTEST_TASK_STATE_TEST_INPROGRESS,
	/*! This state is set after test is finished i.e EOMTime */
    SPTEST_TASK_STATE_TEST_COMPLETED,
	/*! This state is set from thread routine while exiting */ 
    SPTEST_TASK_STATE_CLOSING,
	/*! speedtest manager will set this state, after freeing the reference of kthread */
	SPTEST_TASK_STATE_CLOSED,
	/*! This state will be set when thread routine fails to
		establish connection or fails to send data */
    SPTEST_TASK_STATE_START_FAILED,
};

/*! The struct to hold parameters for http session kthread configuration
 *	This strcture is referred in http_user_config
 */
struct http_user_config {
	int nconns;		/*! Number of simultaneous connections */
	enum test_type ttype;  /*! Test type - UPLOAD/DOWNLOAD */
	uint8_t dscp; /*! DSCP value for TCP connection */
	uint8_t tmode; /*! Test mode 0 - File Transfer , 1 -  Time-Based */
	/*! Time in seconds */
	uint64_t duration;
	/*! IncrementalResult interval in seconds */
	int inc_interval;
	/*! data length in bytes */
	uint64_t data_len;
	/*! http server ip server address to be connected */
	struct ipaddr server_ip;
	/*! For HTTP port is 80.
	 *	Keeping this configuration as it is */
	uint16_t server_port;
	/*! Server name. If not passed serverip is used in HTTP request */
	char server_name[TR143_HOSTNAME_LEN];
	/*! URL - Upload/download URL */
	char url[TR143_DIAG_URL_LEN];
	/*! WAN IP address */
	struct ipaddr wanip;
	/*! WAN interface name */
	char wanif[IFNAMSIZ];
};

/*! This structure for stats collection thread with timer */
struct tr143_stats_collector_thread {
	int tr143_prev;
	/*! task_struct to hold ref of tr143 stats collector kthread */
	struct task_struct *task_stats_collect;
	/*! wait queue for stats collector thread */
	wait_queue_head_t tr143_wait_queue;
	/*!flag for wait_event condition */
	bool tr143_t;
};

/*! TR143 timer structure for TimeBased and incremental results
 */
struct tr143_hrtimer {
	struct hrtimer hrtimer;		/*! High resolution timer instance */
	uint64_t inc_interval;		/*! IncrementalResult interval timer */
	/*! Remaining time after number of Incremental Results like duration % inc_interval */
	uint64_t remaining_time;	/*! remaining time after number of Incremental Results */
	/*! Reference to shared_bits of sptest_task_common_data */
	void *shared_bits;
	struct tr143inc_result *result;	/*! Reference to IncrementalResult */
	struct net_device *net_dev;
	struct tr143_stats_collector_thread *tr143_thread;
	/*! Reference to IncrementalResult for stats collection */
	struct IncrementalResult *tr143_incresult;
};

/*! This structure provides common interface across connections
 */
struct sptest_task_common_data {
	/*! This flag is shared across multiple connections
	 *  while enabling full loading, to start interval timer etc..
	 *  Currently bits used 0-TimeBased, 1-FullLoading, 2-IncrementalResult  */
	unsigned long shared_bits;
	/*! timer for TimeBased test mode and to get IncrementalResult */
	struct tr143_hrtimer tr143hrtimer;
	/*! to store incremental results, incremental results will be obtained from
	 *  PerConnectionResult */
	struct tr143inc_result *inc_result;
};

/*! The struct to hold speedtest task configuration */
struct sptest_task_config {
	uint8_t id; /*! unique id of sptest_task */
	/*! To notify the states to interested kthreads  */
	struct completion notify_state;
	/*! supported protocol,by default it will be HTTP */
	int8_t protocol;
	/*! socket options to be specified */
	uint8_t sock_options;
	/*! kernel_recvmsg 0 - block, 1 - unblock */
	int8_t non_block;
	/*! buffer size to allocate number of pages */
	uint64_t buffer_size;
	/*! page order (i.e 2^order) to specify page size */
	int8_t page_order;
	/*! data pointer used to store or communicate http session information */
	void *data;
	/*! HTTP session user configuration */
	struct http_user_config *user_cfg;
	/*! Reference for sptest_task_common_data */
	struct sptest_task_common_data *common;
};

/*! The struct to hold speedtest kernel thread configuration
 *	'nconn' instances of this task will get updated in global task_list
 */
struct sptest_task {
	/*! unique id to identify each sptest_task
	 *	This will be having auto-incremented value as part of configuration */
    uint8_t id;
	/*! priority of http client kthread
	 *	Should be RT priority(0-99), default will be (MAX_RT_PRIO-1) */
    int priority;
	/*! CPU affinity for http client kthread */
    int cpu_affinity;
	/*! Current state of sptest_task */
    enum sptest_task_state state;
	/*! scheduling policy for http client kthread i.e SCHED_RR */
    uint8_t sched_policy;
	/*! list entry for global task_list */
    struct list_head list_node;
	/*! task_struct to hold ref of sptest task kthread */
    struct task_struct *task;
	/*! Configuration pertaining to sptest task kthread */
    struct sptest_task_config *config;
};

/*! Information type communicated from http client kthread to waiting IOCTL
 */
enum session_info_type {
	HTTP_SESSION_INFO_START_STATE,	/*! Indicates start of http_client kthread */
	HTTP_SESSION_INFO_BOMTIME,		/*! Indicates BOMTime of http_client kthread */
	HTTP_SESSION_INFO_EOMTIME,		/*! Indicates EOMTime*/
	HTTP_SESSION_INFO_RESULTS,		/*! Indicates test is finished */
	HTTP_SESSION_INFO_MAX,
};

/*! The struct used to communicate HTTP client state and statistics
 *	information to stats aggregator thread
 */
struct http_session_info {
	/*! current state of http client thread */ 
	enum sptest_task_state state;
	/*! tr143diag_error code */
	enum tr143diag_error err_code;
	/*http status code */
	int http_status_code;
	/*! stores session statistics */
	struct PerConnectionResult *perconn_result;
	/*! to store bytes /received/sent not under full loading */
	uint64_t non_full_loading_test_bytes;
	/*! to store interface bytes received/sent before/after full
	 * loading at IP layer */
	uint64_t interface_bytes_before_full_load;
	uint64_t interface_bytes_after_full_load;
	uint64_t BOMTime;	/*! To store BOMTime of current session */
	uint64_t EOMTime;	/*! To store EOMTime of current session */
};

/* structure to hold the each page information */
struct page_node {
	/*! page node for list of page */
	struct list_head list_node;
	/*! starting address of a page */
	struct page *page;
	/*! order of page for current page node */
	uint8_t porder;
	/*! data size associated with current page */
	uint32_t dsize;
};

/* function to create sptest procfs entries */
int sptest_create_procfs(struct sptest_driver *driver);

/* function to remove sptest procfs entries */
void sptest_remove_procfs(struct sptest_driver *driver);

/* function to display a list of tasks */
void sptest_display_task(struct sptest_task *sp_task);

/* function to update default config parameters of global sptest_task */
int sptest_update_task_list_by_defcfg(struct list_head *task_list,
									struct sptest_task *task, int ntasks);

/*! function to stop the sptest_task */
int sptest_stop_task(struct sptest_task *sp_task);

/* function to start the sptest_task by thread id */
int sptest_start_task_by_id(struct list_head *task_list, int thread_id);

/* function to stop the sptest_task by thread id */
int sptest_stop_task_by_id(struct list_head *task_list, int thread_id);

/*! function to start task list */
int sptest_start_task_list(struct list_head *task_list);

/*! function to stop task list */
int sptest_stop_task_list(struct list_head *task_list);

/*! function to display the task list and it's parameters */
void sptest_display_task_list(struct list_head *task_list);

/*! function to update task list by new sptest_task */
int sptest_update_task_list(struct list_head *task_list, struct sptest_task *new_task);

/*! Function to get sptest task list */
struct list_head *sptest_get_task_list(void);

/*! Function to get result list */
struct list_head *sptest_get_result_list(void);

/*! function to get a sptest_task from sptest_task id */
struct sptest_task* sptest_get_task_by_id(struct list_head *task_list, int task_id);

/*! function to clear the task list i.e removing all entries */
void sptest_clear_task_list(struct list_head *task_list);

/*! function to register charectore device file i.e /dev/tr143diagtool */
int sptest_register_chardev_ops(struct sptest_driver *driver);

/*! function to unregister charectore device file i.e /dev/tr143diagtool */
void sptest_unregister_chardev_ops(struct sptest_driver *driver);

/*! helper function to print PerConnectionResult result structure */
void print_perconn_result(struct PerConnectionResult *result);

/*! function to resolve host name to ip address */
int sptest_dns_resolve_hostname(struct http_user_config *user_cfg, const char *url);

/*! function to print sptest_task_state in string format
 */
static inline const char* sptest_task_state_to_str(enum sptest_task_state  state)
{
	switch (state) {
		case SPTEST_TASK_STATE_CREATED:
			return STR(SPTEST_TASK_STATE_CREATED);
		case SPTEST_TASK_STATE_RUNNING:
			return STR(SPTEST_TASK_STATE_RUNNING);
		case SPTEST_TASK_STATE_CLOSING:
			return STR(SPTEST_TASK_STATE_CLOSING);
		case SPTEST_TASK_STATE_CLOSED:
			return STR(SPTEST_TASK_STATE_CLOSED);
		case SPTEST_TASK_STATE_START_FAILED:
			return STR(SPTEST_TASK_STATE_START_FAILED);
		case SPTEST_TASK_STATE_RUN_FAILED:
			return STR(SPTEST_TASK_STATE_RUN_FAILED);
		case SPTEST_TASK_STATE_TEST_COMPLETED:
			return STR(SPTEST_TASK_STATE_TEST_COMPLETED);
		case SPTEST_TASK_STATE_TEST_INPROGRESS:
			return STR(SPTEST_TASK_STATE_TEST_INPROGRESS);
		default:
			return STR(INVALID);
	}
	return STR(INVALID);
}

#endif //__SPEEDTEST_DRIVER_H_
