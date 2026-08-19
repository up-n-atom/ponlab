
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

#include <uci.h>
#include <libubox/blobmsg_json.h>
#include <libubox/uloop.h>
#include <libubus.h>
#include "uci_wrapper.h"
#include "help_logging.h"
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

//Logging
#ifndef LOG_LEVEL
unsigned int LOGLEVEL = SYS_LOG_DEBUG + 1;
#else
unsigned int LOGLEVEL = LOG_LEVEL + 1;
#endif

#ifndef LOG_TYPE
unsigned int LOGTYPE = SYS_LOG_TYPE_CONSOLE | SYS_LOG_TYPE_FILE;
#else
unsigned int LOGTYPE = LOG_TYPE;
#endif

unsigned int LOGPROFILE;
//End Logging

static bool uci_converter_if_ap(struct uci_section *s);

bool is_empty_str(const char *str)
{
	if (str == NULL)
		return true;

	return str[0] == '\0';
}

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


/**************************************************************************/
/*! \fn int uci_converter_set_param_changed(enum paramType type, int index)
 **************************************************************************
 *  \brief set flag that indicates that a change to the entry was made
 *  \param[in] enum paramType ifType: TYPE_RADIO/TYPE_RADIO_VAP/TYPE_VAP
 *  \param[in] int index - The index of the entry
 *  \param[in] bool reconf_val - true = 1, false = 0
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int uci_converter_set_param_changed(enum paramType type, int index, bool reconf_val)
{
	int status;
	char path[MAX_UCI_BUF_LEN] = "";
	char radio_str[] = "default_radio"; /* == TYPE_VAP / TYPE_RADIO_VAP */
	char reconf_off[] = "0";
	char reconf_on[] = "1";

	if (type == TYPE_RADIO)
		strncpy_s(radio_str, sizeof(radio_str), "radio", 5);

	status = sprintf_s(path, MAX_UCI_BUF_LEN, "meta-wireless.%s%d", radio_str, rpc_to_uci_index(type, index));
	if (status <= 0) {
		 ERROR("%s failed sprintf_s.  index=%d reconf\n", __func__, index);
		return RETURN_ERR;
	}

	status = uci_converter_set(path, "param_changed", reconf_val ? reconf_on : reconf_off);
	if (status == RETURN_OK) {
		DEBUG("%s index=%d param_changed=%s\n", __func__, index, reconf_val ? reconf_on : reconf_off);
	} else {
		ERROR("%s failed. index=%d param_changed=%s\n", __func__, index, reconf_val ? reconf_on : reconf_off);
	}

	return status;
}

/**************************************************************************/
/*! \fn int rpc_to_uci_index(enum paramType iftype, int index)
 **************************************************************************
 *  \brief convert rpc index to uci index using uci-db
 *  \param[in] enum paramType ifType: TYPE_RADIO/TYPE_RADIO_VAP/TYPE_VAP
 *  \param[in] int index - The rpc index
 *  \return the index if success, negative if error
 ***************************************************************************/
int rpc_to_uci_index(enum paramType iftype, int index)
{
	char index_path[MAX_UCI_BUF_LEN] = "";
	char index_path_pre[MAX_UCI_BUF_LEN] = "";
	char value_str[MAX_UCI_BUF_LEN] = "";
	int status = 0;
	switch(iftype)
	{
		case TYPE_RADIO:
			status = sprintf_s(index_path_pre, MAX_UCI_BUF_LEN, "wireless.radio_rpc_indexes");
			break;
		case TYPE_RADIO_VAP:
			status = sprintf_s(index_path_pre, MAX_UCI_BUF_LEN, "wireless.radio_vap_rpc_indexes");
			break;
		case TYPE_VAP:
			status = sprintf_s(index_path_pre, MAX_UCI_BUF_LEN, "wireless.vap_rpc_indexes");
			break;
		default:
			ERROR("%s invalid iftype.", __func__);
			return RETURN_ERR;
	}
	if (status <= 0) {
		ERROR("%s failed sprintf_s.", __func__);
		return RETURN_ERR;
	}

	status = sprintf_s(index_path, MAX_UCI_BUF_LEN, "%s.index%d",
			   index_path_pre, index);
	if (status <= 0) {
		ERROR("%s failed sprintf_s.  index=%d", __func__, index);
		return RETURN_ERR;
	}

	status = uci_converter_get(index_path, value_str, MAX_UCI_BUF_LEN);
	if (status == RETURN_ERR)
		return RETURN_ERR;
	return atoi(value_str);
}

/**************************************************************************/
/*! \fn int uci_to_rpc_index(enum paramType iftype, int index)
 **************************************************************************
 *  \brief convert uci index to rpc index using uci-db
 *  \param[in] enum paramType ifType: TYPE_RADIO/TYPE_RADIO_VAP/TYPE_VAP
 *  \param[in] int index - The uci index
 *  \return the index if success, negative if error
 ***************************************************************************/
int uci_to_rpc_index(enum paramType iftype, int index)
{
	char index_path[MAX_UCI_BUF_LEN] = "";
	char index_path_pre[MAX_UCI_BUF_LEN] = "";
	char value_str[MAX_UCI_BUF_LEN] = "";
	int status = 0;
	switch(iftype)
	{
		case TYPE_RADIO:
			status = sprintf_s(index_path_pre, MAX_UCI_BUF_LEN, "wireless.radio");
			break;
		case TYPE_RADIO_VAP:
			/* fall through */
		case TYPE_VAP:
			status = sprintf_s(index_path_pre, MAX_UCI_BUF_LEN, "wireless.default_radio");
			break;
		default:
			ERROR("%s invalid iftype.", __func__);
			return RETURN_ERR;
	}
	if (status <= 0) {
		ERROR("%s failed sprintf_s.", __func__);
		return RETURN_ERR;
	}

	status = sprintf_s(index_path, MAX_UCI_BUF_LEN, "%s%d.rpc_index",
			   index_path_pre, index);
	if (status <= 0) {
		ERROR("%s failed sprintf_s. index=%d", __func__, index);
		return RETURN_ERR;
	}

	status = uci_converter_get(index_path, value_str, MAX_UCI_BUF_LEN);
	if (status == RETURN_ERR)
		return RETURN_ERR;
	return atoi(value_str);
}

/**************************************************************************/
/*! \fn int dummy_to_radio_index(enum paramType iftype, int index)
 **************************************************************************
 *  \brief convert dummy vap uci index to radio uci index using uci-db
 *  \param[in] int index - The uci index of the dummy vap
 *  \return the index if success, negative if error
 ***************************************************************************/
static int dummy_to_radio_index(int index)
{
	char radio[MAX_UCI_BUF_LEN] = "";
	if (uci_converter_get_str(TYPE_RADIO_VAP, uci_to_rpc_index(TYPE_RADIO_VAP, index), "device", radio) == RETURN_ERR)
		return RETURN_ERR;

	return (radio[sizeof("radio") -1] - '0');

}

