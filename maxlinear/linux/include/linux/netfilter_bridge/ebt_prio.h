/*
 * ebtable priority target can be used in any chain
 */

#ifndef __LINUX_BRIDGE_EBT_PRIO_H
#define __LINUX_BRIDGE_EBT_PRIO_H

struct ebt_prio_info {
	int bitmask;
	int default_prio;
	/* EBT_ACCEPT, EBT_DROP, EBT_CONTINUE or EBT_RETURN */
	int target;
};

#endif /* ! __LINUX_BRIDGE_EBT_PRIO_H */
