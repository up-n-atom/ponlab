# ******************************************************************************** #
#       Copyright © 2020 MaxLinear, Inc.                                           #
#       Copyright (C) 2017-2018 Intel Corporation                                  #
#       Lantiq Beteiligungs-GmbH & Co. KG                                          #
#       Lilienthalstrasse 15, 85579 Neubiberg, Germany                             #
#       For licensing information, see the file 'LICENSE' in the root folder of    #
#        this software module.                                                     #
# *******************************************************************************  #

PKG_NAME := libscapi

bins := libscapi.so utils/scapiutil utils/try_reboot

libscapi.so_sources := $(wildcard *.c)
libscapi.so_cflags := -Wno-unused-function -std=gnu11 -I./include -DMEM_DEBUG
libscapi.so_ldflags :=  -lsafec-3.3

scapiutil := utils/scapiutil.c
scapiutil_ldflags := -lsafec-3.3 -L./ -lscapi
scapiutil_cflags := -I./include/

include make.inc
