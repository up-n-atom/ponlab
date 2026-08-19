
/*##################################################################################################
# "Copyright (c) 2019 Intel Corporation                                                            #
# DISTRIBUTABLE AS SAMPLE SOURCE SOFTWARE                                                          #
# This Distributable As Sample Source Software is subject to the terms and conditions              #
# specified in the LICENSE file                                                                    #
##################################################################################################*/

/*! \file 	uci_wrapper.c
	\brief 	This file implements a wrapper over uci library.
	\todo 	Add license header
	\todo   Add null pointer verifications and clean-up
 */

#include <stdlib.h>
#include <pthread.h>
#include <uci.h>
#include <libubox/blobmsg_json.h>
#include <libubox/uloop.h>
#include <libubus.h>
#include "spal_logging.h"
#include "uci_wrapper.h"
#ifndef ONEWIFI_UCI_TRANSITION
#define NETIFD_RESP_TIMEOUT 10000


/* list of banned SDL methods */
#define strlen(...)  	SDL_BANNED_FUNCTION ERROR_TOKEN
#define strtok(...)  	SDL_BANNED_FUNCTION ERROR_TOKEN
#define strcat(...)  	SDL_BANNED_FUNCTION ERROR_TOKEN
#define strncat(...) 	SDL_BANNED_FUNCTION ERROR_TOKEN
#define strcpy(...)  	SDL_BANNED_FUNCTION ERROR_TOKEN
#define strncpy(...) 	SDL_BANNED_FUNCTION ERROR_TOKEN
#define snprintf(...) 	SDL_BANNED_FUNCTION ERROR_TOKEN
#define sscanf(...)  	SDL_BANNED_FUNCTION ERROR_TOKEN

#endif /* ONEWIFI_UCI_TRANSITION */ 
//Logging
#ifndef LOG_LEVEL
__LOGGER_THREADVAR unsigned int LOGLEVEL = SYS_LOG_DEBUG + 1;
#else
__LOGGER_THREADVAR unsigned int LOGLEVEL = LOG_LEVEL + 1;
#endif

#ifndef LOG_TYPE
__LOGGER_THREADVAR unsigned int LOGTYPE = SYS_LOG_TYPE_CONSOLE | SYS_LOG_TYPE_FILE;
#else
__LOGGER_THREADVAR unsigned int LOGTYPE = LOG_TYPE;
#endif

__LOGGER_THREADVAR unsigned int LOGPROFILE;
//End Logging
#ifndef ONEWIFI_UCI_TRANSITION

#ifdef WAVEAPI_USE_EXTERN_LOG
waveapi_extern_log_print_t waveapi_extern_log_print_func = NULL;
waveapi_extern_ipc_client_name_t waveapi_extern_ipc_client_name_get = NULL;
#endif /* WAVEAPI_USE_EXTERN_LOG */

static bool uci_converter_if_ap(struct uci_section *s);

enum ifType
{
	IFTYPE_AP = 0,
	IFTYPE_STA,
	IFTYPE_BOTH
};

//UCI HELPER APIS

/*! \file uci_wrapper.c
    \brief SWPAL library

	The UCI (Unified Configuration Interface) subsystem that we take from openwrt is intended to centralize
	the configuration of the wireless configuration.
	The SWPAL scripts (mac80211.sh, and few other scripts taken from openwrt) shall be in charge of taking the parameters
	from the UCI DB and apply them for configuring hostapd.
	The SWPAL can be used in many platforms. For example:
	UGW - where the WLAN SL will do the translation from TR181 to UCI and then call SWPAL (scripts/uci_wrapper.c)
	RDKB - where the SWPAL will be called from the WIFI_HAL (wlan_config_server_api.c).
*/

#ifndef CONFIG_RPCD
#define USE_INDEX_CACHE
#endif

#endif /* ONEWIFI_UCI_TRANSITION */ 
static __thread struct uci_context *__ctx = NULL;
static __thread unsigned __ctx_ref_cnt = 0;

#define THEAD_LOCAL_CTX_ALLOC_LIMIT (24)

static struct uci_context* _thread_local_uci_get_context(void)
{
alloc:
	if (!__ctx)
		__ctx = uci_alloc_context();

	if (!__ctx)
		return NULL;

	if (++__ctx_ref_cnt >= THEAD_LOCAL_CTX_ALLOC_LIMIT) {
		ERROR("__ctx_ref_cnt reached %d ==> ctx mem leak detected\n", __ctx_ref_cnt);
		uci_free_context(__ctx);
		__ctx_ref_cnt = 0;
		__ctx = NULL;
		goto alloc;
	}

	return __ctx;
}

static void _thread_local_uci_put_context(struct uci_context* ctx)
{
	if (!__ctx_ref_cnt) {
		ERROR("__ctx_ref_cnt is equal to 0, __ctx=%p\n", __ctx);
		return;
	}

	if (ctx != __ctx) {
		ERROR("ctx (%p) != __ctx (%p)\n", ctx, __ctx);
		return;
	}

	__ctx_ref_cnt--;
	if (!__ctx_ref_cnt) {
		uci_free_context(__ctx);
		__ctx = NULL;
	}
}
#ifndef ONEWIFI_UCI_TRANSITION

int uci_converter_alloc_local_uci_context(void)
{
	if (!_thread_local_uci_get_context())
		return RETURN_ERR;

	return RETURN_OK;
}

void uci_converter_free_local_uci_context(void)
{
	_thread_local_uci_put_context(__ctx);
}

#ifndef UNUSED_PARAM
#define UNUSED_PARAM(param) (void)param
#endif

#ifdef __KLOCWORK__
/* Implementation of UCI macros ONLY for KLOCWORK analysis.
   KW generates many false positives when using UCI API macros
   due to unknown issues with parsing UCI headers. */

#undef uci_foreach_element
#undef uci_foreach_element_safe

#define uci_foreach_element(_list, _ptr)		\
	for(_ptr = list_to_element((_list)->next);	\
		&_ptr->list != (_list);			\
		_ptr = list_to_element(_ptr->list.next))

#define uci_foreach_element_safe(_list, _tmp, _ptr)		\
	for(_ptr = list_to_element((_list)->next),		\
		_tmp = list_to_element(_ptr->list.next);	\
		&_ptr->list != (_list);			\
		_ptr = _tmp, _tmp = list_to_element(_ptr->list.next))

#endif


static inline const char* get_type_str(enum paramType type) {
	return (type == TYPE_RADIO) ? "radio" : "default_radio";
}

#endif /* ONEWIFI_UCI_TRANSITION */ 
/* Internal macro which has to be called instead of direct call of uci_lookup_ptr().
 * This macro makes a local copy of "path" and prevents an external string buffer
 * from the modifications by uci_lookup_ptr()
 * Used mostly if "path" is passed as an input parameter of the API function. */
#define UCI_CONVERTER_LOOKUP_PTR(ctx,ptr,buffer,path,extended) ({ \
		int ret; \
		if (strcpy_s(buffer, sizeof(buffer), path)) { \
			ERROR("path is too long!\n"); \
			ret = UCI_ERR_NOTFOUND; \
		} else \
			ret = uci_lookup_ptr(ctx, ptr, buffer, extended); \
		ret; \
	})
#ifndef ONEWIFI_UCI_TRANSITION

#ifdef CONFIG_RPCD

static int _call_ubus_uci(struct blob_buf *blob, const char *operation)
{
	unsigned uci_id = (unsigned)-1;
	int ubus_ret;
	int ret = RETURN_ERR;
	struct ubus_context *ubus_ctx = NULL;
	const char *ubus_socket = NULL;

	ubus_ctx = ubus_connect(ubus_socket);
	if (!ubus_ctx) {
		CRIT("ubus_connect failed.\n");
		return RETURN_ERR;
	}

	if (ubus_lookup_id(ubus_ctx, "uci", &uci_id)) {
		CRIT("Requested server 'uci' not available \n");
		goto end;
	}

	/* DEBUG("uci id = %u\n", uci_id); */
	ubus_ret = ubus_invoke(ubus_ctx, uci_id, operation, blob->head, NULL, NULL, 30 * 1000);
	if(ubus_ret != 0) {
		ERROR("Ubus request to 'uci %s' failed (%d: %s)\n", operation, ubus_ret, ubus_strerror(ubus_ret));
		goto end;
	}
	DEBUG("Ubus request to 'uci %s': success\n", operation);
	ret = RETURN_OK;

end:
	ubus_free(ubus_ctx);
	return ret;
}

/* Write the parameter to the UCI database using UBUS service */
static int _ubus_uci_converter_set(const char* path, const char* option, const char* value)
{
	int ret;
	char buf_path[MAX_UCI_BUF_LEN] = "";
	char *config, *section, *start;
	struct blob_buf blob;
	void *tbl;

	if (is_empty_str(path) || is_empty_str(option) || is_empty_str(value)) {
		ERROR("At least one of received parameters is empty\n");
		return RETURN_ERR;
	}

	/* DEBUG("uci set operation called for path:'%s', option:'%s', value:'%s'\n", path, option, value); */
	if (strcpy_s(buf_path, sizeof(buf_path), path)) {
		ERROR("path is too long\n");
		return RETURN_ERR;
	}
	start = buf_path;

	/* Parse input path */
	config = start; strsep(&start, ".");
	if (!start) {
		ERROR("Can't find delimeter '.' in string '%s'\n", buf_path);
		return RETURN_ERR;
	}
	section = start;
	/* DEBUG("config:%s, section:%s\n", config, section); */

	memset(&blob, 0, sizeof(blob));
	blobmsg_buf_init(&blob);
	blobmsg_add_string(&blob, "config", config);
	blobmsg_add_string(&blob, "section", section);
	tbl = blobmsg_open_table(&blob, "values");
	blobmsg_add_string(&blob, option, value);
	blobmsg_close_table(&blob, tbl);

	ret = _call_ubus_uci(&blob, "set");
	if (blob.head != NULL)
		blob_buf_free(&blob);
	return ret;
}

/* Delete the parameter from the UCI database using UBUS */
static int _ubus_uci_converter_del(const char* path)
{
	int ret;
	char buf_path[MAX_UCI_BUF_LEN] = "";
	char *config = NULL, *section = NULL, *option = NULL, *start;
	struct blob_buf blob;

	if (is_empty_str(path)) {
		ERROR("Received parameters is empty\n");
		return RETURN_ERR;
	}

	/* DEBUG("uci del operation called for path:'%s'\n", path); */
	if (strcpy_s(buf_path, sizeof(buf_path), path)) {
		ERROR("Path is too long\n");
		return RETURN_ERR;
	}
	start = buf_path;

	/* Parse input path */
	config = start;
	strsep(&start, "."); if (start) section = start;
	strsep(&start, "."); if (start) option = start;

	/* DEBUG("config:%s, section:%s, option:%s\n", config, section, option); */

	memset(&blob, 0, sizeof(blob));
	blobmsg_buf_init(&blob);
	blobmsg_add_string(&blob, "config", config);
	if (section) blobmsg_add_string(&blob, "section", section);
	if (option) blobmsg_add_string(&blob, "option", option);

	ret = _call_ubus_uci(&blob, "delete");
	if (blob.head != NULL)
		blob_buf_free(&blob);
	return ret;
}

static int _ubus_uci_converter_commit(const char* config)
{
	int ret;
	struct blob_buf blob;

	if (is_empty_str(config)) {
		ERROR("Received parameters is empty\n");
		return RETURN_ERR;
	}

	memset(&blob, 0, sizeof(blob));
	blobmsg_buf_init(&blob);
	blobmsg_add_string(&blob, "config", config);

	ret = _call_ubus_uci(&blob, "commit");
	if (blob.head != NULL)
		blob_buf_free(&blob);

	return ret;
}
#endif /* CONFIG_RPCD */

/* Size of the lookup table. Can be increased if needed */
#define INDEX_LOOKUP_TABLE_SIZE 64
#define INDEX_EMPTY_ENTRY       (-1)

#ifdef USE_INDEX_CACHE

struct index_lookup_entry {
	short uci_index, rpc_index;
};

struct index_lookup_table {
	unsigned count;
	struct index_lookup_entry table[INDEX_LOOKUP_TABLE_SIZE];
};

static struct index_lookup_table g_uci_to_rpc_indexes[TYPE_TOTAL] = {
	{0, { [0 ... (INDEX_LOOKUP_TABLE_SIZE)-1] = { INDEX_EMPTY_ENTRY, INDEX_EMPTY_ENTRY }} },
	{0, { [0 ... (INDEX_LOOKUP_TABLE_SIZE)-1] = { INDEX_EMPTY_ENTRY, INDEX_EMPTY_ENTRY }} },
	{0, { [0 ... (INDEX_LOOKUP_TABLE_SIZE)-1] = { INDEX_EMPTY_ENTRY, INDEX_EMPTY_ENTRY }} },
};

static short g_rpc_to_uci_indexes[TYPE_TOTAL][INDEX_LOOKUP_TABLE_SIZE] = {
	{ [0 ... (INDEX_LOOKUP_TABLE_SIZE)-1] = INDEX_EMPTY_ENTRY },
	{ [0 ... (INDEX_LOOKUP_TABLE_SIZE)-1] = INDEX_EMPTY_ENTRY },
	{ [0 ... (INDEX_LOOKUP_TABLE_SIZE)-1] = INDEX_EMPTY_ENTRY }
};

static pthread_mutex_t g_index_mutex[TYPE_TOTAL] = {
	PTHREAD_MUTEX_INITIALIZER,
	PTHREAD_MUTEX_INITIALIZER,
	PTHREAD_MUTEX_INITIALIZER,
};

#define INDEX_MUTEX_LOCK(iftype)   do {if (pthread_mutex_lock(&g_index_mutex[iftype])) {}; } while(0)
#define INDEX_MUTEX_UNLOCK(iftype) do {if (pthread_mutex_unlock(&g_index_mutex[iftype])) {}; } while(0)


/* The maximum RPC index value is INDEX_LOOKUP_TABLE_SIZE. A simple lookup table is used
	to optimize the RPC -> UCI conversion, but we have a limit on the size of the table.
	If this doesn't meet the new requirements, either increase INDEX_LOOKUP_TABLE_SIZE
	or develop a lookup function similar to _find_rpc_index().
*/
static inline bool _valid_rpc_index(int rpc_index)
{
	return (rpc_index >= 0 && rpc_index < INDEX_LOOKUP_TABLE_SIZE);
}

static inline bool _valid_uci_index(int uci_index)
{
	return (uci_index >=0 && uci_index <= 32767); // max short value
}

static int _find_rpc_index(enum paramType iftype, int uci_index)
{
	unsigned i;
	struct index_lookup_table* table = &g_uci_to_rpc_indexes[iftype];

	for (i = 0; i < table->count; ++i) {
		if (table->table[i].uci_index == uci_index)
			return table->table[i].rpc_index;
	}
	return INDEX_EMPTY_ENTRY;
}

