#ifdef LINUX
#include <asm/types.h>
#include <sys/socket.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <linux/in6.h>
#include <linux/if_bridge.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <errno.h>
#include <net/if.h>
#endif

#define PON_NET_EXTERN_IMPL
#include "pon_net_extern.h"
