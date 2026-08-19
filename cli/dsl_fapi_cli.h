/* Copyright 2017, Intel Corporation */
/******************************************************************************

                          Copyright (c) 2015-2017
                     Lantiq Beteiligungs-GmbH & Co. KG

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/

#ifndef _dsl_fapi_cli_h
#define _dsl_fapi_cli_h

#include "dsl_fapid.h"
#include "lib_cli_config.h"
#include "lib_cli_core.h"
#include "ifxos_print.h"

#define INCLUDE_CLI_PIPE_SUPPORT   1

#ifdef INCLUDE_CLI_PIPE_SUPPORT
#  include "lib_cli_pipe.h"
#endif

#ifdef INCLUDE_CLI_PIPE_SUPPORT
#  ifndef DSL_FAPI_MAX_CLI_PIPES
#    define DSL_FAPI_MAX_CLI_PIPES                         1
#  endif
#   define DSL_FAPI_PIPE_NAME                        "dsl_fapi"
#endif

#define DSL_FAPI_CRLF IFXOS_CRLF
#define DSL_FAPI "DSL FAPI CLI: "
int cli_start(const bool console, int entity, char *cfg_file);

#endif