static inline bool _update_rpc_index(enum paramType iftype, int uci_index, int rpc_index)
{
	unsigned i;
	struct index_lookup_table* table = &g_uci_to_rpc_indexes[iftype];

	if (!_valid_rpc_index(rpc_index)) {
		ERROR("unsupported RPC index (%d)\n", rpc_index);
		return false;
	}

	/* search in table */
	for (i = 0; i < table->count; ++i) {
		if (table->table[i].uci_index == uci_index) {
			table->table[i].rpc_index = rpc_index;
			return true;
		}
	}

	/* create a new entry */
	if (table->count >= INDEX_LOOKUP_TABLE_SIZE) {
		ERROR("lookup table g_uci_to_rpc_indexes[] is full\n");
		return false;
	}

	table->table[table->count].uci_index = uci_index;
	table->table[table->count].rpc_index = rpc_index;
	table->count++;
	return true;
}

static inline int _find_uci_index(enum paramType iftype, int rpc_index)
{
	return g_rpc_to_uci_indexes[iftype][rpc_index];
}

static inline bool _update_uci_index(enum paramType iftype, int rpc_index, int uci_index)
{
	if (!_valid_uci_index(uci_index)) {
		ERROR("unsupported UCI index (%d)\n", uci_index);
		return false;
	}
	g_rpc_to_uci_indexes[iftype][rpc_index] = uci_index;
	return true;
}

#else /* USE_INDEX_CACHE */
/* Don't use index cache */
#define INDEX_MUTEX_LOCK(iftype)
#define INDEX_MUTEX_UNLOCK(iftype)

static inline bool _valid_rpc_index(int rpc_index)
{
	return (rpc_index >= 0);
}

static inline bool _valid_uci_index(int uci_index)
{
	return (uci_index >= 0);
}

static inline int _find_rpc_index(enum paramType iftype, int uci_index)
{
	(void)iftype;
	(void)uci_index;
	return INDEX_EMPTY_ENTRY;
}

static inline bool _update_rpc_index(enum paramType iftype, int uci_index, int rpc_index)
{
	(void)iftype;
	(void)uci_index;
	(void)rpc_index;
#ifdef __KLOCWORK__
	/* to supress KW message */
	return _valid_rpc_index(rpc_index);
#else
	return true;
#endif
}

static inline int _find_uci_index(enum paramType iftype, int rpc_index)
{
	(void)iftype;
	(void)rpc_index;
	return INDEX_EMPTY_ENTRY;
}

static inline bool _update_uci_index(enum paramType iftype, int rpc_index, int uci_index)
{
	(void)iftype;
	(void)rpc_index;
	(void)uci_index;
#ifdef __KLOCWORK__
	/* to supress KW message */
	return _valid_uci_index(uci_index);
#else
	return true;
#endif
}

#endif /* USE_INDEX_CACHE */


static inline bool _valid_iftype(enum paramType iftype)
{
	return (iftype >= TYPE_RADIO && iftype <= TYPE_VAP);
}

/**************************************************************************/
/*! \fn void uci_invalidate_index_cache(enum paramType iftype, int rpc_index)
 **************************************************************************
 *  \brief Invalidate index cache for the given rpc_index.
 *         Must be called after VAP(s) deleting.
 *  \param[in] enum paramType ifType: TYPE_RADIO/TYPE_RADIO_VAP/TYPE_VAP
 *  \param[in] int rpc_index - The rpc index
 *  \return void
 ***************************************************************************/
void uci_invalidate_index_cache(enum paramType iftype, int rpc_index)
{
#ifndef USE_INDEX_CACHE
	(void)iftype;
	(void)rpc_index;
#else
	if (!_valid_iftype(iftype))
		return;

	if (!_valid_rpc_index(rpc_index))
		return;

	/* Reset UCI indexes cache */
	g_rpc_to_uci_indexes[iftype][rpc_index] = INDEX_EMPTY_ENTRY;

	/* Reset RPC indexes cache completely */
	{
		unsigned i;
		struct index_lookup_table* table = &g_uci_to_rpc_indexes[iftype];
		for (i = 0; i < table->count; ++i) {
			table->table[i].rpc_index = INDEX_EMPTY_ENTRY;
			table->table[i].uci_index = INDEX_EMPTY_ENTRY;
		}
		table->count = 0;
	}
#endif
}

/**************************************************************************/
/*! \fn int rpc_to_uci_index(enum paramType iftype, int rpc_index)
 **************************************************************************
 *  \brief convert rpc index to uci index using uci-db
 *  \param[in] enum paramType ifType: TYPE_RADIO/TYPE_RADIO_VAP/TYPE_VAP
 *  \param[in] int rpc_index - The rpc index
 *  \return the uci index if success, negative if error
 ***************************************************************************/
int rpc_to_uci_index(enum paramType iftype, int rpc_index)
{
	static const char* index_path_pre[TYPE_TOTAL] = {
	/* TYPE_RADIO     */ "wireless.radio_rpc_indexes",
	/* TYPE_RADIO_VAP */ "wireless.radio_vap_rpc_indexes",
	/* TYPE_VAP       */ "wireless.vap_rpc_indexes"
	};

	char index_path[MAX_UCI_BUF_LEN];
	char value_str[MAX_UCI_BUF_LEN];
	short uci_index;

	if (!_valid_iftype(iftype)) {
		ERROR("invalid iftype (%d)\n", iftype);
		return UCI_INVALID_INDEX;
	}

	if (!_valid_rpc_index(rpc_index)) {
		ERROR("unsupported RPC index (%d)\n", rpc_index);
		return UCI_INVALID_INDEX;
	}

	INDEX_MUTEX_LOCK(iftype);

	uci_index = _find_uci_index(iftype, rpc_index);
	if (INDEX_EMPTY_ENTRY == uci_index ) {
		// DEBUG("empty RPC(%d) index, load...\n", rpc_index);
		int status = sprintf_s(index_path, sizeof(index_path), "%s.index%d", index_path_pre[iftype], rpc_index);
		if (status <= 0) {
			ERROR("failed sprintf_s. rpc_index=%d\n", rpc_index);
			goto failure;
		}

		if (RETURN_OK != uci_converter_get(index_path, value_str, sizeof(value_str)))
			goto failure;

		uci_index = atoi(value_str);
		// DEBUG("update RPC(%d) --> UCI(%d), iftype %d...\n", rpc_index, uci_index, iftype);
		if (!_update_uci_index(iftype, rpc_index, uci_index)) {
			// ERROR("UCI index update failed\n");
			goto failure;
		}
	}
	// else {
	// 	DEBUG("index loaded from cache, RPC(%d) --> UCI(%d), iftype %d...\n", rpc_index, uci_index, iftype);
	// }
	INDEX_MUTEX_UNLOCK(iftype);
	return uci_index;

failure:
	INDEX_MUTEX_UNLOCK(iftype);
	return UCI_INVALID_INDEX;
}

/**************************************************************************/
/*! \fn int uci_to_rpc_index(enum paramType iftype, int index)
 **************************************************************************
 *  \brief convert uci index to rpc index using uci-db
 *  \param[in] enum paramType ifType: TYPE_RADIO/TYPE_RADIO_VAP/TYPE_VAP
 *  \param[in] int index - The uci index
 *  \return the index if success, negative if error
 ***************************************************************************/
int uci_to_rpc_index(enum paramType iftype, int uci_index)
{
	static const char* index_path_pre[TYPE_TOTAL] = {
	/* TYPE_RADIO     */ "wireless.radio",
	/* TYPE_RADIO_VAP */ "wireless.default_radio",
	/* TYPE_VAP       */ "wireless.default_radio"
	};

	char index_path[MAX_UCI_BUF_LEN];
	char value_str[MAX_UCI_BUF_LEN];
	int rpc_index;

	if (!_valid_iftype(iftype)) {
		ERROR("invalid iftype (%d)\n", iftype);
		return UCI_INVALID_INDEX;
	}

	if (!_valid_uci_index(uci_index)) {
		ERROR("unsupported UCI index (%d)\n", uci_index);
		return UCI_INVALID_INDEX;
	}

	INDEX_MUTEX_LOCK(iftype);

	rpc_index = _find_rpc_index(iftype, uci_index);
	if (INDEX_EMPTY_ENTRY == rpc_index ) {
		// DEBUG("empty UCI(%d) index, load...\n", uci_index);
		int status = sprintf_s(index_path, MAX_UCI_BUF_LEN, "%s%d.rpc_index", index_path_pre[iftype], uci_index);
		if (status <= 0) {
			ERROR("failed sprintf_s(). uci_index=%d\n", uci_index);
			goto failure;
		}

		if (RETURN_OK != uci_converter_get(index_path, value_str, MAX_UCI_BUF_LEN))
			goto failure;

		rpc_index = atoi(value_str);
		// DEBUG("update UCI(%d) --> RPC(%d), iftype %d...\n", uci_index, rpc_index, iftype);
		if (!_update_rpc_index(iftype, uci_index, rpc_index)) {
			// ERROR("RPC index update failed\n");
			goto failure;
		}
	}
	// else {
	// 	DEBUG("index loaded from cache, UCI(%d) --> RPC(%d), iftype %d...\n", uci_index, rpc_index, iftype);
	// }
	INDEX_MUTEX_UNLOCK(iftype);
	return rpc_index;

failure:
	INDEX_MUTEX_UNLOCK(iftype);
	return UCI_INVALID_INDEX;
}

/**************************************************************************/
/*! \fn int dummy_to_radio_index(enum paramType iftype, int index)
 **************************************************************************
 *  \brief convert dummy vap uci index to radio uci index using uci-db
 *  \param[in] int index - The uci index of the dummy vap
 *  \return the index if success, negative if error
 ***************************************************************************/
static int dummy_to_radio_index(int uci_index)
{
	int radio_idx = 0;
	char radio[MAX_UCI_BUF_LEN] = "";

	int rpc_index = uci_to_rpc_index(TYPE_RADIO_VAP, uci_index);
	if (UCI_INVALID_INDEX == rpc_index) {
		// ERROR("failed getting RPC index from UCI index (%d)\n", uci_index);
		return RETURN_ERR;
	}

	if (uci_converter_get_str_ext(TYPE_RADIO_VAP, rpc_index, "device", radio, sizeof(radio)) == RETURN_ERR)
		return RETURN_ERR;

	if (sscanf_s(radio, "radio%d", &radio_idx) != 1) {
		return RETURN_ERR;
	}

	return radio_idx;
}

static int uci_get_existing_interfaces_imp(enum ifType if_type, const char *radioName,
		int *ifacesArr, const unsigned int ifacesArrSize, unsigned int *numOfIfFound)
{
	int itemsRead = 0;
	int index_read = 0;
	struct uci_ptr ptr;
	struct uci_context *ctx;
	struct uci_package *p = NULL;
	struct uci_element *e;
	struct uci_section *s;
	char lookup_str[MAX_UCI_BUF_LEN] = "wireless";

	if ((ifacesArr == NULL) || (numOfIfFound == NULL)) {
		ERROR("ifacesArr or numOfIfFound is NULL!\n");
		return RETURN_ERR;
	}

	*numOfIfFound = 0;

	ctx = _thread_local_uci_get_context();
	if (!ctx)
		return RETURN_ERR;

	if ((uci_lookup_ptr(ctx, &ptr, lookup_str, true) != UCI_OK)) {
		ERROR("uci lookup failed!\n");
		_thread_local_uci_put_context(ctx);
		return RETURN_ERR;
	}

	p = ptr.p;

	uci_foreach_element(&p->sections, e) {
		struct uci_element *n = 0;

		s = uci_to_section(e);
		if (strncmp(s->type, "wifi-iface", MAX_UCI_BUF_LEN))
			continue;

		switch (if_type) {
			case IFTYPE_AP:
				if (!uci_converter_if_ap(s)) continue;
				break;
			case IFTYPE_STA:
				if (uci_converter_if_ap(s)) continue;
				break;
			default:
				break;
		}

		uci_foreach_element(&s->options, n) {

			if (strncmp(n->name, "device", MAX_UCI_BUF_LEN))
				continue;

			if (radioName && strncmp(uci_to_option(n)->v.string, radioName, MAX_UCI_BUF_LEN))
				continue;/*the vap does not belong to the requested radio*/

			itemsRead = sscanf_s(s->e.name, "default_radio%d", &index_read);
			if (uci_converter_is_dummy(index_read))
				continue;

			if (itemsRead == 1) {
				if(ifacesArrSize <= *numOfIfFound) {
					_thread_local_uci_put_context(ctx);
					return RETURN_ERR;
				}

				ifacesArr[*numOfIfFound] = index_read;
				(*numOfIfFound)++;
			}
		}
	}

	_thread_local_uci_put_context(ctx);
	return RETURN_OK;
}

