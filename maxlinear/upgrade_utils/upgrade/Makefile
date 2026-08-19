##################################################################################
#                                                                                #
#       Copyright © 2020-2024 MaxLinear, Inc.                                    #
#                                                                                #
#  For licensing information, see the file 'LICENSE' in the root folder of       #
#  this software module.                                                         #
##################################################################################


#CFLAGS += -I./
PKG_NAME:= upgrade_utils

bins := upgrade read_img uboot_env

upgrade_sources := command.c flash.c upgrade_core.c upgrade_cmd.c upgrade.c crc32.c
upgrade_ldflags := -lsafec
read_img_sources := command.c flash.c upgrade_core.c upgrade_cmd.c read_img.c crc32.c
read_img_ldflags := -lsafec
uboot_env_sources := uboot_env.c command.c flash.c upgrade_core.c upgrade_cmd.c crc32.c
uboot_env_ldflags := -lsafec

include make.inc
