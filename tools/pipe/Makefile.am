## Process this file with automake to produce Makefile.in

bin_PROGRAMS = dsl_cpe_pipe

if ADD_APPL_CFLAGS_SET
dsl_cpe_pipe_cflags = $(ADD_APPL_CFLAGS)
else
dsl_cpe_pipe_cflags =
endif

dsl_cpe_pipe_compile_cflags = $(DSL_DRIVER_INCL_PATH) $(IFXOS_INCLUDE_PATH)

if SAFEC_SUPPORT
dsl_cpe_pipe_compile_cflags += @SAFEC_INCL_PATH@
endif

dsl_cpe_pipe_common_cflags = -Wall -Wimplicit -Wreturn-type -Wunused -Wundef \
	-Wswitch -Wcomment -Wuninitialized -DLINUX -g -s

dsl_cpe_pipe_SOURCES = dsl_pipe_client.c dsl_pipe_lib.c

dsl_cpe_pipe_CFLAGS = $(dsl_cpe_pipe_common_cflags) \
                      $(dsl_cpe_pipe_cflags) \
                      $(dsl_cpe_pipe_compile_cflags)

dsl_cpe_pipe_LDADD = -lifxos -lpthread -lm -lrt

EXTRA_DIST = dsl_cpe_safec_wrapper.h

if SAFEC_SUPPORT
dsl_cpe_pipe_LDADD += -l@SAFEC_LIB_NAME@
endif