/**************************************************************************/
/*! \fn int uci_get_existing_interfaces(int *ifacesArr, const unsigned int ifacesArrSize)
 **************************************************************************
 *  \brief return an array containing the indexes of all AP interfaces
 *  \param[out] int *ifacesArr - buffer to contain the ouput array
 *  \param[in] const unsigned int ifacesArrSize - max size of ifacesArr
 *	\param[out] int *numOfIfFound - the number of interfaces found
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int uci_get_existing_interfaces(int *ifacesArr, const unsigned int ifacesArrSize, unsigned int *numOfIfFound)
{
	return uci_get_existing_interfaces_imp(IFTYPE_AP, NULL, ifacesArr, ifacesArrSize, numOfIfFound);
}

/**************************************************************************/
/*! \fn int uci_get_existing_interfaces_in_radio(int *ifacesArr, const unsigned int ifacesArrSize)
 **************************************************************************
 *  \brief return an array containing the indexes of all AP interfaces in the current radio
 *  \param[in] const char *radio - radio name
 *  \param[out] int *ifacesArr - buffer to contain the ouput array
 *  \param[in] const unsigned int ifacesArrSize - max size of ifacesArr
 *	\param[out] int *numOfIfFound - the number of interfaces found
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int uci_get_existing_interfaces_in_radio(const char *radio, int *ifacesArr, const unsigned int ifacesArrSize, unsigned int *numOfIfFound)
{
	return uci_get_existing_interfaces_imp(IFTYPE_AP, radio, ifacesArr, ifacesArrSize, numOfIfFound);
}

/**************************************************************************/
/*! \fn int uci_get_existing_sta_interfaces(int *ifacesArr, const unsigned int ifacesArrSize)
 **************************************************************************
 *  \brief return an array containing the indexes of all STA interfaces
 *  \param[out] int *ifacesArr - buffer to contain the ouput array
 *  \param[in] const unsigned int ifacesArrSize - max size of ifacesArr
 *	\param[out] int *numOfIfFound - the number of interfaces found
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int uci_get_existing_sta_interfaces(int *ifacesArr, const unsigned int ifacesArrSize, unsigned int *numOfIfFound)
{
	return uci_get_existing_interfaces_imp(IFTYPE_STA, NULL, ifacesArr, ifacesArrSize, numOfIfFound);
}

/**************************************************************************/
/*! \fn int uci_get_existing_sta_interfaces_in_radio(int *ifacesArr, const unsigned int ifacesArrSize)
 **************************************************************************
 *  \brief return an array containing the indexes of all STA interfaces in the current radio
 *  \param[in] const char *radio - radio name
 *  \param[out] int *ifacesArr - buffer to contain the ouput array
 *  \param[in] const unsigned int ifacesArrSize - max size of ifacesArr
 *	\param[out] int *numOfIfFound - the number of interfaces found
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int uci_get_existing_sta_interfaces_in_radio(const char *radio, int *ifacesArr, const unsigned int ifacesArrSize, unsigned int *numOfIfFound)
{
	return uci_get_existing_interfaces_imp(IFTYPE_STA, radio, ifacesArr, ifacesArrSize, numOfIfFound);
}

/**************************************************************************/
/*! \fn int uci_get_param_change(enum paramType ifType, int index, const char *paramName, char* paramChange)
 **************************************************************************
 *  \brief get the new value of the parameter as it will be pushed in the new uci commit
 *  \param[in] enum paramType ifType: TYPE_RADIO/TYPE_VAP
 *  \param[in] int index - The index of the entry
 *  \param[in] const char *paramName - parameter name to get the value of
 *  \param[out] char* paramChange - the new parameter value if a new value is found
 *  \param[in] size_t size - size of the output string buffer
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int uci_get_param_change(enum paramType ifType, int index, const char *paramName, char* paramChange, size_t size)
{
	struct uci_ptr ptr = {0};
	struct uci_element *e = NULL;
	struct uci_context *ctx = NULL;
	int status, uci_index;
	char path[MAX_UCI_BUF_LEN] = "";

	if ((paramName == NULL) || (paramChange == NULL)) {
		ERROR("paramName or paramChange is NULL!\n");
		return RETURN_ERR;
	}

	if (ifType != TYPE_VAP && ifType != TYPE_RADIO) {
		ERROR("ifType %d is not supported!\n", ifType);
		return RETURN_ERR;
	}

	uci_index = rpc_to_uci_index(ifType, index);
	if (UCI_INVALID_INDEX == uci_index) {
		// ERROR("failed getting UCI index from RPC index (%d)\n", index);
		return RETURN_ERR;
	}

	status = sprintf_s(path, sizeof(path), "wireless.%s%d", get_type_str(ifType), uci_index);
	if (status <= 0) {
		ERROR("failed sprintf_s(). index=%d\n", index);
		return RETURN_ERR;
	}

	ctx = _thread_local_uci_get_context();
	if (!ctx)
		return RETURN_ERR;

	if ((uci_lookup_ptr(ctx, &ptr, path, true) != UCI_OK)) {
		ERROR("uci lookup failed!\n");
		_thread_local_uci_put_context(ctx);
		return RETURN_ERR;
	}

	if (ptr.p == NULL) {
		ERROR("uci lookup returned a NULL ptr in ptr.p!\n");
		_thread_local_uci_put_context(ctx);
		return RETURN_ERR;
	}

	status = sprintf_s(path, sizeof(path), "%s%d", get_type_str(ifType), uci_index);
	if (status <= 0) {
		ERROR("failed sprintf_s(). index=%d\n", index);
		_thread_local_uci_put_context(ctx);
		return RETURN_ERR;
	}

	uci_foreach_element(&ptr.p->saved_delta, e) {
		struct uci_delta *h = uci_to_delta(e);
		if (h && !strncmp(e->name, paramName, MAX_UCI_BUF_LEN)) {
			if (!strncmp(h->section, path, MAX_UCI_BUF_LEN)) {
				status = strcpy_s(paramChange, size, h->value);
				_thread_local_uci_put_context(ctx);
				if (status) {
					ERROR("output buffer is too small!\n");
					return RETURN_ERR;
				}
				return RETURN_OK;
			}
		}
	}

	_thread_local_uci_put_context(ctx);
	return RETURN_OK;
}

static bool uci_converter_if_ap(struct uci_section *s)
{
	bool res = false;
	struct uci_element *n;

	uci_foreach_element(&s->options, n) {
		struct uci_option *o = uci_to_option(n);

		if (strncmp(n->name, "mode", MAX_UCI_BUF_LEN))
			continue;

		if (o->type != UCI_TYPE_STRING)
			continue;

		if (!strncmp(o->v.string, "ap", MAX_UCI_BUF_LEN))
			res = true;

		break;
	}

	return res;
}

static bool uci_converter_is_sta(int uci_index)
{
	int status;
	char path[MAX_UCI_BUF_LEN] = "";
	char uci_ret[MAX_UCI_BUF_LEN] = "";

	status = sprintf_s(path, MAX_UCI_BUF_LEN, "wireless.default_radio%d.mode", uci_index);
	if (status <= 0) {
		ERROR("failed sprintf_s.  index=%d\n", uci_index);
		return false;
	}

	status = uci_converter_get(path, uci_ret, MAX_UCI_BUF_LEN);
	if ((status == RETURN_OK) && !strncmp(uci_ret, "sta", MAX_UCI_BUF_LEN))
		return true;
	return false;
}

static int sta_to_radio_index(int index)
{
	int radio_idx = 0;
	char radio[MAX_UCI_BUF_LEN] = "";
	if (uci_converter_get_str_by_uci_index(TYPE_VAP, index, "device", radio, sizeof(radio)) == RETURN_ERR)
		return RETURN_ERR;

	if (sscanf_s(radio, "radio%d", &radio_idx) != 1) {
		return RETURN_ERR;
	}

	return radio_idx;
}

/**************************************************************************/
/*! \fn int uci_converter_is_dummy(int uci_index)
 **************************************************************************
 *  \brief get wheter vap is dummy or not based on uci index
 *  \param[in] int uci_index - The index of the vap entry
 *  \return true if dummy, false if not dummy / error
 ***************************************************************************/
bool uci_converter_is_dummy(int uci_index)
{
	int status;
	char path[MAX_UCI_BUF_LEN] = "";
	char uci_ret[MAX_UCI_BUF_LEN] = "";

	int rpc_index = uci_to_rpc_index(TYPE_RADIO_VAP, uci_index);
	if (UCI_INVALID_INDEX == rpc_index) {
		// ERROR("failed getting RPC index from UCI index (%d)\n", uci_index);
		return RETURN_ERR;
	}

	status = sprintf_s(path, MAX_UCI_BUF_LEN, "wireless.radio_vap_rpc_indexes.index%d", rpc_index);
	if (status <= 0) {
		ERROR("failed sprintf_s(). rpc_index=%d\n", rpc_index);
		return false;
	}

	status = uci_converter_get(path, uci_ret, MAX_UCI_BUF_LEN);
	if ((status == RETURN_OK) && (atoi(uci_ret) == uci_index))
		return true;
	return false;
}

static int uci_converter_count_elems_by_type(enum ifType if_type, const char *sec_type, const char *sec_name,
		const char* opt_name, const char* opt_val, int *num)
{
	int wireless_idx = -1;
	struct uci_ptr ptr;
	struct uci_context *ctx;
	struct uci_package *p = NULL;
	struct uci_element *e;
	struct uci_section *s;
	char lookup_str[MAX_UCI_BUF_LEN] = "wireless";

	ctx = _thread_local_uci_get_context();
	if (!ctx)
		return RETURN_ERR;

	if ((uci_lookup_ptr(ctx, &ptr, lookup_str, true) != UCI_OK)) {
		ERROR("uci lookup failed!\n");
		_thread_local_uci_put_context(ctx);
		return RETURN_ERR;
	}

	p = ptr.p;
	*num = 0;

	uci_foreach_element(&p->sections, e) {
		struct uci_element *n;

		s = uci_to_section(e);

		if (strncmp(s->type, sec_type, MAX_UCI_BUF_LEN))
			continue;


		if (sec_name && strncmp(s->e.name, sec_name, MAX_UCI_BUF_LEN))
			continue;

		if (sscanf_s(s->e.name, "default_radio%d", &wireless_idx) == 1) {
			if (uci_converter_is_dummy(wireless_idx))
				continue;
		}

		if (!strncmp(sec_type, "wifi-iface", MAX_UCI_BUF_LEN)) {
			switch (if_type) {
				case IFTYPE_AP:
					if (!uci_converter_if_ap(s)) continue;
					break;
				case IFTYPE_STA:
					if (uci_converter_if_ap(s)) continue;
					break;
				default:
					break;
			}
		}

		if (opt_name){
			uci_foreach_element(&s->options, n) {
				struct uci_option *o = uci_to_option(n);

				if (strncmp(n->name, opt_name, MAX_UCI_BUF_LEN))
					continue;

				if (o->type != UCI_TYPE_STRING)
					continue;

				if (opt_val && strncmp(opt_val, o->v.string, MAX_UCI_BUF_LEN))
					continue;
				(*num)++;
			}
		} else {
			(*num)++;
		}
	}

	_thread_local_uci_put_context(ctx);

	return RETURN_OK;
}

/**************************************************************************/
/*! \fn int uci_converter_count_elems(const char *sec_type, const char *sec_name, const char* opt_name, const char* opt_val,int *num)
 **************************************************************************
 *  \brief count elements in UCI DB (for iface count ap only)
 *  \param[in] const char *sec_type - section type, mandatory
 *  \param[in] const char *sec_name - section name, optional, used for extra filtering
 *  \param[in] const char* opt_name - option name, optional, used for extra filtering
 *  \param[in] const char* opt_val - option value, optional, used for extra filtering
 *  \param[out] int *num - return the element count
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int uci_converter_count_elems(const char *sec_type, const char *sec_name, const char* opt_name, const char* opt_val, int *num)
{
	return uci_converter_count_elems_by_type(IFTYPE_AP, sec_type, sec_name, opt_name, opt_val, num);
}

/**************************************************************************/
/*! \fn int uci_converter_count_sta_elems(const char *sec_type, const char *sec_name, const char* opt_name, const char* opt_val,int *num)
 **************************************************************************
 *  \brief count elements in UCI DB (for iface count stations only)
 *  \param[in] const char *sec_type - section type, mandatory
 *  \param[in] const char *sec_name - section name, optional, used for extra filtering
 *  \param[in] const char* opt_name - option name, optional, used for extra filtering
 *  \param[in] const char* opt_val - option value, optional, used for extra filtering
 *  \param[out] int *num - return the element count
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int uci_converter_count_sta_elems(const char *sec_type, const char *sec_name, const char* opt_name, const char* opt_val, int *num)
{
	return uci_converter_count_elems_by_type(IFTYPE_STA, sec_type, sec_name, opt_name, opt_val, num);
}

/**************************************************************************/
/*! \fn int uci_getIndexFromInterface(const char *interfaceName, int *rpc_index)
 **************************************************************************
 *  \brief get UCI index from the interface name
 *  \param[in] const char *interfaceName - interface name string
 *  \param[out] int *rpc_index - output the UCI index
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int uci_getIndexFromInterface(const char *interfaceName, int *rpc_index)
{
	struct uci_ptr ptr;
	struct uci_context *ctx;
	struct uci_package *p = NULL;
	struct uci_element *e;
	struct uci_section *s;
	char lookup_str[MAX_UCI_BUF_LEN] = "wireless";

	if ((interfaceName == NULL) || (rpc_index == NULL)) {
		ERROR("interfaceName or rpc_index is NULL!\n");
		return RETURN_ERR;
	}

	ctx = _thread_local_uci_get_context();
	if (!ctx)
		return RETURN_ERR;

	if ((uci_lookup_ptr(ctx, &ptr, lookup_str, true) != UCI_OK)) {
		ERROR("uci lookup failed!\n");
		_thread_local_uci_put_context(ctx);
		return RETURN_ERR;
	}

	p = ptr.p;

	uci_foreach_element(&p->sections, e) {
		struct uci_element *n;
		int scanf_res;

		s = uci_to_section(e);

		if (strncmp(s->type, "wifi-iface", MAX_UCI_BUF_LEN))
			continue;

		uci_foreach_element(&s->options, n) {
			struct uci_option *o = uci_to_option(n);

			if (strncmp(n->name, "ifname", MAX_UCI_BUF_LEN))
				continue;

			if (o->type != UCI_TYPE_STRING)
				continue;

			if (strncmp(interfaceName, o->v.string, MAX_UCI_BUF_LEN))
				continue;

			scanf_res = sscanf_s(s->e.name, "default_radio%d", rpc_index);

			if (scanf_res < 1 || *rpc_index < 0) {
				*rpc_index = -1;
				_thread_local_uci_put_context(ctx);
				return RETURN_ERR;
			}
			/* if it is dummy we want the radio index, not the dummy one */
			if (uci_converter_is_dummy(*rpc_index)) {
				*rpc_index = dummy_to_radio_index(*rpc_index);
				if (*rpc_index == RETURN_ERR) {
					_thread_local_uci_put_context(ctx);
					return RETURN_ERR;
				}
			}

			/* if it is sta we want the radio index, not the sta one */
			if (uci_converter_is_sta(*rpc_index)) {
				*rpc_index = sta_to_radio_index(*rpc_index);
				if (*rpc_index == RETURN_ERR) {
					_thread_local_uci_put_context(ctx);
					return RETURN_ERR;
				}
			}

			_thread_local_uci_put_context(ctx);
			return RETURN_OK;
		}
	}
	*rpc_index = -1;
	_thread_local_uci_put_context(ctx);
	return RETURN_ERR;

}

