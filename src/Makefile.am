



#
#                         Copyright (c) 2024 MaxLinear, Inc.
#                              Copyright (c) 2009
#                            Lantiq Deutschland GmbH
#                     Am Campeon 3; 85579 Neubiberg, Germany
#
# THE DELIVERY OF THIS SOFTWARE AS WELL AS THE HEREBY GRANTED NON-EXCLUSIVE,
# WORLDWIDE LICENSE TO USE, COPY, MODIFY, DISTRIBUTE AND SUBLICENSE THIS
# SOFTWARE IS FREE OF CHARGE.
#
# THE LICENSED SOFTWARE IS PROVIDED "AS IS" AND INFINEON EXPRESSLY DISCLAIMS
# ALL REPRESENTATIONS AND WARRANTIES, WHETHER EXPRESS OR IMPLIED, INCLUDING
# WITHOUT LIMITATION, WARRANTIES OR REPRESENTATIONS OF WORKMANSHIP,
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, DURABILITY, THAT THE
# OPERATING OF THE LICENSED SOFTWARE WILL BE ERROR FREE OR FREE OF ANY THIRD
# PARTY CLAIMS, INCLUDING WITHOUT LIMITATION CLAIMS OF THIRD PARTY INTELLECTUAL
# PROPERTY INFRINGEMENT.
#
# EXCEPT FOR ANY LIABILITY DUE TO WILFUL ACTS OR GROSS NEGLIGENCE AND EXCEPT
# FOR ANY PERSONAL INJURY INFINEON SHALL IN NO EVENT BE LIABLE FOR ANY CLAIM
# OR DAMAGES OF ANY KIND, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
# ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
# DEALINGS IN THE SOFTWARE.
#

#AUTOMAKE_OPTIONS = subdir-objects

bin_PROGRAMS = pm_util pmchip

nodist_pm_util_SOURCES =

## pm_util program
libpmutil_sources= \
    cpufreq.c \
    sysfs.c \
    sysfsmon.c \
    sysfsreg.c \
    sysfsdbg.c \
    sysfstzone.c \
    sysfscdevice.c \
    sysfsadp.c \
    logger.c \
    sysfscpuidle.c \
    i2cpmic.c

pm_util_SOURCES = \
    pm_util.c \
    $(libpmutil_sources)

pmchip_SOURCES = \
    pm_chip.c

pm_util_LDADD = -lsafec
pmchip_LDADD = -lsafec

EXTRA_DIST = \
    include/ltq_pmutil_interface.h

lib_LIBRARIES = libpmutil.a
libpmutil_a_SOURCES = $(libpmutil_sources)

AM_CFLAGS = -ggdb -Wall -Wextra -DLINUX \
   -I \
   -Iinclude

clean-generic:
	@echo "Cleanup Linux kernel object build"
	@- find . -name ".*.cmd" | xargs rm -f
	@- find . -name "*.o" | xargs rm -f
	@- rm -f Module.symvers Kbuild
	@- rm -rf .tmp_versions *.mod.c *.order

install-exec-hook: $(bin_PROGRAMS)
	@for p in $(bin_PROGRAMS); do \
		$(STRIP) $(DESTDIR)$(bindir)/$$p; \
		echo "  $(STRIP) $(DESTDIR)$(bindir)/$$p"; \
	done


