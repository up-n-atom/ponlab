# ******************************************************************************** #
#       Copyright (c) 2024 MaxLinear, Inc.                                         #
#       Copyright (C) 2017-2018 Intel Corporation                                  #
#       Lantiq Beteiligungs-GmbH & Co. KG                                          #
#       Lilienthalstrasse 15, 85579 Neubiberg, Germany                             #
#       For licensing information, see the file 'LICENSE' in the root folder of    #
#        this software module.                                                     #
# *******************************************************************************  #

PKG_NAME:= libmsgapi

opt_no_flags := -Wcast-qual

bins := libmsgapi.so

libmsgapi.so_sources := ugw_msg_api.c
libmsgapi.so_cflags := -I./include/ 
libmsgapi.so_ldflags := -lubox -lubus -lsafec

include make.inc
