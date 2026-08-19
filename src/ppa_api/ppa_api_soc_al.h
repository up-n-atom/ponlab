/*******************************************************************************

  Copyright © 2020-2022 MaxLinear, Inc.

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

*******************************************************************************/

#ifndef PPA_API_SOC_AL_H
#define PPA_API_SOC_AL_H

static inline uint64_t mib_wraparound(uint64_t old, uint64_t now,
				      size_t wbits)
{
	if (now >= old)
		return (now - old);
	else
		return ((1UL << wbits) + now - old);
}

int ppa_update_base_inf_mib(PPA_ITF_MIB_INFO *mib_tmp, uint32_t hal_id);
#if KERNEL_VERSION(4, 10, 17) < LINUX_VERSION_CODE
void
ppa_netif_get_stats64(struct net_device *dev,
		      struct rtnl_link_stats64 *storage);
#else
struct rtnl_link_stats64*
ppa_netif_get_stats64(struct net_device *dev,
		      struct rtnl_link_stats64 *storage);
#endif
bool is_pmac_rxcsum_enabled(int32_t port);

#endif /* PPA_API_SOC_AL_H */