/**************************************************************************/
/*! \fn int uci_converter_rewrite_param(char* device, int uci_index, const char* param)
 **************************************************************************
 *  \brief write a parameter from meta db to regular db
 *  \param[in] char* device - radio or default_radio
 *  \param[in] int uci_index - uci index
 *  \param[in] const char* param - parameter to rewrite
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
static int uci_converter_rewrite_param(const char* device, int uci_index, const char* param)
{
	int status;
	char value_str[MAX_UCI_BUF_LEN] = "";
	char meta_path[MAX_UCI_BUF_LEN] = "";
	char path[MAX_UCI_BUF_LEN] = "";

	status = sprintf_s(meta_path, MAX_UCI_BUF_LEN, "meta-wireless.%s%d.%s",
		device, uci_index, param);
	if (status <= 0) {
		ERROR("%s failed sprintf_s.  index=%d param=%s",
			__func__, uci_index, param);
		return RETURN_ERR;
	}

	status = sprintf_s(path, MAX_UCI_BUF_LEN, "wireless.%s%d",
		device, uci_index);
	if (status <= 0) {
		ERROR("%s failed sprintf_s.  index=%d param=%s",
			__func__, uci_index, param);
		return RETURN_ERR;
	}

	status = uci_converter_get(meta_path, value_str, MAX_UCI_BUF_LEN);
	if (status == RETURN_ERR)
		strncpy_s(value_str, MAX_UCI_BUF_LEN, "0", 1);

	return uci_converter_set(path, param, value_str);
}

/**************************************************************************/
/*! \fn int uci_converter_rewrite_vaps(char* vap_radio, char* param)
 **************************************************************************
 *  \brief write a parameter for all vaps from meta db to regular db
 *  \param[in] char* vap_radio - name of device vaps belong to
 *  \param[in] char* param - parameter to rewrite
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
static int uci_converter_rewrite_vaps(char* vap_radio, char* param)
{
	int status;
	int wireless_idx;
	struct uci_ptr ptr;
	struct uci_context *ctx;
	struct uci_package *p = NULL;
	struct uci_element *e;
	struct uci_section *s;
	char curr_device[MAX_UCI_BUF_LEN] = "";
	char lookup_str[MAX_UCI_BUF_LEN] = "meta-wireless";
	char meta_path[MAX_UCI_BUF_LEN] = "";
	char vap_type[MAX_UCI_BUF_LEN] = "wifi-iface";

	ctx = uci_alloc_context();
	if (!ctx)
		return RETURN_ERR;

	if ((uci_lookup_ptr(ctx, &ptr, lookup_str, true) != UCI_OK)) {
		ERROR("%s, uci lookup failed!\n", __FUNCTION__);
		uci_free_context(ctx);
		return RETURN_ERR;
	}

	p = ptr.p;
	uci_foreach_element(&p->sections, e) {
		s = uci_to_section(e);

		if (!strncmp(s->type, vap_type, MAX_UCI_BUF_LEN)) {
			if (sscanf_s(s->e.name, "default_radio%d", &wireless_idx) == 1) {
				status = sprintf_s(meta_path, MAX_UCI_BUF_LEN,
					"meta-wireless.default_radio%d.device", wireless_idx);
				if (status <= 0) {
					ERROR("%s failed sprintf_s.  index=%d param=device",
						__func__, wireless_idx);
					break;
				}

				status = uci_converter_get(meta_path, curr_device, MAX_UCI_BUF_LEN);
				if (status == RETURN_ERR)
					continue;

				if (strncmp(curr_device, vap_radio, MAX_UCI_BUF_LEN))
					continue;

				uci_converter_rewrite_param("default_radio", wireless_idx, param);
			}
		}
	}
	uci_free_context(ctx);
	return RETURN_OK;
}

/**************************************************************************/
/*! \fn int uci_converter_add_meta_wireless(int radio_idx, int vap_idx)
 **************************************************************************
 *  \brief add meta data for a vap
 *  \param[in] int radio_idx - uci radio index
 *  \param[in] int vap_idx - uci default_radio index
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int uci_converter_add_meta_wireless(int radio_idx, int vap_idx)
{
	int status;
	char path[MAX_UCI_BUF_LEN] = "";
	char vap_name[MAX_UCI_BUF_LEN] = "";
	char radio[MAX_UCI_BUF_LEN] = "";
	char change_value[] = "0";

	status = sprintf_s(vap_name, MAX_UCI_BUF_LEN, "default_radio%d", vap_idx);
	if (status <= 0) {
        ERROR("%s sprintf_s failed, vap index: %d\n", __FUNCTION__, vap_idx);
        return RETURN_ERR;
    }

	status = uci_converter_add_device("meta-wireless", vap_name, "wifi-iface");
	if (status == RETURN_ERR) {
		ERROR("%s creating new vap %d failed\n", __FUNCTION__, vap_idx);
		return RETURN_ERR;
	}

	status = sprintf_s(path, MAX_UCI_BUF_LEN, "meta-wireless.default_radio%d", vap_idx);
	if (status <= 0) {
		ERROR("%s path error, vap index: %d\n", __FUNCTION__, vap_idx);
		return RETURN_ERR;
	}

	status = sprintf_s(radio, MAX_UCI_BUF_LEN, "radio%d", radio_idx);
	if (status <= 0) {
		ERROR("%s device name error, radio index: %d\n", __FUNCTION__, radio_idx);
		return RETURN_ERR;
	}

	status = uci_converter_set(path, "device", radio);
	if (status == RETURN_ERR) {
		ERROR("%s setting device in DB failed\n", __FUNCTION__);
		return RETURN_ERR;
	}

	status = sprintf_s(path, MAX_UCI_BUF_LEN, "meta-wireless.default_radio%d", vap_idx);
	if (status <= 0) {
		ERROR("%s path error, vap index: %d\n", __FUNCTION__, vap_idx);
		return RETURN_ERR;
	}

	status = uci_converter_set(path, "param_changed", change_value);
	if (status == RETURN_ERR) {
		ERROR("%s setting device in DB failed\n", __FUNCTION__);
		return RETURN_ERR;
	}

	return RETURN_OK;
}

/**************************************************************************/
/*! \fn int uci_converter_reset_param_changed(int radio_index)
 **************************************************************************
 *  \brief set change flags to 0 for a specifiac radio in meta db
 *  \param[in] int radio_index - radio index
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int uci_converter_reset_param_changed(int radio_index)
{
	int status;
	int wireless_idx;
	struct uci_ptr ptr;
	struct uci_context *ctx;
	struct uci_package *p = NULL;
	struct uci_element *e;
	struct uci_section *s;
	char lookup_str[MAX_UCI_BUF_LEN] = "meta-wireless";
	char radio_type[MAX_UCI_BUF_LEN] = "wifi-device";
	char vap_type[MAX_UCI_BUF_LEN] = "wifi-iface";
	char vap_radio_option[] = "device";
	char radio_name[MAX_UCI_BUF_LEN] = "";
	char radio_path[MAX_UCI_BUF_LEN] = "";
	char vap_path[MAX_UCI_BUF_LEN] = "";
	char reset_value[] = "0";
	int uci_radio_index = rpc_to_uci_index(TYPE_RADIO, radio_index);

	ctx = uci_alloc_context();
	if (!ctx)
		return RETURN_ERR;

	if ((uci_lookup_ptr(ctx, &ptr, lookup_str, true) != UCI_OK)) {
		ERROR("%s, uci lookup failed!\n", __FUNCTION__);
		uci_free_context(ctx);
		return RETURN_ERR;
	}

	status = sprintf_s(radio_name, MAX_UCI_BUF_LEN, "radio%d", uci_radio_index);
	if (status <= 0) {
		ERROR("%s sprintf_s : radio_name error, radio index: %d\n", __FUNCTION__, uci_radio_index);
		uci_free_context(ctx);
		return RETURN_ERR;
	}

	p = ptr.p;
	uci_foreach_element(&p->sections, e) {
		struct uci_element *n;
		s = uci_to_section(e);

		if (!strncmp(s->type, radio_type, MAX_UCI_BUF_LEN)) {
			if (!strncmp(s->e.name, radio_name, MAX_UCI_BUF_LEN)) {
				status = sprintf_s(radio_path, MAX_UCI_BUF_LEN, "meta-wireless.%s", radio_name);
				if (status <= 0) {
					ERROR("%s sprintf_s failed: radio_path error, radio name: %s\n", __FUNCTION__, radio_name);
					uci_free_context(ctx);
					return RETURN_ERR;
				}
				uci_converter_set(radio_path, "param_changed", reset_value);
				status = sprintf_s(radio_path, MAX_UCI_BUF_LEN, "meta-wireless.%s", radio_name);
				if (status <= 0) {
					ERROR("%s sprintf_s failed: radio_path error, radio name: %s\n", __FUNCTION__, radio_name);
					uci_free_context(ctx);
					return RETURN_ERR;
				}
				uci_converter_set(radio_path, "interface_changed", reset_value);
				continue;
			} else {
				continue;
			}
		}

		if (!strncmp(s->type, vap_type, MAX_UCI_BUF_LEN)) {
			if (sscanf_s(s->e.name, "default_radio%d", &wireless_idx) == 1) {
				if (uci_converter_is_dummy(wireless_idx))
					continue;
			}

			uci_foreach_element(&s->options, n) {
				struct uci_option *o = uci_to_option(n);

				if (strncmp(n->name, vap_radio_option, MAX_UCI_BUF_LEN))
					continue;

				if (o->type != UCI_TYPE_STRING)
					continue;

				if (!strncmp(radio_name, o->v.string, MAX_UCI_BUF_LEN)) {
					status = sprintf_s(vap_path, MAX_UCI_BUF_LEN, "meta-wireless.%s", s->e.name);
					if (status <= 0) {
						ERROR("%s sprintf_s failed: vap_path error, e.name: %s\n", __FUNCTION__, s->e.name);
						uci_free_context(ctx);
						return RETURN_ERR;
					}
					uci_converter_set(vap_path, "param_changed", reset_value);
					break;
				} else {
					break;
				}
			}
		}
	}

	uci_free_context(ctx);
	return RETURN_OK;
}

/**************************************************************************/
/*! \fn int uci_converter_clean_param_changed(int radio_index)
 **************************************************************************
 *  \brief remove change flags for a specifiac radio
 *  \param[in] int radio_index - radio index
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int uci_converter_clean_param_changed(int radio_index)
{
	int status;
	struct uci_ptr ptr;
	struct uci_context *ctx;
	struct uci_package *p = NULL;
	struct uci_element *e;
	struct uci_section *s;
	char lookup_str[MAX_UCI_BUF_LEN] = "wireless";
	char radio_type[MAX_UCI_BUF_LEN] = "wifi-device";
	char vap_type[MAX_UCI_BUF_LEN] = "wifi-iface";
	char vap_radio_option[] = "device";
	char radio_name[MAX_UCI_BUF_LEN] = "";
	char radio_path[MAX_UCI_BUF_LEN] = "";
	char vap_path[MAX_UCI_BUF_LEN] = "";
	char radio_param_changed[] = "param_changed";
	char radio_interface_changed[] = "interface_changed";
	char vap_param_changed[] = "param_changed";
	int uci_radio_index = rpc_to_uci_index(TYPE_RADIO, radio_index);

	ctx = uci_alloc_context();
	if (!ctx)
		return RETURN_ERR;

	if ((uci_lookup_ptr(ctx, &ptr, lookup_str, true) != UCI_OK)) {
		ERROR("%s, uci lookup failed!\n", __FUNCTION__);
		uci_free_context(ctx);
		return RETURN_ERR;
	}

	status = sprintf_s(radio_name, MAX_UCI_BUF_LEN, "radio%d", uci_radio_index);
	if (status <= 0) {
		ERROR("%s sprintf_s failed: radio error, uci_radio_index: %d\n", __FUNCTION__, uci_radio_index);
		uci_free_context(ctx);
		return RETURN_ERR;
	}

	p = ptr.p;
	uci_foreach_element(&p->sections, e) {
		struct uci_element *n;

		s = uci_to_section(e);

		if (!strncmp(s->type, radio_type, MAX_UCI_BUF_LEN)) {
			if (!strncmp(s->e.name, radio_name, MAX_UCI_BUF_LEN)) {
				status = sprintf_s(radio_path, MAX_UCI_BUF_LEN, "wireless.%s.%s",
					radio_name, radio_param_changed);
					if (status <= 0) {
						ERROR("%s sprintf_s failed: radio path error, radio_name: %s radio_param_changed %s\n", __FUNCTION__,
										radio_name, radio_param_changed);
						uci_free_context(ctx);
						return RETURN_ERR;
					}
				if (uci_converter_del_elem(radio_path) != RETURN_OK)
					ERROR("%s, uci del failed for %s %s!\n",
						__FUNCTION__, radio_path, radio_param_changed);

				status = sprintf_s(radio_path, MAX_UCI_BUF_LEN, "wireless.%s.%s",
					radio_name, radio_interface_changed);
					if (status <= 0) {
						ERROR("%s sprintf_s failed: radio path error, radio_name: %s radio_param_changed %s\n", __FUNCTION__,
										radio_name, radio_param_changed);
						uci_free_context(ctx);
						return RETURN_ERR;
					}
				if (uci_converter_del_elem(radio_path) != RETURN_OK)
					ERROR("%s, uci del failed for %s %s!\n",
						__FUNCTION__, radio_path, radio_interface_changed);

				continue;
			} else {
				continue;
			}
		}

		if (!strncmp(s->type, vap_type, MAX_UCI_BUF_LEN)) {
			uci_foreach_element(&s->options, n) {
				struct uci_option *o = uci_to_option(n);

				if (strncmp(n->name, vap_radio_option, MAX_UCI_BUF_LEN))
					continue;

				if (o->type != UCI_TYPE_STRING)
					continue;

				if (!strncmp(radio_name, o->v.string, MAX_UCI_BUF_LEN)) {
					status = sprintf_s(vap_path, MAX_UCI_BUF_LEN, "wireless.%s.%s",
						s->e.name, vap_param_changed);
					if (status <= 0) {
						ERROR("%s sprintf_s failed: vap path error, e.name: %s vap_param_changed %s\n", __FUNCTION__,
										s->e.name, vap_param_changed);
						uci_free_context(ctx);
						return RETURN_ERR;
					}
					if (uci_converter_del_elem(vap_path) != RETURN_OK)
						ERROR("%s, uci del failed for %s %s!\n",
							__FUNCTION__, vap_path, vap_param_changed);
					break;
				} else {
					break;
				}
			}
		}
	}

	uci_free_context(ctx);
	return RETURN_OK;
}


/**************************************************************************/
/*! \fn void uci_converter_rewrite_param_changed(int radio_index)
 **************************************************************************
 *  \brief write change flags from meta db to regular db
 *  \param[in] int radio_index - radio index
 *  \return no
 ***************************************************************************/
