#
# Makefile for GPY211 PHY
#

CC = $(TARGET_CC)

EN_API_FLAG = -DEN_MSEC_DEPRECATED_API=0 \
	      -DSUPPORT_MACSEC_VLAN=0 \
	      -DEN_GMAC_DEBUG_ACCESS=1 \
	      -DEN_MSEC_DEBUG_ACCESS=1 \
	      -DEN_REPROGM_MISSING_4B=0 \
	      -DTEST_ALL_ZERO_IMAGE=0 \
	      -DEN_EXEC_TIME_PER_MDIO_FREQ=0 \
	      -DEN_ERASE_FLASH_ONLY_TEST=0 \
	      -DEN_USER_CHOICE=0 \
	      -DEN_TEST_REG_READ_WRITE=0 \
	      -DEN_ERASE_FLASH_TO_OTP_BOOT=0

INCLUDES = -I./src/os/linux \
	   -I./src/inc \
	   -I./ISC-DDK-160-BSD_v2.3/SafeXcel-IP-160_DDK_v2.3/Kit/SABuilder_MACsec/Example_AES/incl

LFLAGS = -L.

CFLAGS = ${EN_API_FLAG} \
	 -DP31G_OPT_SPEED=0 \
	 -DEN_SMDIO_RW=1 \
	 -DP31G_B0=1 \
	 -DSAFE_C_LIB=0 \
	 -DREG_RD_PRINT=0 \
	 -DREG_WR_PRINT=0 \
	 -DEN_BACKWARD_COMP_API=0

CFLAGS += -Wno-format -Wall -fstack-protector-all

SRCS = src/api/phy/gpy211_phy.c \
       src/api/phy/gpy211_chip.o \
       src/api/macsec/gpy211_macsec.o \
       ISC-DDK-160-BSD_v2.3/SafeXcel-IP-160_DDK_v2.3/Kit/SABuilder_MACsec/Example_AES/src/aes.o \
       utility/fmx/gpy2xx_fw_dl.o

OBJS = $(SRCS:.c=.o)

MAIN = gphy_fw_update


.PHONY: depend clean

all:    $(MAIN)
	@echo  Build ${MAIN}...

$(MAIN): $(OBJS) 
	$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

clean:
	$(RM) $(OBJS) $(MAIN)

depend: $(SRCS)
	makedepend $(INCLUDES) $^

