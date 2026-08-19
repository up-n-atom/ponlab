#CFLAGS += -I./
PKG_NAME:= upgrade_utils

bins := secupg read_img uboot_env upgrade

secupg_sources := sec_upgrade.c
secupg_ldflags := -lsafec-3.3 -lsecurefapi
upgrade_sources := upgrade.c sse_env.c
upgrade_ldflags := -lsafec-3.3
read_img_sources := sse_env.c read_img.c
read_img_ldflags := -lsafec-3.3
uboot_env_sources := uboot_env.c sse_env.c 
uboot_env_ldflags := -lsafec-3.3

include make.inc