void uci_converter_rewrite_param_changed(int radio_index)
{
	int status;
	char vap_radio[MAX_UCI_BUF_LEN] = "";
	int uci_radio_idx = rpc_to_uci_index(TYPE_RADIO, radio_index);

	uci_converter_rewrite_param("radio", uci_radio_idx, "param_changed");
	uci_converter_rewrite_param("radio", uci_radio_idx, "interface_changed");

	status = sprintf_s(vap_radio, MAX_UCI_BUF_LEN, "radio%d", uci_radio_idx);
	if (status <= 0) {
		ERROR("%s failed snprinf", __func__);
		return;
	}

	uci_converter_rewrite_vaps(vap_radio, "param_changed");
	return;
}


static int uci_get_existing_interfaces_imp(const char *radioName, int *ifacesArr, const unsigned int ifacesArrSize, unsigned int *numOfIfFound)
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
		ERROR("%s, ifacesArr or numOfIfFound is NULL!\n", __FUNCTION__);
		return RETURN_ERR;
	}

	*numOfIfFound = 0;

	ctx = uci_alloc_context();
	if (!ctx)
		return RETURN_ERR;

	if ((uci_lookup_ptr(ctx, &ptr, lookup_str, true) != UCI_OK)) {
		ERROR("%s, uci lookup failed!\n", __FUNCTION__);
		uci_free_context(ctx);
		return RETURN_ERR;
	}

	p = ptr.p;

	uci_foreach_element(&p->sections, e) {
		struct uci_element *n = 0;

		s = uci_to_section(e);

		if (strncmp(s->type, "wifi-iface", MAX_UCI_BUF_LEN))
			continue;

		if(uci_converter_if_ap(s) == false)
			continue;

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
					uci_free_context(ctx);
					return RETURN_ERR;
				}

				ifacesArr[*numOfIfFound] = index_read;
				(*numOfIfFound)++;
			}
		}
	}

	uci_free_context(ctx);
	return RETURN_OK;
}

/**************************************************************************/
/*! \fn int uci_get_existing_interfaces(int *ifacesArr, const unsigned int ifacesArrSize)
 **************************************************************************
 *  \brief return an array containing the indexes of all interfaces
 *  \param[out] int *ifacesArr - buffer to contain the ouput array
 *  \param[in] const unsigned int ifacesArrSize - max size of ifacesArr
 *	\param[out] int *numOfIfFound - the number of interfaces found
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int uci_get_existing_interfaces(int *ifacesArr, const unsigned int ifacesArrSize, unsigned int *numOfIfFound)
{
	return uci_get_existing_interfaces_imp(NULL, ifacesArr, ifacesArrSize, numOfIfFound);
}

/**************************************************************************/
/*! \fn int uci_get_existing_interfaces_in_radio(int *ifacesArr, const unsigned int ifacesArrSize)
 **************************************************************************
 *  \brief return an array containing the indexes of all interfaces in the current radio
 *  \param[in] const char *radio - radio name
 *  \param[out] int *ifacesArr - buffer to contain the ouput array
 *  \param[in] const unsigned int ifacesArrSize - max size of ifacesArr
 *	\param[out] int *numOfIfFound - the number of interfaces found
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int uci_get_existing_interfaces_in_radio(const char *radio, int *ifacesArr, const unsigned int ifacesArrSize, unsigned int *numOfIfFound)
{
	return uci_get_existing_interfaces_imp(radio, ifacesArr, ifacesArrSize, numOfIfFound);
}



/**************************************************************************/
/*! \fn int uci_get_param_change(enum paramType ifType, int index, const char *paramName, char* paramChange)
 **************************************************************************
 *  \brief get the new value of the parameter as it will be pushed in the new uci commit
 *  \param[in] enum paramType ifType: TYPE_RADIO/TYPE_VAP
 *  \param[in] int index - The index of the entry
 *  \param[in] const char *paramName - parameter name to get the value of
 *  \param[out] char* paramChange - the new parameter value if a new value is found
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int uci_get_param_change(enum paramType ifType, int index, const char *paramName, char* paramChange)
{
	struct uci_ptr ptr = {0};
	struct uci_element *e = NULL;
	struct uci_context *ctx = NULL;
	int status = 0;
	char path[MAX_UCI_BUF_LEN] = "";
	char radio_str[] = "default_radio"; /* == TYPE_VAP */

	if ((paramName == NULL) || (paramChange == NULL)) {
		ERROR("%s, paramName or paramChange is NULL!\n", __FUNCTION__);
		return RETURN_ERR;
	}

	if (ifType != TYPE_VAP && ifType != TYPE_RADIO) {
		ERROR("%s, ifType %d is not supported!\n", __FUNCTION__, ifType);
		return RETURN_ERR;
	}

	if (ifType == TYPE_RADIO)
		strncpy_s(radio_str, sizeof(radio_str), "radio", 5);

	status = sprintf_s(path, MAX_UCI_BUF_LEN, "wireless.%s%d", radio_str, rpc_to_uci_index(ifType, index));
	if (status <= 0) {
		ERROR("%s failed sprintf_s.  index=%d\n", __func__, index);
		return RETURN_ERR;
	}

	ctx = uci_alloc_context();
	if (!ctx)
		return RETURN_ERR;

	if ((uci_lookup_ptr(ctx, &ptr, path, true) != UCI_OK)) {
		ERROR("%s, uci lookup failed!\n", __FUNCTION__);
		uci_free_context(ctx);
		return RETURN_ERR;
	}

	if (ptr.p == NULL) {
		ERROR("%s, uci lookup returned a NULL ptr in ptr.p!\n", __FUNCTION__);
		uci_free_context(ctx);
		return RETURN_ERR;
	}

	uci_foreach_element(&ptr.p->saved_delta, e) {
		struct uci_delta *h = uci_to_delta(e);
		if (h && !strncmp(e->name, paramName, strnlen_s(paramName, MAX_UCI_BUF_LEN))) {
			status = sprintf_s(path, MAX_UCI_BUF_LEN, "%s%d", radio_str, rpc_to_uci_index(ifType, index));
			if (!strncmp(h->section, path, strnlen_s(path, MAX_UCI_BUF_LEN))) {
				size_t len = strnlen_s(h->value, MAX_UCI_BUF_LEN - 1);
				strncpy_s(paramChange, len + 1, h->value, len);
				uci_free_context(ctx);
				return RETURN_OK;
			}
		}
	}

	uci_free_context(ctx);
	return RETURN_OK;
}

/**************************************************************************/
/*! \fn int uci_converter_set_interface_changed(int radio_index)
 **************************************************************************
 *  \brief set flag that indicates that the number of VAPs has been changed
 *  \param[in] int radio_index - The index of the radio entry
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int uci_converter_set_interface_changed(int radio_index)
{
	int status;
	char path[MAX_UCI_BUF_LEN] = "";
	char interface_enable[] = "1"; /* always enable */

	status = sprintf_s(path, MAX_UCI_BUF_LEN, "meta-wireless.radio%d", rpc_to_uci_index(TYPE_RADIO, radio_index));
	if (status <= 0) {
		ERROR("%s failed sprintf_s.  index=%d\n", __func__, radio_index);
		return RETURN_ERR;
	}

	status = uci_converter_set(path, "interface_changed", interface_enable);
	if (status == RETURN_OK) {
		DEBUG("%s index=%d\n", __func__, radio_index);
	} else {
		ERROR("%s failed. index=%d\n", __func__, radio_index);
	}

	return status;
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

	status = sprintf_s(path, MAX_UCI_BUF_LEN, "wireless.radio_vap_rpc_indexes.index%d", uci_to_rpc_index(TYPE_RADIO_VAP, uci_index));
	if (status <= 0) {
		ERROR("%s failed sprintf_s.  index=%d\n", __func__, uci_index);
		return false;
	}

	status = uci_converter_get(path, uci_ret, MAX_UCI_BUF_LEN);
	if ((status == RETURN_OK) && (atoi(uci_ret) == uci_index))
		return true;
	return false;
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
int uci_converter_count_elems(const char *sec_type, const char *sec_name,
		const char* opt_name, const char* opt_val,int *num)
{
	int wireless_idx;
	struct uci_ptr ptr;
	struct uci_context *ctx;
	struct uci_package *p = NULL;
	struct uci_element *e;
	struct uci_section *s;
	char lookup_str[MAX_UCI_BUF_LEN] = "wireless";

	ctx = uci_alloc_context();
	if (!ctx)
		return RETURN_ERR;

	if ((uci_lookup_ptr(ctx, &ptr, lookup_str, true) != UCI_OK)) {
		ERROR("%s, uci lookup failed!\n", __FUNCTION__);
		uci_free_context(ctx);
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

		if (!strncmp(sec_type, "wifi-iface", MAX_UCI_BUF_LEN) && !uci_converter_if_ap(s))
			continue;

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

	uci_free_context(ctx);

	return RETURN_OK;
}

/**************************************************************************/
/*! \fn int uci_getIndexFromInterface(char *interfaceName, int *rpc_index)
 **************************************************************************
 *  \brief get UCI index from the interface name
 *  \param[in] char *interfaceName - interface name string
 *  \param[out] int *rpc_index - output the UCI index
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int uci_getIndexFromInterface(char *interfaceName, int *rpc_index)
{
	struct uci_ptr ptr;
	struct uci_context *ctx;
	struct uci_package *p = NULL;
	struct uci_element *e;
	struct uci_section *s;
	char lookup_str[MAX_UCI_BUF_LEN] = "wireless";

	if ((interfaceName == NULL) || (rpc_index == NULL)) {
		ERROR("%s, interfaceName or rpc_index is NULL!\n", __FUNCTION__);
		return RETURN_ERR;
	}

	ctx = uci_alloc_context();
	if (!ctx)
		return RETURN_ERR;

	if ((uci_lookup_ptr(ctx, &ptr, lookup_str, true) != UCI_OK)) {
		ERROR("%s, uci lookup failed!\n", __FUNCTION__);
		uci_free_context(ctx);
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
				uci_free_context(ctx);
				return RETURN_ERR;
			}
			/* if it is dummy we want the radio index, not the dummy one */
			if (uci_converter_is_dummy(*rpc_index)) {
				*rpc_index = dummy_to_radio_index(*rpc_index);
				if (*rpc_index == RETURN_ERR) {
					uci_free_context(ctx);
					return RETURN_ERR;
				}
			}

			uci_free_context(ctx);
			return RETURN_OK;
		}
	}
	*rpc_index = -1;
	uci_free_context(ctx);
	return RETURN_ERR;

}

