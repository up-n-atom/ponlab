##################################################################################
#                                                                                #
#       Copyright © 2021-2024 MaxLinear, Inc.                                    #
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

CFLAGS += $(PPA_CFG_FLAG)

LDFLAGS += -lsafec

all: qoscmd

qoscmd.o: qoscmd.c

#ppacmd: ppacmd.o

qoscmd: qoscmd.o

clean:
	rm -f *.o qoscmd
