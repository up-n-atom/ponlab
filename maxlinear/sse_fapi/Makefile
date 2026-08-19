# ******************************************************************************** #
#       Copyright (C) 2020-2024 MaxLinear, Inc.                                    #
#       Copyright (C) 2019 Intel Corporation                                       #
#       Copyright (C) 2017-2018 Intel Corporation                                  #
#       Lantiq Beteiligungs-GmbH & Co. KG                                          #
#                                                                                  #
#       For licensing information, see the file 'LICENSE' in the root folder of    #
#       this software module.                                                      #
# ******************************************************************************** #

PKG_NAME := libsecurefapi

opt_no_flags := -Wcast-qual
CFLAGS := $(filter-out -DPACKAGE_ID=\"securefapi\" -DLOGGING_ID="securefapi",$(CFLAGS) -I./include)
CFLAGS += -DCONFIG_SYS_REDUNDAND_ENVIRONMENT=1

bins := libsecurefapi.so
ifeq ($(LGM_SECURE),1)
bins += sse_test_app sse_ctest_util secdbg_ctest secdbg_test_app
endif

ifeq ($(LGM_SECURE),1)
secdbg_ctest_cflags := -DSECDBG_CTEST -DSECDBG_TOOL
sse_test_app_cflags := -DSECDBG_TOOL -DUBOOT_OPS
secdbg_test_app_cflags := -DSECDBG_TOOL -DMXLTEE
endif

libsecurefapi.so_sources := fapi_sec_services.c fapi_img_auths.c sse_env.c
ifeq ($(LGM_SECURE),1)
libsecurefapi.so_sources += fapi_sec_storages.c fapi_sec_debugs.c
endif

ifeq ($(LGM_SECURE),1)
sse_test_app_sources := testApplication.c secdbg_test_app.c
sse_ctest_util_sources := test_main.c sec_storage_ctest.c
secdbg_ctest_sources := test_main.c secdbg_ctest_util.c secdbg_test_app.c
secdbg_test_app_sources := secdbg_test_app.c secdbg.c
endif

libsecurefapi.so_ldflags := -lsafec -lfdt -lscapi
ifeq ($(LGM_SECURE),1)
sse_test_app_ldflags := -L./ -lsecurefapi -lsafec
sse_ctest_util_ldflags := -L./ -lsecurefapi -lsafec
secdbg_ctest_ldflags := -L./ -lsecurefapi -lsafec
secdbg_test_app_ldflags := -L./ -lsecurefapi -lsafec -lteec -lsecurefapi -lcrypto -lssl
endif

include make.inc