/**************************************************************************/
/*! \fn int uci_getIndexFromBssid(char *bssid, int *rpc_index)
 **************************************************************************
 *  \brief get UCI index from the bssid
 *  \param[in] char *bssid - bssid string
 *  \param[out] int *rpc_index - output the UCI index
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int uci_getIndexFromBssid(char *bssid, int *rpc_index)
{
	struct uci_ptr ptr;
	struct uci_context *ctx;
	struct uci_package *p = NULL;
	struct uci_element *e;
	struct uci_section *s;
	char lookup_str[MAX_UCI_BUF_LEN] = "wireless";

	if ((bssid == NULL) || (rpc_index == NULL)) {
		ERROR("%s, bssid or rpc_index is NULL!\n", __FUNCTION__);
		return RETURN_ERR;
	}

	ctx = uci_alloc_context();
	if (!ctx)
		return RETURN_ERR;

	if ((uci_lookup_ptr(ctx, &ptr, lookup_str, true) != UCI_OK)) {
		ERROR("%s, uci lookup failed!\n", __FUNCTION__);
		uci_free_context(ctx);
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
				uci_free_context(ctx);
				return RETURN_ERR;
			}
			/* if it is dummy we want the radio index, not the dummy one */
			if (uci_converter_is_dummy(*rpc_index)) {
				*rpc_index = dummy_to_radio_index(*rpc_index);
				if (*rpc_index == RETURN_ERR) {
					uci_free_context(ctx);
					return RETURN_ERR;
				}
			}

			uci_free_context(ctx);
			return RETURN_OK;
		}
	}
	*rpc_index = -1;
	uci_free_context(ctx);
	return RETURN_ERR_NOT_FOUND;

}

/**************************************************************************/
/*! \fn int uci_converter_system(char *cmd)
 **************************************************************************
 *  \brief run command using system - not recommended to use (plus - need root permissions!)
 *  \param[in] char *cmd - command string to run via system
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int uci_converter_system(char *cmd)
{
	FILE *fp = NULL;
	int st;

	if (cmd == NULL) {
		ERROR("%s, cmd is NULL!\n", __FUNCTION__);
		return RETURN_ERR;
	}

	fp = popen(cmd, "r");

	if (fp == NULL)
		return RETURN_ERR;

	st = pclose(fp);
	if (WIFEXITED(st)) {
		if (WEXITSTATUS(st) == 0) {
			return RETURN_OK;
		} else {
			ERROR("%s cmd: %s exit code: %i\n", __FUNCTION__, cmd, WEXITSTATUS(st));
			return RETURN_ERR;
		}
	}

	//Returning SUCCESS here, because Ccsp daemon always returns -1 for child termination.
	return RETURN_OK;
}

/**************************************************************************/
/*! \fn int uci_converter_popen(char *cmd, char *output, unsigned int outputSize)
 **************************************************************************
 *  \brief run command using system, saving result - not recommended to use (plus - need root permissions!)
 *  \param[in] char *cmd - command string to run via system
 *  \param[out] char *output - save result
 *  \param[in] unsigned int outputSize - size of output buffer
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int uci_converter_popen(char *cmd, char *output, unsigned int outputSize)
{
	FILE *fp = NULL;
	int st;

	if ((cmd == NULL) || (output == NULL)) {
		ERROR("%s, cmd or output is NULL!\n", __FUNCTION__);
		return RETURN_ERR;
	}

	fp = popen(cmd, "r");

	if (fp == NULL)
		return RETURN_ERR;

	if (fgets(output, outputSize, fp) == NULL)
		output[0]='\0';

	st = pclose(fp);
	if (WIFEXITED(st)) {
		DEBUG("popen: cmd: %s exit code: %i\n", cmd, WEXITSTATUS(st));
	}

	/* Remove trailing new line character returned from fgets() */
	if (output[strnlen_s(output, outputSize)-1] == '\n')
		output[strnlen_s(output, outputSize)-1] = '\0';

	return RETURN_OK;
}

/**************************************************************************/
/*! \fn int uci_converter_get(char* path, char* value, size_t length)
 **************************************************************************
 *  \brief generic get from UCI database
 *  \param[in] char *path - variable full name to get (config file.device name.option) for example wireless.default_radio45.wps_state
 *  \param[out] char *value - value from UCI DB 
 *  \param[in] size_t length - size of value buffer
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int uci_converter_get(char* path, char* value, size_t length)
{
	struct uci_ptr ptr;
	struct uci_context *cont = uci_alloc_context();

	if (!cont)
		return RETURN_ERR;

	if ((path == NULL) || (value == NULL)) {
		ERROR("%s, path or value is NULL!\n", __FUNCTION__);
		uci_free_context(cont);
		return RETURN_ERR;
	}

	if (uci_lookup_ptr(cont, &ptr, path, true) != UCI_OK || !ptr.o) {
		uci_free_context(cont);
		return RETURN_ERR;
	}

	strncpy_s(value, length, ptr.o->v.string, strnlen_s(ptr.o->v.string, length - 1));

	uci_free_context(cont);

	return RETURN_OK;
}

/**************************************************************************/
/*! \fn int uci_converter_get_str(enum paramType type, int index, const char param[], char *value)
 **************************************************************************
 *  \brief get a specific radio or VAP param string from UCI database
 *  \param[in] enum paramType type - TYPE_RADIO / TYPE_RADIO_VAP / TYPE_VAP
 *  \param[in] int index - radio or VAP index (regular index, not UCI index)
 *  \param[in] const char param[] - param name to get (not full path)
 *  \param[out] char *value - string value from UCI DB 
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int uci_converter_get_str(enum paramType type, int index, const char param[], char *value)
{
	int status;
	char path[MAX_UCI_BUF_LEN] = "";
	char radio_str[] = "default_radio"; /* == TYPE_VAP / TYPE_RADIO_VAP */

	if ((param == NULL) || (value == NULL)) {
		ERROR("%s, param or value is NULL!\n", __FUNCTION__);
		return RETURN_ERR;
	}

	if (type == TYPE_RADIO)
		strncpy_s(radio_str, sizeof(radio_str), "radio", 5);

	status = sprintf_s(path, MAX_UCI_BUF_LEN, "wireless.%s%d.%s", radio_str, rpc_to_uci_index(type, index), param);
	if (status <= 0) {
		ERROR("%s failed sprintf_s.  index=%d param=%s\n", __func__, index, param);
		return RETURN_ERR;
	}

	status = uci_converter_get(path, value, MAX_UCI_BUF_LEN);
	if (status == RETURN_OK) {
		DEBUG("%s index=%d %s=%s\n", __func__, index, param, value);
	} else {
		INFO("%s option N/A. index=%d param=%s\n", __func__, index, param);
	}

	return status;
}

/**************************************************************************/
/*! \fn void uci_converter_get_optional_str(enum paramType type, int index, const char param[], char *value, char* default_val)
 **************************************************************************
 *  \brief get a specific radio or VAP param string from UCI database, if not found return a default value
 *  \param[in] enum paramType type - TYPE_RADIO / TYPE_RADIO_VAP / TYPE_VAP
 *  \param[in] int index - radio or VAP index (regular index, not UCI index)
 *  \param[in] const char param[] - param name to get (not full path)
 *  \param[out] char *value - string value from UCI DB 
 *  \param[in] size_t *size - size of value buffer
 *  \param[in] char *default_val - in case not found return this value as default
 ***************************************************************************/
void uci_converter_get_optional_str(enum paramType type, int index, const char param[], char *value, size_t size, char* default_val)
{
	int status;

	if ((param == NULL) || (value == NULL) || (default_val == NULL)) {
		ERROR("%s, param or value or default_val is NULL!\n", __FUNCTION__);
		return;
	}

	status = uci_converter_get_str(type, index, param, value);
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
		ERROR("%s, param or value is NULL!\n", __FUNCTION__);
		return RETURN_ERR;
	}

	status = uci_converter_get_str(type, index, param, val);
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
		ERROR("%s, param or value is NULL!\n", __FUNCTION__);
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
		ERROR("%s, param or value is NULL!\n", __FUNCTION__);
		return RETURN_ERR;
	}

	status = uci_converter_get_str(type, index, param, val);
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
		ERROR("%s, param or value is NULL!\n", __FUNCTION__);
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
		ERROR("%s, param or value is NULL!\n", __FUNCTION__);
		return RETURN_ERR;
	}

	status = uci_converter_get_str(type, index, param, val);
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
		ERROR("%s, param or value is NULL!\n", __FUNCTION__);
		return;
	}

	status = uci_converter_get_ulong(type, index, param, value);
	if (status == RETURN_ERR)
		*value = default_val;
}

int uci_converter_get_ushort(enum paramType type, int index, const char param[], unsigned short *value)
{
	int status;
	char val[MAX_UCI_BUF_LEN] = "";

	if ((param == NULL) || (value == NULL)) {
		ERROR("%s, param or value is NULL!\n", __FUNCTION__);
		return RETURN_ERR;
	}

	status = uci_converter_get_str(type, index, param, val);
	if (status == RETURN_ERR)
		return RETURN_ERR;

	status = sscanf_s(val, "%u", value);
	if (status < 1)
		return RETURN_ERR;

	return RETURN_OK;
}

