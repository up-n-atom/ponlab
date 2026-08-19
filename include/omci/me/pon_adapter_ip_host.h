/******************************************************************************
 *
 *  Copyright (c) 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 ******************************************************************************/

/**
 * \file
 *
 * This is a PON Adapter OMCI Managed Entity header file for
 * "IP host config data", "IPv6 host config data" and
 * "IP host performance monitoring data".
 */

#ifndef _pon_adapter_me_ip_host
#define _pon_adapter_me_ip_host

#include "pon_adapter_base.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \addtogroup PON_ADAPTER_ME
 * @{
 */

/** \defgroup PON_ADAPTER_ME_IP_HOST IP Host
 *
 *  The IP host config data configures IP based services offered on the ONU.
 *  The ONU automatically creates instances of this Managed Entity if IP host
 *  services are available.
 *
 * @{
 */

/** IP Host address fields
 * This defines the default type for IPv4 address fields.
 */
struct pa_ip_host_address {
	/** Address values in network order */
	uint8_t value[4];
};

/** IPv6 Host address fields
 * This defines the default type for IPv6 address fields.
 */
struct pa_ipv6_host_address {
	/** Address values in network order */
	uint8_t value[16];
};

/** IP Host configuration data update structure */
struct pa_ip_host_update_data {
	/** IP Options
	 *  Bitmask for following options:
	 *  - Bit 0: Enable DHCP
	 *  - Bit 1: Respond to "Ping"
	 *  - Bit 2: Respond to traceroute messages
	 *  - Bit 3: Enable IP stack
	 */
	uint8_t ip_options;

	/** ONU Identifier
	 *  Null terminated string.
	 */
	char ont_id[26];

	/** IPv4 Address
	 *  This is the address used for IP host services, the attribute has
	 *  the default value 0x0000 0000 (not set). If this value is set, it
	 *  overrides any values returned in DHCP.
	 */
	struct pa_ip_host_address ip_address;

	/** IPv4 Address Mask
	 *  This is the subnet mask for IP host services, the attribute has the
	 *  default value 0x0000 0000 (not set). If this value is set, it
	 *  overrides any values returned in DHCP.
	 */
	struct pa_ip_host_address mask;

	/** Gateway IP Address
	 *  This is the default gateway address used for IP host services, this
	 *  attribute has the default value 0x0000 0000 (not set). If this value
	 *  is set, it overrides any values returned in DHCP.
	 */
	struct pa_ip_host_address gateway;

	/** Primary DNS Address
	 *  This is the address of the primary DNS server, the attribute has the
	 *  default value 0x0000 0000 (not set). If this value is set, it
	 *  overrides any values returned in DHCP.
	 */
	struct pa_ip_host_address primary_dns;

	/** Secondary DNS Address
	 *  This is the address of the secondary DNS server, the attribute has
	 *  the default value 0x0000 0000 (not set). If this value is set, it
	 *  overrides any values returned in DHCP.
	 */
	struct pa_ip_host_address secondary_dns;
};

/** IPv6 Host configuration data update structure */
struct pa_ipv6_host_update_data {
	/** IP options
	 *  Bitmask for following options:
	 *  - Bit 0: Unlock, IPv6 stack administrative unlock.
	 *  - Bit 1: Enable RS, The host generates router solicitation (RS)
	 *           messages, if necessary, and responds to router
	 *           advertisements. If the router advertisement (RA) message
	 *           has the M flag set to 1, the ONU is expected to request
	 *           address and other configuration information via DHCPv6.
	 *           If the RA message has the O flag set to 1 and M to 0, the
	 *           ONU is expected to only request additional configuration
	 *           information via DHCPv6, but not addresses.
	 *  - Bit 2: Enable DHCPv6,
	 *  - Bit 3: Respond to pings, ICMPv6 echo replies are enabled
	 */
	uint8_t ip_options;

	/** ONU Identifier
	 *  Null terminated string.
	 */
	char onu_id[26];

	/** IPv6 address */
	struct pa_ipv6_host_address ipv6_addr;

	/** IPv6 gateway (default router) */
	struct pa_ipv6_host_address default_router;

	/** Primary DNS Address */
	struct pa_ipv6_host_address primary_dns;

	/** Secondary DNS Address */
	struct pa_ipv6_host_address secondary_dns;

	/** IPv6 on link prefix */
	struct pa_ipv6_host_address on_link_prefix;

	/** IPv6 on link prefix length in bits */
	uint8_t on_link_prefix_len;
};

/** IP Host counters structure */
struct pa_ip_host_counters {
	/** ICMP errors */
	uint32_t icmp_errors;

	/** DNS errors */
	uint32_t dns_errors;

	/** DHCP time-outs */
	uint16_t dhcp_timeouts;

	/** IP address conflict */
	uint16_t ip_address_conflict;

	/** Out of memory */
	uint16_t out_of_memory;

	/** Internal error */
	uint16_t internal_error;
};

/** IP Host operations structure */
struct pa_ip_host_ops {
	/** Create IP Host configuration data Managed Entity.
	 *
	 * \remark Enable Managed Entity identifier mapping to interface index
	 * and initialize corresponding internal structures when it is called.
	 *
	 * \param[in]  ll_handle        Lower layer context pointer
	 * \param[in]  me_id            Managed Entity identifier
	 * \param[in]  ipv6             Indicated the IPv6 ME
	 */
	enum pon_adapter_errno (*create)(void *ll_handle,
					 uint16_t me_id,
					 bool ipv6);

	/** Update IP Host configuration data Managed Entity resources
	 *
	 * \param[in]  ll_handle        Lower layer context pointer
	 * \param[in]  me_id            Managed Entity identifier
	 * \param[in]  update_data      IP Host configuration data
	 */
	enum pon_adapter_errno (*update)(void *ll_handle,
					 uint16_t me_id,
					 const struct pa_ip_host_update_data
						*update_data);

	/** Update IPv6 Host configuration data Managed Entity resources
	 *
	 * \param[in]  ll_handle        Lower layer context pointer
	 * \param[in]  me_id            Managed Entity identifier
	 * \param[in]  update_data      IPv6 Host configuration data
	 */
	enum pon_adapter_errno (*update_v6)(void *ll_handle,
					    uint16_t me_id,
					    const struct
						pa_ipv6_host_update_data
							*update_data);

	/** Destroy IP Host configuration data Managed Entity resources
	 *
	 * \param[in]  ll_handle        Lower layer context pointer
	 * \param[in]  me_id            Managed Entity identifier
	 */
	enum pon_adapter_errno (*destroy)(void *ll_handle, uint16_t me_id);

	/** Get current address
	 *
	 * \param[in]  ll_handle        Lower layer context pointer
	 * \param[in]  me_id            Managed Entity identifier
	 * \param[out] address          Returned ip address
	 */
	enum pon_adapter_errno (*current_address_get)(void *ll_handle,
						      uint16_t me_id,
						      struct pa_ip_host_address
								*address);

	/** Get current mask
	 *
	 * \param[in]  ll_handle        Lower layer context pointer
	 * \param[in]  me_id            Managed Entity identifier
	 * \param[out] mask             Returned mask
	 */
	enum pon_adapter_errno (*current_mask_get)(void *ll_handle,
						   uint16_t me_id,
						   struct pa_ip_host_address
								*mask);

	/** Get current gateway
	 *
	 * \param[in]  ll_handle        Lower layer context pointer
	 * \param[in]  me_id            Managed Entity identifier
	 * \param[out] gateway          Returned gateway
	 */
	enum pon_adapter_errno (*current_gateway_get)(void *ll_handle,
						      uint16_t me_id,
						      struct pa_ip_host_address
								*gateway);

	/** Get current DNS (primary/secondary)
	 *
	 * \param[in]  ll_handle        Lower layer context pointer
	 * \param[in]  me_id            Managed Entity identifier
	 * \param[in]  secondary        Select primary (false) or
	 *                              secondary (true)
	 * \param[out] dns              Returned DNS
	 */
	enum pon_adapter_errno (*current_dns_get)(void *ll_handle,
						  uint16_t me_id,
						  bool secondary,
						  struct pa_ip_host_address
								*dns);

	/** Get IPv6 link local address
	 *
	 * \param[in]  ll_handle        Lower layer context pointer
	 * \param[in]  me_id            Managed Entity identifier
	 * \param[out] ipv6             Returned link local address
	 */
	enum pon_adapter_errno (*link_local_addr_get)(void *ll_handle,
						      uint16_t me_id,
						      struct
							pa_ipv6_host_address
								*ipv6);

	/** Get domain name
	 *
	 * \param[in]  ll_handle        Lower layer context pointer
	 * \param[in]  me_id            Managed Entity identifier
	 * \param[in]  size             Size of buffer for domain
	 * \param[out] domain           Returned domain
	 */
	enum pon_adapter_errno (*domain_name_get)(void *ll_handle,
						  uint16_t me_id,
						  uint8_t size,
						  char *domain);

	/** Get host name
	 *
	 * \param[in]  ll_handle        Lower layer context pointer
	 * \param[in]  me_id            Managed Entity identifier
	 * \param[in]  size             Size of buffer for host
	 * \param[out] host             Returned host
	 */
	enum pon_adapter_errno (*host_name_get)(void *ll_handle,
						uint16_t me_id,
						uint8_t size,
						char *host);

	/** Get MAC Address
	 *
	 * \param[in]  ll_handle        Lower layer context pointer
	 * \param[in]  me_id            Managed Entity identifier
	 * \param[out] mac_address      Returned mac_address
	 */
	enum pon_adapter_errno (*mac_address_get)(void *ll_handle,
						  uint16_t me_id,
						  uint8_t mac_address[6]);

	/** Get counters for monitoring data
	 *
	 * \param[in]  ll_handle        Lower layer context pointer
	 * \param[in]  me_id            Managed Entity identifier
	 * \param[out] counters         Returned counters
	 */
	enum pon_adapter_errno (*counters_get)(void *ll_handle,
					       uint16_t me_id,
					       struct pa_ip_host_counters
							*counters);
};

/** @} */

/** @} */

#ifdef __cplusplus
}
#endif

#endif