/**************************************************************************/
/*! \fn int uci_getIndexFromBssid(const char *bssid, int *rpc_index)
 **************************************************************************
 *  \brief get UCI index from the bssid
 *  \param[in] const char *bssid - bssid string
 *  \param[out] int *rpc_index - output the UCI index
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int uci_getIndexFromBssid(const char *bssid, int *rpc_index)
{
	struct uci_ptr ptr;
	struct uci_context *ctx;
	struct uci_package *p = NULL;
	struct uci_element *e;
	struct uci_section *s;
	char lookup_str[MAX_UCI_BUF_LEN] = "wireless";

	if ((bssid == NULL) || (rpc_index == NULL)) {
		ERROR("bssid or rpc_index is NULL!\n");
		return RETURN_ERR;
	}

	ctx = _thread_local_uci_get_context();
	if (!ctx)
		return RETURN_ERR;

	if ((uci_lookup_ptr(ctx, &ptr, lookup_str, true) != UCI_OK)) {
		ERROR("uci lookup failed!\n");
		_thread_local_uci_put_context(ctx);
		return RETURN_ERR;
	}

	p = ptr.p;

	uci_foreach_element(&p->sections, e) {
		struct uci_element *n;
		int scanf_res;

		s = uci_to_section(e);

		if (strncmp(s->type, "wifi-iface", MAX_UCI_BUF_LEN))
			continue;

		uci_foreach_element(&s->options, n) {
			struct uci_option *o = uci_to_option(n);

			if (strncmp(n->name, "macaddr", MAX_UCI_BUF_LEN))
				continue;

			if (o->type != UCI_TYPE_STRING)
				continue;

			if (strncmp(bssid, o->v.string, MAX_UCI_BUF_LEN))
				continue;

			scanf_res = sscanf_s(s->e.name, "default_radio%d", rpc_index);

			if (scanf_res < 1 || *rpc_index < 0) {
				*rpc_index = -1;
				_thread_local_uci_put_context(ctx);
				return RETURN_ERR;
			}
			/* if it is dummy we want the radio index, not the dummy one */
			if (uci_converter_is_dummy(*rpc_index)) {
				*rpc_index = dummy_to_radio_index(*rpc_index);
				if (*rpc_index == RETURN_ERR) {
					_thread_local_uci_put_context(ctx);
					return RETURN_ERR;
				}
			}

			_thread_local_uci_put_context(ctx);
			return RETURN_OK;
		}
	}
	*rpc_index = -1;
	_thread_local_uci_put_context(ctx);
	return RETURN_ERR_NOT_FOUND;

}


#endif /* ONEWIFI_UCI_TRANSITION */ 
/**************************************************************************/
/*! \fn int uci_converter_get(const char* path, char* value, size_t size)
 **************************************************************************
 *  \brief generic get from UCI database
 *  \param[in] const char *path - variable full name to get (config file.device name.option) for example wireless.default_radio45.wps_state
 *  \param[out] char *value - value from UCI DB
 *  \param[in] size_t size - size of the value buffer
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int uci_converter_get(const char* path, char* value, size_t size)
{
	struct uci_ptr ptr;
	struct uci_context *cont;
	char lookup_path[MAX_UCI_BUF_LEN] = "";

	if ((path == NULL) || (value == NULL)) {
		ERROR("path or value is NULL!\n");
		return RETURN_ERR;
	}

	if (size == 0) {
		ERROR("%s, output buffer size is 0!\n", __FUNCTION__);
		return RETURN_ERR;
	}

	value[0] = '\0';
	cont = _thread_local_uci_get_context();
	if (!cont)
		return RETURN_ERR;

	if (UCI_CONVERTER_LOOKUP_PTR(cont, &ptr, lookup_path, path, true) != UCI_OK || !ptr.o) {
		_thread_local_uci_put_context(cont);
		return RETURN_ERR;
	}

	if (strcpy_s(value, size, ptr.o->v.string)) {
		ERROR("output buffer is too small!\n");
		_thread_local_uci_put_context(cont);
		return RETURN_ERR;
	}

	_thread_local_uci_put_context(cont);
	return RETURN_OK;
}

#ifndef ONEWIFI_UCI_TRANSITION
/**************************************************************************/
/*! \fn int uci_converter_get_str_ext(enum paramType type, int index, const char param[], char *value, size_t size)
 **************************************************************************
 *  \brief get a specific radio or VAP param string from UCI database
 *  \param[in] enum paramType type - TYPE_RADIO / TYPE_RADIO_VAP / TYPE_VAP
 *  \param[in] int index - radio or VAP UCI index
 *  \param[in] const char param[] - param name to get (not full path)
 *  \param[out] char *value - string value from UCI DB
 *  \param[in] size_t size - size of the output string buffer
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int uci_converter_get_str_by_uci_index(enum paramType type, int uciIndex, const char param[], char *value, size_t size)
{
	int status;
	char path[MAX_UCI_BUF_LEN] = "";

	if ((param == NULL) || (value == NULL)) {
		ERROR("param or value is NULL!\n");
		return RETURN_ERR;
	}

	status = sprintf_s(path, MAX_UCI_BUF_LEN, "wireless.%s%d.%s", get_type_str(type), uciIndex, param);
	if (status <= 0) {
		ERROR("failed sprintf_s(). uciIndex=%d param=%s\n", uciIndex, param);
		return RETURN_ERR;
	}

	status = uci_converter_get(path, value, size);
#if 0
	if (status == RETURN_OK) {
		DEBUG("uciIndex=%d %s=%s\n", uciIndex, param, value);
	} else {
		DEBUG("option N/A. uciIndex=%d param=%s\n", uciIndex, param);
	}
#endif
	return status;
}

/**************************************************************************/
/*! \fn int uci_converter_get_str_ext(enum paramType type, int index, const char param[], char *value, size_t size)
 **************************************************************************
 *  \brief get a specific radio or VAP param string from UCI database
 *  \param[in] enum paramType type - TYPE_RADIO / TYPE_RADIO_VAP / TYPE_VAP
 *  \param[in] int index - radio or VAP index (regular index, not UCI index)
 *  \param[in] const char param[] - param name to get (not full path)
 *  \param[out] char *value - string value from UCI DB
 *  \param[in] size_t size - size of the output string buffer
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int uci_converter_get_str_ext(enum paramType type, int index, const char param[], char *value, size_t size)
{
	int ret, uci_index;

	if (!_valid_iftype(type)) {
		ERROR("invalid iftype (%d)\n", type);
		return RETURN_ERR;
	}

	if (uci_converter_alloc_local_uci_context()) {
		ERROR("alloc local context returned err!\n");
		return RETURN_ERR;
	}

	uci_index = rpc_to_uci_index(type, index);
	if (UCI_INVALID_INDEX == uci_index) {
		// ERROR("failed getting UCI index from RPC index (%d)\n", index);
		uci_converter_free_local_uci_context();
		return RETURN_ERR;
	}

	ret = uci_converter_get_str_by_uci_index(type, uci_index, param, value, size);
	uci_converter_free_local_uci_context();
	return ret;
}

/**************************************************************************/
/*! \fn int uci_converter_get_str(enum paramType type, int index, const char param[], char *value)
 **************************************************************************
 *  \brief get a specific radio or VAP param string from UCI database.
 *  THIS IS OBSOLETE FUNCTION and MUST NOT BE USED!
 *  Use uci_converter_get_str_ext() insted.
 *  \param[in] enum paramType type - TYPE_RADIO / TYPE_RADIO_VAP / TYPE_VAP
 *  \param[in] int index - radio or VAP index (regular index, not UCI index)
 *  \param[in] const char param[] - param name to get (not full path)
 *  \param[out] char *value - string value from UCI DB
  *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int uci_converter_get_str(enum paramType type, int index, const char param[], char *value)
{
       return uci_converter_get_str_ext(type, index, param, value, MAX_UCI_BUF_LEN);
}

/**************************************************************************/
/*! \fn void uci_converter_get_optional_str(enum paramType type, int index, const char param[], char *value, const char* default_val)
 **************************************************************************
 *  \brief get a specific radio or VAP param string from UCI database, if not found return a default value
 *  \param[in] enum paramType type - TYPE_RADIO / TYPE_RADIO_VAP / TYPE_VAP
 *  \param[in] int index - radio or VAP index (regular index, not UCI index)
 *  \param[in] const char param[] - param name to get (not full path)
 *  \param[out] char *value - string value from UCI DB
 *  \param[in] size_t *size - size of the value buffer
 *  \param[in] char *default_val - in case not found return this value as default
 ***************************************************************************/
void uci_converter_get_optional_str(enum paramType type, int index, const char param[], char *value, size_t size, const char* default_val)
{
	int status;

	if ((param == NULL) || (value == NULL) || (default_val == NULL)) {
		ERROR("param or value or default_val is NULL!\n");
		return;
	}

	status = uci_converter_get_str_ext(type, index, param, value, size);
	if (status == RETURN_ERR)
		strncpy_s(value, size, default_val, strnlen_s(default_val, size - 1));
}

/**************************************************************************/
/*! \fn int uci_converter_get_int(enum paramType type, int index, const char param[], int *value)
 **************************************************************************
 *  \brief get a specific radio or VAP param integer from UCI database
 *  \param[in] enum paramType type - TYPE_RADIO / TYPE_RADIO_VAP / TYPE_VAP
 *  \param[in] int index - radio or VAP index (regular index, not UCI index)
 *  \param[in] const char param[] - param name to get (not full path)
 *  \param[out] int *value - integer value from UCI DB
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int uci_converter_get_int(enum paramType type, int index, const char param[], int *value)
{
	int status;
	char val[MAX_UCI_BUF_LEN] = "";

	if ((param == NULL) || (value == NULL)) {
		ERROR("param or value is NULL!\n");
		return RETURN_ERR;
	}

	status = uci_converter_get_str_ext(type, index, param, val, sizeof(val));
	if (status == RETURN_ERR)
		return RETURN_ERR;

	status = sscanf_s(val, "%d", value);
	if (status != 1)
		return RETURN_ERR;

	return RETURN_OK;
}

/**************************************************************************/
/*! \fn void uci_converter_get_optional_int(enum paramType type, int index, const char param[], int *value, int default_val)
 **************************************************************************
 *  \brief get a specific radio or VAP param integer from UCI database, if not found return a default value
 *  \param[in] enum paramType type - TYPE_RADIO / TYPE_RADIO_VAP / TYPE_VAP
 *  \param[in] int index - radio or VAP index (regular index, not UCI index)
 *  \param[in] const char param[] - param name to get (not full path)
 *  \param[out] int *value - integer value from UCI DB
 *  \param[in] int default_val - in case not found return this value as default
 ***************************************************************************/
void uci_converter_get_optional_int(enum paramType type, int index, const char param[], int *value, int default_val)
{
	int status;

	if ((param == NULL) || (value == NULL)) {
		ERROR("param or value is NULL!\n");
		return;
	}

	status = uci_converter_get_int(type, index, param, value);
	if (status == RETURN_ERR)
		*value = default_val;
}

/**************************************************************************/
/*! \fn int uci_converter_get_uint(enum paramType type, int index, const char param[], unsigned int *value)
 **************************************************************************
 *  \brief get a specific radio or VAP param unsigned integer from UCI database
 *  \param[in] enum paramType type - TYPE_RADIO / TYPE_RADIO_VAP / TYPE_VAP
 *  \param[in] int index - radio or VAP index (regular index, not UCI index)
 *  \param[in] const char param[] - param name to get (not full path)
 *  \param[out] unsigned int *value - unsigned integer value from UCI DB
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int uci_converter_get_uint(enum paramType type, int index, const char param[], unsigned int *value)
{
	int status;
	char val[MAX_UCI_BUF_LEN] = "";

	if ((param == NULL) || (value == NULL)) {
		ERROR("param or value is NULL!\n");
		return RETURN_ERR;
	}

	status = uci_converter_get_str_ext(type, index, param, val, sizeof(val));
	if (status == RETURN_ERR)
		return RETURN_ERR;

	status = sscanf_s(val, "%u", value);
	if (status != 1)
		return RETURN_ERR;

	return RETURN_OK;
}

/**************************************************************************/
/*! \fn void uci_converter_get_optional_uint(enum paramType type, int index, const char param[], unsigned int *value, unsigned int default_val)
 **************************************************************************
 *  \brief get a specific radio or VAP param unsigned integer from UCI database, if not found return a default value
 *  \param[in] enum paramType type - TYPE_RADIO / TYPE_RADIO_VAP / TYPE_VAP
 *  \param[in] int index - radio or VAP index (regular index, not UCI index)
 *  \param[in] const char param[] - param name to get (not full path)
 *  \param[out] unsigned int *value - unsigned integer value from UCI DB
 *  \param[in] unsigned int default_val - in case not found return this value as default
 ***************************************************************************/
void uci_converter_get_optional_uint(enum paramType type, int index, const char param[], unsigned int *value, unsigned int default_val)
{
	int status;

	if ((param == NULL) || (value == NULL)) {
		ERROR("param or value is NULL!\n");
		return;
	}

	status = uci_converter_get_uint(type, index, param, value);
	if (status == RETURN_ERR)
		*value = default_val;
}

/**************************************************************************/
/*! \fn int uci_converter_get_ulong(enum paramType type, int index, const char param[], unsigned long *value)
 **************************************************************************
 *  \brief get a specific radio or VAP param unsigned long from UCI database
 *  \param[in] enum paramType type - TYPE_RADIO / TYPE_RADIO_VAP / TYPE_VAP
 *  \param[in] int index - radio or VAP index (regular index, not UCI index)
 *  \param[in] const char param[] - param name to get (not full path)
 *  \param[out] unsigned long *value - unsigned long value from UCI DB
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int uci_converter_get_ulong(enum paramType type, int index, const char param[], unsigned long *value)
{
	int status;
	char val[MAX_UCI_BUF_LEN] = "";

	if ((param == NULL) || (value == NULL)) {
		ERROR("param or value is NULL!\n");
		return RETURN_ERR;
	}

	status = uci_converter_get_str_ext(type, index, param, val, sizeof(val));
	if (status == RETURN_ERR)
		return RETURN_ERR;

	status = sscanf_s(val, "%lu", value);
	if (status < 1)
		return RETURN_ERR;

	return RETURN_OK;
}

/**************************************************************************/
/*! \fn void uci_converter_get_optional_ulong(enum paramType type, int index, const char param[], unsigned long *value, unsigned long default_val)
 **************************************************************************
 *  \brief get a specific radio or VAP param unsigned long from UCI database, if not found return a default value
 *  \param[in] enum paramType type - TYPE_RADIO / TYPE_RADIO_VAP / TYPE_VAP
 *  \param[in] int index - radio or VAP index (regular index, not UCI index)
 *  \param[in] const char param[] - param name to get (not full path)
 *  \param[out] unsigned long *value - unsigned long value from UCI DB
 *  \param[in] unsigned long default_val - in case not found return this value as default
 ***************************************************************************/
void uci_converter_get_optional_ulong(enum paramType type, int index, const char param[], unsigned long *value, unsigned long default_val)
{
	int status;

	if ((param == NULL) || (value == NULL)) {
		ERROR("param or value is NULL!\n");
		return;
	}

	status = uci_converter_get_ulong(type, index, param, value);
	if (status == RETURN_ERR)
		*value = default_val;
}