void uci_converter_get_optional_ushort(enum paramType type, int index, const char param[], unsigned short *value, unsigned short default_val)
{
	int status;

	if ((param == NULL) || (value == NULL)) {
		ERROR("%s, param or value is NULL!\n", __FUNCTION__);
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
	int res;

	if ((param == NULL) || (value == NULL)) {
		ERROR("%s, param or value is NULL!\n", __FUNCTION__);
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
		ERROR("%s, param or value is NULL!\n", __FUNCTION__);
		return;
	}

	status = uci_converter_get_bool(type, index, param, value);
	if (status == RETURN_ERR)
		*value = default_val;
}

#ifdef CONFIG_RPCD
void set_uci_converter_fun(int (*callback)(char* path, const char* option, const char* value))
{
        ERROR ("%s:%d\n", __FUNCTION__, __LINE__);
        printf("%s:%d uci_converter_set: %p cb: %p\n", __FUNCTION__, __LINE__, (void*)uci_converter_set_str, (void*)callback);
        uci_converter_set=callback;

        return;
}
#else
/**************************************************************************/
/*! \fn int uci_converter_set(char* path, const char* option, const char* value)
 **************************************************************************
 *  \brief generic set to UCI database
 *  \param[in] char *path - config name to set (config file.device name) for example wireless.default_radio45
 *  \param[in] char *option - option (param) name to set
 *  \param[in] char *value - value to write tom UCI DB 
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int uci_converter_set(char* path, const char* option, const char* value)
{
	int status;
	struct uci_ptr ptr;
	struct uci_context *cont = uci_alloc_context();

	if (!cont)
		return RETURN_ERR;

	if ((path == NULL) || (option == NULL) || (value == NULL)) {
		ERROR("%s, path or option or value is NULL!\n", __FUNCTION__);
		uci_free_context(cont);
		return RETURN_ERR;
	}

	if ((uci_lookup_ptr(cont, &ptr, path, true) != UCI_OK) || (!ptr.s)) {
		uci_free_context(cont);
		return RETURN_ERR;
	}

	ptr.option = option;
	ptr.value = value;

	if (uci_set(cont, &ptr) != UCI_OK) {
		uci_free_context(cont);
		return RETURN_ERR;
	}

	status = uci_save(cont, ptr.p);

	uci_free_context(cont);

	return (status == UCI_OK) ? RETURN_OK : RETURN_ERR;
}
#endif

/**************************************************************************/
/*! \fn int uci_converter_add_device(char* config_file, char* device_name, char* device_type)
 **************************************************************************
 *  \brief add a device to UCI database (like a new VAP)
 *  \param[in] char* config_file - config file name for example wireless
 *  \param[in] char* device_name - device name for example default_radio45
 *  \param[in] char* device_type - device type for example wifi-iface
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int uci_converter_add_device(char* config_file, char* device_name, char* device_type)
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

	cont = uci_alloc_context();
	if (!cont)
		return RETURN_ERR;

	if ((uci_lookup_ptr(cont, &ptr, path, true) != UCI_OK)) {
		uci_free_context(cont);
		return RETURN_ERR;
	}

	if (uci_set(cont, &ptr) != UCI_OK) {
		uci_free_context(cont);
		return RETURN_ERR;
	}

	status = uci_save(cont, ptr.p);

	uci_free_context(cont);

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
	int status;
	char path[MAX_UCI_BUF_LEN] = "";
	char radio_str[] = "default_radio"; /* == TYPE_VAP / TYPE_RADIO_VAP */
	char get_value[MAX_UCI_BUF_LEN] = "";

	if ((param == NULL) || (value == NULL)) {
		ERROR("%s, param or value is NULL!\n", __FUNCTION__);
		return RETURN_ERR;
	}

	if (type == TYPE_RADIO)
		strncpy_s(radio_str, sizeof(radio_str), "radio", 5);

	status = sprintf_s(path, MAX_UCI_BUF_LEN, "wireless.%s%d", radio_str, rpc_to_uci_index(type, index));
	if (status <= 0) {
		ERROR("%s failed sprintf_s.  index=%d param=%s\n", __func__, index, param);
		return RETURN_ERR;
	}

	uci_converter_get_optional_str(type, index, param, get_value, MAX_UCI_BUF_LEN, "");
	if (!strncmp(value, get_value, MAX_UCI_BUF_LEN)) {
		DEBUG("%s same value being set index=%d %s=%s\n", __func__, index, param, value);
		return RETURN_OK;
	}

	status = uci_converter_set(path, param, value);
	if (status == RETURN_OK) {
		DEBUG("%s index=%d %s=%s\n", __func__, index, param, value);
	} else {
		ERROR("%s failed. index=%d %s=%s\n", __func__, index, param, value);
		return RETURN_ERR;
	}

	return uci_converter_set_param_changed(type, index, true);
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
		ERROR("%s, param is NULL!\n", __FUNCTION__);
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
		ERROR("%s, param is NULL!\n", __FUNCTION__);
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
		ERROR("%s, param is NULL!\n", __FUNCTION__);
		return RETURN_ERR;
	}

	status = sprintf_s(val, MAX_UCI_BUF_LEN, "%lu", value);
	if (status <= 0)
		return RETURN_ERR;

	status = uci_converter_set_str(type, index, param, val);

	return status;
}

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
		ERROR("%s, param is NULL!\n", __FUNCTION__);
		return RETURN_ERR;
	}

	return uci_converter_set_int(type, index, param, (value ? 1 : 0));
}

/**************************************************************************/
/*! \fn int uci_converter_del_elem(char* path)
 **************************************************************************
 *  \brief remove an element from the UCI database (using full path)
 *  \param[in] char *path - complete path of the element to delete
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int uci_converter_del_elem(char* path)
{
	int status = RETURN_ERR;
	struct uci_ptr ptr;
	struct uci_context *cont = uci_alloc_context();

	if (!cont)
		goto out;

	if (path == NULL) {
		ERROR("%s, path is NULL!\n", __FUNCTION__);
		status = RETURN_ERR;
		goto out_after_cont;
	}

	if (uci_lookup_ptr(cont, &ptr, path, true) != UCI_OK)
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
	uci_free_context(cont);
out:
	return status;
}

/**************************************************************************/
/*! \fn int uci_converter_del(enum paramType type, int index, char* param)
 **************************************************************************
 *  \brief remove an element from the UCI database (using param name)
 *  \param[in] enum paramType type - TYPE_RADIO / TYPE_RADIO_VAP / TYPE_VAP
 *  \param[in] int index - radio or VAP index (regular index, not UCI index)
 *  \param[in] const char param[] - param name to delete (not full path)
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int uci_converter_del(enum paramType type, int index, char* param)
{
	int status;
	char path[MAX_UCI_BUF_LEN] = "";
	char radio_str[] = "default_radio"; /* == TYPE_VAP / TYPE_RADIO_VAP */

	if (param == NULL) {
		ERROR("%s, param is NULL!\n", __FUNCTION__);
		return RETURN_ERR;
	}

	if (type == TYPE_RADIO)
		strncpy_s(radio_str, sizeof(radio_str), "radio", 5);

	if (sprintf_s(path, MAX_UCI_BUF_LEN, "wireless.%s%d.%s",
		radio_str, rpc_to_uci_index(type, index), param) <= 0) {
		ERROR("%s failed sprintf_s.  index=%d param=%s", __func__, index, param);
		return RETURN_ERR;
	}

	status = uci_converter_del_elem(path);
	if (status == RETURN_ERR) {
		ERROR("%s failed: delete %s for index %d\n", __func__, param, index);
		return status;
	}

	return uci_converter_set_param_changed(type, index, true);
}

/**************************************************************************/
/*! \fn int uci_converter_add_list(char* path, char* option, char* value)
 **************************************************************************
 *  \brief append a string to a UCI element list (using path and param)
 *  \param[in] char *path - element path (config file.device name) 
 *  \param[in] char *option - param name to append string to
 *  \param[in] char *value - string value to append
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int uci_converter_add_list(char* path, char* option, char* value)
{
	int status;
	struct uci_ptr ptr;
	struct uci_context *cont = uci_alloc_context();

	if (!cont)
		return RETURN_ERR;

	if ((path == NULL) || (option == NULL) || (value == NULL)) {
		ERROR("%s, path or option or value is NULL!\n", __FUNCTION__);
		uci_free_context(cont);
		return RETURN_ERR;
	}

	if ((uci_lookup_ptr(cont, &ptr, path, true) != UCI_OK) || (!ptr.s)) {
		uci_free_context(cont);
		return RETURN_ERR;
	}

	ptr.option = option;
	ptr.value = value;

	if (uci_add_list(cont, &ptr) != UCI_OK) {
		uci_free_context(cont);
		return RETURN_ERR;
	}

	status = uci_save(cont, ptr.p);

	uci_free_context(cont);

	return (status == UCI_OK) ? RETURN_OK : RETURN_ERR;
}

/**************************************************************************/
/*! \fn int uci_converter_add_list_str(enum paramType type, int index, char param[], char *value)
 **************************************************************************
 *  \brief append a string to a UCI element list (using param)
 *  \param[in] char *path - element path (config file.device name) 
 *  \param[in] enum paramType type - TYPE_RADIO / TYPE_RADIO_VAP / TYPE_VAP
 *  \param[in] int index - radio or VAP index (regular index, not UCI index)
 *  \param[in] const char param[] - param name to append string to
 *  \param[in] char *value - string value to append
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int uci_converter_add_list_str(enum paramType type, int index, char param[], char *value)
{
	int status;
	char path[MAX_UCI_BUF_LEN] = "";
	char radio_str[] = "default_radio"; /* == TYPE_VAP / TYPE_RADIO_VAP */

	if ((param == NULL) || (value == NULL)) {
		ERROR("%s, param or value is NULL!\n", __FUNCTION__);
		return RETURN_ERR;
	}

	if (type == TYPE_RADIO)
		strncpy_s(radio_str, sizeof(radio_str), "radio", 5);

	status = sprintf_s(path, MAX_UCI_BUF_LEN, "wireless.%s%d", radio_str, rpc_to_uci_index(type, index));
	if (status <= 0)
		return RETURN_ERR;

	status = uci_converter_add_list(path, param, value);
	if (status == RETURN_OK) {
		DEBUG("%s index=%d %s=%s\n", __func__, index, param, value);
	} else {
		ERROR("%s failed. index=%d %s=%s\n", __func__, index, param, value);
		return RETURN_ERR;
	}

	return uci_converter_set_param_changed(type, index, true);
}

/**************************************************************************/
/*! \fn int uci_converter_del_list(char* path, char* option, char* value)
 **************************************************************************
 *  \brief delete a string from a UCI element list (using path and param)
 *  \param[in] char *path - element path (config file.device name) 
 *  \param[in] char *option - param name to remove string from
 *  \param[in] char *value - string value to remove
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int uci_converter_del_list(char* path, char* option, char* value)
{
	int status;
	char full_path[MAX_LEN_VALID_VALUE] = "";
	struct uci_ptr ptr;
	struct uci_context *cont = uci_alloc_context();

	if (!cont)
		return RETURN_ERR;

	if ((path == NULL) || (option == NULL) || (value == NULL)) {
		ERROR("%s, path or option or value is NULL!\n", __FUNCTION__);
		uci_free_context(cont);
		return RETURN_ERR;
	}

	status = sprintf_s(full_path, (MAX_LEN_VALID_VALUE - 1), "%s.%s=%s", path, option, value);
	if (status <= 0) {
		ERROR("%s sprintf_s failed. full path, option=%s value=%s\n", __func__, option, value);
		uci_free_context(cont);
		return RETURN_ERR;
	}

	if ((uci_lookup_ptr(cont, &ptr, full_path, true) != UCI_OK) || (!ptr.s)) {
		uci_free_context(cont);
		return RETURN_ERR;
	}

	if (uci_del_list(cont, &ptr) != UCI_OK) {
		uci_free_context(cont);
		return RETURN_ERR;
	}

	status = uci_save(cont, ptr.p);

	uci_free_context(cont);

	return (status == UCI_OK) ? RETURN_OK : RETURN_ERR;
}

/**************************************************************************/
/*! \fn int uci_converter_del_list_str(enum paramType type, int index, char param[], char *value)
 **************************************************************************
 *  \brief delete a string from a UCI element list (using param)
 *  \param[in] enum paramType type - TYPE_RADIO / TYPE_RADIO_VAP / TYPE_VAP
 *  \param[in] int index - radio or VAP index (regular index, not UCI index)
 *  \param[in] const char param[] - param name to remove string from
 *  \param[in] char *value - string value to remove
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int uci_converter_del_list_str(enum paramType type, int index, char param[], char *value)
{
	int status;
	char path[MAX_UCI_BUF_LEN] = "";
	char radio_str[] = "default_radio"; /* == TYPE_VAP / TYPE_RADIO_VAP */

	if ((param == NULL) || (value == NULL)) {
		ERROR("%s, param or value is NULL!\n", __FUNCTION__);
		return RETURN_ERR;
	}

	if (type == TYPE_RADIO)
		strncpy_s(radio_str, sizeof(radio_str), "radio", 5);

	status = sprintf_s(path, MAX_UCI_BUF_LEN, "wireless.%s%d", radio_str, rpc_to_uci_index(type, index));
	if (status <= 0)
		return RETURN_ERR;

	status = uci_converter_del_list(path, param, value);
	if (status == RETURN_OK) {
		DEBUG("%s index=%d %s=%s\n", __func__, index, param, value);
	} else {
		ERROR("%s failed. index=%d %s=%s\n", __func__, index, param, value);
		return RETURN_ERR;
	}

	return uci_converter_set_param_changed(type, index, true);
}

