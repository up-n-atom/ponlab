#ifdef LINUX
#include_next <linux/wait.h>
#else
#include "../fake_linux.hpp"
#endif
