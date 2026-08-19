/*****************************************************************************
 *
 * Copyright (c) 2017 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

#ifndef __uci_config_h__
#define __uci_config_h__

#define UCI_TUPLE_STR_MAX_SIZE	256
#define UCI_PARAM_STR_MAX_SIZE	256

/**
 * Get config parameter from the UCI.
 *
 * \param[in] path UCI file name
 * \param[in] sec UCI section name
 * \param[in] opt UCI option name
 * \param[out] out UCI option value
 *
 * \return true(1) in case of failure, false(0) in case of success
 */
int uci_config_get(const char *path, const char *sec, const char *opt,
		   char *out);

#endif