/**************************************************************************/
/*! \fn int uci_converter_get_ushort(enum paramType type, int index, const char param[], unsigned short *value)
 **************************************************************************
 *  \brief get a specific radio or VAP param unsigned short from UCI database
 *  \param[in] enum paramType type - TYPE_RADIO / TYPE_RADIO_VAP / TYPE_VAP
 *  \param[in] int index - radio or VAP index (regular index, not UCI index)
 *  \param[in] const char param[] - param name to get (not full path)
 *  \param[out] unsigned short *value - unsigned short value from UCI DB
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int uci_converter_get_ushort(enum paramType type, int index, const char param[], unsigned short *value)
{
	int status;
	char val[MAX_UCI_BUF_LEN] = "";

	if ((param == NULL) || (value == NULL)) {
		ERROR("param or value is NULL!\n");
		return RETURN_ERR;
	}

	status = uci_converter_get_str_ext(type, index, param, val, sizeof(val));
	if (status == RETURN_ERR)
		return RETURN_ERR;

	status = sscanf_s(val, "%hu", value);
	if (status < 1)
		return RETURN_ERR;

	return RETURN_OK;
}

/**************************************************************************/
/*! \fn void uci_converter_get_optional_ushort(enum paramType type, int index, const char param[], unsigned short *value, unsigned short default_val)
 **************************************************************************
 *  \brief get a specific radio or VAP param unsigned short from UCI database, if not found return a default value
 *  \param[in] enum paramType type - TYPE_RADIO / TYPE_RADIO_VAP / TYPE_VAP
 *  \param[in] int index - radio or VAP index (regular index, not UCI index)
 *  \param[in] const char param[] - param name to get (not full path)
 *  \param[out] unsigned short *value - unsigned short value from UCI DB
 *  \param[in] unsigned short default_val - in case not found return this value as default
 ***************************************************************************/
void uci_converter_get_optional_ushort(enum paramType type, int index, const char param[], unsigned short *value, unsigned short default_val)
{
	int status;

	if ((param == NULL) || (value == NULL)) {
		ERROR("param or value is NULL!\n");
		return;
	}

	status = uci_converter_get_ushort(type, index, param, value);
	if (status == RETURN_ERR)
		*value = default_val;
}

/**************************************************************************/
/*! \fn int uci_converter_get_bool(enum paramType type, int index, const char param[], bool *value)
 **************************************************************************
 *  \brief get a specific radio or VAP param boolean from UCI database
 *  \param[in] enum paramType type - TYPE_RADIO / TYPE_RADIO_VAP / TYPE_VAP
 *  \param[in] int index - radio or VAP index (regular index, not UCI index)
 *  \param[in] const char param[] - param name to get (not full path)
 *  \param[out] bool *value - boolean value from UCI DB
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int uci_converter_get_bool(enum paramType type, int index, const char param[], bool *value)
{
	int status;
	int res = 0;

	if ((param == NULL) || (value == NULL)) {
		ERROR("param or value is NULL!\n");
		return RETURN_ERR;
	}

	status = uci_converter_get_int(type, index, param, &res);
	if (status == RETURN_ERR)
		return RETURN_ERR;

	*value = (res != 0) ? true : false;

	return RETURN_OK;
}

/**************************************************************************/
/*! \fn void uci_converter_get_optional_bool(enum paramType type, int index, const char param[], bool *value, bool default_val)
 **************************************************************************
 *  \brief get a specific radio or VAP param boolean from UCI database, if not found return a default value
 *  \param[in] enum paramType type - TYPE_RADIO / TYPE_RADIO_VAP / TYPE_VAP
 *  \param[in] int index - radio or VAP index (regular index, not UCI index)
 *  \param[in] const char param[] - param name to get (not full path)
 *  \param[out] bool *value - boolean value from UCI DB
 *  \param[in] bool default_val - in case not found return this value as default
 ***************************************************************************/
void uci_converter_get_optional_bool(enum paramType type, int index, const char param[], bool *value, bool default_val)
{
	int status;

	if ((param == NULL) || (value == NULL)) {
		ERROR("param or value is NULL!\n");
		return;
	}

	status = uci_converter_get_bool(type, index, param, value);
	if (status == RETURN_ERR)
		*value = default_val;
}

#ifdef CONFIG_RPCD

/* "Set" for compatibility with old API */
static int (*g_uci_converter_cb_set_compat)(char* path,
		const char* option, const char* value) = NULL;

/* Obsolete function */
void set_uci_converter_fun(int (*callback)(char* path, const char* option, const char* value))
{
        /* DEBUG("%s:%d uci_converter_set: %p cb: %p\n", __FUNCTION__, __LINE__, (void*)uci_converter_set_str, (void*)callback); */
        g_uci_converter_cb_set_compat = callback;
        return;
}

#else

void set_uci_converter_fun(int (*callback)(char* path, const char* option, const char* value))
{
	/* Do nothing */
	UNUSED_PARAM(callback);
}
#endif /* CONFIG_RPCD */

#endif /* ONEWIFI_UCI_TRANSITION */ 
/**************************************************************************/
/*! \fn int uci_converter_set(const char* path, const char* option, const char* value)
 **************************************************************************
 *  \brief generic set to UCI database
 *  \param[in] const char *path - config name to set (config file.device name) for example wireless.default_radio45
 *  \param[in] const char *option - option (param) name to set
 *  \param[in] const char *value - value to write tom UCI DB
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int uci_converter_set(const char* path, const char* option, const char* value)
#ifdef CONFIG_RPCD
{
	if (g_uci_converter_cb_set_compat) {
		/* Call the old API*/
		if (path) {
			char path_buf[MAX_UCI_BUF_LEN] = "";

			if (strcpy_s(path_buf, sizeof(path_buf), path)) {
				ERROR("path is too long!\n");
				return RETURN_ERR;
			}
			return g_uci_converter_cb_set_compat(path_buf, option, value);
		}
		else {
			return g_uci_converter_cb_set_compat(NULL, option, value);
		}
	} else {
		return _ubus_uci_converter_set(path, option, value);
	}
}
#else
{
	int status;
	struct uci_ptr ptr;
	struct uci_context *cont;
	char lookup_path[MAX_UCI_BUF_LEN] = "";

	if ((path == NULL) || (option == NULL) || (value == NULL)) {
		ERROR("path or option or value is NULL!\n");
		return RETURN_ERR;
	}

	cont = _thread_local_uci_get_context();
	if (!cont)
		return RETURN_ERR;

	if ((UCI_CONVERTER_LOOKUP_PTR(cont, &ptr, lookup_path, path, true) != UCI_OK) || (!ptr.s)) {
		_thread_local_uci_put_context(cont);
		return RETURN_ERR;
	}

	ptr.option = option;
	ptr.value = value;

	if (uci_set(cont, &ptr) != UCI_OK) {
		_thread_local_uci_put_context(cont);
		return RETURN_ERR;
	}

	status = uci_save(cont, ptr.p);

	_thread_local_uci_put_context(cont);

	return (status == UCI_OK) ? RETURN_OK : RETURN_ERR;
}
#endif /* CONFIG_RPCD */
#ifndef ONEWIFI_UCI_TRANSITION

/**************************************************************************/
/*! \fn int uci_converter_add_device(const char* config_file, const char* device_name, const char* device_type)
 **************************************************************************
 *  \brief add a device to UCI database (like a new VAP)
 *  \param[in] const char* config_file - config file name for example wireless
 *  \param[in] const char* device_name - device name for example default_radio45
 *  \param[in] const char* device_type - device type for example wifi-iface
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int uci_converter_add_device(const char* config_file, const char* device_name, const char* device_type)
{
	int status;
	char path[MAX_UCI_BUF_LEN] = "";
	struct uci_ptr ptr;
	struct uci_context *cont;

	if (!config_file || !device_name || !device_type)
		return RETURN_ERR;

	status = sprintf_s(path, MAX_UCI_BUF_LEN, "%s.%s=%s", config_file, device_name, device_type);
	if (status <= 0)
		return RETURN_ERR;

	cont = _thread_local_uci_get_context();
	if (!cont)
		return RETURN_ERR;

	if ((uci_lookup_ptr(cont, &ptr, path, true) != UCI_OK)) {
		_thread_local_uci_put_context(cont);
		return RETURN_ERR;
	}

	if (uci_set(cont, &ptr) != UCI_OK) {
		_thread_local_uci_put_context(cont);
		return RETURN_ERR;
	}

	status = uci_save(cont, ptr.p);

	_thread_local_uci_put_context(cont);

	return (status == UCI_OK) ? RETURN_OK : RETURN_ERR;
}

/**************************************************************************/
/*! \fn int uci_converter_set_str(enum paramType type, int index, const char param[], const char *value)
 **************************************************************************
 *  \brief set a specific radio or VAP param string to UCI database
 *  \param[in] enum paramType type - TYPE_RADIO / TYPE_RADIO_VAP / TYPE_VAP
 *  \param[in] int index - radio or VAP index (regular index, not UCI index)
 *  \param[in] const char param[] - param name to get (not full path)
 *  \param[in] const char *value - string value to set in UCI DB
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int uci_converter_set_str(enum paramType type, int index, const char param[], const char *value)
{
	int ret = RETURN_ERR;
	int status;
	char path[MAX_UCI_BUF_LEN] = "";
	char get_value[MAX_LEN_VALID_VALUE] = "";
	int uci_index;
	const char* s_type;

	if ((param == NULL) || (value == NULL)) {
		ERROR("param or value is NULL!\n");
		return RETURN_ERR;
	}

	if (!_valid_iftype(type)) {
		ERROR("invalid iftype (%d)\n", type);
		return RETURN_ERR;
	}

	if (uci_converter_alloc_local_uci_context()) {
		ERROR("alloc local context returned err!\n");
		return RETURN_ERR;
	}

	uci_index = rpc_to_uci_index(type, index);
	if (UCI_INVALID_INDEX == uci_index) {
		// ERROR("failed getting UCI index from RPC index (%d)\n", index);
		uci_converter_free_local_uci_context();
		return RETURN_ERR;
	}

	s_type = get_type_str(type);
	status = sprintf_s(path, MAX_UCI_BUF_LEN, "wireless.%s%d.%s", s_type, uci_index, param);
	if (status <= 0) {
		ERROR("failed sprintf_s(). index=%d param=%s\n", index, param);
		goto end;
	}

	status = uci_converter_get(path, get_value, sizeof(get_value));
	if (status != RETURN_OK)
		get_value[0] = '\0';

	if (!strncmp(value, get_value, sizeof(get_value))) {
		DEBUG("same value being set index=%d %s=%s\n", index, param, value);
		ret = RETURN_OK;
		goto end;
	}

	sprintf_s(path, MAX_UCI_BUF_LEN, "wireless.%s%d", s_type, uci_index);
	status = uci_converter_set(path, param, value);
	if (status == RETURN_OK) {
		DEBUG("index=%d %s=%s\n", index, param, value);
	} else {
		ERROR("failed. index=%d %s=%s\n", index, param, value);
		goto end;
	}

	ret = RETURN_OK;

end:
	uci_converter_free_local_uci_context();
	return ret;
}

/**************************************************************************/
/*! \fn int uci_converter_set_optional_int(enum paramType type, int index, const char param[], int value, int default_val)
 **************************************************************************
 *  \brief set an optional specific radio or VAP param integer to UCI database.
 *  \param[in] enum paramType type - TYPE_RADIO / TYPE_RADIO_VAP / TYPE_VAP
 *  \param[in] int index - radio or VAP index (regular index, not UCI index)
 *  \param[in] const char param[] - param name to get (not full path)
 *  \param[in] int value - integer value to set in UCI DB
 *  \param[in] int default_val - default value
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int uci_converter_set_optional_int(enum paramType type, int index, const char param[], int value, int default_val)
{
	int actual_val = -1;
	int status;
	char val[MAX_UCI_BUF_LEN] = "";

	if (param == NULL) {
		ERROR("param is NULL!\n");
		return RETURN_ERR;
	}

	status = uci_converter_get_int(type, index, param, &actual_val);

	/* Skip setting if the param not been set before
           and the new value is equal to the default value */
	if ((status == RETURN_ERR) && (value == default_val))
		return RETURN_OK;

	/* Also don't set if the new value is equal to the actual value */
	if ((status == RETURN_OK) && (value == actual_val))
		return RETURN_OK;

	status = sprintf_s(val, MAX_UCI_BUF_LEN, "%d", value);
	if (status <= 0)
		return RETURN_ERR;

	return uci_converter_set_str(type, index, param, val);
}

