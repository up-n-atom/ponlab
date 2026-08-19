#/******************************************************************************
#
#         Copyright (c) 2020, MaxLinear, Inc.
#         Copyright 2016 - 2020 Intel Corporation
#         Copyright 2015 - 2016 Lantiq Beteiligungs-GmbH & Co. KG
#         Copyright 2009 - 2014 Lantiq Deutschland GmbH
#         Copyright 2007 - 2008 Infineon Technologies AG
#
#  For licensing information, see the file 'LICENSE' in the root folder of
#  this software module.
#
#*******************************************************************************/

export abs_top = $(shell pwd)
export abs_top_srcdir = $(abs_top)
export abs_top_builddir = $(abs_top_srcdir)
export abs_logprep_srcdir = $(abs_top_srcdir)/wireless/shared_iwlwav-dev
export STAGING_DIR = 

# Log macros header file that is included in all source files
logm_hdr_file := $(abs_top)/logmacros.h

CFLAGS += -Wall -Werror -Wl,-strip-debug
CFLAGS += `pkg-config --cflags libnl-3.0` -Wno-unused-result -Wno-empty-body
LDFLAGS += `pkg-config libnl-3.0 libnl-genl-3.0 --libs` -lpthread

ifeq ($(ONEWIFI_BUILD),true)
all: logm_headers dutserver dump_handler logserver drvhlpr BclSockServer rcvry_monitor
release: logm_headers dutserver dump_handler drvhlpr rcvry_monitor
else
all: logm_headers dutserver dump_handler logserver drvhlpr BclSockServer whm_handler rcvry_monitor rx_measure_handler
release: logm_headers dutserver dump_handler drvhlpr whm_handler rcvry_monitor rx_measure_handler
endif

debug: all
osp: release
osp-debug: all

clean:
	$(MAKE) -C tools/shared/linux/               clean
	$(MAKE) -C wireless/                         clean
	$(MAKE) -C tools/shared/3rd_party/iniparser/ clean
	$(MAKE) -C tools/rtlogger/logserver/linux/   clean
	$(MAKE) -C tools/dutserver/linux/            clean
	$(MAKE) -C tools/drvhlpr/                    clean
	$(MAKE) -C tools/dump_handler/               clean
	$(MAKE) -C tools/whm_handler/                clean
	$(MAKE) -C tools/rcvry_monitor/              clean
	$(MAKE) -C tools/BCLSockServer/              clean
	$(MAKE) -C tools/rx_measure_handler/         clean
	find $(abs_top) -name "logmacros.*"   | xargs rm -f
	find $(abs_top) -name "Makefile.am"   | xargs rm -f
	find $(abs_top) -name "Makefile.in"   | xargs rm -f
	find $(abs_top) -name "*.scd"         | xargs rm -f
	find $(abs_top) -name "*.o.cmd"       | xargs rm -f
	find $(abs_top) -name "*.stderr"      | xargs rm -f
	find $(abs_top) -name "*.pipe.result" | xargs rm -f
	find $(abs_top) -name ".logprep_ts"   | xargs rm -rf

# Create an empty log macros header file if it does not already exist.
# This file will be automatically updated by the log preprocessor.
logm_headers:
	test -e $(logm_hdr_file) || touch -m -t 198001010000.00 $(logm_hdr_file)


libmtlkc:
	$(MAKE) -C tools/shared/linux/

libmtlkwls:
	$(MAKE) -C wireless/

libiniparser:
	$(MAKE) -C tools/shared/3rd_party/iniparser/

dutserver: libmtlkc libmtlkwls libiniparser
	$(MAKE) -C tools/dutserver/linux/

logserver: libmtlkc libmtlkwls
	$(MAKE) -C tools/rtlogger/logserver/linux/

drvhlpr: libmtlkc libmtlkwls libiniparser
	$(MAKE) -C tools/drvhlpr/

dump_handler: libmtlkc libmtlkwls
	$(MAKE) -C tools/dump_handler/

whm_handler: libmtlkc libmtlkwls
	$(MAKE) -C tools/whm_handler/

rcvry_monitor: libmtlkc libmtlkwls
	$(MAKE) -C tools/rcvry_monitor/

BclSockServer: libmtlkc
	$(MAKE) -C tools/BCLSockServer

rx_measure_handler:
	$(MAKE) -C tools/rx_measure_handler/
