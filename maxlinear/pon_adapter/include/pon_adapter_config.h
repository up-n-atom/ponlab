/******************************************************************************
 *
 * Copyright (c) 2020 - 2023 MaxLinear, Inc.
 * Copyright (c) 2018 - 2019 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 ******************************************************************************/

/**
 * \file pon_adapter_config.h
 *
 * This is the PON adapter config header file, defining event callbacks.
 */

#ifndef _PON_ADAPTER_CONFIG_H_
#define _PON_ADAPTER_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdbool.h>

/** \addtogroup PON_ADAPTER
 *
 *  @{
 */

/** Maximum size of the parameter string */
#define PA_CONFIG_PARAM_STR_MAX_SIZE 1024

/* Optional structures needed for ubus_call */
struct ubus_request;
struct blob_attr;
typedef void (*ubus_data_handler_t)(struct ubus_request *req, int type,
				    struct blob_attr *msg);

/* Default UBUS timeout of 3 seconds */
#define PON_UBUS_TIMEOUT 3000

struct pa_config {
	/** Read a configuration value
	 *
	 *  \param[in]     caller	Caller context pointer
	 *  \param[in]     path		Config path
	 *  \param[in]     sec		Config section
	 *  \param[in]     opt		Config option
	 *  \param[in]     size		Option value size
	 *  \param[out]    out		Option value
	 */
	enum pon_adapter_errno (*get)(void *caller,
				      const char *path,
				      const char *sec,
				      const char *opt,
				      size_t size,
				      char *out);

	/** Save a configuration value
	 *
	 *  \param[in]     caller	Caller context pointer
	 *  \param[in]     path		Config path
	 *  \param[in]     sec		Config section
	 *  \param[in]     opt		Config option
	 *  \param[in]     val		Option value
	 *  \param[in]     append	Append an option value to a list or
	 *				create a new list if it does not exist
	 */
	enum pon_adapter_errno (*set)(void *caller,
				      const char *path,
				      const char *sec,
				      const char *opt,
				      const char *val,
				      bool append);

	/** Commit the changes made to the config file
	 *
	 *  \param[in]     caller	Caller context pointer
	 *  \param[in]     path		Config path
	 */
	enum pon_adapter_errno (*commit)(void *ctx, const char *path);

	/** Read a configuration value from secure storage
	 * Note: It is optional to provide this function (depending on an
	 * external library presence)
	 *
	 *  \param[in]	caller		Caller context pointer,
	 *				provided in ll_register_ops
	 *  \param[in]	path		Config path
	 *  \param[in]	sec		Config section
	 *  \param[in]	opt		Config option
	 *  \param[in]	option_size	Option value size
	 *  \param[out]	option_value	Option value
	 */
	enum pon_adapter_errno (*get_secure)(void *caller,
					     const char *path,
					     const char *sec,
					     const char *opt,
					     size_t option_size,
					     char *option_value);

	/** Save a configuration value in secure storage
	 * Note: It is optional to provide this function (depending on an
	 * external library presence)
	 *
	 *  \param[in]	caller		Caller context pointer,
	 *				provided in ll_register_ops
	 *  \param[in]	path		Config path
	 *  \param[in]	sec		Config section
	 *  \param[in]	opt		Config option
	 *  \param[in]	val		Option value
	 *  \param[in]	append	Append an option value to a list or
	 *				create a new list if it does not exist
	 */
	enum pon_adapter_errno (*set_secure)(void *caller,
					     const char *path,
					     const char *sec,
					     const char *opt,
					     const char *val,
					     bool append);

	/** Invoke a ubus function
	 *
	 * This function can be called from arbitrary threads, but it does
	 * locking to make sure only one thread at a time is using ubus.
	 *
	 * Note: It is optional to provide this function (depending on an
	 * external library presence)
	 *
	 *  \param[in]	caller		Caller context pointer,
	 *				provided in ll_register_ops
	 *  \param[in]	path		ubus path to call e.g. system
	 *  \param[in]	method		ubus method to call e.g. reboot
	 *  \param[in]	msg		ubus msg to send (optional)
	 *  \param[in]	cb		callback to handle answer (optional)
	 *  \param[in]	priv		private data for callback (optional)
	 *  \param[in]	timeout		timeout for the method call,
	 *				use PON_UBUS_TIMEOUT if unsure
	 *
	 *  \return
	 *     On success return 0. In case of an error an error code from
	 *     enum ubus_msg_status is returned. (This is a positive number)
	 */
	int (*ubus_call)(void *caller,
			const char *path,
			const char *method,
			struct blob_attr *msg,
			ubus_data_handler_t cb,
			void *priv,
			int timeout);
};

/** @} */ /* PON_ADAPTER */

#ifdef __cplusplus
}
#endif

#endif /* _PON_ADAPTER_CONFIG_H_ */