/**************************************************************************/
/*! \fn int uci_converter_get_list(char* list, char **output)
 **************************************************************************
 *  \brief get a list of strings from UCI element list (using full path)
 *  \param[in] char *list - full path of list to get (config file.device name,param) 
 *  \param[out] char **output - an array of strings returned
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int uci_converter_get_list(char* list, char **output)
{
	unsigned int index = 0;
	struct uci_element *e, *tmp;
	struct uci_ptr ptr;
	struct uci_context *cont = uci_alloc_context();

	if (!cont)
		return RETURN_ERR;

	if ((list == NULL) || (output == NULL) || (*output == NULL)) {
		ERROR("%s, list or output or *output is NULL!\n", __FUNCTION__);
		uci_free_context(cont);
		return RETURN_ERR;
	}

	if (uci_lookup_ptr(cont, &ptr, list, true) != UCI_OK) {
		uci_free_context(cont);
		return RETURN_ERR;
	}

	if (!(ptr.o && ptr.option)) {
		uci_free_context(cont);
		return RETURN_ERR;
	}

	if ((ptr.o->type != UCI_TYPE_LIST)) {
		uci_free_context(cont);
		return RETURN_ERR;
	}

	uci_foreach_element_safe(&ptr.o->v.list, tmp, e) {
		size_t len = strnlen_s(uci_to_option(e)->e.name, MAX_UCI_BUF_LEN - 1);
		strncpy_s((output)[index++], len + 1, uci_to_option(e)->e.name, len);
	}

	uci_free_context(cont);

	return RETURN_OK;
}

/**************************************************************************/
/*! \fn int uci_converter_get_list_str(enum paramType type, int index, char param[], char **output)
 **************************************************************************
 *  \brief get a list of strings from UCI element list (using param)
 *  \param[in] enum paramType type - TYPE_RADIO / TYPE_RADIO_VAP / TYPE_VAP
 *  \param[in] int index - radio or VAP index (regular index, not UCI index)
 *  \param[in] const char param[] - param name of string list to return
 *  \param[out] char **output - an array of strings returned
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int uci_converter_get_list_str(enum paramType type, int index,
		char param[], char **output)
{
	int status;
	char path[MAX_UCI_BUF_LEN] = "";
	char radio_str[] = "default_radio"; /* == TYPE_VAP / TYPE_RADIO_VAP */

	if ((param == NULL) || (output == NULL) || (*output == NULL)) {
		ERROR("%s, param or output or *output is NULL!\n", __FUNCTION__);
		return RETURN_ERR;
	}

	if (type == TYPE_RADIO)
		strncpy_s(radio_str, sizeof(radio_str), "radio", 5);

	status = sprintf_s(path, MAX_UCI_BUF_LEN, "wireless.%s%d.%s", radio_str, rpc_to_uci_index(type, index), param);
	if (status <= 0)
		return RETURN_ERR;

	return uci_converter_get_list(path, output);
}

/**************************************************************************/
/*! \fn int uci_count_elements_list(char *list, unsigned int *num_of_elements)
 **************************************************************************
 *  \brief count the number of elements in UCI element list (using full path)
 *  \param[in] char *list - full path of list to count elements in (config file.device name,param) 
 *  \param[out] unsigned int *num_of_elements - return number of elements
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int uci_count_elements_list(char *list, unsigned int *num_of_elements)
{
	unsigned int count = 0;
	struct uci_element *e, *tmp;
	struct uci_ptr ptr;
	struct uci_context *cont = uci_alloc_context();

	if (!cont)
		return RETURN_ERR;

	if ((list == NULL) || (num_of_elements == NULL)) {
		ERROR("%s, list or num_of_elements is NULL!\n", __FUNCTION__);
		uci_free_context(cont);
		return RETURN_ERR;
	}

	if (uci_lookup_ptr(cont, &ptr, list, true) != UCI_OK) {
		uci_free_context(cont);
		*num_of_elements = count;
		return RETURN_ERR;
	}

	if (!(ptr.o && ptr.option)) {
		uci_free_context(cont);
		*num_of_elements = count;
		return RETURN_ERR;
	}

	if ((ptr.o->type != UCI_TYPE_LIST)) {
		uci_free_context(cont);
		*num_of_elements = count;
		return RETURN_ERR;
	}

	uci_foreach_element_safe(&ptr.o->v.list, tmp, e) {
		count++;
	}

	uci_free_context(cont);

	*num_of_elements = count;

	return RETURN_OK;
}

/**************************************************************************/
/*! \fn int uci_converter_count_elements_list(enum paramType type, int index, char param[], unsigned int *num_of_elements)
 **************************************************************************
 *  \brief count the number of elements in UCI element list (using param)
 *  \param[in] enum paramType type - TYPE_RADIO / TYPE_RADIO_VAP / TYPE_VAP
 *  \param[in] int index - radio or VAP index (regular index, not UCI index)
 *  \param[in] const char param[] - param name of string list to count elements in
 *  \param[out] unsigned int *num_of_elements - return number of elements
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int uci_converter_count_elements_list(enum paramType type, int index, char param[],
		unsigned int *num_of_elements)
{
	int status;
	char path[MAX_UCI_BUF_LEN] = "";
	char radio_str[] = "default_radio"; /* == TYPE_VAP / TYPE_RADIO_VAP */

	if ((param == NULL) || (num_of_elements == NULL)) {
		ERROR("%s, param or num_of_elements is NULL!\n", __FUNCTION__);
		return RETURN_ERR;
	}

	if (type == TYPE_RADIO)
		strncpy_s(radio_str, sizeof(radio_str), "radio", 5);

	status = sprintf_s(path, MAX_UCI_BUF_LEN, "wireless.%s%d.%s", radio_str, rpc_to_uci_index(type, index), param);
	if (status <= 0)
		return RETURN_ERR;

	return uci_count_elements_list(path, num_of_elements);
}

int uci_converter_reorder(enum paramType type, int index, int pos)
{
	int status;
	char path[MAX_UCI_BUF_LEN] = "";
	char radio_str[] = "default_radio"; /* == TYPE_VAP / TYPE_RADIO_VAP */

	if (type == TYPE_RADIO)
		strncpy_s(radio_str, sizeof(radio_str), "radio", 5);

	status = sprintf_s(path, MAX_UCI_BUF_LEN, "wireless.%s%d", radio_str, rpc_to_uci_index(type, index));
	if (status <= 0)
		return RETURN_ERR;

	status = uci_converter_reorder_section(path, pos);
	if (status != RETURN_OK)
		return RETURN_ERR;

	status = sprintf_s(path, MAX_UCI_BUF_LEN, "meta-wireless.%s%d", radio_str, rpc_to_uci_index(type, index));
	if (status <= 0)
		return RETURN_ERR;

	return uci_converter_reorder_section(path, pos);
}

int uci_converter_reorder_section(char* path, int pos)
{
	int status;
	struct uci_ptr ptr;
	struct uci_context *cont = uci_alloc_context();

	if (!cont)
		return RETURN_ERR;

	if ((path == NULL)) {
		ERROR("%s, path NULL!\n", __FUNCTION__);
		uci_free_context(cont);
		return RETURN_ERR;
	}

	if ((uci_lookup_ptr(cont, &ptr, path, true) != UCI_OK) || (!ptr.s)) {
		uci_free_context(cont);
		return RETURN_ERR;
	}

	if (uci_reorder_section(cont, ptr.s, pos) != UCI_OK) {
		uci_free_context(cont);
		return RETURN_ERR;
	}

	status = uci_save(cont, ptr.p);

	uci_free_context(cont);

	return (status == UCI_OK) ? RETURN_OK : RETURN_ERR;
}

