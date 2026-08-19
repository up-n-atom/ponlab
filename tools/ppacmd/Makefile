##################################################################################
#                                                                                #
#       Copyright © 2020-2024 MaxLinear, Inc.                                    #
#       Copyright (c) 2017 Intel Corporation.                                    #
#       Copyright (c) 2011 - 2016 Lantiq Beteiligungs-GmbH & Co. KG.             #
#                                                                                #
#  For licensing information, see the file 'LICENSE' in the root folder of       #
#  this software module.                                                         #
##################################################################################
#
# Makefile for ppacmd
#

PPA_CFG_FLAG := $(CFG_FLAG)
PPA_CFG_FLAG +=-DCONFIG_PPA -DCONFIG_PPA_QOS 
PPA_CFG_FLAG +=-DCONFIG_IPV6
PPA_CFG_FLAG += $(if $(PLATFORM_XRX500),-DCONFIG_SOC_GRX500)
PPA_CFG_FLAG += $(if $(PLATFORM_PUMA7),-DCONFIG_PPA_PUMA7 -DCONFIG_PPA -DCONFIG_PPA_PUMA_HAL)
PPA_CFG_FLAG +=-DCONFIG_PPA_API_SW_FASTPATH

CFLAGS += -Werror
CFLAGS += $(PPA_CFG_FLAG)

LDFLAGS += -lsafec

all: ppacmd

ppacmd.o: ppacmd.c
#ppacmd_autotest.o: ppacmd_autotest.c

#ppacmd: ppacmd.o

ppacmd: ppacmd_autotest.o ppacmd.o

clean:
	rm -f *.o ppacmd
