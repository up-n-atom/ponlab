##################################################################################
#                                                                                #
#       Copyright © 2020-2024 MaxLinear, Inc.                                    #
#                                                                                #
#  For licensing information, see the file 'LICENSE' in the root folder of       #
#  this software module.                                                         #
##################################################################################

#CFLAGS += -I./
CFLAGS += -DCONFIG_SYS_REDUNDAND_ENVIRONMENT=1
PKG_NAME:= upgrade_utils

bins := secupg read_img uboot_env upgrade

secupg_sources := sec_upgrade.c
secupg_ldflags := -lsafec -lsecurefapi -lscapi
upgrade_sources := upgrade.c sse_env.c
upgrade_ldflags := -lsafec
read_img_sources := sse_env.c read_img.c
read_img_ldflags := -lsafec
uboot_env_sources := uboot_env.c sse_env.c 
uboot_env_ldflags := -lsafec

include make.inc