/**************************************************************************/
/*! \fn int uci_converter_revert_vaps(char* path, char* device)
 **************************************************************************
 *  \brief remove uncommited changes for all vaps of a radio
 *  \param[in] char* path - file name of the db
 *  \param[in] char* device - name of the radio
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
static int uci_converter_revert_vaps(char* path, char* device)
{
	int wireless_idx;
	struct uci_ptr ptr;
	struct uci_context *ctx;
	struct uci_package *p = NULL;
	struct uci_element *e;
	struct uci_section *s;
	char lookup_path[MAX_UCI_BUF_LEN] = "";
	char vap_type[MAX_UCI_BUF_LEN] = "wifi-iface";

	if ((path == NULL) || (device == NULL)) {
		ERROR("%s, path or device is NULL!\n", __FUNCTION__);
		return RETURN_ERR;
	}

	ctx = uci_alloc_context();
	if (!ctx)
		return RETURN_ERR;

	strncpy_s(lookup_path, MAX_UCI_BUF_LEN, path, strnlen_s(path, MAX_UCI_BUF_LEN - 1));

	if ((uci_lookup_ptr(ctx, &ptr, lookup_path, true) != UCI_OK)) {
		ERROR("%s, uci lookup failed!\n", __FUNCTION__);
		uci_free_context(ctx);
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
					uci_free_context(ctx);
					return RETURN_ERR;
				}

				if (!strncmp(device, o->v.string, MAX_UCI_BUF_LEN))
					uci_converter_revert_elem(path, TYPE_VAP, uci_to_rpc_index(TYPE_VAP, wireless_idx), NULL);

				break;
			}
		}
	}
	uci_free_context(ctx);
	return RETURN_OK;
}

/**************************************************************************/
/*! \fn int uci_converter_revert(char* path)
 **************************************************************************
 *  \brief remove uncommited changes for a giver path
 *  \param[in] char* path - config<.device><.parameter>
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
static int uci_converter_revert(char* path)
{
	int status;
	struct uci_ptr ptr;
	struct uci_context *cont = uci_alloc_context();

	if (!cont)
		return RETURN_ERR;

	if (path == NULL) {
		ERROR("%s, path is NULL!\n", __FUNCTION__);
		uci_free_context(cont);
		return RETURN_ERR;
	}

	if (uci_lookup_ptr(cont, &ptr, path, true) != UCI_OK) {
        ERROR("%s: lookup failed\n", __FUNCTION__);
		uci_free_context(cont);
		return RETURN_ERR;
	}

	if (uci_revert(cont, &ptr) != UCI_OK) {
        ERROR("%s: revert failed\n", __FUNCTION__);
		uci_free_context(cont);
		return RETURN_ERR;
	}

	status = uci_save(cont, ptr.p);

	uci_free_context(cont);

	return (status == UCI_OK) ? RETURN_OK : RETURN_ERR;
}

/**************************************************************************/
/*! \fn int uci_converter_revert_elem(char* config, enum paramType type,
 *          int index, char* param)
 **************************************************************************
 *  \brief remove uncommited changes using the given path
 *  \param[in] char* config - name of config file (default "wireless")
 *  \param[in] enum paramType type - radio or vap (if someting else, revert the
 *                                   whole config file
 *  \param[in] int index - index of the device (if less the 0, revert the whole
 *                         config file
 *  \param[in] char* param - parameter name (if not given, revert the whole device)
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int uci_converter_revert_elem(char* config, enum paramType type, int index, char* param)
{
	int status;
	char path[MAX_UCI_BUF_LEN] = "";
	char device[MAX_UCI_BUF_LEN] = "";
	char radio_str[] = "default_radio"; /* == TYPE_VAP / TYPE_RADIO_VAP */

	if ((config == NULL) || (param == NULL)) {
		ERROR("%s, config or param is NULL!\n", __FUNCTION__);
		return RETURN_ERR;
	}

	if (type == TYPE_RADIO)
		strncpy_s(radio_str, sizeof(radio_str), "radio", 5);

	if (!config || !strcmp(config, "")) {
		strncat_s(path, MAX_UCI_BUF_LEN, "wireless", strnlen_s("wireless", MAX_UCI_BUF_LEN - 1));
	} else {
		strncat_s(path, MAX_UCI_BUF_LEN, config, strnlen_s(config, MAX_UCI_BUF_LEN - 1));
	}

	if (index < 0 || (type != TYPE_RADIO && type != TYPE_VAP && type != TYPE_RADIO_VAP)) {
		goto revert;
	} else {
		status = sprintf_s(device, MAX_UCI_BUF_LEN, "%s%d", radio_str, rpc_to_uci_index(type, index));
		if (status <= 0) {
			ERROR("%s sprintf_s failed: device, radio_str=%s index=%d\n", __func__, radio_str, index);
			return RETURN_ERR;
		}
		strncat_s(path, MAX_UCI_BUF_LEN, ".", strnlen_s(".", MAX_UCI_BUF_LEN - 1));
		strncat_s(path, MAX_UCI_BUF_LEN, device, strnlen_s(device, MAX_UCI_BUF_LEN - 1));
	}

	if (param && strcmp(param, "")) {
		strncat_s(path, MAX_UCI_BUF_LEN, ".", strnlen_s(".", MAX_UCI_BUF_LEN - 1));
		strncat_s(path, MAX_UCI_BUF_LEN, param, strnlen_s(param, MAX_UCI_BUF_LEN - 1));
	}

revert:
	return uci_converter_revert(path);
}

/**************************************************************************/
/*! \fn int uci_converter_revert_radio(char* path, int index)
 **************************************************************************
 *  \brief remove uncommited changes for the given radio and all its vaps
 *  \param[in] char* path - name of the config file
 *  \param[in] int index - index of the radio
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int uci_converter_revert_radio(char* path, int index)
{
	int status;
	char radio[MAX_UCI_BUF_LEN] = "";

	if (path == NULL) {
		ERROR("%s, path is NULL!\n", __FUNCTION__);
		return RETURN_ERR;
	}

	uci_converter_revert_elem(path, TYPE_RADIO, index, NULL);

	status = sprintf_s(radio, MAX_UCI_BUF_LEN, "radio%d", rpc_to_uci_index(TYPE_RADIO, index));
	if (status <= 0) {
		ERROR("%s sprintf_s failed: radio, index=%d\n", __func__, index);
		return RETURN_ERR;
	}

	return uci_converter_revert_vaps(path, radio);
}

/**************************************************************************/
/*! \fn int uci_converter_revert_meta(void)
 **************************************************************************
 *  \brief remove uncommited changes for meta-wireless
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int uci_converter_revert_meta(void)
{
	char meta[MAX_UCI_BUF_LEN] = "meta-wireless";
	return uci_converter_revert_elem(meta, -1, -1, NULL);
}

/**************************************************************************/
/*! \fn int uci_converter_commit(char* path, const char* alt_dir)
 **************************************************************************
 *  \brief commit all the changes made to the UCI database
 *  \param[in] char* path - name of the config file (for example wireless)
 *  \param[in] const char* alt_dir - alternate directory (default is /etc/config)
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int uci_converter_commit(char* path, const char* alt_dir)
{
	int status;
	struct uci_ptr ptr;
	struct uci_context *cont = uci_alloc_context();

	if (!cont)
		return RETURN_ERR;

	if (path == NULL) {
		ERROR("%s, path is NULL!\n", __FUNCTION__);
		uci_free_context(cont);
		return RETURN_ERR;
	}

	if (alt_dir)
		uci_set_confdir(cont, alt_dir);

	if (uci_lookup_ptr(cont, &ptr, path, true) != UCI_OK) {
		uci_free_context(cont);
		return RETURN_ERR;
	}

	status = uci_commit(cont, &ptr.p, false);

	uci_free_context(cont);

	return (status == UCI_OK) ? RETURN_OK : RETURN_ERR;
}

/**************************************************************************/
/*! \fn int uci_converter_commit_wireless(void)
 **************************************************************************
 *  \brief commit all the changes made to the UCI database in /etc/config/wireless package
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int uci_converter_commit_wireless(void)
{
	char wireless[] = "wireless";
	return uci_converter_commit(wireless, NULL);
}

/**************************************************************************/
/*! \fn int uci_converter_commit_metawireless(void)
 **************************************************************************
 *  \brief commit all the changes made to the UCI database in /tmp/meta-wireless package
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int uci_converter_commit_metawireless(void)
{
	char wireless[] = "meta-wireless";
	return uci_converter_commit(wireless, "/tmp/");
}

/**************************************************************************/
/*! \fn int get_htmode_enum(int index, enum uci_htmode *uci_htmode) {
 **************************************************************************
 *  \brief get htmode for a specific radio from UCI database
 *  \param[in] int index - radio index
 *  \param[out] enum uci_htmode *uci_htmode - output the radio htmode
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int get_htmode_enum(int index, enum uci_htmode *uci_htmode) {
	int status;
	char htmode_str[MAX_UCI_BUF_LEN];

	if (uci_htmode == NULL) {
		ERROR("%s, uci_htmode is NULL!\n", __FUNCTION__);
		return RETURN_ERR;
	}

	*uci_htmode = UCI_HTMODE_LAST;

	status = uci_converter_get_str(TYPE_RADIO, index, "htmode", htmode_str);
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
/*! \fn int set_htmode_enum(int index, enum uci_htmode uci_htmode) {
 **************************************************************************
 *  \brief set htmode for a specific radio
 *  \param[in] int index - radio index
 *  \param[in] enum uci_htmode uci_htmode - the radio htmode to set
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int set_htmode_enum(int index, enum uci_htmode uci_htmode) {
	char htmode_str[MAX_UCI_BUF_LEN] = "";

	if (uci_htmode == UCI_HTMODE_HT20)
		strncpy_s(htmode_str, MAX_UCI_BUF_LEN, "HT20", 4);
	else if (uci_htmode == UCI_HTMODE_HT40PLUS)
		strncpy_s(htmode_str, MAX_UCI_BUF_LEN, "HT40+", 5);
	else if (uci_htmode == UCI_HTMODE_HT40MINUS)
		strncpy_s(htmode_str, MAX_UCI_BUF_LEN, "HT40-", 5);
	else if (uci_htmode == UCI_HTMODE_VHT20)
		strncpy_s(htmode_str, MAX_UCI_BUF_LEN, "VHT20", 5);
	else if (uci_htmode == UCI_HTMODE_VHT40PLUS)
		strncpy_s(htmode_str, MAX_UCI_BUF_LEN, "VHT40+", 6);
	else if (uci_htmode == UCI_HTMODE_VHT40MINUS)
		strncpy_s(htmode_str, MAX_UCI_BUF_LEN, "VHT40-", 6);
	else if (uci_htmode == UCI_HTMODE_VHT80)
		strncpy_s(htmode_str, MAX_UCI_BUF_LEN, "VHT80", 5);
	else if (uci_htmode == UCI_HTMODE_VHT160)
		strncpy_s(htmode_str, MAX_UCI_BUF_LEN, "VHT160", 6);
	else {
		ERROR("Failed with uci_htmode=%d\n", uci_htmode);
		return RETURN_ERR;
	}

	return uci_converter_set_str(TYPE_RADIO, index, "htmode", htmode_str);
}

/**************************************************************************/
/*! \fn int get_hwmode_enum(int index, enum uci_hwmode *uci_hwmode) {
 **************************************************************************
 *  \brief get hwmode for a specific radio from UCI database
 *  \param[in] int index - radio index
 *  \param[out] enum uci_hwmode *uci_hwmode - output the radio hwmode
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int get_hwmode_enum(int index, enum uci_hwmode *uci_hwmode) {
	int status;
	char band[MAX_UCI_BUF_LEN];
	char hwmode_str[MAX_UCI_BUF_LEN];

	if (uci_hwmode == NULL) {
		ERROR("%s, uci_hwmode is NULL!\n", __FUNCTION__);
		return RETURN_ERR;
	}

	*uci_hwmode = UCI_HWMODE_LAST;

	status = uci_converter_get_str(TYPE_RADIO, index, "hwmode", hwmode_str);
	if (status == RETURN_ERR)
		return RETURN_ERR;

	status = uci_converter_get_str(TYPE_RADIO, index, "band", band);
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
	else if (!strncmp(hwmode_str, "11bgnax", MAX_UCI_BUF_LEN))
		*uci_hwmode = UCI_HWMODE_11BGNAX;
	else if (!strncmp(hwmode_str, "11n", MAX_UCI_BUF_LEN) && !strncmp(band, "2.4GHz", MAX_UCI_BUF_LEN))
		*uci_hwmode = UCI_HWMODE_11N_24G;
	else if (!strncmp(hwmode_str, "11n", MAX_UCI_BUF_LEN) && !strncmp(band, "5GHz", MAX_UCI_BUF_LEN))
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
	else {
		ERROR("Failed with hwmode_str=%s, band=%s\n", hwmode_str, band);
		return RETURN_ERR;
	}

	return RETURN_OK;
}

/**************************************************************************/
/*! \fn int set_hwmode_enum(int index, enum uci_hwmode uci_hwmode) {
 **************************************************************************
 *  \brief set hwmode for a specific radio
 *  \param[in] int index - radio index
 *  \param[in] enum uci_hwmode uci_hwmode - the radio hwmode to set
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int set_hwmode_enum(int index, enum uci_hwmode uci_hwmode) {
	char hwmode_str[MAX_UCI_BUF_LEN] = "";

	if (uci_hwmode == UCI_HWMODE_11B)
		strncpy_s(hwmode_str, MAX_UCI_BUF_LEN, "11b", 3);
	else if (uci_hwmode == UCI_HWMODE_11BG)
		strncpy_s(hwmode_str, MAX_UCI_BUF_LEN, "11bg", 4);
	else if (uci_hwmode == UCI_HWMODE_11G)
		strncpy_s(hwmode_str, MAX_UCI_BUF_LEN, "11g", 3);
	else if (uci_hwmode == UCI_HWMODE_11NG)
		strncpy_s(hwmode_str, MAX_UCI_BUF_LEN, "11ng", 4);
	else if (uci_hwmode == UCI_HWMODE_11BGN)
		strncpy_s(hwmode_str, MAX_UCI_BUF_LEN, "11bgn", 5);
	else if (uci_hwmode == UCI_HWMODE_11BGNAX)
		strncpy_s(hwmode_str, MAX_UCI_BUF_LEN, "11bgnax", 7);
	else if (uci_hwmode == UCI_HWMODE_11N_24G || uci_hwmode == UCI_HWMODE_11N_5G)
		strncpy_s(hwmode_str, MAX_UCI_BUF_LEN, "11n", 3);
	else if (uci_hwmode == UCI_HWMODE_11A)
		strncpy_s(hwmode_str, MAX_UCI_BUF_LEN, "11a", 3);
	else if (uci_hwmode == UCI_HWMODE_11AN)
		strncpy_s(hwmode_str, MAX_UCI_BUF_LEN, "11an", 4);
	else if (uci_hwmode == UCI_HWMODE_11NAC)
		strncpy_s(hwmode_str, MAX_UCI_BUF_LEN, "11nac", 5);
	else if (uci_hwmode == UCI_HWMODE_11AC)
		strncpy_s(hwmode_str, MAX_UCI_BUF_LEN, "11ac", 4);
	else if (uci_hwmode == UCI_HWMODE_11ANAC)
		strncpy_s(hwmode_str, MAX_UCI_BUF_LEN, "11anac", 6);
	else if (uci_hwmode == UCI_HWMODE_11ANACAX)
		strncpy_s(hwmode_str, MAX_UCI_BUF_LEN, "11anacax", 8);
	else {
		ERROR("Failed with uci_hwmode=%d\n", uci_hwmode);
		return RETURN_ERR;
	}

	return uci_converter_set_str(TYPE_RADIO, index, "hwmode", hwmode_str);
}

/**************************************************************************/
/*! \fn void uci_converter_prepare_for_up(int radio_idx)
 **************************************************************************
 *  \brief prepare uci db to be precessed by netifd
 *  \param[in] int radio_idx - the index of the radio to prepare
 ***************************************************************************/
