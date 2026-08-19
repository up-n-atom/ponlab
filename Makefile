# ******************************************************************************** #
#       Copyright (C) 2020-2024 MaxLinear, Inc.
#       Copyright (C) 2019 Intel Corporation                                       #
#       Copyright (C) 2017-2018 Intel Corporation                                  #
#       Lantiq Beteiligungs-GmbH & Co. KG                                          #
#       For licensing information, see the file 'LICENSE' in the root folder of    #
#        this software module.                                                     #
# *******************************************************************************  #

PKG_NAME := fwupgrade

opt_no_flags := -Wcast-qual
CFLAGS += -I./include

bins := fwupgrade

fwupgrade_sources := sysupgrade_ubus.c sysupgrade.c 

fwupgrade_ldflags := -lsafec -lscapi -lugwhelper -lubox -lblobmsg_json -lubus -lhelper -lmsgapi -lsecurefapi

include make.inc
