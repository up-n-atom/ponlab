# ******************************************************************************** #
#       Copyright (c) 2015                                                         #
#       Lantiq Beteiligungs-GmbH & Co. KG                                          #
#       Lilienthalstrasse 15, 85579 Neubiberg, Germany                             #
#       For licensing information, see the file 'LICENSE' in the root folder of    #
#        this software module.                                                     #
# *******************************************************************************  #

PKG_NAME:=ltq_qoscli

bins := qos_cli

qos_cli_sources := $(wildcard *.c)
qos_cli_cflags := -I. -I$(STAGING_DIR)/usr/include/
qos_cli_ldflags := -lqosipc -lqosfapi -lutils -lhelper -L$(STAGING_DIR)/usr/lib/ -lsafec-1.0

include make.inc
