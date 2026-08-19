# ******************************************************************************** #
#       Copyright (C) 2018 Intel Corporation                                       #
#       Copyright (C) 2017-2018 Intel Corporation                                  #
#       Lantiq Beteiligungs-GmbH & Co. KG                                          #
#       Lilienthalstrasse 15, 85579 Neubiberg, Germany                             #
#       For licensing information, see the file 'LICENSE' in the root folder of    #
#        this software module.                                                     #
# *******************************************************************************  #

PKG_NAME := libsecurefapi

opt_no_flags := -Wcast-qual
CFLAGS := $(filter-out -DPACKAGE_ID=\"securefapi\" -DLOGGING_ID="securefapi",$(CFLAGS))

bins := libsecurefapi.so \
	sse_test_app

libsecurefapi.so_sources := fapi_sec_services.c
libsecurefapi.so_ldflags := -lsafec-3.3

sse_test_app_sources := testApplication.c

sse_test_app_ldflags := -L./ -lsecurefapi -lsafec-3.3

include make.inc
