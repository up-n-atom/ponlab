# ******************************************************************************** #
#       Copyright (c) 2024 MaxLinear, Inc.                                         #
#       Copyright (C) 2017-2018 Intel Corporation                                  #
#       Lantiq Beteiligungs-GmbH & Co. KG                                          #
#       Lilienthalstrasse 15, 85579 Neubiberg, Germany                             #
#       For licensing information, see the file 'LICENSE' in the root folder of    #
#        this software module.                                                     #
# *******************************************************************************  #

PKG_NAME:= libugwhelper
bins := libugwhelper.so 

libugwhelper.so_sources := $(wildcard *.c)
libugwhelper.so_cflags := -I./include/ 
libugwhelper.so_ldflags := -lsafec -lhelper -lubox -lubus -lblobmsg_json
	
include make.inc
