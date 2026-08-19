/******************************************************************************
 *
 * Copyright (c) 2017 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/
#ifndef _FAPI_PON_CLI_H_
#define _FAPI_PON_CLI_H_

#define UNUSED(x) (void)(x)

#include "lib_cli_core.h"

int pon_cli_cmd_register(struct cli_core_context_s *p_core_ctx);

int pon_ext_cli_cmd_register(struct cli_core_context_s *p_core_ctx);

#endif /* _FAPI_PON_CLI_H_ */
