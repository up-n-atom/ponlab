# Process this file with automake to produce Makefile.in
bin_PROGRAMS = dsl_fapid
bin_SCRIPTS = @top_srcdir@/tools/dsl_fapi_pipe.sh

if DSL_FAPI_DAEMON_CLI
dsl_fapid_SOURCES = \
   @top_srcdir@/tools/dsl_fapid_cli.c \
   @top_srcdir@/cli/dsl_fapi_cli.c \
   @top_srcdir@/cli/dsl_fapi_cli_misc.c \
   @top_srcdir@/cli/dsl_fapi_cli_autogen.c \
   @top_srcdir@/cli/dsl_fapi_cli_ext_misc.c
else
dsl_fapid_SOURCES = \
   @top_srcdir@/tools/dsl_fapid.c
endif

EXTRA_DIST = \
   @top_srcdir@/tools/dsl_fapid.h \
   @top_srcdir@/cli/dsl_fapi_cli.h \
   @top_srcdir@/tools/dsl_fapi_pipe.sh

if DSL_FAPI_DAEMON_CLI
EXTRA_DIST += \
   @top_srcdir@/cli/dsl_fapi_cli_ext_autogen.c
endif

if INCLUDE_CLI_SUPPORT
cli_library=-lcli
else
cli_library=
endif

AM_CFLAGS = -D_GNU_SOURCE \
            -I@top_srcdir@/include/ \
            -I@top_srcdir@/cli/ \
            @IFXOS_INCLUDE_PATH@ \
            @FAPI_INCL_PATH@ \
            @LIB_CLI_INCLUDE_PATH@

AM_CFLAGS += -DLINUX -D__LINUX__

AM_LDFLAGS = -L@builddir@ \
             -L@top_srcdir@/src/ \
             @IFXOS_LIBRARY_PATH@ \
             @FAPI_LIBRARY_PATH@ \
             @LIB_CLI_LIBRARY_PATH@

dsl_fapid_CFLAGS = $(AM_CFLAGS)
dsl_fapid_LDFLAGS = $(AM_LDFLAGS)
dsl_fapid_LDADD = \
    $(cli_library)\
    -lifxos -lpthread -lrt -lm -ldslfapi