void uci_converter_prepare_for_up(int radio_idx)
{
	uci_converter_rewrite_param_changed(radio_idx);

	if (uci_converter_commit_wireless() != RETURN_OK)
		ERROR("%s uci_converter_commit_wireless failed\n", __FUNCTION__);

	if (uci_converter_commit_metawireless() != RETURN_OK)
		ERROR("%s uci_converter_commit_metawireless failed\n", __FUNCTION__);
}

/**************************************************************************/
/*! \fn void uci_converter_clean_meta_after_up(int radio_idx, bool change_status)
 **************************************************************************
 *  \brief clean uci meta bd after being processed by netifd
 *  \param[in] int radio_idx - the index of the radio to prepare
 *  \param[in] bool change_status - flag that indicates if there was a change
 ***************************************************************************/
void uci_converter_clean_meta_after_up(int radio_idx, bool change_status)
{
	if (change_status) {
		if (uci_converter_reset_param_changed(radio_idx) != RETURN_OK)
			ERROR("%s uci_converter_reset_param_changed failed\n", __FUNCTION__);

		if (uci_converter_commit_metawireless() != RETURN_OK)
			ERROR("%s uci_converter_commit_wireless failed\n", __FUNCTION__);
	}
}

/**************************************************************************/
/*! \fn void uci_converter_clean_config_after_up(int radio_idx)
 **************************************************************************
 *  \brief clean uci bd after being processed by netifd
 *  \param[in] int radio_idx - the index of the radio to prepare
 ***************************************************************************/
void uci_converter_clean_config_after_up(int radio_idx)
{
	if (uci_converter_clean_param_changed(radio_idx) != RETURN_OK)
		ERROR("%s uci_converter_clean_param_changed failed\n", __FUNCTION__);

	if (uci_converter_commit_wireless() != RETURN_OK)
		ERROR("%s uci_converter_commit_metawireless failed\n", __FUNCTION__);
}

enum {
	HAL_RADIO,
	HAL_CHANGE_STATUS,
	__HAL_MAX
};

static const struct blobmsg_policy confirmation_policy[] = {
	[HAL_RADIO] = { .name = "radio", .type = BLOBMSG_TYPE_STRING },
	[HAL_CHANGE_STATUS] = { .name = "status", .type = BLOBMSG_TYPE_INT32 },
};

static void radio_cb_timeout(struct uloop_timeout *t)
{
	(void)t;
	DEBUG("%s: TIMEOUT received", __FUNCTION__);
	uloop_end();
}

static int confirmation_func(struct ubus_context *ctx, struct ubus_object *obj,
              struct ubus_request_data *req, const char *method,
              struct blob_attr *msg)
{
	static struct blob_buf resp_buf;
	struct blob_attr *hb[__HAL_MAX];
	char *radio;
	unsigned int change_status, radio_index;
	(void)obj;
	(void)method;

	blobmsg_parse(confirmation_policy, __HAL_MAX, hb, blob_data(msg), blob_len(msg));
	if (!hb[HAL_RADIO] || !hb[HAL_CHANGE_STATUS]) {
		ERROR("%s: UBUS_STATUS_INVALID_ARGUMENT", __FUNCTION__);
		return UBUS_STATUS_INVALID_ARGUMENT;
	}

	radio = blobmsg_get_string(hb[HAL_RADIO]);
	change_status =  blobmsg_get_u32(hb[HAL_CHANGE_STATUS]);

	if (!radio) {
		ERROR("%s: UBUS_STATUS_UNKNOWN_ERROR", __FUNCTION__);
		return UBUS_STATUS_UNKNOWN_ERROR;
	}

	blob_buf_init(&resp_buf, 0);
	ubus_send_reply(ctx, req, resp_buf.head);

	if (sscanf_s(radio, "radio%d", (int*)&radio_index) != 1) {
		ERROR("%s: Invalid radio index\n", __FUNCTION__);
		uloop_end();
		return 0;
	}

	uci_converter_clean_meta_after_up(uci_to_rpc_index(TYPE_RADIO, radio_index), change_status);

	uloop_end();
	return 0;
}

static const struct ubus_method hal_methods[] = {
	UBUS_METHOD("confirmation", confirmation_func, confirmation_policy),
};

static struct ubus_object_type hal_object_type =
	UBUS_OBJECT_TYPE("hal", hal_methods);

static struct ubus_object hal_object = {
	.name = "hal",
	.type = &hal_object_type,
	.methods = hal_methods,
	.n_methods = ARRAY_SIZE(hal_methods),
};

static void uci_converter_run_uloop(void)
{
	int ret;
	const char *ubus_socket = NULL;
	static struct ubus_context *ctx = NULL;
	struct uloop_timeout resp_timeout;
	resp_timeout.pending = false;
	resp_timeout.cb = radio_cb_timeout;

	uloop_init();
	signal(SIGPIPE, SIG_IGN);

	ctx = ubus_connect(ubus_socket);
	if (!ctx) {
		ERROR("%s: Failed to connect to ubus\n", __FUNCTION__);
		return;
	}

	ubus_add_uloop(ctx);
	ret = ubus_add_object(ctx, &hal_object);
	if (ret) {
		ERROR("%s: Failed to add object: %s\n", __FUNCTION__, ubus_strerror(ret));
		return;
	}

	uloop_timeout_set(&resp_timeout, NETIFD_RESP_TIMEOUT);
	uloop_run();

	ubus_remove_object(ctx, &hal_object);
	ubus_free(ctx);
	uloop_done();
	ctx = NULL;
}

static int uci_converter_start_uloop(struct ubus_context **ctx)
{
	int ret = RETURN_OK;
	const char *ubus_socket = NULL;

	if (ctx == NULL)
		return RETURN_ERR;

	uloop_init();
	signal(SIGPIPE, SIG_IGN);

	*ctx = ubus_connect(ubus_socket);
	if (!*ctx) {
		ERROR("%s: Failed to connect to ubus\n", __FUNCTION__);
        ret = RETURN_ERR;
	} else {
		ubus_add_uloop(*ctx);
		ret = ubus_add_object(*ctx, &hal_object);
		if (ret)
			ERROR("%s: Failed to add object: %s\n", __FUNCTION__, ubus_strerror(ret));
	}

    return ret;
}

static void uci_converter_clean_uloop(struct ubus_context **ctx)
{
	if (ctx == NULL)
		return;

	ubus_remove_object(*ctx, &hal_object);
	ubus_free(*ctx);
	uloop_done();
	*ctx = NULL;
}

static void uci_converter_finish_uloop(struct ubus_context **ctx)
{
	struct uloop_timeout resp_timeout;
	resp_timeout.pending = false;
	resp_timeout.cb = radio_cb_timeout;

	uloop_timeout_set(&resp_timeout, NETIFD_RESP_TIMEOUT);
	uloop_run();

	uci_converter_clean_uloop(ctx);
}

int uci_converter_netifd_connect(int radio_idx, bool initNetifd, char* command)
{
	static struct ubus_context *ctx = NULL;
	int ret, status = RETURN_OK;

	ret = uci_converter_start_uloop(&ctx);

	uci_converter_prepare_for_up(radio_idx);

	if (uci_converter_system(command) == RETURN_OK) {
		DEBUG("%s completed successfully\n", __FUNCTION__);
	} else {
		ERROR("%s failed\n", __FUNCTION__);
		uci_converter_clean_uloop(&ctx);
		status = RETURN_ERR;
		goto cleanup;
	}

	if (!ret && status == RETURN_OK) {
		uci_converter_finish_uloop(&ctx);
	}

	/* on init run uloop twice*/
	if (initNetifd)
		uci_converter_run_uloop();

cleanup:
	uci_converter_clean_config_after_up(radio_idx);

	return status;
}

//UCI HELPER ENDS