/**************************************************************************/
/*! \fn int uci_converter_set_int(enum paramType type, int index, const char param[], int value)
 **************************************************************************
 *  \brief set a specific radio or VAP param integer to UCI database
 *  \param[in] enum paramType type - TYPE_RADIO / TYPE_RADIO_VAP / TYPE_VAP
 *  \param[in] int index - radio or VAP index (regular index, not UCI index)
 *  \param[in] const char param[] - param name to get (not full path)
 *  \param[in] int value - integer value to set in UCI DB
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int uci_converter_set_int(enum paramType type, int index, const char param[], int value)
{
	int status;
	char val[MAX_UCI_BUF_LEN] = "";

	if (param == NULL) {
		ERROR("param is NULL!\n");
		return RETURN_ERR;
	}

	status = sprintf_s(val, MAX_UCI_BUF_LEN, "%d", value);
	if (status <= 0)
		return RETURN_ERR;

	status = uci_converter_set_str(type, index, param, val);

	return status;
}

/**************************************************************************/
/*! \fn int uci_converter_set_uint(enum paramType type, int index, const char param[], unsigned int value)
 **************************************************************************
 *  \brief set a specific radio or VAP param unsigned integer to UCI database
 *  \param[in] enum paramType type - TYPE_RADIO / TYPE_RADIO_VAP / TYPE_VAP
 *  \param[in] int index - radio or VAP index (regular index, not UCI index)
 *  \param[in] const char param[] - param name to get (not full path)
 *  \param[in] unsigned int value - unsigned integer value to set in UCI DB
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int uci_converter_set_uint(enum paramType type, int index, const char param[], unsigned int value)
{
	int status;
	char val[MAX_UCI_BUF_LEN] = "";

	if (param == NULL) {
		ERROR("param is NULL!\n");
		return RETURN_ERR;
	}

	status = sprintf_s(val, MAX_UCI_BUF_LEN, "%u", value);
	if (status <= 0)
		return RETURN_ERR;

	status = uci_converter_set_str(type, index, param, val);

	return status;
}

/**************************************************************************/
/*! \fn int uci_converter_set_ulong(enum paramType type, int index, const char param[], unsigned long value)
 **************************************************************************
 *  \brief set a specific radio or VAP param unsigned long to UCI database
 *  \param[in] enum paramType type - TYPE_RADIO / TYPE_RADIO_VAP / TYPE_VAP
 *  \param[in] int index - radio or VAP index (regular index, not UCI index)
 *  \param[in] const char param[] - param name to get (not full path)
 *  \param[in] unsigned long value - unsigned long value to set in UCI DB
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int uci_converter_set_ulong(enum paramType type, int index, const char param[], unsigned long value)
{
	int status;
	char val[MAX_UCI_BUF_LEN] = "";

	if (param == NULL) {
		ERROR("param is NULL!\n");
		return RETURN_ERR;
	}

	status = sprintf_s(val, MAX_UCI_BUF_LEN, "%lu", value);
	if (status <= 0)
		return RETURN_ERR;

	status = uci_converter_set_str(type, index, param, val);

	return status;
}

/**************************************************************************/
/*! \fn int uci_converter_set_ushort(enum paramType type, int index, const char param[], unsigned short value)
 **************************************************************************
 *  \brief set a specific radio or VAP param unsigned short to UCI database
 *  \param[in] enum paramType type - TYPE_RADIO / TYPE_RADIO_VAP / TYPE_VAP
 *  \param[in] int index - radio or VAP index (regular index, not UCI index)
 *  \param[in] const char param[] - param name to get (not full path)
 *  \param[in] unsigned short value - unsigned short value to set in UCI DB
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int uci_converter_set_ushort(enum paramType type, int index, const char param[], unsigned short value)
{
	int status;
	char val[MAX_UCI_BUF_LEN] = "";

	status = sprintf_s(val, MAX_UCI_BUF_LEN, "%u", value);
	if (status <= 0)
		return RETURN_ERR;

	status = uci_converter_set_str(type, index, param, val);

	return status;
}

/**************************************************************************/
/*! \fn int uci_converter_set_optional_bool(enum paramType type, int index, const char param[], bool value, bool default_val)
 **************************************************************************
 *  \brief set an optional specific radio or VAP param boolean to UCI database
 *  \param[in] enum paramType type - TYPE_RADIO / TYPE_RADIO_VAP / TYPE_VAP
 *  \param[in] int index - radio or VAP index (regular index, not UCI index)
 *  \param[in] const char param[] - param name to get (not full path)
 *  \param[in] bool value - boolean value to set in UCI DB
 *  \param[in] int default_val - default value
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int uci_converter_set_optional_bool(enum paramType type, int index, const char param[], bool value, bool default_val)
{
	return uci_converter_set_optional_int(type, index, param, (value ? 1 : 0), (default_val ? 1 : 0));
}

/**************************************************************************/
/*! \fn int uci_converter_set_bool(enum paramType type, int index, const char param[], bool value)
 **************************************************************************
 *  \brief set a specific radio or VAP param boolean to UCI database
 *  \param[in] enum paramType type - TYPE_RADIO / TYPE_RADIO_VAP / TYPE_VAP
 *  \param[in] int index - radio or VAP index (regular index, not UCI index)
 *  \param[in] const char param[] - param name to get (not full path)
 *  \param[in] bool value - boolean value to set in UCI DB
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int uci_converter_set_bool(enum paramType type, int index, const char param[], bool value)
{
	if (param == NULL) {
		ERROR("param is NULL!\n");
		return RETURN_ERR;
	}

	return uci_converter_set_int(type, index, param, (value ? 1 : 0));
}

/**************************************************************************/
/*! \fn int uci_converter_del_elem(const char* path)
 **************************************************************************
 *  \brief remove an element from the UCI database (using full path)
 *  \param[in] const char *path - complete path of the element to delete
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int uci_converter_del_elem(const char* path)
#ifdef CONFIG_RPCD
{
	return _ubus_uci_converter_del(path);
}
#else
{
	int status = RETURN_ERR;
	struct uci_ptr ptr;
	struct uci_context *cont;
	char lookup_path[MAX_UCI_BUF_LEN] = "";

	if (path == NULL) {
		ERROR("path is NULL!\n");
		goto out;
	}

	cont = _thread_local_uci_get_context();
	if (!cont)
		goto out;

	if (UCI_CONVERTER_LOOKUP_PTR(cont, &ptr, lookup_path, path, true) != UCI_OK)
		goto out_after_cont;

	if (!ptr.o) { /* Param not found, nothing to do apprently */
		status = RETURN_OK;
		goto out_after_cont;
	}

	if (uci_delete(cont, &ptr) != UCI_OK)
		goto out_after_cont;

	if (uci_save(cont, ptr.p) != UCI_OK)
		goto out_after_cont;

	status = RETURN_OK;

out_after_cont:
	_thread_local_uci_put_context(cont);
out:
	return status;
}
#endif /* CONFIG_RPCD */

