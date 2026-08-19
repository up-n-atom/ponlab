#
# Secure Boot Image Authentication usign EIP-123 DDK
# @Author : William Widjaja <w.widjaja.ee@lantiq.com>
# Date : 22-Dec-2014
#

CROSS_COMPILE ?= mips-openwrt-linux-
DDK_PATH:=../lib_inc

OUT  := tss
OBJS := main.o

CC      := $(CROSS_COMPILE)gcc
#CFLAGS  += -g3 -O0 -Wall -DDBG_DUMP
CFLAGS  += -g -O2 -Wall
CFLAGS  += -I$(DDK_PATH)/usr/include

#LDFLAGS :=
LDFLAGS := -L$(DDK_PATH)/usr/lib -lcal -lrt -lpthread

##################################################
# OBFUSCATION
##################################################
CFLAGS  += -DCONFIG_OBFUSCATION

##################################################
# ENCRYPT ATTRIBUTE
##################################################
CFLAGS  += -DCONFIG_ENCRYPT_ATTRIBUTE

##################################################
# ECDSA SW
##################################################

ECDSA_SW := YES

ifeq "$(ECDSA_SW)" "YES"
	CFLAGS += -DECDSA_SW
endif

##################################################
# ICC IMAGE
##################################################

ICC_IMAGE := YES

ifeq "$(ICC_IMAGE)" "YES"
	CFLAGS += -DICC_IMAGE
	CFLAGS += -I$(CURDIR)/icc
	VPATH     = icc
	OBJS     += icc.o
endif

##################################################
# rules

all:$(OUT)

$(OUT): $(OBJS)
	$(CC) $^ $(LDFLAGS) -o $@

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY:clean
clean :
	rm -rf $(OBJS) $(OUT)
