##################################################################################
#                                                                                #
#       Copyright © 2020-2024 MaxLinear, Inc.                                    #
#                                                                                #
#  For licensing information, see the file 'LICENSE' in the root folder of       #
#  this software module.                                                         #
##################################################################################

ccflags-y := -Werror

ifeq ($(CONFIG_PPA_MPE_HAL),m)
obj-m	+= ltq_mpe_hal_drv.o
ltq_mpe_hal_drv-objs	+= ltq_mpe_hal_proc.o ltq_mpe_tmpl_buf.o ltq_mpe_hal.o \
		mpe_hal_qos.o \
		ltq_itc.o
endif