/**************************************************************************/
/*! \fn int uci_converter_del(enum paramType type, int index, const char* param)
 **************************************************************************
 *  \brief remove an element from the UCI database (using param name)
 *  \param[in] enum paramType type - TYPE_RADIO / TYPE_RADIO_VAP / TYPE_VAP
 *  \param[in] int index - radio or VAP index (regular index, not UCI index)
 *  \param[in] const char param[] - param name to delete (not full path)
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int uci_converter_del(enum paramType type, int index, const char* param)
{
	int status, uci_index;
	char path[MAX_UCI_BUF_LEN] = "";

	if (param == NULL) {
		ERROR("param is NULL!\n");
		return RETURN_ERR;
	}

	if (!_valid_iftype(type)) {
		ERROR("invalid iftype (%d)\n", type);
		return RETURN_ERR;
	}

	uci_index = rpc_to_uci_index(type, index);
	if (UCI_INVALID_INDEX == uci_index) {
		// ERROR("failed getting UCI index from RPC index (%d)\n", index);
		return RETURN_ERR;
	}

	if (sprintf_s(path, MAX_UCI_BUF_LEN, "wireless.%s%d.%s", get_type_str(type), uci_index, param) <= 0) {
		ERROR("failed sprintf_s(). index=%d param=%s", index, param);
		return RETURN_ERR;
	}

	status = uci_converter_del_elem(path);
	if (status == RETURN_ERR) {
		ERROR("failed: delete %s for index %d\n", param, index);
		return status;
	}

	return RETURN_OK;
}

/**************************************************************************/
/*! \fn int uci_converter_add_list(const char* path, const char* option, const char* value)
 **************************************************************************
 *  \brief append a string to a UCI element list (using path and param)
 *  \param[in] const char *path - element path (config file.device name)
 *  \param[in] const char *option - param name to append string to
 *  \param[in] const char *value - string value to append
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int uci_converter_add_list(const char* path, const char* option, const char* value)
{
	int status;
	struct uci_ptr ptr;
	struct uci_context *cont;
	char lookup_path[MAX_UCI_BUF_LEN] = "";

	if ((path == NULL) || (option == NULL) || (value == NULL)) {
		ERROR("path or option or value is NULL!\n");
		return RETURN_ERR;
	}

	cont = _thread_local_uci_get_context();
	if (!cont)
		return RETURN_ERR;

	if ((UCI_CONVERTER_LOOKUP_PTR(cont, &ptr, lookup_path, path, true) != UCI_OK) || (!ptr.s)) {
		_thread_local_uci_put_context(cont);
		return RETURN_ERR;
	}

	ptr.option = option;
	ptr.value = value;

	if (uci_add_list(cont, &ptr) != UCI_OK) {
		_thread_local_uci_put_context(cont);
		return RETURN_ERR;
	}

	status = uci_save(cont, ptr.p);

	_thread_local_uci_put_context(cont);

	return (status == UCI_OK) ? RETURN_OK : RETURN_ERR;
}

/**************************************************************************/
/*! \fn int uci_converter_add_list_str(enum paramType type, int index, const char param[], const char *value)
 **************************************************************************
 *  \brief append a string to a UCI element list (using param)
 *  \param[in] char *path - element path (config file.device name)
 *  \param[in] enum paramType type - TYPE_RADIO / TYPE_RADIO_VAP / TYPE_VAP
 *  \param[in] int index - radio or VAP index (regular index, not UCI index)
 *  \param[in] const char param[] - param name to append string to
 *  \param[in] const char *value - string value to append
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int uci_converter_add_list_str(enum paramType type, int index, const char param[], const char *value)
{
	int status, uci_index;
	char path[MAX_UCI_BUF_LEN] = "";

	if ((param == NULL) || (value == NULL)) {
		ERROR("param or value is NULL!\n");
		return RETURN_ERR;
	}

	if (!_valid_iftype(type)) {
		ERROR("invalid iftype (%d)\n", type);
		return RETURN_ERR;
	}

	uci_index = rpc_to_uci_index(type, index);
	if (UCI_INVALID_INDEX == uci_index) {
		// ERROR("failed getting UCI index from RPC index (%d)\n", index);
		return RETURN_ERR;
	}

	status = sprintf_s(path, MAX_UCI_BUF_LEN, "wireless.%s%d", get_type_str(type), uci_index);
	if (status <= 0)
		return RETURN_ERR;

	status = uci_converter_add_list(path, param, value);
	if (status == RETURN_OK) {
		DEBUG("index=%d %s=%s\n", index, param, value);
	} else {
		ERROR("failed. index=%d %s=%s\n", index, param, value);
		return RETURN_ERR;
	}

	return RETURN_OK;
}

/**************************************************************************/
/*! \fn int uci_converter_del_list(const char* path, const char* option, const char* value)
 **************************************************************************
 *  \brief delete a string from a UCI element list (using path and param)
 *  \param[in] const char *path - element path (config file.device name)
 *  \param[in] const char *option - param name to remove string from
 *  \param[in] const char *value - string value to remove
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int uci_converter_del_list(const char* path, const char* option, const char* value)
{
	int status;
	char full_path[MAX_LEN_VALID_VALUE] = "";
	struct uci_ptr ptr;
	struct uci_context *cont;

	if ((path == NULL) || (option == NULL) || (value == NULL)) {
		ERROR("path or option or value is NULL!\n");
		return RETURN_ERR;
	}

	status = sprintf_s(full_path, (MAX_LEN_VALID_VALUE - 1), "%s.%s=%s", path, option, value);
	if (status <= 0) {
		ERROR("sprintf_s failed. full path, option=%s value=%s\n", option, value);
		return RETURN_ERR;
	}

	cont = _thread_local_uci_get_context();
	if (!cont)
		return RETURN_ERR;

	if ((uci_lookup_ptr(cont, &ptr, full_path, true) != UCI_OK) || (!ptr.s)) {
		_thread_local_uci_put_context(cont);
		return RETURN_ERR;
	}

	if (uci_del_list(cont, &ptr) != UCI_OK) {
		_thread_local_uci_put_context(cont);
		return RETURN_ERR;
	}

	status = uci_save(cont, ptr.p);

	_thread_local_uci_put_context(cont);

	return (status == UCI_OK) ? RETURN_OK : RETURN_ERR;
}

/**************************************************************************/
/*! \fn int uci_converter_del_list_str(enum paramType type, int index, const char param[], const char *value)
 **************************************************************************
 *  \brief delete a string from a UCI element list (using param)
 *  \param[in] enum paramType type - TYPE_RADIO / TYPE_RADIO_VAP / TYPE_VAP
 *  \param[in] int index - radio or VAP index (regular index, not UCI index)
 *  \param[in] const char param[] - param name to remove string from
 *  \param[in] const char *value - string value to remove
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int uci_converter_del_list_str(enum paramType type, int index, const char param[], const char *value)
{
	int status, uci_index;
	char path[MAX_UCI_BUF_LEN] = "";

	if ((param == NULL) || (value == NULL)) {
		ERROR("param or value is NULL!\n");
		return RETURN_ERR;
	}

	if (!_valid_iftype(type)) {
		ERROR("invalid iftype (%d)\n", type);
		return RETURN_ERR;
	}

	uci_index = rpc_to_uci_index(type, index);
	if (UCI_INVALID_INDEX == uci_index) {
		// ERROR("failed getting UCI index from RPC index (%d)\n", index);
		return RETURN_ERR;
	}

	status = sprintf_s(path, MAX_UCI_BUF_LEN, "wireless.%s%d", get_type_str(type), uci_index);
	if (status <= 0) {
		ERROR("failed sprintf_s(). uci_index=%d\n", uci_index);
		return RETURN_ERR;
	}

	status = uci_converter_del_list(path, param, value);
	if (status == RETURN_OK) {
		DEBUG("index=%d %s=%s\n", index, param, value);
	} else {
		ERROR("failed. index=%d %s=%s\n", index, param, value);
		return RETURN_ERR;
	}

	return RETURN_OK;
}

/**************************************************************************/
/*! \fn int uci_converter_get_list(const char* list, char **output,
 *                                 int max_elems, int elem_max_size)
 **************************************************************************
 *  \brief get a list of strings from UCI element list (using full path)
 *  \param[in] const char *list - full path of list to get (config file.device name,param)
 *  \param[out] char **output - an array of strings returned
 *  \param[in] int max_elems - maximum number of elements to return
 *  \param[in] int elem_max_size - maximum buffer size for each element
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int uci_converter_get_list(const char* list, char **output, int max_elems, int elem_max_size)
{
	unsigned int index = 0;
	struct uci_element *e, *tmp;
	struct uci_ptr ptr;
	struct uci_context *cont;
	char lookup_path[MAX_UCI_BUF_LEN] = "";

	if ((list == NULL) || (output == NULL) || (*output == NULL)) {
		ERROR("list or output or *output is NULL!\n");
		return RETURN_ERR;
	}

	cont = _thread_local_uci_get_context();
	if (!cont)
		return RETURN_ERR;

	if (UCI_CONVERTER_LOOKUP_PTR(cont, &ptr, lookup_path, list, true) != UCI_OK) {
		_thread_local_uci_put_context(cont);
		return RETURN_ERR;
	}

	if (!(ptr.o && ptr.option)) {
		_thread_local_uci_put_context(cont);
		return RETURN_ERR;
	}

	if ((ptr.o->type != UCI_TYPE_LIST)) {
		_thread_local_uci_put_context(cont);
		return RETURN_ERR;
	}

	uci_foreach_element_safe(&ptr.o->v.list, tmp, e) {
		size_t len = strnlen_s(uci_to_option(e)->e.name, elem_max_size - 1);
		strncpy_s((output)[index++], elem_max_size, uci_to_option(e)->e.name, len);
		if ((unsigned int)max_elems == index)
			break;
	}

	_thread_local_uci_put_context(cont);

	return RETURN_OK;
}

/**************************************************************************/
/*! \fn int uci_converter_get_list_str(enum paramType type, int index,
 *      const char param[], char **output, int max_elems, int elem_max_size)
 **************************************************************************
 *  \brief get a list of strings from UCI element list (using param)
 *  \param[in] enum paramType type - TYPE_RADIO / TYPE_RADIO_VAP / TYPE_VAP
 *  \param[in] int index - radio or VAP index (regular index, not UCI index)
 *  \param[in] const char param[] - param name of string list to return
 *  \param[out] char **output - an array of strings returned
 *  \param[in] int max_elems - maximum number of elements to return
 *  \param[in] int elem_max_size - maximum buffer size for each element
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int uci_converter_get_list_str(enum paramType type, int index,
		const char param[], char **output, int max_elems, int elem_max_size)
{
	int status, uci_index;
	char path[MAX_UCI_BUF_LEN] = "";

	if ((param == NULL) || (output == NULL) || (*output == NULL)) {
		ERROR("param or output or *output is NULL!\n");
		return RETURN_ERR;
	}

	if (!_valid_iftype(type)) {
		ERROR("invalid iftype (%d)\n", type);
		return RETURN_ERR;
	}

	uci_index = rpc_to_uci_index(type, index);
	if (UCI_INVALID_INDEX == uci_index) {
		// ERROR("failed getting UCI index from RPC index (%d)\n", index);
		return RETURN_ERR;
	}

	status = sprintf_s(path, MAX_UCI_BUF_LEN, "wireless.%s%d.%s", get_type_str(type), uci_index, param);
	if (status <= 0)
		return RETURN_ERR;

	return uci_converter_get_list(path, output, max_elems, elem_max_size);
}

/**************************************************************************/
/*! \fn uci_converter_find_in_list(const char *list, const char* value, bool* found);
 **************************************************************************
 *  \brief find a value in UCI element list (using full path)
 *  \param[in] const char *list - full path of list to search value in (config file.device name,param)
 *  \param[in] const char* value - value for search
 *  \param[out] bool* found - returns 'true' if element exists
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int uci_converter_find_in_list(const char *list, const char* value, bool* found)
{
	struct uci_element *e, *tmp;
	struct uci_ptr ptr;
	struct uci_context *cont;
	char lookup_path[MAX_UCI_BUF_LEN] = "";

	if ((list == NULL) || (value == NULL) || (found == NULL)) {
		ERROR("%s, 'list' or 'value' or 'found' is NULL!\n", __FUNCTION__);
		return RETURN_ERR;
	}

	*found = false;
	cont = _thread_local_uci_get_context();
	if (!cont) {
		ERROR("can't get thread local UCI context. list: %s, value: %s\n", list, value);
		return RETURN_ERR;
	}

	if (UCI_CONVERTER_LOOKUP_PTR(cont, &ptr, lookup_path, list, true) != UCI_OK) {
		_thread_local_uci_put_context(cont);
		ERROR("failed at UCI_CONVERTER_LOOKUP_PTR. list: %s, value: %s\n", list, value);
		return RETURN_ERR;
	}

	if (!(ptr.o)) {
		_thread_local_uci_put_context(cont);
		ERROR("ptr.o is NULL. list: %s, value: %s\n", list, value);
		return RETURN_ERR;
	}

	if (!(ptr.option)) {
		_thread_local_uci_put_context(cont);
		ERROR("ptr.option is NULL. list: %s, value: %s\n", list, value);
		return RETURN_ERR;
	}

	if ((ptr.o->type != UCI_TYPE_LIST)) {
		_thread_local_uci_put_context(cont);
		ERROR("ptr type must be list. list: %s, value: %s\n", list, value);
		return RETURN_ERR;
	}

	uci_foreach_element_safe(&ptr.o->v.list, tmp, e) {
		if (0 == strncmp(value, uci_to_option(e)->e.name, MAX_UCI_BUF_LEN)) {
			*found = true;
			break;
		}
	}

	_thread_local_uci_put_context(cont);
	return RETURN_OK;
}

/**************************************************************************/
/*! \fn uci_converter_find_in_list_str(enum paramType type, int index, const char param[], const char* value, bool* found)
 **************************************************************************
 *  \brief find a value in UCI element list (using param)
 *  \param[in] enum paramType type - TYPE_RADIO / TYPE_RADIO_VAP / TYPE_VAP
 *  \param[in] int index - radio or VAP index (regular index, not UCI index)
 *  \param[in] const char param[] - param name of string list to search value in
 *  \param[in] const char* value - value for search
 *  \param[out] bool* found - returns 'true' if element exists
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int uci_converter_find_in_list_str(enum paramType type, int index, const char param[], const char* value, bool* found)
{
	int status, uci_index;
	char path[MAX_UCI_BUF_LEN] = "";

	if ((param == NULL) || (value == NULL) || (found == NULL)) {
		ERROR("%s, 'param' or 'value' or 'found' is NULL!\n", __FUNCTION__);
		return RETURN_ERR;
	}

	if (!_valid_iftype(type)) {
		ERROR("invalid iftype (%d)\n", type);
		return RETURN_ERR;
	}

	uci_index = rpc_to_uci_index(type, index);
	if (UCI_INVALID_INDEX == uci_index) {
		// ERROR("failed getting UCI index from RPC index (%d)\n", index);
		return RETURN_ERR;
	}

	status = sprintf_s(path, MAX_UCI_BUF_LEN, "wireless.%s%d.%s", get_type_str(type), uci_index, param);
	if (status <= 0)
		return RETURN_ERR;

	return uci_converter_find_in_list(path, value, found);
}


/**************************************************************************/
/*! \fn int uci_count_elements_list(const char *list, unsigned int *num_of_elements)
 **************************************************************************
 *  \brief count the number of elements in UCI element list (using full path)
 *  \param[in] const char *list - full path of list to count elements in (config file.device name,param)
 *  \param[out] unsigned int *num_of_elements - return number of elements
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int uci_count_elements_list(const char *list, unsigned int *num_of_elements)
{
	unsigned int count = 0;
	struct uci_element *e, *tmp;
	struct uci_ptr ptr;
	struct uci_context *cont;
	char lookup_path[MAX_UCI_BUF_LEN] = "";

	if ((list == NULL) || (num_of_elements == NULL)) {
		ERROR("list or num_of_elements is NULL!\n");
		return RETURN_ERR;
	}

	cont = _thread_local_uci_get_context();
	if (!cont)
		return RETURN_ERR;

	if (UCI_CONVERTER_LOOKUP_PTR(cont, &ptr, lookup_path, list, true) != UCI_OK) {
		_thread_local_uci_put_context(cont);
		*num_of_elements = count;
		return RETURN_ERR;
	}

	if (!(ptr.o && ptr.option)) {
		_thread_local_uci_put_context(cont);
		*num_of_elements = count;
		return RETURN_ERR;
	}

	if ((ptr.o->type != UCI_TYPE_LIST)) {
		_thread_local_uci_put_context(cont);
		*num_of_elements = count;
		return RETURN_ERR;
	}

	uci_foreach_element_safe(&ptr.o->v.list, tmp, e) {
		count++;
	}

	_thread_local_uci_put_context(cont);

	*num_of_elements = count;

	return RETURN_OK;
}

/**************************************************************************/
/*! \fn int uci_converter_count_elements_list(enum paramType type, int index, const char param[], unsigned int *num_of_elements)
 **************************************************************************
 *  \brief count the number of elements in UCI element list (using param)
 *  \param[in] enum paramType type - TYPE_RADIO / TYPE_RADIO_VAP / TYPE_VAP
 *  \param[in] int index - radio or VAP index (regular index, not UCI index)
 *  \param[in] const char param[] - param name of string list to count elements in
 *  \param[out] unsigned int *num_of_elements - return number of elements
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int uci_converter_count_elements_list(enum paramType type, int index, const char param[],
		unsigned int *num_of_elements)
{
	int status, uci_index;
	char path[MAX_UCI_BUF_LEN] = "";

	if ((param == NULL) || (num_of_elements == NULL)) {
		ERROR("param or num_of_elements is NULL!\n");
		return RETURN_ERR;
	}

	if (!_valid_iftype(type)) {
		ERROR("invalid iftype (%d)\n", type);
		return RETURN_ERR;
	}

	uci_index = rpc_to_uci_index(type, index);
	if (UCI_INVALID_INDEX == uci_index) {
		// ERROR("failed getting UCI index from RPC index (%d)\n", index);
		return RETURN_ERR;
	}

	status = sprintf_s(path, MAX_UCI_BUF_LEN, "wireless.%s%d.%s", get_type_str(type), uci_index, param);
	if (status <= 0)
		return RETURN_ERR;

	return uci_count_elements_list(path, num_of_elements);
}

int uci_converter_reorder(enum paramType type, int index, int pos)
{
	int status, uci_index;
	char path[MAX_UCI_BUF_LEN] = "";

	if (!_valid_iftype(type)) {
		ERROR("invalid iftype (%d)\n", type);
		return RETURN_ERR;
	}

	uci_index = rpc_to_uci_index(type, index);
	if (UCI_INVALID_INDEX == uci_index) {
		// ERROR("failed getting UCI index from RPC index (%d)\n", index);
		return RETURN_ERR;
	}

	status = sprintf_s(path, MAX_UCI_BUF_LEN, "wireless.%s%d", get_type_str(type), uci_index);
	if (status <= 0)
		return RETURN_ERR;

	return uci_converter_reorder_section(path, pos);
}

int uci_converter_reorder_section(const char* path, int pos)
{
	int status;
	struct uci_ptr ptr;
	struct uci_context *cont;
	char lookup_path[MAX_UCI_BUF_LEN] = "";

	if (path == NULL) {
		ERROR("path is NULL!\n");
		return RETURN_ERR;
	}

	cont = _thread_local_uci_get_context();
	if (!cont)
		return RETURN_ERR;

	if ((UCI_CONVERTER_LOOKUP_PTR(cont, &ptr, lookup_path, path, true) != UCI_OK) || (!ptr.s)) {
		_thread_local_uci_put_context(cont);
		return RETURN_ERR;
	}

	if (uci_reorder_section(cont, ptr.s, pos) != UCI_OK) {
		_thread_local_uci_put_context(cont);
		return RETURN_ERR;
	}

	status = uci_save(cont, ptr.p);

	_thread_local_uci_put_context(cont);

	return (status == UCI_OK) ? RETURN_OK : RETURN_ERR;
}

/**************************************************************************/
/*! \fn int uci_converter_revert_vaps(const char* path, const char* device)
 **************************************************************************
 *  \brief remove uncommited changes for all vaps of a radio
 *  \param[in] const char* path - file name of the db
 *  \param[in] const char* device - name of the radio
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
static int uci_converter_revert_vaps(const char* path, const char* device)
{
	int wireless_idx = -1;
	struct uci_ptr ptr;
	struct uci_context *ctx;
	struct uci_package *p = NULL;
	struct uci_element *e;
	struct uci_section *s;
	char lookup_path[MAX_UCI_BUF_LEN] = "";
	char vap_type[MAX_UCI_BUF_LEN] = "wifi-iface";

	if ((path == NULL) || (device == NULL)) {
		ERROR("path or device is NULL!\n");
		return RETURN_ERR;
	}

	ctx = _thread_local_uci_get_context();
	if (!ctx)
		return RETURN_ERR;

	if ((UCI_CONVERTER_LOOKUP_PTR(ctx, &ptr, lookup_path, path, true) != UCI_OK)) {
		ERROR("uci lookup failed!\n");
		_thread_local_uci_put_context(ctx);
		return RETURN_ERR;
	}

	p = ptr.p;
	uci_foreach_element(&p->sections, e) {
		struct uci_element *n;
		s = uci_to_section(e);

		if (sscanf_s(s->e.name, "default_radio%d", &wireless_idx) != 1)
			continue;

		if (!strncmp(s->type, vap_type, MAX_UCI_BUF_LEN)) {
			uci_foreach_element(&s->options, n) {
				struct uci_option *o = uci_to_option(n);

				if (strncmp(n->name, "device", MAX_UCI_BUF_LEN))
					continue;

				if (o->type != UCI_TYPE_STRING)
					continue;

				if (wireless_idx < 0) {
					_thread_local_uci_put_context(ctx);
					return RETURN_ERR;
				}

				if (!strncmp(device, o->v.string, MAX_UCI_BUF_LEN)) {
					int rpc_index = uci_to_rpc_index(TYPE_VAP, wireless_idx);
					if (UCI_INVALID_INDEX == rpc_index) {
						// ERROR("failed getting RPC index from UCI index (%d)\n", wireless_idx);
						break;
					}
					uci_converter_revert_elem(path, TYPE_VAP, rpc_index, NULL);
					break;
				}
			}
		}
	}
	_thread_local_uci_put_context(ctx);
	return RETURN_OK;
}

/**************************************************************************/
/*! \fn int uci_converter_revert(const char* path)
 **************************************************************************
 *  \brief remove uncommited changes for a giver path
 *  \param[in] const char* path - config<.device><.parameter>
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
static int uci_converter_revert(const char* path)
{
	int status;
	struct uci_ptr ptr;
	struct uci_context *cont;
	char lookup_path[MAX_UCI_BUF_LEN] = "";

	if (path == NULL) {
		ERROR("path is NULL!\n");
		return RETURN_ERR;
	}

	cont = _thread_local_uci_get_context();
	if (!cont)
		return RETURN_ERR;

	if (UCI_CONVERTER_LOOKUP_PTR(cont, &ptr, lookup_path, path, true) != UCI_OK) {
        ERROR("lookup failed\n");
		_thread_local_uci_put_context(cont);
		return RETURN_ERR;
	}

	if (uci_revert(cont, &ptr) != UCI_OK) {
        ERROR("revert failed\n");
		_thread_local_uci_put_context(cont);
		return RETURN_ERR;
	}

	status = uci_save(cont, ptr.p);

	_thread_local_uci_put_context(cont);

	return (status == UCI_OK) ? RETURN_OK : RETURN_ERR;
}

/**************************************************************************/
/*! \fn int uci_converter_revert_elem(const char* config, enum paramType type,
 *          int index, const char* param)
 **************************************************************************
 *  \brief remove uncommited changes using the given path
 *  \param[in] const char* config - name of config file (default "wireless")
 *  \param[in] enum paramType type - radio or vap (if someting else, revert the
 *                                   whole config file
 *  \param[in] int index - index of the device (if less the 0, revert the whole
 *                         config file
 *  \param[in] const char* param - parameter name (if not given, revert the whole device)
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int uci_converter_revert_elem(const char* config, enum paramType type, int index, const char* param)
{
	int status, uci_index;
	char path[MAX_UCI_BUF_LEN] = "";
	char device[MAX_UCI_BUF_LEN] = "";

	if (is_empty_str(config)) {
		strcpy_s(path, MAX_UCI_BUF_LEN, "wireless");
	} else {
		strncpy_s(path, MAX_UCI_BUF_LEN, config, strnlen_s(config, MAX_UCI_BUF_LEN - 1));
	}

	if (!_valid_iftype(type))
		goto revert;

	uci_index = rpc_to_uci_index(type, index);
	if (UCI_INVALID_INDEX == uci_index) {
		// ERROR("failed getting UCI index from RPC index (%d)\n", index);
		goto revert;
	}

	status = sprintf_s(device, MAX_UCI_BUF_LEN, "%s%d", get_type_str(type), uci_index);
	if (status <= 0) {
		ERROR("sprintf_s() failed: device, radio_str=%s index=%d\n", get_type_str(type), index);
		return RETURN_ERR;
	}
	strcat_s(path, MAX_UCI_BUF_LEN, ".");
	strcat_s(path, MAX_UCI_BUF_LEN, device);

	if (!is_empty_str(param)) {
		strcat_s(path, MAX_UCI_BUF_LEN, ".");
		strcat_s(path, MAX_UCI_BUF_LEN, param);
	}

revert:
	return uci_converter_revert(path);
}

/**************************************************************************/
/*! \fn int uci_converter_revert_radio(const char* path, int index)
 **************************************************************************
 *  \brief remove uncommited changes for the given radio and all its vaps
 *  \param[in] const char* path - name of the config file
 *  \param[in] int index - index of the radio
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int uci_converter_revert_radio(const char* path, int index)
{
	int status, uci_index;
	char radio[MAX_UCI_BUF_LEN] = "";

	if (path == NULL) {
		ERROR("path is NULL!\n");
		return RETURN_ERR;
	}

	uci_converter_revert_elem(path, TYPE_RADIO, index, NULL);

	uci_index = rpc_to_uci_index(TYPE_RADIO, index);
	if (UCI_INVALID_INDEX == uci_index) {
		// ERROR("failed getting UCI index from RPC index (%d)\n", index);
		return RETURN_ERR;
	}

	status = sprintf_s(radio, MAX_UCI_BUF_LEN, "radio%d", uci_index);
	if (status <= 0) {
		ERROR("sprintf_s() failed: radio, index=%d\n", index);
		return RETURN_ERR;
	}

	return uci_converter_revert_vaps(path, radio);
}

/**************************************************************************/
/*! \fn int uci_converter_commit(const char* path, const char* alt_dir)
 **************************************************************************
 *  \brief commit all the changes made to the UCI database
 *  \param[in] const char* path - name of the config file (for example wireless)
 *  \param[in] const char* alt_dir - alternate directory (default is /etc/config)
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int uci_converter_commit(const char* path, const char* alt_dir)
{
#ifdef CONFIG_RPCD
	(void) alt_dir;
	return _ubus_uci_converter_commit(path);
#else
	int status;
	struct uci_ptr ptr;
	struct uci_context *cont;
	char lookup_path[MAX_UCI_BUF_LEN] = "";

	if (path == NULL) {
		ERROR("path is NULL!\n");
		return RETURN_ERR;
	}

	cont = _thread_local_uci_get_context();
	if (!cont)
		return RETURN_ERR;

	if (alt_dir)
		uci_set_confdir(cont, alt_dir);

	if (UCI_CONVERTER_LOOKUP_PTR(cont, &ptr, lookup_path, path, true) != UCI_OK) {
		_thread_local_uci_put_context(cont);
		return RETURN_ERR;
	}

	status = uci_commit(cont, &ptr.p, false);

	_thread_local_uci_put_context(cont);

	return (status == UCI_OK) ? RETURN_OK : RETURN_ERR;
#endif
}

/**************************************************************************/
/*! \fn int uci_converter_commit_wireless(void)
 **************************************************************************
 *  \brief commit all the changes made to the UCI database in /etc/config/wireless package
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int uci_converter_commit_wireless(void)
{
	return uci_converter_commit("wireless", NULL);
}

/**************************************************************************/
/*! \fn int get_htmode_enum(int radioIndex, enum uci_htmode *uci_htmode) {
 **************************************************************************
 *  \brief get htmode for a specific radio from UCI database
 *  \param[in] int radioIndex - radio index
 *  \param[out] enum uci_htmode *uci_htmode - output the radio htmode
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int get_htmode_enum(int radioIndex, enum uci_htmode *uci_htmode) {
	int status;
	char htmode_str[MAX_UCI_BUF_LEN];

	if (uci_htmode == NULL) {
		ERROR("uci_htmode is NULL!\n");
		return RETURN_ERR;
	}

	*uci_htmode = UCI_HTMODE_LAST;

	status = uci_converter_get_str_ext(TYPE_RADIO, radioIndex, "htmode", htmode_str, sizeof(htmode_str));
	if (status == RETURN_ERR)
		return RETURN_ERR;

	if (!strncmp(htmode_str, "HT20", MAX_UCI_BUF_LEN))
		*uci_htmode = UCI_HTMODE_HT20;
	else if (!strncmp(htmode_str, "HT40+", MAX_UCI_BUF_LEN))
		*uci_htmode = UCI_HTMODE_HT40PLUS;
	else if (!strncmp(htmode_str, "HT40-", MAX_UCI_BUF_LEN))
		*uci_htmode = UCI_HTMODE_HT40MINUS;
	else if (!strncmp(htmode_str, "VHT20", MAX_UCI_BUF_LEN))
		*uci_htmode = UCI_HTMODE_VHT20;
	else if (!strncmp(htmode_str, "VHT40+", MAX_UCI_BUF_LEN))
		*uci_htmode = UCI_HTMODE_VHT40PLUS;
	else if (!strncmp(htmode_str, "VHT40-", MAX_UCI_BUF_LEN))
		*uci_htmode = UCI_HTMODE_VHT40MINUS;
	else if (!strncmp(htmode_str, "VHT80", MAX_UCI_BUF_LEN))
		*uci_htmode = UCI_HTMODE_VHT80;
	else if (!strncmp(htmode_str, "VHT160", MAX_UCI_BUF_LEN))
		*uci_htmode = UCI_HTMODE_VHT160;
	else {
		ERROR("Failed with htmode_str=%s\n", htmode_str);
		return RETURN_ERR;
	}

	return RETURN_OK;
}

/**************************************************************************/
/*! \fn int set_htmode_enum(int radioIndex, enum uci_htmode uci_htmode) {
 **************************************************************************
 *  \brief set htmode for a specific radio
 *  \param[in] int radioIndex - radio index
 *  \param[in] enum uci_htmode uci_htmode - the radio htmode to set
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int set_htmode_enum(int radioIndex, enum uci_htmode uci_htmode)
{
	const char *htmode_str = "";

	switch (uci_htmode) {
		case UCI_HTMODE_HT20:
			htmode_str = "HT20"; break;
		case UCI_HTMODE_HT40PLUS:
			htmode_str = "HT40+"; break;
		case UCI_HTMODE_HT40MINUS:
			htmode_str = "HT40-"; break;
		case UCI_HTMODE_VHT20:
			htmode_str = "VHT20"; break;
		case UCI_HTMODE_VHT40PLUS:
			htmode_str = "VHT40+"; break;
		case UCI_HTMODE_VHT40MINUS:
			htmode_str = "VHT40-"; break;
		case UCI_HTMODE_VHT80:
			htmode_str = "VHT80"; break;
		case UCI_HTMODE_VHT160:
			htmode_str = "VHT160"; break;
		default:
			ERROR("Failed with uci_htmode=%d\n", uci_htmode);
			return RETURN_ERR;
	}

	return uci_converter_set_str(TYPE_RADIO, radioIndex, "htmode", htmode_str);
}

/**************************************************************************/
/*! \fn int get_hwmode_enum(int radioIndex, enum uci_hwmode *uci_hwmode) {
 **************************************************************************
 *  \brief get hwmode for a specific radio from UCI database
 *  \param[in] int radioIndex - radio index
 *  \param[out] enum uci_hwmode *uci_hwmode - output the radio hwmode
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int get_hwmode_enum(int radioIndex, enum uci_hwmode *uci_hwmode) {
	int status;
	char band[MAX_UCI_BUF_LEN];
	char hwmode_str[MAX_UCI_BUF_LEN];

	if (uci_hwmode == NULL) {
		ERROR("uci_hwmode is NULL!\n");
		return RETURN_ERR;
	}

	*uci_hwmode = UCI_HWMODE_LAST;

	status = uci_converter_get_str_ext(TYPE_RADIO, radioIndex, "hwmode", hwmode_str, sizeof(hwmode_str));
	if (status == RETURN_ERR)
		return RETURN_ERR;

	status = uci_converter_get_str_ext(TYPE_RADIO, radioIndex, "band", band, sizeof(band));
	if (status == RETURN_ERR)
		return RETURN_ERR;

	if (!strncmp(hwmode_str, "11b", MAX_UCI_BUF_LEN))
		*uci_hwmode = UCI_HWMODE_11B;
	else if (!strncmp(hwmode_str, "11bg", MAX_UCI_BUF_LEN))
		*uci_hwmode = UCI_HWMODE_11BG;
	else if (!strncmp(hwmode_str, "11g", MAX_UCI_BUF_LEN))
		*uci_hwmode = UCI_HWMODE_11G;
	else if (!strncmp(hwmode_str, "11ng", MAX_UCI_BUF_LEN))
		*uci_hwmode = UCI_HWMODE_11NG;
	else if (!strncmp(hwmode_str, "11bgn", MAX_UCI_BUF_LEN))
		*uci_hwmode = UCI_HWMODE_11BGN;
	else if (!strncmp(hwmode_str, "11bgnac", MAX_UCI_BUF_LEN))
		*uci_hwmode = UCI_HWMODE_11BGNAC;
	else if (!strncmp(hwmode_str, "11bgnax", MAX_UCI_BUF_LEN))
		*uci_hwmode = UCI_HWMODE_11BGNAX;
	else if (!strncmp(hwmode_str, "11gnac", MAX_UCI_BUF_LEN))
		*uci_hwmode = UCI_HWMODE_11GNAC;
	else if (!strncmp(hwmode_str, "11gnax", MAX_UCI_BUF_LEN))
		*uci_hwmode = UCI_HWMODE_11GNAX;
	else if (!strncmp(hwmode_str, "11n", MAX_UCI_BUF_LEN) && !strncmp(band, "2.4G", sizeof("2.4G")-1))
		*uci_hwmode = UCI_HWMODE_11N_24G;
	else if (!strncmp(hwmode_str, "11n", MAX_UCI_BUF_LEN) && !strncmp(band, "5G", sizeof("5G")-1))
		*uci_hwmode = UCI_HWMODE_11N_5G;
	else if (!strncmp(hwmode_str, "11a", MAX_UCI_BUF_LEN))
		*uci_hwmode = UCI_HWMODE_11A;
	else if (!strncmp(hwmode_str, "11an", MAX_UCI_BUF_LEN))
		*uci_hwmode = UCI_HWMODE_11AN;
	else if (!strncmp(hwmode_str, "11nac", MAX_UCI_BUF_LEN))
		*uci_hwmode = UCI_HWMODE_11NAC;
	else if (!strncmp(hwmode_str, "11ac", MAX_UCI_BUF_LEN))
		*uci_hwmode = UCI_HWMODE_11AC;
	else if (!strncmp(hwmode_str, "11anac", MAX_UCI_BUF_LEN))
		*uci_hwmode = UCI_HWMODE_11ANAC;
	else if (!strncmp(hwmode_str, "11anacax", MAX_UCI_BUF_LEN))
		*uci_hwmode = UCI_HWMODE_11ANACAX;
	else if (!strncmp(hwmode_str, "11ax", MAX_UCI_BUF_LEN))
		*uci_hwmode = UCI_HWMODE_11AX;
	else {
		ERROR("Failed with hwmode_str=%s, band=%s\n", hwmode_str, band);
		return RETURN_ERR;
	}

	return RETURN_OK;
}

/**************************************************************************/
/*! \fn int set_hwmode_enum(int radioIndex, enum uci_hwmode uci_hwmode) {
 **************************************************************************
 *  \brief set hwmode for a specific radio
 *  \param[in] int radioIndex - radio index
 *  \param[in] enum uci_hwmode uci_hwmode - the radio hwmode to set
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int set_hwmode_enum(int radioIndex, enum uci_hwmode uci_hwmode)
{
	const char *hwmode_str = "";

	switch (uci_hwmode) {
		case UCI_HWMODE_11B:
			hwmode_str = "11b"; break;
		case UCI_HWMODE_11BG:
			hwmode_str = "11bg"; break;
		case UCI_HWMODE_11G:
			hwmode_str = "11g"; break;
		case UCI_HWMODE_11NG:
			hwmode_str = "11ng"; break;
		case UCI_HWMODE_11BGN:
			hwmode_str = "11bgn"; break;
		case UCI_HWMODE_11BGNAC:
			hwmode_str = "11bgnac"; break;
		case UCI_HWMODE_11BGNAX:
			hwmode_str = "11bgnax"; break;
		case UCI_HWMODE_11GNAC:
			hwmode_str = "11gnac"; break;
		case UCI_HWMODE_11GNAX:
			hwmode_str = "11gnax"; break;
		case UCI_HWMODE_11N_24G:
		case UCI_HWMODE_11N_5G:
			hwmode_str = "11n"; break;
		case UCI_HWMODE_11A:
			hwmode_str = "11a"; break;
		case UCI_HWMODE_11AN:
			hwmode_str = "11an"; break;
		case UCI_HWMODE_11NAC:
			hwmode_str = "11nac"; break;
		case UCI_HWMODE_11AC:
			hwmode_str = "11ac"; break;
		case UCI_HWMODE_11ANAC:
			hwmode_str = "11anac"; break;
		case UCI_HWMODE_11ANACAX:
			hwmode_str = "11anacax"; break;
		case UCI_HWMODE_11AX:
			hwmode_str = "11ax"; break;
		case UCI_HWMODE_11BGNAXBE:
			hwmode_str = "11bgnaxbe"; break;
		case UCI_HWMODE_11ANACAXBE:
			hwmode_str = "11anacaxbe"; break;
		case UCI_HWMODE_11AXBE:
			hwmode_str = "11axbe"; break;
		default:
			ERROR("Failed with uci_hwmode=%d\n", uci_hwmode);
			return RETURN_ERR;
	}

	return uci_converter_set_str(TYPE_RADIO, radioIndex, "hwmode", hwmode_str);
}

/**************************************************************************/
/*! \fn void uci_converter_prepare_for_up(int radio_idx)
 **************************************************************************
 *  \brief prepare uci db to be precessed by netifd
 *  \param[in] int radio_idx - the index of the radio to prepare
 ***************************************************************************/
void uci_converter_prepare_for_up(int radio_idx)
{
	UNUSED_PARAM(radio_idx);
	if (uci_converter_commit_wireless() != RETURN_OK)
		ERROR("uci_converter_commit_wireless failed\n");
}

int uci_converter_netifd_connect(int radio_idx, bool initNetifd, char* command)
{
	(void)(initNetifd);

	uci_converter_prepare_for_up(radio_idx);

	if (uci_converter_system(command) == RETURN_OK) {
		DEBUG("Completed successfully\n");
	} else {
		ERROR("Failed\n");
		return RETURN_ERR;
	}

	return RETURN_OK;
}
#endif /* ONEWIFI_UCI_TRANSITION */ 

//UCI HELPER ENDS
