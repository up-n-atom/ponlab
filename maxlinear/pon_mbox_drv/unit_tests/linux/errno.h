#ifdef LINUX
#include_next <linux/errno.h>
#else
#include "../fake_linux.hpp"
#endif
