# SPDX-License-Identifier: GPL-2.0
#
# MXL-TEE Trusted Execution Environment Configuration

obj-$(CONFIG_MXLTEE) += mxltee.o

mxltee-objs += core.o
mxltee-objs += cmds.o
mxltee-objs += helpers.o
mxltee-objs += shm_pool.o
mxltee-objs += keygenload.o
mxltee-objs += signing.o
mxltee-objs += wrap_unwrap.o
mxltee-objs += macgen.o

ccflags-y := -I $(srctree)/drivers/tee/mxltee/
