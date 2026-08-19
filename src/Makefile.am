
# Define the library name
lib_LTLIBRARIES = libpatemplate.la

# Source files for the library
libpatemplate_la_SOURCES = pon_pa_template.c pon_pa_template.h

# Compiler and linker flags
libpatemplate_la_CFLAGS = \
	@PON_ADAPTER_INCLUDE_PATH@ \
	-Wall -g

libpatemplate_la_LDFLAGS = \
	@PON_ADAPTER_LIBRARY_PATH@ \
	-Wl,--no-undefined \
	-version-info 0:0:0

# Include headers
include_HEADERS = pon_pa_template.h
